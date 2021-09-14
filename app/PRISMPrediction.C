#include "CAFAna/Analysis/AnalysisVars.h"
#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/Prediction/IPrediction.h"

#include "CAFAna/PRISM/PRISMExtrapolator.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PRISMDetectorExtrapolation.h"
#include "CAFAna/PRISM/PRISMMCEffCorrection.h"
#include "CAFAna/PRISM/PredictionPRISM.h"
#include "CAFAna/PRISM/EigenUtils.h"

#include "CAFAna/Systs/DUNEFluxSysts.h"

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
  bool vary_NDFD_MCData = pred.get<bool>("vary_NDFD_data", false);

  std::pair<double, double> gauss_flux =
      pred.get<std::pair<double, double>>("gauss_flux", {0, 0});

  (void)GetListOfSysts();

  SystShifts shift = GetSystShifts(pred.get<fhicl::ParameterSet>("syst", {}));

  SystShifts fluxshift = FilterFluxSystShifts(shift);

  bool do_gauss = gauss_flux.first != 0;

  auto PRISMps = pred.get<fhicl::ParameterSet>("PRISM", {});

  bool PRISM_SetNDDataErrs =
      PRISMps.get<bool>("set_ND_errors_from_rate", false);

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

  double RegFactorExtrap = PRISMps.get<double>("reg_factor_extrap"); 
  std::cout << "Reg for Extrap = " << RegFactorExtrap << std::endl;

  osc::IOscCalculatorAdjustable *calc =
      ConfigureCalc(pred.get<fhicl::ParameterSet>("true_osc", {}));

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

  TDirectory *dir = &f;

  if (output_dir.size()) {
    dir = dir->mkdir(output_dir.c_str());
  }

  dir->cd();

  PRISMExtrapolator fluxmatcher;
  if (use_PRISM) {

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
    size_t NDConfig_293kA = (NDConfig_enum == kND_nu) ? kND_293kA_nu : kND_293kA_nub;
    size_t NDConfig_280kA = (NDConfig_enum == kND_nu) ? kND_280kA_nu : kND_280kA_nub; 
    size_t FDConfig_enum = GetConfigFromNuChan(ch.second.to, false);
    size_t FDfdConfig_enum = GetFDConfigFromNuChan(ch.second.to);

    std::cout << "ND Config = " << DescribeConfig(NDConfig_enum) << std::endl;
    std::cout << "FD Config = " << DescribeConfig(FDConfig_enum) << std::endl; 
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
    auto FarDetDataPred = state.FarDetDataPreds[FDfdConfig_enum]->Predict(calc);
    auto *DataPred = FarDetDataPred.ToTHX(POT_FD);
    if (FarDetDataPred.NDimensions() == 1) {
      for (int bin_it = 1; bin_it <= DataPred->GetXaxis()->GetNbins(); bin_it++) {
        DataPred->SetBinError(bin_it, sqrt(DataPred->GetBinContent(bin_it)));
      }
    } else if (FarDetDataPred.NDimensions() == 2) {
      for (int x_it = 1; x_it <= DataPred->GetXaxis()->GetNbins(); x_it++) {
        for (int y_it = 1; y_it <= DataPred->GetYaxis()->GetNbins(); y_it++) {
          double be = sqrt(DataPred->GetBinContent(x_it, y_it));
          DataPred->SetBinError(x_it, y_it, be);
        }
      }
    }
    DataPred->Scale(1, "width");
    chan_dir->WriteTObject(DataPred, "DataPred_Total");
    DataPred->SetDirectory(nullptr);

    //----------------------------------------------------------
    // Probably redundant now, may remove in the future.
    // Disappearance: non_swap oscillates numus away.
    // Appearance: non_swap looks at the intrinsic nues.
    DataSpectra.push_back(state.FarDetData_nonswap[FDfdConfig_enum]->Oscillated(
        calc, 
        (IsNueConfig(FDConfig_enum)
        ? osc_to : osc_from),
        osc_to)); 

    TH1 *Data_nonswap = DataSpectra.back().ToTH1(POT_FD);
    Data_nonswap->Scale(1, "width");
    chan_dir->WriteTObject(Data_nonswap, "Data_nonswap_component");

    if (state.Have(GetConfigNueSwap(FDConfig_enum))) {

      DataSpectra.back() +=
          state.FarDetData_nueswap[FDfdConfig_enum]->Oscillated(calc, osc_from,
                                                                osc_to);

      TH1 *Data_nueswap = state.FarDetData_nueswap[FDfdConfig_enum]
                              ->Oscillated(calc, osc_from, osc_to)
                              .ToTH1(POT_FD);
      if (Data_nueswap->Integral() > 0) {
        Data_nueswap->Scale(1, "width");
        chan_dir->WriteTObject(Data_nueswap, "Data_nueswap_component");
      }
      Data_nueswap->SetDirectory(nullptr);
    }

    auto *Data = DataSpectra.back().ToTHX(POT_FD);
    if (DataSpectra.back().NDimensions() == 1) {
      for (int bin_it = 1; bin_it <= Data->GetXaxis()->GetNbins(); bin_it++) {
        Data->SetBinError(bin_it, sqrt(Data->GetBinContent(bin_it)));
      }
    } else if (DataSpectra.back().NDimensions() == 2) {
      for (int x_it = 1; x_it <= Data->GetXaxis()->GetNbins(); x_it++) {
        for (int y_it = 1; y_it <= Data->GetYaxis()->GetNbins(); y_it++) {
          double be = sqrt(Data->GetBinContent(x_it, y_it));
          Data->SetBinError(x_it, y_it, be);
        }
      }
    }
    Data->Scale(1, "width");
    chan_dir->WriteTObject(Data, "Data_Total");
    Data->SetDirectory(nullptr);
    //-------------------------------------------------------------

    // Smearing matrices for ND and FD
    // For detector and selection corrections
    NDFD_Matrix SmearMatrices(state.NDMatrixPredInterps[NDConfig_enum].get(),
                              state.FDMatrixPredInterps[FDfdConfig_enum].get(), 
                              RegFactorExtrap);
    // Set PredictionPRISM to own a pointer to this NDFD_Matrix
    state.PRISM->SetNDFDDetExtrap(&SmearMatrices);
    // MC efficiency correction
    MCEffCorrection NDFDEffCorr(state.NDUnselTruePredInterps[NDConfig_293kA].get(),
                                state.NDSelTruePredInterps[NDConfig_293kA].get(),
                                state.NDUnselTruePredInterps[NDConfig_280kA].get(),
                                state.NDSelTruePredInterps[NDConfig_280kA].get(),
                                state.FDUnselTruePredInterps[FDfdConfig_enum].get(), 
                                state.FDSelTruePredInterps[FDfdConfig_enum].get());
    // Set PredictionPRISM to own a pointer to this MCEffCorrection
    state.PRISM->SetMC_NDFDEff(&NDFDEffCorr);

    if (use_PRISM) {
      if (do_gauss) { // Gaussian spectra prediction
        auto PRISMComponents = state.PRISM->PredictGaussianFlux(
            gauss_flux.first, gauss_flux.second, shift, ch.second.from);
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

            TH1 *PRISMComp_h = comp.second.ToTHX(POT_FD);

            if (PRISMComp_h->Integral() > 0) {
              chan_dir->WriteTObject(
                  PRISMComp_h,
                  PredictionPRISM::GetComponentString(comp.first).c_str());
            }
          }

          fluxmatcher.Write(chan_dir->mkdir("Gauss_matcher"));
          dir->cd();
        }
      } else { // FD spectra prediction
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
        chan_dir->WriteTObject(PRISMExtrap, "NDDataCorr_FDExtrap");
        PRISMExtrap->SetDirectory(nullptr);
        auto *PRISMExtrapCovMat = 
              PRISMComponents.at(PredictionPRISM::kExtrapCovarMatrix).ToTHX(POT);
        // Careful: covariance matrix needs to be scaled by the factor squared
        PRISMExtrapCovMat->Scale(std::pow(POT_FD/POT, 2));
        PRISMExtrapCovMat->Scale(1, "width");
        chan_dir->WriteTObject(PRISMExtrapCovMat, "ExtrapCovarMatrix");
        PRISMExtrapCovMat->SetDirectory(nullptr);
        // Get nue/numu xsec correction (don't want this bin scaled).
        if (PRISMComponents.count(PredictionPRISM::kFD_NumuNueCorr)) {
        auto *FD_NueNumuCorr = 
              PRISMComponents.at(PredictionPRISM::kFD_NumuNueCorr).ToTHX(POT);
        chan_dir->WriteTObject(FD_NueNumuCorr, "FD_NumuNueCorr");
        FD_NueNumuCorr->SetDirectory(nullptr);
        }
        if (PRISM_write_debug) {
          for (auto const &comp : PRISMComponents) {
            // we always write this
            if (comp.first == PredictionPRISM::kPRISMPred) {
              continue;
            } else if (comp.first == PredictionPRISM::kNDDataCorr_FDExtrap) {
              continue;
            } else if (comp.first == PredictionPRISM::kExtrapCovarMatrix) {
              continue;
            } else if (comp.first == PredictionPRISM::kFD_NumuNueCorr) {
              continue;
            }

            auto *PRISMComp_h = comp.second.ToTHX(POT_FD); // POT_FD
            PRISMComp_h->Scale(1, "width");
            if (PRISMComp_h->Integral() != 0) {
              chan_dir->WriteTObject(
                  PRISMComp_h,
                  PredictionPRISM::GetComponentString(comp.first).c_str());
            }
          }

          fluxmatcher.Write(chan_dir->mkdir("NDFD_matcher"));
          SmearMatrices.Write(chan_dir->mkdir("Unfold_Matrices"));
          NDFDEffCorr.Write(chan_dir->mkdir("MCEfficiency"));     

          dir->cd();
        }
      }

    } else {
      auto *FarDetPred = state.FarDetPredInterps[FDfdConfig_enum]
                              ->PredictSyst(calc, shift).ToTHX(POT_FD);
      chan_dir->WriteTObject(FarDetPred, "FarDetPred");
      FarDetPred->SetDirectory(nullptr);
    }

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
