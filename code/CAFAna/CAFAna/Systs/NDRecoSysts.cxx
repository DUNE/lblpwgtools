#include "CAFAna/Systs/NDRecoSysts.h"

#include "StandardRecord/StandardRecord.h"
#include "CAFAna/Core/Utilities.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include <cassert>

namespace ana
{
  const RecoNCSyst kRecoNCSyst;
  const LeptonAccSyst kLeptonAccSyst;
  const HadronAccSyst kHadronAccSyst;
  const FVNumuNDSyst kFVNumuNDSyst;

  //----------------------------------------------------------------------
  NDRecoSystVector GetNDRecoSysts()
  {
    NDRecoSystVector vec;
    vec.push_back(&kRecoNCSyst);
    vec.push_back(&kFVNumuNDSyst);
    return vec;
  }

  //----------------------------------------------------------------------
  void RecoNCSyst::Shift(double sigma,
                         Restorer& restore,
                         caf::StandardRecord* sr,
                         double& weight) const
  {
    // Is ND
    if(!sr->dune.isFD) {
      // CC event selection but is NC
      if((sr->dune.reco_numu || sr->dune.reco_nue) && (sr->dune.muon_contained || sr->dune.muon_tracker) && (sr->dune.reco_q == -1 || sr->dune.reco_q == 1) && sr->dune.Ehad_veto<30 && !sr->dune.isCC) {
        weight *= 1 + .2*sigma;
      }
    }
  }

  //----------------------------------------------------------------------
  void LeptonAccSyst::Shift(double sigma,
                            Restorer& restore,
                            caf::StandardRecord* sr,
                            double& weight) const
  {
    // Load hist if it hasn't been loaded already
    const double m_mu = 0.105658;
    if (!fHist) {
      #ifndef DONT_USE_FQ_HARDCODED_SYST_PATHS
      TFile f("/dune/app/users/marshalc/ND_syst/ND_eff_syst.root", "read");
      #else
      TFile f((FindCAFAnaDir()+"/Systs/ND_eff_syst.root").c_str());
      #endif
      assert(!f.IsZombie());
      fHist = (TH2*)f.Get("unc");
      fHist->SetDirectory(0);
    }

    // Is ND and is a true numu CC event
    if (!sr->dune.isFD && sr->dune.isCC && abs(sr->dune.nuPDG) == 14) {
      double LepE = sr->dune.LepE;
      int bin = fHist->FindBin(sqrt(LepE*LepE - m_mu*m_mu) * cos(sr->dune.LepNuAngle), sqrt(LepE*LepE - m_mu*m_mu) * sin(sr->dune.LepNuAngle));
      double w = fHist->GetBinContent(bin);
      weight *= 1. + w*sigma;
    }
  }

  //----------------------------------------------------------------------
  void HadronAccSyst::Shift(double sigma,
                            Restorer& restore,
                            caf::StandardRecord* sr,
                            double& weight) const
  {
    // Load hist if it hasn't been loaded already
    if (!fHist) {
      #ifndef DONT_USE_FQ_HARDCODED_SYST_PATHS
      TFile f("/dune/app/users/marshalc/ND_syst/ND_eff_syst.root", "read");
      #else
      TFile f((FindCAFAnaDir()+"/Systs/ND_eff_syst.root").c_str());
      #endif
      assert(!f.IsZombie());
      fHist = (TH1*)f.Get("hunc");
      fHist->SetDirectory(0);
    }

    // Is ND
    if (!sr->dune.isFD) {
      double HadE = sr->dune.Ev - sr->dune.LepE;
      if (HadE > 5.) {
        HadE = 5.;
      }
      int bin = fHist->FindBin(HadE);
      double w = fHist->GetBinContent(bin);
      weight *= 1. + w*sigma;
    }
  }
}
