#include "CAFAna/Core/SpectrumLoaderBase.h"

#include <variant>

#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/ReweightableSpectrum.h"
#ifdef WITH_SAM
#include "CAFAna/Core/SAMQuerySource.h"
#include "CAFAna/Core/SAMProjectSource.h"
#endif
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Core/WildcardSource.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#ifdef WITH_SAM
#include "ifdh.h"
#endif

#include "TFile.h"
#include "TH1.h"
#include "TTree.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include "boost/algorithm/string.hpp"

namespace ana
{

  //----------------------------------------------------------------------
  // adding fPOTFromHist and fNReadouts
  SpectrumLoaderBase::SpectrumLoaderBase()
    : fGone(false), fPOT(0),  fPOTFromHist(0), fNReadouts(0)
  {
  }

  //----------------------------------------------------------------------
  SpectrumLoaderBase::SpectrumLoaderBase(const std::string& wildcard, const std::string& pathInFile)
    : SpectrumLoaderBase()
  {
    fWildcard = wildcard;
    fPathInFile = pathInFile;
    fFileSource = std::unique_ptr<IFileSource>(WildcardOrSAMQuery(wildcard));
  }

  //----------------------------------------------------------------------
  SpectrumLoaderBase::SpectrumLoaderBase(const std::vector<std::string>& fnames, const std::string& pathInFile)
    : SpectrumLoaderBase()
  {
    fWildcard = "file list";
    fPathInFile = pathInFile;
    fFileSource = std::unique_ptr<IFileSource>(new FileListSource(fnames));

    // Apparently MakePredInterps runs over empty file lists?
    //    assert(!fnames.empty());
    std::cout << "Loading from " << fnames.size() << " files" << std::endl;
  }

  //----------------------------------------------------------------------
  SpectrumLoaderBase::~SpectrumLoaderBase()
  {}

  //----------------------------------------------------------------------
  IFileSource* SpectrumLoaderBase::
  WildcardOrSAMQuery(const std::string& str) const
  {
    int stride = -1;
    int offset = -1;
    if(getenv("CAFANA_STRIDE")){
      stride = atoi(getenv("CAFANA_STRIDE"));
      if(stride > 1 && getenv("CAFANA_OFFSET")){
        offset = atoi(getenv("CAFANA_OFFSET"));
      }
    }

    // stat() blows up on strings with spaces
    if(str.find(' ') == std::string::npos){
      WildcardSource* ret = new WildcardSource(str, stride, offset);
      if(ret->NFiles() > 0) return ret;
      else {
	std::cout << "Warning: " << str << " does not exist!" << std::endl;
	abort();
      }
      delete ret;
    }

#ifdef WITH_SAM
    // Maybe this the name of a SAM project?
    ifdh_ns::ifdh i;
    const std::string info = i.dumpProject(i.findProject(str, "nova"));
    // findProject always gives back an address just by gluing bits
    // together. But dumpProject will give an empty result for a nonexistent
    // project.
    if(!info.empty()){
      if(stride > 1)
        std::cout << "Warning: --stride has no effect on SAM projects"
                  << std::endl;

      return new SAMProjectSource(str);
    }

    // Maybe this is a SAM dataset or query?
    return new SAMQuerySource(str, stride, offset);
#else
    std::cout << "[ERROR]: Built without SAM support but found apparent SAM query in WildcardOrSAMQuery." << std::endl;
    abort();
#endif
  }
  //----------------------------------------------------------------------
  int SpectrumLoaderBase::NFiles() const
  {
    return fFileSource->NFiles();
  }

  //----------------------------------------------------------------------
  TFile* SpectrumLoaderBase::GetNextFile()
  {
    TFile* f = fFileSource->GetNextFile();
    if(!f) return 0; // out of files

    if(f->IsZombie()){
      std::cout << "Bad file (zombie): " << f->GetName() << std::endl;
      abort();
    }

    TDirectoryFile * df = GetDirectoryFile(f);

    // CAF-making is in flux.
    // In principle, beam.pulsepot in the cafTree should have per-entry POT,
    // but that's not currently always filled correctly.
    // Try that first, but fall back on meta tree if not...
    std::variant<float, double> pot;  // sometimes we have double, sometimes float <facepalm>
    double filePOT = 0;
    for (const auto & treeVarPair : std::vector<std::pair<std::string, std::string>>{{"cafTree", "rec.beam.pulsepot"}, {"meta", "pot"}})
    {
      bool perReadout = treeVarPair.first == "cafTree";

      TTree* trPot = df->Get<TTree>(treeVarPair.first.c_str());

      TClass * klass;
      EDataType expType;
      trPot->GetBranch(treeVarPair.second.c_str())->GetExpectedType(klass, expType);
      if (expType == kFloat_t)
      {
        pot = 0.f;
        // return value is 0 when match was achieved
        if (trPot->SetBranchAddress(treeVarPair.second.c_str(), &std::get<float>(pot)))
          continue;
      }
      else if (expType == kDouble_t)
      {
        pot = 0.;
        // return value is 0 when match was achieved
        if (trPot->SetBranchAddress(treeVarPair.second.c_str(), &std::get<double>(pot)))
          continue;
      }

      for(int n = 0; n < trPot->GetEntries(); ++n)
      {
        trPot->GetEntry(n);
        filePOT += (expType == kFloat_t) ? std::get<float>(pot) : std::get<double>(pot);

        // don't mindlessly add up tons of 0s
        if (n > 100 && filePOT <= 0)
          break;

        if (perReadout)
          fNReadouts++;
      }

      if (filePOT > 0)
        break;
    }

    // did we successfully count POT at all?
    if (filePOT > 0)
    {
      fPOTFromHist += filePOT;
      fPOT += filePOT;
    }
    else
    {
      std::cerr << "Couldn't find the appropriate branch in your CAF file for POT accounting.\n"
                << "(Are your TTrees in a subfolder in the file?  Pass that as an argument to the SpectrumLoader constructor.)\n"
                << "Can't proceed.  Abort.\n";
      abort();
    }

    return f;
  }

  //----------------------------------------------------------------------
  TDirectoryFile *SpectrumLoaderBase::GetDirectoryFile(TFile *f) const
  {
    if (fPathInFile.empty())
      return f;

    return f->Get<TDirectoryFile>(fPathInFile.c_str());
  }

//  //----------------------------------------------------------------------
//  void NullLoader::Go()
//  {
//  }
//
//  //----------------------------------------------------------------------
//  NullLoader::~NullLoader()
//  {
//  }
//
//  // Apparently the existence of fHistDefs isn't enough and I need to spell
//  // this out to make sure the function bodies are generated.
//  template struct SpectrumLoaderBase::IDMap<SystShifts,
//    SpectrumLoaderBase::IDMap<Cut,
//      SpectrumLoaderBase::IDMap<Weight,
//        SpectrumLoaderBase::IDMap<SpectrumLoaderBase::VarOrMultiVar,
//          SpectrumLoaderBase::SpectList>>>>;

} // namespace
