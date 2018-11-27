#include "CAFAna/Systs/FDRecoSysts.h"
#include "TH2.h"
#include "TFile.h"

namespace ana {

  FDRecoSyst::~FDRecoSyst() 
  {
    f->Close();
    delete hist;
    delete f;
  }

  void FDRecoSyst::Shift(double sigma,
		    Restorer& restore,
		    caf::StandardRecord* sr,
		    double& weight) const
  {

    // FD & FHC only and passes cvn numu selection cut
    if (sr->dune.isFD && sr->dune.isFHC && sr->dune.cvnnumu >= 0.5) {
      //      TFile f((FindCAFAnaDir()+"/Systs/modelComp.root").c_str());
      //      assert(!f.IsZombie());
      // For each event find the correct bin in Ev & X
      /*TH2*&h = hist;
      h = (TH2*)f.Get("hx_ratiofhc");
      assert(h);
      h = (TH2*)h->Clone(UniqueName().c_str());
      h->SetDirectory(0);

      TH2* h = hist;*/
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

  //  const FDRecoSyst kFDRecoSyst(v);
}
