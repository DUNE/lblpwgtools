#pragma once

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/Var.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "StandardRecord/StandardRecord.h"

#include "TGraph.h"
#include "TH1D.h"

#include <vector>

namespace ana {

class FVMassCorrection {
  size_t fOverflow;

public:
  TH1D *fvmasscor;

  FVMassCorrection() {
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
  double GetWeight(double vtx_x_cm) {
    size_t bi_it = fvmasscor->FindFixBin(vtx_x_cm);
    if ((bi_it == 0) || (bi_it == fOverflow)) {
      return 0;
    }
    return fvmasscor->GetBinContent(bi_it);
  }
};

inline double FD_ND_FVRatio(double x_slice_cm) {
  double NDSliceFV = x_slice_cm * 200 * (350 - 50);
  double FDFV = 620 * 1100 * (1244 - 50);

  return FDFV / NDSliceFV;
}

const ana::Cut kIsOutOfTheDesert({"kIsOutOfTheDesert"},
                                 [](const caf::StandardRecord *sr) {
                                   return (fabs(sr->dune.vtx_x) < 200);
                                 });

std::vector<double> const FDnumuFHCSelEff_enu = {
    0.546, 0.683, 0.821, 0.945, 1.07, 1.19, 1.44, 1.67,
    1.94,  2.25,  2.86,  4.17,  4.58, 5.44, 6.57, 7.65};

std::vector<double> const FDnumuFHCSelEff_eff = {
    0.624, 0.708, 0.761, 0.808, 0.839, 0.869, 0.902, 0.914,
    0.930, 0.944, 0.956, 0.958, 0.960, 0.943, 0.929, 0.912};

std::vector<double> const ND_UnGeoCorrectible_enu = {
    0.183, 0.502, 0.870, 1.25, 1.68, 2.45, 3.27, 4.19, 5.28, 6.48, 9.05};

std::vector<double> const ND_UnGeoCorrectible_eff = {0.996, 0.996, 0.985, 0.966,
                                                     0.958, 0.932, 0.898, 0.867,
                                                     0.845, 0.807, 0.742};

TGraph FDnumuFHCSelEff(FDnumuFHCSelEff_enu.size(), FDnumuFHCSelEff_enu.data(),
                       FDnumuFHCSelEff_eff.data());
const ana::Var kFDEff({}, [&](const caf::StandardRecord *sr) -> double {
  return FDnumuFHCSelEff.Eval(sr->dune.Ev);
});

TGraph ND_UnGeoCorrectibleEff(ND_UnGeoCorrectible_enu.size(),
                              ND_UnGeoCorrectible_enu.data(),
                              ND_UnGeoCorrectible_eff.data());
const ana::Var kNDEff({}, [&](const caf::StandardRecord *sr) -> double {
  return ND_UnGeoCorrectibleEff.Eval(sr->dune.Ev);
});

// Use to weight by Exposure
const ana::Var kRunPlanWeight({}, [&](const caf::StandardRecord *sr) -> double {
  return sr->dune.perPOTWeight * sr->dune.perFileWeight;
});

ana::FVMassCorrection fvmc;
const ana::Var kMassCorrection({},
                               [&](const caf::StandardRecord *sr) -> double {
                                 return fvmc.GetWeight(sr->dune.vtx_x);
                               });

} // namespace ana
