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

// Implement ND energy scale uncertainties on truth information
// for use in eRecProxy
// LepE, eP, ePip, ePim, ePi0, eother

namespace ana
{

  // Global true energy scale syst for eRecProxy
  // Don't shift muon energies with this

  class TruthEnergyScaleND : public ISyst {
  public:
    TruthEnergyScaleND() : ISyst("TruthEnergyScaleND", "Global Truth Energy Scale ND Syst") {}
    void Shift(double sigma,
               Restorer& restore, 
               caf::StandardRecord* sr,
               double& weight) const override {
  
      restore.Add(sr->eRecProxy,
                  sr->LepE);

      const double scale = 0.02 * sigma;
      if (!sr->isFD) { // in the ND
        if (sr->isCC && abs(sr->nuPDG) == 14) { // take away muon energy
          sr->eRecProxy += (sr->eRecProxy - sr->LepE) * scale;
        }
        else if (sr->isCC && abs(sr->nuPDG) == 12) { // fine to include electron energy
          sr->eRecProxy += sr->eRecProxy * scale; 
        }
      }
    }
  };

  extern const TruthEnergyScaleND kTruthEnergyScaleND;

  // Total energy scale syst varying with sqrt of the energy
  class TruthEnergySqrtND: public ISyst {
  public:
    TruthEnergySqrtND() : ISyst("TruthEnergySqrtND", "Sqrt Total Energy Scale ND Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {
      
      restore.Add(sr->eRecProxy,
                  sr->LepE);

      const double scale = 0.01 * sigma;
      if (!sr->isFD) { // in the ND
        if (sr->isCC && abs(sr->nuPDG) == 14) { // take away muon energy
          sr->eRecProxy += (sr->eRecProxy - sr->LepE) * scale *
            pow((sr->eRecProxy - sr->LepE), 0.5);
        }
        else if (sr->isCC && abs(sr->nuPDG) == 12) { // fine to include electron energy
          sr->eRecProxy += (sr->eRecProxy) * scale *
            pow((sr->eRecProxy), 0.5);
        } 
      }
    }
  };

  extern const TruthEnergySqrtND kTruthEnergySqrtND;

  // Total energy scale syst varying with sqrt of the energy
  class TruthEnergyInvSqrtND: public ISyst {
  public:
    TruthEnergyInvSqrtND() : ISyst("TruthEnergyInvSqrtND", "Inv Sqrt Total Energy Scale ND Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->eRecProxy,
                  sr->LepE);

      const double scale = 0.02 * sigma;
      if (!sr->isFD) { // in the ND
        if (sr->isCC && abs(sr->nuPDG) == 14) { // take away muon energy
          sr->eRecProxy += (sr->eRecProxy - sr->LepE) * scale *
            pow((sr->eRecProxy - sr->LepE) + 0.1, -0.5);
        }
        else if (sr->isCC && abs(sr->nuPDG) == 12) { // fine to include electron energy
          sr->eRecProxy += (sr->eRecProxy) * scale *
            pow((sr->eRecProxy + 0.1), -0.5);
        }
      }
    }
  };

  extern const TruthEnergyInvSqrtND kTruthEnergyInvSqrtND;

  //------------------------------------------------------------------------------
  // Electromagnetic

  // Systematic for pi0s and electrons
  // 2.5% on true energy of electrons and pi0s
  class EMTruthUncorrND : public ISyst {
  public:
    EMTruthUncorrND() : ISyst("EMTruthUncorrND", "EM Shower Uncorrelated ND Syst") {}
  
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, 
               double& weight) const override {

      restore.Add(sr->LepE,
                  sr->ePi0,
                  sr->eRecProxy);

      const double scale = 0.025 * sigma;
      if (!sr->isFD) { // in the ND
        sr->ePi0 += sr->ePi0 * scale;
        sr->eRecProxy += sr->ePi0 * scale;
        if (sr->isCC && abs(sr->nuPDG) == 12) {
          sr->LepE += sr->LepE * scale;
          sr->eRecProxy += sr->LepE * scale;
        }
      }
    }
  };  

  extern const EMTruthUncorrND kEMTruthUncorrND;

  // 2.5% systematic on EM energy for sqrt parameter
  //
  class EMTruthUncorrSqrtND : public ISyst {
  public:
    EMTruthUncorrSqrtND() : ISyst("EMTruthUncorrSqrtND", "EM Shower Uncorrelated ND Syst Sqrt") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, 
               double& weight) const override {
      
      restore.Add(sr->LepE,
                  sr->ePi0,
                  sr->eRecProxy);

      const double scale = 0.025 * sigma;
      if (!sr->isFD) { // in the ND
        sr->ePi0 += sr->ePi0 * scale * pow(sr->ePi0, 0.5);
        sr->eRecProxy += sr->ePi0 * scale * pow(sr->ePi0, 0.5); 
        if (sr->isCC && abs(sr->nuPDG) == 12) {
          sr->LepE += sr->LepE * scale * pow(sr->LepE, 0.5);
          sr->eRecProxy += sr->LepE * scale * pow(sr->LepE, 0.5);
        }
      }
    }
  };

  extern const EMTruthUncorrSqrtND kEMTruthUncorrSqrtND;

  // 2.5% syst on EM energy for inv sqrt param
  //
  class EMTruthUncorrInvSqrtND : public ISyst {
  public: 
    EMTruthUncorrInvSqrtND() : ISyst("EMTruthUncorrInvSqrtND", "EM Shower Uncorrelated ND Syst Inv Sqrt") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->LepE,
                  sr->ePi0,
                  sr->eRecProxy);

      const double scale = 0.025 * sigma;
      if (!sr->isFD) { // in the ND
        sr->ePi0 += sr->ePi0 * scale * pow(sr->ePi0+0.1, -0.5);
        sr->eRecProxy += sr->ePi0 * scale * pow(sr->ePi0+0.1, -0.5);
        if (sr->isCC && abs(sr->nuPDG) == 12) {
          sr->LepE += sr->LepE * scale * pow(sr->LepE+0.1, -0.5);
          sr->eRecProxy += sr->LepE * scale * pow(sr->LepE+0.1, -0.5);
        }
      }
    }
  };

  extern const EMTruthUncorrInvSqrtND kEMTruthUncorrInvSqrtND;

  //---------------------------------------------------------------------
  // Charged Hadrons

  // Systematic for charged hadrons: p, pi+, pi-
  // 5% on true energy of charged pions and protons
  class ChargedHadTruthUncorrND : public ISyst {
  public:
    ChargedHadTruthUncorrND() : ISyst("ChargedHadTruthUncorrND", "Charged Hadron Uncorrelated ND Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {
    
      restore.Add(sr->ePip,
                  sr->ePim,
                  sr->eP,
                  sr->eRecProxy);

      const double scale = 0.05 * sigma;
      const double sumE = sr->ePip + sr->ePim + sr->eP;
      if (!sr->isFD) { // in the ND
        sr->ePip += sr->ePip * scale;
        sr->ePim += sr->ePim * scale;
        sr->eP += sr->eP * scale;
        sr->eRecProxy += sumE * scale;
      }
    }
  };

  extern const ChargedHadTruthUncorrND kChargedHadTruthUncorrND;

  // 5% syst for charged hadrons sqrt param
  //
  class ChargedHadTruthUncorrSqrtND : public ISyst {
  public: 
    ChargedHadTruthUncorrSqrtND() : ISyst("ChargedHadTruthUncorrSqrtND", "Charged Had Uncorrelated Sqrt ND Syst") {}
    
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->ePip,
                  sr->ePim,
                  sr->eP,
                  sr->eRecProxy);

      const double scale = 0.05 * sigma;
      const double sumE = sr->ePip + sr->ePim + sr->eP;
      if (!sr->isFD) { // in the ND
        sr->ePip += sr->ePip * scale * pow(sumE, 0.5);
        sr->ePim += sr->ePim * scale * pow(sumE, 0.5);
        sr->eP += sr->eP * scale * pow(sumE, 0.5); 
        sr->eRecProxy += sumE * scale * pow(sumE, 0.5);
      }
    }
  };

  extern const ChargedHadTruthUncorrSqrtND kChargedHadTruthUncorrSqrtND;

  // 5% syst for charged hadrons inv sqrt param
  //
  class ChargedHadTruthUncorrInvSqrtND : public ISyst {
  public:
    ChargedHadTruthUncorrInvSqrtND() : ISyst("ChargedHadTruthUncorrInvSqrtND", "Charged Had Uncorrelated Inv Sqrt ND Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->ePip,
                  sr->ePim,
                  sr->eP,
                  sr->eRecProxy);

      const double scale = 0.05 * sigma;
      const double sumE = sr->ePip + sr->ePim + sr->eP;
      if (!sr->isFD) { // in the ND
        sr->ePip += sr->ePip * scale * pow(sumE+0.1, -0.5);
        sr->ePim += sr->ePim * scale * pow(sumE+0.1, -0.5);
        sr->eP += sr->eP * scale * pow(sumE+0.1, -0.5);
        sr->eRecProxy += sumE * scale * pow(sumE+0.1, -0.5);
      }
    }
  };

  extern const ChargedHadTruthUncorrInvSqrtND kChargedHadTruthUncorrInvSqrtND;

  //-------------------------------------------------------------------
  // Muons

  // Systematic for Muon 
  // 2% on true energy of muon from CC numu event
  class ETruthScaleMuLArND : public ISyst {
  public:
    ETruthScaleMuLArND() : ISyst("ETruthScaleMuLArND", "Muon Energy Scale ND Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->LepE,
                  sr->eRecProxy);
 
      const double scale = 0.02 * sigma;
      if (!sr->isFD) { // in the ND
        if (sr->isCC && abs(sr->nuPDG) == 14) { 
          sr->LepE += sr->LepE * scale;
          sr->eRecProxy += sr->LepE * scale;
        }
      }
    } 
  };

  extern const ETruthScaleMuLArND kETruthScaleMuLArND;

  // true energy of muon from CC numu event
  // Sqrt param 2% for LArTPC
  class ETruthScaleMuLArSqrtND : public ISyst {
  public:
    ETruthScaleMuLArSqrtND() : ISyst("ETruthScaleMuLArSqrtND", "Muon E Scale Sqrt ND Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->LepE,
                  sr->eRecProxy);
 
      // 2% in LArTPC reco by range
      const double scale = 0.02 * sigma; // is 0.005 in FD
      if (!sr->isFD) { // in the ND
        if (sr->isCC && abs(sr->nuPDG) == 14) {
          sr->LepE += sr->LepE * scale * pow(sr->LepE, 0.5);
          sr->eRecProxy += sr->LepE * scale * pow(sr->LepE, 0.5);
        }
      }
    }
  };

  extern const ETruthScaleMuLArSqrtND kETruthScaleMuLArSqrtND;

  // 2% on true energy of muon from CC numu event
  // Inv Sqrt param
  class ETruthScaleMuLArInvSqrtND : public ISyst {
  public:
    ETruthScaleMuLArInvSqrtND() : ISyst("ETruthScaleMuLArInvSqrtND", "Muon E Scale Inv Sqrt ND Syst") {}
  
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->LepE,
                  sr->eRecProxy);

      const double scale = 0.02 * sigma;
      if (!sr->isFD) { // in the ND
        if (sr->isCC && abs(sr->nuPDG) == 14) {
          sr->LepE += sr->LepE * scale * pow(sr->LepE+0.1, -0.5);
          sr->eRecProxy += sr->LepE * scale * pow(sr->LepE+0.1, -0.5);
        }
      }
    }
  };

  extern const ETruthScaleMuLArInvSqrtND kETruthScaleMuLArInvSqrtND;

  //---------------------------------------------------------------------------------

  // Vector of the truth energy scale systematics
  struct TruthEnergyNDSystVector : public std::vector<const ISyst*> {};

  TruthEnergyNDSystVector GetTrueENDSysts();

  bool IsNDdetSyst(std::string);
}
