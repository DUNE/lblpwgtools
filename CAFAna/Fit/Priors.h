#pragma once

#include "CAFAna/Core/StanTypedefs.h"

namespace ana
{
  /// Prior uniform _in the variable that is being fitted_.
  stan::math::var PriorUniformInFitVar(const stan::math::var&, const osc::IOscCalculatorAdjustableStan*);

  /// Gaussian prior on dm32, in 'scaled' units, i.e., dm32x1000.
  /// Gaussian is centered around 2.45e-3 with sigma=0.5e-3.
  stan::math::var GaussianPriorDm32Scaled(const stan::math::var& dm32, const osc::IOscCalculatorAdjustableStan*);

  /// Prior uniform in theta_23 for use when fitting sin^2(2*theta_23).
  stan::math::var UniformTh23Prior_FitSS2Th23(const stan::math::var& ss2th23, const osc::IOscCalculatorAdjustableStan*);

  /// Prior uniform in theta_23 for use when fitting sin^2(theta_23)
  stan::math::var UniformTh23Prior_FitSSTh23(const stan::math::var& ssth23, const osc::IOscCalculatorAdjustableStan*);
}
