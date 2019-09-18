#include "CAFAna/PRISM/PRISMUtils.h"

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

const ana::Var kMassCorrection({},
                               [&](const caf::StandardRecord *sr) -> double {
                                 return sr->dune.NDMassCorrWeight;
                               });

PRISMStateBlob LoadPRISMState(TFile &f, std::string const &varname,
                              bool IsRHC) {
  PRISMStateBlob blob;
  blob.PRISM = PredictionPRISM::LoadFrom(
      f.GetDirectory((std::string("PRISM_") + varname +
                      std::string(IsRHC ? "_rhc" : "_fhc"))
                         .c_str()));

  blob.NDMatchInterp = PredictionInterp::LoadFrom(
      f.GetDirectory((std::string("NDMatchInterp_ETrue") +
                      std::string(IsRHC ? "_rhc" : "_fhc"))
                         .c_str()));
  blob.FDMatchInterp = PredictionInterp::LoadFrom(
      f.GetDirectory((std::string("FDMatchInterp_ETrue") +
                      std::string(IsRHC ? "_rhc" : "_fhc"))
                         .c_str()));

  blob.FarDet = PredictionNoExtrap::LoadFrom(
      f.GetDirectory((std::string("FarDet_") + varname +
                      std::string(IsRHC ? "_rhc" : "_fhc"))
                         .c_str()));

  return blob;
}

} // namespace ana
