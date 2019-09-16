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

  FVMassCorrection();
  double GetWeight(double vtx_x_cm);
};

inline double FD_ND_FVRatio(double x_slice_cm) {
  double NDSliceFV = x_slice_cm * 200 * (350 - 50);
  double FDFV = 620 * 1100 * (1244 - 50);

  return FDFV / NDSliceFV;
}

extern const ana::Cut kIsOutOfTheDesert;

// Use to weight by Exposure
extern const ana::Var kRunPlanWeight;

extern ana::FVMassCorrection fvmc;
extern const ana::Var kMassCorrection;

} // namespace ana
