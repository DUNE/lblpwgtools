#pragma once

#include "CAFAna/Core/ISyst.h"
#include "StandardRecord/StandardRecord.h"
#include "CAFAna/Core/Utilities.h"

#include "TFile.h"
#include "TH1.h"
#include "TRandom3.h"

#include <cassert>
#include <string>
#include <vector>
#include <algorithm>

// Implement FD energy scale uncertainties on truth information
// for use in eRecProxy
// LepE, eP, ePip, ePim, ePi0, eother

namespace ana
{

  // Global true energy scale syst for eRecProxy
  // Don't shift muon energies with this

  class RecoEnergyScaleFD: public ISyst {
  public:
    RecoEnergyScaleFD() : ISyst("RecoEnergyScaleFD", "Global Reco Energy Scale FD Syst") {}
    void Shift(double sigma,
               Restorer& restore, 
               caf::StandardRecord* sr,
               double& weight) const override {
  
      restore.Add(sr->VisReco_NDFD,
                  sr->RecoLepE_NDFD);

      const double scale = 0.02 * sigma;
      if (sr->isFD) {
        if (sr->isCC && abs(sr->nuPDG) == 14) { // take away muon energy
          sr->VisReco_NDFD += (sr->VisReco_NDFD - sr->RecoLepE_NDFD) * scale;
        }
        else if (sr->isCC && abs(sr->nuPDG) == 12) { // fine to include electron energy
          sr->VisReco_NDFD += sr->VisReco_NDFD * scale; 
        }
      }
    }
  };

  extern const RecoEnergyScaleFD kRecoEnergyScaleFD;

  // Total energy scale syst varying with sqrt of the energy
  class RecoEnergySqrtFD: public ISyst {
  public:
    RecoEnergySqrtFD() : ISyst("RecoEnergySqrtFD", "Sqrt Total Energy Scale FD Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {
      
      restore.Add(sr->VisReco_NDFD,
                  sr->RecoLepE_NDFD);

      const double scale = 0.01 * sigma;
      if (sr->isFD) {
        if (sr->isCC && abs(sr->nuPDG) == 14) { // take away muon energy
          sr->VisReco_NDFD += (sr->VisReco_NDFD - sr->RecoLepE_NDFD) * scale *
            pow((sr->VisReco_NDFD - sr->RecoLepE_NDFD), 0.5);
        }
        else if (sr->isCC && abs(sr->nuPDG) == 12) { // fine to include electron energy
          sr->VisReco_NDFD += (sr->VisReco_NDFD) * scale *
            pow((sr->VisReco_NDFD), 0.5);
        }
      } 
    }
  };

  extern const RecoEnergySqrtFD kRecoEnergySqrtFD;

  // Total energy scale syst varying with sqrt of the energy
  class RecoEnergyInvSqrtFD: public ISyst {
  public:
    RecoEnergyInvSqrtFD() : ISyst("RecoEnergyInvSqrtFD", "Inv Sqrt Total Energy Scale FD Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->VisReco_NDFD,
                  sr->RecoLepE_NDFD);

      const double scale = 0.02 * sigma;
      if (sr->isFD) {
        if (sr->isCC && abs(sr->nuPDG) == 14) { // take away muon energy
          sr->VisReco_NDFD += (sr->VisReco_NDFD - sr->RecoLepE_NDFD) * scale *
            pow((sr->VisReco_NDFD - sr->RecoLepE_NDFD)+0.1, -0.5);
        }
        else if (sr->isCC && abs(sr->nuPDG) == 12) { // fine to include electron energy
          sr->VisReco_NDFD += (sr->VisReco_NDFD) * scale *
            pow((sr->VisReco_NDFD+0.1), -0.5);
        }
      }
    }
  };

  extern const RecoEnergyInvSqrtFD kRecoEnergyInvSqrtFD;

  //------------------------------------------------------------------------------
  // Electromagnetic

  // Systematic for pi0s and electrons
  // 2.5% on true energy of electrons and pi0s
  class EMRecoUncorrFD : public ISyst {
  public:
    EMRecoUncorrFD() : ISyst("EMRecoUncorrFD", "EM Shower Uncorrelated FD Syst") {}
  
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, 
               double& weight) const override {

      restore.Add(sr->RecoLepE_NDFD,
                  sr->eRecoPi0,
                  sr->VisReco_NDFD);

      const double scale = 0.025 * sigma;
      if (sr->isFD) {
        sr->eRecoPi0 += sr->eRecoPi0 * scale;
        sr->VisReco_NDFD += sr->eRecoPi0 * scale;
        if (sr->isCC && abs(sr->nuPDG) == 12) {
          sr->RecoLepE_NDFD += sr->RecoLepE_NDFD * scale;
          sr->VisReco_NDFD += sr->RecoLepE_NDFD * scale;
        }
      }
    }
  };  

  extern const EMRecoUncorrFD kEMRecoUncorrFD;

  // 2.5% systematic on EM energy for sqrt parameter
  //
  class EMRecoUncorrSqrtFD : public ISyst {
  public:
    EMRecoUncorrSqrtFD() : ISyst("EMRecoUncorrSqrtFD", "EM Shower Uncorrelated FD Syst Sqrt") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, 
               double& weight) const override {
      
      restore.Add(sr->RecoLepE_NDFD,
                  sr->eRecoPi0,
                  sr->VisReco_NDFD);

      const double scale = 0.025 * sigma;
      if (sr->isFD) {
        sr->eRecoPi0 += sr->eRecoPi0 * scale * pow(sr->eRecoPi0, 0.5);
        sr->VisReco_NDFD += sr->eRecoPi0 * scale * pow(sr->eRecoPi0, 0.5); 
        if (sr->isCC && abs(sr->nuPDG) == 12) {
          sr->RecoLepE_NDFD += sr->RecoLepE_NDFD * scale * pow(sr->RecoLepE_NDFD, 0.5);
          sr->VisReco_NDFD += sr->RecoLepE_NDFD * scale * pow(sr->RecoLepE_NDFD, 0.5);
        }
      }
    }
  };

  extern const EMRecoUncorrSqrtFD kEMRecoUncorrSqrtFD;

  // 2.5% syst on EM energy for inv sqrt param
  //
  class EMRecoUncorrInvSqrtFD : public ISyst {
  public: 
    EMRecoUncorrInvSqrtFD() : ISyst("EMRecoUncorrInvSqrtFD", "EM Shower Uncorrelated FD Syst Inv Sqrt") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->RecoLepE_NDFD,
                  sr->eRecoPi0,
                  sr->VisReco_NDFD);

      const double scale = 0.025 * sigma;
      if (sr->isFD) {
        sr->eRecoPi0 += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
        sr->VisReco_NDFD += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
        if (sr->isCC && abs(sr->nuPDG) == 12) {
          sr->RecoLepE_NDFD += sr->RecoLepE_NDFD * scale * pow(sr->RecoLepE_NDFD+0.1, -0.5);
          sr->VisReco_NDFD += sr->RecoLepE_NDFD * scale * pow(sr->RecoLepE_NDFD+0.1, -0.5);
        }
      }
    }
  };

  extern const EMRecoUncorrInvSqrtFD kEMRecoUncorrInvSqrtFD;

  //---------------------------------------------------------------------
  // Charged Hadrons

  // Systematic for charged hadrons: p, pi+, pi-
  // 5% on true energy of charged pions and protons
  class ChargedHadRecoUncorrFD : public ISyst {
  public:
    ChargedHadRecoUncorrFD() : ISyst("ChargedHadRecoUncorrFD", "Charged Hadron Uncorrelated FD Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {
    
      restore.Add(sr->eRecoPip,
                  sr->eRecoPim,
                  sr->eRecoP,
                  sr->VisReco_NDFD);

      const double scale = 0.05 * sigma;
      if (sr->isFD) {
        if (sr->eRecoP < 0) sr->eRecoP = 0.;
        if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
        if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
        const double sumE = sr->eRecoPip + sr->eRecoPim + sr->eRecoP;
        sr->eRecoPip += sr->eRecoPip * scale;
        sr->eRecoPim += sr->eRecoPim * scale;
        sr->eRecoP += sr->eRecoP * scale;
        sr->VisReco_NDFD += sumE * scale;
      }
    }
  };

  extern const ChargedHadRecoUncorrFD kChargedHadRecoUncorrFD;

  // 5% syst for charged hadrons sqrt param
  //
  class ChargedHadRecoUncorrSqrtFD : public ISyst {
  public: 
    ChargedHadRecoUncorrSqrtFD() : ISyst("ChargedHadRecoUncorrSqrtFD", "Charged Had Uncorrelated Sqrt FD Syst") {}
    
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->eRecoPip,
                  sr->eRecoPim,
                  sr->eRecoP,
                  sr->VisReco_NDFD);

      const double scale = 0.05 * sigma;
      if (sr->isFD) {
        if (sr->eRecoP < 0) sr->eRecoP = 0.;
        if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
        if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
        const double sumE = sr->eRecoPip + sr->eRecoPim + sr->eRecoP;
        sr->eRecoPip += sr->eRecoPip * scale * pow(sr->eRecoPip, 0.5); // sumE before
        sr->eRecoPim += sr->eRecoPim * scale * pow(sr->eRecoPim, 0.5); // sumE
        sr->eRecoP += sr->eRecoP * scale * pow(sr->eRecoP, 0.5); // sumE
        sr->VisReco_NDFD += sumE * scale * pow(sumE, 0.5);
      }
    }
  };

  extern const ChargedHadRecoUncorrSqrtFD kChargedHadRecoUncorrSqrtFD;

  // 5% syst for charged hadrons inv sqrt param
  //
  class ChargedHadRecoUncorrInvSqrtFD : public ISyst {
  public:
    ChargedHadRecoUncorrInvSqrtFD() : ISyst("ChargedHadRecoUncorrInvSqrtFD", "Charged Had Uncorrelated Inv Sqrt FD Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->eRecoPip,
                  sr->eRecoPim,
                  sr->eRecoP,
                  sr->VisReco_NDFD);

      const double scale = 0.05 * sigma;
      if (sr->isFD) {
        if (sr->eRecoP < 0) sr->eRecoP = 0.;
        if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
        if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
        const double sumE = sr->eRecoPip + sr->eRecoPim + sr->eRecoP;
        sr->eRecoPip += sr->eRecoPip * scale * pow(sr->eRecoPip+0.1, -0.5); // sumE
        sr->eRecoPim += sr->eRecoPim * scale * pow(sr->eRecoPim+0.1, -0.5); // sumE
        sr->eRecoP += sr->eRecoP * scale * pow(sr->eRecoP+0.1, -0.5); // sumE
        sr->VisReco_NDFD += sumE * scale * pow(sumE+0.1, -0.5);
      }
    }
  };

  extern const ChargedHadRecoUncorrInvSqrtFD kChargedHadRecoUncorrInvSqrtFD;

  //-------------------------------------------------------------------
  // Muons

  // Systematic for Muon 
  // 2% on true energy of muon from CC numu event
  class ERecoScaleMuLArFD : public ISyst {
  public:
    ERecoScaleMuLArFD() : ISyst("ERecoScaleMuLArFD", "Muon Energy Scale FD Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->RecoLepE_NDFD,
                  sr->VisReco_NDFD);
 
      const double scale = 0.02 * sigma;
      if (sr->isFD) {
        if (sr->isCC && abs(sr->nuPDG) == 14) { 
          sr->RecoLepE_NDFD += sr->RecoLepE_NDFD * scale;
          sr->VisReco_NDFD += sr->RecoLepE_NDFD * scale;
        }
      }
    } 
  };

  extern const ERecoScaleMuLArFD kERecoScaleMuLArFD;

  // true energy of muon from CC numu event
  // Sqrt param 0.5%
  class ERecoScaleMuLArSqrtFD : public ISyst {
  public:
    ERecoScaleMuLArSqrtFD() : ISyst("ERecoScaleMuLArSqrtFD", "Muon E Scale Sqrt FD Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->RecoLepE_NDFD,
                  sr->VisReco_NDFD);

      const double scale = 0.02 * sigma; // was 0.005
      if (sr->isFD) {
        if (sr->isCC && abs(sr->nuPDG) == 14) {
          sr->RecoLepE_NDFD += sr->RecoLepE_NDFD * scale * pow(sr->RecoLepE_NDFD, 0.5);
          sr->VisReco_NDFD += sr->RecoLepE_NDFD * scale * pow(sr->RecoLepE_NDFD, 0.5);
        }
      }
    }
  };

  extern const ERecoScaleMuLArSqrtFD kERecoScaleMuLArSqrtFD;

  // 2% on true energy of muon from CC numu event
  // Inv Sqrt param
  class ERecoScaleMuLArInvSqrtFD : public ISyst {
  public:
    ERecoScaleMuLArInvSqrtFD() : ISyst("ERecoScaleMuLArInvSqrtFD", "Muon E Scale Inv Sqrt FD Syst") {}
  
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->RecoLepE_NDFD,
                  sr->VisReco_NDFD);

      const double scale = 0.02 * sigma;
      if (sr->isFD) {
        if (sr->isCC && abs(sr->nuPDG) == 14) {
          sr->RecoLepE_NDFD += sr->RecoLepE_NDFD * scale * pow(sr->RecoLepE_NDFD+0.1, -0.5);
          sr->VisReco_NDFD += sr->RecoLepE_NDFD * scale * pow(sr->RecoLepE_NDFD+0.1, -0.5);
        }
      }
    }
  };

  extern const ERecoScaleMuLArInvSqrtFD kERecoScaleMuLArInvSqrtFD;

  //---------------------------------------------------------------------------------

  // Resolution systematics for Muon
  class MuonRecoResFD : public ISyst {
  public:
    MuonRecoResFD() : ISyst("MuonRecoResFD", "Muon Far Detector Resolution") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {
  
      restore.Add(sr->RecoLepE_NDFD,
                  sr->LepE,
                  sr->VisReco_NDFD);

      const double scale = 0.02 * sigma;
      if (sr->isFD) { // in the FD
        sr->RecoLepE_NDFD += (sr->LepE - sr->RecoLepE_NDFD) * scale;
        sr->VisReco_NDFD += (sr->LepE - sr->RecoLepE_NDFD) * scale;
      }
    }
  };

  extern const MuonRecoResFD kMuonRecoResFD;

  //---------------------------------------------------------------------------------

  // Vector of the truth energy scale systematics
  struct RecoEnergyFDSystVector : public std::vector<const ISyst*> {};

  RecoEnergyFDSystVector GetRecoEFDSysts();

  bool IsFDdetRecoSyst(std::string);
}
