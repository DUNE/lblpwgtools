#include "CAFAna/Analysis/AnalysisVars.h"
#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/PRISM/PRISMExtrapolator.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PredictionPRISM.h"
#include "CAFAna/PRISM/EigenUtils.h"

#include "CAFAna/Systs/DUNEFluxSysts.h"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"

#include "TGraphErrors.h"

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

  for (auto const &syst : shift.ActiveSysts()) {
    std::cout << "Syst " << syst->ShortName() << std::endl; //" with shift = " << 
      //syst.GetShift() << std::endl;
  }

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

    // Smearing matrices for ND and FD
    // For detector and selection corrections
    NDFD_Matrix SmearMatrices(state.NDMatrixPredInterps[NDConfig_enum].get(), 
                              state.FDMatrixPredInterps[FDConfig_enum].get(),
                              RegFactorExtrap);
    // Set PredictionPRISM to own a pointer to this NDFD_Matrix
    state.PRISM->SetNDFDDetExtrap(&SmearMatrices);
    // MC efficiency correction
    MCEffCorrection NDFDEffCorr(state.NDUnselTruePredInterps[kND_293kA_nu].get(),
                                state.NDSelTruePredInterps[kND_293kA_nu].get(),                                                       state.NDUnselTruePredInterps[kND_280kA_nu].get(),
                                state.NDSelTruePredInterps[kND_280kA_nu].get(),
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
        chan_dir->WriteTObject(PRISMExtrap, "PRISMExtrap");

        if (PRISM_write_debug) {
          fluxmatcher.Write(chan_dir->mkdir("NDFD_matcher"));
        }
      

        //----------------------------------------------------
        // Scan for variation due to systematics
        //----------------------------------------------------

        chan_dir->cd();

        std::vector<double> AvVx, AvVy, err68pc;
        // x points on graph are bin centers of PRISM prediction
        for (int i = 0; i < PRISMExtrap->GetNbinsX(); i++) {
          AvVx.push_back(PRISMExtrap->GetXaxis()->GetBinCenter(i + 1));
        }

        std::vector<std::vector<double>> toGetErr;
        SystShifts shift_throw = shift;
        int number_throws(5000);
        int step_count(0);
        std::cout << "Start syst throws. Doing " << number_throws << " throws." << std::endl;
        for (int j = 1; j <= number_throws; j++) {
            //step_count += (number_throws / 10);
          //std::cout << j; 
          std::vector<double> throws;
          for (auto const &syst : shift_throw.ActiveSysts()) {
            //std::cout << "min = " << syst->Min() << std::endl;
            shift_throw.SetShift(syst,
              GetBoundedGausThrow(syst->Min() * 0.8, syst->Max() * 0.8));
          }

          auto PRISM_ShiftedComps = state.PRISM->
            PredictPRISMComponents(calc, shift_throw, ch.second);
          auto PRISM_NomComps = state.PRISM->
            PredictPRISMComponents(calc, kNoShift, ch.second);
  
          TH1D *PRISM_Shift_h = PRISM_ShiftedComps.at(PredictionPRISM::kNDDataCorr_FDExtrap)
                                  .ToTH1(POT_FD);
          TH1D *PRISM_Nom_h = PRISM_NomComps.at(PredictionPRISM::kNDDataCorr_FDExtrap)
                                  .ToTH1(POT_FD);
          TH1D *FDUnOsc_h = PRISM_NomComps.at(PredictionPRISM::kFDUnOscPred) 
                                  .ToTH1(POT_FD);

          for (int ebin = 1; ebin <= PRISMExtrap->GetNbinsX(); ebin++) {
            double fracDiff = (PRISM_Shift_h->GetBinContent(ebin) - 
                               PRISM_Nom_h->GetBinContent(ebin)) /
                               PRISM_Nom_h->GetBinContent(ebin); // PRISM_Nom_h FDUnOsc_h
            throws.push_back(fracDiff * 100);
          }
          toGetErr.push_back(throws); 
          /*if (j == 1) {
            std::cout << "[="; 
          } else if (j == number_throws) {
            std::cout << "=]" << std::endl; 
          } else {
            std::cout << "=";
          }*/
          HistCache::Delete(PRISM_Shift_h);
          HistCache::Delete(PRISM_Nom_h);
          HistCache::Delete(FDUnOsc_h);
        }

        std::vector<double> zero_errX;
        std::vector<std::vector<double>>::iterator row;
        std::vector<double>::iterator col;
        bool plot(true);   

        std::cout << "Finished throws; now calculate std. dev." << std::endl;

        for (int ebin = 0; ebin < PRISMExtrap->GetNbinsX(); ebin++) {
          double sumsq(0), sum(0);
          int N(0);
          for (row = toGetErr.begin(); row != toGetErr.end(); row++) {
          //TAxis h_axis = TAxis(50, -10, 10); // Changed to %
          //TH1D * hTest = HistCache::New("h_throw", &h_axis);
            sum += row->at(ebin);
            N++;
          }
          //std::string str = std::to_string(i);
          //if (plot) hTest->Write((std::string("h_throw_") + str).c_str());
          plot = false;
          double average = sum / N;
          AvVy.push_back(average);

          for (row = toGetErr.begin(); row != toGetErr.end(); row++) {
            sumsq += std::pow((row->at(ebin) - average), 2);
          }
          double var = sumsq / N;
          err68pc.push_back(std::pow(var, 0.5));
          zero_errX.push_back(0);

          //HistCache::Delete(hTest);
        }
        std::unique_ptr<TGraphErrors> g_ShiftVar = std::make_unique<TGraphErrors>(
          AvVx.size(), &AvVx[0], &AvVy[0], &zero_errX[0], &err68pc[0]);

        chan_dir->WriteTObject(g_ShiftVar.release(), "g_ShiftVar");
      
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
