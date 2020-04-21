#include "CAFAna/Analysis/AnalysisVars.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/PRISM/PRISMExtrapolator.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PredictionPRISM.h"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"

using namespace ana;
using namespace PRISM;

std::map<std::string, PRISMStateBlob> States;

void PRISMPrediction(fhicl::ParameterSet const &pred) {

  std::string const &state_file = pred.get<std::string>("state_file");
  std::vector<std::string> const &output_file =
      pred.get<std::vector<std::string>>("output_file");
  std::string const &output_dir = pred.get<std::string>("output_dir");
  std::string const &varname = pred.get<std::string>("projection_name");

  double reg = pred.get<double>("reg_factor");
  std::array<double, 2> fit_range =
      pred.get<std::array<double, 2>>("fit_range");

  std::pair<double, double> gauss_flux =
      pred.get<std::pair<double, double>>("gauss_flux", {0, 0});

  auto NDFlavorSelection = GetBeamChan(
      pred.get<std::string>("flavor_selection.ND", "numu_numode"), true);
  auto FDFlavorSelection = GetBeamChan(
      pred.get<std::string>("flavor_selection.FD", "numu_numode"), false);

  (void)GetListOfSysts();

  osc::IOscCalculatorAdjustable *calc =
      ConfigureCalc(pred.get<fhicl::ParameterSet>("Osc", {}));

  if (!States.count(state_file)) {
    TFile fs(state_file.c_str());
    std::cout << "Loading " << varname << " state from " << state_file
              << std::endl;
    if (!fs.IsOpen()) {
      std::cout << "[ERROR]: Cannot open " << state_file << " for reading. "
                << std::endl;
      abort();
    }
    States[state_file] = LoadPRISMState(fs, varname);
    std::cout << "Done!" << std::endl;
    fs.Close();
  }

  SystShifts shift = GetSystShifts(pred.get<fhicl::ParameterSet>("syst", {}));

  SystShifts fluxshift = GetFluxSystShifts(shift);

  PRISMStateBlob &state = States[state_file];

  TFile f(output_file[0].c_str(),
          output_file.size() > 1 ? output_file[1].c_str() : "RECREATE");

  TDirectory *dir = &f;
  if (output_dir.size()) {
    dir = f.mkdir(output_dir.c_str());
  }
  dir->cd();

  int id = 0;
  PRISMExtrapolator fluxmatcher;

  // PRISM
  // MatchPredInterps
  // SelPredInterps
  // FarDetPredInterps
  // FarDetData_nonswap
  // FarDetData_nueswap

  fluxmatcher.Initialize({
      {"ND_nu", state.MatchPredInterps[kND_nu].get()},
      {"FD_nu", state.MatchPredInterps[kFD_nu_nonswap].get()},
      {"ND_nub", state.MatchPredInterps[kND_nub].get()},
      {"FD_nub", state.MatchPredInterps[kFD_nub_nonswap].get()},
  });

  fluxmatcher.SetStoreDebugMatches();
  if (pred.get<bool>("is_fake_spec_run", false)) {
    fluxmatcher.SetTargetConditioning(reg,
                                      {
                                          {0},
                                      },
                                      fit_range[0], fit_range[1]);
  } else {
    fluxmatcher.SetTargetConditioning(reg, {}, fit_range[0], fit_range[1]);
  }

  state.PRISM->SetFluxMatcher(&fluxmatcher);

  bool do_gauss = gauss_flux.first != 0;

  double pot = pot_fd * (1.0 / 3.5);

  auto const &comps =
      do_gauss
          ? state.PRISM->PredictGaussianFlux(
                gauss_flux.first, gauss_flux.second, shift, NDFlavorSelection)

          : state.PRISM->PredictPRISMComponents(calc, shift, NDFlavorSelection,
                                                FDFlavorSelection);

  TH1 *PRISMPredEvRateMatch_h =
      comps.at(PredictionPRISM::kPRISMPred).ToTHX(pot);

  PRISMPredEvRateMatch_h->Scale(1, "width");
  double gauss_sf = 1;
  if (do_gauss) {
    gauss_sf = (1.0 / (gauss_flux.second * sqrt(2 * M_PI) *
                       PRISMPredEvRateMatch_h->GetMaximum()));
    PRISMPredEvRateMatch_h->Scale(gauss_sf);
  }
  PRISMPredEvRateMatch_h->SetTitle(";E_{#nu} (GeV);Pred. FD EvRate per 1 GeV");
  PRISMPredEvRateMatch_h->Write("PRISMPredEvRateMatch");

  for (auto const &compspec : comps) {
    TH1 *comp = compspec.second.ToTHX(pot);
    comp->Scale(1, "width");
    if (do_gauss) {
      comp->Scale(gauss_sf);
    }
    comp->SetTitle(";E_{#nu} (GeV);Pred. FD Contribution per 1 GeV");
    comp->Write((std::string("PRISMPredEvRateMatch_") +
                 PredictionPRISM::GetComponentString(compspec.first))
                    .c_str());
  }

  fluxmatcher.Write(dir->mkdir("PRISMEventRateMatches"));

  size_t NDConfig_enum = GetConfigFromNuChan(NDFlavorSelection, true);
  size_t FDConfig_enum = GetConfigFromNuChan(FDFlavorSelection, false);
  size_t FDfdConfig_enum = GetFDConfigFromNuChan(FDFlavorSelection);

  // Sort out the flavors and signs
  auto NDSigFlavor = (NDFlavorSelection.chan & NuChan::kNumuNumuBar)
                         ? Flavors::kAllNuMu
                         : Flavors::kAllNuE;
  auto NDSigSign = ((NDFlavorSelection.chan & NuChan::kNumu) ||
                    (NDFlavorSelection.chan & NuChan::kNue))
                       ? Sign::kNu
                       : Sign::kAntiNu;
  auto NDWrongSign = (NDSigSign == Sign::kNu) ? Sign::kAntiNu : Sign::kNu;
  auto NDWrongFlavor =
      (NDSigFlavor == Flavors::kAllNuMu) ? Flavors::kAllNuE : Flavors::kAllNuMu;

  auto FDSigFlavor = (FDFlavorSelection.chan & NuChan::kNumuNumuBar)
                         ? Flavors::kNuMuToNuMu
                         : Flavors::kNuMuToNuE;
  auto FDSigSign = ((FDFlavorSelection.chan & NuChan::kNumu) ||
                    (FDFlavorSelection.chan & NuChan::kNue))
                       ? Sign::kNu
                       : Sign::kAntiNu;

  auto FDWrongSign = (FDSigSign == Sign::kNu) ? Sign::kAntiNu : Sign::kNu;
  auto FDWrongFlavor = (FDSigFlavor == Flavors::kNuMuToNuMu)
                           ? Flavors::kAllNuE
                           : Flavors::kAllNuMu;
  auto FDIntrinsicFlavor = (FDSigFlavor == Flavors::kNuMuToNuMu)
                               ? Flavors::kNuEToNuMu
                               : Flavors::kNuEToNuE;
  TH1 *NearDet_predict_h =
      state.SelPredInterps[NDConfig_enum]->PredictSyst(calc, shift).ToTHX(pot);

  NearDet_predict_h->SetTitle(";E_{#nu} (GeV);FD EvRate");
  NearDet_predict_h->Write("NearDet_Sel_predict");

  std::unique_ptr<TH2> ND_NC_h(
      state.SelPredInterps[NDConfig_enum]
          ->PredictComponentSyst(calc, shift, Flavors::kAll, Current::kNC,
                                 Sign::kBoth)
          .ToTH2(pot));
  ND_NC_h->Scale(1, "Width");
  ND_NC_h->Write("NearDet_Sel_NCBkg");
  ND_NC_h->SetDirectory(nullptr);

  std::unique_ptr<TH2> ND_WLB_h(
      state.SelPredInterps[NDConfig_enum]
          ->PredictComponentSyst(calc, shift, NDWrongFlavor, Current::kCC,
                                 Sign::kBoth)
          .ToTH2(pot));
  ND_WLB_h->Scale(1, "Width");
  ND_WLB_h->Write("NearDet_Sel_WLBkg");
  ND_WLB_h->SetDirectory(nullptr);

  std::unique_ptr<TH2> ND_WSB_h(
      state.SelPredInterps[NDConfig_enum]
          ->PredictComponentSyst(calc, shift, NDSigFlavor, Current::kCC,
                                 NDWrongSign)
          .ToTH2(pot));
  ND_WSB_h->Scale(1, "Width");
  ND_WSB_h->Write("NearDet_Sel_WSBkg");
  ND_WSB_h->SetDirectory(nullptr);

  if (do_gauss) {
    return;
  }

  std::unique_ptr<TH1> FD_NC_h(
      state.SelPredInterps[FDConfig_enum]
          ->PredictComponentSyst(calc, shift, Flavors::kAll, Current::kNC,
                                 Sign::kBoth)
          .ToTH1(pot));
  FD_NC_h->Scale(1, "Width");
  FD_NC_h->Write("FarDet_Sel_NCBkg");
  FD_NC_h->SetDirectory(nullptr);

  std::unique_ptr<TH1> FD_WLB_h(
      state.SelPredInterps[FDConfig_enum]
          ->PredictComponentSyst(calc, shift, FDWrongFlavor, Current::kCC,
                                 Sign::kBoth)
          .ToTH1(pot));
  FD_WLB_h->Scale(1, "Width");
  FD_WLB_h->Write("FarDet_Sel_WLBkg");
  FD_WLB_h->SetDirectory(nullptr);

  std::unique_ptr<TH1> FD_WSB_h(
      state.SelPredInterps[FDConfig_enum]
          ->PredictComponentSyst(calc, shift, FDSigFlavor, Current::kCC,
                                 FDWrongSign)
          .ToTH1(pot));
  FD_WSB_h->Scale(1, "Width");
  FD_WSB_h->Write("FarDet_Sel_WSBkg");
  FD_WSB_h->SetDirectory(nullptr);

  std::unique_ptr<TH1> FD_IntrinsicB_h(
      state.SelPredInterps[FDConfig_enum]
          ->PredictComponentSyst(calc, shift, FDIntrinsicFlavor, Current::kCC,
                                 FDSigSign)
          .ToTH1(pot));
  FD_IntrinsicB_h->Scale(1, "Width");
  FD_IntrinsicB_h->Write("FarDet_Sel_IntrinsicBkg");
  FD_IntrinsicB_h->SetDirectory(nullptr);

  std::cout << "Config: " << FDConfig_enum
            << " FDfdConfig_enum: " << FDfdConfig_enum << std::endl;

  TH1 *FarDet_predict_h = state.FarDetPredInterps[FDfdConfig_enum]
                              ->PredictSyst(calc, shift)
                              .ToTHX(pot);

  for (int bin_it = 0; bin_it < FarDet_predict_h->GetXaxis()->GetNbins();
       ++bin_it) {
    FarDet_predict_h->SetBinError(
        bin_it + 1, sqrt(FarDet_predict_h->GetBinContent(bin_it + 1)));
  }
  FarDet_predict_h->Scale(1, "width");

  FarDet_predict_h->SetTitle(";E_{#nu} (GeV);FD EvRate");
  FarDet_predict_h->Write("FarDet_predict");

  int osc_from = FluxSpeciesPDG(NDFlavorSelection.chan);
  int osc_to = FluxSpeciesPDG(FDFlavorSelection.chan);

  TH1 *FarDetData_nonswap_h = state.FarDetData_nonswap[FDfdConfig_enum]
                                  ->Oscillated(calc, osc_from, osc_to)
                                  .ToTHX(pot);

  for (int bin_it = 0; bin_it < FarDetData_nonswap_h->GetXaxis()->GetNbins();
       ++bin_it) {
    FarDetData_nonswap_h->SetBinError(
        bin_it + 1, sqrt(FarDetData_nonswap_h->GetBinContent(bin_it + 1)));
  }
  FarDetData_nonswap_h->Scale(1, "width");

  FarDetData_nonswap_h->SetTitle(";E_{#nu} (GeV);FD EvRate");
  FarDetData_nonswap_h->Write("FarDetData_nonswap");

  std::cout << "FDConfig_enum: " << FDConfig_enum
            << ", nueswap: " << GetConfigNueSwap(FDConfig_enum) << ", Have? "
            << state.Have(GetConfigNueSwap(FDConfig_enum)) << std::endl;

  if (state.Have(GetConfigNueSwap(FDConfig_enum))) {
    TH1 *FarDetData_nueswap_h = state.FarDetData_nueswap[FDfdConfig_enum]
                                    ->Oscillated(calc, osc_from, osc_to)
                                    .ToTHX(pot);

    for (int bin_it = 0; bin_it < FarDetData_nueswap_h->GetXaxis()->GetNbins();
         ++bin_it) {
      FarDetData_nueswap_h->SetBinError(
          bin_it + 1, sqrt(FarDetData_nueswap_h->GetBinContent(bin_it + 1)));
    }
    FarDetData_nueswap_h->Scale(1, "width");

    FarDetData_nueswap_h->SetTitle(";E_{#nu} (GeV);FD EvRate");
    FarDetData_nueswap_h->Write("FarDetData_nueswap");
    FarDetData_nueswap_h->Add(FarDetData_nonswap_h);
    FarDetData_nueswap_h->Write("FarDetData_total");
  }

  f.Write();
}

int main(int argc, char const *argv[]) {
  // Make sure systs are applied to ND distributions which are per 1 POT.
  setenv("CAFANA_PRED_MINMCSTATS", "0", 1);
  gROOT->SetMustClean(false);

  if (argc != 2) {
    std::cout << "[ERROR]: Expected to be passed the location of a single "
                 "configuration FHiCL file."
              << std::endl;
    return 1;
  }

  fhicl::ParameterSet const &ps = fhicl::make_ParameterSet(argv[1]);

  for (fhicl::ParameterSet const &pred :
       ps.get<std::vector<fhicl::ParameterSet>>("predictions")) {
    PRISMPrediction(pred);
  }
}
