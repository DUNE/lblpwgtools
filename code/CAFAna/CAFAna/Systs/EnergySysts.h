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

  /// Energy scale systematics for hadronic final state particles
  // Assume 25% of the neutron energy is visible - this is fairly crude and should be changed later
  // Neutron energy scale
  class EnergyScaleNSyst: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleN";}
    std::string LatexName() const override {return "Neutron Energy Scale";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      restore.Add(sr->dune.Ev_reco_numu);
      restore.Add(sr->dune.RecoHadEnNumu);
      restore.Add(sr->dune.RecoHadEnNue);

      const double scale = 1 + .25*sigma;
      const double fracN = ((sr->dune.eN*.25) / sr->dune.Ev);
      const double fracNY = ((sr->dune.eN*.25) / (sr->dune.Ev*sr->dune.Y));

      sr->dune.Ev_reco      = sr->dune.Ev_reco * (fracN * scale + (1 - fracN));
      sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (fracN * scale + (1 - fracN));
      sr->dune.Ev_reco_nue  = sr->dune.Ev_reco_nue * (fracN * scale + (1 - fracN));
      sr->dune.RecoHadEnNumu = sr->dune.RecoHadEnNumu * (fracNY * scale + (1 - fracNY));
      sr->dune.RecoHadEnNue  = sr->dune.RecoHadEnNue * (fracNY * scale + (1 - fracNY));
    }
  };

  static const EnergyScaleNSyst kEnergyScaleNSyst;  

  // Proton energy scale
  class EnergyScalePSyst: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScaleP";}
    std::string LatexName() const override {return "Proton Energy Scale";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      restore.Add(sr->dune.Ev_reco_numu);
      restore.Add(sr->dune.RecoHadEnNumu);
      restore.Add(sr->dune.RecoHadEnNue);

      const double scale = 1 + .1 * sigma;
      const double fracP = (sr->dune.eP / sr->dune.Ev);
      const double fracPY = (sr->dune.eP / (sr->dune.Ev * sr->dune.Y));

      sr->dune.Ev_reco      = sr->dune.Ev_reco * (fracP * scale + (1 - fracP));
      sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (fracP * scale + (1 - fracP));
      sr->dune.Ev_reco_nue  = sr->dune.Ev_reco_nue * (fracP * scale + (1 - fracP));
      sr->dune.RecoHadEnNumu = sr->dune.RecoHadEnNumu * (fracPY * scale + (1 - fracPY));
      sr->dune.RecoHadEnNue  = sr->dune.RecoHadEnNue * (fracPY * scale + (1 - fracPY));
    }
  };

  static const EnergyScalePSyst kEnergyScalePSyst;  

  // Pi plus energy scale
  class EnergyScalePipSyst: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScalePip";}
    std::string LatexName() const override {return "Pi plus Energy Scale";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      restore.Add(sr->dune.Ev_reco_numu);
      restore.Add(sr->dune.RecoHadEnNumu);
      restore.Add(sr->dune.RecoHadEnNue);

      const double scale = 1 + .05 * sigma;
      const double fracPip = (sr->dune.ePip / sr->dune.Ev);
      const double fracPipY = (sr->dune.ePip / (sr->dune.Ev*sr->dune.Y));

      sr->dune.Ev_reco      = sr->dune.Ev_reco * (fracPip * scale + (1 - fracPip));
      sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (fracPip * scale + (1 - fracPip));
      sr->dune.Ev_reco_nue  = sr->dune.Ev_reco_nue * (fracPip * scale + (1 - fracPip));
      sr->dune.RecoHadEnNumu = sr->dune.RecoHadEnNumu * (fracPipY * scale + (1 - fracPipY));
      sr->dune.RecoHadEnNue  = sr->dune.RecoHadEnNue * (fracPipY * scale + (1 - fracPipY));
    }
  };

  static const EnergyScalePipSyst kEnergyScalePipSyst;

  // Pi minus energy scale
  class EnergyScalePimSyst: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScalePim";}
    std::string LatexName() const override {return "Pi minus Energy Scale";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      restore.Add(sr->dune.Ev_reco_numu);
      restore.Add(sr->dune.RecoHadEnNumu);
      restore.Add(sr->dune.RecoHadEnNue);

      const double scale = 1 + .05 * sigma;
      const double fracPim = (sr->dune.ePim / sr->dune.Ev);
      const double fracPimY = (sr->dune.ePim / (sr->dune.Ev*sr->dune.Y));

      sr->dune.Ev_reco      = sr->dune.Ev_reco * (fracPim * scale + (1 - fracPim));
      sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (fracPim * scale + (1 - fracPim));
      sr->dune.Ev_reco_nue  = sr->dune.Ev_reco_nue * (fracPim * scale + (1 - fracPim));
      sr->dune.RecoHadEnNumu = sr->dune.RecoHadEnNumu * (fracPimY * scale + (1 - fracPimY));
      sr->dune.RecoHadEnNue  = sr->dune.RecoHadEnNue * (fracPimY * scale + (1 - fracPimY));
    }
  };

  static const EnergyScalePimSyst kEnergyScalePimSyst;

  // Pi0 energy scale
  class EnergyScalePi0Syst: public ISyst
  {
  public:
    std::string ShortName() const override {return "eScalePi0";}
    std::string LatexName() const override {return "Pi0 Energy Scale";}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_nue);
      restore.Add(sr->dune.Ev_reco_numu);
      restore.Add(sr->dune.RecoHadEnNumu);
      restore.Add(sr->dune.RecoHadEnNue);

      const double scale = 1 + .1 * sigma;
      const double fracPi0 = (sr->dune.ePi0 / sr->dune.Ev);
      const double fracPi0Y = (sr->dune.ePi0 / (sr->dune.Ev*sr->dune.Y));

      sr->dune.Ev_reco      = sr->dune.Ev_reco * (fracPi0 * scale + (1 - fracPi0));
      sr->dune.Ev_reco_numu = sr->dune.Ev_reco_numu * (fracPi0 * scale + (1 - fracPi0));
      sr->dune.Ev_reco_nue  = sr->dune.Ev_reco_nue * (fracPi0 * scale + (1 - fracPi0));
      sr->dune.RecoHadEnNumu = sr->dune.RecoHadEnNumu * (fracPi0Y * scale + (1 - fracPi0Y));
      sr->dune.RecoHadEnNue  = sr->dune.RecoHadEnNue * (fracPi0Y * scale + (1 - fracPi0Y));
    }
  };

  static const EnergyScalePi0Syst kEnergyScalePi0Syst;
}
