#pragma once

#include "CAFAna/Core/ISyst.h"
#include "StandardRecord/StandardRecord.h"
#include "CAFAna/Core/Utilities.h"

#include "TFile.h"
#include "TH1.h"
#include "TRandom3.h"

namespace ana
{
  /// 2% systematic on muon energy
  /// 100% correlated between near and far detectos
  class EnergyScaleMuSyst: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleMu";}
    std::string LatexName() const override {return "Muon Energy Scale";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_numu);

      const double scale = 1 + .02*sigma;

      // Checks if ND
      if(sr->dune.run == 1 && abs(sr->dune.neu) == 14 && sr->dune.ccnc == 0){
	double YCalc = 1 - (sr->dune.Elep/sr->dune.Ev);
	sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - YCalc) * scale + (sr->dune.Ev_reco * YCalc);
      }
      // Otherwise is FD
      else {
	if(sr->dune.ccnc == 0 && abs(sr->dune.neu) == 14){
	  double Y = sr->dune.Y;
	  sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - Y) * scale + (sr->dune.Ev_reco * Y);
	  sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (1 - Y) * scale + (sr->dune.Ev_reco_numu * Y);
	}
	else {}
      }
    }
  };

  static const EnergyScaleMuSyst kEnergyScaleMuSyst;


  /// 2% energy scale systematic on electron energy
  /// 100% correlated between near and far detectors
  class EnergyScaleESyst: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleE";}
    std::string LatexName() const override {return "Electron Energy Scale";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);

      const double scale = 1 + .02*sigma;

      // Checks if ND
      if(sr->dune.run == 1 && abs(sr->dune.neu) == 12 && sr->dune.ccnc == 0){
	double YCalc = 1 - (sr->dune.Elep/sr->dune.Ev);
	sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - YCalc) * scale + (sr->dune.Ev_reco * YCalc);
      }
      // Otherwise is FD
      else {
	if(sr->dune.ccnc == 0 && abs(sr->dune.neu) == 12){
	  double Y = sr->dune.Y;
	  sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - Y) * scale + (sr->dune.Ev_reco * Y);
	  sr->dune.Ev_reco_nue = sr->dune.Ev_reco_nue * (1 - Y) * scale + (sr->dune.Ev_reco_nue * Y);
	}
	else {}
      }
    }
  };

  static const EnergyScaleESyst kEnergyScaleESyst;


  /// 15% energy scale systematic on hadronic component of neutrino interactions
  /// Applies to both CC and NC interactions
  /// 100% correlated between near and far detectors
  class EnergyScaleHadSyst: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleHad";}
    std::string LatexName() const override {return "Hadronic Energy Scale";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      restore.Add(sr->dune.Ev_reco_numu);

      const double scale = 1 + .15*sigma;
      // Checks if ND
      if(sr->dune.run == 1){
	if(sr->dune.ccnc == 1 || sr->dune.ccnc == 0){
	  double YCalc = 1. - (sr->dune.Elep/sr->dune.Ev);
	  sr->dune.Ev_reco = sr->dune.Ev_reco * YCalc * scale + (sr->dune.Ev_reco * (1. - YCalc));
	}
	else {}
      }
      // Otherwise is FD
      else {
	if(sr->dune.ccnc == 1 || sr->dune.ccnc == 0){
	  double Y = sr->dune.Y;
	  sr->dune.Ev_reco = sr->dune.Ev_reco * Y * scale + (sr->dune.Ev_reco * (1 - Y));
	  sr->dune.Ev_reco_nue  = sr->dune.Ev_reco_nue * Y * scale + (sr->dune.Ev_reco_nue * (1 - Y));
	  sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * Y * scale + (sr->dune.Ev_reco_numu * (1 - Y));
	}
	else {}
      }
    }
  };

  static const EnergyScaleHadSyst kEnergyScaleHadSyst;


  /// 2% muon energy scale syst, applies to FD only
  class EnergyScaleMuSystFD: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleMuFD";}
    std::string LatexName() const override {return "Muon Energy Scale Far Detector";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_numu);

      const double scale = 1 + .02*sigma;

      // Checks if ND
      if(sr->dune.run == 1) { } // Does nothing
      // Otherwise is FD
      else {
	if(sr->dune.ccnc == 0 && abs(sr->dune.neu) == 14){
	  double Y = sr->dune.Y;
	  sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - Y) * scale + (sr->dune.Ev_reco * Y);
	  sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (1 - Y) * scale + (sr->dune.Ev_reco_numu * Y);
	}
	else {}
      }
    }
  };

  static const EnergyScaleMuSystFD kEnergyScaleMuSystFD;


  /// 2% muon energy scale systematic in ND only
  class EnergyScaleMuSystND: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleMuND";}
    std::string LatexName() const override {return "Muon Energy Scale Near Detector";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_numu);

      const double scale = 1 + .02*sigma;

      // Checks if ND
      if(sr->dune.run == 1 && abs(sr->dune.neu) == 14 && sr->dune.ccnc == 0){
	double YCalc = 1 - (sr->dune.Elep/sr->dune.Ev); 
	sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - YCalc) * scale + (sr->dune.Ev_reco * YCalc);
      } 
      // Otherwise is FD
      else { } // Do nothing
    }
  };

  static const EnergyScaleMuSystND kEnergyScaleMuSystND;


  /// 2% electron energy scale systematic in FD only
  class EnergyScaleESystFD: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleEFD";}
    std::string LatexName() const override {return "Electron Energy Scale Far Detector";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);

      const double scale = 1 + .02*sigma;

      // Checks if ND
      if(sr->dune.run == 1) { } // Does nothing
      // Otherwise is FD
      else {
	if(sr->dune.ccnc == 0 && abs(sr->dune.neu) == 12){
	  double Y = sr->dune.Y;
	  sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - Y) * scale + (sr->dune.Ev_reco * Y);
	  sr->dune.Ev_reco_nue = sr->dune.Ev_reco_nue * (1 - Y) * scale + (sr->dune.Ev_reco_nue * Y);
	}
	else {}
      }
    }
  };

  static const EnergyScaleESystFD kEnergyScaleESystFD;


  /// 2% muon energy scale systematic in ND only
  class EnergyScaleESystND: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleEND";}
    std::string LatexName() const override {return "Electron Energy Scale Near Detector";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);

      const double scale = 1 + .02*sigma;

      // Checks if ND
      if(sr->dune.run == 1 && abs(sr->dune.neu) == 12 && sr->dune.ccnc == 0){
	double YCalc = 1 - (sr->dune.Elep/sr->dune.Ev); 
	sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - YCalc) * scale + (sr->dune.Ev_reco * YCalc);
      } 
      // Otherwise is FD
      else { } // Do nothing
    }
  };

  static const EnergyScaleESystND kEnergyScaleESystND;


  /// 15% hadronic energy scale systematic in far detector only
  class EnergyScaleHadSystFD: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleHadFD";}
    std::string LatexName() const override {return "Hadronic Energy Scale Far Detector";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      restore.Add(sr->dune.Ev_reco_numu);

      const double scale = 1 + .15*sigma;

      // Checks if ND
      if(sr->dune.run == 1) { } // Does nothing
      // Otherwise is FD
      else {
	if(sr->dune.ccnc == 0 || sr->dune.ccnc == 1){
	  double Y = sr->dune.Y;
	  sr->dune.Ev_reco = sr->dune.Ev_reco * Y * scale + (sr->dune.Ev_reco * (1 - Y));
	  sr->dune.Ev_reco_nue = sr->dune.Ev_reco_nue * Y * scale + (sr->dune.Ev_reco_nue * (1 - Y));
	  sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * Y * scale + (sr->dune.Ev_reco_numu * (1 - Y));
	}
	else {}
      }
    }
  };

  static const EnergyScaleHadSystFD kEnergyScaleHadSystFD;


  /// 15% hadronic energy scale systematic in near detector only
  class EnergyScaleHadSystND: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleHadND";}
    std::string LatexName() const override {return "Hadronic Energy Scale Near Detector";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);

      const double scale = 1 + .15*sigma;
      // Checks if ND
      if(sr->dune.run == 1){
	double YCalc = 1. - (sr->dune.Elep/sr->dune.Ev);
	sr->dune.Ev_reco = sr->dune.Ev_reco * YCalc * scale + (sr->dune.Ev_reco * (1. - YCalc));
      }
      // Otherwise is FD
      else { } // Do nothing       
    }
  };

  static const EnergyScaleHadSystND kEnergyScaleHadSystND;
}
