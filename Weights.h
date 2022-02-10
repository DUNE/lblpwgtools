#pragma once

#include "CAFAna/Core/Weight.h"

#include <string>

namespace PRISM {
ana::Weight GetNDCVWeight();
ana::Weight GetFDCVWeight();

double Get280kAWeight_numu(double enu, bool isNu);
} // namespace PRISM