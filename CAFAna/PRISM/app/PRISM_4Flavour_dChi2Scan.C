//
// Created by hasnipl on 29/06/2020.
//

#include "CAFAna/Analysis/AnalysisVars.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/Core/SystShifts.h"

#include "CAFAna/Experiment/ReactorExperiment.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"

#include "CAFAna/PRISM/PRISMExtrapolator.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PRISMDetectorExtrapolation.h"
#include "CAFAna/PRISM/PRISMMCEffCorrection.h"
#include "CAFAna/PRISM/PredictionPRISM.h"
#include "CAFAna/PRISM/SimpleChi2Experiment.h"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"

#include "TArrow.h"
#include "TCanvas.h"
#include "TGraph2D.h"
#include "TMarker.h"
#include "TSystem.h"
#include <map>
#include <utility>
#include <assert.h>

using namespace ana;
using namespace PRISM;

std::map<std::string, PRISMStateBlob> States;

void PRISMScan(fhicl::ParameterSet const &scan, int fit_binx, int fit_biny) {

  std::string const &state_file = scan.get<std::string>("state_file");

  std::vector<std::string> const &output_file =
    scan.get<std::vector<std::string>>("output_file");
  std::string output_file_edit = output_file[0];
  // For when we are fitting one bin at a time
  if (fit_binx != -1) {
    size_t root_pos = output_file_edit.find(".root");
    std::string out_name = output_file_edit.substr(0, root_pos);
    if (fit_biny == -1) {
      output_file_edit = out_name + "_" + std::to_string(fit_binx) + ".root";
    } else {
      output_file_edit = out_name + "_" + std::to_string(fit_binx) + "_" +
                         std::to_string(fit_biny) + ".root";
    }
  }
  std::cout << "Output file: " << output_file_edit << std::endl;

  std::string const &output_dir = scan.get<std::string>("output_dir", "");

  std::string const &varname =
  scan.get<std::string>("projection_name", "EProxy");

  // default to 1 year
  double POT = scan.get<double>("POT_years", 1) * 1.1e21; // POT120 = 1.1e21
  double POT_FD = POT * pot_fd_FVMassFactor;
  std::cout << "POT : " << POT << ", " << POT_FD << std::endl;

  bool vary_NDFD_MCData = scan.get<bool>("vary_NDFD_data", false);
  bool prism_debugplots = scan.get<bool>("prism_debugplots", false); // should always be false
  bool use_fake_data = scan.get<bool>("use_fake_data", false);
  bool match_intrinsic_nue_bkg = scan.get<bool>("match_intrinsic_nue", false);
  bool match_ws_bkg = scan.get<bool>("match_ws", false);
  bool th13_constraint = scan.get<bool>("reactor_constraint", true);

  bool use_PRISM_ND_stats = scan.get<bool>("use_ND_stats", true);

  std::vector<const IFitVar *> free_oscpars = GetOscVars(
    scan.get<std::vector<std::string>>("free_osc_params", {}));
  std::vector<ISyst const *> freesysts = ana::GetListOfSysts(
    scan.get<std::vector<std::string>>("free_syst_params", {}));

  for (auto &o : free_oscpars) {
    std::cout << "\t" << o->ShortName() << " free osc." << std::endl;
  }

  for (auto &s : freesysts) {
    std::cout << "\t" << s->ShortName() << " free syst." << std::endl;
  }

  SystShifts shift = kNoShift;

  auto PRISMps = scan.get<fhicl::ParameterSet>("PRISM", {});

  auto RunPlans = PRISMps.get<fhicl::ParameterSet>("RunPlans", {});
  bool Use_EventRateMatching = PRISMps.get<bool>("Use_EventRateMatching", false);

  bool PRISM_SetNDDataErrs =
    PRISMps.get<bool>("set_ND_errors_from_rate", false);

  RunPlan run_plan_nu, run_plan_nub;

  if (RunPlans.has_key("numode")) {
    run_plan_nu =
      make_RunPlan(RunPlans.get<fhicl::ParameterSet>("numode"), POT);
  }
  if (RunPlans.has_key("nubmode")) {
    run_plan_nub =
      make_RunPlan(RunPlans.get<fhicl::ParameterSet>("nubmode"), POT);
  }

  auto params = scan.get<std::vector<fhicl::ParameterSet>>("scan_params");
  std::vector<std::string> param_names;
  std::vector<std::vector<double>> steps;
  for (auto &sp : params) {
    param_names.push_back(sp.get<std::string>("name", ""));
    std::cout << "param name: " << sp.get<std::string>("name", "") << std::endl;
    auto vec_element = sp.get<std::vector<double>>("scan_steps");
    for (auto &v : vec_element)
      std::cout << "scan def: " << v << std::endl;
    steps.push_back(vec_element);
  }

  //-------------------------------------
  // For when we are fitting one bin at a time on the Grid
  if (fit_binx != -1 && fit_biny == -1 && steps.size() == 1) { // 1D scan
    for (auto &sp : steps) {
      double scan_step = (sp.at(2) - sp.at(1)) / sp.at(0);
      double first_step = sp.at(1);
      sp.at(1) = first_step + (fit_binx - 1) * scan_step;
      sp.at(2) = first_step + (fit_binx) * scan_step;
      sp.at(0) = 1;
    }
  } else if (fit_binx != -1 && fit_biny != -1 && steps.size() == 2) { // 2D scan
      // X binning
      double scan_stepx = (steps.at(0).at(2) - steps.at(0).at(1)) / steps.at(0).at(0);
      double first_stepx = steps.at(0).at(1);
      steps.at(0).at(1) = first_stepx + (fit_binx - 1) * scan_stepx;
      steps.at(0).at(2) = first_stepx + (fit_binx) * scan_stepx;
      steps.at(0).at(0) = 1;
      // Y binning
      double scan_stepy = (steps.at(1).at(2) - steps.at(1).at(1)) / steps.at(1).at(0);
      double first_stepy = steps.at(1).at(1);
      steps.at(1).at(1) = first_stepy + (fit_biny - 1) * scan_stepy;
      steps.at(1).at(2) = first_stepy + (fit_biny) * scan_stepy;
      steps.at(1).at(0) = 1;
  } else if (fit_binx == -1 && fit_biny == -1) {
    // do nothing - we are not specifying bins
  } else {
    std::cout << "[ERROR] Make sure you are defining bins correctly for given ScanDefinitions."
      << std::endl;
    exit(1);
  }
  //------------------------------------

  // Get x and y parameter names
  int nparams = param_names.size();
  std::string xparam_name = param_names[0];
  std::string yparam_name;
  if (param_names.size() > 1) yparam_name = param_names[1];
  bool dmsq32_scan(false), ssth23_scan(false), dcp_scan(false), ss2th13_scan(false);

  // vector<const IFitVar *> of parameters to scan
  std::vector<const IFitVar *> scan_vars = GetOscVars(param_names, 1, 0);

  // Don't let scan params move freely
  ScrubOscVars(free_oscpars, param_names);

  // set up seed points for multiple fits.
  std::map<const IFitVar *, std::vector<double>> oscSeeds;
  if (std::find(free_oscpars.begin(), free_oscpars.end(),
    &kFitDeltaInPiUnits) != free_oscpars.end()) {
    oscSeeds[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};
  }
  //if (std::find(free_oscpars.begin(), free_oscpars.end(), &kFitSinSqTheta23) !=
  //  free_oscpars.end()) {
  //  oscSeeds[&kFitSinSqTheta23] = {0.4, 0.6};
  //}

  SeedList oscSeedsList(oscSeeds);

  bool dmsq32_scale = false;
  if (std::find(scan_vars.begin(), scan_vars.end(), &kFitDmSq32NHScaled) !=
      scan_vars.end())
    dmsq32_scale = true;

  // True oscillation point
  osc::IOscCalcAdjustable *calc_true =
    ConfigureCalc(scan.get<fhicl::ParameterSet>("true_osc", {}));
  // Oscillation calculator for fitter
  // For this script it is the same as the true -> asimov-like fitting
  osc::IOscCalcAdjustable *calc_fit = calc_true->Copy();

  std::vector<double> seedValues;
  for (auto &o : free_oscpars) {
    seedValues.push_back(o->GetValue(calc_fit));
  }

  // Lazy load the state file
  if (!States.count(state_file)) {
    TFile *fs = TFile::Open(state_file.c_str());
    if (fs->IsZombie()) {
      std::cout << "[ERROR]: Failed to read file " << state_file << std::endl;
      abort();
    }
    std::cout << "Loading " << varname << " state from " << state_file
      << std::endl;
    States[state_file] = LoadPRISMState(*fs, varname);
    std::cout << "Done!" << std::endl;
    fs->Close();
  }

  PRISMStateBlob &state = States[state_file];

  TFile f(output_file_edit.c_str(),
    output_file.size() > 1 ? output_file[1].c_str() : "RECREATE");

  std::cout << "Output file: " << output_file_edit << std::endl;

  TDirectory *dir = &f;
  if (output_dir.size()) {
    dir = f.mkdir(output_dir.c_str());
  }
  dir->cd();

  if (run_plan_nu.GetPlanPOT() > 0) {
    state.PRISM->SetNDRunPlan(run_plan_nu, BeamMode::kNuMode);
  }

  if (run_plan_nub.GetPlanPOT() > 0) {
    state.PRISM->SetNDRunPlan(run_plan_nub, BeamMode::kNuBarMode);
  }

  state.PRISM->SetNDDataErrorsFromRate(PRISM_SetNDDataErrs);

  if (vary_NDFD_MCData) {
    std::cout << "Apply systs to ND and FD MC data. No shifts on weights." << std::endl;
  } else {
    std::cout << "Nominal MC as mock-data. Apply flux systs to weights." << std::endl;
  }
  state.PRISM->SetVaryNDFDMCData(vary_NDFD_MCData);

  if (prism_debugplots) {
    std::cout << "Add extra plots to output file for histogram studies." << std::endl;
  } else {
    std::cout << "No extra histograms, only do what is necessary for linear combination" << std::endl;
  }
  state.PRISM->SetVaryPRISMDebugPlots(prism_debugplots);

  if (use_fake_data) {
    std::cout << "Use fake data biased MC as FD and ND 'data'." << std::endl;
  } else {
    std::cout << "Use nominal MC as FD and ND 'data'." << std::endl;
  }
  state.PRISM->SetUseFakeData(use_fake_data);

  if (match_intrinsic_nue_bkg) {
    std::cout << "Include FD intrinsic nue in flux matching." << std::endl;
  } else {
    std::cout << "Use FD MC to predict FD intrinsic nue bkg." << std::endl;
  }
  state.PRISM->SetIntrinsicBkgCorr(match_intrinsic_nue_bkg);

  if(match_ws_bkg){
    std::cout <<" include FD WS bkg in PRISM Pred." << std::endl;
  }
  else{
    std::cout<<" Use FD MC to predict FD WS bkg." << std::endl;
  }
  state.PRISM->SetWSBkgCorr(match_ws_bkg);

  std::map<std::string, MatchChan> Channels;
  if (scan.is_key_to_sequence("samples")) {
    for (auto const &fs : scan.get<std::vector<fhicl::ParameterSet>>("samples")) {
      auto ch = GetMatchChan(fs);
      Channels[GetMatchChanShortName(ch)] = ch;
    }
  } else {
    auto ch = GetMatchChan(scan.get<fhicl::ParameterSet>("samples"));
    Channels[GetMatchChanShortName(ch)] = ch;
  }

  //-------------
  // Create flux matcher object
  PRISMExtrapolator fluxmatcher;
  PRISMExtrapolator fluxmatcherWSB;

  if (Use_EventRateMatching) {
    std::cout << "Using event rate matching" << std::endl;
    fluxmatcher.Initialize({
      {"ND_293kA_nu", state.MatchPredInterps[kND_293kA_nu].get()},
      {"ND_280kA_nu", state.MatchPredInterps[kND_280kA_nu].get()},
      {"FD_nu", state.MatchPredInterps[kFD_nu_nonswap].get()},
      {"ND_293kA_nub", state.MatchPredInterps[kND_293kA_nub].get()},
      {"ND_280kA_nub", state.MatchPredInterps[kND_280kA_nub].get()},
      {"FD_nub", state.MatchPredInterps[kFD_nub_nonswap].get()},
    });
    fluxmatcherWSB.Initialize({
        {"ND_293kA_nu", state.MatchPredInterps[kND_293kA_nu].get()},
        {"ND_280kA_nu", state.MatchPredInterps[kND_280kA_nu].get()},
        {"FD_nu", state.MatchPredInterps[kFD_nu_nonswap].get()},
        {"ND_293kA_nub", state.MatchPredInterps[kND_293kA_nub].get()},
        {"ND_280kA_nub", state.MatchPredInterps[kND_280kA_nub].get()},
        {"FD_nub", state.MatchPredInterps[kFD_nub_nonswap].get()},
    });
    std::cout << "Using event rate matching" << std::endl;
  } else {
    std::cout << "Using flux matching" << std::endl;
    fluxmatcher.Initialize({
      {"ND_293kA_nu", state.NDFluxPred_293kA_nu.get()},
      {"ND_280kA_nu", state.NDFluxPred_280kA_nu.get()},
      {"FD_nu", state.FDFluxPred_293kA_nu.get()},
      {"ND_293kA_nub", state.NDFluxPred_293kA_nub.get()},
      {"ND_280kA_nub", state.NDFluxPred_280kA_nub.get()},
      {"FD_nub", state.FDFluxPred_293kA_nub.get()},
    });
  }
  std::cout << "Done filling fluxmatcher." << std::endl;
  for (auto const &channel_conditioning :
    PRISMps.get<std::vector<fhicl::ParameterSet>>("match_conditioning")) {

    auto ch = GetMatchChan(channel_conditioning.get<fhicl::ParameterSet>("chan"));

    double chan_reg_293 = channel_conditioning.get<double>("reg_factor_293kA", 1E-16);
    double chan_reg_280 = channel_conditioning.get<double>("reg_factor_280kA", 1E-16);
    std::array<double, 2> chan_energy_range =
      channel_conditioning.get<std::array<double, 2>>("energy_range",
                                                      {0, 4});

    fluxmatcher.SetTargetConditioning(ch, chan_reg_293, chan_reg_280,
                                     chan_energy_range);
    fluxmatcherWSB.SetTargetConditioning(ch, chan_reg_293, chan_reg_280,
                                       chan_energy_range); //same targetting condition
  }

  if ( match_intrinsic_nue_bkg ) fluxmatcher.SetMatchIntrinsicNue();

  state.PRISM->SetFluxMatcher(&fluxmatcher);
  state.PRISM->SetFluxMatcherWSB(&fluxmatcherWSB);

  // Create a multi experiment
  MultiExperiment CombExpts;
  // Penalty terms for th13, solar mixing and earth density
  std::unique_ptr<Penalizer_GlbLike> glob_penal_exp =
      std::make_unique<Penalizer_GlbLike>(1 /*hie*/, 1 /*oct*/, th13_constraint,
                                          false /*th23*/, false /*dmsq32*/, 0/*asimov*/);
  CombExpts.Add(glob_penal_exp.get());

  // Try defining extrapolator object before channel loop
  NDFD_Matrix SmearMatrices;
  NDFD_Matrix SmearMatricesWSB; // the smearing matrices are taken from the anti channel
  MCEffCorrection NDFDEffCorr;
  MCEffCorrection NDFDEffCorrWSB; //efficinecy correction also from anti channel

  // Keep PRISM experiment objects in scope
  std::vector<std::unique_ptr<PRISMChi2CovarExperiment>> Expts;

  for (auto const &ch : Channels) {

    int osc_from = FluxSpeciesPDG(ch.second.from.chan);
    int osc_to = FluxSpeciesPDG(ch.second.to.chan);
    size_t NDConfig_enum = GetConfigFromNuChan(ch.second.from, true);
    size_t NDConfig_293kA = (NDConfig_enum == kND_nu) ? kND_293kA_nu : kND_293kA_nub;
    size_t NDConfig_280kA = (NDConfig_enum == kND_nu) ? kND_280kA_nu : kND_280kA_nub;
    size_t FDConfig_enum = GetConfigFromNuChan(ch.second.to, false);
    size_t FDfdConfig_enum = GetFDConfigFromNuChan(ch.second.to);

    MatchChan AntiChannel = GetAntiChannel(ch.second);

    size_t NDConfig_enum_antich = GetConfigFromNuChan(AntiChannel.from, true);
    size_t NDConfig_293kA_antich = (NDConfig_enum_antich == kND_nu) ? kND_293kA_nu : kND_293kA_nub;
    size_t NDConfig_280kA_antich = (NDConfig_enum_antich == kND_nu) ? kND_280kA_nu : kND_280kA_nub;
    size_t FDConfig_enum_antich = GetConfigFromNuChan(AntiChannel.to, false);
    size_t FDfdConfig_enum_antich = GetFDConfigFromNuChan(AntiChannel.to);

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

    TDirectory *chan_dir =
      dir->mkdir(DescribeFDConfig(FDfdConfig_enum).c_str());
    chan_dir->cd();

    // New data prediction object to compare PRISM prediction to.
    // This is the 'correct' FD data we want to use.
    auto FarDetDataPred =
        state.FarDetDataPreds[FDfdConfig_enum]->Predict(calc_true).FakeData(POT_FD);
    auto *DataPred = FarDetDataPred.ToTHX(POT_FD);
    DataPred->Scale(1, "width");
    chan_dir->WriteTObject(DataPred, "DataPred_Total");
    DataPred->SetDirectory(nullptr);

    Spectrum FarDetFakeDataBiasPred = Spectrum::Uninitialized();
    if (state.FarDetFakeDataBiasPreds[FDfdConfig_enum]) {
      FarDetFakeDataBiasPred =
          state.FarDetFakeDataBiasPreds[FDfdConfig_enum]->Predict(calc_true).FakeData(POT_FD);
      auto *FakeDataBiasPred = FarDetFakeDataBiasPred.ToTHX(POT_FD);
      FakeDataBiasPred->Scale(1, "width");
      chan_dir->WriteTObject(FakeDataBiasPred, "FakeDataBiasPred_Total");
      FakeDataBiasPred->SetDirectory(nullptr);
    }
    std::cout << "Set up matrices and efficiency correction." << std::endl;

    // Begin ND to FD extrapolation
    SmearMatrices.Initialize(
        {state.NDMatrixPredInterps[NDConfig_enum].get(), NDConfig_enum},
        {state.FDMatrixPredInterps[FDfdConfig_enum].get(), FDfdConfig_enum});

    state.PRISM->SetNDFDDetExtrap(&SmearMatrices);

    // ND to FD MC efficiency correction
    NDFDEffCorr.Initialize(
        {state.NDUnselTruePredInterps[NDConfig_293kA].get(), NDConfig_293kA},
        {state.NDSelTruePredInterps[NDConfig_293kA].get(), NDConfig_293kA},
        {state.NDUnselTruePredInterps[NDConfig_280kA].get(), NDConfig_280kA},
        {state.NDSelTruePredInterps[NDConfig_280kA].get(), NDConfig_280kA},
        {state.FDUnselTruePredInterps[FDfdConfig_enum].get(), FDfdConfig_enum},
        {state.FDSelTruePredInterps[FDfdConfig_enum].get(), FDfdConfig_enum});

    // Set PredictionPRISM to own a pointer to this MCEffCorrection
    state.PRISM->SetMC_NDFDEff(&NDFDEffCorr);

    if(match_ws_bkg){ // smearing matrices and efficiency correction for WSBkg prediction
       SmearMatricesWSB.Initialize(
           {state.NDMatrixPredInterps[NDConfig_enum_antich].get(), NDConfig_enum_antich},
           {state.FDMatrixPredInterps[FDfdConfig_enum_antich].get(), FDfdConfig_enum_antich});
       state.PRISM->SetNDFDDetExtrapAntiChannel(&SmearMatricesWSB);

       // MC efficiency correction
       NDFDEffCorrWSB.Initialize(
           {state.NDUnselTruePredInterps[NDConfig_293kA_antich].get(), NDConfig_293kA_antich},
           {state.NDSelTruePredInterps[NDConfig_293kA_antich].get(), NDConfig_293kA_antich},
           {state.NDUnselTruePredInterps[NDConfig_280kA_antich].get(), NDConfig_280kA_antich},
           {state.NDSelTruePredInterps[NDConfig_280kA_antich].get(), NDConfig_280kA_antich},
           {state.FDUnselTruePredInterps[FDfdConfig_enum_antich].get(), FDfdConfig_enum_antich},
           {state.FDSelTruePredInterps[FDfdConfig_enum_antich].get(), FDfdConfig_enum_antich});
       state.PRISM->SetMC_NDFDEffAntiChannel(&NDFDEffCorrWSB);
    }

    std::cout << "Calculate nominal PRISM prediction." << std::endl;

    auto PRISMComponents =
      state.PRISM->PredictPRISMComponents(calc_true, shift, ch.second);
    auto *PRISMExtrap =
      PRISMComponents.at(PredictionPRISM::kNDDataCorr_FDExtrap).ToTHX(POT_FD);
    PRISMExtrap->Scale(1, "width");
    chan_dir->WriteTObject(PRISMExtrap, "PRISMExtrap");
    PRISMExtrap->SetDirectory(nullptr);
    auto *PRISMExtrapCovMat =
          PRISMComponents.at(PredictionPRISM::kExtrapCovarMatrix).ToTHX(POT);
    PRISMExtrapCovMat->Scale(std::pow(POT_FD/POT, 2));
    PRISMExtrapCovMat->Scale(1, "width");
    chan_dir->WriteTObject(PRISMExtrapCovMat, "ExtrapCovarMatrix");
    PRISMExtrapCovMat->SetDirectory(nullptr);

    auto PRISMPred_spec =
      PRISMComponents.at(PredictionPRISM::kNDDataCorr_FDExtrap);

    std::cout << "Fill Experiment objects." << std::endl;

    // FarDetFakeDataBiasPred FarDetDataPred
    Expts.emplace_back(new PRISMChi2CovarExperiment(state.PRISM.get(),
                                                    (use_fake_data ?
                                                    FarDetFakeDataBiasPred.FakeData(POT_FD) :
                                                    FarDetDataPred.FakeData(POT_FD)),
                                                    use_PRISM_ND_stats,
                                                    POT, POT_FD, ch.second));

    if (PRISMPred_spec.NDimensions() == 1) {
      Expts.back().get()->SetFitBoundaries(0.5, 10);
    }
    CombExpts.Add(Expts.back().get());
  }

  dir->cd();

  std::vector<double> x_scan;
  std::vector<double> y_scan;
  std::unique_ptr<TH1D> scan_hist_1D;
  std::unique_ptr<TH2D> scan_hist_2D;

  std::cout << "Create histograms to fill with minimisation result." << std::endl;

  if (nparams == 1) {
    const int NXSteps = steps.at(0).at(0);
    const double x_low_bound = dmsq32_scale ? steps.at(0).at(1) * 1000 : steps.at(0).at(1);
    const double x_high_bound = dmsq32_scale ? steps.at(0).at(2) * 1000 : steps.at(0).at(2);
    std::cout << NXSteps << ", " << x_low_bound << ", " << x_high_bound << std::endl;
    scan_hist_1D = std::make_unique<TH1D>("dchi2_1D", "dchi2_1D",
                                           NXSteps, x_low_bound, x_high_bound);
    scan_hist_1D->SetDirectory(nullptr);
    // place the scan points in vector
    for (int i = 0; i < scan_hist_1D->GetNbinsX(); i++) {
      x_scan.emplace_back(scan_hist_1D->GetXaxis()->GetBinCenter(i + 1));
    }
    // else its a 2D hist
  } else {

    const int NXSteps = steps.at(0).at(0);
    const double x_low_bound = steps.at(0).at(1);
    const double x_high_bound = steps.at(0).at(2);
    const int NYSteps = steps.at(1).at(0);
    const double y_low_bound = dmsq32_scale ? steps.at(1).at(1) * 1000 : steps.at(1).at(1);
    const double y_high_bound = dmsq32_scale ? steps.at(1).at(2) * 1000 : steps.at(1).at(2);
    std::cout << NXSteps << ", " << x_low_bound << ", " << x_high_bound << std::endl;
    std::cout << NYSteps << ", " << y_low_bound << ", " << y_high_bound << std::endl;
    scan_hist_2D = std::make_unique<TH2D>("dchi2_2D", "dchi2_2D",
                                          NXSteps, x_low_bound, x_high_bound,
                                          NYSteps, y_low_bound, y_high_bound);
    scan_hist_2D->SetDirectory(nullptr);
    // place scan points in vector
    for (int i = 0; i < scan_hist_2D->GetNbinsX(); i++)
      x_scan.emplace_back(scan_hist_2D->GetXaxis()->GetBinCenter(i + 1));
    for (int i = 0; i < scan_hist_2D->GetNbinsY(); i++)
      y_scan.emplace_back(scan_hist_2D->GetYaxis()->GetBinCenter(i + 1));
  }

  const IFitVar *ssTh23 = &kFitSinSqTheta23;
  const IFitVar *dmsq32 = &kFitDmSq32Scaled;
  const IFitVar *dCPpi = &kFitDeltaInPiUnits;
  const IFitVar *ss2Th13 = &kFitSinSq2Theta13;


  if (std::find(scan_vars.begin(), scan_vars.end(), &kFitSinSqTheta23)
      != scan_vars.end()) {
    std::cout << "found ssth23" << std::endl;
    ssth23_scan = true;
  }
  if (std::find(scan_vars.begin(), scan_vars.end(), &kFitDmSq32NHScaled)
      != scan_vars.end()) {
    std::cout << "found dmsq32" << std::endl;
    dmsq32_scan = true;
  }
  if (std::find(scan_vars.begin(), scan_vars.end(), &kFitDeltaInPiUnits)
      != scan_vars.end()) {
    std::cout << "found dCP in pi units" << std::endl;
    dcp_scan = true;
  }
  if (std::find(scan_vars.begin(), scan_vars.end(), &kFitSinSq2Theta13)
      != scan_vars.end()) {
    std::cout << "found ss2th13" << std::endl;
    ss2th13_scan = true;
  }

  //-----------------------
  // Do the minimization 1D
  //-----------------------
  if (nparams == 1) {
    MinuitFitter fitter(&CombExpts, free_oscpars, freesysts, MinuitFitter::kNormal);
    for (const auto &x : x_scan) {

      // Put oscillation values back to their seed position each iteration
      for(unsigned int i = 0; i < seedValues.size(); ++i)
        free_oscpars.at(i)->SetValue(calc_fit, seedValues.at(i));

      if (ssth23_scan) {
        ssTh23->SetValue(calc_fit, x);
      } else if (dmsq32_scan) {
        dmsq32->SetValue(calc_fit, x);
      } else if (dcp_scan) {
        dCPpi->SetValue(calc_fit, x);
      } else if (ss2th13_scan) {
        ss2Th13->SetValue(calc_fit, x);
      }

      std::cout << "dmsq32 = " << calc_fit->GetDmsq32() << std::endl;
      std::cout << "th23 = " << calc_fit->GetTh23() << std::endl;
      std::cout << "dCP = " << calc_fit->GetdCP() << std::endl;
      std::cout << "th13 = " << calc_fit->GetTh13() << std::endl;

      std::cerr << "[INFO]: Beginning fit. ";
      auto start_fit = std::chrono::system_clock::now();
      SystShifts bestSysts = kNoShift;
      double chi = fitter.Fit(calc_fit, bestSysts, oscSeedsList,
                              {}, MinuitFitter::kVerbose)->EvalMetricVal();
      // fill hist
      double minchi = TMath::Max(chi, 1e-6);
      scan_hist_1D->Fill(x, minchi);
      auto end_fit = std::chrono::system_clock::now();
      // Profile memory usage
      ProcInfo_t procinfo;
      gSystem->GetProcInfo(&procinfo);
      std::cerr << "[MEM]: Resident = " << procinfo.fMemResident << std::endl
                << "[MEM]: Virtual = " << procinfo.fMemVirtual << std::endl;
      std::cerr << "[FIT]: Finished fit in "
                << std::chrono::duration_cast<std::chrono::seconds>(end_fit -
                                                                    start_fit).count()
                << " s after " << fitter.GetNFCN() << " iterations."
                << std::endl;
    }

    dir->WriteTObject(scan_hist_1D.get(), "dChi2Scan");
  }
  //-----------------------
  // Do the minimization 2D
  //-----------------------
  else if (param_names.size() > 1) {
    MinuitFitter fitter(&CombExpts, free_oscpars, freesysts);
    for (const auto &x : x_scan) {
      for (const auto &y : y_scan) {

        // Put oscillation values back to their seed position each iteration
        for(unsigned int i = 0; i < seedValues.size(); ++i)
          free_oscpars.at(i)->SetValue(calc_fit, seedValues.at(i));

        std::cout << "x = " << x << ", y = " << y << std::endl;
        if (ssth23_scan) ssTh23->SetValue(calc_fit, x); // ssth23 always on x axis
        if (dmsq32_scan) dmsq32->SetValue(calc_fit, y); // dmsq32 always on y axis
        if (dcp_scan) dCPpi->SetValue(calc_fit, x); // dCP always on x axis
        if (ss2th13_scan) ss2Th13->SetValue(calc_fit, y); // ss2th13 always on y axis
        std::cerr << "[INFO]: Beginning fit. ";
        auto start_fit = std::chrono::system_clock::now();

        SystShifts bestSysts = kNoShift;
        double chi = fitter.Fit(calc_fit, bestSysts, oscSeedsList,
                                {}, MinuitFitter::kVerbose)->EvalMetricVal();
        // fill hist
        double minchi = TMath::Max(chi, 1e-6);
        scan_hist_2D->Fill(x, y, minchi);
        auto end_fit = std::chrono::system_clock::now();
        std::cerr << "[FIT]: Finished fit in "
                  << std::chrono::duration_cast<std::chrono::seconds>(end_fit -
                                                                      start_fit).count()
                  << " s after " << fitter.GetNFCN() << " iterations."
                  << std::endl;
      }
    }
    // Simply write Chi2 2D histogram
    dir->WriteTObject(scan_hist_2D.get(), "dChi2Scan");
  }

  // Write total output file
  f.Write();
  f.Close();
}

//-------------------------------------------

void SayUsage(char const *argv[]) {
  std::cout
      << "[USAGE]: " << argv[0] << "\n"
      << "\t--fcl <file>    : Input fcl file - always needed.\n"
      << "\t--binx <int>    : [OPTIONAL] Specific x bin to fit (useful on grid).\n"
      << "\t--biny <int>    : [OPTIONAL] Specific y bin to fit (useful on grid).\n"
      << std::endl;
}

// main function

int main(int argc, char const *argv[]) {
  // Make sure systs are applied to ND distributions which are per 1 POT.
  setenv("CAFANA_PRED_MINMCSTATS", "0", 1);
  setenv("CAFANA_STAT_ERRS", "1", 1);
  gROOT->SetMustClean(false);

  int fit_binx(-1), fit_biny(-1), opt(1);
  std::string fcl("");

  while (opt < argc) {
    if (std::string(argv[opt]) == "-?" || std::string(argv[opt]) == "--help") {
      SayUsage(argv);
      exit(0);
    } else if (std::string(argv[opt]) == "--fcl") {
      fcl = argv[++opt];
    } else if (std::string(argv[opt]) == "--binx") {
      fit_binx = atoi(argv[++opt]);
    } else if (std::string(argv[opt]) == "--biny") {
      fit_biny = atoi(argv[++opt]);
    } else {
      std::cout << "[ERROR] Unknown option." << std::endl;
      exit(1);
    }
    opt++;
  }

  if (fcl == "") {
    std::cout << "[ERROR] Need a fcl file." << std::endl;
    exit(1);
  }

  if (fit_biny != -1 && fit_binx == -1) {
    std::cout << "[ERROR] use --binx option for 1D fits." << std::endl;
    exit(1);
  }

  // Allow the fhiclcpp to lookup the included fcl scripts
  cet::filepath_first_absolute_or_lookup_with_dot
    f_maker((ana::FindCAFAnaDir() + "/fcl/PRISM/").c_str());

  fhicl::ParameterSet const &ps = fhicl::ParameterSet::make(fcl, f_maker);

  for (fhicl::ParameterSet const &pred :
       ps.get<std::vector<fhicl::ParameterSet>>("scans")) {
    PRISMScan(pred, fit_binx, fit_biny);
  }
}
