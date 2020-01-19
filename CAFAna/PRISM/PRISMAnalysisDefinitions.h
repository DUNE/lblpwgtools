#pragma once

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/Var.h"

#include <array>
#include <string>
#include <utility>

namespace PRISM {

struct PRISMAxisBlob {
  ana::HistAxis XProjection;
  ana::HistAxis OffAxisPosition;
};

ana::HistAxis GetEventRateMatchAxes(std::string const &binning = "uniform");
PRISMAxisBlob GetPRISMAxes(std::string const &varname,
                           std::string const &xbinning = "default",
                           std::array<double, 3> OABinning = {0.5, -3, 33});

extern const ana::Cut kETrueLT10GeV;

extern const ana::Cut kIsOutOfTheDesert;

// Use to weight by Exposure
extern const ana::Var kRunPlanWeight;

ana::Cut GetNDSignalCut(bool UseOnAxisSelection = false);
ana::Cut GetFDSignalCut(bool UseOnAxisSelection = false);

ana::Var GetNDWeight(std::string const &eweight = "");
ana::Var GetFDWeight(std::string const &eweight = "");

} // namespace PRISM