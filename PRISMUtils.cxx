#include "CAFAna/PRISM/PRISMUtils.h"

#include "CAFAna/Analysis/CalcsNuFit.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/Cuts/TruthCuts.h"

namespace ana {

FVMassCorrection::FVMassCorrection() {
  double det_min_cm = -400;
  double det_max_cm = 400;
  double step_cm = 25;
  size_t NDetSteps = (det_max_cm - det_min_cm) / step_cm;

  fvmasscor = new TH1D("fvmasscor", "", NDetSteps, det_min_cm, det_max_cm);
  fvmasscor->SetDirectory(nullptr);
  double avg_step = 1E-4;
  size_t const navg_steps = step_cm / avg_step;

  fOverflow = fvmasscor->GetXaxis()->GetNbins() + 1;
  for (int bi_it = 0; bi_it < fvmasscor->GetXaxis()->GetNbins(); ++bi_it) {
    double bin_low_edge = fvmasscor->GetXaxis()->GetBinLowEdge(bi_it + 1);
    for (size_t avg_step_it = 0; avg_step_it < navg_steps; ++avg_step_it) {
      double pos_cm = bin_low_edge + (double(avg_step_it) * avg_step);
      if (!ana::IsInNDFV(pos_cm, 0, 150)) {
        continue;
      }
      fvmasscor->Fill(pos_cm, 1.0 / double(navg_steps));
    }

    if (fvmasscor->GetBinContent(bi_it + 1) > 0) {
      fvmasscor->SetBinContent(bi_it + 1,
                               1.0 / fvmasscor->GetBinContent(bi_it + 1));
    }
  }
}
double FVMassCorrection::GetWeight(double vtx_x_cm) {
  size_t bi_it = fvmasscor->FindFixBin(vtx_x_cm);
  if ((bi_it == 0) || (bi_it == fOverflow)) {
    return 0;
  }
  return fvmasscor->GetBinContent(bi_it);
}

const ana::Var kMassCorrection({}, [](const caf::StandardRecord *sr) -> double {
  return sr->dune.NDMassCorrWeight;
});

template <class T>
void LoadFrom(TFile &f, std::string const &dirname, std::unique_ptr<T> &out) {
  TDirectory *dir = f.GetDirectory(dirname.c_str());
  if (!dir) {
    std::cout << "[ERROR]: Failed to read directory: " << dirname << std::endl;
    abort();
  }
  out = T::LoadFrom(dir);
}

PRISMStateBlob LoadPRISMState(TFile &f, std::string const &varname,
                              bool IsRHC) {
  PRISMStateBlob blob;
  LoadFrom<PredictionPRISM>(
      f, std::string("PRISM_") + varname + std::string(IsRHC ? "_rhc" : "_fhc"),
      blob.PRISM);

  LoadFrom<PredictionInterp>(f,
                             std::string("NDMatchInterp_ETrue") +
                                 std::string(IsRHC ? "_rhc" : "_fhc"),
                             blob.NDMatchInterp);

  LoadFrom<PredictionInterp>(f,
                             std::string("FDMatchInterp_ETrue") +
                                 std::string(IsRHC ? "_rhc" : "_fhc"),
                             blob.FDMatchInterp);

  LoadFrom<PredictionInterp>(f,
                             std::string("FarDet_") + varname +
                                 std::string(IsRHC ? "_rhc" : "_fhc"),
                             blob.FarDet);

  LoadFrom<OscillatableSpectrum>(f,
                                 std::string("FarDetData_") + varname +
                                     std::string(IsRHC ? "_rhc" : "_fhc"),
                                 blob.FarDetData);

  return blob;
}

osc::IOscCalculatorAdjustable *
ConfigureCalc(fhicl::ParameterSet const &ps,
              osc::IOscCalculatorAdjustable *calc) {

  static std::set<std::string> keys{
      "L",    "Rho",       "Dmsq21",    "Dmsq32",    "Th12", "Th13",
      "Th23", "SinsqTh12", "SinsqTh13", "SinsqTh23", "dCP",  "dCP_pi"};

  for (std::string const &k : ps.get_names()) {
    if (!keys.count(k)) {
      std::cout << "[ERROR]: Unexpected key in oscillation configuration: " << k
                << std::endl;
      abort();
    }
  }

  if (!calc) {
    calc = NuFitOscCalc(1);
  }

  if (ps.has_key("L")) {
    calc->SetL(ps.get<double>("L"));
  }
  if (ps.has_key("Rho")) {
    calc->SetRho(ps.get<double>("Rho"));
  }
  if (ps.has_key("Dmsq21")) {
    calc->SetDmsq21(ps.get<double>("Dmsq21"));
  }
  if (ps.has_key("Dmsq32")) {
    calc->SetDmsq32(ps.get<double>("Dmsq32"));
  }
  if (ps.has_key("Th12")) {
    calc->SetTh12(ps.get<double>("Th12"));
  }
  if (ps.has_key("Th13")) {
    calc->SetTh13(ps.get<double>("Th13"));
  }
  if (ps.has_key("Th23")) {
    calc->SetTh23(ps.get<double>("Th23"));
  }
  if (ps.has_key("SinsqTh12")) {
    calc->SetTh12(asin(sqrt(ps.get<double>("SinsqTh12"))));
  }
  if (ps.has_key("SinsqTh13")) {
    calc->SetTh13(asin(sqrt(ps.get<double>("SinsqTh13"))));
  }
  if (ps.has_key("SinsqTh23")) {
    calc->SetTh23(asin(sqrt(ps.get<double>("SinsqTh23"))));
  }
  if (ps.has_key("dCP_pi")) {
    calc->SetdCP(ps.get<double>("dCP_pi") * M_PI);
  }
  if (ps.has_key("dCP")) {
    calc->SetdCP(ps.get<double>("dCP"));
  }

  return calc;
}

SystShifts GetSystShifts(fhicl::ParameterSet const &ps) {

  std::vector<ISyst const *> los = GetListOfSysts();

  SystShifts shift;

  for (std::pair<std::string, double> const &s :
       ps.get<std::vector<std::pair<std::string, double>>>("shifts", {})) {
    std::vector<ISyst const *> reduced_los = los;
    KeepSysts(reduced_los, {
                               s.first,
                           });
    if (!reduced_los.size()) {
      std::cout << "[ERROR]: Unknown systematic " << s.first << std::endl;
      abort();
    }
    shift.SetShift(reduced_los[0], s.second);
    std::cout << "Set " << s.first << " to " << s.second << std::endl;
  }
  return shift;
}

} // namespace ana
