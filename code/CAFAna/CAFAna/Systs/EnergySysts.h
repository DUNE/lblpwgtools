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
  class EnergyScaleFDSyst: public ISyst
  {
  public:
  EnergyScaleFDSyst() : ISyst("eScaleFD", "Global FD Energy Scale Syst") {}
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
  
  extern const EnergyScaleFDSyst kEnergyScaleFDSyst;

  // Total energy scale shape systematics
  class UncorrFDTotSqrtSyst: public ISyst
  {
  public:
  UncorrFDTotSqrtSyst() : ISyst("UncorrFDTotSqrtSyst", "Uncorrelated FD Sqrt Total Syst") {}
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
  extern const UncorrFDTotSqrtSyst kUncorrFDTotSqrtSyst;

  class UncorrFDTotInvSqrtSyst: public ISyst
  {
  public:
  UncorrFDTotInvSqrtSyst() : ISyst("UncorrFDTotInvSqrtSyst", "Uncorrelated FD Inverse Sqrt Total Syst") {}
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
  extern const UncorrFDTotInvSqrtSyst kUncorrFDTotInvSqrtSyst;

  //------------------------------------------------------------------------------------------------

  // FD three parameter systematics to match ND ones
  // Slope energy scale systematics
  // Charged hadrons
  class UncorrFDHadSqrtSyst: public ISyst
  {
  public:
  UncorrFDHadSqrtSyst() : ISyst("UncorrFDHadSqrtSyst", "Uncorrelated FD Sqrt Hadron Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
		  sr->dune.Ev_reco_nue,
		  sr->dune.RecoHadEnNumu,
		  sr->dune.RecoHadEnNue);
      if (sr->dune.isFD) {
	const double scale = .05 * sigma;
	double sumE = sr->dune.eRecoP + sr->dune.eRecoPip + sr->dune.eRecoPim;
	sr->dune.Ev_reco_numu += sumE * scale * pow(sumE, 0.5);
	sr->dune.Ev_reco_nue  += sumE * scale * pow(sumE, 0.5);
	sr->dune.RecoHadEnNumu += sumE * scale * pow(sumE, 0.5);
	sr->dune.RecoHadEnNue  += sumE * scale * pow(sumE, 0.5);
      }
    }
  };
  extern const UncorrFDHadSqrtSyst kUncorrFDHadSqrtSyst;

  class UncorrFDHadInvSqrtSyst: public ISyst
  {
  public:
  UncorrFDHadInvSqrtSyst() : ISyst("UncorrFDHadInvSqrtSyst", "Uncorrelated FD Inv Sqrt Hadron Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
		  sr->dune.Ev_reco_nue,
		  sr->dune.RecoHadEnNumu,
		  sr->dune.RecoHadEnNue);
      if (sr->dune.isFD) {
	const double scale = .05 * sigma;
	double sumE = sr->dune.eRecoP + sr->dune.eRecoPip + sr->dune.eRecoPim;
	sr->dune.Ev_reco_numu += sumE * scale * pow(sumE+0.1, -0.5);
	sr->dune.Ev_reco_nue  += sumE * scale * pow(sumE+0.1, -0.5);
	sr->dune.RecoHadEnNumu += sumE * scale * pow(sumE+0.1, -0.5);
	sr->dune.RecoHadEnNue  += sumE * scale * pow(sumE+0.1, -0.5);
      }
    }
  };
  extern const UncorrFDHadInvSqrtSyst kUncorrFDHadInvSqrtSyst;

  //------------------------------------------------------------------------------------

  // Muons
  class UncorrFDMuSqrtSyst: public ISyst
  {
  public:
  UncorrFDMuSqrtSyst() : ISyst("UncorrFDMuSqrtSyst", "Uncorrelated FD Sqrt Muon Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
		  sr->dune.RecoLepEnNumu);
      if (sr->dune.isFD && sr->dune.isCC==1 && abs(sr->dune.nuPDG)==14) {
	const double scale = .005 * sigma;
	sr->dune.Ev_reco_numu  += sr->dune.RecoLepEnNumu*sr->dune.RecoLepEnNumu*scale;
	sr->dune.RecoLepEnNumu += sr->dune.RecoLepEnNumu*sr->dune.RecoLepEnNumu*scale;
      }
    }
  };

  extern const UncorrFDMuSqrtSyst kUncorrFDMuSqrtSyst;

  class UncorrFDMuInvSqrtSyst: public ISyst
  {
  public:
  UncorrFDMuInvSqrtSyst() : ISyst("UncorrFDMuInvSqrtSyst", "Uncorrelated FD Inv Sqrt Muon Syst") {}
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
  extern const UncorrFDMuInvSqrtSyst kUncorrFDMuInvSqrtSyst;

  //------------------------------------------------------------------------------------

  //Neutrons

  class UncorrFDNSqrtSyst: public ISyst
  {
  public:
  UncorrFDNSqrtSyst() : ISyst("UncorrFDNSqrtSyst", "Uncorrelated FD Sqrt Neutron Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
		  sr->dune.Ev_reco_nue,
		  sr->dune.RecoHadEnNumu,
		  sr->dune.RecoHadEnNue);
      if (sr->dune.isFD) {
	const double scale = .3 * sigma;
	sr->dune.Ev_reco_numu += sr->dune.eRecoN * pow(sr->dune.eRecoN, 0.5) * scale;
	sr->dune.Ev_reco_nue  += sr->dune.eRecoN * pow(sr->dune.eRecoN, 0.5) * scale;
	sr->dune.RecoHadEnNumu += sr->dune.eRecoN * pow(sr->dune.eRecoN, 0.5) * scale;
	sr->dune.RecoHadEnNue  += sr->dune.eRecoN * pow(sr->dune.eRecoN, 0.5) * scale;
      }
    }
  };
  extern const UncorrFDNSqrtSyst kUncorrFDNSqrtSyst;

  class UncorrFDNInvSqrtSyst: public ISyst
  {
  public:
  UncorrFDNInvSqrtSyst() : ISyst("UncorrFDNInvSqrtSyst", "Uncorrelated FD Inv Sqrt Neutron Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
		  sr->dune.Ev_reco_nue,
		  sr->dune.RecoHadEnNumu,
		  sr->dune.RecoHadEnNue);
      if (sr->dune.isFD) {
	const double scale = .3 * sigma;
	sr->dune.Ev_reco_numu += sr->dune.eRecoN * scale * pow(sr->dune.eRecoN+0.1, -0.5);
	sr->dune.Ev_reco_nue  += sr->dune.eRecoN * scale * pow(sr->dune.eRecoN+0.1, -0.5);
	sr->dune.RecoHadEnNumu += sr->dune.eRecoN * scale * pow(sr->dune.eRecoN+0.1, -0.5);
	sr->dune.RecoHadEnNue  += sr->dune.eRecoN * scale * pow(sr->dune.eRecoN+0.1, -0.5);
      }
    }
  };
  extern const UncorrFDNInvSqrtSyst kUncorrFDNInvSqrtSyst;

  //------------------------------------------------------------------------------------

  // Electromagnetic
  class UncorrFDEMSqrtSyst: public ISyst
  {
  public:
  UncorrFDEMSqrtSyst() : ISyst("UncorrFDEMSqrtSyst", "Uncorrelated FD Sqrt EM Syst") {}
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
	const double scale = .025 * sigma;
	sr->dune.Ev_reco_numu += sr->dune.eRecoPi0 * pow(sr->dune.eRecoPi0, 0.5) * scale;
	sr->dune.Ev_reco_nue  += sr->dune.eRecoPi0 * pow(sr->dune.eRecoPi0, 0.5) * scale;
	sr->dune.RecoHadEnNumu += sr->dune.eRecoPi0 * pow(sr->dune.eRecoPi0, 0.5) * scale;
	sr->dune.RecoHadEnNue  += sr->dune.eRecoPi0 * pow(sr->dune.eRecoPi0, 0.5) * scale;
	if (sr->dune.isCC==1 && abs(sr->dune.nuPDG)==12) {
	  sr->dune.Ev_reco_nue  += sr->dune.RecoLepEnNue*pow(sr->dune.RecoLepEnNue, 0.5)*scale;
	  sr->dune.RecoLepEnNue += sr->dune.RecoLepEnNue*pow(sr->dune.RecoLepEnNue, 0.5)*scale;
	}
      }
    }
  };
  extern const UncorrFDEMSqrtSyst kUncorrFDEMSqrtSyst;

  class UncorrFDEMInvSqrtSyst: public ISyst
  {
  public:
  UncorrFDEMInvSqrtSyst() : ISyst("UncorrFDEMInvSqrtSyst", "Uncorrelated FD Inv Sqrt EM Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
		  sr->dune.Ev_reco_nue,
		  sr->dune.RecoHadEnNumu,
		  sr->dune.RecoHadEnNue,
		  sr->dune.RecoLepEnNumu,
		  sr->dune.RecoLepEnNue);
      const double scale = .025 * sigma;
      if (sr->dune.isFD) {
	sr->dune.Ev_reco_numu += sr->dune.eRecoPi0 * scale * pow(sr->dune.eRecoPi0+0.1, -0.5);
	sr->dune.Ev_reco_nue  += sr->dune.eRecoPi0 * scale * pow(sr->dune.eRecoPi0+0.1, -0.5);
	sr->dune.RecoHadEnNumu += sr->dune.eRecoPi0 * scale * pow(sr->dune.eRecoPi0+0.1, -0.5);
	sr->dune.RecoHadEnNue  += sr->dune.eRecoPi0 * scale * pow(sr->dune.eRecoPi0+0.1, -0.5);
	if (sr->dune.isCC==1 && abs(sr->dune.nuPDG)==12) {
	  sr->dune.Ev_reco_nue  += sr->dune.RecoLepEnNue * scale * pow(sr->dune.RecoLepEnNue+0.1, -0.5);
	  sr->dune.RecoLepEnNue += sr->dune.RecoLepEnNue * scale * pow(sr->dune.RecoLepEnNue+0.1, -0.5);
	}
      }
    }
  };
  extern const UncorrFDEMInvSqrtSyst kUncorrFDEMInvSqrtSyst;

  // FD muon LAr systematic
  // 2% on CC numu events
  class eScaleMuLArFDSyst: public ISyst
  {
  public:
  eScaleMuLArFDSyst() : ISyst("eScaleMuLArFD", "Muon Energy Scale LAr Far Detector") {}

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

  extern const eScaleMuLArFDSyst keScaleMuLArFDSyst;

  // 5% uncorrelated FD syst for charged hadrons
  class ChargedHadUncorrFDSyst: public ISyst
  {
  public:
  ChargedHadUncorrFDSyst() : ISyst("ChargedHadUncorrFD", "Charged Hadron Uncorrelated FD Syst") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_nue,
                  sr->dune.Ev_reco_numu,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue);

      const double scale =  .05*sigma;
      
      if(sr->dune.isFD) { 
	const double sumE = sr->dune.eRecoP + sr->dune.eRecoPim + sr->dune.eRecoPip;
	sr->dune.Ev_reco_numu += sumE * scale;
	sr->dune.Ev_reco_nue  += sumE * scale;
	sr->dune.RecoHadEnNumu += sumE * scale;
	sr->dune.RecoHadEnNue  += sumE * scale;
      }
    }
  };
  
  extern const ChargedHadUncorrFDSyst kChargedHadUncorrFDSyst;

  // Neutron energy scale
  // 20% on visible energy
  class NUncorrFDSyst: public ISyst
  {
  public:
  NUncorrFDSyst() : ISyst("eScaleN_FD", "Neutron Energy Scale FD") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
                  sr->dune.Ev_reco_nue,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue);

      const double scale = .20 * sigma;

      if(sr->dune.isFD) {
 	sr->dune.Ev_reco_numu  += sr->dune.eRecoN * scale;
	sr->dune.Ev_reco_nue   += sr->dune.eRecoN * scale;
	sr->dune.RecoHadEnNumu += sr->dune.eRecoN * scale;
	sr->dune.RecoHadEnNue  += sr->dune.eRecoN * scale;
	
      }
    }
  };

  extern const NUncorrFDSyst kNUncorrFDSyst;  

  // FD systematic correlated for pi0s and electrons
  // 2.5% on reco energy for electrons and pi0s
  class EMUncorrFDSyst: public ISyst
  {
  public:
  EMUncorrFDSyst() : ISyst("EMUncorrFD", "Electromagnetic shower FD Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.RecoLepEnNue,
		  sr->dune.RecoHadEnNue,
		  sr->dune.RecoHadEnNumu,
		  sr->dune.Ev_reco_nue,
		  sr->dune.Ev_reco_numu);

      const double scale = 0.025 * sigma;
      if (sr->dune.isFD) {
	sr->dune.Ev_reco_nue  += sr->dune.eRecoPi0 * scale;
	sr->dune.Ev_reco_numu += sr->dune.eRecoPi0 * scale;
	sr->dune.RecoHadEnNue  += sr->dune.eRecoPi0 * scale; 
	sr->dune.RecoHadEnNumu += sr->dune.eRecoPi0 * scale;
	if (sr->dune.isCC && abs(sr->dune.nuPDG) == 12) {
	  sr->dune.Ev_reco_nue  += sr->dune.RecoLepEnNue * scale;
	  sr->dune.RecoLepEnNue += sr->dune.RecoLepEnNue * scale;
	}
      }
    }
  };

  extern const EMUncorrFDSyst kEMUncorrFDSyst;

  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Resolution systematics
  // Muon energy resolution
  class MuonResFDSyst: public ISyst
  {
  public:
  MuonResFDSyst() : ISyst("MuonResFD", "Muon Far Detector Resolution Syst") {}
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

  extern const MuonResFDSyst kMuonResFDSyst;

  // Electron/pi0 energy resolution
  class EMResFDSyst: public ISyst
  {
  public:
  EMResFDSyst() : ISyst("EMResFD", "EM shower Far Detector Resolution Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_nue,
		  sr->dune.Ev_reco_numu,
		  sr->dune.RecoHadEnNumu,
		  sr->dune.RecoHadEnNue,
		  sr->dune.RecoLepEnNue);

      const double scale = .02*sigma;      
      if (sr->dune.isFD){
	sr->dune.Ev_reco_nue  += (sr->dune.ePi0 - sr->dune.eRecoPi0) * scale;
	sr->dune.Ev_reco_numu += (sr->dune.ePi0 - sr->dune.eRecoPi0) * scale;
	sr->dune.RecoHadEnNumu += (sr->dune.ePi0 - sr->dune.eRecoPi0) * scale;
	sr->dune.RecoHadEnNue  += (sr->dune.ePi0 - sr->dune.eRecoPi0) * scale;
	if (sr->dune.isCC && abs(sr->dune.nuPDG)==12) {
	  sr->dune.Ev_reco_nue  += (sr->dune.LepE - sr->dune.RecoLepEnNue) * scale;
	  sr->dune.RecoLepEnNue += (sr->dune.LepE - sr->dune.RecoLepEnNue) * scale;
	}
      }
    }
  };

  extern const EMResFDSyst kEMResFDSyst;

  // Charged hadron energy resolution
  class ChargedHadResFDSyst: public ISyst
  {
  public:
  ChargedHadResFDSyst() : ISyst("ChargedHadResFD", "Charged Hadron Far Detector Resolution Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_nue,
		  sr->dune.Ev_reco_numu,
		  sr->dune.RecoHadEnNue,
		  sr->dune.RecoHadEnNumu);

      const double scale = .02*sigma;      
      if (sr->dune.isFD) {
	const double trueSum = sr->dune.ePip + sr->dune.ePim + sr->dune.eP;
	const double recoSum = sr->dune.eRecoPip + sr->dune.eRecoPim + sr->dune.eRecoP;
	sr->dune.Ev_reco_nue   += (trueSum - recoSum) * scale;
	sr->dune.Ev_reco_numu  += (trueSum - recoSum) * scale;
	sr->dune.RecoHadEnNue  += (trueSum - recoSum) * scale;
	sr->dune.RecoHadEnNumu += (trueSum - recoSum) * scale;
      }
    }
  };

  extern const ChargedHadResFDSyst kChargedHadResFDSyst;

  // Neutron energy resolution
  class NResFDSyst: public ISyst
  {
  public:
  NResFDSyst() : ISyst("NResFD", "Neutron Far Detector Resolution Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_nue,
		  sr->dune.Ev_reco_numu,
		  sr->dune.RecoHadEnNue,
		  sr->dune.RecoHadEnNumu);

      const double scale = .1*sigma;      
      // FD charged current numus only
      if (sr->dune.isFD) {
	sr->dune.Ev_reco_nue   += (sr->dune.eN - sr->dune.eRecoN) * scale;
	sr->dune.Ev_reco_numu  += (sr->dune.eN - sr->dune.eRecoN) * scale;
	sr->dune.RecoHadEnNue  += (sr->dune.eN - sr->dune.eRecoN) * scale;
	sr->dune.RecoHadEnNumu += (sr->dune.eN - sr->dune.eRecoN) * scale;
      }
    }
  };

  extern const NResFDSyst kNResFDSyst;

  // Vector of energy scale systematics
  struct EnergySystVector: public std::vector<const ISyst*>
  {

  };


  EnergySystVector GetEnergySysts();

}
