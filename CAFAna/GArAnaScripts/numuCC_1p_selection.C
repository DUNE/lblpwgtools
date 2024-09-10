// Make a simple ND-GAr plot
// cafe numuCC_1p_selection.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"

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

#include <string>
#include <vector>
#include <unordered_map>

using namespace ana;

void numuCC_1p_selection(float proton_dEdx_cut, float proton_tof_cut, float delta_calo, float distance_cut)
{

  // Environment variables and wildcards work. As do SAM datasets.
  const std::vector<std::string> fname = { "/pnfs/dune/persistent/users/fmlopez/GAr/ndgar_mini_cafs/hadded/ndgar_mini_prod_caf.root" };

  TFile *fout = new TFile("numuCC_1p_selection.root","RECREATE");

  // I think CAFAna gets the total POT normalization automatically...
  const double pot = 5.0e18;

  // This vector controls at which cut to stop
  const int flow_vec[4] = { 0, 0, 0, 0 };
    
  // Source of events
  SpectrumLoader loader(fname);

  const TruthVar kTrueE = SIMPLETRUTHVAR(E);
  const Binning binsTrueE = Binning::Simple(40, 0, 15);
  const TruthHistAxis axisTrueE("True neutrino energy [GeV]", binsTrueE, kTrueE);

  const float muon_score_cut = 0.20;
  const float delta_r        = 35.0;

  const float p_thres = 0.08;

  // Define the cuts given the input parameters

  //const SRCut kSelectedCut  = kIsNoCut;
  const SRCut kSelectedCut  = kIsContainedNuMuCC(muon_score_cut, delta_r, true, -1);

  const SRCut kTruthCut = kIsTrueNPi(1, p_thres);
  const SRCut kRecoCut  = kIsNPi(1, muon_score_cut, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut, flow_vec);
  //const SRCut kRecoCut  = kIsNParticles(1);

  Spectrum sTP(loader[kSelectedCut && kRecoCut && kTruthCut].NuTruths(), axisTrueE);
  Spectrum sFP(loader[kSelectedCut && kRecoCut && !kTruthCut].NuTruths(), axisTrueE);
  Spectrum sFN(loader[kSelectedCut && !kRecoCut && kTruthCut].NuTruths(), axisTrueE);

  loader.Go();

  fout->cd();
  TString dir_name = TString::Format("proton_dEdx_%.2f,proton_tof_%.2f,delta_calo_%.2f,distance_cut_%.2f_9999", proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut);
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
