#include "CAFAna/Analysis/AnalysisVars.h"
#include "CAFAna/Analysis/Exposures.h"
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
  std::string const &output_dir = pred.get<std::string>("output_dir", "");

  std::string const &varname =
      pred.get<std::string>("projection_name", "EProxy");

  // default to 1 year
  double POT = pred.get<double>("POT_years", 1) * POT120;
  double POT_FD = POT * pot_fd_FVMassFactor;
  bool use_PRISM = pred.get<bool>("use_PRISM", true);

  std::pair<double, double> gauss_flux =
      pred.get<std::pair<double, double>>("gauss_flux", {0, 0});

  (void)GetListOfSysts();

  SystShifts shift = GetSystShifts(pred.get<fhicl::ParameterSet>("syst", {}));

  SystShifts fluxshift = GetFluxSystShifts(shift);

  bool do_gauss = gauss_flux.first != 0;

  auto PRISMps = pred.get<fhicl::ParameterSet>("PRISM", {});

  bool PRISM_SetNDDataErrs =
      PRISMps.get<bool>("set_ND_errors_from_rate", false);

  auto RunPlans = PRISMps.get<fhicl::ParameterSet>("RunPlans", {});

  RunPlan run_plan_nu, run_plan_nub;

  if (RunPlans.has_key("numode")) {
    run_plan_nu =
        make_RunPlan(RunPlans.get<fhicl::ParameterSet>("numode"), POT);
  }
  if (RunPlans.has_key("nubmode")) {
    run_plan_nub =
        make_RunPlan(RunPlans.get<fhicl::ParameterSet>("nubmode"), POT);
  }

  bool PRISM_write_debug = PRISMps.get<bool>("write_debug");

  osc::IOscCalculatorAdjustable *calc =
      ConfigureCalc(pred.get<fhicl::ParameterSet>("true_osc", {}));

  // Lazy load the state file
  if (!States.count(state_file)) {
    TFile fs(state_file.c_str());
    std::cout << "Loading " << varname << " state from " << state_file
              << std::endl;
    States[state_file] = LoadPRISMState(fs, varname);
    std::cout << "Done!" << std::endl;
    fs.Close();
  }

  PRISMStateBlob &state = States[state_file];

  TFile f(output_file[0].c_str(),
          output_file.size() > 1 ? output_file[1].c_str() : "RECREATE");

  TDirectory *dir = &f;

  if (output_dir.size()) {
    dir = dir->mkdir(output_dir.c_str());
  }

  dir->cd();

  PRISMExtrapolator fluxmatcher;
  if (use_PRISM) {
    fluxmatcher.Initialize({
        {"ND_nu", state.MatchPredInterps[kND_nu].get()},
        {"FD_nu", state.MatchPredInterps[kFD_nu_nonswap].get()},
        {"ND_nub", state.MatchPredInterps[kND_nub].get()},
        {"FD_nub", state.MatchPredInterps[kFD_nub_nonswap].get()},
    });

    for (auto const &channel_conditioning :
         PRISMps.get<std::vector<fhicl::ParameterSet>>("match_conditioning")) {

      auto ch =
          GetMatchChan(channel_conditioning.get<fhicl::ParameterSet>("chan"));

      double chan_reg = channel_conditioning.get<double>("reg_factor", 1E-16);
      std::array<double, 2> chan_energy_range =
          channel_conditioning.get<std::array<double, 2>>("energy_range",
                                                          {0, 4});
      bool chan_is_fake_spec_run =
          channel_conditioning.get<bool>("is_fake_spec_run", false);

      fluxmatcher.SetTargetConditioning(ch, chan_reg, (chan_is_fake_spec_run ? 
            std::vector<double>{{0},} 
          : std::vector<double>{}),
          chan_energy_range);
    }

    if (PRISM_write_debug) {
      fluxmatcher.SetStoreDebugMatches();
    }
    state.PRISM->SetFluxMatcher(&fluxmatcher);
  }

  if (run_plan_nu.GetPlanPOT() > 0) {
    state.PRISM->SetNDRunPlan(run_plan_nu, BeamMode::kNuMode);
  }

  if (run_plan_nub.GetPlanPOT() > 0) {
    state.PRISM->SetNDRunPlan(run_plan_nub, BeamMode::kNuBarMode);
  }

  state.PRISM->SetNDDataErrorsFromRate(PRISM_SetNDDataErrs);

  std::map<std::string, MatchChan> Channels;
  if (pred.is_key_to_sequence("samples")) {
    for (auto const &fs :
         pred.get<std::vector<fhicl::ParameterSet>>("samples")) {
      auto ch = GetMatchChan(fs);
      Channels[GetMatchChanShortName(ch)] = ch;
    }
  } else {
    auto ch = GetMatchChan(pred.get<fhicl::ParameterSet>("samples"));
    Channels[GetMatchChanShortName(ch)] = ch;
  }

  std::vector<Spectrum> DataSpectra;
  DataSpectra.reserve(Channels.size());

  for (auto const ch : Channels) {
    int osc_from = FluxSpeciesPDG(ch.second.from.chan);
    int osc_to = FluxSpeciesPDG(ch.second.to.chan);
    size_t NDConfig_enum = GetConfigFromNuChan(ch.second.from, true);
    size_t FDConfig_enum = GetConfigFromNuChan(ch.second.to, false);
    size_t FDfdConfig_enum = GetFDConfigFromNuChan(ch.second.to);

    if ((NDConfig_enum == kND_nu) && !run_plan_nu.GetPlanPOT()) {
      std::cout << "[ERROR]: Have ND nu channel, but no numode run plan."
                << std::endl;
      abort();
    }
    if ((NDConfig_enum == kND_nub) && !run_plan_nub.GetPlanPOT()) {
      std::cout << "[ERROR]: Have ND nubar channel, but no numode run plan."
                << std::endl;
      abort();
    }

    DataSpectra.push_back(state.FarDetData_nonswap[FDfdConfig_enum]->Oscillated(
        calc, osc_from, osc_to));

    TDirectory *chan_dir =
        dir->mkdir(DescribeFDConfig(FDfdConfig_enum).c_str());
    chan_dir->cd();

    if (state.Have(GetConfigNueSwap(FDConfig_enum))) {
      DataSpectra.back() +=
          state.FarDetData_nueswap[FDfdConfig_enum]->Oscillated(calc, osc_from,
                                                                osc_to);

      TH1 *Data_nueswap = state.FarDetData_nueswap[FDfdConfig_enum]
                              ->Oscillated(calc, osc_from, osc_to)
                              .ToTH1(POT_FD);
      if (Data_nueswap->Integral() > 0) {
        chan_dir->WriteTObject(Data_nueswap, "Data_nueswap_component");
      }
      Data_nueswap->SetDirectory(nullptr);
    }

    TH1 *Data = DataSpectra.back().ToTH1(POT_FD);
    chan_dir->WriteTObject(Data, "Data_Total");
    Data->SetDirectory(nullptr);

    if (use_PRISM) {
      auto PRISMComponents =
          state.PRISM->PredictPRISMComponents(calc, shift, ch.second);
      TH1 *PRISMPred =
          PRISMComponents.at(PredictionPRISM::kPRISMPred).ToTH1(POT_FD);
      chan_dir->WriteTObject(PRISMPred, "PRISMPred");
      PRISMPred->SetDirectory(nullptr);

      if (PRISM_write_debug) {

        for (auto const &comp : PRISMComponents) {
          // we always write this
          if (comp.first == PredictionPRISM::kPRISMPred) {
            continue;
          }

          TH1 *PRISMComp_h = comp.second.ToTHX(POT);

          if (PRISMComp_h->Integral() > 0) {
            chan_dir->WriteTObject(
                PRISMComp_h,
                PredictionPRISM::GetComponentString(comp.first).c_str());
          }
        }

        fluxmatcher.Write(chan_dir->mkdir("NDFD_matcher"));

        dir->cd();
      }

    } else {
      TH1 *FarDetPred = state.FarDetPredInterps[FDfdConfig_enum]
                            ->PredictSyst(calc, shift)
                            .ToTH1(POT_FD);
      chan_dir->WriteTObject(FarDetPred, "FarDetPred");
      FarDetPred->SetDirectory(nullptr);
    }

    if (NDConfig_enum == kND_nu) {
      TH1D *run_plan_nu_h = run_plan_nu.AsTH1();
      chan_dir->WriteTObject(run_plan_nu_h, "run_plan_nu");
      run_plan_nu_h->SetDirectory(nullptr);
    } else {
      {
        TH1D *run_plan_nub_h = run_plan_nub.AsTH1();
        chan_dir->WriteTObject(run_plan_nub_h, "run_plan_nub");
        run_plan_nub_h->SetDirectory(nullptr);
      }
    }
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
