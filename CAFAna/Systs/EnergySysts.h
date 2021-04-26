#pragma once

#include "CAFAna/Core/ISyst.h"
#include "StandardRecord/Proxy/SRProxy.h"
#include "CAFAna/Core/Utilities.h"

#include "TFile.h"
#include "TH1.h"
#include "TRandom3.h"

#include <cassert>

namespace ana
{
  // FD global energy scale syst
  // Don't shift muon energies with this
  class EnergyScaleFD: public ISyst
  {
  public:
  EnergyScaleFD() : ISyst("EnergyScaleFD", "Global FD Energy Scale Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      double scale = .02 * sigma;
      if (sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        if (sr->Ev_reco_nue < 0.) sr->Ev_reco_nue = 0.;
        sr->Ev_reco_numu  += scale * sr->RecoHadEnNumu;
        sr->Ev_reco_nue   += scale * sr->RecoHadEnNue;
        sr->RecoHadEnNumu *= 1. + scale;
        sr->RecoHadEnNue  *= 1. + scale;
        if ( !(sr->isCC==1 && abs(sr->nuPDG) == 14) ) {
          sr->Ev_reco_nue  += scale * sr->RecoLepEnNue;
          sr->RecoLepEnNue *= 1. + scale;
        }
      }
    }
  };

  extern const EnergyScaleFD kEnergyScaleFD;

  // Total energy scale shape systematics
  class UncorrFDTotSqrt: public ISyst
  {
  public:
  UncorrFDTotSqrt() : ISyst("UncorrFDTotSqrt", "Uncorrelated FD Sqrt Total Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
        const double scale = .01 * sigma;
        if (sr->isFD) {
          if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
          sr->Ev_reco_numu  += scale*sqrt(sr->RecoHadEnNumu)*sr->RecoHadEnNumu;
          sr->Ev_reco_nue   += scale*sqrt(sr->RecoHadEnNue)*sr->RecoHadEnNue;
          sr->RecoHadEnNumu += scale*sqrt(sr->RecoHadEnNumu)*sr->RecoHadEnNumu;
          sr->RecoHadEnNue  += scale*sqrt(sr->RecoHadEnNue)*sr->RecoHadEnNue;
          if ( !(sr->isCC==1 && abs(sr->nuPDG) == 14) ) {
            sr->Ev_reco_nue  += scale*sqrt(sr->RecoLepEnNue)*sr->RecoLepEnNue;
            sr->RecoLepEnNue += scale*sqrt(sr->RecoLepEnNue)*sr->RecoLepEnNue;
          }
        }
    }
  };
  extern const UncorrFDTotSqrt kUncorrFDTotSqrt;

  class UncorrFDTotInvSqrt: public ISyst
  {
  public:
  UncorrFDTotInvSqrt() : ISyst("UncorrFDTotInvSqrt", "Uncorrelated FD Inverse Sqrt Total Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      if (sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        const double scale = .02 * sigma;
        sr->Ev_reco_numu += sr->RecoHadEnNumu*scale/sqrt(sr->RecoHadEnNumu+0.1);
        sr->Ev_reco_nue  += sr->RecoHadEnNue*scale/sqrt(sr->RecoHadEnNue+0.1);
        sr->RecoHadEnNumu += sr->RecoHadEnNumu*scale/sqrt(sr->RecoHadEnNumu+0.1);
        sr->RecoHadEnNue  += sr->RecoHadEnNue*scale/sqrt(sr->RecoHadEnNue+0.1);
        if ( !(sr->isCC==1 && abs(sr->nuPDG) == 14) ) {
          sr->Ev_reco_nue  += sr->RecoLepEnNue * scale / sqrt(sr->RecoLepEnNue+0.1);
          sr->RecoLepEnNue += sr->RecoLepEnNue * scale / sqrt(sr->RecoLepEnNue+0.1);
        }
      }
    }
  };
  extern const UncorrFDTotInvSqrt kUncorrFDTotInvSqrt;

  //------------------------------------------------------------------------------------------------

  // FD three parameter systematics to match ND ones
  // Slope energy scale systematics
  // Charged hadrons
  class UncorrFDHadSqrt: public ISyst
  {
  public:
  UncorrFDHadSqrt() : ISyst("UncorrFDHadSqrt", "Uncorrelated FD Sqrt Hadron Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      const double scale = .05 * sigma;
      if (sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        if (sr->eRecoP < 0) sr->eRecoP = 0.;
        if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
        if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
        double sumE = sr->eRecoP + sr->eRecoPip + sr->eRecoPim;
        sr->eRecoP   += sr->eRecoP * scale * sqrt(sumE);
        sr->eRecoPim += sr->eRecoPim * scale * sqrt(sumE);
        sr->eRecoPip += sr->eRecoPip * scale * sqrt(sumE);
        sr->Ev_reco_numu += sumE * scale * sqrt(sumE);
        sr->Ev_reco_nue  += sumE * scale * sqrt(sumE);
        sr->RecoHadEnNumu += sumE * scale * sqrt(sumE);
        sr->RecoHadEnNue  += sumE * scale * sqrt(sumE);
      }
    }
  };
  extern const UncorrFDHadSqrt kUncorrFDHadSqrt;

  class UncorrFDHadInvSqrt: public ISyst
  {
  public:
  UncorrFDHadInvSqrt() : ISyst("UncorrFDHadInvSqrt", "Uncorrelated FD Inv Sqrt Hadron Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      const double scale = .05 * sigma;
      if (sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        if (sr->eRecoP < 0) sr->eRecoP = 0.;
        if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
        if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
        double sumE = sr->eRecoP + sr->eRecoPip + sr->eRecoPim;
        sr->Ev_reco_numu += sumE * scale / sqrt(sumE+0.1);
        sr->Ev_reco_nue  += sumE * scale / sqrt(sumE+0.1);
        sr->eRecoP   += sr->eRecoP * scale / sqrt(sumE+0.1);
        sr->eRecoPim += sr->eRecoPim * scale / sqrt(sumE+0.1);
        sr->eRecoPip += sr->eRecoPip * scale / sqrt(sumE+0.1);
        sr->RecoHadEnNumu += sumE * scale / sqrt(sumE+0.1);
        sr->RecoHadEnNue  += sumE * scale / sqrt(sumE+0.1);
      }
    }
  };
  extern const UncorrFDHadInvSqrt kUncorrFDHadInvSqrt;

  //------------------------------------------------------------------------------------

  // Muons
  class UncorrFDMuSqrt: public ISyst
  {
  public:
  UncorrFDMuSqrt() : ISyst("UncorrFDMuSqrt", "Uncorrelated FD Sqrt Muon Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      if (sr->isFD && sr->isCC==1 && abs(sr->nuPDG)==14) {
        const double scale = .005 * sigma;
        sr->Ev_reco_numu  += sr->RecoLepEnNumu * sqrt(sr->RecoLepEnNumu) * scale;
        sr->RecoLepEnNumu += sr->RecoLepEnNumu * sqrt(sr->RecoLepEnNumu) * scale;
      }
    }
  };

  extern const UncorrFDMuSqrt kUncorrFDMuSqrt;

  class UncorrFDMuInvSqrt: public ISyst
  {
  public:
  UncorrFDMuInvSqrt() : ISyst("UncorrFDMuInvSqrt", "Uncorrelated FD Inv Sqrt Muon Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      if (sr->isFD && sr->isCC==1 && abs(sr->nuPDG)==14) {
        const double scale = .02 * sigma;
        sr->Ev_reco_numu  += sr->RecoLepEnNumu * scale / sqrt(sr->RecoLepEnNumu+0.1);
        sr->RecoLepEnNumu += sr->RecoLepEnNumu * scale / sqrt(sr->RecoLepEnNumu+0.1);
      }
    }
  };
  extern const UncorrFDMuInvSqrt kUncorrFDMuInvSqrt;

  //------------------------------------------------------------------------------------

  //Neutrons

  class UncorrFDNSqrt: public ISyst
  {
  public:
  UncorrFDNSqrt() : ISyst("UncorrFDNSqrt", "Uncorrelated FD Sqrt Neutron Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      const double scale = .3 * sigma;
      if (sr->isFD) {
        if (sr->eRecoN < 0.) { sr->eRecoN = 0.; }
        sr->Ev_reco_numu += sr->eRecoN * sqrt(sr->eRecoN) * scale;
        sr->Ev_reco_nue  += sr->eRecoN * sqrt(sr->eRecoN) * scale;
        sr->RecoHadEnNumu += sr->eRecoN * sqrt(sr->eRecoN) * scale;
        sr->RecoHadEnNue  += sr->eRecoN * sqrt(sr->eRecoN) * scale;
      }
    }
  };
  extern const UncorrFDNSqrt kUncorrFDNSqrt;

  class UncorrFDNInvSqrt: public ISyst
  {
  public:
  UncorrFDNInvSqrt() : ISyst("UncorrFDNInvSqrt", "Uncorrelated FD Inv Sqrt Neutron Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      const double scale = .3 * sigma;
      if (sr->isFD) {
        if (sr->eRecoN < 0.) { sr->eRecoN = 0.; }
        sr->Ev_reco_numu += sr->eRecoN * scale / sqrt(sr->eRecoN+0.1);
        sr->Ev_reco_nue  += sr->eRecoN * scale / sqrt(sr->eRecoN+0.1);
        sr->RecoHadEnNumu += sr->eRecoN * scale / sqrt(sr->eRecoN+0.1);
        sr->RecoHadEnNue  += sr->eRecoN * scale / sqrt(sr->eRecoN+0.1);
      }
    }
  };
  extern const UncorrFDNInvSqrt kUncorrFDNInvSqrt;

  //------------------------------------------------------------------------------------

  // Electromagnetic
  class UncorrFDEMSqrt: public ISyst
  {
  public:
  UncorrFDEMSqrt() : ISyst("UncorrFDEMSqrt", "Uncorrelated FD Sqrt EM Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      const double scale = .025 * sigma;
      if (sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        if (sr->eRecoPi0 < 0) sr->eRecoPi0 = 0.;
        sr->Ev_reco_numu += sr->eRecoPi0 * sqrt(sr->eRecoPi0) * scale;
        sr->Ev_reco_nue  += sr->eRecoPi0 * sqrt(sr->eRecoPi0) * scale;
        sr->RecoHadEnNumu += sr->eRecoPi0 * sqrt(sr->eRecoPi0) * scale;
        sr->RecoHadEnNue  += sr->eRecoPi0 * sqrt(sr->eRecoPi0) * scale;
        sr->eRecoPi0 += sr->eRecoPi0 * sqrt(sr->eRecoPi0) * scale;
        if (sr->eRecoPi0 < 0) sr->eRecoPi0 = 0.;
        if (sr->isCC==1 && abs(sr->nuPDG)==12) {
          sr->Ev_reco_nue  += sr->RecoLepEnNue*sqrt(sr->RecoLepEnNue)*scale;
          sr->RecoLepEnNue += sr->RecoLepEnNue*sqrt(sr->RecoLepEnNue)*scale;
        }
      }
    }
  };
  extern const UncorrFDEMSqrt kUncorrFDEMSqrt;

  class UncorrFDEMInvSqrt: public ISyst
  {
  public:
  UncorrFDEMInvSqrt() : ISyst("UncorrFDEMInvSqrt", "Uncorrelated FD Inv Sqrt EM Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      const double scale = .025 * sigma;
      if (sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        if (sr->eRecoPi0 < 0) sr->eRecoPi0 = 0.;
        sr->Ev_reco_numu += sr->eRecoPi0 * scale / sqrt(sr->eRecoPi0+0.1);
        sr->Ev_reco_nue  += sr->eRecoPi0 * scale / sqrt(sr->eRecoPi0+0.1);
        sr->RecoHadEnNumu += sr->eRecoPi0 * scale / sqrt(sr->eRecoPi0+0.1);
        sr->RecoHadEnNue  += sr->eRecoPi0 * scale / sqrt(sr->eRecoPi0+0.1);
        sr->eRecoPi0 += sr->eRecoPi0 * scale / sqrt(sr->eRecoPi0+0.1);
        if (sr->isCC==1 && abs(sr->nuPDG)==12) {
          sr->Ev_reco_nue  += sr->RecoLepEnNue * scale / sqrt(sr->RecoLepEnNue+0.1);
          sr->RecoLepEnNue += sr->RecoLepEnNue * scale / sqrt(sr->RecoLepEnNue+0.1);
        }
      }
    }
  };
  extern const UncorrFDEMInvSqrt kUncorrFDEMInvSqrt;

  // FD muon LAr systematic
  // 2% on CC numu events
  class EScaleMuLArFD: public ISyst
  {
  public:
  EScaleMuLArFD() : ISyst("EScaleMuLArFD", "Muon Energy Scale LAr Far Detector") {}

    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      const double scale = .02 * sigma;
      // Checks if FD  and select only CC muon neutrino events
      if(sr->isFD && abs(sr->nuPDG) == 14 && sr->isCC == 1){
        sr->Ev_reco_numu   += sr->RecoLepEnNumu * scale;
        sr->RecoLepEnNumu  *= 1. + scale;
      }
    }
  };

  extern const EScaleMuLArFD kEScaleMuLArFD;

  // 5% uncorrelated FD syst for charged hadrons
  class ChargedHadUncorrFD: public ISyst
  {
  public:
  ChargedHadUncorrFD() : ISyst("ChargedHadUncorrFD", "Charged Hadron Uncorrelated FD Syst") {}

    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      const double scale = .05 * sigma;
      if(sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        if (sr->eRecoP < 0) sr->eRecoP = 0.;
        if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
        if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
        const double sumE = sr->eRecoP + sr->eRecoPim + sr->eRecoPip;
        sr->eRecoP   += sr->eRecoP * scale;
        sr->eRecoPim += sr->eRecoPim * scale;
        sr->eRecoPip += sr->eRecoPip * scale;
        sr->Ev_reco_numu += sumE * scale;
        sr->Ev_reco_nue  += sumE * scale;
        sr->RecoHadEnNumu += sumE * scale;
        sr->RecoHadEnNue  += sumE * scale;
      }
    }
  };

  extern const ChargedHadUncorrFD kChargedHadUncorrFD;

  // Neutron energy scale
  // 20% on visible energy
  class NUncorrFD: public ISyst
  {
  public:
  NUncorrFD() : ISyst("NUncorrFD", "Neutron Energy Scale FD") {}

    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      const double scale = .20 * sigma;
      if(sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        if (sr->eRecoN < 0.) sr->eRecoN = 0.;
        sr->Ev_reco_numu  += sr->eRecoN * scale;
        sr->Ev_reco_nue   += sr->eRecoN * scale;
        sr->RecoHadEnNumu += sr->eRecoN * scale;
        sr->RecoHadEnNue  += sr->eRecoN * scale;
        sr->eRecoN += sr->eRecoN * scale;
      }
    }
  };

  extern const NUncorrFD kNUncorrFD;

  // FD systematic correlated for pi0s and electrons
  // 2.5% on reco energy for electrons and pi0s
  class EMUncorrFD: public ISyst
  {
  public:
  EMUncorrFD() : ISyst("EMUncorrFD", "Electromagnetic shower FD Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      const double scale = 0.025 * sigma;
      if (sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        if (sr->eRecoPi0 < 0.) sr->eRecoPi0 = 0.;
        sr->Ev_reco_nue  += sr->eRecoPi0 * scale;
        sr->Ev_reco_numu += sr->eRecoPi0 * scale;
        sr->RecoHadEnNue  += sr->eRecoPi0 * scale;
        sr->RecoHadEnNumu += sr->eRecoPi0 * scale;
        sr->eRecoPi0 += sr->eRecoPi0 * scale;
        if (sr->isCC && abs(sr->nuPDG) == 12) {
          sr->Ev_reco_nue  += sr->RecoLepEnNue * scale;
          sr->RecoLepEnNue += sr->RecoLepEnNue * scale;
        }
      }
    }
  };

  extern const EMUncorrFD kEMUncorrFD;

  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Resolution systematics
  // Muon energy resolution
  class MuonResFD: public ISyst
  {
  public:
  MuonResFD() : ISyst("MuonResFD", "Muon Far Detector Resolution Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      const double scale = .02*sigma;
      // FD charged current numus only
      if (sr->isFD && sr->isCC && abs(sr->nuPDG)==14) {
        sr->Ev_reco_numu  += (sr->LepE - sr->RecoLepEnNumu) * scale;
        sr->RecoLepEnNumu += (sr->LepE - sr->RecoLepEnNumu) * scale;
      }
    }
  };

  extern const MuonResFD kMuonResFD;

  // Electron/pi0 energy resolution
  class EMResFD: public ISyst
  {
  public:
  EMResFD() : ISyst("EMResFD", "EM shower Far Detector Resolution Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      const double scale = .02*sigma;
      if (sr->isFD){
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        if (sr->eRecoPi0 < 0.) sr->eRecoPi0 = 0.;
        sr->Ev_reco_nue  += (sr->ePi0 - sr->eRecoPi0) * scale;
        sr->Ev_reco_numu += (sr->ePi0 - sr->eRecoPi0) * scale;
        sr->RecoHadEnNumu += (sr->ePi0 - sr->eRecoPi0) * scale;
        sr->RecoHadEnNue  += (sr->ePi0 - sr->eRecoPi0) * scale;
        sr->eRecoPi0     += (sr->ePi0 - sr->eRecoPi0) * scale;
        if (sr->isCC && abs(sr->nuPDG)==12) {
          sr->Ev_reco_nue  += (sr->LepE - sr->RecoLepEnNue) * scale;
          sr->RecoLepEnNue += (sr->LepE - sr->RecoLepEnNue) * scale;
        }
      }
    }
  };

  extern const EMResFD kEMResFD;

  // Charged hadron energy resolution
  class ChargedHadResFD: public ISyst
  {
  public:
  ChargedHadResFD() : ISyst("ChargedHadResFD", "Charged Hadron Far Detector Resolution Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      const double scale = .02*sigma;
      if (sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        if (sr->eRecoP < 0.) sr->eRecoP = 0.;
        if (sr->eRecoPip < 0.) sr->eRecoPip = 0.;
        if (sr->eRecoPim < 0.) sr->eRecoPim = 0.;
        const double trueSum = sr->ePip + sr->ePim + sr->eP;
        const double recoSum = sr->eRecoPip + sr->eRecoPim + sr->eRecoP;
        sr->Ev_reco_nue   += (trueSum - recoSum) * scale;
        sr->Ev_reco_numu  += (trueSum - recoSum) * scale;
        sr->RecoHadEnNue  += (trueSum - recoSum) * scale;
        sr->RecoHadEnNumu += (trueSum - recoSum) * scale;
      }
    }
  };

  extern const ChargedHadResFD kChargedHadResFD;

  // Neutron energy resolution
  class NResFD: public ISyst
  {
  public:
  NResFD() : ISyst("NResFD", "Neutron Far Detector Resolution Syst") {}
    void Shift(double sigma,
               caf::SRProxy* sr, double& weight) const override
    {
      const double scale = .1*sigma;
      // FD charged current numus only
      if (sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        if (sr->eRecoN < 0.) sr->eRecoN = 0.;
        sr->Ev_reco_nue   += (sr->eN - sr->eRecoN) * scale;
        sr->Ev_reco_numu  += (sr->eN - sr->eRecoN) * scale;
        sr->RecoHadEnNue  += (sr->eN - sr->eRecoN) * scale;
        sr->RecoHadEnNumu += (sr->eN - sr->eRecoN) * scale;
        sr->eRecoN += (sr->eN - sr->eRecoN) * scale;
      }
    }
  };

  extern const NResFD kNResFD;

/*
  /// why have a structure (yet) if not needed
  /// make things simpler
  // Vector of energy scale systematics
  struct EnergySystVector: public std::vector<const ISyst*>
  {
  };
  EnergySystVector GetEnergySysts();
*/

  std::vector<const ISyst*> GetEnergySysts();

}
