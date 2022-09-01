#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Cuts/AnaCuts.h"

#include "StandardRecord/SRProxy.h"

#include "TFile.h"
#include "TH1.h"
#include "TRandom3.h"

#include <cassert>
#include <string>
#include <vector>
#include <algorithm>

// Implement ND energy scale uncertainties on truth information
// for use in eRecProxy
// LepE, eP, ePip, ePim, ePi0, eother
class TH1;
class TH2;

namespace ana
{
  class SystShifts;

  class RecoEnergyScaleND : public ISyst {
  public:
    RecoEnergyScaleND() : ISyst("RecoEnergyScaleND", "Global Reco Energy Scale ND Syst") {}
    void Shift(double sigma,
               Restorer& restore, 
               caf::SRProxy* sr,
               double& weight) const override {
  
      restore.Add(sr->EVisReco_ND,
                  sr->HadEVisReco_ND,
                  sr->Elep_reco);

      const double scale = 0.02 * sigma;
      if (!sr->isFD) {
        // To match FD:
        // Whether NC or CC, Numu or Nue, we want to shift total "reconstructed neutrino energy"
        // by the "reconstructed hadronic energy".
        sr->EVisReco_ND += (sr->EVisReco_ND - sr->Elep_reco) * scale;
        // Also shift the hadronic energy variable, if we don't do this then shifted 
        // plots of the reconstructed hadronic energy will not be different to nominal.
        sr->HadEVisReco_ND *= 1. + scale;
        // If it isn't Numu-CC, also shift the "reconstructed neutrino energy" by the
        // "reconstructed leptonic energy".
        // Also shift the reconstructed leptonic energy itself.
        if (!(sr->isCC && abs(sr->nuPDG) == 14)) {
          sr->EVisReco_ND += sr->Elep_reco * scale; 
          sr->Elep_reco *= 1. + scale;
        }
      }
    }
  };

  extern const RecoEnergyScaleND kRecoEnergyScaleND;

  // Total energy scale syst varying with sqrt of the energy
  class RecoEnergySqrtND: public ISyst {
  public:
    RecoEnergySqrtND() : ISyst("RecoEnergySqrtND", "Sqrt Reco Energy Scale ND Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr,
               double& weight) const override {
      
      restore.Add(sr->EVisReco_ND,
                  sr->HadEVisReco_ND,
                  sr->Elep_reco);

      const double scale = 0.01 * sigma;
      if (!sr->isFD) {
        // To match LBL TDR:
        // Whether NC or CC, Numu or Nue, we want to shift total "reconstructed neutrino energy"
        // by the "reconstructed hadronic energy".
        sr->EVisReco_ND += (sr->EVisReco_ND - sr->Elep_reco) * scale *
          pow((sr->EVisReco_ND - sr->Elep_reco), 0.5);
        // Also shift the hadronic energy variable, if we don't do this then shifted 
        // plots of the reconstructed hadronic energy will not be different to nominal.
        sr->HadEVisReco_ND += sr->HadEVisReco_ND * scale * pow(sr->HadEVisReco_ND, 0.5);
        // If it isn't Numu-CC, also shift the "reconstructed neutrino energy" by the
        // "reconstructed leptonic energy".
        // Also shift the reconstructed leptonic energy itself.
        if (!(sr->isCC && abs(sr->nuPDG) == 14)) {
          sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
          sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
        }
      }
    }
  };

  extern const RecoEnergySqrtND kRecoEnergySqrtND;

  // Total energy scale syst varying with inverse sqrt of the energy
  class RecoEnergyInvSqrtND: public ISyst {
  public:
    RecoEnergyInvSqrtND() : ISyst("RecoEnergyInvSqrtND", "Inv Sqrt Total Energy Scale ND Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr,
               double& weight) const override {

      restore.Add(sr->EVisReco_ND,
                  sr->HadEVisReco_ND,
                  sr->Elep_reco);

      const double scale = 0.02 * sigma;
      if (!sr->isFD) {
        // To match LBL TDR:
        // Whether NC or CC, Numu or Nue, we want to shift total "reconstructed neutrino energy"
        // by the "reconstructed hadronic energy".
        sr->EVisReco_ND += (sr->EVisReco_ND - sr->Elep_reco) * scale *
          pow((sr->EVisReco_ND - sr->Elep_reco+0.1), -0.5);
        // Also shift the hadronic energy variable, if we don't do this then shifted 
        // plots of the reconstructed hadronic energy will not be different to nominal.
        sr->HadEVisReco_ND += sr->HadEVisReco_ND * scale * pow(sr->HadEVisReco_ND+0.1, -0.5);
        // If it isn't Numu-CC, also shift the "reconstructed neutrino energy" by the
        // "reconstructed leptonic energy".
        // Also shift the reconstructed leptonic energy itself.
        if (!(sr->isCC && abs(sr->nuPDG) == 14)) {
          sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
          sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
        }
      }
    }
  };

  extern const RecoEnergyInvSqrtND kRecoEnergyInvSqrtND;

  //------------------------------------------------------------------------------
  // Electromagnetic

  // Systematic for pi0s and electrons
  // 2.5% on true energy of electrons and pi0s
  class EMRecoUncorrND : public ISyst {
  public:
    EMRecoUncorrND() : ISyst("EMRecoUncorrND", "EM Shower Uncorrelated ND Syst") {}
  
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, 
               double& weight) const override {

      restore.Add(sr->Elep_reco,
                  sr->eRecoPi0,
                  sr->EVisReco_ND);

      const double scale = 0.025 * sigma;
      if (!sr->isFD) { // in the ND
        sr->eRecoPi0 += sr->eRecoPi0 * scale;
        sr->EVisReco_ND += sr->eRecoPi0 * scale;
        if (sr->isCC && abs(sr->nuPDG) == 12) {
          sr->Elep_reco += sr->Elep_reco * scale;
          sr->EVisReco_ND += sr->Elep_reco * scale;
        }
      }
    }
  };  

  extern const EMRecoUncorrND kEMRecoUncorrND;

  // 2.5% systematic on EM energy for sqrt parameter
  //
  class EMRecoUncorrSqrtND : public ISyst {
  public:
    EMRecoUncorrSqrtND() : ISyst("EMRecoUncorrSqrtND", "EM Shower Uncorrelated ND Syst Sqrt") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, 
               double& weight) const override {
      
      restore.Add(sr->Elep_reco,
                  sr->eRecoPi0,
                  sr->EVisReco_ND);

      const double scale = 0.025 * sigma;
      if (!sr->isFD) { // in the ND
        sr->eRecoPi0 += sr->eRecoPi0 * scale * pow(sr->eRecoPi0, 0.5);
        sr->EVisReco_ND += sr->eRecoPi0 * scale * pow(sr->eRecoPi0, 0.5); 
        if (sr->isCC && abs(sr->nuPDG) == 12) {
          sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
          sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
        }
      }
    }
  };

  extern const EMRecoUncorrSqrtND kEMRecoUncorrSqrtND;

  // 2.5% syst on EM energy for inv sqrt param
  //
  class EMRecoUncorrInvSqrtND : public ISyst {
  public: 
    EMRecoUncorrInvSqrtND() : ISyst("EMRecoUncorrInvSqrtND", "EM Shower Uncorrelated ND Syst Inv Sqrt") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr,
               double& weight) const override {

      restore.Add(sr->Elep_reco,
                  sr->eRecoPi0,
                  sr->EVisReco_ND);

      const double scale = 0.025 * sigma;
      if (!sr->isFD) { // in the ND
        sr->eRecoPi0 += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
        sr->EVisReco_ND += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
        if (sr->isCC && abs(sr->nuPDG) == 12) {
          sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
          sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
        }
      }
    }
  };

  extern const EMRecoUncorrInvSqrtND kEMRecoUncorrInvSqrtND;

  //---------------------------------------------------------------------
  // Charged Hadrons

  // Systematic for charged hadrons: p, pi+, pi-
  // 5% on true energy of charged pions and protons
  class ChargedHadRecoUncorrND : public ISyst {
  public:
    ChargedHadRecoUncorrND() : ISyst("ChargedHadRecoUncorrND", "Charged Hadron Uncorrelated ND Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr,
               double& weight) const override {
    
      restore.Add(sr->eRecoPip,
                  sr->eRecoPim,
                  sr->eRecoP,
                  sr->EVisReco_ND,
                  sr->HadEVisReco_ND);

      const double scale = 0.05 * sigma;
      if (!sr->isFD) { // in the ND
        if (sr->eRecoP < 0) sr->eRecoP = 0.;
        if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
        if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
        const double sumE = sr->eRecoP + sr->eRecoPim + sr->eRecoPip;
        sr->eRecoPip += sr->eRecoPip * scale;
        sr->eRecoPim += sr->eRecoPim * scale;
        sr->eRecoP += sr->eRecoP * scale;
        sr->EVisReco_ND += sumE * scale;
        sr->HadEVisReco_ND += sumE * scale;
      }
    }
  };

  extern const ChargedHadRecoUncorrND kChargedHadRecoUncorrND;

  // 5% syst for charged hadrons sqrt param
  //
  class ChargedHadRecoUncorrSqrtND : public ISyst {
  public: 
    ChargedHadRecoUncorrSqrtND() : ISyst("ChargedHadRecoUncorrSqrtND", "Charged Had Uncorrelated Sqrt ND Syst") {}
    
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr,
               double& weight) const override {

      restore.Add(sr->eRecoPip,
                  sr->eRecoPim,
                  sr->eRecoP,
                  sr->EVisReco_ND,
                  sr->HadEVisReco_ND);

      const double scale = 0.05 * sigma;
      if (!sr->isFD) { // in the ND
        if (sr->eRecoP < 0) sr->eRecoP = 0.;
        if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
        if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
        const double sumE = sr->eRecoPip + sr->eRecoPim + sr->eRecoP;
        sr->eRecoPip += sr->eRecoPip * scale * pow(sr->eRecoPip, 0.5);
        sr->eRecoPim += sr->eRecoPim * scale * pow(sr->eRecoPim, 0.5);
        sr->eRecoP += sr->eRecoP * scale * pow(sr->eRecoP, 0.5);
        sr->EVisReco_ND += sumE * scale * pow(sumE, 0.5);
        sr->HadEVisReco_ND += sumE * scale * pow(sumE, 0.5);
      }
    }
  };

  extern const ChargedHadRecoUncorrSqrtND kChargedHadRecoUncorrSqrtND;

  // 5% syst for charged hadrons inv sqrt param
  //
  class ChargedHadRecoUncorrInvSqrtND : public ISyst {
  public:
    ChargedHadRecoUncorrInvSqrtND() : ISyst("ChargedHadRecoUncorrInvSqrtND", "Charged Had Uncorrelated Inv Sqrt ND Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr,
               double& weight) const override {

      restore.Add(sr->eRecoPip,
                  sr->eRecoPim,
                  sr->eRecoP,
                  sr->EVisReco_ND,
                  sr->HadEVisReco_ND);

      const double scale = 0.05 * sigma;
      if (!sr->isFD) { // in the ND
        if (sr->eRecoP < 0) sr->eRecoP = 0.; 
        if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
        if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
        const double sumE = sr->eRecoPip + sr->eRecoPim + sr->eRecoP;
        sr->eRecoPip += sr->eRecoPip * scale * pow(sr->eRecoPip+0.1, -0.5);
        sr->eRecoPim += sr->eRecoPim * scale * pow(sr->eRecoPim+0.1, -0.5);
        sr->eRecoP += sr->eRecoP * scale * pow(sr->eRecoP+0.1, -0.5);
        sr->EVisReco_ND += sumE * scale * pow(sumE+0.1, -0.5);
        sr->HadEVisReco_ND += sumE * scale * pow(sumE+0.1, -0.5);
      }
    }
  };

  extern const ChargedHadRecoUncorrInvSqrtND kChargedHadRecoUncorrInvSqrtND;

  //-------------------------------------------------------------------
  // Muons

  // Systematic for Muon 
  // 2% on true energy of muon from CC numu event
  class ERecoScaleMuLArND : public ISyst {
  public:
    ERecoScaleMuLArND() : ISyst("ERecoScaleMuLArND", "Muon Energy Scale ND Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr,
               double& weight) const override {

      restore.Add(sr->Elep_reco,
                  sr->EVisReco_ND);
      // Different EScale uncertainty for reconstruction by range or in
      // GAr tracker
      double var(0);
      if (sr->muon_contained) var = 0.02;
      else if (sr->muon_tracker) var = 0.01;
      else var = 0.02; // selection requires muon to be inLAr or GAr so never get here
      const double scale = var * sigma;
      if (!sr->isFD) { // in the ND
        if (sr->isCC && abs(sr->nuPDG) == 14) { 
          sr->Elep_reco += sr->Elep_reco * scale;
          sr->EVisReco_ND += sr->Elep_reco * scale;
        }
      }
    } 
  };

  extern const ERecoScaleMuLArND kERecoScaleMuLArND;

  // true energy of muon from CC numu event
  // Sqrt param 2% for LArTPC
  class ERecoScaleMuLArSqrtND : public ISyst {
  public:
    ERecoScaleMuLArSqrtND() : ISyst("ERecoScaleMuLArSqrtND", "Muon E Scale Sqrt ND Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr,
               double& weight) const override {

      restore.Add(sr->Elep_reco,
                  sr->EVisReco_ND);
 
      double var(0);
      if (sr->muon_contained) var = 0.02;
      else if (sr->muon_tracker) var = 0.01;
      else var = 0.02; // selection requires muon to be inLAr or GAr so never get here
      const double scale = var * sigma; // is 0.005 in FD
      if (!sr->isFD) { // in the ND
        if (sr->isCC && abs(sr->nuPDG) == 14) {
          sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
          sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
        }
      }
    }
  };

  extern const ERecoScaleMuLArSqrtND kERecoScaleMuLArSqrtND;

  // 2% on true energy of muon from CC numu event
  // Inv Sqrt param
  class ERecoScaleMuLArInvSqrtND : public ISyst {
  public:
    ERecoScaleMuLArInvSqrtND() : ISyst("ERecoScaleMuLArInvSqrtND", "Muon E Scale Inv Sqrt ND Syst") {}
  
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr,
               double& weight) const override {

      restore.Add(sr->Elep_reco,
                  sr->EVisReco_ND);

      double var(0);
      if (sr->muon_contained) var = 0.02;
      else if (sr->muon_tracker) var = 0.01;
      else var = 0.02; // selection requires muon to be inLAr or GAr so never get here
      const double scale = var * sigma;
      if (!sr->isFD) { // in the ND
        if (sr->isCC && abs(sr->nuPDG) == 14) {
          sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
          sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
        }
      }
    }
  };

  extern const ERecoScaleMuLArInvSqrtND kERecoScaleMuLArInvSqrtND;

  //---------------------------------------------------------------------------------
  
  // Resolution systematics for Muon
  class MuonRecoResND : public ISyst {
  public:
    MuonRecoResND() : ISyst("MuonRecoResND", "Muon Near Detector Resolution") {}

    void Shift(double sigma, 
               Restorer& restore,
               caf::SRProxy* sr,
               double& weight) const override {
    
      restore.Add(sr->Elep_reco,
                  sr->EVisReco_ND);
      
      const double scale = 0.02 * sigma;
      if (!sr->isFD) { // in the ND
        sr->Elep_reco += (sr->LepE - sr->Elep_reco) * scale;
        sr->EVisReco_ND += (sr->LepE - sr->Elep_reco) * scale;          
      }
    }
  }; 

  extern const MuonRecoResND kMuonRecoResND;

  // Electron/pi0 energy resolution
  class EMRecoResND: public ISyst
  {
  public:
  EMRecoResND() : ISyst("EMRecoResND", "EM shower Near Detector Resolution Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->Elep_reco,
                  sr->eRecoPi0,
                  sr->EVisReco_ND);
      const double scale = .02*sigma;
      if (!sr->isFD){
        sr->EVisReco_ND  += (sr->ePi0 - sr->eRecoPi0) * scale;
        sr->eRecoPi0      += (sr->ePi0 - sr->eRecoPi0) * scale;
        if (sr->isCC && abs(sr->nuPDG)==12) {
          sr->EVisReco_ND  += (sr->LepE - sr->Elep_reco) * scale;
          sr->Elep_reco += (sr->LepE - sr->Elep_reco) * scale;
        }
      }
    }
  };

  extern const EMRecoResND kEMRecoResND;

  // Charged hadron energy resolution
  class ChargedHadRecoResND: public ISyst
  {
  public:
  ChargedHadRecoResND() : ISyst("ChargedHadRecoResND", "Charged Hadron Near Detector Resolution Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override
    {
      restore.Add(sr->EVisReco_ND,
                  sr->eRecoP,
                  sr->eRecoPip,
                  sr->eRecoPim,
                  sr->HadEVisReco_ND);
      const double scale = .02*sigma;
      if (!sr->isFD) {
        if (sr->eRecoP < 0.) sr->eRecoP = 0.;
        if (sr->eRecoPip < 0.) sr->eRecoPip = 0.;
        if (sr->eRecoPim < 0.) sr->eRecoPim = 0.;
        const double trueSum = sr->ePip + sr->ePim + sr->eP;
        const double recoSum = sr->eRecoPip + sr->eRecoPim + sr->eRecoP;
        sr->eRecoPip += (sr->ePip - sr->eRecoPip) * scale;
        sr->eRecoPim += (sr->ePim - sr->eRecoPim) * scale;
        sr->eRecoP   += (sr->eP   - sr->eRecoP)   * scale;
        sr->EVisReco_ND += (trueSum - recoSum)   * scale;
        sr->HadEVisReco_ND += (trueSum - recoSum)   * scale;
      }
    }
  };

  extern const ChargedHadRecoResND kChargedHadRecoResND;

  //---------------------------------------------------------------------------------

  // Vector of the reco energy scale systematics
  struct RecoEnergyNDSystVector : public std::vector<const ISyst*> {};

  RecoEnergyNDSystVector GetRecoENDSysts();

  bool IsNDdetRecoSyst(std::string);

  std::vector<ana::ISyst const *> FilterOutNDSysts(std::vector<ana::ISyst const *> systs);

}
