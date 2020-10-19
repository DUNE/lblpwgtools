#pragma once

#include "CAFAna/Core/ISyst.h"
#include "StandardRecord/SRProxy.h"
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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_numu,
                  sr->Ev_reco_nue,
                  sr->RecoHadEnNumu,
                  sr->RecoHadEnNue,
                  sr->RecoLepEnNue);

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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_numu,
                  sr->Ev_reco_nue,
                  sr->RecoHadEnNumu,
                  sr->RecoHadEnNue,
                  sr->RecoLepEnNue);
        const double scale = .01 * sigma;
        if (sr->isFD) {
          if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
          sr->Ev_reco_numu  += scale*pow(sr->RecoHadEnNumu, 0.5)*sr->RecoHadEnNumu;
          sr->Ev_reco_nue   += scale*pow(sr->RecoHadEnNue, 0.5)*sr->RecoHadEnNue;
          sr->RecoHadEnNumu += scale*pow(sr->RecoHadEnNumu, 0.5)*sr->RecoHadEnNumu;
          sr->RecoHadEnNue  += scale*pow(sr->RecoHadEnNue, 0.5)*sr->RecoHadEnNue;
          if ( !(sr->isCC==1 && abs(sr->nuPDG) == 14) ) {
            sr->Ev_reco_nue  += scale*pow(sr->RecoLepEnNue, 0.5)*sr->RecoLepEnNue;
            sr->RecoLepEnNue += scale*pow(sr->RecoLepEnNue, 0.5)*sr->RecoLepEnNue;
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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_numu,
                  sr->Ev_reco_nue,
                  sr->RecoHadEnNumu,
                  sr->RecoHadEnNue,
                  sr->RecoLepEnNue);
      if (sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        const double scale = .02 * sigma;
        sr->Ev_reco_numu += sr->RecoHadEnNumu*scale*pow(sr->RecoHadEnNumu+0.1, -0.5);
        sr->Ev_reco_nue  += sr->RecoHadEnNue*scale*pow(sr->RecoHadEnNue+0.1, -0.5);
        sr->RecoHadEnNumu += sr->RecoHadEnNumu*scale*pow(sr->RecoHadEnNumu+0.1, -0.5);
        sr->RecoHadEnNue  += sr->RecoHadEnNue*scale*pow(sr->RecoHadEnNue+0.1, -0.5);
        if ( !(sr->isCC==1 && abs(sr->nuPDG) == 14) ) {
          sr->Ev_reco_nue  += sr->RecoLepEnNue * scale * pow(sr->RecoLepEnNue+0.1, -0.5);
          sr->RecoLepEnNue += sr->RecoLepEnNue * scale * pow(sr->RecoLepEnNue+0.1, -0.5);
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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_numu,
                  sr->Ev_reco_nue,
                  sr->RecoHadEnNumu,
                  sr->RecoHadEnNue,
                  sr->eRecoP,
                  sr->eRecoPim,
                  sr->eRecoPip);
      const double scale = .05 * sigma;
      if (sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        if (sr->eRecoP < 0) sr->eRecoP = 0.;
        if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
        if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
        double sumE = sr->eRecoP + sr->eRecoPip + sr->eRecoPim;
        sr->eRecoP   += sr->eRecoP * scale * pow(sumE, 0.5);
        sr->eRecoPim += sr->eRecoPim * scale * pow(sumE, 0.5);
        sr->eRecoPip += sr->eRecoPip * scale * pow(sumE, 0.5);
        sr->Ev_reco_numu += sumE * scale * pow(sumE, 0.5);
        sr->Ev_reco_nue  += sumE * scale * pow(sumE, 0.5);
        sr->RecoHadEnNumu += sumE * scale * pow(sumE, 0.5);
        sr->RecoHadEnNue  += sumE * scale * pow(sumE, 0.5);
      }
    }
  };
  extern const UncorrFDHadSqrt kUncorrFDHadSqrt;

  class UncorrFDHadInvSqrt: public ISyst
  {
  public:
  UncorrFDHadInvSqrt() : ISyst("UncorrFDHadInvSqrt", "Uncorrelated FD Inv Sqrt Hadron Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_numu,
                  sr->Ev_reco_nue,
                  sr->RecoHadEnNumu,
                  sr->RecoHadEnNue,
                  sr->eRecoP,
                  sr->eRecoPim,
                  sr->eRecoPip);
      const double scale = .05 * sigma;
      if (sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        if (sr->eRecoP < 0) sr->eRecoP = 0.;
        if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
        if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
        double sumE = sr->eRecoP + sr->eRecoPip + sr->eRecoPim;
        sr->Ev_reco_numu += sumE * scale * pow(sumE+0.1, -0.5);
        sr->Ev_reco_nue  += sumE * scale * pow(sumE+0.1, -0.5);
        sr->eRecoP   += sr->eRecoP * scale * pow(sumE+0.1, -0.5);
        sr->eRecoPim += sr->eRecoPim * scale * pow(sumE+0.1, -0.5);
        sr->eRecoPip += sr->eRecoPip * scale * pow(sumE+0.1, -0.5);
        sr->RecoHadEnNumu += sumE * scale * pow(sumE+0.1, -0.5);
        sr->RecoHadEnNue  += sumE * scale * pow(sumE+0.1, -0.5);
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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_numu,
                  sr->RecoLepEnNumu);
      if (sr->isFD && sr->isCC==1 && abs(sr->nuPDG)==14) {
        const double scale = .005 * sigma;
        sr->Ev_reco_numu  += sr->RecoLepEnNumu * pow(sr->RecoLepEnNumu, 0.5) * scale;
        sr->RecoLepEnNumu += sr->RecoLepEnNumu * pow(sr->RecoLepEnNumu, 0.5) * scale;
      }
    }
  };

  extern const UncorrFDMuSqrt kUncorrFDMuSqrt;

  class UncorrFDMuInvSqrt: public ISyst
  {
  public:
  UncorrFDMuInvSqrt() : ISyst("UncorrFDMuInvSqrt", "Uncorrelated FD Inv Sqrt Muon Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_numu,
                  sr->RecoLepEnNumu);
      if (sr->isFD && sr->isCC==1 && abs(sr->nuPDG)==14) {
        const double scale = .02 * sigma;
        sr->Ev_reco_numu  += sr->RecoLepEnNumu * scale * pow(sr->RecoLepEnNumu+0.1, -0.5);
        sr->RecoLepEnNumu += sr->RecoLepEnNumu * scale * pow(sr->RecoLepEnNumu+0.1, -0.5);
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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_numu,
                  sr->Ev_reco_nue,
                  sr->RecoHadEnNumu,
                  sr->RecoHadEnNue,
                  sr->eRecoN);
      const double scale = .3 * sigma;
      if (sr->isFD) {
        if (sr->eRecoN < 0.) { sr->eRecoN = 0.; }
        sr->Ev_reco_numu += sr->eRecoN * pow(sr->eRecoN, 0.5) * scale;
        sr->Ev_reco_nue  += sr->eRecoN * pow(sr->eRecoN, 0.5) * scale;
        sr->RecoHadEnNumu += sr->eRecoN * pow(sr->eRecoN, 0.5) * scale;
        sr->RecoHadEnNue  += sr->eRecoN * pow(sr->eRecoN, 0.5) * scale;
      }
    }
  };
  extern const UncorrFDNSqrt kUncorrFDNSqrt;

  class UncorrFDNInvSqrt: public ISyst
  {
  public:
  UncorrFDNInvSqrt() : ISyst("UncorrFDNInvSqrt", "Uncorrelated FD Inv Sqrt Neutron Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_numu,
                  sr->Ev_reco_nue,
                  sr->RecoHadEnNumu,
                  sr->RecoHadEnNue,
                  sr->eRecoN);
      const double scale = .3 * sigma;
      if (sr->isFD) {
        if (sr->eRecoN < 0.) { sr->eRecoN = 0.; }
        sr->Ev_reco_numu += sr->eRecoN * scale * pow(sr->eRecoN+0.1, -0.5);
        sr->Ev_reco_nue  += sr->eRecoN * scale * pow(sr->eRecoN+0.1, -0.5);
        sr->RecoHadEnNumu += sr->eRecoN * scale * pow(sr->eRecoN+0.1, -0.5);
        sr->RecoHadEnNue  += sr->eRecoN * scale * pow(sr->eRecoN+0.1, -0.5);
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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_numu,
                  sr->Ev_reco_nue,
                  sr->RecoHadEnNumu,
                  sr->RecoHadEnNue,
                  sr->RecoLepEnNue,
                  sr->eRecoPi0);
      const double scale = .025 * sigma;
      if (sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        if (sr->eRecoPi0 < 0) sr->eRecoPi0 = 0.;
        sr->Ev_reco_numu += sr->eRecoPi0 * pow(sr->eRecoPi0, 0.5) * scale;
        sr->Ev_reco_nue  += sr->eRecoPi0 * pow(sr->eRecoPi0, 0.5) * scale;
        sr->RecoHadEnNumu += sr->eRecoPi0 * pow(sr->eRecoPi0, 0.5) * scale;
        sr->RecoHadEnNue  += sr->eRecoPi0 * pow(sr->eRecoPi0, 0.5) * scale;
        sr->eRecoPi0 += sr->eRecoPi0 * pow(sr->eRecoPi0, 0.5) * scale;
        if (sr->eRecoPi0 < 0) sr->eRecoPi0 = 0.;
        if (sr->isCC==1 && abs(sr->nuPDG)==12) {
          sr->Ev_reco_nue  += sr->RecoLepEnNue*pow(sr->RecoLepEnNue, 0.5)*scale;
          sr->RecoLepEnNue += sr->RecoLepEnNue*pow(sr->RecoLepEnNue, 0.5)*scale;
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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_numu,
                  sr->Ev_reco_nue,
                  sr->RecoHadEnNumu,
                  sr->RecoHadEnNue,
                  sr->RecoLepEnNumu,
                  sr->RecoLepEnNue,
                  sr->eRecoPi0);
      const double scale = .025 * sigma;
      if (sr->isFD) {
        if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
        if (sr->eRecoPi0 < 0) sr->eRecoPi0 = 0.;
        sr->Ev_reco_numu += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
        sr->Ev_reco_nue  += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
        sr->RecoHadEnNumu += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
        sr->RecoHadEnNue  += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
        sr->eRecoPi0 += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
        if (sr->isCC==1 && abs(sr->nuPDG)==12) {
          sr->Ev_reco_nue  += sr->RecoLepEnNue * scale * pow(sr->RecoLepEnNue+0.1, -0.5);
          sr->RecoLepEnNue += sr->RecoLepEnNue * scale * pow(sr->RecoLepEnNue+0.1, -0.5);
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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_numu,
                  sr->RecoLepEnNumu);
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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_nue,
                  sr->Ev_reco_numu,
                  sr->RecoHadEnNumu,
                  sr->RecoHadEnNue,
                  sr->eRecoP,
                  sr->eRecoPip,
                  sr->eRecoPim);
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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_numu,
                  sr->Ev_reco_nue,
                  sr->RecoHadEnNumu,
                  sr->RecoHadEnNue,
                  sr->eRecoN);
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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->RecoLepEnNue,
                  sr->RecoHadEnNue,
                  sr->RecoHadEnNumu,
                  sr->Ev_reco_nue,
                  sr->Ev_reco_numu,
                  sr->eRecoPi0);
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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_numu,
                  sr->RecoLepEnNumu);
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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_nue,
                  sr->Ev_reco_numu,
                  sr->RecoHadEnNumu,
                  sr->RecoHadEnNue,
                  sr->RecoLepEnNue,
                  sr->eRecoPi0);
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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_nue,
                  sr->Ev_reco_numu,
                  sr->RecoHadEnNue,
                  sr->RecoHadEnNumu,
                  sr->eRecoP,
                  sr->eRecoPip,
                  sr->eRecoPim);
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
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Ev_reco_nue,
                  sr->Ev_reco_numu,
                  sr->RecoHadEnNue,
                  sr->RecoHadEnNumu,
                  sr->eRecoN);
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
