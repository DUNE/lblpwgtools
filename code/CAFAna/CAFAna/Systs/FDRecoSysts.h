// FDRecoSysts.h
// Uses genie and neut file kinematic discrepancies to simulate reconstruction inefficiencies
// Use make_FD_reco_systs.C to make the histograms
#pragma once

#include "CAFAna/Core/ISyst.h"
#include "StandardRecord/StandardRecord.h"
#include "CAFAna/Core/Utilities.h"

#include "TFile.h"
#include "TH1.h"
#include "TRandom3.h"

namespace ana {

  class FDRecoSyst: public ISyst
  {
  public:
    // Kinematic variable
    enum Variable{kX, kY, kW, kQ2};

  FDRecoSyst(Variable v) : ISyst("FDRecoSyst", "Far Detector Reconstruction Syst") 
      {
	assert(v==kX || v==kY || v==kW || v==kQ2);
	var = v;
	//TFile f((FindCAFAnaDir()+"/Systs/modelComp.root").c_str());
	TFile *f = new TFile((FindCAFAnaDir()+"/Systs/modelComp.root").c_str(), "read");
	assert(!f->IsZombie());
	if (var == kX) {
	  hist = (TH2*)f->Get("hx_ratiofhc");
	}
	else if (var == kY) {
	  hist = (TH2*)f->Get("hy_ratiofhc");
	}
	else if (var == kW) {
	  hist = (TH2*)f->Get("hw_ratiofhc");
	}
	else if (var == kQ2) {
	  hist = (TH2*)f->Get("hq2_ratiofhc");
	}
	assert(hist);
      }
    virtual ~FDRecoSyst();

    virtual void Shift(double sigma,
		       Restorer& restore,
		       caf::StandardRecord* sr,
		       double& weight) const override; 

  protected:
    TH2* hist;
    TFile *f;
    Variable var;
  };

  //  extern const FDRecoSyst kFDRecoSyst(v);
}
