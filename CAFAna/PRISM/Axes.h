#pragma once

#include "CAFAna/Core/HistAxis.h"

#include <string>

namespace PRISM {

struct PRISMAxisBlob {
  ana::HistAxis XProjectionND;
  ana::HistAxis XProjectionFD;
  ana::HistAxis OffAxisPosition;
  ana::HistAxis OffAxis280kAPosition;
};

ana::HistAxis GetEventRateMatchAxes(std::string const &binning = "uniform");
PRISMAxisBlob GetPRISMAxes(std::string const &varname,
                           std::string const &xbinning = "default",
                           std::string const &oabinning = "default");

ana::HistAxis TrueObservable(std::string const &obsvarname = "EProxy",
                             std::string const &binning = "uniform_smallrange");

ana::HistAxis RecoObservable(std::string const &obsvarname = "EProxy",
                             std::string const &binning = "uniform_smallrange");

bool isRecoND(std::string var = "ETrue");

}; // namespace PRISM