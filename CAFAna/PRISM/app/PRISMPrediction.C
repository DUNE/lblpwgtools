#include "CAFAnaCore/CAFAna/Core/Ratio.h"

#include "CAFAna/Analysis/common_fit_definitions.h"
#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Prediction/IPrediction.h"

#include "CAFAna/PRISM/PRISMExtrapolator.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PRISMDetectorExtrapolation.h"
#include "CAFAna/PRISM/PRISMMCEffCorrection.h"
#include "CAFAna/PRISM/PredictionPRISM.h"

#include "CAFAna/Systs/DUNEFluxSysts.h"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"

#include "TSystem.h"

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
  double POT_yrs = pred.get<double>("POT_years", 1);
  std::string POT_staging = pred.get<std::string>("POT_staging", "nostage");
  auto exp_scale_FD = ExposureScale(POT_yrs, POT_staging);
  auto exp_scale_ND = ExposureScaleND(POT_yrs, POT_staging);
  double POT = exp_scale_ND.first * POT120;
  double POT_FD = exp_scale_FD.first * POT120;
  std::cout << POT_staging << ": Yrs = " << POT_yrs << ", MWYrs = " << exp_scale_ND.second <<
    ", ktMWYrs = " << exp_scale_FD.second << std::endl;

  bool vary_NDFD_MCData = pred.get<bool>("vary_NDFD_data", false);
  bool prism_debugplots = pred.get<bool>("prism_debugplots", false);
  bool use_fake_data = pred.get<bool>("use_fake_data", false);
  bool match_intrinsic_nue_bkg = pred.get<bool>("match_intrinsic_nue", false);
  double unfold_reg_param = pred.get<double>("unfold_reg_param", 0.0001);

  if (vary_NDFD_MCData == true && prism_debugplots == false) {
    std::cout << "[ERROR] you can have just 'prism_debugplots', "
              << "but you cannot have just 'vary_NDFD_data'" << std::endl;
    abort();
  }

  (void)GetListOfSysts();

  SystShifts shift = GetSystShifts(pred.get<fhicl::ParameterSet>("syst", {}));

  auto PRISMps = pred.get<fhicl::ParameterSet>("PRISM", {});

  bool PRISM_SetNDDataErrs =
      PRISMps.get<bool>("set_ND_errors_from_rate", true);

  auto RunPlans = PRISMps.get<fhicl::ParameterSet>("RunPlans", {});

  bool Use_EventRateMatching =
      PRISMps.get<bool>("Use_EventRateMatching", false);

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

  osc::IOscCalcAdjustable *calc =
      ConfigureCalc(pred.get<fhicl::ParameterSet>("true_osc", {}));
   osc::NoOscillations no;

  // Profile memory usage
  ProcInfo_t procinfo;
  gSystem->GetProcInfo(&procinfo);
  std::cerr << "[MEM]: Resident = " << procinfo.fMemResident << std::endl;

  // Lazy load the state file
  if (!States.count(state_file)) {
    TFile *fs = TFile::Open(state_file.c_str());
    if (fs->IsZombie() || !fs) {
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
  
  TFile f(output_file[0].c_str(),
          output_file.size() > 1 ? output_file[1].c_str() : "RECREATE");

  TDirectory *dir = &f;

  if (output_dir.size()) {
    dir = dir->mkdir(output_dir.c_str());
  }

  dir->cd();

  if (run_plan_nu.GetPlanPOT() > 0) {
    state.PRISM->SetNDRunPlan(run_plan_nu, BeamMode::kNuMode);
  }

  if (run_plan_nub.GetPlanPOT() > 0) {
    state.PRISM->SetNDRunPlan(run_plan_nub, BeamMode::kNuBarMode);
  }

  if (PRISM_SetNDDataErrs) {
    std::cout << "Set errors from run-plan event rate" << std::endl;
  } else {
    std::cout << "Set errors from MC" << std::endl;
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

  bool EnabledStats = (getenv("CAFANA_STAT_ERRS") != 0);
  if (EnabledStats) {
    std::cout << "CAFANA_STAT_ERRS Enabled!" << std::endl;
  } else {
    std::cout << "CAFANA_STAT_ERRS disabled!" << std::endl;
  }

  //-------------
  // Create flux matcher object
  PRISMExtrapolator fluxmatcher;

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

  for (auto const &channel_conditioning :
       PRISMps.get<std::vector<fhicl::ParameterSet>>("match_conditioning")) {

    auto ch =
        GetMatchChan(channel_conditioning.get<fhicl::ParameterSet>("chan"));

    double chan_reg_293 =
        channel_conditioning.get<double>("reg_factor_293kA", 1E-16);
    double chan_reg_280 =
        channel_conditioning.get<double>("reg_factor_280kA", 1E-16);
    std::array<double, 2> chan_energy_range =
        channel_conditioning.get<std::array<double, 2>>("energy_range",
                                                        {0, 4});

    fluxmatcher.SetTargetConditioning(ch, chan_reg_293, chan_reg_280,
                                        chan_energy_range);
  }

  if ( PRISM_write_debug )       fluxmatcher.SetStoreDebugMatches();
  if ( match_intrinsic_nue_bkg ) fluxmatcher.SetMatchIntrinsicNue();

  state.PRISM->SetFluxMatcher(&fluxmatcher);
  //-------------

  // Define extrapolator object before channel loop
  NDFD_Matrix SmearMatrices;
  MCEffCorrection NDFDEffCorr;

  for (auto const &ch : Channels) {
    int osc_from = FluxSpeciesPDG(ch.second.from.chan);
    int osc_to = FluxSpeciesPDG(ch.second.to.chan);
    size_t NDConfig_enum = GetConfigFromNuChan(ch.second.from, true);
    size_t NDConfig_293kA = (NDConfig_enum == kND_nu) ? kND_293kA_nu : kND_293kA_nub;
    size_t NDConfig_280kA = (NDConfig_enum == kND_nu) ? kND_280kA_nu : kND_280kA_nub;
    size_t FDConfig_enum = GetConfigFromNuChan(ch.second.to, false);
    size_t FDfdConfig_enum = GetFDConfigFromNuChan(ch.second.to);

    std::cout << "ND Config = " << DescribeConfig(NDConfig_enum) << std::endl;
    std::cout << "FDfd Config = " << DescribeFDConfig(FDfdConfig_enum) << std::endl;

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
    std::cout << "POT_FD = " << POT_FD << ", norm = " << POT_FD / POT120 << std::endl;
    auto FarDetDataPred = state.FarDetDataPreds[FDfdConfig_enum]->Predict(calc).FakeData(POT_FD);
    auto *DataPred = FarDetDataPred.ToTHX(POT_FD);
    DataPred->Scale(1, "width");
    chan_dir->WriteTObject(DataPred, "DataPred_Total");
    DataPred->SetDirectory(nullptr);

    // Data prediction with satistical error from MC rather than made up exposure 
    // stat error
    auto FarDetPred_MCErrs = state.FarDetDataPreds[FDfdConfig_enum]->Predict(calc);
    auto *DataPredMCErrs = FarDetPred_MCErrs.ToTHX(POT_FD);
    DataPredMCErrs->Scale(1, "width");
    chan_dir->WriteTObject(DataPredMCErrs, "DataPred_MCErrs");
    DataPredMCErrs->SetDirectory(nullptr);

    Spectrum FarDetFakeDataBiasPred = Spectrum::Uninitialized();
    if (state.FarDetFakeDataBiasPreds[FDfdConfig_enum]) {
      FarDetFakeDataBiasPred =
          state.FarDetFakeDataBiasPreds[FDfdConfig_enum]->Predict(calc).FakeData(POT_FD);
      auto *FakeDataBiasPred = FarDetFakeDataBiasPred.ToTHX(POT_FD);
      FakeDataBiasPred->Scale(1, "width");
      chan_dir->WriteTObject(FakeDataBiasPred, "FakeDataBiasPred_Total");
      FakeDataBiasPred->SetDirectory(nullptr);
    }

    auto FarDetDataUnOscPred = state.FarDetDataPreds[FDfdConfig_enum]->Predict(&no).FakeData(POT_FD);
    auto *DataUnOscPred = FarDetDataUnOscPred.ToTHX(POT_FD);
    DataUnOscPred->Scale(1, "width");
    chan_dir->WriteTObject(DataUnOscPred, "DataPredUnOsc_Total");
    DataUnOscPred->SetDirectory(nullptr);

    // Smearing matrices for ND and FD
    // For detector and selection corrections
    SmearMatrices.Initialize({state.NDMatrixPredInterps[NDConfig_enum].get(), NDConfig_enum},
                             {state.FDMatrixPredInterps[FDfdConfig_enum].get(), FDfdConfig_enum});
    SmearMatrices.SetUnfoldRegParam(unfold_reg_param);
    // Set PredictionPRISM to own a pointer to this NDFD_Matrix
    state.PRISM->SetNDFDDetExtrap(&SmearMatrices);

    // MC efficiency correction
    NDFDEffCorr.Initialize({state.NDUnselTruePredInterps[NDConfig_293kA].get(), NDConfig_293kA},
                           {state.NDSelTruePredInterps[NDConfig_293kA].get(), NDConfig_293kA},
                           {state.NDUnselTruePredInterps[NDConfig_280kA].get(), NDConfig_280kA},
                           {state.NDSelTruePredInterps[NDConfig_280kA].get(), NDConfig_280kA},
                           {state.FDUnselTruePredInterps[FDfdConfig_enum].get(), FDfdConfig_enum},
                           {state.FDSelTruePredInterps[FDfdConfig_enum].get(), FDfdConfig_enum});

    // Set PredictionPRISM to own a pointer to this MCEffCorrection
    state.PRISM->SetMC_NDFDEff(&NDFDEffCorr);

    //--------------------
    auto PRISMComponents =
        state.PRISM->PredictPRISMComponents(calc, shift, ch.second);
    std::cout << "Done predicting components." << std::endl;
    auto *PRISMExtrap =
          PRISMComponents.at(PredictionPRISM::kNDDataCorr_FDExtrap).ToTHX(POT_FD);
    PRISMExtrap->Scale(1, "width");
    chan_dir->WriteTObject(PRISMExtrap, "NDDataCorr_FDExtrap");
    PRISMExtrap->SetDirectory(nullptr);

    if (PRISMComponents.count(PredictionPRISM::kExtrapCovarMatrix)) {
      auto *PRISMExtrapCovMat =
            PRISMComponents.at(PredictionPRISM::kExtrapCovarMatrix).ToTH2(POT);
      // Careful: covariance matrix needs to be scaled by the factor squared
      PRISMExtrapCovMat->Scale(std::pow(POT_FD/POT, 2));
      PRISMExtrapCovMat->Scale(1, "width");
      chan_dir->WriteTObject(PRISMExtrapCovMat, "ExtrapCovarMatrix");
      PRISMExtrapCovMat->SetDirectory(nullptr);
    }
    // Get nue/numu xsec correction (don't want this bin scaled).
    if (PRISMComponents.count(PredictionPRISM::kFD_NumuNueCorr)) {
      auto *FD_NueNumuCorr =
            PRISMComponents.at(PredictionPRISM::kFD_NumuNueCorr).ToTHX(POT);
      chan_dir->WriteTObject(FD_NueNumuCorr, "FD_NumuNueCorr");
      FD_NueNumuCorr->SetDirectory(nullptr);
    }

    if (PRISM_write_debug) {
      for (auto const &comp : PRISMComponents) {
        if (!PRISMComponents.count(comp.first)) continue;
        // we always write this
        if (comp.first == PredictionPRISM::kNDDataCorr_FDExtrap) {
          continue;
        } else if (comp.first == PredictionPRISM::kExtrapCovarMatrix) {
          continue;
        } else if (comp.first == PredictionPRISM::kFD_NumuNueCorr) {
          continue;
        }

        double POT_NDFD = POT_FD;
        if (comp.second.NDimensions() > 1) POT_NDFD = POT;

        auto *PRISMComp_h = comp.second.ToTHX(POT_NDFD); 
        PRISMComp_h->Scale(1, "width");
        if (PRISMComp_h->Integral() != 0) {
          chan_dir->WriteTObject(
              PRISMComp_h,
              PredictionPRISM::GetComponentString(comp.first).c_str());
        }
      }
      fluxmatcher.Write(chan_dir->mkdir("NDFD_matcher"));
      state.PRISM->Get_NDFD_Matrix()->Write(chan_dir->mkdir("Unfold_Matrices"));
      state.PRISM->Get_MCEffCorrection()->Write(chan_dir->mkdir("MCEfficiency"));
      dir->cd();
    }
    std::cout << "Finished writing." << std::endl;
    if (NDConfig_enum == kND_nu) {
      TH1D *run_plan_nu_h = run_plan_nu.AsTH1(293);
      chan_dir->WriteTObject(run_plan_nu_h, "run_plan_nu_293kA");
      run_plan_nu_h->SetDirectory(nullptr);

      TH1D *run_plan_nu_280kA_h = run_plan_nu.AsTH1(280);
      chan_dir->WriteTObject(run_plan_nu_280kA_h, "run_plan_nu_280kA");
      run_plan_nu_280kA_h->SetDirectory(nullptr);
    } else {
      TH1D *run_plan_nub_h = run_plan_nub.AsTH1(293);
      chan_dir->WriteTObject(run_plan_nub_h, "run_plan_nub_293kA");
      run_plan_nub_h->SetDirectory(nullptr);

      TH1D *run_plan_nub_280kA_h = run_plan_nub.AsTH1(280);
      chan_dir->WriteTObject(run_plan_nub_280kA_h, "run_plan_nub_280kA");
      run_plan_nub_280kA_h->SetDirectory(nullptr);
    }
  }
  std::cout << "Write f." << std::endl;
  f.Write();
}

//-------------------------------------------

void SayUsage(char const *argv[]) {
  std::cout
      << "[USAGE]: " << argv[0] << "\n"
      << "\t--fcl <file>    : Input fcl file - always needed.\n"
      << std::endl;
}

int main(int argc, char const *argv[]) {
  // Make sure systs are applied to ND distributions which are per 1 POT.
  //setenv("CAFANA_PRED_MINMCSTATS", "0", 1);
  setenv("CAFANA_STAT_ERRS", "1", 1);
  gROOT->SetMustClean(false);

  std::string fcl("");
  int opt(1);

  while (opt < argc) {
    if (std::string(argv[opt]) == "-?" || std::string(argv[opt]) == "--help") {
      SayUsage(argv);
      exit(0);
    } else if (std::string(argv[opt]) == "--fcl") {
      fcl = argv[++opt];
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
  // Allow the fhiclcpp to lookup the included fcl scripts
  //cet::filepath_first_absolute_or_lookup_with_dot 
  //  f_maker((ana::FindCAFAnaDir() + "/fcl/PRISM/").c_str());

  //fhicl::ParameterSet const &ps = fhicl::ParameterSet::make(fcl, f_maker);
  fhicl::ParameterSet const &ps = fhicl::make_ParameterSet(fcl);

  for (fhicl::ParameterSet const &pred :
       ps.get<std::vector<fhicl::ParameterSet>>("predictions")) {
    PRISMPrediction(pred);
  }
}
