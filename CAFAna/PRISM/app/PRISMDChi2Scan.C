#include "CAFAna/Analysis/AnalysisVars.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/Core/HistCache.h"

#include "CAFAna/PRISM/PRISMExtrapolator.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PredictionPRISM.h"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"

#include "TCanvas.h"
#include "TGraph2D.h"

using namespace ana;

std::map<std::string, PRISMStateBlob> States;

namespace ana {
class SimpleChi2Experiment : public IChiSqExperiment {
public:
  SimpleChi2Experiment(IPrediction const *Pred, Spectrum const &Data,
                       bool UseHistError = false, double POT = 0)
      : fPred(Pred), fUseHistError(UseHistError) {
    fPOT = POT;
    if (fPOT == 0) {
      fPOT = Data.POT();
    }
    fData = Data.ToTH1(fPOT);
  }

  IPrediction const *fPred;
  TH1 const *fData;
  double fPOT;
  bool fUseHistError;

  double ChiSq(osc::IOscCalculatorAdjustable *osc,
               const SystShifts &syst = SystShifts::Nominal()) const {

    TH1D *PredHist = fPred->Predict(osc).ToTH1(fPOT);
    double chi2 = 0;
    for (int bi = 0; bi < fData->GetXaxis()->GetNbins(); ++bi) {
      double pbc = PredHist->GetBinContent(bi + 1);
      double pbe =
          (fUseHistError ? pow(PredHist->GetBinError(bi + 1), 2) : 0) + pbc;

      // if (bi == 5) {
      //   std::cout << (fUseHistError ? "PRISM" : "Standard") << " bin " << bi
      //             << " content = " << pbc << " +/- " << sqrt(pbc) << " ("
      //             << sqrt(pbc) / pbc << ", POT = " << fPOT << ")" << std::endl;
      // }
      double dbc = fData->GetBinContent(bi + 1);

      chi2 += pow((pbc - dbc), 2) / pbe;
    }
    HistCache::Delete(PredHist);

    return chi2;
  }
};
} // namespace ana

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

void PRISMScan(fhicl::ParameterSet const &scan) {

  std::string const &state_file = scan.get<std::string>("state_file");

  std::vector<std::string> const &output_file =
      scan.get<std::vector<std::string>>("output_file");
  std::string const &output_dir = scan.get<std::string>("output_dir", "");

  std::string const &varname =
      scan.get<std::string>("projection_name", "EProxy");
  bool isfhc = scan.get<bool>("isFHC", true);

  double reg = scan.get<double>("reg_factor", 1E-16);
  std::array<double, 2> fit_range =
      scan.get<std::array<double, 2>>("fit_range", {0, 4});

  bool FitNuisance = scan.get<bool>("fit_nuisance", false);

  auto oscpars = GetOscVars(scan.get<std::vector<std::string>>("osc_pars", {}));

  auto params = scan.get<std::vector<fhicl::ParameterSet>>("params");
  size_t nxsteps = 1, nysteps = 1;
  double step_widthx = 0, step_widthy = 0;
  double startx = 0, starty = 0;
  std::string xname = "", yname = "";
  size_t nparams = params.size();

  TH1 *dchi2map;
  if (params.size() == 1) {
    std::array<double, 3> xscan_steps =
        params.front().get<std::array<double, 3>>("scan_steps");
    nxsteps = xscan_steps[0];
    step_widthx = (xscan_steps[2] - xscan_steps[1]) / xscan_steps[0];

    xname = params.front().get<std::string>("name");
    startx = xscan_steps[1];

    std::stringstream ttl("");
    ttl << ";" << xname << ";#Delta#Chi^{2}";

    dchi2map = new TH1D("dchi2", ttl.str().c_str(), xscan_steps[0],
                        xscan_steps[1], xscan_steps[2]);
  } else if (params.size() == 2) {
    std::array<double, 3> xscan_steps =
        params.front().get<std::array<double, 3>>("scan_steps");
    nxsteps = xscan_steps[0];
    step_widthx = (xscan_steps[2] - xscan_steps[1]) / xscan_steps[0];

    xname = params.front().get<std::string>("name");
    startx = xscan_steps[1];
    std::array<double, 3> yscan_steps =
        params.back().get<std::array<double, 3>>("scan_steps");
    nysteps = yscan_steps[0];
    step_widthy = (yscan_steps[2] - yscan_steps[1]) / yscan_steps[0];

    yname = params.back().get<std::string>("name");
    starty = yscan_steps[1];
    std::stringstream ttl("");
    ttl << ";" << xname << ";" << yname << ";#Delta#Chi^{2}";

    dchi2map = new TH2D("dchi2", ttl.str().c_str(), xscan_steps[0],
                        xscan_steps[1], xscan_steps[2], yscan_steps[0],
                        yscan_steps[1], yscan_steps[2]);
  } else {
    std::cout << "Can only do 1 or 2 dimensional chi2 scans, but recieved: "
              << params.size() << " parameters." << std::endl;
    abort();
  }
  dchi2map->SetDirectory(0);

  (void)GetListOfSysts();

  osc::IOscCalculatorAdjustable *calc =
      ConfigureCalc(scan.get<fhicl::ParameterSet>("Osc", {}));

  if (!States.count(state_file)) {
    TFile fs(state_file.c_str());
    std::cout << "Loading " << varname << " state from " << state_file
              << std::endl;
    States[state_file] = LoadPRISMState(fs, varname, !isfhc);
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

  int id = 0;
  PRISMExtrapolator fluxmatcher;

  fluxmatcher.InitializeEventRateMatcher(state.NDMatchInterp.get(),
                                         state.FDMatchInterp.get());
  if (scan.get<bool>("is_fake_spec_run", false)) {
    fluxmatcher.SetTargetConditioning(reg,
                                      {
                                          {0},
                                      },
                                      fit_range[0], fit_range[1]);
  } else {
    fluxmatcher.SetTargetConditioning(reg, {}, fit_range[0], fit_range[1]);
  }
  state.PRISM->SetFluxMatcher(&fluxmatcher);

  std::vector<double> points_PRISM, points_FarDet;

  TH1D *PRISMPredEvRateMatch_h_min = nullptr;
  TH1D *FarDet_h_min = nullptr;
  int disp_pid = isfhc ? 14 : -14;

  // default to 1 year
  double POT = scan.get<double>("POT_years", 1) * (pot_fd / 3.5);

  // std::cout << "POT = " << POT
  //           << ", (read: " << scan.get<double>("POT_years", 1) << ")"
  //           << std::endl;

  // state.FarDetData->ScaleToPOT(POT);
  // std::cout << "data POT: " << state.FarDetData->POT() << ", Spectrum POT: "
  //           << state.FarDetData->Oscillated(calc, disp_pid, disp_pid).POT()
  //           << std::endl;

  TH1D *FarDetData_h =
      state.FarDetData->Oscillated(calc, disp_pid, disp_pid).ToTH1(POT);

  SimpleChi2Experiment PRISMExpt(
      state.PRISM.get(), state.FarDetData->Oscillated(calc, disp_pid, disp_pid),
      true, POT);
  SimpleChi2Experiment FarDetExpt(
      state.FarDet.get(),
      state.FarDetData->Oscillated(calc, disp_pid, disp_pid), false, POT);
  TH1D *PRISMPredEvRateMatchNom_h = state.PRISM->Predict(calc).ToTH1(POT);
  TH1D *FarDetNom_h = state.FarDet->Predict(calc).ToTH1(POT);

  double min_PRISM = std::numeric_limits<double>::max();
  double min_FarDet = std::numeric_limits<double>::max();
  double min_all = std::numeric_limits<double>::max();
  for (size_t stepx = 0; stepx < nxsteps; ++stepx) {
    double xval = startx + stepx * step_widthx;
    std::cout << "\r(" << stepx << "/" << nxsteps << ") " << xname << ": "
              << xval << std::flush;

    for (size_t stepy = 0; stepy < nysteps; ++stepy) {

      fhicl::ParameterSet step_ps;
      step_ps.put(xname, xval);

      if (nparams == 2) {
        double yval = starty + stepy * step_widthy;
        step_ps.put(yname, yval);
      }

      (void)ConfigureCalc(step_ps, calc);

      double chi2_PRISM = PRISMExpt.ChiSq(calc);
      double chi2_FarDet = FarDetExpt.ChiSq(calc);

      if (chi2_PRISM < min_PRISM) {
        HistCache::Delete(PRISMPredEvRateMatch_h_min);
        PRISMPredEvRateMatch_h_min = state.PRISM->Predict(calc).ToTH1(POT);
        min_PRISM = chi2_PRISM;
      }
      if (chi2_FarDet < min_FarDet) {
        HistCache::Delete(FarDet_h_min);
        FarDet_h_min = state.FarDet->Predict(calc).ToTH1(POT);
        min_FarDet = chi2_FarDet;
      }
      min_all = std::min(min_all, std::min(min_PRISM, min_FarDet));
      points_PRISM.push_back(chi2_PRISM);
      points_FarDet.push_back(chi2_FarDet);
    }
  }
  std::cout << "\r" << std::endl;
  TH1 *Scan_PRISM = static_cast<TH1 *>(dchi2map->Clone("PRISM_Chi2_Scan")),
      *Scan_d_PRISM =
          static_cast<TH1 *>(dchi2map->Clone("PRISM_Delta_Chi2_Scan"));
  TH1 *Scan_FarDet = static_cast<TH1 *>(dchi2map->Clone("FarDet_Chi2_Scan")),
      *Scan_d_FarDet =
          static_cast<TH1 *>(dchi2map->Clone("FarDet_Delta_Chi2_Scan"));

  for (size_t stepx = 0; stepx < nxsteps; ++stepx) {
    double xval = startx + stepx * step_widthx;

    for (size_t stepy = 0; stepy < nysteps; ++stepy) {
      if (nparams == 2) {
        double yval = starty + stepy * step_widthy;
        // ----------- Bottom UF Left UF        rows
        size_t hstep = (nxsteps + 2) + 1 + (stepy * (nxsteps + 2)) + stepx;
        size_t step = stepx * nysteps + stepy;

        static_cast<TH2 *>(Scan_PRISM)
            ->SetBinContent(hstep, points_PRISM[step]);
        static_cast<TH2 *>(Scan_d_PRISM)
            ->SetBinContent(hstep, points_PRISM[step] - min_all);

        static_cast<TH2 *>(Scan_FarDet)
            ->SetBinContent(hstep, points_FarDet[step]);
        static_cast<TH2 *>(Scan_d_FarDet)
            ->SetBinContent(hstep, points_FarDet[step] - min_all);

      } else {
        Scan_PRISM->SetBinContent(stepx + 1, points_PRISM[stepx]);
        Scan_d_PRISM->SetBinContent(stepx + 1, points_PRISM[stepx] - min_all);

        Scan_FarDet->SetBinContent(stepx + 1, points_FarDet[stepx]);
        Scan_d_FarDet->SetBinContent(stepx + 1, points_FarDet[stepx] - min_all);
      }
    }
  }

  dir->WriteTObject(PRISMPredEvRateMatch_h_min, "PRISMPredEvRateMatch_minchi2");
  dir->WriteTObject(FarDet_h_min, "FarDet_minchi2");

  dir->WriteTObject(Scan_PRISM, "PRISM_Chi2_Scan");
  dir->WriteTObject(Scan_d_PRISM, "PRISM_Delta_Chi2_Scan");

  Scan_PRISM->Scale(1.0 / double(FarDetData_h->GetXaxis()->GetNbins()));
  Scan_d_PRISM->Scale(1.0 / double(FarDetData_h->GetXaxis()->GetNbins()));

  Scan_PRISM->GetZaxis()->SetTitle("#Delta#Chi^{2}/NBins");
  Scan_d_PRISM->GetZaxis()->SetTitle("#Delta#Chi^{2}/NBins");

  dir->WriteTObject(Scan_PRISM, "PRISM_redChi2_Scan");
  dir->WriteTObject(Scan_d_PRISM, "PRISM_Delta_redChi2_Scan");

  if (nparams == 2) {
    TGraph Scan_PRISM_68 = GetContour(static_cast<TH2 *>(Scan_d_PRISM), 2.3);
    dir->WriteTObject(&Scan_PRISM_68, "Scan_d_PRISM_68");
    TGraph Scan_PRISM_90 = GetContour(static_cast<TH2 *>(Scan_d_PRISM), 4.61);
    dir->WriteTObject(&Scan_PRISM_90, "Scan_d_PRISM_90");
  }

  dir->WriteTObject(Scan_FarDet, "FarDet_Chi2_Scan");
  dir->WriteTObject(Scan_d_FarDet, "FarDet_Delta_Chi2_Scan");

  Scan_FarDet->Scale(1.0 / double(FarDetData_h->GetXaxis()->GetNbins()));
  Scan_d_FarDet->Scale(1.0 / double(FarDetData_h->GetXaxis()->GetNbins()));

  Scan_FarDet->GetZaxis()->SetTitle("#Delta#Chi^{2}/NBins");
  Scan_d_FarDet->GetZaxis()->SetTitle("#Delta#Chi^{2}/NBins");
  dir->WriteTObject(Scan_FarDet, "FarDet_redChi2_Scan");
  dir->WriteTObject(Scan_d_FarDet, "FarDet_Delta_redChi2_Scan");

  if (nparams == 2) {
    TGraph Scan_FarDet_68 = GetContour(static_cast<TH2 *>(Scan_d_FarDet), 2.3);
    dir->WriteTObject(&Scan_FarDet_68, "Scan_d_FarDet_68");
    TGraph Scan_FarDet_90 = GetContour(static_cast<TH2 *>(Scan_d_FarDet), 4.61);
    dir->WriteTObject(&Scan_FarDet_90, "Scan_d_FarDet_90");
  }

  dir->WriteTObject(PRISMPredEvRateMatchNom_h, "PRISMPredEvRateMatchTrueOsc");
  dir->WriteTObject(FarDetNom_h, "FarDetTrueOsc");
  dir->WriteTObject(FarDetData_h, "FarDetData");

  PRISMPredEvRateMatch_h_min->SetDirectory(0);
  FarDet_h_min->SetDirectory(0);
  Scan_PRISM->SetDirectory(0);
  Scan_d_PRISM->SetDirectory(0);
  Scan_FarDet->SetDirectory(0);
  Scan_d_FarDet->SetDirectory(0);
  PRISMPredEvRateMatchNom_h->SetDirectory(0);
  FarDetNom_h->SetDirectory(0);
  FarDetData_h->SetDirectory(0);

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

  for (fhicl::ParameterSet const &scan :
       ps.get<std::vector<fhicl::ParameterSet>>("scans")) {
    PRISMScan(scan);
  }
}
