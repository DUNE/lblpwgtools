// Make a simple ND-GAr plot
// cafe pion_multiplicity_optimization.C

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

void pion_multiplicity_optimization(float proton_dEdx_cut, float proton_tof_cut, float delta_calo, float distance_cut)
{

  // Environment variables and wildcards work. As do SAM datasets.
  const std::vector<std::string> fname = { "/pnfs/dune/persistent/users/fmlopez/GAr/ndgar_mini_cafs/hadded/ndgar_mini_prod_caf.root" };

  TFile *fout = new TFile("pion_multiplicity_optimization.root","RECREATE");

  // I think CAFAna gets the total POT normalization automatically...
  const double pot = 5.0e18;

  // This vector controls at which cut to stop
  const int flow_vec[4] = { 0, 0, 0, 0 };
    
  // Source of events
  SpectrumLoader loader(fname);

  const std::vector<double> kBinEdgesMultiplicity = {-0.5, 0.5, 1.5, 2.5, 3.5, 25.0};
  const Binning binsMultiplicity = Binning::Custom(kBinEdgesMultiplicity);

  const float muon_score_cut = 0.20;
  const float delta_r        = 35.0;

  const SRCut kRecoCut  = kIsContainedNuMuCC(muon_score_cut, delta_r, momentum_method, sign_selection, flow_vec);
  //const SRCut kRecoCut  = kIsNoCut;

  const float p_thres = 0.08;

  //const float proton_dEdx_cut = 0.8;
  //const float proton_tof_cut = 0.8;
  //const float delta_calo = 0.1;
  //const float distance_cut = 100.0;

  const SRVar kTruePionMultiplicity = kGetTruePionMultiplicity(p_thres);
  const SRVar kPionMultiplicity = kGetPionMultiplicity(muon_score_cut, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut);

  Spectrum sPionMultiplicity(loader[kRecoCut],
                             SRHistAxis("True pion multiplicity", binsMultiplicity, kTruePionMultiplicity,
                                        "Reco pion multiplicity", binsMultiplicity, kPionMultiplicity));

  loader.Go();

  fout->cd();
  TString dir_name = TString::Format("proton_dEdx_%.2f,proton_tof_%.2f,delta_calo_%.2f,distance_cut_%.2f", proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut);
  fout->mkdir(dir_name);
  fout->cd(dir_name);

  TH2* hPionMultiplicity(sPionMultiplicity.ToTH2(pot));
  hPionMultiplicity->SetName("PionMultiplicity");
  hPionMultiplicity->Write("PionMultiplicity", TObject::kWriteDelete);

  fout->Close();

}
