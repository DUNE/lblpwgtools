#include "CAFAna/Core/SpectrumLoader.h"

#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/ReweightableSpectrum.h"
#ifndef DONT_USE_SAM
#include "CAFAna/Core/SAMProjectSource.h"
#endif
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Core/GenieWeightList.h"

#include "CAFAna/Core/ModeConversionUtilities.h"

#include "StandardRecord/StandardRecord.h"

#include <cassert>
#include <iostream>
#include <cmath>

#include "TFile.h"
#include "TH2.h"
#include "TTree.h"

namespace ana
{
  //----------------------------------------------------------------------
  SpectrumLoader::SpectrumLoader(const std::string& wildcard, DataSource src, int max)
    : SpectrumLoaderBase(wildcard, src), max_entries(max)
  {
  }

  //----------------------------------------------------------------------
  SpectrumLoader::SpectrumLoader(const std::vector<std::string>& fnames,
                                 DataSource src, int max)
    : SpectrumLoaderBase(fnames, src), max_entries(max)
  {
  }

  //----------------------------------------------------------------------
  SpectrumLoader::SpectrumLoader(DataSource src)
    : SpectrumLoaderBase(src)
  {
  }

#ifndef DONT_USE_SAM
  //----------------------------------------------------------------------
  SpectrumLoader SpectrumLoader::FromSAMProject(const std::string& proj,
                                                DataSource src,
                                                int fileLimit)
  {
    SpectrumLoader ret;
    ret.fSource = src;
    ret.fWildcard = "project "+proj;
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
    bool operator()(const Cut& a, const Cut& b)
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

    int fileIdx = -1;
    while(TFile* f = GetNextFile()){
      ++fileIdx;

      if(Nfiles >= 0 && !prog) prog = new Progress(TString::Format("Filling %lu spectra from %d files matching '%s'", fHistDefs.TotalSize(), Nfiles, fWildcard.c_str()).Data());

      HandleFile(f, Nfiles == 1 ? prog : 0);

      if(Nfiles > 1 && prog) prog->SetProgress((fileIdx+1.)/Nfiles);
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
  // Helper function that can give us a friendlier error message
  template<class T> void
  SetBranchChecked(TTree* tr, const std::string& bname, T* dest)
  {
    if(tr->FindBranch(bname.c_str()))
      tr->SetBranchAddress(bname.c_str(), dest);
    else
      std::cout << "Warning: Branch '" << bname << "' not found, field will not be filled" << std::endl;
  }

  //----------------------------------------------------------------------
  void SpectrumLoader::HandleFile(TFile* f, Progress* prog)
  {
    assert(!f->IsZombie());
    TTree* tr;
    //    if(f->GetListOfKeys()->Contains("cafmaker")){
    //      tr = (TTree*)f->Get("cafmaker/caf");
    //    }
    //    else{
    //      tr = (TTree*)f->Get("mvaselect/MVASelection");
    //    }
    tr = (TTree*)f->Get("caf");
    if(!tr){
      tr = (TTree*)f->Get("cafTree");
    }
    assert(tr);

    // Surely no-one will generate 1000 universes?
    std::vector<std::array<double, 1000>> genie_tmp;
    const std::vector<std::string> genie_names = GetGenieWeightNames();
    genie_tmp.resize(genie_names.size());
    std::vector<int> genie_size_tmp;
    genie_size_tmp.resize(genie_names.size());

    FloatingExceptionOnNaN fpnan(false);

    caf::StandardRecord sr;
    SetBranchChecked(tr, "Ev_reco", &sr.dune.Ev_reco);
    SetBranchChecked(tr, "Ev_reco_nue", &sr.dune.Ev_reco_nue);
    SetBranchChecked(tr, "Ev_reco_numu", &sr.dune.Ev_reco_numu);
    SetBranchChecked(tr, "Elep_reco", &sr.dune.Elep_reco);
    SetBranchChecked(tr, "theta_reco", &sr.dune.theta_reco);
    SetBranchChecked(tr, "mvaresult", &sr.dune.mvaresult);
    SetBranchChecked(tr, "mvanue", &sr.dune.mvanue);
    SetBranchChecked(tr, "mvanumu", &sr.dune.mvanumu);
    SetBranchChecked(tr, "cvnnue", &sr.dune.cvnnue);
    SetBranchChecked(tr, "cvnnumu", &sr.dune.cvnnumu);
    SetBranchChecked(tr, "numu_pid", &sr.dune.numu_pid);
    SetBranchChecked(tr, "nue_pid", &sr.dune.nue_pid);
    SetBranchChecked(tr, "reco_q", &sr.dune.reco_q);
    SetBranchChecked(tr, "RecoLepEnNue", &sr.dune.RecoLepEnNue);
    SetBranchChecked(tr, "RecoHadEnNue", &sr.dune.RecoHadEnNue);
    SetBranchChecked(tr, "RecoLepEnNumu", &sr.dune.RecoLepEnNumu);
    SetBranchChecked(tr, "RecoHadEnNumu", &sr.dune.RecoHadEnNumu);
    // ND pseudo-reconstruction flags
    SetBranchChecked(tr, "reco_numu", &sr.dune.reco_numu);
    SetBranchChecked(tr, "reco_nue", &sr.dune.reco_nue);
    SetBranchChecked(tr, "reco_nc", &sr.dune.reco_nc);
    // CW: add variables that Chris (M) wants for ND selections
    SetBranchChecked(tr, "muon_exit", &sr.dune.muon_exit);
    SetBranchChecked(tr, "muon_contained", &sr.dune.muon_contained);
    SetBranchChecked(tr, "muon_ecal", &sr.dune.muon_ecal);
    SetBranchChecked(tr, "muon_tracker", &sr.dune.muon_tracker);
    SetBranchChecked(tr, "Ehad_veto", &sr.dune.Ehad_veto);

    SetBranchChecked(tr, "Ev", &sr.dune.Ev);
    SetBranchChecked(tr, "Elep", &sr.dune.Elep);
    //    SetBranchChecked(tr, "ccnc", &sr.dune.ccnc);
    SetBranchChecked(tr, "isCC", &sr.dune.isCC);
    //    SetBranchChecked(tr, "beamPdg", &sr.dune.beamPdg);
    //    SetBranchChecked(tr, "neu", &sr.dune.neu);
    SetBranchChecked(tr, "nuPDG", &sr.dune.nuPDG);
    SetBranchChecked(tr, "nuPDGunosc", &sr.dune.nuPDGunosc);
    SetBranchChecked(tr, "LepPDG", &sr.dune.LepPDG);
    SetBranchChecked(tr, "mode", &sr.dune.mode);
    SetBranchChecked(tr, "nP", &sr.dune.nP);
    SetBranchChecked(tr, "nN", &sr.dune.nN);
    SetBranchChecked(tr, "nipi0", &sr.dune.nipi0);
    SetBranchChecked(tr, "nipip", &sr.dune.nipip);
    SetBranchChecked(tr, "nipim", &sr.dune.nipim);
    SetBranchChecked(tr, "Q2", &sr.dune.Q2);
    SetBranchChecked(tr, "W", &sr.dune.W);
    SetBranchChecked(tr, "Y", &sr.dune.Y);
    SetBranchChecked(tr, "X", &sr.dune.X);
    //    SetBranchChecked(tr, "cc", &sr.dune.cc);
    SetBranchChecked(tr, "NuMomX", &sr.dune.NuMomX);
    SetBranchChecked(tr, "NuMomY", &sr.dune.NuMomY);
    SetBranchChecked(tr, "NuMomZ", &sr.dune.NuMomZ);
    SetBranchChecked(tr, "LepMomX", &sr.dune.LepMomX);
    SetBranchChecked(tr, "LepMomY", &sr.dune.LepMomY);
    SetBranchChecked(tr, "LepMomZ", &sr.dune.LepMomZ);
    SetBranchChecked(tr, "LepE", &sr.dune.LepE);
    SetBranchChecked(tr, "LepNuAngle", &sr.dune.LepNuAngle);

    // Numu track containment flag
    SetBranchChecked(tr, "LongestTrackContNumu", &sr.dune.LongestTrackContNumu);

    SetBranchChecked(tr, "vtx_x",  &sr.dune.vtx_x);
    SetBranchChecked(tr, "vtx_y",  &sr.dune.vtx_y);
    SetBranchChecked(tr, "vtx_z",  &sr.dune.vtx_z);

    SetBranchChecked(tr, "det_x",  &sr.dune.det_x);

    SetBranchChecked(tr, "eP", &sr.dune.eP);
    SetBranchChecked(tr, "eN", &sr.dune.eN);
    SetBranchChecked(tr, "ePip", &sr.dune.ePip);
    SetBranchChecked(tr, "ePim", &sr.dune.ePim);
    SetBranchChecked(tr, "ePi0", &sr.dune.ePi0);
    SetBranchChecked(tr, "eOther", &sr.dune.eOther);
    SetBranchChecked(tr, "eRecoP", &sr.dune.eRecoP);
    SetBranchChecked(tr, "eRecoN", &sr.dune.eRecoN);
    SetBranchChecked(tr, "eRecoPip", &sr.dune.eRecoPip);
    SetBranchChecked(tr, "eRecoPim", &sr.dune.eRecoPim);
    SetBranchChecked(tr, "eRecoPi0", &sr.dune.eRecoPi0);
    SetBranchChecked(tr, "eRecoOther", &sr.dune.eRecoOther);

    SetBranchChecked(tr, "eDepP", &sr.dune.eDepP);
    SetBranchChecked(tr, "eDepN", &sr.dune.eDepN);
    SetBranchChecked(tr, "eDepPip", &sr.dune.eDepPip);
    SetBranchChecked(tr, "eDepPim", &sr.dune.eDepPim);
    SetBranchChecked(tr, "eDepPi0", &sr.dune.eDepPi0);
    SetBranchChecked(tr, "eDepOther", &sr.dune.eDepOther);

    SetBranchChecked(tr, "run", &sr.dune.run);
    SetBranchChecked(tr, "isFD", &sr.dune.isFD);
    SetBranchChecked(tr, "isFHC", &sr.dune.isFHC);

    SetBranchChecked(tr, "sigma_Ev_reco", &sr.dune.sigma_Ev_reco);
    SetBranchChecked(tr, "sigma_Elep_reco", &sr.dune.sigma_Elep_reco);
    SetBranchChecked(tr, "sigma_numu_pid", &sr.dune.sigma_numu_pid);
    SetBranchChecked(tr, "sigma_nue_pid", &sr.dune.sigma_nue_pid);

    // GENIE uncertainties and CVs
    sr.dune.genie_wgt    .resize(genie_names.size());
    sr.dune.genie_cv_wgt .resize(genie_names.size());

    for(unsigned int i = 0; i < genie_names.size(); ++i){
      SetBranchChecked(tr, "wgt_"+genie_names[i], &genie_tmp[i]);
      SetBranchChecked(tr, genie_names[i]+"_nshifts", &genie_size_tmp[i]);
      SetBranchChecked(tr, genie_names[i]+"_cvwgt", &sr.dune.genie_cv_wgt[i]);
    }

    int Nentries = tr->GetEntries();
    if (max_entries != 0 && max_entries < Nentries) Nentries = max_entries;

    for(int n = 0; n < Nentries; ++n){
      tr->GetEntry(n);

      //Set GENIE_ScatteringMode and eRec_FromDep
      if (sr.dune.isFD) {
        sr.dune.eRec_FromDep = sr.dune.eDepP + sr.dune.eDepN + sr.dune.eDepPip +
                               sr.dune.eDepPim + sr.dune.eDepPi0 +
                               sr.dune.eDepOther + sr.dune.LepE;

        sr.dune.GENIE_ScatteringMode = ana::GetGENIEModeFromSimbMode(sr.dune.mode);
      } else {
        sr.dune.eRec_FromDep = sr.dune.eRecoP + sr.dune.eRecoN +
                               sr.dune.eRecoPip + sr.dune.eRecoPim +
                               sr.dune.eRecoPi0 + sr.dune.eRecoOther +
                               sr.dune.LepE;
        sr.dune.GENIE_ScatteringMode = sr.dune.mode;
      }

      // Patch up isFD which isn't set properly in FD CAFs
      if(sr.dune.isFD){
        if(sr.dune.isFHC != 0 && sr.dune.isFHC != 1){
          if(sr.dune.run == 20000001 ||
             sr.dune.run == 20000002 ||
             sr.dune.run == 20000003){
            sr.dune.isFHC = true;
            static bool once = true;
            if(once){
              std::cout << "\nPatching up FD file to be considered FHC" << std::endl;
              once = false;
            }
          }
          else if(sr.dune.run == 20000004 ||
                  sr.dune.run == 20000005 ||
                  sr.dune.run == 20000006){
            sr.dune.isFHC = false;
            static bool once = true;
            if(once){
              std::cout << "\nPatching up FD file to be considered RHC" << std::endl;
              once = false;
            }
          }
          else{
            std::cout << "When patching FD CAF with unknown isFHC, saw unknown run " << sr.dune.run << std::endl;
            abort();
          }
        }
      }
      else{
        // ND
        if(sr.dune.isFHC == -1){
          // nu-on-e files
          sr.dune.isFHC = 0;
          static bool once = true;
          if(once){
            std::cout << "\nPatching up nu-on-e file to be considered FHC" << std::endl;
            once = false;
          }
        }
        else if(sr.dune.isFHC != 0 && sr.dune.isFHC != 1){
          std::cout << "isFHC not set properly in ND file: " << sr.dune.isFHC << std::endl;
          abort();
        }
      }

      // Reformat the genie systs
      sr.dune.total_cv_wgt = 1;

      for(unsigned int i = 0; i < genie_names.size(); ++i){
        const int Nuniv = genie_size_tmp[i];
        assert(Nuniv >= 0 && Nuniv <= int(genie_tmp[i].size()));
        sr.dune.genie_wgt[i].resize(Nuniv);

	// Do some error checking here
	if (std::isnan(sr.dune.genie_cv_wgt[i]) ||
	    std::isinf(sr.dune.genie_cv_wgt[i]) ||
	    sr.dune.genie_cv_wgt[i] == 0)
	  std::cout << "Warning: " << genie_names[i] << " has a bad CV of "
		    << sr.dune.genie_cv_wgt[i] << std::endl;
	else
	  sr.dune.total_cv_wgt *= sr.dune.genie_cv_wgt[i];

        for(int j = 0; j < Nuniv; ++j){
          sr.dune.genie_wgt[i][j] = genie_tmp[i][j];
        }
      }

      HandleRecord(&sr);

      if(prog && n%10000 == 0) prog->SetProgress(double(n)/Nentries);
    } // end for n
  }

  //----------------------------------------------------------------------
  /// Helper for \ref HandleRecord
  template<class T, class U> class CutVarCache
  {
  public:
    CutVarCache() : fVals(U::MaxID()+1), fValsSet(U::MaxID()+1, false) {}

    inline T Get(const U& var, const caf::StandardRecord* sr)
    {
      const unsigned int id = var.ID();

      if(fValsSet[id]){
        return fVals[id];
      }
      else{
        const T val = var(sr);
        fVals[id] = val;
        fValsSet[id] = true;
        return val;
      }
    }

  protected:
    // Seems to be faster to do this than [unordered_]map
    std::vector<T> fVals;
    std::vector<bool> fValsSet;
  };

  //----------------------------------------------------------------------
  void SpectrumLoader::HandleRecord(caf::StandardRecord* sr)
  {
    // Some shifts only adjust the weight, so they're effectively nominal, but
    // aren't grouped with the other nominal histograms. Keep track of the
    // results for nominals in these caches to speed those systs up.
    CutVarCache<bool, Cut> nomCutCache;
    CutVarCache<double, Var> nomWeiCache;
    CutVarCache<double, Var> nomVarCache;

    for(auto& shiftdef: fHistDefs){
      const SystShifts& shift = shiftdef.first;

      // Need to provide a clean slate for each new set of systematic shifts to
      // work from. Unfortunately, copying the whole StandardRecord is pretty
      // expensive. So we need to rely on this slightly dangerous "Restorer"
      // mechanism.

      // Spot checks to try and make sure no-one misses adding a variable to
      // Restorer
      static int iterationNo = 0;
      // Prime means we should get good coverage over all combinations
      const int kTestIterations = 9973;

      const TestVals* save = 0;
      if(++iterationNo % kTestIterations == 0)
        save = GetVals(sr, shiftdef.second);

      Restorer* restore = 0;
      double systWeight = 1;
      bool shifted = false;
      // Can special-case nominal to not pay cost of Shift() or Restorer
      if(!shift.IsNominal()){
        restore = new Restorer;
        shift.Shift(*restore, sr, systWeight);
        // Did the Shift actually modify the event at all?
        shifted = !restore->Empty();
      }

      for(auto& cutdef: shiftdef.second){
        const Cut& cut = cutdef.first;

        const bool pass = shifted ? cut(sr) : nomCutCache.Get(cut, sr);
        // Cut failed, skip all the histograms that depended on it
        if(!pass) continue;

        for(auto& weidef: cutdef.second){
          const Var& weivar = weidef.first;

          double wei = shifted ? weivar(sr) : nomWeiCache.Get(weivar, sr);

          wei *= systWeight;
          if(wei == 0) continue;

          for(auto& vardef: weidef.second){
            if(vardef.first.IsMulti()){
              for(double val: vardef.first.GetMultiVar()(sr)){
                for(Spectrum* s: vardef.second.spects)
                  s->Fill(val, wei);
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
              std::cout << " Not filling into this histogram for this slice." << std::endl;
              continue;
            }

            for(Spectrum* s: vardef.second.spects) s->Fill(val, wei);

            for(ReweightableSpectrum* rw: vardef.second.rwSpects){
              const double yval = rw->ReweightVar()(sr);

              if(std::isnan(yval) || std::isinf(yval)){
                std::cerr << "Warning: Bad value: " << yval
                          << " for reweighting Var";
                std::cout << ". Not filling into histogram." << std::endl;
                continue;
              }

              // TODO: ignoring events with no true neutrino etc
              if(yval != 0) rw->fHist->Fill(val, yval, wei);
            } // end for rw
          } // end for vardef
        } // end for weidef
      } // end for cutdef

      // Delete Restorer at this point and return StandardRecord to its
      // unshifted form ready for the next histogram.
      delete restore;

      // Make sure the record went back the way we found it
      if(save){
        CheckVals(save, sr, shift.ShortName(), shiftdef.second);
        delete save;
      }
    } // end for shiftdef
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

  //----------------------------------------------------------------------
  void SpectrumLoader::AccumulateExposures(const caf::SRSpill* spill)
  {
  }

  //----------------------------------------------------------------------
  void SpectrumLoader::StoreExposures()
  {
    for(auto& shiftdef: fHistDefs){
      for(auto& cutdef: shiftdef.second){
        for(auto& weidef: cutdef.second){
          for(auto& vardef: weidef.second){
            for(Spectrum* s: vardef.second.spects) s->fPOT += fPOT;
            for(ReweightableSpectrum* rw: vardef.second.rwSpects) rw->fPOT += fPOT;
          }
        }
      }
    }

    // std::map<int, double> livetime;
    // std::map<int, double> pot;

    // for(unsigned int i = 0; i < fAllCuts.size(); ++i){
    //   const int id = fAllCuts[i].ID();
    //   if(fLivetimeByCut[i] < 0){
    //     fLivetimeByCut[i] = 0;
    //     std::cout << "WARNING: no way to compute livetime for FD data spectrum. If you want a livetime you need to be applying one of the cuts from TimingCuts.h or similar. You probably should be anyway to remove bad data near the spill ends." << std::endl;
    //   }
    //   livetime.emplace(id, fLivetimeByCut[i]);
    //   pot.emplace(id, fPOTByCut[i]);
    // }

    // for(auto& shiftdef: fHistDefs){
    //   for(auto& cutdef: shiftdef.second){
    //     const Cut& cut = cutdef.first;
    //     const int id = cut.ID();

    //     for(auto& weidef: cutdef.second){
    //       for(auto& vardef: weidef.second){
    //         for(Spectrum* s: vardef.second.spects){
    //           s->fPOT += pot[id];
    //           s->fLivetime += livetime[id];
    //         }

    //         for(ReweightableSpectrum* rw: vardef.second.rwSpects){
    //           rw->fPOT += pot[id];
    //           rw->fLivetime += livetime[id];
    //         }
    //       }
    //     }
    //   }
    // }
  }

  //----------------------------------------------------------------------
  const SpectrumLoader::TestVals* SpectrumLoader::
  GetVals(const caf::StandardRecord* sr,
          IDMap<Cut, IDMap<Var, IDMap<VarOrMultiVar, SpectList>>>& hists) const
  {
    TestVals* ret = new TestVals;

    // Store values for all Vars and Cuts of interest
    for(auto& cutdef: hists){
      const bool cutval = cutdef.first(sr);
      ret->cuts.push_back(cutval);
      // Don't evaluate Vars when the Cut fails, might not be safe
      if(!cutval) continue;

      for(auto& weidef: cutdef.second){
        ret->weis.push_back(weidef.first(sr));

        for(auto& vardef: weidef.second){
          if(!vardef.first.IsMulti())
            ret->vars.push_back((vardef.first.GetVar())(sr));
        }
      }
    }

    return ret;
  }

  //----------------------------------------------------------------------
  void SpectrumLoader::ValError(const std::string& type,
                                const std::string& shift,
                                const std::set<std::string>& /*req*/,
                                double orig, double now) const
  {
    // Try and print a comprehensive error message, I imagine this might be
    // hard to track down.

    std::cerr << std::endl;

    std::cerr << "Error. Value of " << type
              << " changed after it was shifted and then restored."
              << std::endl;

    std::cerr << "While applying shift " << shift;

    std::cerr << " initially had value " << orig
              << " now has " << now << std::endl;

    std::cerr << "Please check your use of Restorer very carefully"
              << std::endl;

    abort();
  }

  //----------------------------------------------------------------------
  void SpectrumLoader::CheckVals(const TestVals* v,
                                 const caf::StandardRecord* sr,
                                 const std::string& shiftName,
                                 IDMap<Cut, IDMap<Var, IDMap<VarOrMultiVar, SpectList>>>& hists) const
  {
    unsigned int cutIdx = 0;
    unsigned int weiIdx = 0;
    unsigned int varIdx = 0;

    // Ensure everything is as TestVals says it should be

    for(auto& cutdef: hists){
      const bool cutval = cutdef.first(sr);

      if(cutval != v->cuts[cutIdx]){
        ValError("Cut", shiftName, {},
                 v->cuts[cutIdx], cutval);
      }
      ++cutIdx;

      // Don't evaluate Vars when the Cut fails, might not be safe
      if(!cutval) continue;

      for(auto& weidef: cutdef.second){
        const double weival = weidef.first(sr);
        if(!std::isnan(weival) && weival != v->weis[weiIdx]){
          ValError("Cut", shiftName, {},
                   v->weis[weiIdx], weival);
        }
        ++weiIdx;

        for(auto& vardef: weidef.second){
          if(vardef.first.IsMulti()) continue;
          const double varval = vardef.first.GetVar()(sr);
          if(!std::isnan(varval) && varval != v->vars[varIdx]){
            ValError("Var", shiftName, {},
                     v->vars[varIdx], varval);
          }
          ++varIdx;
        } // end for vardef
      } // end for weidef
    } // end for cutdef
  }
} // namespace
