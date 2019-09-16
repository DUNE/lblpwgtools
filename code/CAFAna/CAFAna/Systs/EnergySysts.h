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
  // Don't shift muon energies with this
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

  // Vector of energy scale systematics
  struct EnergySystVector: public std::vector<const ISyst*>
  {

  };


  EnergySystVector GetEnergySysts();

}
