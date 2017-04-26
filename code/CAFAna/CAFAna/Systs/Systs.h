#pragma once

#include "CAFAna/Core/ISyst.h"
#include "StandardRecord/StandardRecord.h"
#include "CAFAna/Core/Utilities.h"

#include "TFile.h"
#include "TH1.h"
#include "TRandom3.h"

namespace ana
{

  /// Absolute energy scale systematic
  class EnergyScaleSyst: public ISyst
  {
  public:
    std::set<std::string> Requires() const override
    {
      return {"dune.Ev_reco"};
    }
    std::string ShortName() const override {return "eScale";}
    std::string LatexName() const override {return "Energy Scale";}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);

      const double scale = 1 + .02*sigma;
      sr->dune.Ev_reco *= scale;
    }
  };

  static const EnergyScaleSyst kEnergyScaleSyst;


  /// Energy resolution systematic
  class EnergyResSyst: public ISyst
  {
  public:
    std::set<std::string> Requires() const override
      {
	return {"dune.Ev","dune.Ev_reco"};
      }
    std::string ShortName() const override {return "eRes";}
    std::string LatexName() const override {return "Energy Resolution";}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      const double scale = 1 + .06*sigma;
      const float trueE = sr->dune.Ev;

      restore.Add(sr->dune.Ev_reco);

      sr->dune.Ev_reco = trueE + scale*(sr->dune.Ev_reco - trueE);

    }
  };

  static const EnergyResSyst kEnergyResSyst;



  /// 5% normalization syst for NC on numu analysis
  class NCSyst: public ISyst
  {
  public:
    std::set<std::string> Requires() const override
      {
	return {"dune.Ev","dune.Ev_reco", "dune.ccnc"};
      }
    std::string ShortName() const override {return "NC";}
    std::string LatexName() const override {return "NC Norm Syst";}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(sr->dune.ccnc==1) weight *= 1 + .05*sigma;
    }
  };

  static const NCSyst kNCSyst;



  /// 5% normalization syst for NC on numu analysis
  class NCSyst2: public ISyst
  {
  public:
    std::set<std::string> Requires() const override
      {
	return {"dune.Ev","dune.Ev_reco", "dune.ccnc"};
      }
    std::string ShortName() const override {return "NC";}
    std::string LatexName() const override {return "NC Norm Syst";}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(sr->dune.ccnc==1) weight *= 1 + .5*sigma;
    }
  };

  static const NCSyst2 kNCSyst2;



  /// 5% normalization syst for NC on numu analysis
  class NutauSyst: public ISyst
  {
  public:
    std::set<std::string> Requires() const override
      {
	return {"dune.Ev","dune.Ev_reco", "dune.ccnc", "dune.neu"};
      }
    std::string ShortName() const override {return "nutau";}
    std::string LatexName() const override {return "nutau Norm Syst";}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(sr->dune.ccnc==0 && abs(sr->dune.neu)==16) weight *= 1 + .2*sigma;
    }
  };

  static const NutauSyst kNutauSyst;



  /// 5% normalization syst for NC on numu analysis
  class NumuNCSyst: public ISyst
  {
  public:
    std::set<std::string> Requires() const override
      {
	return {"dune.Ev","dune.Ev_reco", "dune.ccnc"};
      }
    std::string ShortName() const override {return "numuNC";}
    std::string LatexName() const override {return "Numu NC Syst";}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(sr->dune.ccnc==1) weight *= 1 + .05*sigma;
    }
  };

  static const NumuNCSyst kNumuNCSyst;



  /// 5% normalization syst for NC on numu analysis
  class NueNumuSyst: public ISyst
  {
  public:
    std::set<std::string> Requires() const override
      {
	return {"dune.Ev","dune.Ev_reco", "dune.ccnc", "dune.neu"};
      }
    std::string ShortName() const override {return "nuenumuNC";}
    std::string LatexName() const override {return "Nue Numu Norm Syst";}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(sr->dune.ccnc==0 && abs(sr->dune.neu)==14) weight *= 1 + .05*sigma;
    }
  };

  static const NueNumuSyst kNueNumuSyst;


  /// 5% normalization syst for NC on numu analysis
  class NueBeamSyst: public ISyst
  {
  public:
    std::set<std::string> Requires() const override
      {
	return {"dune.Ev","dune.Ev_reco", "dune.ccnc", "dune.neu", "dune.beamPdg"};
      }
    std::string ShortName() const override {return "nuebeam";}
    std::string LatexName() const override {return "Nue Beam Norm Syst";}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(sr->dune.ccnc==0 && fabs(sr->dune.neu==12) && fabs(sr->dune.beamPdg)==12) weight *= 1 + .05*sigma;
    }
  };

  static const NueBeamSyst kNueBeamSyst;

}
