#include "CAFAna/Analysis/AnalysisVars.h"
#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/PRISM/PRISMExtrapolator.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PRISMDetectorExtrapolation.h"
#include "CAFAna/PRISM/PRISMMCEffCorrection.h"
#include "CAFAna/PRISM/PredictionPRISM.h"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"

using namespace ana;
using namespace PRISM;

std::map<std::string, PRISMStateBlob> States;

double deriv(double *evals, double step) {
  return (-evals[2] + 8 * evals[1] - 8 * evals[-1] + evals[-2]) / (12 * step);
}

double second_deriv(double *evals, double step) {
  double first_deriv_evals[5];

  first_deriv_evals[0] = deriv(&evals[-2], step);
  first_deriv_evals[1] = deriv(&evals[-1], step);
  first_deriv_evals[3] = deriv(&evals[1], step);
  first_deriv_evals[4] = deriv(&evals[2], step);

  return deriv(&first_deriv_evals[2], step);
}

void PRISMRegOptimizer(fhicl::ParameterSet const &reg_scan) {

  std::string const &state_file = reg_scan.get<std::string>("state_file");

  std::vector<std::string> const &output_file =
      reg_scan.get<std::vector<std::string>>("output_file");
  std::string const &output_dir = reg_scan.get<std::string>("output_dir", "");
  std::string const &varname =
      reg_scan.get<std::string>("projection_name", "EProxy");

  double POT = reg_scan.get<double>("POT_years", 1) * POT120;

  auto PRISMps = reg_scan.get<fhicl::ParameterSet>("PRISM", {});
  bool Use_EventRateMatching =
      PRISMps.get<bool>("Use_EventRateMatching", true);
  double RegFactorExtrap = PRISMps.get<double>("reg_factor_extrap");

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

  osc::IOscCalculatorAdjustable *calc =
      ConfigureCalc(reg_scan.get<fhicl::ParameterSet>("true_osc", {}));

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

  if (Use_EventRateMatching) {
    fluxmatcher.Initialize({
        {"ND_293kA_nu", state.MatchPredInterps[kND_293kA_nu].get()},
        {"ND_280kA_nu", state.MatchPredInterps[kND_280kA_nu].get()},
        {"FD_nu", state.MatchPredInterps[kFD_nu_nonswap].get()},
        {"ND_293kA_nub", state.MatchPredInterps[kND_293kA_nub].get()},
        {"ND_280kA_nub", state.MatchPredInterps[kND_280kA_nub].get()},
        {"FD_nub", state.MatchPredInterps[kFD_nub_nonswap].get()},
    });
  } else {
    fluxmatcher.Initialize({
        {"ND_293kA_nu", state.NDFluxPred_293kA_nu.get()},
        {"ND_280kA_nu", state.NDFluxPred_280kA_nu.get()},
        {"FD_nu", state.FDFluxPred_293kA_nu.get()},
        {"ND_293kA_nub", state.NDFluxPred_293kA_nub.get()},
        {"ND_280kA_nub", state.NDFluxPred_280kA_nub.get()},
        {"FD_nub", state.FDFluxPred_293kA_nub.get()},
    });
  }

  std::map<MatchChan, fhicl::ParameterSet> channel_conditioning_configs;
  std::map<std::string, MatchChan> Channels;

  for (auto const &channel_conditioning :
       PRISMps.get<std::vector<fhicl::ParameterSet>>("match_conditioning")) {

    auto ch =
        GetMatchChan(channel_conditioning.get<fhicl::ParameterSet>("chan"));

    Channels[GetMatchChanShortName(ch)] = ch;

    channel_conditioning_configs[ch] = channel_conditioning;
    double chan_reg_293kA =
        channel_conditioning.get<double>("reg_factor_293kA", 1E-16);
    double chan_reg_280kA =
        channel_conditioning.get<double>("reg_factor_280kA", 1E-16);
    std::array<double, 2> chan_energy_range =
        channel_conditioning.get<std::array<double, 2>>("energy_range", {0, 4});

    fluxmatcher.SetTargetConditioning(ch, chan_reg_293kA, chan_reg_280kA,
                                      chan_energy_range);
  }

  state.PRISM->SetFluxMatcher(&fluxmatcher);

  if (run_plan_nu.GetPlanPOT() > 0) {
    state.PRISM->SetNDRunPlan(run_plan_nu, BeamMode::kNuMode);
  }
  if (run_plan_nub.GetPlanPOT() > 0) {
    state.PRISM->SetNDRunPlan(run_plan_nub, BeamMode::kNuBarMode);
  }

  std::vector<TGraph> LCurves;
  std::vector<TGraph> Resid;
  std::vector<TGraph> Soln;
  std::vector<TGraph> DiffLCurves;

  for (auto const &ch : Channels) {
    std::cout << "Channel start" << std::endl;
    auto const &chan_scan_config = channel_conditioning_configs[ch.second];

    std::vector<double> steps;
    std::array<double, 3> scan_descriptor =
        chan_scan_config.get<std::array<double, 3>>("scan");
    bool use_log_steps = chan_scan_config.get<bool>("use_log_steps");
    size_t nsteps = size_t(scan_descriptor[0]);

    if (scan_descriptor[2] < scan_descriptor[1]) {
      std::swap(scan_descriptor[1], scan_descriptor[2]);
    }

    double min = use_log_steps ? log10(scan_descriptor[1]) : scan_descriptor[1];
    double max = use_log_steps ? log10(scan_descriptor[2]) : scan_descriptor[2];
    double step = (max - min) / double(nsteps);

    steps.push_back(use_log_steps ? std::pow(10.0, min) : min);
    for (size_t sit = 0; sit < size_t(scan_descriptor[0]); ++sit) {
      double v = (min + double(sit + 1) * step);
      steps.push_back(use_log_steps ? std::pow(10.0, v) : v);
    }

    int osc_from = FluxSpeciesPDG(ch.second.from.chan);
    int osc_to = FluxSpeciesPDG(ch.second.to.chan);
    size_t NDConfig_enum = GetConfigFromNuChan(ch.second.from, true);
    size_t FDConfig_enum = GetConfigFromNuChan(ch.second.to, false);
    size_t FDfdConfig_enum = GetFDConfigFromNuChan(ch.second.to);

    LCurves.emplace_back(steps.size());
    LCurves.back().SetTitle(
        ";|P(#Phi_{ND} #times c - #Phi_{FD})|;|A #times c|");

    Resid.emplace_back(steps.size());
    Resid.back().SetTitle(";p_{reg};|P(M_{ND} #times E_{true} - E_{reco})|");

    Soln.emplace_back(steps.size());
    Soln.back().SetTitle(";p_{reg};|A #times E_{true}|");

    std::cout << "Set channel directory." << std::endl;
    TDirectory *chan_dir =
        dir->mkdir(DescribeFDConfig(FDfdConfig_enum).c_str());
    chan_dir->cd();
    std::cout << "Channel directory set." << std::endl;

    //------------------------------------
    NDFD_Matrix SmearMatrices(state.NDMatrixPredInterps[NDConfig_enum].get(),
                              state.FDMatrixPredInterps[FDConfig_enum].get(),
                              RegFactorExtrap);

    state.PRISM->SetNDFDDetExtrap(SmearMatrices);

    MCEffCorrection NDFDEffCorr(state.NDUnselTruePredInterps[kND_293kA_nu].get(),
                                state.NDSelTruePredInterps[kND_293kA_nu].get(),
                                state.NDUnselTruePredInterps[kND_280kA_nu].get(),
                                state.NDSelTruePredInterps[kND_280kA_nu].get(),
                                state.FDUnselTruePredInterps[FDfdConfig_enum].get(),
                                state.FDSelTruePredInterps[FDfdConfig_enum].get());

    state.PRISM->SetMC_NDFDEff(NDFDEffCorr);
    //-----------------------------------

    std::vector<double> eta_hat, rho_hat;
    for (size_t sit = 0; sit < steps.size(); ++sit) {

      SmearMatrices.SetRegFactor(steps[sit]);

      auto PRISMComponents =
            state.PRISM->PredictPRISMComponents(calc, kNoShift, ch.second);
      
      double resid_norm = SmearMatrices.GetResid_NormVec().at(6);
      double soln_norm = SmearMatrices.GetSoln_NormVec().at(6);
      std::cout << "resid_norm = " << resid_norm << " ; " << 
        "soln_norm = " << soln_norm << std::endl;

      rho_hat.push_back(std::log(resid_norm));
      eta_hat.push_back(std::log(soln_norm));

      if (!std::isnormal(std::log(resid_norm)) ||
          !std::isnormal(std::log(soln_norm))) {
        continue;
      }

      LCurves.back().SetPoint(sit, resid_norm, soln_norm);
      Resid.back().SetPoint(sit, steps[sit], resid_norm);
      Soln.back().SetPoint(sit, steps[sit], soln_norm);
    }

    DiffLCurves.emplace_back(steps.size() - 8);

    double max_curv = -std::numeric_limits<double>::max();
    double best_reg = 0;
    for (size_t sit = 4; sit < (steps.size() - 4); ++sit) {

      double curv =
          2.0 *
          (deriv(&rho_hat[sit], step) * second_deriv(&eta_hat[sit], step) -
           deriv(&eta_hat[sit], step) * second_deriv(&rho_hat[sit], step)) /
          pow(pow(deriv(&rho_hat[sit], step), 2) +
                  pow(deriv(&eta_hat[sit], step), 2),
              3.0 / 2.0);

      if (!std::isnormal(curv)) {
        continue;
      }

      DiffLCurves.back().SetPoint(sit - 4, steps[sit], curv);

      if (curv > max_curv) {
        max_curv = curv;
        best_reg = steps[sit];
      }
    }

    std::cout << ch.second << " Best reg: " << best_reg << std::endl;
    std::stringstream ss("");

    ss << "(best reg = " << best_reg << ");#lambda;L-curvature";
    DiffLCurves.back().SetTitle(ss.str().c_str());

    chan_dir->WriteTObject(&LCurves.back(), "LCurve");
    chan_dir->WriteTObject(&Resid.back(), "Resid");
    chan_dir->WriteTObject(&Soln.back(), "Soln");
    chan_dir->WriteTObject(&DiffLCurves.back(), "DiffLCurve");

    fluxmatcher.SetStoreDebugMatches();

    fluxmatcher.SetChannelRegFactor(ch.second, 293, best_reg);
    fluxmatcher.SetChannelRegFactor(ch.second, 280, best_reg);

    (void)fluxmatcher.GetFarMatchCoefficients(calc, ch.second, kNoShift);
    fluxmatcher.Write(chan_dir->mkdir("NDFD_matcher"));
    fluxmatcher.SetStoreDebugMatches(false);
  
    SmearMatrices.Write(chan_dir->mkdir("Unfold_Matrices"));
    NDFDEffCorr.Write(chan_dir->mkdir("MCEfficiency"));
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

  for (fhicl::ParameterSet const &reg_scan :
       ps.get<std::vector<fhicl::ParameterSet>>("extrap_reg_scans")) {
    PRISMRegOptimizer(reg_scan);
  }
}
