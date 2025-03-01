#include "CAFAna/Core/SpectrumLoader.h"

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/ReweightableSpectrum.h"

#ifdef WITH_SAM
#include "CAFAna/Core/SAMProjectSource.h"
#endif

#include "CAFAna/Core/SignalHandlers.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Utilities.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include <cassert>
#include <cmath>
#include <iostream>

#include "TFile.h"
#include "TH2.h"
#include "TTree.h"

namespace ana
{
  //----------------------------------------------------------------------
  SpectrumLoader::SpectrumLoader(const std::string& wildcard, const std::string& pathInFile)//, int max)
    : SpectrumLoaderBase(wildcard, pathInFile)//, max_entries(max)
  {
  }

  //----------------------------------------------------------------------
  SpectrumLoader::SpectrumLoader(const std::vector<std::string>& fnames, const std::string& pathInFile)//, int max)
    : SpectrumLoaderBase(fnames, pathInFile)//, max_entries(max)
  {
  }

  //----------------------------------------------------------------------
  SpectrumLoader::SpectrumLoader()
  {
  }

#ifdef WITH_SAM
  //----------------------------------------------------------------------
  SpectrumLoader* SpectrumLoader::FromSAMProject(const std::string& proj,
                                                int fileLimit)
  {
    SpectrumLoader* ret = new SpectrumLoader;
    ret->fWildcard = "project "+proj;
    ret->fFileSource = std::unique_ptr<IFileSource>(new SAMProjectSource(proj, fileLimit));
    return ret;
  }
#endif

  //----------------------------------------------------------------------
  void SpectrumLoader::Go()
  {
    if(fGone){
      std::cerr << "Error: can only call Go() once on a SpectrumLoader" << std::endl;
      abort();
    }
    fGone = true;

    const int Nfiles = NFiles();

    Progress* prog = 0;

    caf::SRBranchRegistry::clear();

    int fileIdx = -1;
    while(TFile* f = GetNextFile()){
      ++fileIdx;

   // need to find ISpillSink
   if(Nfiles >= 0 && !prog){
        unsigned int totsinks = 0;
        for(const ISRSink* s: fSinks) totsinks += s->NSinks();

        prog = new Progress(TString::Format("Filling %u spectra from %d files matching '%s'", totsinks, Nfiles, fWildcard.c_str()).Data());
      }


      HandleFile(f, Nfiles == 1 ? prog : 0);

      if(Nfiles > 1 && prog) prog->SetProgress((fileIdx+1.)/Nfiles);

      if(CAFAnaQuitRequested()) break;
    } // end for fileIdx

    StoreExposures();

    if(prog){
      prog->Done();
      delete prog;
    }

    for(ISRSink* sink: fSinks) sink->RemoveSource(this);
    fSinks.clear();
  }

  //----------------------------------------------------------------------
  void SpectrumLoader::PrintGraph(std::ostream& os) const
  {
    os << "digraph{" << std::endl;
    os << "comment = \"Render me with a command like: dot -Tpdf graph.dot > graph.pdf\"" << std::endl << std::endl;
    Passthrough<caf::SRProxy>::PrintGraph(os);
    os << "}";
  }

  //----------------------------------------------------------------------
  void SpectrumLoader::PrintGraph() const
  {
    PrintGraph(std::cout);
  }

  //----------------------------------------------------------------------
  void SpectrumLoader::HandleFile(TFile* f, Progress* prog)
  {
    assert(!f->IsZombie());

    // In files with both "caf" and "cafTree", "cafTree" is the correct
    // version. "caf" is ROOT's temporary save while the file is being produced
    // and may be incomplete.
    auto tr = GetDirectoryFile(f)->Get<TTree>("cafTree");
    assert(tr);

    caf::SRProxy sr(tr, "rec");

    FloatingExceptionOnNaN fpnan(false);

    long Nentries = tr->GetEntries();
    //if(max_entries != 0 && max_entries < Nentries)
    //  Nentries = max_entries;

    for(long n = 0; n < Nentries; ++n){
      tr->LoadTree(n);

      // SBN here has fPOT and fNReadouts accounting... 
      // dont know what to do about that
      // at the moment, I left fPOT to be handled in SpectrumLoaderBase same as fPOTFromHist

      // todo: we'll resurrect this if we have to...
      //FixupRecord(&sr, tr);

      HandleRecord(&sr, 1);

      if(prog) prog->SetProgress(double(n)/Nentries);
    } // end for n
  }

  //----------------------------------------------------------------------
  void SpectrumLoader::StoreExposures()
  {
    if(fabs(fPOT - fPOTFromHist)/std::min(fPOT, fPOTFromHist) > 0.001){
      std::cout << fPOT << " POT from hdr differs from " << fPOTFromHist << " POT from the TotalPOT histogram!" << std::endl;
      abort();
    }

    std::cout << fPOT << " POT over " << fNReadouts << " readouts" << std::endl;

    FillPOT(fPOT);
    FillLivetime(fNReadouts);
  }

} // namespace ana
