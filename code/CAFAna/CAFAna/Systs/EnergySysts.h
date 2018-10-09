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
      if(!sr->dune.isFD){
        if(abs(sr->dune.nuPDG) == 14 && sr->dune.isCC){
          double YCalc = 1 - (sr->dune.Elep/sr->dune.Ev);
          sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - YCalc) * scale + (sr->dune.Ev_reco * YCalc);
        }
      }
      // Otherwise is FD
      else {
	if(sr->dune.isCC && abs(sr->dune.nuPDG) == 14){
	  double Y = sr->dune.Y;
	  sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - Y) * scale + (sr->dune.Ev_reco * Y);
	  sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (1 - Y) * scale + (sr->dune.Ev_reco_numu * Y);
	}
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
      if(!sr->dune.isFD){
        if(abs(sr->dune.nuPDG) == 12 && sr->dune.isCC){
          double YCalc = 1 - (sr->dune.Elep/sr->dune.Ev);
          sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - YCalc) * scale + (sr->dune.Ev_reco * YCalc);
        }
      }
      // Otherwise is FD
      else {
        if(sr->dune.isCC && abs(sr->dune.nuPDG) == 12){
          double Y = sr->dune.Y;
          sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - Y) * scale + (sr->dune.Ev_reco * Y);
          sr->dune.Ev_reco_nue = sr->dune.Ev_reco_nue * (1 - Y) * scale + (sr->dune.Ev_reco_nue * Y);
        }
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
      if(!sr->dune.isFD){
        double YCalc = 1. - (sr->dune.Elep/sr->dune.Ev);
        sr->dune.Ev_reco = sr->dune.Ev_reco * YCalc * scale + (sr->dune.Ev_reco * (1. - YCalc));
      }
      // Otherwise is FD
      else {
        double Y = sr->dune.Y;
        sr->dune.Ev_reco = sr->dune.Ev_reco * Y * scale + (sr->dune.Ev_reco * (1 - Y));
        sr->dune.Ev_reco_nue  = sr->dune.Ev_reco_nue * Y * scale + (sr->dune.Ev_reco_nue * (1 - Y));
        sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * Y * scale + (sr->dune.Ev_reco_numu * (1 - Y));
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

      if(sr->dune.isFD){
	if(sr->dune.isCC && abs(sr->dune.nuPDG) == 14){
	  double Y = sr->dune.Y;
	  sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - Y) * scale + (sr->dune.Ev_reco * Y);
	  sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (1 - Y) * scale + (sr->dune.Ev_reco_numu * Y);
	}
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

      if(!sr->dune.isFD && abs(sr->dune.nuPDG) == 14 && sr->dune.isCC){
	double YCalc = 1 - (sr->dune.Elep/sr->dune.Ev); 
	sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - YCalc) * scale + (sr->dune.Ev_reco * YCalc);
      } 
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

      if(sr->dune.isFD){
	if(sr->dune.isCC && abs(sr->dune.nuPDG) == 12){
	  double Y = sr->dune.Y;
	  sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - Y) * scale + (sr->dune.Ev_reco * Y);
	  sr->dune.Ev_reco_nue = sr->dune.Ev_reco_nue * (1 - Y) * scale + (sr->dune.Ev_reco_nue * Y);
	}
      }
    }
  };

  static const EnergyScaleESystFD kEnergyScaleESystFD;


  /// 2% electron energy scale systematic in ND only
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

      if(!sr->dune.isFD && abs(sr->dune.nuPDG) == 12 && sr->dune.isCC){
	double YCalc = 1 - (sr->dune.Elep/sr->dune.Ev); 
	sr->dune.Ev_reco = sr->dune.Ev_reco * (1 - YCalc) * scale + (sr->dune.Ev_reco * YCalc);
      }
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

      if(sr->dune.isFD){
        double Y = sr->dune.Y;
        sr->dune.Ev_reco = sr->dune.Ev_reco * Y * scale + (sr->dune.Ev_reco * (1 - Y));
        sr->dune.Ev_reco_nue = sr->dune.Ev_reco_nue * Y * scale + (sr->dune.Ev_reco_nue * (1 - Y));
        sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * Y * scale + (sr->dune.Ev_reco_numu * (1 - Y));
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
      if(!sr->dune.isFD){
	double YCalc = 1 - (sr->dune.Elep/sr->dune.Ev);
	sr->dune.Ev_reco = sr->dune.Ev_reco * YCalc * scale + (sr->dune.Ev_reco * (1. - YCalc));
      }
    }
  };

  static const EnergyScaleHadSystND kEnergyScaleHadSystND;

  // Adjustable energy syst
  // For muonic correlated component of energy
  class EnergyScaleMuSystAdj: public ISyst
  {
  public:
    std::string ShortName() const override{return "eScaleMuAdj";}
    std::string LatexName() const override{return "Adjustable muonic energy scale";}

    EnergyScaleMuSystAdj(double FracCorr)
      : fFracCorr(FracCorr)
    {
    }
    double fFracCorr;

    void Shift(double sigma,
	       //double percentCorr,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);      
      restore.Add(sr->dune.Ev_reco_numu);
      const double scale = 1 + .02*sigma;
      // Checks if ND
      if(!sr->dune.isFD){
        if(abs(sr->dune.nuPDG) == 14 && sr->dune.isCC){
          double YCalc = 1 - (sr->dune.Elep/sr->dune.Ev);
          sr->dune.Ev_reco = fFracCorr * ( sr->dune.Ev_reco * (1 - YCalc) * scale + sr->dune.Ev_reco * YCalc ) + (1 - fFracCorr) * sr->dune.Ev_reco;
        }
      }
      // Otherwise is FD
      else{
	if(abs(sr->dune.nuPDG) == 14 && sr->dune.isCC){
	  double Y = sr->dune.Y;
	  sr->dune.Ev_reco = fFracCorr * ( sr->dune.Ev_reco * (1 - Y) * scale + sr->dune.Ev_reco * Y ) + (1 - fFracCorr) * sr->dune.Ev_reco;
	  sr->dune.Ev_reco_numu = fFracCorr * ( sr->dune.Ev_reco_numu * (1 - Y) * scale + sr->dune.Ev_reco_numu * Y ) + (1 - fFracCorr) * sr->dune.Ev_reco_numu;
	}
      }
    }
  };

  // Adjustable energy syst
  // For electronic correlated component of energy
  class EnergyScaleESystAdj: public ISyst
  {
  public:
    std::string ShortName() const override{return "eScaleEAdj";}
    std::string LatexName() const override{return "Adjustable electronic energy scale";}

    EnergyScaleESystAdj(double FracCorr)
      : fFracCorr(FracCorr)
    {
    }
    // Fraction of energy to be correlated
    double fFracCorr;

    void Shift(double sigma,
	       //double percentCorr,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      const double scale = 1 + .02*sigma;
      // Checks if ND
      if(!sr->dune.isFD){
        if(abs(sr->dune.nuPDG) == 12 && sr->dune.isCC){
          double YCalc = 1 - (sr->dune.Elep/sr->dune.Ev);
          sr->dune.Ev_reco = fFracCorr * ( sr->dune.Ev_reco * (1 - YCalc) * scale + sr->dune.Ev_reco * YCalc ) + (1 - fFracCorr) * sr->dune.Ev_reco;
        }
      }
      // Otherwise is FD
      else{
	if(abs(sr->dune.nuPDG) == 12 && sr->dune.isCC){
	  double Y = sr->dune.Y;
	  sr->dune.Ev_reco = fFracCorr * ( sr->dune.Ev_reco * (1 - Y) * scale + sr->dune.Ev_reco * Y) + (1 - fFracCorr) * sr->dune.Ev_reco;
	  sr->dune.Ev_reco_nue = fFracCorr * ( sr->dune.Ev_reco_nue * (1 - Y) * scale + sr->dune.Ev_reco_nue * Y ) + (1 - fFracCorr) * sr->dune.Ev_reco_nue;
	}
      }
    }
  };

  // Adjustable energy syst
  // For hadronic correlated component of energy
  class EnergyScaleHadSystAdj: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleHadAdj";}
    std::string LatexName() const override {return "Adjustable hadronic Energy Scale";}

    EnergyScaleHadSystAdj(double FracCorr)
      : fFracCorr(FracCorr)
    {
    }
    double fFracCorr;

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      restore.Add(sr->dune.Ev_reco_numu);

      const double scale = 1 + .15*sigma;
      // Checks if ND
      if(!sr->dune.isFD){
        double YCalc = 1. - (sr->dune.Elep/sr->dune.Ev);
        sr->dune.Ev_reco = fFracCorr * sr->dune.Ev_reco * YCalc * scale + (1 - fFracCorr) * sr->dune.Ev_reco * (1. - YCalc);
      }
      // Otherwise is FD
      else {
        double Y = sr->dune.Y;
        sr->dune.Ev_reco = fFracCorr * ( sr->dune.Ev_reco * Y * scale + sr->dune.Ev_reco * (1 - Y) ) + (1 - fFracCorr) * sr->dune.Ev_reco;
        sr->dune.Ev_reco_nue  = fFracCorr * ( sr->dune.Ev_reco_nue * Y * scale + sr->dune.Ev_reco_nue * (1 - Y) ) + (1 - fFracCorr) * sr->dune.Ev_reco_nue;
        sr->dune.Ev_reco_numu = fFracCorr * ( sr->dune.Ev_reco_numu * Y * scale + sr->dune.Ev_reco_numu * (1 - Y) ) + (1 - fFracCorr) * sr->dune.Ev_reco_numu;
      }
    }
  };

  // Adjustable energy syst
  // For muonic component of energy
  // Near detector only
  class EnergyScaleMuSystAdjND: public ISyst
  {
  public:
    std::string ShortName() const override{return "eScaleMuAdjND";}
    std::string LatexName() const override{return "Adjustable muonic energy scale ND only";}

    EnergyScaleMuSystAdjND(double FracUncorr)
      : fFracUncorr(FracUncorr)
    {
    }
    // Fraction of energy systematic uncorrelated
    double fFracUncorr;

    void Shift(double sigma,
	       //double percentCorr,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);      
      const double scale = 1 + .02*sigma;

      if(!sr->dune.isFD && abs(sr->dune.nuPDG) == 14 && sr->dune.isCC){
	double YCalc = 1 - (sr->dune.Elep/sr->dune.Ev);
	sr->dune.Ev_reco = fFracUncorr * ( sr->dune.Ev_reco * (1 - YCalc) * scale + sr->dune.Ev_reco * YCalc ) + (1 - fFracUncorr) * sr->dune.Ev_reco;
      }
    }
  };

  // Adjustable energy syst
  // For muonic component of energy
  // Far detector only
  class EnergyScaleMuSystAdjFD: public ISyst
  {
  public:
    std::string ShortName() const override{return "eScaleMuAdjFD";}
    std::string LatexName() const override{return "Adjustable muonic energy scale FD only";}

    EnergyScaleMuSystAdjFD(double FracUncorr)
      : fFracUncorr(FracUncorr)
    {
    }
    double fFracUncorr;

    void Shift(double sigma,
	       //double percentCorr,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);      
      restore.Add(sr->dune.Ev_reco_numu);
      const double scale = 1 + .02*sigma;

      if(sr->dune.isFD){
	if(abs(sr->dune.nuPDG) == 14 && sr->dune.isCC){
	  double Y = sr->dune.Y;
	  sr->dune.Ev_reco      = fFracUncorr * ( sr->dune.Ev_reco      * (1 - Y) * scale + sr->dune.Ev_reco      * Y ) + (1 - fFracUncorr) * sr->dune.Ev_reco;
	  sr->dune.Ev_reco_numu = fFracUncorr * ( sr->dune.Ev_reco_numu * (1 - Y) * scale + sr->dune.Ev_reco_numu * Y ) + (1 - fFracUncorr) * sr->dune.Ev_reco_numu;
	}
      }
    }
  };

  // Adjustable energy syst
  // For electronic component of energy
  // Near detector only
  class EnergyScaleESystAdjND: public ISyst
  {
  public:
    std::string ShortName() const override{return "eScaleEAdjND";}
    std::string LatexName() const override{return "Adjustable electronic energy scale ND only";}

    EnergyScaleESystAdjND(double FracUncorr)
      : fFracUncorr(FracUncorr)
    {
    }
    // Fraction of energy to be uncorrelated
    double fFracUncorr;

    void Shift(double sigma,
	       //double percentCorr,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      const double scale = 1 + .02*sigma;
      if(!sr->dune.isFD){
        if(abs(sr->dune.nuPDG) == 12 && sr->dune.isCC){
          double YCalc = 1 - (sr->dune.Elep/sr->dune.Ev);
          sr->dune.Ev_reco = fFracUncorr * ( sr->dune.Ev_reco * (1 - YCalc) * scale + sr->dune.Ev_reco * YCalc ) + (1 - fFracUncorr) * sr->dune.Ev_reco;
        }
      }
    }
  };

  // Adjustable energy syst
  // For electronic component of energy
  // Far detector only
  class EnergyScaleESystAdjFD: public ISyst
  {
  public:
    std::string ShortName() const override{return "eScaleEAdjFD";}
    std::string LatexName() const override{return "Adjustable electronic energy scale FD only";}

    EnergyScaleESystAdjFD(double FracUncorr)
      : fFracUncorr(FracUncorr)
    {
    }
    // Fraction of energy to be unorrelated
    double fFracUncorr;

    void Shift(double sigma,
	       //double percentCorr,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      const double scale = 1 + .02*sigma;

      if(sr->dune.isFD){
	if(abs(sr->dune.nuPDG) == 12 && sr->dune.isCC){
	  double Y = sr->dune.Y;
	  sr->dune.Ev_reco = fFracUncorr * ( sr->dune.Ev_reco * (1 - Y) * scale + sr->dune.Ev_reco * Y) + (1 - fFracUncorr) * sr->dune.Ev_reco;
	  sr->dune.Ev_reco_nue = fFracUncorr * ( sr->dune.Ev_reco_nue * (1 - Y) * scale + sr->dune.Ev_reco_nue * Y ) + (1 - fFracUncorr) * sr->dune.Ev_reco_nue;
	}
      }
    }
  };

  // Adjustable energy syst
  // For hadronic component of energy
  // Near detector only
  class EnergyScaleHadSystAdjND: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleHadAdjND";}
    std::string LatexName() const override {return "Adjustable hadronic Energy Scale ND only";}

    EnergyScaleHadSystAdjND(double FracUncorr)
      : fFracUncorr(FracUncorr)
    {
    }
    // Fraction of energy uncorrelated
    double fFracUncorr;

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);

      const double scale = 1 + .15*sigma;

      if(!sr->dune.isFD){
        double YCalc = 1. - (sr->dune.Elep/sr->dune.Ev);
        sr->dune.Ev_reco = fFracUncorr * ( sr->dune.Ev_reco * YCalc * scale + sr->dune.Ev_reco * (1. - YCalc) ) + (1 - fFracUncorr) * sr->dune.Ev_reco;
      }
    }
  };

  // Adjustable energy syst
  // For hadronic component of energy
  // Far detector only
  class EnergyScaleHadSystAdjFD: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleHadAdjFD";}
    std::string LatexName() const override {return "Adjustable hadronic Energy Scale FD only";}

    EnergyScaleHadSystAdjFD(double FracUncorr)
      : fFracUncorr(FracUncorr)
    {
    }  
    // Fraction of energy uncorrelated
    double fFracUncorr;

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      restore.Add(sr->dune.Ev_reco_numu);

      const double scale = 1 + .15*sigma;

      if(sr->dune.isFD){
        double Y = sr->dune.Y;
        sr->dune.Ev_reco = fFracUncorr * ( sr->dune.Ev_reco * Y * scale + sr->dune.Ev_reco * (1 - Y) ) + (1 - fFracUncorr) * sr->dune.Ev_reco;
        sr->dune.Ev_reco_nue  = fFracUncorr * ( sr->dune.Ev_reco_nue * Y * scale + sr->dune.Ev_reco_nue * (1 - Y) ) + (1 - fFracUncorr) * sr->dune.Ev_reco_nue;
        sr->dune.Ev_reco_numu = fFracUncorr * ( sr->dune.Ev_reco_numu * Y * scale + sr->dune.Ev_reco_numu * (1 - Y) ) + (1 - fFracUncorr) * sr->dune.Ev_reco_numu;
      }
    }
  };

}
