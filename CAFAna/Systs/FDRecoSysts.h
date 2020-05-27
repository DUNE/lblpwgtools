// FDRecoSysts.h
// Uses genie and neut file kinematic discrepancies to simulate reconstruction inefficiencies
// Use make_FD_reco_systs.C to make the histograms
#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Cuts/AnaCuts.h"

#include "StandardRecord/Proxy/SRProxy.h"

#include "TFile.h"
#include "TH2.h"

#include <cassert>

namespace ana {

  class FDRecoNumuSyst: public ISyst
  {
  public:
  FDRecoNumuSyst() : ISyst("FDRecoNumuSyst", "Far Detector Numu Reconstruction Syst") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::SRProxy* sr,
	       double& weight) const override 
    {
      // Load histograms if they have not been loaded already
      if (!hist) {
	TFile f((FindCAFAnaDir()+"/Systs/modelComp.root").c_str());
	assert(!f.IsZombie());
	hist = (TH2*)f.Get("hYratio_neutfhc_geniefhc");
	hist->SetDirectory(0);
	assert(hist);
      }
      // Passes FD selection cut
      if (sr->isFD && kPassFD_CVN_NUMU(sr)) {
	int EBin   = hist->GetXaxis()->FindBin(sr->Ev);
	int VarBin = hist->GetYaxis()->FindBin(sr->Y);
	double w   = hist->GetBinContent(EBin, VarBin);
	weight    *= 1. + sigma * (1. - w) ;
      }
    }
    
  protected:
    mutable TH2* hist;
  }; 

  extern const FDRecoNumuSyst kFDRecoNumuSyst;

  // Nue reco syst
  class FDRecoNueSyst: public ISyst
  {
  public:
  FDRecoNueSyst() : ISyst("FDRecoNueSyst", "Far Detector Nue Reconstruction Syst") {}

    void Shift(double sigma,
	       Restorer& restore,
	       caf::SRProxy* sr,
	       double& weight) const override 
    {
      // Load histograms if they have not been loaded already
      if (!hist) {
	TFile f((FindCAFAnaDir()+"/Systs/modelComp.root").c_str());
	assert(!f.IsZombie());
	hist = (TH2*)f.Get("hYratio_neutfhc_geniefhc");
	hist->SetDirectory(0);
	assert(hist);
      }
      // Passes FD nue selection
      if (sr->isFD && kPassFD_CVN_NUE(sr)) {
	int EBin   = hist->GetXaxis()->FindBin(sr->Ev);
	int VarBin = hist->GetYaxis()->FindBin(sr->Y);
	double w   = hist->GetBinContent(EBin, VarBin);
	weight    *= 1. + sigma * (1. - w) ;
      }
    }
    
  protected:
    mutable TH2* hist;
  };

  extern const FDRecoNueSyst kFDRecoNueSyst;

  // Fiducial volume normalization for nues
  class FVNueFDSyst: public ISyst
  {
  public:
  FVNueFDSyst() : ISyst("FVNueFD", "Far Detector Nue Fiducial Volume") {}
    void Shift(double sigma, 
	       Restorer& restore,
	       caf::SRProxy* sr,
	       double& weight) const override
    {
      const double scale = 1. + 0.01 * sigma;
      if (sr->isFD && kPassFD_CVN_NUE(sr)) {
	weight *= scale;
      }
    }
  };

  extern const FVNueFDSyst kFVNueFDSyst;

  // Fiducial volume normalization for numus
  class FVNumuFDSyst: public ISyst
  {
  public:
  FVNumuFDSyst() : ISyst("FVNumuFD", "Far Detector Numu Fiducial Volume") {}
    void Shift(double sigma, 
	       Restorer& restore,
	       caf::SRProxy* sr,
	       double& weight) const override
    {
      const double scale = 1. + 0.01 * sigma;
      if (sr->isFD && kPassFD_CVN_NUMU(sr)) {
	weight *= scale;
      }
    }
  };

  extern const FVNumuFDSyst kFVNumuFDSyst;

  // Fiducial volume normalization
  // Correlated between nues and numus
  class FVCorrFDSyst: public ISyst
  {
  public:
  FVCorrFDSyst() : ISyst("FVCorrFD", "Far Detector Correlated Fiducial Volume") {}
    void Shift(double sigma, 
	       Restorer& restore,
	       caf::SRProxy* sr,
	       double& weight) const override
    {
      const double scale = 1. + 0.01 * sigma;
      if (sr->isFD && (kPassFD_CVN_NUMU(sr) || kPassFD_CVN_NUMU(sr))) {
	weight *= scale;
      }
    }
  };

  extern const FVCorrFDSyst kFVCorrFDSyst;


  struct FDRecoSystVector: public std::vector<const ISyst*>
  {
  };

  FDRecoSystVector GetFDRecoSysts();
  
}
