#include "CAFAna/Fit/Priors.h"

#include "Utilities/func/Stan.h"
#include "stan/math/prim/scal/prob/normal_lpdf.hpp"

namespace ana
{
  stan::math::var PriorUniformInFitVar(const stan::math::var&, const osc::IOscCalculatorAdjustableStan*)
  {
    return 0.;
  }

  stan::math::var GaussianPriorDm32Scaled(const stan::math::var &dm32, const osc::IOscCalculatorAdjustableStan *)
  {
    return stan::math::normal_lpdf(dm32, 2.45, 0.5);
  }

  stan::math::var UniformTh23Prior_FitSS2Th23(const stan::math::var& ss2th23, const osc::IOscCalculatorAdjustableStan*)
  {
    // need the Jacobian from sin^2(2*theta_23) to theta_23.
    // that's 4*sin(2*theta_23) = 4 * sqrt(sin^2(2*theta_23))
    //return log(4 * sqrt(ss2th23));
    return log(4) + 0.5 * log(ss2th23);
  }

  stan::math::var UniformTh23Prior_FitSSTh23(const stan::math::var& ssth23, const osc::IOscCalculatorAdjustableStan*)
  {
    // Jacobian from sin^2(theta_23) to theta_23 = 2*sin(theta_23)
    //return log(2 * sqrt(ssth23));
    return log(2) + 0.5*log(ssth23);
  }

}
