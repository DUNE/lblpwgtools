#pragma once

#include "CAFAna/Core/HistAxis.h"

#include <string>

namespace PRISM {

struct PRISMAxisBlob {
  ana::HistAxis XProjectionND;
  ana::HistAxis XProjectionFD_numu;
  ana::HistAxis XProjectionFD_nue;
  ana::HistAxis OffAxisPosition;
  ana::HistAxis OffAxis280kAPosition;
  ana::HistAxis XPairedData_numu_E;
  ana::HistAxis YPred_numu_E;
};

ana::HistAxis GetEventRateMatchAxes(std::string const &binning = "event_rate_match");
PRISMAxisBlob GetPRISMAxes(std::string const &varname,
                           std::string const &xbinning = "default",
                           std::string const &oabinning = "default");

ana::HistAxis TrueObservable(std::string const &obsvarname = "EVisReco",
                             std::string const &binning = "prism_noextrap");

ana::HistAxis RecoObservable(std::string const &obsvarname = "EVisReco",
                             std::string const &binning = "prism_noextrap");

bool isRecoND(std::string var = "ETrue");

}; // namespace PRISM
