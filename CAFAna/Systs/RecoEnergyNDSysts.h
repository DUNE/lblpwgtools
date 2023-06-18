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
               double& weight) const override;
  
  };

  extern const RecoEnergyScaleND kRecoEnergyScaleND;

  // Total energy scale syst varying with sqrt of the energy
  class RecoEnergySqrtND: public ISyst {
  public:
    RecoEnergySqrtND() : ISyst("RecoEnergySqrtND", "Sqrt Reco Energy Scale ND Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr,
               double& weight) const override; 
      
  };

  extern const RecoEnergySqrtND kRecoEnergySqrtND;

  // Total energy scale syst varying with inverse sqrt of the energy
  class RecoEnergyInvSqrtND: public ISyst {
  public:
    RecoEnergyInvSqrtND() : ISyst("RecoEnergyInvSqrtND", "Inv Sqrt Total Energy Scale ND Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr,
               double& weight) const override;

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
               double& weight) const override;

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
               double& weight) const override;
      
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
               double& weight) const override;

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
               double& weight) const override;
    
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
               double& weight) const override;

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
               double& weight) const override;

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
               double& weight) const override;

  };

  extern const ERecoScaleMuLArND kERecoScaleMuLArND;

  // true energy of muon from CC numu event
  // Sqrt param 0.5% for LArTPC
  class ERecoScaleMuLArSqrtND : public ISyst {
  public:
    ERecoScaleMuLArSqrtND() : ISyst("ERecoScaleMuLArSqrtND", "Muon E Scale Sqrt ND Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr,
               double& weight) const override;

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
               double& weight) const override;

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
               double& weight) const override;
    
  }; 

  extern const MuonRecoResND kMuonRecoResND;

  // Electron/pi0 energy resolution
  class EMRecoResND: public ISyst
  {
  public:
  EMRecoResND() : ISyst("EMRecoResND", "EM shower Near Detector Resolution Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
    
  };

  extern const EMRecoResND kEMRecoResND;

  // Charged hadron energy resolution
  class ChargedHadRecoResND: public ISyst
  {
  public:
  ChargedHadRecoResND() : ISyst("ChargedHadRecoResND", "Charged Hadron Near Detector Resolution Syst") {}
    void Shift(double sigma,
               Restorer& restore,
               caf::SRProxy* sr, double& weight) const override;
  
  };

  extern const ChargedHadRecoResND kChargedHadRecoResND;

  //---------------------------------------------------------------------------------

  // Vector of the reco energy scale systematics
  struct RecoEnergyNDSystVector : public std::vector<const ISyst*> {};

  RecoEnergyNDSystVector GetRecoENDSysts();

  bool IsNDdetRecoSyst(std::string);

  std::vector<ana::ISyst const *> FilterOutNDSysts(std::vector<ana::ISyst const *> systs);

}
