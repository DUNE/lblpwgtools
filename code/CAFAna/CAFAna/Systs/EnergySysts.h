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
  // Overall energy scale systematics applied to all interactions
  // Uncorrelated between ND & FD
  class EnergyScaleNDSyst: public ISyst
  {
  public:
  EnergyScaleNDSyst() : ISyst("eScaleND", "Global ND Energy Scale Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
		  sr->dune.Elep_reco);

      double scale = 1 + 0.02 * sigma;
      if (!sr->dune.isFD) {
	sr->dune.Ev_reco   *= scale;
	sr->dune.Elep_reco *= scale;
      }
    }
  };  
  
  extern const EnergyScaleNDSyst kEnergyScaleNDSyst;

  // FD global energy scale syst
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
		  sr->dune.RecoLepEnNumu,
		  sr->dune.RecoLepEnNue);

      double scale = 1 + 0.02 * sigma;
      if (sr->dune.isFD) {
	sr->dune.Ev_reco_numu  *= scale;
	sr->dune.Ev_reco_nue   *= scale;
	sr->dune.RecoHadEnNumu *= scale;
	sr->dune.RecoHadEnNue  *= scale;
	sr->dune.RecoLepEnNumu *= scale;
	sr->dune.RecoLepEnNue  *= scale;
      }
    }
  };  
  
  extern const EnergyScaleFDSyst kEnergyScaleFDSyst;

  // Total energy scale shape systematics
  class UncorrFDTotLinSyst: public ISyst
  {
  public:
  UncorrFDTotLinSyst() : ISyst("UncorrFDTotLinSyst", "Uncorrelated FD Linear Total Syst") {}
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
      if (sr->dune.isFD) {
	const double scale = .01 * sigma;
	sr->dune.Ev_reco_numu += sr->dune.Ev_reco_numu * sr->dune.Ev_reco_numu * scale;
	sr->dune.Ev_reco_nue  += sr->dune.Ev_reco_nue * sr->dune.Ev_reco_nue * scale;
	sr->dune.RecoHadEnNumu += sr->dune.RecoHadEnNumu * sr->dune.RecoHadEnNumu * scale;
	sr->dune.RecoHadEnNue  += sr->dune.RecoHadEnNue * sr->dune.RecoHadEnNue * scale;
	sr->dune.RecoLepEnNumu += sr->dune.RecoLepEnNumu * sr->dune.RecoLepEnNumu * scale;
	sr->dune.RecoLepEnNue  += sr->dune.RecoLepEnNue * sr->dune.RecoLepEnNue * scale;
      }
    }
  };
  extern const UncorrFDTotLinSyst kUncorrFDTotLinSyst;

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
		  sr->dune.RecoLepEnNumu,
		  sr->dune.RecoLepEnNue);
      if (sr->dune.isFD) {
	const double scale = .02 * sigma;
	sr->dune.Ev_reco_numu += sr->dune.Ev_reco_numu * scale * pow(sr->dune.Ev_reco_numu+0.1, -0.5);
	sr->dune.Ev_reco_nue  += sr->dune.Ev_reco_nue * scale * pow(sr->dune.Ev_reco_nue+0.1, -0.5);
	sr->dune.RecoHadEnNumu += sr->dune.RecoHadEnNumu * scale * pow(sr->dune.RecoHadEnNumu+0.1, -0.5);
	sr->dune.RecoHadEnNue  += sr->dune.RecoHadEnNue * scale * pow(sr->dune.RecoHadEnNue+0.1, -0.5);
	sr->dune.RecoLepEnNumu += sr->dune.RecoLepEnNumu * scale * pow(sr->dune.RecoLepEnNumu+0.1, -0.5);
	sr->dune.RecoLepEnNue  += sr->dune.RecoLepEnNue * scale * pow(sr->dune.RecoLepEnNue+0.1, -0.5);	
      }
    }
  };
  extern const UncorrFDTotSqrtSyst kUncorrFDTotSqrtSyst;

  //------------------------------------------------------------------------------------------------

  // FD three parameter systematics to match ND ones
  // Slope energy scale systematics
  // Charged hadrons
  class UncorrFDHadLinSyst: public ISyst
  {
  public:
  UncorrFDHadLinSyst() : ISyst("UncorrFDHadLinSyst", "Uncorrelated FD Linear Hadron Syst") {}
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
	sr->dune.Ev_reco_numu += sumE * scale * sumE;
	sr->dune.Ev_reco_nue  += sumE * scale * sumE;
	sr->dune.RecoHadEnNumu += sumE * scale * sumE;
	sr->dune.RecoHadEnNue  += sumE * scale * sumE;
      }
    }
  };
  extern const UncorrFDHadLinSyst kUncorrFDHadLinSyst;

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
	sr->dune.Ev_reco_numu += sumE * scale * pow(sumE+0.1, -0.5);
	sr->dune.Ev_reco_nue  += sumE * scale * pow(sumE+0.1, -0.5);
	sr->dune.RecoHadEnNumu += sumE * scale * pow(sumE+0.1, -0.5);
	sr->dune.RecoHadEnNue  += sumE * scale * pow(sumE+0.1, -0.5);
      }
    }
  };
  extern const UncorrFDHadSqrtSyst kUncorrFDHadSqrtSyst;

  //------------------------------------------------------------------------------------

  // Muons
  class UncorrFDMuLinSyst: public ISyst
  {
  public:
  UncorrFDMuLinSyst() : ISyst("UncorrFDMuLinSyst", "Uncorrelated FD Linear Muon Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_numu,
		  sr->dune.RecoLepEnNumu);
      if (sr->dune.isFD && sr->dune.isCC==1 && sr->dune.nuPDG==14) {
	const double scale = .005 * sigma;
	sr->dune.Ev_reco_numu  += sr->dune.RecoLepEnNumu*sr->dune.RecoLepEnNumu*scale;
	sr->dune.RecoLepEnNumu += sr->dune.RecoLepEnNumu*sr->dune.RecoLepEnNumu*scale;
      }
    }
  };

  extern const UncorrFDMuLinSyst kUncorrFDMuLinSyst;

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
      if (sr->dune.isFD && sr->dune.isCC==1 && sr->dune.nuPDG==14) {
	const double scale = .02 * sigma;
	sr->dune.Ev_reco_numu  += sr->dune.RecoLepEnNumu * scale * pow(sr->dune.RecoLepEnNumu+0.1, -0.5);
	sr->dune.RecoLepEnNumu += sr->dune.RecoLepEnNumu * scale * pow(sr->dune.RecoLepEnNumu+0.1, -0.5);
      }
    }
  };
  extern const UncorrFDMuSqrtSyst kUncorrFDMuSqrtSyst;

  //------------------------------------------------------------------------------------

  //Neutrons

  class UncorrFDNLinSyst: public ISyst
  {
  public:
  UncorrFDNLinSyst() : ISyst("UncorrFDNLinSyst", "Uncorrelated FD Linear Neutron Syst") {}
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
	sr->dune.Ev_reco_numu += sr->dune.eRecoN * sr->dune.eRecoN * scale;
	sr->dune.Ev_reco_nue  += sr->dune.eRecoN * sr->dune.eRecoN * scale;
	sr->dune.RecoHadEnNumu += sr->dune.eRecoN * sr->dune.eRecoN * scale;
	sr->dune.RecoHadEnNue  += sr->dune.eRecoN * sr->dune.eRecoN * scale;
      }
    }
  };
  extern const UncorrFDNLinSyst kUncorrFDNLinSyst;

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
	sr->dune.Ev_reco_numu += sr->dune.eRecoN * scale * pow(sr->dune.eRecoN+0.1, -0.5);
	sr->dune.Ev_reco_nue  += sr->dune.eRecoN * scale * pow(sr->dune.eRecoN+0.1, -0.5);
	sr->dune.RecoHadEnNumu += sr->dune.eRecoN * scale * pow(sr->dune.eRecoN+0.1, -0.5);
	sr->dune.RecoHadEnNue  += sr->dune.eRecoN * scale * pow(sr->dune.eRecoN+0.1, -0.5);
      }
    }
  };
  extern const UncorrFDNSqrtSyst kUncorrFDNSqrtSyst;

  //------------------------------------------------------------------------------------

  // Electromagnetic

  class UncorrFDEMLinSyst: public ISyst
  {
  public:
  UncorrFDEMLinSyst() : ISyst("UncorrFDEMLinSyst", "Uncorrelated FD Linear EM Syst") {}
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
	const double scale = .05 * sigma;
	sr->dune.Ev_reco_numu += sr->dune.eRecoPi0 * sr->dune.eRecoPi0 * scale;
	sr->dune.Ev_reco_nue  += sr->dune.eRecoPi0 * sr->dune.eRecoPi0 * scale;
	sr->dune.RecoHadEnNumu += sr->dune.eRecoPi0 * sr->dune.eRecoPi0 * scale;
	sr->dune.RecoHadEnNue  += sr->dune.eRecoPi0 * sr->dune.eRecoPi0 * scale;
	if (sr->dune.isCC==1 && abs(sr->dune.nuPDG)==12) {
	  sr->dune.Ev_reco_nue  += sr->dune.RecoLepEnNue*sr->dune.RecoLepEnNue*scale;
	  sr->dune.RecoLepEnNue += sr->dune.RecoLepEnNue*sr->dune.RecoLepEnNue*scale;
	}
      }
    }
  };
  extern const UncorrFDEMLinSyst kUncorrFDEMLinSyst;

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
		  sr->dune.RecoLepEnNumu,
		  sr->dune.RecoLepEnNue);
      if (sr->dune.isFD) {
	const double scale = .05 * sigma;
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
  extern const UncorrFDEMSqrtSyst kUncorrFDEMSqrtSyst;

  // Slope energy scale systematics
  // Affect ND only
  // Charged hadrons
  class UncorrNDHadLinSyst: public ISyst
  {
  public:
  UncorrNDHadLinSyst() : ISyst("UncorrNDHadLinSyst", "Uncorrelated ND Linear Hadron Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      if (!sr->dune.isFD) {
	const double scale = .05 * sigma;
	double sumE = sr->dune.eRecoP + sr->dune.eRecoPip + sr->dune.eRecoPim;
	const double fracE = sumE / sr->dune.Ev;
	sr->dune.Ev_reco += sr->dune.Ev_reco * sumE * scale * fracE;
      }
    }
  };

  extern const UncorrNDHadLinSyst kUncorrNDHadLinSyst;

  // Pi0s
  class UncorrNDPi0LinSyst: public ISyst
  {
  public:
  UncorrNDPi0LinSyst() : ISyst("UncorrNDPi0LinSyst", "Uncorrelated ND Linear Pi0 Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      if (!sr->dune.isFD) {
	const double scale = .05 * sigma;
	const double fracE = sr->dune.eRecoPi0 / sr->dune.Ev;
	sr->dune.Ev_reco += sr->dune.Ev_reco * sr->dune.eRecoPi0 * scale * fracE;
      }
    }
  };

  extern const UncorrNDPi0LinSyst kUncorrNDPi0LinSyst;

  // Neutron
  class UncorrNDNLinSyst: public ISyst
  {
  public:
  UncorrNDNLinSyst() : ISyst("UncorrNDNLinSyst", "Uncorrelated ND Linear Neutron Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      if (!sr->dune.isFD) {
	const double scale = .05 * sigma;
	double visE = sr->dune.eRecoN;
	const double fracE = visE / sr->dune.Ev;
	sr->dune.Ev_reco += sr->dune.Ev_reco * visE * scale * fracE;
      }
    }
  };

  extern const UncorrNDNLinSyst kUncorrNDNLinSyst;

  // Energy scale systematics proportional to 1 / sqrt(E)
  // Charged hadrons
  class UncorrNDHadSqrtSyst: public ISyst
  {
  public:
  UncorrNDHadSqrtSyst() : ISyst("UncorrNDHadSqrtSyst", "Uncorrelated ND Inverse Sqrt Hadron Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      const double scale = .05 * sigma;
      if (!sr->dune.isFD) {
	double sumE = sr->dune.eRecoP + sr->dune.eRecoPim + sr->dune.eRecoPip;
	const double fracE = sumE / sr->dune.Ev;
	sr->dune.Ev_reco += sr->dune.Ev_reco * scale * (1. / (sqrt(sumE)+1)) * fracE;
      }
    }
  };

  extern const UncorrNDHadSqrtSyst kUncorrNDHadSqrtSyst;

  // Pi0s
  class UncorrNDPi0SqrtSyst: public ISyst
  {
  public:
  UncorrNDPi0SqrtSyst() : ISyst("UncorrNDPi0SqrtSyst", "Uncorrelated ND Inverse Sqrt Pi0 Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      const double scale = .05 * sigma;
      if (!sr->dune.isFD) {
	const double fracE = sr->dune.eRecoPi0 / sr->dune.Ev;
	sr->dune.Ev_reco += sr->dune.Ev_reco * scale * (1. / (sqrt(sr->dune.eRecoPi0)+1)) * fracE;
      }
    }
  };

  extern const UncorrNDPi0SqrtSyst kUncorrNDPi0SqrtSyst;

  // Neutrons
  class UncorrNDNSqrtSyst: public ISyst
  {
  public:
  UncorrNDNSqrtSyst() : ISyst("UncorrNDNSqrtSyst", "Uncorrelated ND Inverse Sqrt Neutron Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      const double scale = .05 * sigma;
      if (!sr->dune.isFD) {
	double visE = sr->dune.eRecoN;
	const double fracE = visE / sr->dune.Ev;
	sr->dune.Ev_reco += sr->dune.Ev_reco * scale * (1. / (sqrt(visE)+1)) * fracE;
      }
    }
  };

  extern const UncorrNDNSqrtSyst kUncorrNDNSqrtSyst;

  /// 1% systematic on muon energy for energy deposition in liquid argon
  /// 100% correlated between near and far detectors for those ND events that stop in the LAr
  class eScaleMuLArSyst: public ISyst
  {
  public:
  eScaleMuLArSyst() : ISyst("eScaleMuLAr", "Muon Energy Scale LAr") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco,
                  sr->dune.Ev_reco_numu,
                  sr->dune.RecoLepEnNumu);

      const double scale = 1 + .01*sigma;

      // Checks if ND
      if(!sr->dune.isFD){
	// Select only CC muon neutrino events that stop in LAr
        if(abs(sr->dune.nuPDG) == 14 && sr->dune.isCC == 1 && sr->dune.muon_contained == 1){
          sr->dune.Ev_reco   = sr->dune.Ev_reco * ( (1 - sr->dune.Y) * scale + sr->dune.Y );
	  sr->dune.Elep_reco = sr->dune.Elep_reco * scale;
        }
	else { }
      }
      // Otherwise is FD
      else {
	if(sr->dune.isCC && abs(sr->dune.nuPDG) == 14){
	  sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu *( (1 - sr->dune.Y) * scale + sr->dune.Y);
	  sr->dune.RecoLepEnNumu = sr->dune.RecoLepEnNumu * scale;
	}
      }
    }
  };

  extern const eScaleMuLArSyst keScaleMuLArSyst;

  // Muon LAr systematic but uncorrelated near/far
  // 2% on reconstructed muon energy
  // ND version
  class eScaleMuLArNDSyst: public ISyst
  {
  public:
  eScaleMuLArNDSyst() : ISyst("eScaleMuLArND", "Muon Energy Scale LAr Near Detector") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco);

      const double scale = .02 * sigma;

      // Checks if ND
      if(!sr->dune.isFD){
	// Select only CC muon neutrino events that stop in LAr
        if(abs(sr->dune.nuPDG) == 14 && sr->dune.isCC == 1 && sr->dune.muon_contained == 1){
          sr->dune.Ev_reco   += sr->dune.Elep_reco * scale;
	  sr->dune.Elep_reco *= 1. + scale;
        }
	else { }
      }
    }
  };

  extern const eScaleMuLArNDSyst keScaleMuLArNDSyst;

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

      // Checks if FD
      if(sr->dune.isFD){
	// Select only CC muon neutrino events
        if(abs(sr->dune.nuPDG) == 14 && sr->dune.isCC == 1){
          sr->dune.Ev_reco_numu   += sr->dune.RecoLepEnNumu * scale;
	  sr->dune.RecoLepEnNumu  *= 1. + scale;
        }
      }
    }
  };

  extern const eScaleMuLArFDSyst keScaleMuLArFDSyst;

  /// 1% systematics on muon energy for those tracks that are measured by the magnetic field
  // Uncorrelated between ND and FD
  // This is a temporary solution - need some momentum dependent function
  class EnergyScaleMuSystND: public ISyst
  {
  public:
  EnergyScaleMuSystND() : ISyst("eScaleMuND", "Muon Energy Scale Near Detector") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Elep_reco);

      const double scale = 1 + .01*sigma;

      // Is a numu CC and enters the tracker
      if(!sr->dune.isFD && abs(sr->dune.nuPDG)==14 && sr->dune.isCC==1 && sr->dune.muon_tracker==1){
	sr->dune.Ev_reco = sr->dune.Ev_reco * ( (1 - sr->dune.Y) * scale + sr->dune.Y );
	sr->dune.Elep_reco = sr->dune.Elep_reco * scale;
      }
    }
  };

  extern const EnergyScaleMuSystND kEnergyScaleMuSystND;

  /// 2% energy scale systematic on electron energy
  /// 100% correlated between near and far detectors
  class EnergyScaleESyst: public ISyst
  {
  public:
  EnergyScaleESyst() : ISyst("eScaleE", "Electron Energy Scale") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Ev_reco_nue,
                  sr->dune.Elep_reco,
                  sr->dune.RecoLepEnNue);

      const double scale = 1 + .02*sigma;

      // Checks if ND
      if(!sr->dune.isFD){
        if(abs(sr->dune.nuPDG) == 12 && sr->dune.isCC){
	  sr->dune.Ev_reco = sr->dune.Ev_reco * ( (1 - sr->dune.Y) * scale + sr->dune.Y);
	  sr->dune.Elep_reco = sr->dune.Elep_reco * scale;
        }
      }
      // Otherwise is FD
      else {
        if(sr->dune.isCC && abs(sr->dune.nuPDG) == 12){
          sr->dune.Ev_reco_nue = sr->dune.Ev_reco_nue * ( (1 - sr->dune.Y) * scale + sr->dune.Y );
	  sr->dune.RecoLepEnNue = sr->dune.RecoLepEnNue * scale;
        }
      }
    }
  };

  extern const EnergyScaleESyst kEnergyScaleESyst;
 

  /// Energy scale systematics for hadronic final state particles
  // 5% near/far correlated part for charged hadrons
  class ChargedHadCorrSyst: public ISyst
  {
  public:
  ChargedHadCorrSyst() : ISyst("ChargedHadCorr", "Charged Hadron Correlated Syst") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Ev_reco_nue,
                  sr->dune.Ev_reco_numu,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue);

      const double scale = 0.05 * sigma;
      double sumE = 0.;
      if(sr->dune.isFD) {
	sumE = sr->dune.eRecoP + sr->dune.eRecoPim + sr->dune.eRecoPip;
	sr->dune.Ev_reco_numu  += sumE * scale;
	sr->dune.Ev_reco_nue   += sumE * scale;
	sr->dune.RecoHadEnNumu += sumE * scale;
	sr->dune.RecoHadEnNue  += sumE * scale;
      }
      else {
	sumE = sr->dune.eRecoP + sr->dune.eRecoPim + sr->dune.eRecoPip;
	sr->dune.Ev_reco += sumE * scale;
      }
      
      // Want to apply this syst to the reco lepton energy if we have a pion misID'd as a muon
      /*
      if (!sr->dune.isFD && sr->dune.reco_numu == 1 && !sr->dune.isCC) {
	sr->dune.Elep_reco = sr->dune.Elep_reco * ( (1 - fracE) * scale + fracE );
      }
      else if (sr->dune.isFD && !sr->dune.isCC && sr->dune.cvnnumu > 0.5) {
	sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * ( (1 - fracE) * scale + fracE);
	sr->dune.RecoLepEnNumu = sr->dune.RecoLepEnNumu * ( (1 - fracEY) * scale + fracEY);
      } 
      */
    }
  };

  extern const ChargedHadCorrSyst kChargedHadCorrSyst;

  // 1% uncorrelated FD syst for charged hadrons
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

      const double scale = 1. + 0.02*sigma;
      
      if(sr->dune.isFD) { 
	const double sumE = sr->dune.eRecoP + sr->dune.eRecoPim + sr->dune.eRecoPip;

	const double fracE = sumE / sr->dune.Ev;
	const double fracEY = sumE / (sr->dune.Ev * sr->dune.Y);

	sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (fracE * scale + (1 - fracE));
	sr->dune.Ev_reco_nue = sr->dune.Ev_reco_nue * (fracE * scale + (1 - fracE));
	sr->dune.RecoHadEnNumu = sr->dune.RecoHadEnNumu * (fracEY * scale + (1 - fracEY));
	sr->dune.RecoHadEnNue = sr->dune.RecoHadEnNue * (fracEY * scale + (1 - fracEY));
      }
    }
  };
  
  extern const ChargedHadUncorrFDSyst kChargedHadUncorrFDSyst;

  /// 1% uncorrelated ND syst for charged hadrons
  class ChargedHadUncorrNDSyst: public ISyst
  {
  public:
    ChargedHadUncorrNDSyst() : ISyst("ChargedHadUncorrND", "Charged Hadron Uncorrelated ND Syst") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);

      const double scale = 1. + 0.02*sigma;
      
      if(!sr->dune.isFD) { 
	const double sumE = sr->dune.eRecoP + sr->dune.eRecoPim + sr->dune.eRecoPip;
	const double fracE = sumE / sr->dune.Ev;
	sr->dune.Ev_reco = sr->dune.Ev_reco * (fracE * scale + (1 - fracE));
      }
    }
  };
  
  extern const ChargedHadUncorrNDSyst kChargedHadUncorrNDSyst;


  // Neutron energy scale
  class NUncorrNDSyst: public ISyst
  {
  public:
    NUncorrNDSyst() : ISyst("eScaleN_ND", "Neutron Energy Scale ND") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco, sr->dune.Ev_reco_numu, sr->dune.Ev_reco_nue);

      const double scale = .20 * sigma;

      if(!sr->dune.isFD) {
	double visE = sr->dune.eRecoN;
       	sr->dune.Ev_reco       += (visE * scale);
	sr->dune.Ev_reco_numu  += (visE * scale);
	sr->dune.Ev_reco_nue   += (visE * scale);
      }   
    }
  };

  extern const NUncorrNDSyst kNUncorrNDSyst;  


  // Neutron energy scale for FD
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

  // Pi0 energy scale correlated between near and far
  // 5% on reconstructed energy
  class Pi0CorrSyst: public ISyst
  {
  public:
  Pi0CorrSyst() : ISyst("eScalePi0Corr", "Pi0 Correlated Energy Scale") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
                  sr->dune.Ev_reco_nue,
                  sr->dune.Ev_reco_numu,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue);

      const double scale = 1 + .05 * sigma;

      if (sr->dune.isFD) {               
	double fracPi0 = (sr->dune.eRecoPi0 / sr->dune.Ev);
	double fracPi0Y = (sr->dune.eRecoPi0 / (sr->dune.Ev*sr->dune.Y));
	sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (fracPi0 * scale + (1 - fracPi0));
	sr->dune.Ev_reco_nue  = sr->dune.Ev_reco_nue * (fracPi0 * scale + (1 - fracPi0));
	sr->dune.RecoHadEnNumu = sr->dune.RecoHadEnNumu * (fracPi0Y * scale + (1 - fracPi0Y));
	sr->dune.RecoHadEnNue  = sr->dune.RecoHadEnNue * (fracPi0Y * scale + (1 - fracPi0Y));
      }
      else {
	double fracPi0 = (sr->dune.eRecoPi0 / sr->dune.Ev);
	sr->dune.Ev_reco = sr->dune.Ev_reco * (fracPi0 * scale + (1 - fracPi0));
      }
    }
  };

  extern const Pi0CorrSyst kEnergyScalePi0Syst;

  // 2% uncorrelated FD syst for pi0
  class Pi0UncorrFDSyst: public ISyst
  {
  public:
  Pi0UncorrFDSyst() : ISyst("Pi0UncorrFD", "Pi0 Uncorrelated FD Syst") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco_nue,
                  sr->dune.Ev_reco_numu,
                  sr->dune.RecoHadEnNumu,
                  sr->dune.RecoHadEnNue);

      const double scale = 1. + 0.02*sigma;
      
      if(sr->dune.isFD) { 
	const double fracPi0 = sr->dune.eRecoPi0 / sr->dune.Ev;
	const double fracPi0Y = sr->dune.eRecoPi0 / (sr->dune.Ev * sr->dune.Y);

	sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (fracPi0 * scale + (1 - fracPi0));
	sr->dune.Ev_reco_nue = sr->dune.Ev_reco_nue * (fracPi0 * scale + (1 - fracPi0));
	sr->dune.RecoHadEnNumu = sr->dune.RecoHadEnNumu * (fracPi0Y * scale + (1 - fracPi0Y));
	sr->dune.RecoHadEnNue = sr->dune.RecoHadEnNue * (fracPi0Y * scale + (1 - fracPi0Y));
      }
    }
  };
  
  extern const Pi0UncorrFDSyst kPi0UncorrFDSyst;

  /// 2% uncorrelated ND syst for pi0
  class Pi0UncorrNDSyst: public ISyst
  {
  public:
  Pi0UncorrNDSyst() : ISyst("Pi0UncorrND", "Pi0Uncorrelated ND Syst") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);

      const double scale = 1. + 0.02*sigma;
      
      if(!sr->dune.isFD) { 
	const double fracPi0 = sr->dune.eRecoPi0 / sr->dune.Ev;
	sr->dune.Ev_reco = sr->dune.Ev_reco * (fracPi0 * scale + (1 - fracPi0));
      }
    }
  };
  
  extern const Pi0UncorrNDSyst kPi0UncorrNDSyst;

  // ND systematic correlated for pi0s and electrons
  // 2% on reco energy for pi0s and electrons
  // Uncorrelated between ND and FD
  class EMUncorrNDSyst: public ISyst
  {
  public:
  EMUncorrNDSyst() : ISyst("EMUncorrND", "Electromagnetic shower ND Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
		  sr->dune.Elep_reco);

      const double scale = 0.02 * sigma;

      if (!sr->dune.isFD) {
	sr->dune.Ev_reco += sr->dune.eRecoPi0 * scale;
	if (sr->dune.isCC && abs(sr->dune.nuPDG) == 12) {
	  sr->dune.Ev_reco   += sr->dune.Elep_reco * scale;
	  sr->dune.Elep_reco *= 1. + scale;
	}
      }
    }

  };

  extern const EMUncorrNDSyst kEMUncorrNDSyst;

  // ND systematic correlated for pi0s and electrons
  // 2% on reco energy for electrons
  // 5% on reco energy for pi0s
  // Uncorrelated between ND and FD
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

      const double scalePi0 = 0.05 * sigma;
      const double scaleE   = 0.02 * sigma;

      if (sr->dune.isFD) {
	sr->dune.Ev_reco_nue  += sr->dune.eRecoPi0 * scalePi0;
	sr->dune.Ev_reco_numu += sr->dune.eRecoPi0 * scalePi0;
	sr->dune.RecoHadEnNue  *= 1. + scalePi0; 
	sr->dune.RecoHadEnNumu *= 1. + scalePi0;
	if (sr->dune.isCC && abs(sr->dune.nuPDG) == 12) {
	  sr->dune.Ev_reco_nue  += sr->dune.RecoLepEnNue * scaleE;
	  sr->dune.RecoLepEnNue *= 1. + scaleE;
	}
      }
    }

  };

  extern const EMUncorrFDSyst kEMUncorrFDSyst;

  // Anticorrelated pi0 energy scale systematic (between ND & FD)
  // For use in combination with the correlated syst
  // Supercedes Pi0UncorrNDSyst & Pi0UncorrFDSyst
  class Pi0AnticorrSyst: public ISyst
  {
  public:
  Pi0AnticorrSyst() : ISyst("Pi0AnticorrSyst", "Pi0 Anticorrelated Energy Scale Syst") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
		  sr->dune.Ev_reco_numu,
		  sr->dune.Ev_reco_nue,
		  sr->dune.RecoHadEnNumu,
		  sr->dune.RecoHadEnNue);
      // +/-1sigmas based upon pre-existing uncorrND and uncorrFD systs
      const double scaleFD = 1 + 0.01021 * sigma;
      const double scaleND = 1 - 0.01021 * sigma;
      
      // Is FD
      if (sr->dune.isFD) {
	const double fracPi0 = sr->dune.eRecoPi0 / sr->dune.Ev;
	const double fracPi0Y  = sr->dune.eRecoPi0 / (sr->dune.Ev * sr->dune.Y);
	sr->dune.Ev_reco_numu  = sr->dune.Ev_reco_numu * (fracPi0 * scaleFD + (1 - fracPi0));
	sr->dune.Ev_reco_nue   = sr->dune.Ev_reco_nue * (fracPi0 * scaleFD + (1 - fracPi0));
	sr->dune.RecoHadEnNumu = sr->dune.RecoHadEnNumu * (fracPi0Y * scaleFD + (1 - fracPi0Y));
	sr->dune.RecoHadEnNue  = sr->dune.RecoHadEnNue * (fracPi0Y * scaleFD + (1 - fracPi0Y));
      }
      // Is ND
      else {
	const double fracPi0 = sr->dune.eRecoPi0 / sr->dune.Ev;
	sr->dune.Ev_reco = sr->dune.Ev_reco * (fracPi0 * scaleND + (1 - fracPi0));
      }
    }
  };

  extern const Pi0AnticorrSyst kPi0AnticorrSyst;


  // Anticorrelated charged hadron energy scale systematic (between ND & FD)
  // For use in combination with the correlated syst
  // Supercedes ChargedHadUncorrNDSyst & ChargedHadUncorrFDSyst
  class ChargedHadAnticorrSyst: public ISyst
  {
  public:
  ChargedHadAnticorrSyst() : ISyst("ChargedHadAnticorrSyst", "Charged Hadron Anticorrelated Energy Scale Syst") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
		  sr->dune.Ev_reco_numu,
		  sr->dune.Ev_reco_nue,
		  sr->dune.RecoHadEnNumu,
		  sr->dune.RecoHadEnNue);
      // +/-1sigmas based upon pre-existing uncorrFD and uncorrND
      const double scaleFD =  0.010206 * sigma;
      const double scaleND = -0.010206 * sigma;
      // Is FD
      if (sr->dune.isFD) {
	const double sumE = sr->dune.eRecoP + sr->dune.eRecoPip + sr->dune.eRecoPim;
	sr->dune.Ev_reco_numu  += scaleFD * sumE;
	sr->dune.Ev_reco_nue   += scaleFD * sumE;
	sr->dune.RecoHadEnNumu += scaleFD * sumE;
	sr->dune.RecoHadEnNue  += scaleFD * sumE;
      }
      // Is ND
      else {
	const double sumE = sr->dune.eRecoP + sr->dune.eRecoPip + sr->dune.eRecoPim;
	sr->dune.Ev_reco += scaleND * sumE;
      }
    }
  };

  extern const ChargedHadAnticorrSyst kChrargedHadAnticorrSyst;

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

      const double scale = .1*sigma;      
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

      const double scale = .1*sigma;      
      // FD charged current numus only
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

  // Charged Hadron energy resolution
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

      const double scale = .1*sigma;      
      // FD charged current numus only
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

  /// ND resolution systematics
  class MuonResNDSyst: public ISyst
  {
  public:
  MuonResNDSyst() : ISyst("MuonResND", "Muon Near Detector Resolution Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
		  sr->dune.Elep_reco);

      const double scale = .1*sigma;      
      // ND charged current numus only
      if (!sr->dune.isFD && sr->dune.isCC && abs(sr->dune.nuPDG)==14) {

	sr->dune.Ev_reco   += (sr->dune.LepE - sr->dune.Elep_reco) * scale;
	sr->dune.Elep_reco += (sr->dune.LepE - sr->dune.Elep_reco) * scale;
      }
    }
  };

  extern const MuonResNDSyst kMuonResNDSyst;

  // Electron/pi0 energy resolution
  class EMResNDSyst: public ISyst
  {
  public:
  EMResNDSyst() : ISyst("EMResND", "EM Shower Near Detector Resolution Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco,
		  sr->dune.Elep_reco);

      const double scale = .1*sigma;      
      // ND charged current numus only
      if (!sr->dune.isFD){
	sr->dune.Ev_reco += (sr->dune.ePi0 - sr->dune.eRecoPi0) * scale;
	if (sr->dune.isCC && abs(sr->dune.nuPDG)==12) {
	  sr->dune.Ev_reco   += (sr->dune.LepE - sr->dune.Elep_reco) * scale;
	  sr->dune.Elep_reco += (sr->dune.LepE - sr->dune.Elep_reco) * scale;
	}
      }
    }
  };

  extern const EMResNDSyst kEMResNDSyst;

  // Charged Hadron energy resolution
  class ChargedHadResNDSyst: public ISyst
  {
  public:
  ChargedHadResNDSyst() : ISyst("ChargedHadResND", "Charged Hadron Near Detector Resolution Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);

      const double scale = .1*sigma;      
      // ND charged current numus only
      if (!sr->dune.isFD) {
	const double trueSum = sr->dune.ePip + sr->dune.ePim + sr->dune.eP;
	const double recoSum = sr->dune.eRecoPip + sr->dune.eRecoPim + sr->dune.eRecoP;
	sr->dune.Ev_reco += (trueSum - recoSum) * scale;
      }
    }
  };

  extern const ChargedHadResNDSyst kChargedHadResNDSyst;

  // Neutron energy resolution
  class NResNDSyst: public ISyst
  {
  public:
  NResNDSyst() : ISyst("NResND", "Neutron Near Detector Resolution Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);

      const double scale = .1*sigma;      
      // ND charged current numus only
      if (!sr->dune.isFD) {
	sr->dune.Ev_reco += (sr->dune.eN - sr->dune.eRecoN) * scale;
      }
    }
  };

  extern const NResNDSyst kNResNDSyst;


  // Vector of energy scale systematics
  struct EnergySystVector: public std::vector<const ISyst*>
  {

  };


  EnergySystVector GetEnergySysts();

}
