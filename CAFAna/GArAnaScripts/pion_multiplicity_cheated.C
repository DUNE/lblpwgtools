// Make a simple ND-GAr plot
// cafe pion_multiplicity_cheated.C

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

void pion_multiplicity_cheated(bool muon_skip, bool check_momentum_true, bool keep_unique)
{

  // Environment variables and wildcards work. As do SAM datasets.
  const std::vector<std::string> fname = { "/pnfs/dune/persistent/users/fmlopez/GAr/ndgar_mini_cafs/hadded/ndgar_mini_prod_caf.root" };

  TString out_name = TString::Format("pion_multiplicity_cheated/pion_multiplicity_cheated_%i%i%i.root", muon_skip, check_momentum_true, keep_unique);
  TFile *fout = new TFile(out_name,"RECREATE");

  // I think CAFAna gets the total POT normalization automatically...
  const double pot = 5.0e18;
    
  // Source of events
  SpectrumLoader loader(fname);

  const std::vector<double> kBinEdgesMultiplicity = {-0.5, 0.5, 1.5, 2.5, 3.5, 25.0};
  const Binning binsMultiplicity = Binning::Custom(kBinEdgesMultiplicity);

  const float muon_score_cut = 0.20;
  const float delta_r        = 35.0;

  const float p_thres = 0.08;

  const SRVar kTruePionMultiplicity = kGetTruePionMultiplicity(p_thres);
  const SRVar kPionMultiplicity = kGetPionMultiplicityCheated(muon_score_cut, p_thres, muon_skip, check_momentum_true, keep_unique);

  Spectrum sPionMultiplicity(loader[kIsContainedNuMuCC(muon_score_cut, delta_r)],
                             SRHistAxis("True pion multiplicity", binsMultiplicity, kTruePionMultiplicity,
                                        "Cheated pion multiplicity", binsMultiplicity, kPionMultiplicity));

  loader.Go();

  fout->cd();
  TString dir_name = TString::Format("muon_skip_%i,check_momentum_true_%i,keep_unique_%i", muon_skip, check_momentum_true, keep_unique);
  fout->mkdir(dir_name);
  fout->cd(dir_name);

  TH2* hPionMultiplicity(sPionMultiplicity.ToTH2(pot));
  hPionMultiplicity->SetName("PionMultiplicity");
  hPionMultiplicity->Write("PionMultiplicity", TObject::kWriteDelete);

  fout->Close();

}
