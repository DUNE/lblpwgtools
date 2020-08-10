#pragma once

#include "CAFAna/Core/StanTypedefs.h"

namespace ana
{
  /// Prior uniform _in the variable that is being fitted_.
  stan::math::var PriorUniformInFitVar(const stan::math::var&, const osc::IOscCalcAdjustableStan*);

  /// Prior uniform in theta_23 for use when fitting sin^2(2*theta_23).
  stan::math::var UniformTh23Prior_FitSS2Th23(const stan::math::var& ss2th23, const osc::IOscCalcAdjustableStan*);

  /// Prior uniform in theta_23 for use when fitting sin^2(theta_23)
  stan::math::var UniformTh23Prior_FitSSTh23(const stan::math::var& ssth23, const osc::IOscCalcAdjustableStan*);
}
