#pragma once

#include "Utilities/func/StanVar.h"

class TH1;

namespace ana
{
  /// Variant that handles the prediction in the form of Stan vars
  stan::math::var LogLikelihood(const std::vector<stan::math::var>& exp, const TH1* obs);

}
