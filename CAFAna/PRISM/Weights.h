#pragma once

#include "CAFAna/Core/Weight.h"
#include "TH1D.h"

#include <string>

namespace PRISM {

// Yes I am defining a global map and no I am not proud of it.
extern std::map<int, TH1D *> fFileExposures;

ana::Weight GetNDCVWeight();
ana::Weight GetFDCVWeight();

// Very important to call this in the state file maker!
void SetFileExposuresMap();

double GetPerFileWeight(int SpecRun_ID, double abspos_x);

double Get280kAWeight_numu(double enu, bool isNu);
} // namespace PRISM
