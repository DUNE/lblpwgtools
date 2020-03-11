#include "CAFAna/Analysis/AnalysisVars.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/PRISM/PRISMExtrapolator.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PredictionPRISM.h"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"

using namespace ana;

std::map<std::string, PRISMStateBlob> States;

void PRISMScan(fhicl::ParameterSet const &scan) {

  std::string const &data_state_file = scan.get<std::string>("data_state_file");
  std::string const &mc_state_file = scan.get<std::string>("mc_state_file");

  std::vector<std::string> const &output_file =
      scan.get<std::vector<std::string>>("output_file");
  std::string const &output_dir = scan.get<std::string>("output_dir", "");

  std::string const &varname =
      scan.get<std::string>("projection_name", "EProxy");
  bool isfhc = scan.get<bool>("isFHC", true);

  double reg = scan.get<double>("reg_factor", 1E-16);
  std::array<double, 2> fit_range =
      scan.get<std::array<double, 2>>("fit_range", {0, 4});

  std::string param = scan.get<std::string>("param_to_scan");
  std::array<double, 3> scan_steps =
      scan.get<std::array<double, 3>>("scan_steps");

  (void)GetListOfSysts();

  osc::IOscCalculatorAdjustable *calc =
      ConfigureCalc(scan.get<fhicl::ParameterSet>("Osc", {}));

  if (!States.count(data_state_file)) {
    TFile fs(data_state_file.c_str());
    std::cout << "Loading " << varname << " state from " << data_state_file
              << std::endl;
    States[data_state_file] = LoadPRISMState(fs, varname, !isfhc);
    std::cout << "Done!" << std::endl;
    fs.Close();
  }

  if (!States.count(mc_state_file)) {
    TFile fs(mc_state_file.c_str());
    std::cout << "Loading " << varname << " state from " << mc_state_file
              << std::endl;
    States[mc_state_file] = LoadPRISMState(fs, varname, !isfhc);
    std::cout << "Done!" << std::endl;
    fs.Close();
  }

  PRISMStateBlob &data_state = States[data_state_file];
  PRISMStateBlob &mc_state = States[mc_state_file];

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
  fluxmatcher.SetTargetConditioning(reg, {}, fit_range[0], fit_range[1]);
  state.PRISM->SetFluxMatcher(&fluxmatcher);

  std::vector<double> points;

  size_t nsteps = scan_steps[0];
  double step_width = (scan_steps[2] - scan_steps[1]) / scan_steps[0];

  TH1 *PRISMPredEvRateMatch_h_min = nullptr;;
  TH1 *FarDet_h_min = nullptr;;

  double min = std::numeric_limits<double>::max();
  for (size_t step = 0; step < nsteps; ++step) {
    double pval = scan_steps[1] + step * step_width;
    fhicl::ParameterSet step_ps;
    step_ps.put(param, pval);

    (void)ConfigureCalc(step_ps, calc);

    Spectrum PRISMPredEvRateMatchSpec = state.PRISM->Predict(calc);
    TH1 *PRISMPredEvRateMatch_h = PRISMPredEvRateMatchSpec.ToTHX(pot_fd);
    TH1 *FarDet_h = state.FarDet->Predict(calc).ToTHX(pot_fd);

    double chi2 = 0;
    for (int bi = 0; bi < FarDet_h->GetXaxis()->GetNbins(); ++bi) {
      double pbc = PRISMPredEvRateMatch_h->GetBinContent(bi + 1);
      double pbe = PRISMPredEvRateMatch_h->GetBinError(bi + 1);

      double fbc = FarDet_h->GetBinContent(bi + 1);
      double fbe = sqrt(fbc);

      chi2 += pow((pbc - fbc), 2) / (pow(fbe, 2) + pow(pbe, 2));
    }
    if (chi2 < min) {
      PRISMPredEvRateMatch_h_min = PRISMPredEvRateMatch_h;
      FarDet_h_min = FarDet_h;
    }
    min = std::min(min, chi2);
    points.push_back(chi2);
  }
  TGraph Scan, Scan_d;

  for (size_t step = 0; step < nsteps; ++step) {
    double pval = scan_steps[1] + step * step_width;
    Scan.SetPoint(step, pval, points[step]);
    Scan_d.SetPoint(step, pval, points[step] - min);
  }

  dir->WriteTObject(PRISMPredEvRateMatch_h_min, "PRISMPredEvRateMatch_minchi2");
  dir->WriteTObject(FarDet_h_min, "FarDet_minchi2");

  dir->WriteTObject(&Scan, "Chi2_Scan");
  dir->WriteTObject(&Scan_d, "Delta_Chi2_Scan");

  if (scan.has_key("on_axis_scan")) {
    fhicl::ParameterSet on_axis_scan =
        scan.get<fhicl::ParameterSet>("on_axis_scan");
    std::string const &state_file = on_axis_scan.get<std::string>("state_file");
    std::string const &varname =
        on_axis_scan.get<std::string>("projection_name", "EProxy");
    bool isfhc = on_axis_scan.get<bool>("isFHC", true);

    if (!States.count(state_file)) {
      TFile fs(state_file.c_str());
      std::cout << "Loading " << varname << " state from " << state_file
                << std::endl;
      States[state_file] = LoadPRISMState(fs, varname, !isfhc);
      std::cout << "Done!" << std::endl;
      fs.Close();
    }

    PRISMStateBlob &on_axis_state = States[state_file];

    std::vector<double> points;

    size_t nsteps = scan_steps[0];
    double step_width = (scan_steps[2] - scan_steps[1]) / scan_steps[0];

    double min = std::numeric_limits<double>::max();
    TH1 *FarDet_on_axis_h_min = nullptr;
    TH1 *FarDet_h_min = nullptr;
    for (size_t step = 0; step < nsteps; ++step) {
      double pval = scan_steps[1] + step * step_width;
      fhicl::ParameterSet step_ps;
      step_ps.put(param, pval);

      (void)ConfigureCalc(step_ps, calc);

      TH1 *FarDet_on_axis_h = on_axis_state.FarDet->Predict(calc).ToTHX(pot_fd);
      TH1 *FarDet_h = state.FarDet->Predict(calc).ToTHX(pot_fd);

      double chi2 = 0;
      for (int bi = 0; bi < FarDet_h->GetXaxis()->GetNbins(); ++bi) {
        double oabc = FarDet_on_axis_h->GetBinContent(bi + 1);
        double oabe = sqrt(oabc);

        double fbc = FarDet_h->GetBinContent(bi + 1);
        double fbe = sqrt(fbc);

        std::cout << oabc << ", " << oabe << ", " << fbc << ", " << fbe
                  << std::endl;
        assert(std::isnormal(oabc));
        assert(std::isnormal(oabe));
        assert(std::isnormal(fbc));
        assert(std::isnormal(fbe));

        chi2 += pow((oabc - fbc), 2) / (pow(fbe, 2) + pow(oabe, 2));
      }
      if (chi2 < min) {
        FarDet_on_axis_h_min = FarDet_on_axis_h;
        FarDet_h_min = FarDet_h;
      }
      min = std::min(min, chi2);
      points.push_back(chi2);
    }
    TGraph Scan, Scan_d;

    for (size_t step = 0; step < nsteps; ++step) {
      double pval = scan_steps[1] + step * step_width;
      Scan.SetPoint(step, pval, points[step]);
      Scan_d.SetPoint(step, pval, points[step] - min);
    }

    dir->WriteTObject(FarDet_on_axis_h_min, "FD_FarDet_on_axis_minchi2");
    dir->WriteTObject(FarDet_h_min, "FD_FarDet_minchi2");

    dir->WriteTObject(&Scan, "FD_Chi2_Scan");
    dir->WriteTObject(&Scan_d, "FD_Delta_Chi2_Scan");
  }

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
