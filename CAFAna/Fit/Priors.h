#pragma once

#include <functional>

#include "CAFAna/Core/StanTypedefs.h"

namespace ana
{
  /// Prior uniform _in the variable that is being fitted_.
  stan::math::var PriorUniformInFitVar(const stan::math::var&, const osc::IOscCalculatorAdjustableStan*);

  /// Build a smoothed top-hat-like function that can be passed as a prior to (e.g.) FitVarWithPrior.
  /// Has value 1 over the 'hat' and 0 elsewhere, apart from a narrow transition region between them.
  /// The function is continuous and infinitely differentiable.
  /// Based on J. Sci. Comp. 29:1, eq. (2)-(5)
  ///
  /// \param hatCenter             Where you want the center of your top-hat to be
  /// \param hatWidth              How wide you want the top-hat to be
  /// \param transitionFracWidth   How wide you can tolerate the 'transition' region between top and brim of 'hat', in fraction of hat width
  /// \return                      A function using the given parameters that is passable to FitVarWithPrior
  std::function<stan::math::var(const stan::math::var&, const osc::IOscCalculatorAdjustableStan*)>
  TopHatPriorInFitVar(double hatCenter, double hatWidth, double transitionFracWidth=0.01);

  /// Gaussian prior on dm32, in 'scaled' units, i.e., dm32x1000.
  /// Gaussian is centered around 2.45e-3 with sigma=0.5e-3.
  stan::math::var GaussianPriorDm32Scaled(const stan::math::var& dm32, const osc::IOscCalculatorAdjustableStan*);


  /// Prior uniform in theta_23 for use when fitting sin^2(2*theta_23).
  stan::math::var UniformTh23Prior_FitSS2Th23(const stan::math::var& ss2th23, const osc::IOscCalculatorAdjustableStan*);

  /// Prior uniform in theta_23 for use when fitting sin^2(theta_23)
  stan::math::var UniformTh23Prior_FitSSTh23(const stan::math::var& ssth23, const osc::IOscCalculatorAdjustableStan*);
}
