#include "CAFAna/Analysis/AnalysisVars.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/PRISM/PRISMExtrapolator.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PredictionPRISM.h"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"

using namespace ana;

std::map<std::string, PRISMStateBlob> States;

void PRISMPrediction(fhicl::ParameterSet const &pred) {

  std::string const &state_file = pred.get<std::string>("state_file");
  std::vector<std::string> const &output_file =
      pred.get<std::vector<std::string>>("output_file");
  std::string const &output_dir = pred.get<std::string>("output_dir");
  std::string const &varname =
      pred.get<std::string>("projection_name");
  bool isfhc = pred.get<bool>("isFHC", true);

  double reg = pred.get<double>("reg_factor");
  std::array<double, 2> fit_range =
      pred.get<std::array<double, 2>>("fit_range");

  (void)GetListOfSysts();

  osc::IOscCalculatorAdjustable *calc =
      ConfigureCalc(pred.get<fhicl::ParameterSet>("Osc", {}));

  if (!States.count(state_file)) {
    TFile fs(state_file.c_str());
    std::cout << "Loading " << varname << " state from " << state_file
              << std::endl;
    States[state_file] = LoadPRISMState(fs, varname, !isfhc);
    std::cout << "Done!" << std::endl;
    fs.Close();
  }

  SystShifts shift = GetSystShifts(pred.get<fhicl::ParameterSet>("syst", {}));

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

  fluxmatcher.InitializeEventRateMatcher(state.NDMatchInterp.get(),
                                         state.FDMatchInterp.get());
  fluxmatcher.SetStoreDebugMatches();
  if (pred.get<bool>("is_fake_spec_run", false)) {
    fluxmatcher.SetTargetConditioning(reg, {{0},}, fit_range[0], fit_range[1]);
  } else {
    fluxmatcher.SetTargetConditioning(reg, {}, fit_range[0], fit_range[1]);
  }

  state.PRISM->SetFluxMatcher(&fluxmatcher);

  Spectrum PRISMPredEvRateMatchSpec = state.PRISM->PredictSyst(calc, shift);

  double pot = pot_fd*(1.0/3.5);

  TH1 *PRISMPredEvRateMatch_h = PRISMPredEvRateMatchSpec.ToTHX(pot);

  PRISMPredEvRateMatch_h->Scale(1, "width");
  PRISMPredEvRateMatch_h->SetTitle(";E_{#nu} (GeV);Pred. FD EvRate per 1 GeV");
  PRISMPredEvRateMatch_h->Write("PRISMPredEvRateMatch");

  for (auto &compspec : state.PRISM->PredictPRISMComponents(calc, shift)) {
    TH1 *comp = compspec.second.ToTHX(pot);
    comp->Scale(1, "width");
    comp->SetTitle(";E_{#nu} (GeV);Pred. FD Contribution per 1 GeV");
    comp->Write((std::string("PRISMPredEvRateMatch_") +
                 PredictionPRISM::GetComponentString(compspec.first))
                    .c_str());
  }

  fluxmatcher.Write(dir->mkdir("PRISMEventRateMatches"));

  TH1 *FarDet_h = state.FarDet->Predict(calc).ToTHX(pot);

  for (int bin_it = 0; bin_it < FarDet_h->GetXaxis()->GetNbins(); ++bin_it) {
    FarDet_h->SetBinError(bin_it + 1,
                          sqrt(FarDet_h->GetBinContent(bin_it + 1)));
  }
  FarDet_h->Scale(1, "width");

  FarDet_h->SetTitle(";E_{#nu} (GeV);FD EvRate");
  FarDet_h->Write("FarDet");

  TH1 *FarDet_unosc_h = state.FarDet->PredictUnoscillated().ToTHX(pot);
  FarDet_unosc_h->Scale(1, "width");

  FarDet_unosc_h->SetTitle(";E_{#nu} (GeV);FD EvRate");
  FarDet_unosc_h->Write("FarDet_unosc");

  TH1 *NearDet_h = state.NDMatchInterp->Predict(calc).ToTHX(pot);
  NearDet_h->SetTitle(";E_{#nu} (GeV);OffAxis;FD EvRate");
  NearDet_h->Write("NearDet");

  f.Write();
}

int main(int argc, char const *argv[]) {
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
