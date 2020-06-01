#include "CAFAna/Core/SpectrumLoader.h"

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/ReweightableSpectrum.h"
#ifndef DONT_USE_SAM
#include "CAFAna/Core/SAMProjectSource.h"
#endif
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"
#include "StandardRecord/Proxy/SRProxy.h"

#include "CAFAna/Systs/XSecSystList.h"
#include "CAFAna/Core/FixupRecord.h"

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
  template <class T>
  bool SetBranchChecked(TTree *tr, const std::string &bname, T *dest) {
    if (tr->FindBranch(bname.c_str())) {
      tr->SetBranchAddress(bname.c_str(), dest);
      return true;
    } else {
      std::cout << "Warning: Branch '" << bname
                << "' not found, field will not be filled" << std::endl;
    }
    return false;
  }

  //----------------------------------------------------------------------
  void SpectrumLoader::HandleFile(TFile* f, Progress* prog)
  {
    assert(!f->IsZombie());
    TTree* tr = (TTree*)f->Get("caf");
    if(!tr) tr = (TTree*)f->Get("cafTree");

    assert(tr);

  FloatingExceptionOnNaN fpnan(false);

  caf::StandardRecord sr;
  SetBranchChecked(tr, "Ev_reco", &sr.Ev_reco);
  SetBranchChecked(tr, "Ev_reco_nue", &sr.Ev_reco_nue);
  SetBranchChecked(tr, "Ev_reco_numu", &sr.Ev_reco_numu);
  SetBranchChecked(tr, "Elep_reco", &sr.Elep_reco);
  SetBranchChecked(tr, "theta_reco", &sr.theta_reco);
  SetBranchChecked(tr, "mvaresult", &sr.mvaresult);
  SetBranchChecked(tr, "mvanue", &sr.mvanue);
  SetBranchChecked(tr, "mvanumu", &sr.mvanumu);
  SetBranchChecked(tr, "cvnnue", &sr.cvnnue);
  SetBranchChecked(tr, "cvnnumu", &sr.cvnnumu);
  SetBranchChecked(tr, "numu_pid", &sr.numu_pid);
  SetBranchChecked(tr, "nue_pid", &sr.nue_pid);
  SetBranchChecked(tr, "reco_q", &sr.reco_q);
  SetBranchChecked(tr, "RecoLepEnNue", &sr.RecoLepEnNue);
  SetBranchChecked(tr, "RecoHadEnNue", &sr.RecoHadEnNue);
  SetBranchChecked(tr, "RecoLepEnNumu", &sr.RecoLepEnNumu);
  SetBranchChecked(tr, "RecoHadEnNumu", &sr.RecoHadEnNumu);
  // ND pseudo-reconstruction flags
  SetBranchChecked(tr, "reco_numu", &sr.reco_numu);
  SetBranchChecked(tr, "reco_nue", &sr.reco_nue);
  SetBranchChecked(tr, "reco_nc", &sr.reco_nc);
  // CW: add variables that Chris (M) wants for ND selections
  SetBranchChecked(tr, "muon_exit", &sr.muon_exit);
  SetBranchChecked(tr, "muon_contained", &sr.muon_contained);
  SetBranchChecked(tr, "muon_ecal", &sr.muon_ecal);
  SetBranchChecked(tr, "muon_tracker", &sr.muon_tracker);
  SetBranchChecked(tr, "Ehad_veto", &sr.Ehad_veto);

  SetBranchChecked(tr, "Ev", &sr.Ev);
  SetBranchChecked(tr, "Elep", &sr.Elep);
  //    SetBranchChecked(tr, "ccnc", &sr.ccnc);
  SetBranchChecked(tr, "isCC", &sr.isCC);
  //    SetBranchChecked(tr, "beamPdg", &sr.beamPdg);
  //    SetBranchChecked(tr, "neu", &sr.neu);
  SetBranchChecked(tr, "nuPDG", &sr.nuPDG);
  SetBranchChecked(tr, "nuPDGunosc", &sr.nuPDGunosc);
  SetBranchChecked(tr, "LepPDG", &sr.LepPDG);
  SetBranchChecked(tr, "mode", &sr.mode);
  SetBranchChecked(tr, "nP", &sr.nP);
  SetBranchChecked(tr, "nN", &sr.nN);
  SetBranchChecked(tr, "nipi0", &sr.nipi0);
  SetBranchChecked(tr, "nipip", &sr.nipip);
  SetBranchChecked(tr, "nipim", &sr.nipim);
  SetBranchChecked(tr, "niem", &sr.niem);
  SetBranchChecked(tr, "Q2", &sr.Q2);
  SetBranchChecked(tr, "W", &sr.W);
  SetBranchChecked(tr, "Y", &sr.Y);
  SetBranchChecked(tr, "X", &sr.X);
  //    SetBranchChecked(tr, "cc", &sr.cc);
  SetBranchChecked(tr, "NuMomX", &sr.NuMomX);
  SetBranchChecked(tr, "NuMomY", &sr.NuMomY);
  SetBranchChecked(tr, "NuMomZ", &sr.NuMomZ);
  SetBranchChecked(tr, "LepMomX", &sr.LepMomX);
  SetBranchChecked(tr, "LepMomY", &sr.LepMomY);
  SetBranchChecked(tr, "LepMomZ", &sr.LepMomZ);
  SetBranchChecked(tr, "LepE", &sr.LepE);
  SetBranchChecked(tr, "LepNuAngle", &sr.LepNuAngle);

  // Numu track containment flag
  SetBranchChecked(tr, "LongestTrackContNumu", &sr.LongestTrackContNumu);

  SetBranchChecked(tr, "vtx_x", &sr.vtx_x);
  SetBranchChecked(tr, "vtx_y", &sr.vtx_y);
  SetBranchChecked(tr, "vtx_z", &sr.vtx_z);

  SetBranchChecked(tr, "det_x", &sr.det_x);

  SetBranchChecked(tr, "eP", &sr.eP);
  SetBranchChecked(tr, "eN", &sr.eN);
  SetBranchChecked(tr, "ePip", &sr.ePip);
  SetBranchChecked(tr, "ePim", &sr.ePim);
  SetBranchChecked(tr, "ePi0", &sr.ePi0);
  SetBranchChecked(tr, "eOther", &sr.eOther);
  SetBranchChecked(tr, "eRecoP", &sr.eRecoP);
  SetBranchChecked(tr, "eRecoN", &sr.eRecoN);
  SetBranchChecked(tr, "eRecoPip", &sr.eRecoPip);
  SetBranchChecked(tr, "eRecoPim", &sr.eRecoPim);
  SetBranchChecked(tr, "eRecoPi0", &sr.eRecoPi0);
  SetBranchChecked(tr, "eRecoOther", &sr.eRecoOther);

  SetBranchChecked(tr, "eDepP", &sr.eDepP);
  SetBranchChecked(tr, "eDepN", &sr.eDepN);
  SetBranchChecked(tr, "eDepPip", &sr.eDepPip);
  SetBranchChecked(tr, "eDepPim", &sr.eDepPim);
  SetBranchChecked(tr, "eDepPi0", &sr.eDepPi0);
  SetBranchChecked(tr, "eDepOther", &sr.eDepOther);

  SetBranchChecked(tr, "run", &sr.run);
  SetBranchChecked(tr, "isFD", &sr.isFD);
  SetBranchChecked(tr, "isFHC", &sr.isFHC);

  SetBranchChecked(tr, "sigma_Ev_reco", &sr.sigma_Ev_reco);
  SetBranchChecked(tr, "sigma_Elep_reco", &sr.sigma_Elep_reco);
  SetBranchChecked(tr, "sigma_numu_pid", &sr.sigma_numu_pid);
  SetBranchChecked(tr, "sigma_nue_pid", &sr.sigma_nue_pid);

  // Get the crazy fluxes
  std::array<double, 7> crazy_tmp;
  SetBranchChecked(tr, "wgt_CrazyFlux", &crazy_tmp);

  // XSec uncertainties and CVs
  std::vector<std::array<double, 100>> XSSyst_tmp;
  std::vector<double> XSSyst_cv_tmp;
  std::vector<int> XSSyst_size_tmp;

  std::vector<std::string> const &XSSyst_names = GetAllXSecSystNames();
  XSSyst_tmp.resize(XSSyst_names.size());
  XSSyst_cv_tmp.resize(XSSyst_names.size());
  XSSyst_size_tmp.resize(XSSyst_names.size());

  sr.xsSyst_wgt.resize(XSSyst_names.size());

  for (unsigned int syst_it = 0; syst_it < XSSyst_names.size(); ++syst_it) {
    if (!SetBranchChecked(tr, "wgt_" + XSSyst_names[syst_it],
                          &XSSyst_tmp[syst_it])) {
      std::fill_n(XSSyst_tmp[syst_it].begin(), 100, 1);
      XSSyst_cv_tmp[syst_it] = 1;
      XSSyst_size_tmp[syst_it] = 1;
      continue;
    }

    SetBranchChecked(tr, XSSyst_names[syst_it] + "_nshifts",
                     &XSSyst_size_tmp[syst_it]);
    SetBranchChecked(tr, XSSyst_names[syst_it] + "_cvwgt",
                     &XSSyst_cv_tmp[syst_it]);
  }

  int Nentries = tr->GetEntries();
  if (max_entries != 0 && max_entries < Nentries)
    Nentries = max_entries;

  for (int n = 0; n < Nentries; ++n) {
    tr->GetEntry(n);

    for(unsigned int syst_it = 0; syst_it < XSSyst_names.size(); ++syst_it) {
      const int Nuniv = XSSyst_tmp[syst_it].size();
      assert((Nuniv >= 0) && (Nuniv <= int(XSSyst_tmp[syst_it].size())));
      sr.xsSyst_wgt[syst_it].resize(Nuniv);
      for(int u_it = 0; u_it < Nuniv; ++u_it){
        sr.xsSyst_wgt[syst_it][u_it] = XSSyst_tmp[syst_it][u_it];
      }
    }

    // Get the crazy flux info properly
    sr.wgt_CrazyFlux.resize(7);
    for (int i = 0; i < 7; ++i) {
      sr.wgt_CrazyFlux[i] = crazy_tmp[i];
    }

    FixupRecord(&sr);

    HandleRecord(&sr);

    if (prog && n % 10000 == 0)
      prog->SetProgress(double(n) / Nentries);
  } // end for n
}

  //----------------------------------------------------------------------
  /// Helper for \ref HandleRecord
  template <class T, class U> class CutVarCache {
  public:
    CutVarCache() : fVals(U::MaxID() + 1), fValsSet(U::MaxID() + 1, false) {}

    inline T Get(const U& var, const caf::SRProxy* sr) {
      const unsigned int id = var.ID();

      if (fValsSet[id]) {
        return fVals[id];
      } else {
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


      static caf::SRProxy* srp = new caf::SRProxy(0, 0, "", 0, 0);
      *srp = *sr;

      Restorer *restore = 0;
      double systWeight = 1;
      bool shifted = false;
      // Can special-case nominal to not pay cost of Shift() or Restorer
      if(!shift.IsNominal()){
        restore = new Restorer;
        shift.Shift(*restore, srp, systWeight);
        // Did the Shift actually modify the event at all?
        shifted = !restore->Empty();
      }

      for(auto& cutdef: shiftdef.second){
        const Cut& cut = cutdef.first;

        const bool pass = shifted ? cut(srp) : nomCutCache.Get(cut, srp);
        // Cut failed, skip all the histograms that depended on it
        if(!pass) continue;

        for(auto& weidef: cutdef.second){
          const Var& weivar = weidef.first;

          double wei = shifted ? weivar(srp) : nomWeiCache.Get(weivar, srp);

          wei *= systWeight;
          if(wei == 0) continue;

          for(auto& vardef: weidef.second){
            if(vardef.first.IsMulti()){
              for(double val: vardef.first.GetMultiVar()(srp)){
                for(Spectrum* s: vardef.second.spects)
                  s->Fill(val, wei);
              }
              continue;
            }

            const Var& var = vardef.first.GetVar();

            const double val = shifted ? var(srp) : nomVarCache.Get(var, srp);

            if(std::isnan(val) || std::isinf(val)){
              std::cerr << "Warning: Bad value: " << val
                        << " returned from a Var. The input variable(s) could "
                        << "be NaN in the CAF, or perhaps your "
                        << "Var code computed 0/0?";
              std::cout << " Not filling into this histogram for this slice."
                        << std::endl;
              continue;
            }

            for(Spectrum* s: vardef.second.spects)
              s->Fill(val, wei);

            for(ReweightableSpectrum* rw: vardef.second.rwSpects){
              const double yval = rw->ReweightVar()(srp);

              if(std::isnan(yval) || std::isinf(yval)){
                std::cerr << "Warning: Bad value: " << yval
                          << " for reweighting Var";
                std::cout << ". Not filling into histogram." << std::endl;
                continue;
              }

              // TODO: ignoring events with no true neutrino etc
              if(yval != 0)
                rw->fHist->Fill(val, yval, wei);
            } // end for rw
          } // end for vardef
        } // end for weidef
      } // end for cutdef

      // Delete Restorer at this point and return StandardRecord to its
      // unshifted form ready for the next histogram.
      delete restore;
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
  void SpectrumLoader::StoreExposures()
  {
    for(auto& shiftdef: fHistDefs){
      for(auto& cutdef: shiftdef.second){
        for(auto& weidef: cutdef.second){
          for(auto& vardef: weidef.second){
            for(Spectrum* s: vardef.second.spects)
              s->fPOT += fPOT;
            for(ReweightableSpectrum* rw: vardef.second.rwSpects)
              rw->fPOT += fPOT;
          }
        }
      }
    }
  }

} // namespace ana
