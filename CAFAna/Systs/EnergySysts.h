#pragma once

#include "CAFAna/Core/ISyst.h"
#include "StandardRecord/StandardRecord.h"
#include "CAFAna/Core/Utilities.h"

#include "TFile.h"
#include "TH1.h"
#include "TRandom3.h"

#include <cassert>

namespace ana
{
  // FD global energy scale syst
  // Don't shift muon energies with this; muons measured by range/MCS and not deposited energy
  class EnergyScaleFD: public ISyst
  {
  public:
  EnergyScaleFD() : ISyst("EnergyScaleFD", "Global FD Energy Scale Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
                  sr->dune.Ev_reco_nue,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue,
                  sr->dune.RecoLepEnNue);

      double scale = .02 * sigma;
      if (sr->dune.isFD) {
        if (sr->dune.RecoHadEnNue < 0.) sr->dune.RecoHadEnNue = 0.;
        if (sr->dune.Ev_reco_nue < 0.) sr->dune.Ev_reco_nue = 0.;
        sr->dune.Ev_reco_numu  += scale * sr->dune.RecoHadEnNumu;
        sr->dune.Ev_reco_nue   += scale * sr->dune.RecoHadEnNue;
        sr->dune.RecoHadEnNumu *= 1. + scale;
        sr->dune.RecoHadEnNue  *= 1. + scale;
        if ( !(sr->dune.isCC==1 && abs(sr->dune.nuPDG) == 14) ) {
          sr->dune.Ev_reco_nue  += scale * sr->dune.RecoLepEnNue;
          sr->dune.RecoLepEnNue *= 1. + scale;
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
                  sr->dune.Ev_reco_nue,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue,
                  sr->dune.RecoLepEnNue);
        const double scale = .01 * sigma;
        if (sr->dune.isFD) {
          if (sr->dune.RecoHadEnNue < 0.) sr->dune.RecoHadEnNue = 0.;
          sr->dune.Ev_reco_numu  += scale*pow(sr->dune.RecoHadEnNumu, 0.5)*sr->dune.RecoHadEnNumu;
          sr->dune.Ev_reco_nue   += scale*pow(sr->dune.RecoHadEnNue, 0.5)*sr->dune.RecoHadEnNue;
          sr->dune.RecoHadEnNumu += scale*pow(sr->dune.RecoHadEnNumu, 0.5)*sr->dune.RecoHadEnNumu;
          sr->dune.RecoHadEnNue  += scale*pow(sr->dune.RecoHadEnNue, 0.5)*sr->dune.RecoHadEnNue;
          if ( !(sr->dune.isCC==1 && abs(sr->dune.nuPDG) == 14) ) {
            sr->dune.Ev_reco_nue  += scale*pow(sr->dune.RecoLepEnNue, 0.5)*sr->dune.RecoLepEnNue;
            sr->dune.RecoLepEnNue += scale*pow(sr->dune.RecoLepEnNue, 0.5)*sr->dune.RecoLepEnNue;
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
                  sr->dune.Ev_reco_nue,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue,
                  sr->dune.RecoLepEnNue);
      if (sr->dune.isFD) {
        if (sr->dune.RecoHadEnNue < 0.) sr->dune.RecoHadEnNue = 0.;
        const double scale = .02 * sigma;
        sr->dune.Ev_reco_numu += sr->dune.RecoHadEnNumu*scale*pow(sr->dune.RecoHadEnNumu+0.1, -0.5);
        sr->dune.Ev_reco_nue  += sr->dune.RecoHadEnNue*scale*pow(sr->dune.RecoHadEnNue+0.1, -0.5);
        sr->dune.RecoHadEnNumu += sr->dune.RecoHadEnNumu*scale*pow(sr->dune.RecoHadEnNumu+0.1, -0.5);
        sr->dune.RecoHadEnNue  += sr->dune.RecoHadEnNue*scale*pow(sr->dune.RecoHadEnNue+0.1, -0.5);
        if ( !(sr->dune.isCC==1 && abs(sr->dune.nuPDG) == 14) ) {
          sr->dune.Ev_reco_nue  += sr->dune.RecoLepEnNue * scale * pow(sr->dune.RecoLepEnNue+0.1, -0.5);
          sr->dune.RecoLepEnNue += sr->dune.RecoLepEnNue * scale * pow(sr->dune.RecoLepEnNue+0.1, -0.5);
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
                  sr->dune.Ev_reco_nue,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue,
                  sr->dune.eRecoP,
                  sr->dune.eRecoPim,
                  sr->dune.eRecoPip);
      const double scale = .05 * sigma;
      if (sr->dune.isFD) {
        if (sr->dune.RecoHadEnNue < 0.) sr->dune.RecoHadEnNue = 0.;
        if (sr->dune.eRecoP < 0) sr->dune.eRecoP = 0.;
        if (sr->dune.eRecoPim < 0) sr->dune.eRecoPim = 0.;
        if (sr->dune.eRecoPip < 0) sr->dune.eRecoPip = 0.;
        double sumE = sr->dune.eRecoP + sr->dune.eRecoPip + sr->dune.eRecoPim;
        sr->dune.eRecoP   += sr->dune.eRecoP * scale * pow(sumE, 0.5);
        sr->dune.eRecoPim += sr->dune.eRecoPim * scale * pow(sumE, 0.5);
        sr->dune.eRecoPip += sr->dune.eRecoPip * scale * pow(sumE, 0.5);
        sr->dune.Ev_reco_numu += sumE * scale * pow(sumE, 0.5);
        sr->dune.Ev_reco_nue  += sumE * scale * pow(sumE, 0.5);
        sr->dune.RecoHadEnNumu += sumE * scale * pow(sumE, 0.5);
        sr->dune.RecoHadEnNue  += sumE * scale * pow(sumE, 0.5);
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
                  sr->dune.Ev_reco_nue,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue,
                  sr->dune.eRecoP,
                  sr->dune.eRecoPim,
                  sr->dune.eRecoPip);
      const double scale = .05 * sigma;
      if (sr->dune.isFD) {
        if (sr->dune.RecoHadEnNue < 0.) sr->dune.RecoHadEnNue = 0.;
        if (sr->dune.eRecoP < 0) sr->dune.eRecoP = 0.;
        if (sr->dune.eRecoPim < 0) sr->dune.eRecoPim = 0.;
        if (sr->dune.eRecoPip < 0) sr->dune.eRecoPip = 0.;
        double sumE = sr->dune.eRecoP + sr->dune.eRecoPip + sr->dune.eRecoPim;
        sr->dune.Ev_reco_numu += sumE * scale * pow(sumE+0.1, -0.5);
        sr->dune.Ev_reco_nue  += sumE * scale * pow(sumE+0.1, -0.5);
        sr->dune.eRecoP   += sr->dune.eRecoP * scale * pow(sumE+0.1, -0.5);
        sr->dune.eRecoPim += sr->dune.eRecoPim * scale * pow(sumE+0.1, -0.5);
        sr->dune.eRecoPip += sr->dune.eRecoPip * scale * pow(sumE+0.1, -0.5);
        sr->dune.RecoHadEnNumu += sumE * scale * pow(sumE+0.1, -0.5);
        sr->dune.RecoHadEnNue  += sumE * scale * pow(sumE+0.1, -0.5);
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
                  sr->dune.RecoLepEnNumu);
      if (sr->dune.isFD && sr->dune.isCC==1 && abs(sr->dune.nuPDG)==14) {
        const double scale = .005 * sigma;
        sr->dune.Ev_reco_numu  += sr->dune.RecoLepEnNumu * pow(sr->dune.RecoLepEnNumu, 0.5) * scale;
        sr->dune.RecoLepEnNumu += sr->dune.RecoLepEnNumu * pow(sr->dune.RecoLepEnNumu, 0.5) * scale;
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
                  sr->dune.RecoLepEnNumu);
      if (sr->dune.isFD && sr->dune.isCC==1 && abs(sr->dune.nuPDG)==14) {
        const double scale = .02 * sigma;
        sr->dune.Ev_reco_numu  += sr->dune.RecoLepEnNumu * scale * pow(sr->dune.RecoLepEnNumu+0.1, -0.5);
        sr->dune.RecoLepEnNumu += sr->dune.RecoLepEnNumu * scale * pow(sr->dune.RecoLepEnNumu+0.1, -0.5);
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
                  sr->dune.Ev_reco_nue,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue,
                  sr->dune.eRecoN);
      const double scale = .3 * sigma;
      if (sr->dune.isFD) {
        if (sr->dune.eRecoN < 0.) { sr->dune.eRecoN = 0.; }
        sr->dune.Ev_reco_numu += sr->dune.eRecoN * pow(sr->dune.eRecoN, 0.5) * scale;
        sr->dune.Ev_reco_nue  += sr->dune.eRecoN * pow(sr->dune.eRecoN, 0.5) * scale;
        sr->dune.RecoHadEnNumu += sr->dune.eRecoN * pow(sr->dune.eRecoN, 0.5) * scale;
        sr->dune.RecoHadEnNue  += sr->dune.eRecoN * pow(sr->dune.eRecoN, 0.5) * scale;
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
                  sr->dune.Ev_reco_nue,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue,
                  sr->dune.eRecoN);
      const double scale = .3 * sigma;
      if (sr->dune.isFD) {
        if (sr->dune.eRecoN < 0.) { sr->dune.eRecoN = 0.; }
        sr->dune.Ev_reco_numu += sr->dune.eRecoN * scale * pow(sr->dune.eRecoN+0.1, -0.5);
        sr->dune.Ev_reco_nue  += sr->dune.eRecoN * scale * pow(sr->dune.eRecoN+0.1, -0.5);
        sr->dune.RecoHadEnNumu += sr->dune.eRecoN * scale * pow(sr->dune.eRecoN+0.1, -0.5);
        sr->dune.RecoHadEnNue  += sr->dune.eRecoN * scale * pow(sr->dune.eRecoN+0.1, -0.5);
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
                  sr->dune.Ev_reco_nue,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue,
                  sr->dune.RecoLepEnNue,
                  sr->dune.eRecoPi0);
      const double scale = .025 * sigma;
      if (sr->dune.isFD) {
        if (sr->dune.RecoHadEnNue < 0.) sr->dune.RecoHadEnNue = 0.;
        if (sr->dune.eRecoPi0 < 0) sr->dune.eRecoPi0 = 0.;
        sr->dune.Ev_reco_numu += sr->dune.eRecoPi0 * pow(sr->dune.eRecoPi0, 0.5) * scale;
        sr->dune.Ev_reco_nue  += sr->dune.eRecoPi0 * pow(sr->dune.eRecoPi0, 0.5) * scale;
        sr->dune.RecoHadEnNumu += sr->dune.eRecoPi0 * pow(sr->dune.eRecoPi0, 0.5) * scale;
        sr->dune.RecoHadEnNue  += sr->dune.eRecoPi0 * pow(sr->dune.eRecoPi0, 0.5) * scale;
        sr->dune.eRecoPi0 += sr->dune.eRecoPi0 * pow(sr->dune.eRecoPi0, 0.5) * scale;
        if (sr->dune.eRecoPi0 < 0) sr->dune.eRecoPi0 = 0.;
        if (sr->dune.isCC==1 && abs(sr->dune.nuPDG)==12) {
          sr->dune.Ev_reco_nue  += sr->dune.RecoLepEnNue*pow(sr->dune.RecoLepEnNue, 0.5)*scale;
          sr->dune.RecoLepEnNue += sr->dune.RecoLepEnNue*pow(sr->dune.RecoLepEnNue, 0.5)*scale;
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
                  sr->dune.Ev_reco_nue,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue,
                  sr->dune.RecoLepEnNumu,
                  sr->dune.RecoLepEnNue,
                  sr->dune.eRecoPi0);
      const double scale = .025 * sigma;
      if (sr->dune.isFD) {
        if (sr->dune.RecoHadEnNue < 0.) sr->dune.RecoHadEnNue = 0.;
        if (sr->dune.eRecoPi0 < 0) sr->dune.eRecoPi0 = 0.;
        sr->dune.Ev_reco_numu += sr->dune.eRecoPi0 * scale * pow(sr->dune.eRecoPi0+0.1, -0.5);
        sr->dune.Ev_reco_nue  += sr->dune.eRecoPi0 * scale * pow(sr->dune.eRecoPi0+0.1, -0.5);
        sr->dune.RecoHadEnNumu += sr->dune.eRecoPi0 * scale * pow(sr->dune.eRecoPi0+0.1, -0.5);
        sr->dune.RecoHadEnNue  += sr->dune.eRecoPi0 * scale * pow(sr->dune.eRecoPi0+0.1, -0.5);
        sr->dune.eRecoPi0 += sr->dune.eRecoPi0 * scale * pow(sr->dune.eRecoPi0+0.1, -0.5);
        if (sr->dune.isCC==1 && abs(sr->dune.nuPDG)==12) {
          sr->dune.Ev_reco_nue  += sr->dune.RecoLepEnNue * scale * pow(sr->dune.RecoLepEnNue+0.1, -0.5);
          sr->dune.RecoLepEnNue += sr->dune.RecoLepEnNue * scale * pow(sr->dune.RecoLepEnNue+0.1, -0.5);
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
                  sr->dune.RecoLepEnNumu);
      const double scale = .02 * sigma;
      // Checks if FD  and select only CC muon neutrino events
      if(sr->dune.isFD && abs(sr->dune.nuPDG) == 14 && sr->dune.isCC == 1){
        sr->dune.Ev_reco_numu   += sr->dune.RecoLepEnNumu * scale;
        sr->dune.RecoLepEnNumu  *= 1. + scale;
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_nue,
                  sr->dune.Ev_reco_numu,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue,
                  sr->dune.eRecoP,
                  sr->dune.eRecoPip,
                  sr->dune.eRecoPim);
      const double scale = .05 * sigma;
      if(sr->dune.isFD) {
        if (sr->dune.RecoHadEnNue < 0.) sr->dune.RecoHadEnNue = 0.;
        if (sr->dune.eRecoP < 0) sr->dune.eRecoP = 0.;
        if (sr->dune.eRecoPim < 0) sr->dune.eRecoPim = 0.;
        if (sr->dune.eRecoPip < 0) sr->dune.eRecoPip = 0.;
        const double sumE = sr->dune.eRecoP + sr->dune.eRecoPim + sr->dune.eRecoPip;
        sr->dune.eRecoP   += sr->dune.eRecoP * scale;
        sr->dune.eRecoPim += sr->dune.eRecoPim * scale;
        sr->dune.eRecoPip += sr->dune.eRecoPip * scale;
        sr->dune.Ev_reco_numu += sumE * scale;
        sr->dune.Ev_reco_nue  += sumE * scale;
        sr->dune.RecoHadEnNumu += sumE * scale;
        sr->dune.RecoHadEnNue  += sumE * scale;
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
                  sr->dune.Ev_reco_nue,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue,
                  sr->dune.eRecoN);
      const double scale = .20 * sigma;
      if(sr->dune.isFD) {
        if (sr->dune.RecoHadEnNue < 0.) sr->dune.RecoHadEnNue = 0.;
        if (sr->dune.eRecoN < 0.) sr->dune.eRecoN = 0.;
        sr->dune.Ev_reco_numu  += sr->dune.eRecoN * scale;
        sr->dune.Ev_reco_nue   += sr->dune.eRecoN * scale;
        sr->dune.RecoHadEnNumu += sr->dune.eRecoN * scale;
        sr->dune.RecoHadEnNue  += sr->dune.eRecoN * scale;
        sr->dune.eRecoN += sr->dune.eRecoN * scale;
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.RecoLepEnNue,
                  sr->dune.RecoHadEnNue,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.Ev_reco_nue,
                  sr->dune.Ev_reco_numu,
                  sr->dune.eRecoPi0);
      const double scale = 0.025 * sigma;
      if (sr->dune.isFD) {
        if (sr->dune.RecoHadEnNue < 0.) sr->dune.RecoHadEnNue = 0.;
        if (sr->dune.eRecoPi0 < 0.) sr->dune.eRecoPi0 = 0.;
        sr->dune.Ev_reco_nue  += sr->dune.eRecoPi0 * scale;
        sr->dune.Ev_reco_numu += sr->dune.eRecoPi0 * scale;
        sr->dune.RecoHadEnNue  += sr->dune.eRecoPi0 * scale;
        sr->dune.RecoHadEnNumu += sr->dune.eRecoPi0 * scale;
        sr->dune.eRecoPi0 += sr->dune.eRecoPi0 * scale;
        if (sr->dune.isCC && abs(sr->dune.nuPDG) == 12) {
          sr->dune.Ev_reco_nue  += sr->dune.RecoLepEnNue * scale;
          sr->dune.RecoLepEnNue += sr->dune.RecoLepEnNue * scale;
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
                  sr->dune.RecoLepEnNumu);
      const double scale = .02*sigma;
      // FD charged current numus only
      if (sr->dune.isFD && sr->dune.isCC && abs(sr->dune.nuPDG)==14) {
        sr->dune.Ev_reco_numu  += (sr->dune.LepE - sr->dune.RecoLepEnNumu) * scale;
        sr->dune.RecoLepEnNumu += (sr->dune.LepE - sr->dune.RecoLepEnNumu) * scale;
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_nue,
                  sr->dune.Ev_reco_numu,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue,
                  sr->dune.RecoLepEnNue,
                  sr->dune.eRecoPi0);
      const double scale = .02*sigma;
      if (sr->dune.isFD){
        if (sr->dune.RecoHadEnNue < 0.) sr->dune.RecoHadEnNue = 0.;
        if (sr->dune.eRecoPi0 < 0.) sr->dune.eRecoPi0 = 0.;
        sr->dune.Ev_reco_nue  += (sr->dune.ePi0 - sr->dune.eRecoPi0) * scale;
        sr->dune.Ev_reco_numu += (sr->dune.ePi0 - sr->dune.eRecoPi0) * scale;
        sr->dune.RecoHadEnNumu += (sr->dune.ePi0 - sr->dune.eRecoPi0) * scale;
        sr->dune.RecoHadEnNue  += (sr->dune.ePi0 - sr->dune.eRecoPi0) * scale;
        sr->dune.eRecoPi0     += (sr->dune.ePi0 - sr->dune.eRecoPi0) * scale;
        if (sr->dune.isCC && abs(sr->dune.nuPDG)==12) {
          sr->dune.Ev_reco_nue  += (sr->dune.LepE - sr->dune.RecoLepEnNue) * scale;
          sr->dune.RecoLepEnNue += (sr->dune.LepE - sr->dune.RecoLepEnNue) * scale;
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_nue,
                  sr->dune.Ev_reco_numu,
                  sr->dune.RecoHadEnNue,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.eRecoP,
                  sr->dune.eRecoPip,
                  sr->dune.eRecoPim);
      const double scale = .02*sigma;
      if (sr->dune.isFD) {
        if (sr->dune.RecoHadEnNue < 0.) sr->dune.RecoHadEnNue = 0.;
        if (sr->dune.eRecoP < 0.) sr->dune.eRecoP = 0.;
        if (sr->dune.eRecoPip < 0.) sr->dune.eRecoPip = 0.;
        if (sr->dune.eRecoPim < 0.) sr->dune.eRecoPim = 0.;
        const double trueSum = sr->dune.ePip + sr->dune.ePim + sr->dune.eP;
        const double recoSum = sr->dune.eRecoPip + sr->dune.eRecoPim + sr->dune.eRecoP;
        sr->dune.Ev_reco_nue   += (trueSum - recoSum) * scale;
        sr->dune.Ev_reco_numu  += (trueSum - recoSum) * scale;
        sr->dune.RecoHadEnNue  += (trueSum - recoSum) * scale;
        sr->dune.RecoHadEnNumu += (trueSum - recoSum) * scale;
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
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_nue,
                  sr->dune.Ev_reco_numu,
                  sr->dune.RecoHadEnNue,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.eRecoN);
      const double scale = .1*sigma;
      // FD charged current numus only
      if (sr->dune.isFD) {
        if (sr->dune.RecoHadEnNue < 0.) sr->dune.RecoHadEnNue = 0.;
        if (sr->dune.eRecoN < 0.) sr->dune.eRecoN = 0.;
        sr->dune.Ev_reco_nue   += (sr->dune.eN - sr->dune.eRecoN) * scale;
        sr->dune.Ev_reco_numu  += (sr->dune.eN - sr->dune.eRecoN) * scale;
        sr->dune.RecoHadEnNue  += (sr->dune.eN - sr->dune.eRecoN) * scale;
        sr->dune.RecoHadEnNumu += (sr->dune.eN - sr->dune.eRecoN) * scale;
        sr->dune.eRecoN += (sr->dune.eN - sr->dune.eRecoN) * scale;
      }
    }
  };

  extern const NResFD kNResFD;

  //---------------------------------------------------------------------------
  //-----------NEAR DETECTOR---------------------------------------------------
  //---------------------------------------------------------------------------
  // ND-LAr global energy scale syst
  // Don't shift muon energies with this; muons measured by range and not deposited energy
  class EnergyScaleND: public ISyst
  {
  public:
  EnergyScaleND() : ISyst("EnergyScaleND", "Global ND Energy Scale Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco);

      double scale = .02 * sigma;
      if (!sr->dune.isFD) {
        if (sr->dune.Ev_reco < 0.) sr->dune.Ev_reco = 0.;
        if (sr->dune.Elep_reco < 0.) sr->dune.Elep_reco = 0.;
        double ehad_reco = sr->dune.Ev_reco - sr->dune.Elep_reco;
        // Shift the lepton energy only for electrons
        if (abs(sr->dune.LepPDG) == 11) {
          sr->dune.Ev_reco *= (1. + scale);
          sr->dune.Elep_reco *= (1. + scale);
        } else { // scale only the hadronic component of the energy
          sr->dune.Ev_reco += scale * ehad_reco; // shift the hadronic energy
        }
      }
    }
  };

  extern const EnergyScaleND kEnergyScaleND;

  // Total energy scale shape systematics
  class UncorrNDTotSqrt: public ISyst
  {
  public:
  UncorrNDTotSqrt() : ISyst("UncorrNDTotSqrt", "Uncorrelated ND Sqrt Total Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco);

      const double scale = .01 * sigma;
      if (!sr->dune.isFD) {
        if (sr->dune.Ev_reco < 0.) sr->dune.Ev_reco = 0.;
        if (sr->dune.Elep_reco < 0.) sr->dune.Elep_reco = 0.;
        double ehad_reco = sr->dune.Ev_reco - sr->dune.Elep_reco;
        // Shift the lepton energy only for electrons
        if (abs(sr->dune.LepPDG) == 11) {
          const double edep_scale = scale*pow(sr->dune.Ev_reco,0.5);
          sr->dune.Ev_reco *= (1. + edep_scale);
          sr->dune.Elep_reco *= (1. + edep_scale);
        } else { // scale only the hadronic component of the energy
          sr->dune.Ev_reco += scale * pow(ehad_reco,0.5); // shift the hadronic energy
        }
      }       
    }
  };
  extern const UncorrNDTotSqrt kUncorrNDTotSqrt;

  class UncorrNDTotInvSqrt: public ISyst
  {
  public:
  UncorrNDTotInvSqrt() : ISyst("UncorrNDTotInvSqrt", "Uncorrelated ND Inverse Sqrt Total Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco);

      const double scale = .02 * sigma;
      if (!sr->dune.isFD) {
        if (sr->dune.Ev_reco < 0.) sr->dune.Ev_reco = 0.;
        if (sr->dune.Elep_reco < 0.) sr->dune.Elep_reco = 0.;
        double ehad_reco = sr->dune.Ev_reco - sr->dune.Elep_reco;
        // Shift the lepton energy only for electrons
        if (abs(sr->dune.LepPDG) == 11) {
          const double edep_scale = scale*pow(sr->dune.Ev_reco+0.1,-0.5);
          sr->dune.Ev_reco *= (1. + edep_scale);
          sr->dune.Elep_reco *= (1. + edep_scale);
        } else { // scale only the hadronic component of the energy
          sr->dune.Ev_reco += scale * pow(ehad_reco+0.1,-0.5); // shift the hadronic energy
        }
      }       
    }
  };
  extern const UncorrNDTotInvSqrt kUncorrNDTotInvSqrt;

  // Muons; split contained vs spectrometer
  class EScaleMuLArND: public ISyst
  {
  public:
  EScaleMuLArND() : ISyst("EScaleMuLArND", "Muon Energy Scale LAr Near Detector") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco);
      const double scale = .02 * sigma;
      if(!sr->dune.isFD && abs(sr->dune.LepPDG) == 13 && (sr->dune.muon_contained || sr->dune.muon_ecal)) {
        sr->dune.Ev_reco   += sr->dune.Elep_reco * scale;
        sr->dune.Elep_reco *= (1. + scale);
      }
    }
  };
  extern const EScaleMuLArND kEScaleMuLArND;

  class UncorrNDMuLArSqrt: public ISyst
  {
  public:
  UncorrNDMuLArSqrt() : ISyst("UncorrNDMuLArSqrt", "Uncorrelated ND LAr Sqrt Muon Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco);
      if (!sr->dune.isFD && abs(sr->dune.LepPDG) == 13 && (sr->dune.muon_contained || sr->dune.muon_ecal)) {
        const double scale = .005 * sigma;
        sr->dune.Ev_reco  += sr->dune.Elep_reco * pow(sr->dune.Elep_reco, 0.5) * scale;
        sr->dune.Elep_reco += sr->dune.Elep_reco * pow(sr->dune.Elep_reco, 0.5) * scale;
      }
    }
  };
  extern const UncorrNDMuLArSqrt kUncorrNDMuLArSqrt;

  class UncorrNDMuLArInvSqrt: public ISyst
  {
  public:
  UncorrNDMuLArInvSqrt() : ISyst("UncorrNDMuLArInvSqrt", "Uncorrelated ND LAr Inv Sqrt Muon Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco);
      if (!sr->dune.isFD && abs(sr->dune.LepPDG) == 13 && (sr->dune.muon_contained || sr->dune.muon_ecal)) {
        const double scale = .02 * sigma;
        sr->dune.Ev_reco  += sr->dune.Elep_reco * scale * pow(sr->dune.Elep_reco+0.1, -0.5);
        sr->dune.Elep_reco += sr->dune.Elep_reco * scale * pow(sr->dune.Elep_reco+0.1, -0.5);
      }
    }
  };
  extern const UncorrNDMuLArInvSqrt kUncorrNDMuLArInvSqrt;

  // This is probably different for D1ND and GAr
  class EScaleMuSpectND: public ISyst
  {
  public:
  EScaleMuSpectND() : ISyst("EScaleMuSpectND", "Muon Energy Scale Spectrometer Near Detector") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco);
      const double scale = .02 * sigma;
      if(!sr->dune.isFD && abs(sr->dune.LepPDG) == 13 && sr->dune.muon_tracker) {
        sr->dune.Ev_reco   += sr->dune.Elep_reco * scale;
        sr->dune.Elep_reco *= (1. + scale);
      }
    }
  };
  extern const EScaleMuLArND kEScaleMuLArND;

  class UncorrNDMuSpectSqrt: public ISyst
  {
  public:
  UncorrNDMuSpectSqrt() : ISyst("UncorrNDMuSpectSqrt", "Uncorrelated ND Spectrometer Sqrt Muon Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco);
      if(!sr->dune.isFD && abs(sr->dune.LepPDG) == 13 && sr->dune.muon_tracker) {
        const double scale = .005 * sigma;
        sr->dune.Ev_reco  += sr->dune.Elep_reco * pow(sr->dune.Elep_reco, 0.5) * scale;
        sr->dune.Elep_reco += sr->dune.Elep_reco * pow(sr->dune.Elep_reco, 0.5) * scale;
      }
    }
  };
  extern const UncorrNDMuSpectSqrt kUncorrNDMuSpectSqrt;

  class UncorrNDMuSpectInvSqrt: public ISyst
  {
  public:
  UncorrNDMuSpectInvSqrt() : ISyst("UncorrNDMuSpectInvSqrt", "Uncorrelated ND Spectrometer Inv Sqrt Muon Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco);
      if(!sr->dune.isFD && abs(sr->dune.LepPDG) == 13 && sr->dune.muon_tracker) {
        const double scale = .02 * sigma;
        sr->dune.Ev_reco  += sr->dune.Elep_reco * scale * pow(sr->dune.Elep_reco+0.1, -0.5);
        sr->dune.Elep_reco += sr->dune.Elep_reco * scale * pow(sr->dune.Elep_reco+0.1, -0.5);
      }
    }
  };
  extern const UncorrNDMuSpectInvSqrt kUncorrNDMuSpectInvSqrt;

  // Charged hadrons
  class ChargedHadUncorrND: public ISyst
  {
  public:
  ChargedHadUncorrND() : ISyst("ChargedHadUncorrND", "Charged Hadron Uncorrelated ND Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.eRecoP,
                  sr->dune.eRecoPip,
                  sr->dune.eRecoPim);
      const double scale = .05 * sigma;
      if(!sr->dune.isFD) {
        if (sr->dune.Ev_reco < 0.) sr->dune.Ev_reco = 0.;
        if (sr->dune.eRecoP < 0)   sr->dune.eRecoP = 0.;
        if (sr->dune.eRecoPim < 0) sr->dune.eRecoPim = 0.;
        if (sr->dune.eRecoPip < 0) sr->dune.eRecoPip = 0.;
        const double sumE = sr->dune.eRecoP + sr->dune.eRecoPim + sr->dune.eRecoPip;
        sr->dune.eRecoP   += sr->dune.eRecoP * scale;
        sr->dune.eRecoPim += sr->dune.eRecoPim * scale;
        sr->dune.eRecoPip += sr->dune.eRecoPip * scale;
        sr->dune.Ev_reco  += sumE * scale;
      }
    }
  };
  extern const ChargedHadUncorrFD kChargedHadUncorrFD;


  class UncorrNDHadSqrt: public ISyst
  {
  public:
  UncorrNDHadSqrt() : ISyst("UncorrNDHadSqrt", "Uncorrelated ND Sqrt Hadron Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.eRecoP,
                  sr->dune.eRecoPim,
                  sr->dune.eRecoPip);
      const double scale = .05 * sigma;
      if (!sr->dune.isFD) {
        if (sr->dune.Ev_reco < 0.) sr->dune.Ev_reco = 0.;
        if (sr->dune.eRecoP < 0)   sr->dune.eRecoP = 0.;
        if (sr->dune.eRecoPim < 0) sr->dune.eRecoPim = 0.;
        if (sr->dune.eRecoPip < 0) sr->dune.eRecoPip = 0.;
        double sumE = sr->dune.eRecoP + sr->dune.eRecoPip + sr->dune.eRecoPim;
        sr->dune.eRecoP   += sr->dune.eRecoP * scale * pow(sumE, 0.5);
        sr->dune.eRecoPim += sr->dune.eRecoPim * scale * pow(sumE, 0.5);
        sr->dune.eRecoPip += sr->dune.eRecoPip * scale * pow(sumE, 0.5);
        sr->dune.Ev_reco += sumE * scale * pow(sumE, 0.5);
      }
    }
  };
  extern const UncorrNDHadSqrt kUncorrNDHadSqrt;

  class UncorrNDHadInvSqrt: public ISyst
  {
  public:
  UncorrNDHadInvSqrt() : ISyst("UncorrNDHadInvSqrt", "Uncorrelated ND Inv Sqrt Hadron Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.eRecoP,
                  sr->dune.eRecoPim,
                  sr->dune.eRecoPip);
      const double scale = .05 * sigma;
      if (sr->dune.isFD) {
        if (sr->dune.Ev_reco < 0.) sr->dune.RecoHadEnNue = 0.;
        if (sr->dune.eRecoP < 0) sr->dune.eRecoP = 0.;
        if (sr->dune.eRecoPim < 0) sr->dune.eRecoPim = 0.;
        if (sr->dune.eRecoPip < 0) sr->dune.eRecoPip = 0.;
        double sumE = sr->dune.eRecoP + sr->dune.eRecoPip + sr->dune.eRecoPim;
        sr->dune.Ev_reco += sumE * scale * pow(sumE+0.1, -0.5);
        sr->dune.eRecoP   += sr->dune.eRecoP * scale * pow(sumE+0.1, -0.5);
        sr->dune.eRecoPim += sr->dune.eRecoPim * scale * pow(sumE+0.1, -0.5);
        sr->dune.eRecoPip += sr->dune.eRecoPip * scale * pow(sumE+0.1, -0.5);
      }
    }
  };
  extern const UncorrNDHadInvSqrt kUncorrNDHadInvSqrt;

  //------------------------------------------------------------------------------------
  // Electromagnetic
  class EMUncorrND: public ISyst
  {
  public:
  EMUncorrND() : ISyst("EMUncorrND", "Electromagnetic shower ND Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco,
                  sr->dune.eRecoPi0);
      const double scale = 0.025 * sigma;
      if (!sr->dune.isFD) {
        if (sr->dune.Ev_reco < 0.) sr->dune.Ev_reco = 0.;
        if (sr->dune.Elep_reco < 0.) sr->dune.Elep_reco = 0.;
        if (sr->dune.eRecoPi0 < 0.) sr->dune.eRecoPi0 = 0.;
        sr->dune.Ev_reco  += sr->dune.eRecoPi0 * scale;
        sr->dune.eRecoPi0 += sr->dune.eRecoPi0 * scale;
        if (abs(sr->dune.LepPDG) == 11) {
          sr->dune.Ev_reco  += sr->dune.Elep_reco * scale;
          sr->dune.Elep_reco += sr->dune.Elep_reco * scale;
        }
      }
    }
  };
  extern const EMUncorrND kEMUncorrND;

  class UncorrNDEMSqrt: public ISyst
  {
  public:
  UncorrNDEMSqrt() : ISyst("UncorrNDEMSqrt", "Uncorrelated ND Sqrt EM Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco,
                  sr->dune.eRecoPi0);
      const double scale = .025 * sigma;
      if (!sr->dune.isFD) {
        if (sr->dune.Ev_reco < 0.) sr->dune.Ev_reco = 0.;
        if (sr->dune.Elep_reco < 0.) sr->dune.Elep_reco = 0.;
        sr->dune.Ev_reco  += sr->dune.eRecoPi0 * pow(sr->dune.eRecoPi0, 0.5) * scale;
        sr->dune.eRecoPi0 += sr->dune.eRecoPi0 * pow(sr->dune.eRecoPi0, 0.5) * scale;
        if (sr->dune.eRecoPi0 < 0) sr->dune.eRecoPi0 = 0.;
        if (abs(sr->dune.LepPDG) == 11) {
          sr->dune.Ev_reco  += sr->dune.Elep_reco*pow(sr->dune.Elep_reco, 0.5)*scale;
          sr->dune.Elep_reco += sr->dune.Elep_reco*pow(sr->dune.Elep_reco, 0.5)*scale;
        }
      }
    }
  };
  extern const UncorrNDEMSqrt kUncorrNDEMSqrt;

  class UncorrNDEMInvSqrt: public ISyst
  {
  public:
  UncorrNDEMInvSqrt() : ISyst("UncorrNDEMInvSqrt", "Uncorrelated ND Inv Sqrt EM Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco,
                  sr->dune.eRecoPi0);
      const double scale = .025 * sigma;
      if (!sr->dune.isFD) {
        if (sr->dune.Ev_reco < 0.) sr->dune.Ev_reco = 0.;
        if (sr->dune.Elep_reco < 0.) sr->dune.Elep_reco = 0.;
        sr->dune.Ev_reco  += sr->dune.eRecoPi0 * scale * pow(sr->dune.eRecoPi0+0.1, -0.5);
        sr->dune.eRecoPi0 += sr->dune.eRecoPi0 * scale * pow(sr->dune.eRecoPi0+0.1, -0.5);
        if (abs(sr->dune.LepPDG) == 11) {
          sr->dune.Ev_reco  += sr->dune.Elep_reco * scale * pow(sr->dune.Elep_reco+0.1, -0.5);
          sr->dune.Elep_reco += sr->dune.Elep_reco * scale * pow(sr->dune.Elep_reco+0.1, -0.5);
        }
      }
    }
  };
  extern const UncorrNDEMInvSqrt kUncorrNDEMInvSqrt;



  //------------------------------------------------------------------------------------
  //Neutrons are not the best
  class NUncorrND: public ISyst
  {
  public:
  NUncorrND() : ISyst("NUncorrND", "Neutron Energy Scale ND") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.eRecoN);
      const double scale = .20 * sigma;
      if(!sr->dune.isFD) {
        if (sr->dune.Ev_reco < 0.) sr->dune.Ev_reco = 0.;
        if (sr->dune.eRecoN < 0.)  sr->dune.eRecoN = 0.;
        sr->dune.Ev_reco += sr->dune.eRecoN * scale;
        sr->dune.eRecoN  += sr->dune.eRecoN * scale;
      }
    }
  };
  extern const NUncorrND kNUncorrND;

  class UncorrNDNSqrt: public ISyst
  {
  public:
  UncorrNDNSqrt() : ISyst("UncorrNDNSqrt", "Uncorrelated ND Sqrt Neutron Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.eRecoN);
      const double scale = .3 * sigma;
      if (!sr->dune.isFD) {
        if (sr->dune.eRecoN < 0.) sr->dune.eRecoN = 0.;
        sr->dune.Ev_reco += sr->dune.eRecoN * pow(sr->dune.eRecoN, 0.5) * scale;
        sr->dune.eRecoN  += sr->dune.eRecoN * pow(sr->dune.eRecoN, 0.5) * scale;
      }
    }
  };
  extern const UncorrNDNSqrt kUncorrNDNSqrt;

  class UncorrNDNInvSqrt: public ISyst
  {
  public:
  UncorrNDNInvSqrt() : ISyst("UncorrNDNInvSqrt", "Uncorrelated ND Inv Sqrt Neutron Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.eRecoN);
      const double scale = .3 * sigma;
      if (!sr->dune.isFD) {
        if (sr->dune.eRecoN < 0.) sr->dune.eRecoN = 0.;
        sr->dune.Ev_reco += sr->dune.eRecoN * scale * pow(sr->dune.eRecoN+0.1, -0.5);
        sr->dune.eRecoN  += sr->dune.eRecoN * scale * pow(sr->dune.eRecoN+0.1, -0.5);
      }
    }
  };
  extern const UncorrNDNInvSqrt kUncorrNDNInvSqrt;


  // ND resolution systematics
  class MuonResND: public ISyst
  {
  public:
  MuonResND() : ISyst("MuonResND", "Muon Near Detector Resolution Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco);
      const double scale = .02*sigma;
      if (!sr->dune.isFD && abs(sr->dune.LepPDG)==13) {
        sr->dune.Ev_reco  += (sr->dune.LepE - sr->dune.Elep_reco) * scale;
        sr->dune.Elep_reco += (sr->dune.LepE - sr->dune.Elep_reco) * scale;
      }
    }
  };

  extern const MuonResND kMuonResND;

  // Electron/pi0 energy resolution
  class EMResND: public ISyst
  {
  public:
  EMResND() : ISyst("EMResND", "EM shower Near Detector Resolution Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco,
                  sr->dune.eRecoPi0);
      const double scale = .02*sigma;
      if (!sr->dune.isFD){
        if (sr->dune.eRecoPi0 < 0.) sr->dune.eRecoPi0 = 0.;
        sr->dune.Ev_reco  += (sr->dune.ePi0 - sr->dune.eRecoPi0) * scale;
        sr->dune.eRecoPi0 += (sr->dune.ePi0 - sr->dune.eRecoPi0) * scale;
        if (abs(sr->dune.LepPDG)==11) {
          sr->dune.Ev_reco  += (sr->dune.LepE - sr->dune.Elep_reco) * scale;
          sr->dune.Elep_reco += (sr->dune.LepE - sr->dune.Elep_reco) * scale;
        }
      }
    }
  };

  extern const EMResND kEMResND;

  // Charged hadron energy resolution
  class ChargedHadResND: public ISyst
  {
  public:
  ChargedHadResND() : ISyst("ChargedHadResND", "Charged Hadron Near Detector Resolution Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.eRecoP,
                  sr->dune.eRecoPip,
                  sr->dune.eRecoPim);
      const double scale = .02*sigma;
      if (!sr->dune.isFD) {
        if (sr->dune.eRecoP < 0.) sr->dune.eRecoP = 0.;
        if (sr->dune.eRecoPip < 0.) sr->dune.eRecoPip = 0.;
        if (sr->dune.eRecoPim < 0.) sr->dune.eRecoPim = 0.;
        const double trueSum = sr->dune.ePip + sr->dune.ePim + sr->dune.eP;
        const double recoSum = sr->dune.eRecoPip + sr->dune.eRecoPim + sr->dune.eRecoP;
        sr->dune.Ev_reco   += (trueSum - recoSum) * scale;
        sr->dune.eRecoP += (sr->dune.eP - sr->dune.eRecoP) * scale;
        sr->dune.eRecoPip += (sr->dune.ePip - sr->dune.eRecoPip) * scale;
        sr->dune.eRecoPim += (sr->dune.ePim - sr->dune.eRecoPim) * scale;
      }
    }
  };

  extern const ChargedHadResND kChargedHadResND;

  // Neutron energy resolution
  class NResND: public ISyst
  {
  public:
  NResND() : ISyst("NResND", "Neutron Near Detector Resolution Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.eRecoN);
      const double scale = .1*sigma;
      if (!sr->dune.isFD) {
        if (sr->dune.eRecoN < 0.) sr->dune.eRecoN = 0.;
        sr->dune.Ev_reco += (sr->dune.eN - sr->dune.eRecoN) * scale;
        sr->dune.eRecoN += (sr->dune.eN - sr->dune.eRecoN) * scale;
      }
    }
  };
  extern const NResND kNResND;

  // Vector of energy scale systematics
  struct EnergySystVector: public std::vector<const ISyst*>
  {

  };


  EnergySystVector GetEnergySysts();
  EnergySystVector GetNDEnergySysts();
}
