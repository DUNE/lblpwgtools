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

  class TruthEnergyScaleFD: public ISyst {
  public:
    TruthEnergyScaleFD() : ISyst("TruthEnergyScaleFD", "Global Truth Energy Scale FD Syst") {}
    void Shift(double sigma,
               Restorer& restore, 
               caf::StandardRecord* sr,
               double& weight) const override {
  
      restore.Add(sr->eRecProxy,
                  sr->LepE);

      const double scale = 0.02 * sigma;
      if (sr->isFD) {
        if (sr->isCC && abs(sr->nuPDG) == 14) { // take away muon energy
          sr->eRecProxy += (sr->eRecProxy - sr->LepE) * scale;
        }
        else if (sr->isCC && abs(sr->nuPDG) == 12) { // fine to include electron energy
          sr->eRecProxy += sr->eRecProxy * scale; 
        }
      }
    }
  };

  extern const TruthEnergyScaleFD kTruthEnergyScaleFD;

  // Total energy scale syst varying with sqrt of the energy
  class TruthEnergySqrtFD: public ISyst {
  public:
    TruthEnergySqrtFD() : ISyst("TruthEnergySqrtFD", "Sqrt Total Energy Scale FD Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {
      
      restore.Add(sr->eRecProxy,
                  sr->LepE);

      const double scale = 0.01 * sigma;
      if (sr->isFD) {
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

  extern const TruthEnergySqrtFD kTruthEnergySqrtFD;

  // Total energy scale syst varying with sqrt of the energy
  class TruthEnergyInvSqrtFD: public ISyst {
  public:
    TruthEnergyInvSqrtFD() : ISyst("TruthEnergyInvSqrtFD", "Inv Sqrt Total Energy Scale FD Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->eRecProxy,
                  sr->LepE);

      const double scale = 0.02 * sigma;
      if (sr->isFD) {
        if (sr->isCC && abs(sr->nuPDG) == 14) { // take away muon energy
          sr->eRecProxy += (sr->eRecProxy - sr->LepE) * scale *
            pow((sr->eRecProxy - sr->LepE)+0.1, -0.5);
        }
        else if (sr->isCC && abs(sr->nuPDG) == 12) { // fine to include electron energy
          sr->eRecProxy += (sr->eRecProxy) * scale *
            pow((sr->eRecProxy+0.1), -0.5);
        }
      }
    }
  };

  extern const TruthEnergyInvSqrtFD kTruthEnergyInvSqrtFD;

  //------------------------------------------------------------------------------
  // Electromagnetic

  // Systematic for pi0s and electrons
  // 2.5% on true energy of electrons and pi0s
  class EMTruthUncorrFD : public ISyst {
  public:
    EMTruthUncorrFD() : ISyst("EMTruthUncorrFD", "EM Shower Uncorrelated FD Syst") {}
  
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, 
               double& weight) const override {

      restore.Add(sr->LepE,
                  sr->ePi0,
                  sr->eRecProxy);

      const double scale = 0.025 * sigma;
      if (sr->isFD) {
        sr->ePi0 += sr->ePi0 * scale;
        sr->eRecProxy += sr->ePi0 * scale;
        if (sr->isCC && abs(sr->nuPDG) == 12) {
          sr->LepE += sr->LepE * scale;
          sr->eRecProxy += sr->LepE * scale;
        }
      }
    }
  };  

  extern const EMTruthUncorrFD kEMTruthUncorrFD;

  // 2.5% systematic on EM energy for sqrt parameter
  //
  class EMTruthUncorrSqrtFD : public ISyst {
  public:
    EMTruthUncorrSqrtFD() : ISyst("EMTruthUncorrSqrtFD", "EM Shower Uncorrelated FD Syst Sqrt") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, 
               double& weight) const override {
      
      restore.Add(sr->LepE,
                  sr->ePi0,
                  sr->eRecProxy);

      const double scale = 0.025 * sigma;
      if (sr->isFD) {
        sr->ePi0 += sr->ePi0 * scale * pow(sr->ePi0, 0.5);
        sr->eRecProxy += sr->ePi0 * scale * pow(sr->ePi0, 0.5); 
        if (sr->isCC && abs(sr->nuPDG) == 12) {
          sr->LepE += sr->LepE * scale * pow(sr->LepE, 0.5);
          sr->eRecProxy += sr->LepE * scale * pow(sr->LepE, 0.5);
        }
      }
    }
  };

  extern const EMTruthUncorrSqrtFD kEMTruthUncorrSqrtFD;

  // 2.5% syst on EM energy for inv sqrt param
  //
  class EMTruthUncorrInvSqrtFD : public ISyst {
  public: 
    EMTruthUncorrInvSqrtFD() : ISyst("EMTruthUncorrInvSqrtFD", "EM Shower Uncorrelated FD Syst Inv Sqrt") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->LepE,
                  sr->ePi0,
                  sr->eRecProxy);

      const double scale = 0.025 * sigma;
      if (sr->isFD) {
        sr->ePi0 += sr->ePi0 * scale * pow(sr->ePi0+0.1, -0.5);
        sr->eRecProxy += sr->ePi0 * scale * pow(sr->ePi0+0.1, -0.5);
        if (sr->isCC && abs(sr->nuPDG) == 12) {
          sr->LepE += sr->LepE * scale * pow(sr->LepE+0.1, -0.5);
          sr->eRecProxy += sr->LepE * scale * pow(sr->LepE+0.1, -0.5);
        }
      }
    }
  };

  extern const EMTruthUncorrInvSqrtFD kEMTruthUncorrInvSqrtFD;

  //---------------------------------------------------------------------
  // Charged Hadrons

  // Systematic for charged hadrons: p, pi+, pi-
  // 5% on true energy of charged pions and protons
  class ChargedHadTruthUncorrFD : public ISyst {
  public:
    ChargedHadTruthUncorrFD() : ISyst("ChargedHadTruthUncorrFD", "Charged Hadron Uncorrelated FD Syst") {}

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
      if (sr->isFD) {
        sr->ePip += sr->ePip * scale;
        sr->ePim += sr->ePim * scale;
        sr->eP += sr->eP * scale;
        sr->eRecProxy += sumE * scale;
      }
    }
  };

  extern const ChargedHadTruthUncorrFD kChargedHadTruthUncorrFD;

  // 5% syst for charged hadrons sqrt param
  //
  class ChargedHadTruthUncorrSqrtFD : public ISyst {
  public: 
    ChargedHadTruthUncorrSqrtFD() : ISyst("ChargedHadTruthUncorrSqrtFD", "Charged Had Uncorrelated Sqrt FD Syst") {}
    
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
      if (sr->isFD) {
        sr->ePip += sr->ePip * scale * pow(sumE, 0.5);
        sr->ePim += sr->ePim * scale * pow(sumE, 0.5);
        sr->eP += sr->eP * scale * pow(sumE, 0.5); 
        sr->eRecProxy += sumE * scale * pow(sumE, 0.5);
      }
    }
  };

  extern const ChargedHadTruthUncorrSqrtFD kChargedHadTruthUncorrSqrtFD;

  // 5% syst for charged hadrons inv sqrt param
  //
  class ChargedHadTruthUncorrInvSqrtFD : public ISyst {
  public:
    ChargedHadTruthUncorrInvSqrtFD() : ISyst("ChargedHadTruthUncorrInvSqrtFD", "Charged Had Uncorrelated Inv Sqrt FD Syst") {}

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
      if (sr->isFD) {
        sr->ePip += sr->ePip * scale * pow(sumE+0.1, -0.5);
        sr->ePim += sr->ePim * scale * pow(sumE+0.1, -0.5);
        sr->eP += sr->eP * scale * pow(sumE+0.1, -0.5);
        sr->eRecProxy += sumE * scale * pow(sumE+0.1, -0.5);
      }
    }
  };

  extern const ChargedHadTruthUncorrInvSqrtFD kChargedHadTruthUncorrInvSqrtFD;

  //-------------------------------------------------------------------
  // Muons

  // Systematic for Muon 
  // 2% on true energy of muon from CC numu event
  class ETruthScaleMuLArFD : public ISyst {
  public:
    ETruthScaleMuLArFD() : ISyst("ETruthScaleMuLArFD", "Muon Energy Scale FD Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->LepE,
                  sr->eRecProxy);
 
      const double scale = 0.02 * sigma;
      if (sr->isFD) {
        if (sr->isCC && abs(sr->nuPDG) == 14) { 
          sr->LepE += sr->LepE * scale;
          sr->eRecProxy += sr->LepE * scale;
        }
      }
    } 
  };

  extern const ETruthScaleMuLArFD kETruthScaleMuLArFD;

  // true energy of muon from CC numu event
  // Sqrt param 0.5%
  class ETruthScaleMuLArSqrtFD : public ISyst {
  public:
    ETruthScaleMuLArSqrtFD() : ISyst("ETruthScaleMuLArSqrtFD", "Muon E Scale Sqrt FD Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->LepE,
                  sr->eRecProxy);

      const double scale = 0.02 * sigma; // was 0.005
      if (sr->isFD) {
        if (sr->isCC && abs(sr->nuPDG) == 14) {
          sr->LepE += sr->LepE * scale * pow(sr->LepE, 0.5);
          sr->eRecProxy += sr->LepE * scale * pow(sr->LepE, 0.5);
        }
      }
    }
  };

  extern const ETruthScaleMuLArSqrtFD kETruthScaleMuLArSqrtFD;

  // 2% on true energy of muon from CC numu event
  // Inv Sqrt param
  class ETruthScaleMuLArInvSqrtFD : public ISyst {
  public:
    ETruthScaleMuLArInvSqrtFD() : ISyst("ETruthScaleMuLArInvSqrtFD", "Muon E Scale Inv Sqrt FD Syst") {}
  
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override {

      restore.Add(sr->LepE,
                  sr->eRecProxy);

      const double scale = 0.02 * sigma;
      if (sr->isFD) {
        if (sr->isCC && abs(sr->nuPDG) == 14) {
          sr->LepE += sr->LepE * scale * pow(sr->LepE+0.1, -0.5);
          sr->eRecProxy += sr->LepE * scale * pow(sr->LepE+0.1, -0.5);
        }
      }
    }
  };

  extern const ETruthScaleMuLArInvSqrtFD kETruthScaleMuLArInvSqrtFD;

  //---------------------------------------------------------------------------------

  // Vector of the truth energy scale systematics
  struct TruthEnergyFDSystVector : public std::vector<const ISyst*> {};

  TruthEnergyFDSystVector GetTrueEFDSysts();

  bool IsFDdetSyst(std::string);
}
