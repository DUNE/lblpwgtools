//
// Created by hasnipl on 29/06/2020.
//

#include "CAFAna/Analysis/AnalysisVars.h"
#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/Core/SystShifts.h"

#include "CAFAna/Experiment/ReactorExperiment.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"

//#include "CAFAna/Fit/MinuitFitter.h"

#include "CAFAna/PRISM/EigenUtils.h"
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

using namespace ana;
using namespace PRISM;

std::map<std::string, PRISMStateBlob> States;

void PRISMScan(fhicl::ParameterSet const &scan) {

  std::string const &state_file = scan.get<std::string>("state_file");

  std::vector<std::string> const &output_file =
    scan.get<std::vector<std::string>>("output_file");
  std::cout << "Output file: " << output_file[0] << std::endl;

  std::string const &output_dir = scan.get<std::string>("output_dir", "");

  std::string const &varname =
  scan.get<std::string>("projection_name", "EProxy");
  
  // default to 1 year
  double POT = scan.get<double>("POT_years", 1) * POT120;
  double POT_FD = POT * pot_fd_FVMassFactor;
  std::cout << "POT : " << POT << ", " << POT_FD << std::endl;

  bool use_PRISM = scan.get<bool>("use_PRISM", true);
  bool vary_NDFD_MCData = scan.get<bool>("vary_NDFD_data", false);

  //auto GOFps = scan.get<fhicl::ParameterSet>("GOF", {});
  bool use_PRISM_ND_stats = scan.get<bool>("use_ND_stats", true);

  //bool fit_nuisance = GOFps.get<bool>("fit_nuisance", false);
  //bool poisson_throw = GOFps.get<bool>("poisson_throw", false);

  std::vector<const IFitVar *> free_oscpars = GetOscVars(
    scan.get<std::vector<std::string>>("free_osc_params", {}));
  std::vector<ISyst const *> freesysts = ana::GetListOfSysts(
    scan.get<std::vector<std::string>>("free_syst_params", {}));

  for (auto &s : freesysts) {
    std::cout << "\t" << s->ShortName() << " free." << std::endl;
  }

  SystShifts shift = kNoShift;

  auto PRISMps = scan.get<fhicl::ParameterSet>("PRISM", {});

  auto RunPlans = PRISMps.get<fhicl::ParameterSet>("RunPlans", {});
  //bool PRISM_write_debug = PRISMps.get<bool>("write_debug", false);
  bool Use_EventRateMatching = PRISMps.get<bool>("Use_EventRateMatching", false);
  double RegFactorExtrap = PRISMps.get<double>("reg_factor_extrap");
  std::cout << "Reg for Extrap = " << RegFactorExtrap << std::endl;

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

  // Get x and y parameter names
  int nparams = param_names.size();
  std::string xparam_name = param_names[0];
  std::string yparam_name;
  if (param_names.size() > 1) yparam_name = param_names[1];
  bool dmsq32_scan(false), ssth23_scan(false), dcp_scan(false), ssth13_scan(false);

  // vector<const IFitVar *> of parameters to scan
  std::vector<const IFitVar *> scan_vars = GetOscVars(param_names);

  // Don't let scan params move freely
  ScrubOscVars(free_oscpars, param_names);

  // set up seed points for multiple fits.
  std::map<const IFitVar *, std::vector<double>> oscSeeds;
  if (std::find(free_oscpars.begin(), free_oscpars.end(),
    &kFitDeltaInPiUnits) != free_oscpars.end()) {
    oscSeeds[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};
  }
  if (std::find(free_oscpars.begin(), free_oscpars.end(), &kFitSinSqTheta23) !=
    free_oscpars.end()) {
    oscSeeds[&kFitSinSqTheta23] = {0.4, 0.6};
  }

  bool dmsq32_scale = false;
  if (std::find(scan_vars.begin(), scan_vars.end(), &kFitDmSq32NHScaled) !=
      scan_vars.end())
    dmsq32_scale = true;

  osc::IOscCalcAdjustable *calc =
    ConfigureCalc(scan.get<fhicl::ParameterSet>("true_osc", {}));

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

  TFile f(output_file[0].c_str(),
    output_file.size() > 1 ? output_file[1].c_str() : "RECREATE");

  std::cout << "Output file: " << output_file[0] << std::endl;

  TDirectory *dir = &f;
  if (output_dir.size()) {
    dir = f.mkdir(output_dir.c_str());
  }
  dir->cd();

  PRISMExtrapolator fluxmatcher;

  if(use_PRISM) {
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

  if (vary_NDFD_MCData) {
    std::cout << "Apply systs to ND and FD MC data. No shifts on weights." << std::endl;
  } else {
    std::cout << "Nominal MC as mock-data. Apply flux systs to weights." << std::endl;
  }
  state.PRISM->SetVaryNDFDMCData(vary_NDFD_MCData);

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

  // Vector of your different experiment objects which contribute to Chi2
  std::vector<std::unique_ptr<IExperiment>> Expts;
  Expts.reserve(Channels.size());
  // Use reactor contraint.
  Expts.emplace_back(new ReactorExperiment(0.088, 0.003));

  MultiExperiment CombExpts;

  CombExpts.Add(Expts.back().get());

  // Try defining extrapolator object before channel loop
  NDFD_Matrix SmearMatrices(RegFactorExtrap);
  MCEffCorrection NDFDEffCorr;

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

    TDirectory *chan_dir =
      dir->mkdir(DescribeFDConfig(FDfdConfig_enum).c_str());
    chan_dir->cd();

    // New data prediction object to compare PRISM prediction to.
    // This is the 'correct' FD data we want to use.
    auto FarDetDataPred = state.FarDetDataPreds[FDfdConfig_enum]->Predict(calc);
    auto *DataPred = FarDetDataPred.ToTHX(POT_FD);
    DataPred->Scale(1, "width");
    chan_dir->WriteTObject(DataPred, "DataPred_Total");
    DataPred->SetDirectory(nullptr);
 
    std::cout << "Set up matrices and efficiency correction." << std::endl;

    // Begin ND to FD extrapolation
    SmearMatrices.Initialize(state.NDMatrixPredInterps[NDConfig_enum].get(),
                              state.FDMatrixPredInterps[FDfdConfig_enum].get());
    state.PRISM->SetNDFDDetExtrap(&SmearMatrices);
    // ND to FD MC efficiency correction
    NDFDEffCorr.Initialize(state.NDUnselTruePredInterps[kND_293kA_nu].get(),
                            state.NDSelTruePredInterps[kND_293kA_nu].get(),
                            state.NDUnselTruePredInterps[kND_280kA_nu].get(), 
                            state.NDSelTruePredInterps[kND_280kA_nu].get(),
                            state.FDUnselTruePredInterps[FDfdConfig_enum].get(),
                            state.FDSelTruePredInterps[FDfdConfig_enum].get());
    // Set PredictionPRISM to own a pointer to this MCEffCorrection
    state.PRISM->SetMC_NDFDEff(&NDFDEffCorr);

    std::cout << "Calculate nominal PRISM prediction." << std::endl;

    if (use_PRISM) {
      auto PRISMComponents =
        state.PRISM->PredictPRISMComponents(calc, shift, ch.second);
      auto *PRISMPred =
            PRISMComponents.at(PredictionPRISM::kPRISMPred).ToTHX(POT_FD);
      PRISMPred->Scale(1, "width");
      chan_dir->WriteTObject(PRISMPred, "PRISMPred");
      PRISMPred->SetDirectory(nullptr);
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
    }

    std::cout << "Fill Experiment objects." << std::endl;

    Expts.emplace_back(new PRISMChi2CovarExperiment(state.PRISM.get(),
                                                    FarDetDataPred.FakeData(POT_FD),
                                                    use_PRISM_ND_stats,
                                                    POT, POT_FD, ch.second, {0, 6}));

    CombExpts.Add(Expts.back().get());
  
  }   

  dir->cd();

  std::vector<double> x_scan;
  std::vector<double> y_scan;
  std::unique_ptr <TH1D> scan_hist_1D;
  std::unique_ptr <TH2D> scan_hist_2D;

  std::cout << "Create histograms to fill with minimisation result." << std::endl;

  if (nparams == 1) {
    const int NXSteps = steps.at(0).at(0);
    const double x_low_bound = dmsq32_scale ? steps.at(0).at(1) * 1000 : steps.at(0).at(1);
    const double x_high_bound = dmsq32_scale ? steps.at(0).at(2) * 1000 : steps.at(0).at(2);
    std::cout << NXSteps << ", " << x_low_bound << ", " << x_high_bound << std::endl;
    scan_hist_1D = std::make_unique<TH1D>("dchi2_1D", "dchi2_1D",
                                           NXSteps, x_low_bound, x_high_bound);

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

    // place scan points in vector
    for (int i = 0; i < scan_hist_2D->GetNbinsX(); i++)
      x_scan.emplace_back(scan_hist_2D->GetXaxis()->GetBinCenter(i + 1));
    for (int i = 0; i < scan_hist_2D->GetNbinsY(); i++)
      y_scan.emplace_back(scan_hist_2D->GetYaxis()->GetBinCenter(i + 1));
  }

  const IFitVar *ssTh23 = &kFitSinSqTheta23;
  const IFitVar *dmsq32 = &kFitDmSq32Scaled;
  const IFitVar *dCPpi = &kFitDeltaInPiUnits; 
  const IFitVar *ssTh13 = &kFitSinSq2Theta13;


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
    std::cout << "found ssth13" << std::endl;
    ssth13_scan = true;
  }

  //-----------------------
  // Do the minimization 1D
  //-----------------------
  if (nparams == 1) {
    for (const auto &x : x_scan) {
      if (ssth23_scan) {
        ssTh23->SetValue(calc, x);
      } else if (dmsq32_scan) {
        dmsq32->SetValue(calc, x);
      } else if (dcp_scan) {
        dCPpi->SetValue(calc, x);
      } else if (ssth13_scan) {
        ssTh13->SetValue(calc, x);
      }

      std::cout << "dmsq32 = " << calc->GetDmsq32() << std::endl;
      std::cout << "ssth23 = " << calc->GetTh23() << std::endl;
      std::cout << "dCP = " << calc->GetdCP() << std::endl;
      std::cout << "ssth13 = " << calc->GetTh13() << std::endl;
 
      auto calc_fit = calc->Copy();
 
      std::cerr << "[INFO]: Beginning fit. ";
      auto start_fit = std::chrono::system_clock::now();
      MinuitFitter fitter(&CombExpts, free_oscpars, freesysts, MinuitFitter::kNormal);
      SystShifts bestSysts;
      //const SeedList &seedPts = SeedList(); //oscSeeds
      //double chi = fitter.Fit(calc_fit, bestSysts, oscSeeds, {}, MinuitFitter::kVerbose);
      /*std::unique_ptr<MinuitFitSummary>*/auto FitSummary = fitter.Fit(calc_fit, bestSysts, 
                                                     oscSeeds, {}, MinuitFitter::kVerbose);
      double chi = FitSummary->EvalMetricVal();
      // fill hist
      scan_hist_1D->Fill(x, chi);
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
    // Sometimes try 1D fit interactively, so keep this.
    // Get minimum ChiSq value (LL value)
    double minchi = 1e10;
    int minx, miny;
    if (nparams == 1) {
      //double minchi = 1e10;
      minx = scan_hist_1D->GetNbinsX() / 2;
      for (int x = 1; x <= scan_hist_1D->GetNbinsX(); ++x) {
        const double chi = scan_hist_1D->GetBinContent(x);
        if (chi < minchi) {
          minchi = chi;
          minx = x;
        }
      }
    }
    if (ssth23_scan) ssTh23->SetValue(calc, scan_hist_1D->GetXaxis()->GetBinCenter(minx));
    else if (dmsq32_scan) dmsq32->SetValue(calc, scan_hist_1D->GetXaxis()->GetBinCenter(minx));
    else if (dcp_scan) dCPpi->SetValue(calc, scan_hist_1D->GetXaxis()->GetBinCenter(minx));
    else if (ssth13_scan) ssTh13->SetValue(calc, scan_hist_1D->GetXaxis()->GetBinCenter(minx));
    std::cout << "Bestfit parameter values: " <<
      scan_hist_1D->GetXaxis()->GetBinCenter(minx) << std::endl;
    
    dir->WriteTObject(scan_hist_1D.release(), "dChi2Scan");
  }
  //-----------------------
  // Do the minimization 2D
  //-----------------------
  else if (param_names.size() > 1) {
    for (const auto &x : x_scan) {
      for (const auto &y : y_scan) {
        std::cout << "x = " << x << ", y = " << y << std::endl;
        if (ssth23_scan) ssTh23->SetValue(calc, x); // ssth23 always on x axis
        if (dmsq32_scan) dmsq32->SetValue(calc, y); // dmsq32 always on y axis
        if (dcp_scan) dCPpi->SetValue(calc, x); // dCP always on x axis
        if (ssth13_scan) ssTh13->SetValue(calc, y); // ssth13 always on y axis
        std::cerr << "[INFO]: Beginning fit. ";
        auto start_fit = std::chrono::system_clock::now();

        MinuitFitter fitter(&CombExpts, free_oscpars, freesysts); 
        SystShifts bestSysts;
        //const SeedList &seedPts = SeedList();
        
        //double chi = fitter.Fit(calc, bestSysts, oscSeeds, {}, MinuitFitter::kVerbose);
        auto FitSummary = fitter.Fit(calc, bestSysts, oscSeeds, {}, MinuitFitter::kVerbose);
        double chi = FitSummary->EvalMetricVal();
        // fill hist
        scan_hist_2D->Fill(x, y, chi);
        auto end_fit = std::chrono::system_clock::now();
        std::cerr << "[FIT]: Finished fit in "
                  << std::chrono::duration_cast<std::chrono::seconds>(end_fit -
                                                                      start_fit).count()
                  << " s after " << fitter.GetNFCN() << " iterations."
                  << std::endl;
      }
    }
    // Simply write Chi2 2D histogram
    dir->WriteTObject(scan_hist_2D.release(), "dChi2Scan");
  }

  // Write total output file
  f.Write();
}

//-------------------------------------------

// main function

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
       ps.get<std::vector<fhicl::ParameterSet>>("scans")) {
    PRISMScan(pred);
  }
}
