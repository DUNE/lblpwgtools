#include "CAFAna/Core/SpectrumLoader.h"

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/ReweightableSpectrum.h"
#ifndef DONT_USE_SAM
#include "CAFAna/Core/SAMProjectSource.h"
#endif
#include "CAFAna/Core/SignalHandlers.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Systs/XSecSystList.h"
#include "CAFAna/Core/FixupRecord.h"

#include "StandardRecord/Proxy/SRProxy.h"

#include <cassert>
#include <cmath>
#include <iostream>

#include "TFile.h"
#include "TH2.h"
#include "TTree.h"

namespace ana
{
  //----------------------------------------------------------------------
  SpectrumLoader::SpectrumLoader(const std::string& wildcard, int max)
    : SpectrumLoaderBase(wildcard), max_entries(max)
  {
  }

  //----------------------------------------------------------------------
  SpectrumLoader::SpectrumLoader(const std::vector<std::string>& fnames, int max)
    : SpectrumLoaderBase(fnames), max_entries(max)
  {
  }

  //----------------------------------------------------------------------
  SpectrumLoader::SpectrumLoader()
  {
  }

  #ifndef DONT_USE_SAM
  //----------------------------------------------------------------------
  SpectrumLoader SpectrumLoader::FromSAMProject(const std::string& proj,
                                                int fileLimit)
  {
    SpectrumLoader ret;
    ret.fWildcard = "project " + proj;
    ret.fFileSource = std::unique_ptr<IFileSource>(new SAMProjectSource(proj, fileLimit));
    return ret;
  }
  #endif

  //----------------------------------------------------------------------
  SpectrumLoader::~SpectrumLoader()
  {
  }

  struct CompareByID
  {
    bool operator()(const Cut& a, const Cut& b) const
    {
      return a.ID() < b.ID();
    }
  };

  //----------------------------------------------------------------------
  void SpectrumLoader::Go()
  {
    if(fGone){
      std::cerr << "Error: can only call Go() once on a SpectrumLoader" << std::endl;
      abort();
    }
    fGone = true;

    // Find all the unique cuts
    std::set<Cut, CompareByID> cuts;
    for(auto& shiftdef: fHistDefs)
      for(auto& cutdef: shiftdef.second)
        cuts.insert(cutdef.first);
    for(const Cut& cut: cuts) fAllCuts.push_back(cut);

    fLivetimeByCut.resize(fAllCuts.size());
    fPOTByCut.resize(fAllCuts.size());

    const int Nfiles = NFiles();

    Progress* prog = 0;

    caf::SRBranchRegistry::clear();

    int fileIdx = -1;
    while(TFile* f = GetNextFile()){
      ++fileIdx;

      if(Nfiles >= 0 && !prog){
        std::string sum = TString::Format("Filling %lu spectra", fHistDefs.TotalSize()).Data();
        sum += TString::Format(" from %d files matching '%s'", Nfiles, fWildcard.c_str()).Data();
        prog = new Progress(sum);
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

    ReportExposures();

    fHistDefs.RemoveLoader(this);
    fHistDefs.Clear();
  }

  //----------------------------------------------------------------------
  void SpectrumLoader::HandleFile(TFile* f, Progress* prog)
  {
    assert(!f->IsZombie());

    // In files with both "caf" and "cafTree", "cafTree" is the correct
    // version. "caf" is ROOT's temporary save while the file is being produced
    // and may be incomplete.
    TTree* tr = (TTree*)f->Get("cafTree");
    if(!tr) tr = (TTree*)f->Get("caf");

    assert(tr);

    caf::SRProxy sr(tr, "");

    FloatingExceptionOnNaN fpnan(false);

    long Nentries = tr->GetEntries();
    if(max_entries != 0 && max_entries < Nentries)
      Nentries = max_entries;

    for(long n = 0; n < Nentries; ++n){
      tr->LoadTree(n);

      FixupRecord(&sr);

      HandleRecord(&sr);

      if(prog && n%100 == 0) prog->SetProgress(double(n)/Nentries);
    } // end for n
  }

  //----------------------------------------------------------------------
  /// Helper for \ref SpectrumLoader::HandleRecord
  template<class T, class U> class CutVarCache
  {
  public:
    CutVarCache() : fCache(U::MaxID()+1) {}

    inline T Get(const U& var, const caf::SRProxy* sr)
    {
      const unsigned int id = var.ID();

      if(fCache[id].set) return fCache[id].val;

      const T val = var(sr);
      fCache[id].val = val;
      fCache[id].set = true;
      return val;
    }

  protected:
    // Seems to be faster to do this than [unordered_]map
    struct CacheElem{
      T val; bool set;
    };
    std::vector<CacheElem> fCache;
  };

  //----------------------------------------------------------------------
  void SpectrumLoader::HandleRecord(caf::SRProxy* sr)
  {
    // Some shifts only adjust the weight, so they're effectively nominal, but
    // aren't grouped with the other nominal histograms. Keep track of the
    // results for nominals in these caches to speed those systs up.
    CutVarCache<bool, Cut> nomCutCache;
    CutVarCache<double, Weight> nomWeiCache;
    CutVarCache<double, Var> nomVarCache;

    for(auto& shiftdef: fHistDefs){
      const SystShifts& shift = shiftdef.first;

      // Need to provide a clean slate for each new set of systematic shifts to
      // work from. Copying the whole StandardRecord is pretty expensive, so
      // modify it in place and revert it afterwards.

      // Please do not continue/break out of the shiftdef loop. If this is
      // required, let's use a RAII type here.
      caf::SRProxySystController::BeginTransaction();

      double systWeight = 1;
      bool shifted = false;

      // Can special-case nominal to not pay cost of Shift()
      if(!shift.IsNominal()){
        shift.Shift(sr, systWeight);
        // If there were only weighting systs applied then the cached nominal
        // values are still valid.
        shifted = caf::SRProxySystController::AnyShifted();
      }

      for(auto& cutdef: shiftdef.second){
        const Cut& cut = cutdef.first;

        const bool pass = shifted ? cut(sr) : nomCutCache.Get(cut, sr);
        // Cut failed, skip all the histograms that depended on it
        if(!pass) continue;

        for(auto& weidef: cutdef.second){
          const Weight& weivar = weidef.first;

          double wei = shifted ? weivar(sr) : nomWeiCache.Get(weivar, sr);

          if(wei < 0){
            std::cerr << "Negative weight " << wei
                      << " returned from Var";
            std::cerr << std::endl;
            abort();
          }

          wei *= systWeight;
          if(wei == 0) continue;

          for(auto& vardef: weidef.second){
            if(vardef.first.IsMulti()){
              for(double val: vardef.first.GetMultiVar()(sr)){
                for(Spectrum** s : vardef.second.spects)
                  if(*s) (*s)->Fill(val, wei);
              }
              continue;
            }

            const Var& var = vardef.first.GetVar();

            const double val = shifted ? var(sr) : nomVarCache.Get(var, sr);

            if(std::isnan(val) || std::isinf(val)){
              std::cerr << "Warning: Bad value: " << val
                        << " returned from a Var. The input variable(s) could "
                        << "be NaN in the CAF, or perhaps your "
                        << "Var code computed 0/0?";
              std::cout << " Not filling into this histogram for this slice."
                        << std::endl;
              continue;
            }

            for(Spectrum** s : vardef.second.spects)
              if(*s) (*s)->Fill(val, wei);

            for(auto rv: vardef.second.rwSpects){
              ReweightableSpectrum** rw = rv.first;
              if(!*rw) continue;
              const double yval = rv.second(sr);

              if(std::isnan(yval) || std::isinf(yval)){
                std::cerr << "Warning: Bad value: " << yval
                          << " for reweighting Var";
                std::cout << ". Not filling into histogram." << std::endl;
                continue;
              }

              // TODO: ignoring events with no true neutrino etc
              if(yval != 0)
                (*rw)->Fill(val, yval, wei);
            } // end for rw
          } // end for vardef
        } // end for weidef
      } // end for cutdef

      // Return StandardRecord to its unshifted form ready for the next
      // histogram.
      caf::SRProxySystController::Rollback();
    } // end for shiftdef

    assert(!caf::SRProxySystController::AnyShifted());
  }

  //----------------------------------------------------------------------
  void SpectrumLoader::ReportExposures()
  {
    // The POT member variables we use here were filled as part of
    // SpectrumLoaderBase::GetNextFile() as we looped through the input files.

    // Let's just assume no-one is using the Cut::POT() function yet, so this
    // printout remains relevant...

    std::cout << fPOT << " POT" << std::endl;
  }

  // cafanacore's spectra are expecting a different structure of
  // spectrumloader. But we can easily trick it with these.
  struct SpectrumSink
  {
    static void FillPOT(Spectrum* s, double pot){s->fPOT += pot;}
  };
  struct ReweightableSpectrumSink
  {
    static void FillPOT(ReweightableSpectrum* rw, double pot){rw->fPOT += pot;}
  };

  //----------------------------------------------------------------------
  void SpectrumLoader::StoreExposures() {
    for(auto& shiftdef: fHistDefs){
      for(auto& cutdef: shiftdef.second){
        for(auto& weidef: cutdef.second){
          for(auto& vardef: weidef.second){
            for(Spectrum** s : vardef.second.spects)
              if(*s) SpectrumSink::FillPOT(*s, fPOT);
            for(auto rv : vardef.second.rwSpects)
              if(*rv.first) ReweightableSpectrumSink::FillPOT(*rv.first, fPOT);
          }
        }
      }
    }
  }

} // namespace ana
