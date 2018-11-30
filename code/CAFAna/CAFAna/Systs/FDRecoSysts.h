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

#include <cassert>

namespace ana {

  class FDRecoSyst: public ISyst
  {
  public:
    // Kinematic variable & horn current
    enum Variable{kX, kY, kW, kQ2};
    enum HC{kFHC, kRHC};
    enum Flavour{kNue, kNumu};

  FDRecoSyst(Variable v, HC h, Flavour fl) : ISyst("FDRecoSyst", "Far Detector Reconstruction Syst") 
      {
	assert(v==kX || v==kY || v==kW || v==kQ2);
	assert(h==kFHC || h==kRHC);
	assert(fl==kNue || fl==kNumu);
	var = v;
	hc = h;
	flav = fl;

	TFile *f = new TFile((FindCAFAnaDir()+"/Systs/modelComp.root").c_str(), "read");
	assert(!f->IsZombie());
	if (hc == kFHC) {
	  if (var == kX) {
	    hist = (TH2*)f->Get("hXratio_neutfhc_geniefhc");
	  }
	  else if (var == kY) {
	    hist = (TH2*)f->Get("hYratio_neutfhc_geniefhc");
	  }
	  else if (var == kW) {
	    hist = (TH2*)f->Get("hWratio_neutfhc_geniefhc");
	  }
	  else if (var == kQ2) {
	    hist = (TH2*)f->Get("hQ2ratio_neutfhc_geniefhc");
	  }
	}
	else if (hc == kRHC) {
	  if (var == kX) {
	    hist = (TH2*)f->Get("hXratio_neutrhc_genierhc");
	  }
	  else if (var == kY) {
	    hist = (TH2*)f->Get("hYratio_neutrhc_genierhc");
	  }
	  else if (var == kW) {
	    hist = (TH2*)f->Get("hWratio_neutrhc_genierhc");
	  }
	  else if (var == kQ2) {
	    hist = (TH2*)f->Get("hQ2ratio_neutrhc_genierhc");
	  }
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
    HC hc;
    Flavour flav;
  };

  //  extern const FDRecoSyst kFDRecoSyst(v);
}
