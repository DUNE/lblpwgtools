#include "CAFAna/Analysis/AnalysisVars.h"
#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/Core/HistCache.h"

#include "CAFAna/PRISM/PRISMExtrapolator.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PredictionPRISM.h"
#include "CAFAna/PRISM/SimpleChi2Experiment.h"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"

#include "TArrow.h"
#include "TCanvas.h"
#include "TGraph2D.h"
#include "TMarker.h"

using namespace ana;
using namespace PRISM;

std::map<std::string, PRISMStateBlob> States;

TGraph GetContour(TH2 *h, double redchi2l) {
  TGraph outg;
  std::unique_ptr<TH2> hconts(static_cast<TH2 *>(h->Clone("hconts")));
  hconts->SetDirectory(nullptr);
  hconts->Smooth(1);

  TCanvas c("c", "");

  double contlev[1];
  contlev[0] = redchi2l;
  hconts->SetContour(1, contlev);
  hconts->Draw("CONT LIST");
  c.Update();
  c.Clear();
  std::unique_ptr<TH2> dummyhist(static_cast<TH2 *>(h->Clone("dummyhist")));
  for (int xbin = 0; xbin <= dummyhist->GetNbinsX(); xbin++) {
    for (int ybin = 0; ybin <= dummyhist->GetNbinsY(); ybin++) {
      dummyhist->SetBinContent(xbin, ybin, 0);
    }
  }
  dummyhist->Draw();
  gPad->Update();
  TObjArray *contours =
      (TObjArray *)gROOT->GetListOfSpecials()->FindObject("contours");
  if (!contours) {
    std::cout << "Failed to find list of contours" << std::endl;
    abort();
  }
  if (!contours->GetEntries()) {
    std::cout << "List contained no contours" << std::endl;
    abort();
  } else {
    std::cout << "Found " << contours->GetEntries() << " contours."
              << std::endl;
  }
  TList *list = (TList *)contours->At(0);
  std::cout << "\twith " << list->GetEntries() << " entries." << std::endl;
  TIter next(list);
  int it = 0;
  TGraph *g;
  while ((g = (TGraph *)next())) {
    std::cout << "Have " << g->GetN() << " points." << std::endl;
    for (int i = 0; i < g->GetN(); ++i) {
      double x, y;
      g->GetPoint(i, x, y);
      // std::cout << "Set point: " << i << ", @ " << x << ", " << y
      // <<std::endl;
      outg.SetPoint(it, x, y);
      it++;
    }
  }
  return outg;
}

TH1 *BuildDChi2Map(std::vector<fhicl::ParameterSet> const &params) {

  size_t nxsteps = 1, nysteps = 1;
  double step_widthx = 0, step_widthy = 0;
  double startx = 0, starty = 0;
  std::string xparam_name = "", yparam_name = "";
  size_t nparams = params.size();

  TH1 *dchi2map;
  if (params.size() == 1) {
    std::array<double, 3> xscan_steps =
        params.front().get<std::array<double, 3>>("scan_steps");
    nxsteps = xscan_steps[0];
    step_widthx = (xscan_steps[2] - xscan_steps[1]) / xscan_steps[0];

    xparam_name = params.front().get<std::string>("name");

    startx = xscan_steps[1];

    std::stringstream ttl("");
    ttl << ";" << xparam_name << ";#Delta#Chi^{2}";

    return new TH1D("dchi2", ttl.str().c_str(), xscan_steps[0], xscan_steps[1],
                    xscan_steps[2]);
  } else if (params.size() == 2) {
    std::array<double, 3> xscan_steps =
        params.front().get<std::array<double, 3>>("scan_steps");
    nxsteps = xscan_steps[0];
    step_widthx = (xscan_steps[2] - xscan_steps[1]) / xscan_steps[0];

    xparam_name = params.front().get<std::string>("name");
    startx = xscan_steps[1];
    std::array<double, 3> yscan_steps =
        params.back().get<std::array<double, 3>>("scan_steps");
    nysteps = yscan_steps[0];
    step_widthy = (yscan_steps[2] - yscan_steps[1]) / yscan_steps[0];

    yparam_name = params.back().get<std::string>("name");

    starty = yscan_steps[1];
    std::stringstream ttl("");
    ttl << ";" << xparam_name << ";" << yparam_name << ";#Delta#Chi^{2}";

    return new TH2D("dchi2", ttl.str().c_str(), xscan_steps[0], xscan_steps[1],
                    xscan_steps[2], yscan_steps[0], yscan_steps[1],
                    yscan_steps[2]);
  } else {
    std::cout << "Can only do 1 or 2 dimensional chi2 scans, but recieved: "
              << params.size() << " parameters." << std::endl;
    abort();
  }
}

void PRISMScan(fhicl::ParameterSet const &scan) {

  std::string const &state_file = scan.get<std::string>("state_file");

  std::vector<std::string> const &output_file =
      scan.get<std::vector<std::string>>("output_file");
  std::string const &output_dir = scan.get<std::string>("output_dir", "");

  std::string const &varname =
      scan.get<std::string>("projection_name", "EProxy");

  // default to 1 year
  double POT = scan.get<double>("POT_years", 1) * POT120;
  double POT_FD = POT * pot_fd_FVMassFactor;

  bool use_PRISM = scan.get<bool>("use_PRISM", true);

  auto GOFps = scan.get<fhicl::ParameterSet>("GOF", {});
  bool use_PRISM_ND_stats = GOFps.get<bool>("use_PRISM_ND_stats", true);

  bool fit_nuisance = GOFps.get<bool>("fit_nuisance", false);
  bool poisson_throw = GOFps.get<bool>("poisson_throw", false);

  auto free_oscpars =
      GetOscVars(GOFps.get<std::vector<std::string>>("free_osc_params", {}));
  auto freesysts = ana::GetListOfSysts(
      GOFps.get<std::vector<std::string>>("free_syst_params", {}));

  for (auto &s : freesysts) {
    std::cout << "\t" << s->ShortName() << " free." << std::endl;
  }

  auto PRISMps = scan.get<fhicl::ParameterSet>("PRISM", {});

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

  auto params = scan.get<std::vector<fhicl::ParameterSet>>("scan_params");
  TH1 *dchi2map = BuildDChi2Map(params);
  dchi2map->SetDirectory(0);

  size_t nparams = dchi2map->GetDimension();

  std::string xparam_name =
      dchi2map->GetXaxis() ? dchi2map->GetXaxis()->GetTitle() : "";
  std::string yparam_name =
      dchi2map->GetYaxis() ? dchi2map->GetYaxis()->GetTitle() : "";

  // Fix osc pars that are in the scan
  ScrubOscVars(free_oscpars, {xparam_name, yparam_name});

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

  osc::IOscCalculatorAdjustable *true_calc =
      ConfigureCalc(scan.get<fhicl::ParameterSet>("true_osc", {}));
  osc::IOscCalculatorAdjustable *calc = true_calc->Copy();

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
    dir = f.mkdir(output_dir.c_str());
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
    state.PRISM->SetFluxMatcher(&fluxmatcher);
  }

  if (run_plan_nu.GetPlanPOT() > 0) {
    state.PRISM->SetNDRunPlan(run_plan_nu, BeamMode::kNuMode);
  }

  if (run_plan_nub.GetPlanPOT() > 0) {
    state.PRISM->SetNDRunPlan(run_plan_nub, BeamMode::kNuBarMode);
  }

  std::map<std::string, MatchChan> Channels;
  if (scan.is_key_to_sequence("samples")) {
    for (auto const &fs :
         scan.get<std::vector<fhicl::ParameterSet>>("samples")) {
      auto ch = GetMatchChan(fs);
      Channels[GetMatchChanShortName(ch)] = ch;
    }
  } else {
    auto ch = GetMatchChan(scan.get<fhicl::ParameterSet>("samples"));
    Channels[GetMatchChanShortName(ch)] = ch;
  }

  std::map<std::string, std::pair<double, double>> EnergyRanges;
  for (auto const &ch : Channels) {
    EnergyRanges[ch.first] = GOFps.get<std::pair<double, double>>(
        "energy_limits." + ch.first, {-std::numeric_limits<double>::max(),
                                      std::numeric_limits<double>::max()});
  }

  std::vector<TH1D *> MinHist(6, nullptr);
  std::vector<TH1D *> NomHist;

  std::vector<TH1D *> InputDataHist;
  std::vector<TH1D *> DataHist;
  size_t NBins = 0;

  std::vector<Spectrum> DataSpectra;
  DataSpectra.reserve(Channels.size());

  std::vector<std::unique_ptr<IChiSqExperiment>> Expts;
  Expts.reserve(Channels.size());

  MultiExperiment MExpt;

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

    if (state.Have(GetConfigNueSwap(FDConfig_enum))) {
      DataSpectra.back() +=
          state.FarDetData_nueswap[FDfdConfig_enum]->Oscillated(calc, osc_from,
                                                                osc_to);
    }

    InputDataHist.emplace_back(DataSpectra.back().ToTH1(POT_FD));
    DataHist.emplace_back(DataSpectra.back().ToTH1(POT_FD));

    if (poisson_throw) {
      for (int bi_it = 0; bi_it < DataHist.back()->GetXaxis()->GetNbins();
           ++bi_it) {
        double bc = DataHist.back()->GetBinContent(bi_it + 1);
        double thrown = gRandom->Poisson(bc);
        DataHist.back()->SetBinContent(bi_it + 1, thrown);
      }
    }

    NBins += DataHist.back()->GetXaxis()->GetNbins();

    if (use_PRISM) {
      Expts.emplace_back(new PRISMChi2Experiment(
          state.PRISM.get(), DataSpectra.back(), use_PRISM_ND_stats, POT_FD,
          ch.second, EnergyRanges[ch.first]));
    } else {

      Expts.emplace_back(new SimpleChi2Experiment(
          state.FarDetPredInterps[FDfdConfig_enum].get(), DataSpectra.back(),
          false, POT_FD, EnergyRanges[ch.first]));
    }

    MExpt.Add(Expts.back().get());

    if (use_PRISM) {
      NomHist.emplace_back(state.PRISM
                               ->PredictPRISMComponents(
                                   calc, kNoShift, ch.second)
                               .at(PredictionPRISM::kPRISMPred)
                               .ToTH1(POT_FD));
    } else {
      NomHist.emplace_back(
          state.FarDetPredInterps[FDfdConfig_enum]->Predict(calc).ToTH1(POT_FD));
    }
  }

  double min_gof = std::numeric_limits<double>::max();
  std::vector<double> gof_scan, gof_fit;

  std::pair<double, double> min_paramvals;
  for (int stepx = 0; stepx < dchi2map->GetXaxis()->GetNbins(); ++stepx) {
    double xval = dchi2map->GetXaxis()->GetBinCenter(stepx + 1);

    std::cout << (fit_nuisance ? "" : "\r") << "(" << stepx << "/"
              << dchi2map->GetXaxis()->GetNbins() << ") " << xparam_name << ": "
              << xval << (fit_nuisance ? "\n" : "") << std::flush;

    for (int stepy = 0;
         stepy < ((nparams == 2) ? dchi2map->GetYaxis()->GetNbins() : 1);
         ++stepy) {

      fhicl::ParameterSet step_ps;
      step_ps.put(xparam_name, xval);

      double yval = 0;
      if (nparams == 2) {
        yval = dchi2map->GetYaxis()->GetBinCenter(stepy + 1);
        step_ps.put(yparam_name, yval);
      }

      double gof = 0;

      (void)ConfigureCalc(step_ps, calc);

      gof = MExpt.ChiSq(calc);
      gof_scan.push_back(gof);

      if (fit_nuisance) {
        auto calc_fit = calc->Copy();

        auto start_fit = std::chrono::system_clock::now();
        // Now set up the fit itself
        std::cerr << "[INFO]: Beginning fit. ";
        MinuitFitter Fitter(&MExpt, free_oscpars, freesysts);
        gof = Fitter.Fit(calc_fit, junkShifts, oscSeeds);
        gof_fit.push_back(gof);
        auto end_fit = std::chrono::system_clock::now();
        std::cerr << "[FIT]: Finished fit in "
                  << std::chrono::duration_cast<std::chrono::seconds>(end_fit -
                                                                      start_fit)
                         .count()
                  << " s after " << Fitter.GetNFCN() << " iterations."
                  << std::endl;
      }

      if (gof < min_gof) {
        size_t ch_it = 0;
        for (auto const ch : Channels) {
          HistCache::Delete(MinHist[ch_it]);
          if (use_PRISM) {
            MinHist[ch_it] =
                static_cast<PRISMChi2Experiment *>(Expts[ch_it].get())
                    ->GetPred(calc);
          } else {
            MinHist[ch_it] =
                static_cast<SimpleChi2Experiment *>(Expts[ch_it].get())
                    ->GetPred(calc);
          }
          ch_it++;
        }
        min_gof = gof;
        min_paramvals.first = xval;
        min_paramvals.second = yval;
      }
    }
  }
  std::cout << "\r" << std::endl;

  TH1 *Scan = static_cast<TH1 *>(dchi2map->Clone("Chi2_Scan")),
      *Scan_d = static_cast<TH1 *>(dchi2map->Clone("Delta_Chi2_Scan")),
      *Scan_nofit = static_cast<TH1 *>(dchi2map->Clone("Chi2_Scan_nofit"));

  std::vector<double> const &scan_points = (!fit_nuisance) ? gof_scan : gof_fit;

  size_t step = 0;
  for (int stepx = 0; stepx < dchi2map->GetXaxis()->GetNbins(); ++stepx) {
    double xval = dchi2map->GetXaxis()->GetBinCenter(stepx + 1);

    for (int stepy = 0;
         stepy < ((nparams == 2) ? dchi2map->GetYaxis()->GetNbins() : 1);
         ++stepy) {
      if (nparams == 2) {
        double yval = dchi2map->GetYaxis()->GetBinCenter(stepy + 1);
        // ----------- Bottom UF Left UF        rows
        int gbin = dchi2map->GetBin(stepx + 1, stepy + 1);

        static_cast<TH2 *>(Scan)->SetBinContent(gbin, scan_points[step]);
        static_cast<TH2 *>(Scan_d)->SetBinContent(gbin,
                                                  scan_points[step] - min_gof);
        if (fit_nuisance) {
          static_cast<TH2 *>(Scan_nofit)->SetBinContent(gbin, gof_scan[step]);
        }
      } else {
        Scan->SetBinContent(stepx + 1, scan_points[step]);
        Scan_d->SetBinContent(stepx + 1, scan_points[step] - min_gof);
        if (fit_nuisance) {
          Scan_nofit->SetBinContent(stepx + 1, gof_scan[step]);
        }
      }
      step++;
    }
  }

  size_t ch_it = 0;
  for (auto const ch : Channels) {

    dir->WriteTObject(DataHist[ch_it], (ch.first + "_data").c_str());

    if (poisson_throw) {
      dir->WriteTObject(InputDataHist[ch_it],
                        (ch.first + "_data_nothrow").c_str());
    }

    dir->WriteTObject(MinHist[ch_it], (ch.first + "_minchi2").c_str());
    dir->WriteTObject(NomHist[ch_it], (ch.first + "_true").c_str());

    ch_it++;
  }

  if (fit_nuisance) {
    dir->WriteTObject(Scan_nofit, "Chi2_Scan_nofit");
  }

  dir->WriteTObject(Scan, "Chi2_Scan");
  dir->WriteTObject(Scan_d, "Delta_Chi2_Scan");

  if (nparams == 2) {
    TGraph Scan_68 = GetContour(static_cast<TH2 *>(Scan_d), 2.3);
    dir->WriteTObject(&Scan_68, "Scan_d_68");
    TGraph Scan_90 = GetContour(static_cast<TH2 *>(Scan_d), 4.61);
    dir->WriteTObject(&Scan_90, "Scan_d_90");

    std::cout << xparam_name << ": " << GetCalcValue(true_calc, xparam_name)
              << ", " << yparam_name << ": "
              << GetCalcValue(true_calc, yparam_name) << std::endl;
    TMarker true_marker(GetCalcValue(true_calc, xparam_name),
                        GetCalcValue(true_calc, yparam_name), 20);
    true_marker.SetMarkerColor(kBlack);
    true_marker.SetMarkerSize(1);

    dir->WriteTObject(&true_marker, "TruePosition");

    TMarker bf_marker(min_paramvals.first, min_paramvals.second, 20);
    bf_marker.SetMarkerColor(kBlack);
    bf_marker.SetMarkerSize(1);

    dir->WriteTObject(&bf_marker, "best_fit_marker");
  } else {
    double scan_min = Scan->GetMinimum();
    double scan_max = Scan->GetMaximum();
    TArrow true_arrow(GetCalcValue(true_calc, xparam_name), scan_max * 0.2,
                      GetCalcValue(true_calc, xparam_name), scan_min, 0.025,
                      "|>");
    dir->WriteTObject(&true_arrow, "true_arrow");

    TArrow best_fit_arrow(min_paramvals.first, scan_max * 0.2,
                          min_paramvals.first, scan_min, 0.025, "|>");
    dir->WriteTObject(&best_fit_arrow, "best_fit_arrow");
  }

  ch_it = 0;
  for (auto const ch : Channels) {
    DataHist[ch_it]->SetDirectory(nullptr);
    InputDataHist[ch_it]->SetDirectory(nullptr);
    MinHist[ch_it]->SetDirectory(nullptr);
    NomHist[ch_it]->SetDirectory(nullptr);
    ch_it++;
  }
  Scan->SetDirectory(nullptr);
  Scan_d->SetDirectory(nullptr);
  Scan_nofit->SetDirectory(nullptr);

  if (run_plan_nu.GetPlanPOT() > 0) {
    TH1D *run_plan_nu_h = run_plan_nu.AsTH1();
    dir->WriteTObject(run_plan_nu_h, "run_plan_nu");
    run_plan_nu_h->SetDirectory(nullptr);
  }

  if (run_plan_nub.GetPlanPOT() > 0) {
    TH1D *run_plan_nub_h = run_plan_nub.AsTH1();
    dir->WriteTObject(run_plan_nub_h, "run_plan_nub");
    run_plan_nub_h->SetDirectory(nullptr);
  }

  f.Write();
}

int main(int argc, char const *argv[]) {
  gROOT->SetMustClean(false);

  if (argc != 2) {
    std::cout << "[ERROR]: Expected to be passed the location of a single "
                 "configuration FHiCL file. Run "
              << argv[0] << " --help for an example." << std::endl;
    return 1;
  }

  if ((std::string(argv[1]) == "--help") || (std::string(argv[1]) == "-?")) {

    std::cout << "[USAGE]: Runlike " << argv[0] << " <fhicl_config_file> "
              << std::endl;
    std::cout << "Example fhicl that can be customized: <MUST BE FILLED IN>"
              << std::endl;

    std::cout
        << "OscPoints: [ \n"
           "{\n"
           "    ssth23: 0.45\n"
           "    dmsq32: 2.525e-3\n"
           "},\n"
           "# ... Other true oscillation hypotheses/asimov sets\n"
           "]\n"
           "\n"
           "PRISMFitChannels: {\n"
           "    Numu_disp: {\n"
           "        ND: numu_numode\n"
           "        FD: numu_numode\n"
           "    }\n"
           "    Numubar_disp: {\n"
           "        ND: numubar_nubarmode\n"
           "        FD: numubar_nubarmode\n"
           "    }\n"
           "    Nue_app: {\n"
           "        ND: numu_numode\n"
           "        FD: nue_numode\n"
           "    }\n"
           "    Nuebar_app: {\n"
           "        ND: numubar_nubarmode\n"
           "        FD: nuebar_nubarmode\n"
           "    }\n"
           "    DispJoint:[\n"
           "        @local::PRISMFitChannels.Numu_disp,\n"
           "        @local::PRISMFitChannels.Numubar_disp,\n"
           "    ]\n"
           "    AppJoint:[\n"
           "        @local::PRISMFitChannels.Nue_app,\n"
           "        @local::PRISMFitChannels.Nuebar_app,\n"
           "    ]\n"
           "    NuModeJoint:[\n"
           "        @local::PRISMFitChannels.Numu_disp,\n"
           "        @local::PRISMFitChannels.Nue_app,\n"
           "    ]\n"
           "    NuBarModeJoint:[\n"
           "        @local::PRISMFitChannels.Numubar_disp,\n"
           "        @local::PRISMFitChannels.Nuebar_app,\n"
           "    ]\n"
           "    FourFlavor: [\n"
           "        @sequence::PRISMFitChannels.NuModeJoint,\n"
           "        @sequence::PRISMFitChannels.NuBarModeJoint,\n"
           "    ]\n"
           "}\n"
           "scans:[ \n"
           "  {\n"
           "    state_file: <path to state file>\n"
           "    output_file: [PRISMScans.root, UPDATE]\n"
           "    output_dir: ssth23_protonFD_EProxy\n"
           "    projection_name: EProxy\n"
           "\n"
           "    true_osc: @local::OscPoints[0]\n"
           "    POT_years: 3.5\n"
           "\n"
           "    GOF: {\n"
           "      use_PRISM: true # Whether to use PRISM or standard FD-only "
           "fit\n"
           "      use_ND_stats: true # Whether to include ND stats errors in "
           "the GOF\n"
           "      fit_nuisance: false # Whether to fit NUISANCE parameters\n"
           "      poisson_throw: true # Whether to poisson fluctuate the "
           "data.\n"
           "\n"
           "      # Parameters used in the scan get fixed automatically\n"
           "      # Other possible: th13 dmsq21 ssth12 rho\n"
           "      free_osc_params: [dmsq32, ssth23, deltapi] \n"
           "      free_syst_params: [MACCQE, MACCRES]"
           "\n"
           "      energy_limits: {\n"
           "        NumuDisp: [0.5, 8]\n"
           "        NueApp: [0.5, 8]\n"
           "        NumuBarDisp: [0.5, 8]\n"
           "        NueBarApp: [0.5, 8]\n"
           "      }\n"
           "    }\n"
           "\n"
           "    PRISM: {\n"
           "      is_fake_spec_run: true # If the PRISM state includes a "
           "special horn run\n"
           "      reg_factor: 2E-15 # Regularization factor\n"
           "      energy_range: [0.5, 6] # Energy range to include in inear "
           "combination determination\n"
           "    }\n"
           "\n"
           "    scan_params: [\n"
           "      {\n"
           "          name: ssth23\n"
           "          # [nsteps, min, max ]\n"
           "          scan_steps:  [75, 0.4, 0.7]\n"
           "      },\n"
           "      # Can have one or two parameters in this list.\n"
           "      #{\n"
           "      #    name: dmsq32\n"
           "      #    scan_steps:  [20, 2E-3, 3E-3]\n"
           "      #}\n"
           "    ]\n"
           "\n"
           "    samples: @local::PRISMFitChannels.Numu_disp\n"
           "  },\n"
           "]\n"
           "# This makes the first defined scan overwrite the previous output "
           "file.\n"
           "scans[0].output_file[1]: RECREATE\n"
        << std::endl;

    return 0;
  }

  fhicl::ParameterSet const &ps = fhicl::make_ParameterSet(argv[1]);

  for (fhicl::ParameterSet const &scan :
       ps.get<std::vector<fhicl::ParameterSet>>("scans")) {
    PRISMScan(scan);
  }
}
