// Make a simple ND-GAr plot
// cafe numuCC_selection_wrongsign.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"

#include "CAFAna/Core/UtilsExt.h"

#include "GArUtils.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "duneanaobj/StandardRecord/SRGAr.h"

#include <TFile.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TRatioPlot.h"
#include "TLegend.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TColor.h"
#include "TString.h"

#include <string>
#include <vector>
#include <unordered_map>

using namespace ana;

void numuCC_selection_wrongsign(float muon_score_cut, float delta_r, bool momentum_method, int sign_selection)
{

  // Environment variables and wildcards work. As do SAM datasets.
  const std::vector<std::string> fname = { "/pnfs/dune/persistent/users/fmlopez/GAr/ndgar_mini_cafs/hadded/ndgar_mini_prod_caf.root" };

  TFile *fout = new TFile("numuCC_selection_wrongsign.root","RECREATE");

  // I think CAFAna gets the total POT normalization automatically...
  const double pot = 5.0e18;
    
  // Source of events
  SpectrumLoader loader(fname);

  const TruthVar kTrueE = SIMPLETRUTHVAR(E);
  const Binning binsTrueE = Binning::Simple(40, 0, 8);
  const TruthHistAxis axisTrueE("True neutrino energy [GeV]", binsTrueE, kTrueE);

  // Define the cuts given the input parameters
  const SRCut kTruthCut = kIsTrueNuMuCC && kIsTrueContained(delta_r);
  const SRCut kRecoCut  = kIsContainedNuMuCC(muon_score_cut, delta_r, momentum_method, sign_selection);

  // Fill spectra with True Positive, False Positive and False Negative
  // The true spectrum will be given by P = TP+FN, while the reco one is PP = TP+FP
  // We compute the efficiency as TP/(TP+FN)=TP/P, and purity as TP/(TP+FP)=TP/PP
  Spectrum sTP(loader[kRecoCut && kTruthCut].NuTruths(), axisTrueE);
  Spectrum sFP(loader[kRecoCut && !kTruthCut].NuTruths(), axisTrueE);
  Spectrum sFN(loader[!kRecoCut && kTruthCut].NuTruths(), axisTrueE);

  loader.Go();

  fout->cd();
  TString dir_name = TString::Format("muon_score_%.2f,delta_r_%.2f,momentum_method_%i,sign_selection_%i", muon_score_cut, delta_r, momentum_method, sign_selection);
  fout->mkdir(dir_name);
  fout->cd(dir_name);

  // Get the hostograms for the filled spectra
  TH1D* hTP(sTP.ToTH1(pot));
  hTP->SetName("TP");
  hTP->Write("TP", TObject::kWriteDelete);

  TH1D* hFP(sFP.ToTH1(pot));
  hFP->SetName("FP");
  hFP->Write("FP", TObject::kWriteDelete);

  TH1D* hFN(sFN.ToTH1(pot));
  hFN->SetName("FN");
  hFN->Write("FN", TObject::kWriteDelete);  

  fout->Close();

}
