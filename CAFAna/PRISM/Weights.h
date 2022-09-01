#pragma once

#include "CAFAna/Core/Weight.h"

#include <string>

namespace PRISM {
ana::Weight GetNDCVWeight();
ana::Weight GetFDCVWeight();

double GetPerFileWeight(int SpecRun_ID, double abspos_x);

double Get280kAWeight_numu(double enu, bool isNu);
} // namespace PRISM
