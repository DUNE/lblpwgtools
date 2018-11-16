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
    EnergyScaleSyst() : ISyst("eScale", "Energy Scale") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      restore.Add(sr->dune.Ev_reco);
      restore.Add(sr->dune.Ev_reco_numu);
      restore.Add(sr->dune.Ev_reco_nue);

      const double scale = 1 + .02*sigma;
      sr->dune.Ev_reco *= scale;
      sr->dune.Ev_reco_numu *= scale;
      sr->dune.Ev_reco_nue *= scale;
    }
  };

  extern const EnergyScaleSyst kEnergyScaleSyst;


  /// Energy resolution systematic
  class EnergyResSyst: public ISyst
  {
  public:
    EnergyResSyst() : ISyst("eRes", "Energy Resolution") {}

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

  extern const EnergyResSyst kEnergyResSyst;



  ///ETW Rework of Normalization Systematics

  //Signal normalization

  /// 5% normalization syst for numu signal
  class NumuFHCSyst: public ISyst
  {
  public:
    NumuFHCSyst() : ISyst("numufhcnorm", "Numu FHC Norm Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(sr->dune.isCC && abs(sr->dune.nuPDG)==14 && sr->dune.run==20000001) weight *= 1 + .05*sigma;
    }
  };

  extern const NumuFHCSyst kNumuFHCSyst;

  /// 5% normalization syst for numubar
  class NumuRHCSyst: public ISyst
  {
  public:
    NumuRHCSyst() : ISyst("numurhcnorm", "Numu RHC Norm Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(sr->dune.isCC && abs(sr->dune.nuPDG)==14 && sr->dune.run==20000004) weight *= 1 + .05*sigma;
    }
  };

  extern const NumuRHCSyst kNumuRHCSyst;

  /// 2% normalization syst for nue
  class NueFHCSyst: public ISyst
  {
  public:
    NueFHCSyst() : ISyst("nuefhcnorm", "Nue FHC Norm Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(sr->dune.isCC && abs(sr->dune.nuPDG)==12 && sr->dune.run==20000002) weight *= 1 + .02*sigma;
    }
  };

  extern const NueFHCSyst kNueFHCSyst;

  /// 2% normalization syst for nuebar
  class NueRHCSyst: public ISyst
  {
  public:
    NueRHCSyst() : ISyst("nuerhcnorm", "Nue RHC Norm Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(sr->dune.isCC && abs(sr->dune.nuPDG)==12 && sr->dune.run==20000005) weight *= 1 + .02*sigma;
    }
  };

  extern const NueRHCSyst kNueRHCSyst;

  //Background normalization following CDR

  /// 10% normalization syst for NC on disappearance analysis (uncorrelated to NC on appearance analysis)
  class NCDisSyst: public ISyst
  {
  public:
    NCDisSyst() : ISyst("NCDis", "NC Dis Norm Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(!sr->dune.isCC && sr->dune.cvnnumu>0.5 && sr->dune.cvnnue<0.7) weight *= 1 + .1*sigma;
    }
  };

  extern const NCDisSyst kNCDisSyst;

  /// 5% normalization syst for NC on nue analysis, correlated with numu-CC background
  class NCAppSyst: public ISyst
  {
  public:
    NCAppSyst() : ISyst("NCApp", "NC App Norm Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if((!sr->dune.isCC && sr->dune.cvnnumu<0.5 && sr->dune.cvnnue>0.7) || (sr->dune.isCC && abs(sr->dune.nuPDG)==14)) weight *= 1 + .05*sigma;
    }
  };

  extern const NCAppSyst kNCAppSyst;

  /// 20% normalization syst for nutau - correlated for app/dis and fhc/rhc
  class NutauSyst: public ISyst
  {
  public:
    NutauSyst() : ISyst("nutau", "nutau Norm Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(sr->dune.isCC && abs(sr->dune.nuPDG)==16) weight *= 1 + .2*sigma;
    }
  };

  extern const NutauSyst kNutauSyst;

  /// 5% normalization syst for beam nues - uncorrelated in fhc/rhc
  class NueBeamFHCSyst: public ISyst
  {
  public:
    NueBeamFHCSyst() : ISyst("nuebeamfhc", "Nue Beam FHC Norm Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(sr->dune.isCC && abs(sr->dune.nuPDG) == 12 && abs(sr->dune.nuPDGunosc) == 12 && sr->dune.isFHC) weight *= 1 + .05*sigma;
    }
  };

  extern const NueBeamFHCSyst kNueBeamFHCSyst;

  /// 5% normalization syst for beam nues
  class NueBeamRHCSyst: public ISyst
  {
  public:
    NueBeamRHCSyst() : ISyst("nuebeamrhc", "Nue Beam RHC Norm Syst") {}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr, double& weight) const override
    {
      if(sr->dune.isCC && abs(sr->dune.nuPDG) == 12 && abs(sr->dune.nuPDGunosc) == 12 && !sr->dune.isFHC) weight *= 1 + .05*sigma;
    }
  };

  extern const NueBeamRHCSyst kNueBeamRHCSyst;

}
