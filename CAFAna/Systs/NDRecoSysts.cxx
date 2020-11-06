#include "CAFAna/Systs/NDRecoSysts.h"

#include "StandardRecord/Proxy/SRProxy.h"
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
  void RecoNCSyst::Shift(double sigma,
                         caf::SRProxy* sr,
                         double& weight) const
  {
    // Is ND
    if(!sr->isFD) {
      // CC event selection but is NC
      if((sr->reco_numu || sr->reco_nue) && (sr->muon_contained || sr->muon_tracker) && (sr->reco_q == -1 || sr->reco_q == 1) && sr->Ehad_veto<30 && !sr->isCC) {
        weight *= 1 + .2*sigma;
      }
    }
  }

  //----------------------------------------------------------------------
  void LeptonAccSyst::Shift(double sigma,
                            caf::SRProxy* sr,
                            double& weight) const
  {
    // Load hist if it hasn't been loaded already
    const double m_mu = 0.105658;
    if (!fHist) {
      #ifndef DONT_USE_FQ_HARDCODED_SYST_PATHS
      TFile f("/app/users/marshalc/ND_syst/ND_eff_syst.root", "read");
      #else
      TFile f((FindCAFAnaDir()+"/Systs/ND_eff_syst.root").c_str());
      #endif
      assert(!f.IsZombie());
      fHist = (TH2*)f.Get("unc");
      fHist->SetDirectory(0);
    }

    // Is ND and is a true numu CC event
    if (!sr->isFD && sr->isCC && abs(sr->nuPDG) == 14) {
      double LepE = sr->LepE;
      int bin = fHist->FindBin(sqrt(LepE*LepE - m_mu*m_mu) * cos(sr->LepNuAngle), sqrt(LepE*LepE - m_mu*m_mu) * sin(sr->LepNuAngle));
      double w = fHist->GetBinContent(bin);
      weight *= 1. + w*sigma;
    }
  }

  //----------------------------------------------------------------------
  void HadronAccSyst::Shift(double sigma,
                            caf::SRProxy* sr,
                            double& weight) const
  {
    // Load hist if it hasn't been loaded already
    if (!fHist) {
      #ifndef DONT_USE_FQ_HARDCODED_SYST_PATHS
      TFile f("/app/users/marshalc/ND_syst/ND_eff_syst.root", "read");
      #else
      TFile f((FindCAFAnaDir()+"/Systs/ND_eff_syst.root").c_str());
      #endif
      assert(!f.IsZombie());
      fHist = (TH1*)f.Get("hunc");
      fHist->SetDirectory(0);
    }

    // Is ND
    if (!sr->isFD) {
      double HadE = sr->Ev - sr->LepE;
      if (HadE > 5.) {
        HadE = 5.;
      }
      int bin = fHist->FindBin(HadE);
      double w = fHist->GetBinContent(bin);
      weight *= 1. + w*sigma;
    }
  }

  //----------------------------------------------------------------------
  std::vector<const ISyst*> GetNDRecoSysts()
  {
    std::vector<const ISyst*> vec;
    vec.push_back(&kRecoNCSyst);
    vec.push_back(&kFVNumuNDSyst);
    return vec;
  }

}
