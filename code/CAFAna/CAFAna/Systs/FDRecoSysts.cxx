#include "CAFAna/Systs/FDRecoSysts.h"
#include "TH2.h"
#include "TFile.h"

namespace ana {

  FDRecoSyst::~FDRecoSyst() 
  {
    //    f->Close();
    //    delete hist;
    //    delete f;
  }

  void FDRecoSyst::Shift(double sigma,
		    Restorer& restore,
		    caf::StandardRecord* sr,
		    double& weight) const
  {
    if (flav == kNumu) {
      if (hc == kFHC) {
	// FD & FHC only and passes cvn numu selection cut
	if (sr->dune.isFD && sr->dune.isFHC && sr->dune.cvnnumu >= 0.5) {
	  
	  int EBin = hist->GetXaxis()->FindBin(sr->dune.Ev);
	  int VarBin = -1;
	  if (var == kX) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.X);
	  }
	  else if (var == kY) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.Y);
	  }
	  else if (var == kW) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.W);
	  }
	  else if (var == kQ2) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.Q2);
	  }
	  double w = hist->GetBinContent(EBin, VarBin);

	  weight *= 1. + sigma * (1. - w) ;
	}
      }
      else if (hc == kRHC) {
	// FD & RHC only and passes cvn numu selection cut
	if (sr->dune.isFD && !sr->dune.isFHC && sr->dune.cvnnumu >= 0.5) {
	
	  int EBin = hist->GetXaxis()->FindBin(sr->dune.Ev);
	  int VarBin = -1;
	  if (var == kX) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.X);
	  }
	  else if (var == kY) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.Y);
	  }
	  else if (var == kW) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.W);
	  }
	  else if (var == kQ2) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.Q2);
	  }
	  double w = hist->GetBinContent(EBin, VarBin);
	  
	  weight *= 1. + sigma * (1. - w) ;
	}
      }
    } // flav == kNumu
    else if (flav == kNue){
      if (hc == kFHC) {
	if (sr->dune.isFD && sr->dune.isFHC && sr->dune.cvnnue >= 0.5) {
	  
	  int EBin = hist->GetXaxis()->FindBin(sr->dune.Ev);
	  int VarBin = -1;
	  if (var == kX) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.X);
	  }
	  else if (var == kY) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.Y);
	  }
	  else if (var == kW) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.W);
	  }
	  else if (var == kQ2) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.Q2);
	  }
	  double w = hist->GetBinContent(EBin, VarBin);

	  weight *= 1. + sigma * (1. - w) ;
	}
      }
      else if (hc == kRHC) {
	if (sr->dune.isFD && !sr->dune.isFHC && sr->dune.cvnnue >= 0.5) {
	
	  int EBin = hist->GetXaxis()->FindBin(sr->dune.Ev);
	  int VarBin = -1;
	  if (var == kX) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.X);
	  }
	  else if (var == kY) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.Y);
	  }
	  else if (var == kW) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.W);
	  }
	  else if (var == kQ2) {
	    VarBin = hist->GetYaxis()->FindBin(sr->dune.Q2);
	  }
	  double w = hist->GetBinContent(EBin, VarBin);
	  
	  weight *= 1. + sigma * (1. - w) ;
	}
      }
    } // flav == Nue
  }

  //  const FDRecoSyst kFDRecoSyst(v);
}
