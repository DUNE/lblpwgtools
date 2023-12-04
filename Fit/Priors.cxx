#include "CAFAna/Fit/Priors.h"

#include "CAFAna/Core/Stan.h"

#include "stan/math/prim/prob/normal_lpdf.hpp"
#include "stan/math/prim/fun/erf.hpp"

namespace ana
{
  //----------------------------------------------------------------------
  stan::math::var PriorUniformInFitVar(const stan::math::var&, const osc::IOscCalcAdjustableStan*)
  {
    return 0.;
  }

  //----------------------------------------------------------------------
  std::function<stan::math::var(const stan::math::var&, const osc::IOscCalcAdjustableStan*)>
  TopHatPriorInFitVar(double hatCenter, double hatWidth, double transitionFracWidth)
  {
    if (hatWidth <= 0)
    {
      std::cerr << "TopHatPriorInFitVar(): hatWidth parameter must be positive, not " << hatWidth << std::endl;
      abort();
    }

    auto E = [](const stan::math::var & x) -> stan::math::var
    {
      if (x < -1)
        return -1;
      if (x > 1)
        return 1;
      return stan::math::erf(x/sqrt(1-x*x));  // just set L=1, seems to always be ok and saves an operation
    };

    auto H = [=](const stan::math::var & x) -> stan::math::var
    {
      return 0.5 * (1 + E(x));
    };

    return [=](const stan::math::var &var, const osc::IOscCalcAdjustableStan *) -> stan::math::var
    {
      const double chi = 0.5 * hatWidth;
      const double Psi = chi + transitionFracWidth;
      const double Omega = 0.5 * (Psi - chi);

      auto x = var - hatCenter;

      if (abs(x) >= Psi)
        return -std::numeric_limits<stan::math::var>::infinity();  // log(0) ........
      if (abs(x) <= chi)
        return 0;

      if (x < -chi)
        return log(H( (x + chi + Omega)/Omega ));
      if (x > chi)
        return log(H( -(x - chi - Omega)/Omega ));

      std::cerr << "TopHatPriorInFitVar(): How did we get here??" << std::endl;
      abort();
    };

  }


  //----------------------------------------------------------------------
  stan::math::var GaussianPriorDm32Scaled(const stan::math::var &dm32, const osc::IOscCalcAdjustableStan *)
  {
    return stan::math::normal_lpdf(dm32, 2.45, 0.5);
  }


  //----------------------------------------------------------------------
  stan::math::var UniformTh23Prior_FitSS2Th23(const stan::math::var& ss2th23, const osc::IOscCalcAdjustableStan*)
  {
    // need the Jacobian from sin^2(2*theta_23) to theta_23.
    // that's 4*sin(2*theta_23) = 4 * sqrt(sin^2(2*theta_23))
    //return log(4 * sqrt(ss2th23));
    return log(4) + 0.5 * log(ss2th23);
  }

  stan::math::var UniformTh23Prior_FitSSTh23(const stan::math::var& ssth23, const osc::IOscCalcAdjustableStan*)
  {
    // Jacobian from sin^2(theta_23) to theta_23 = 2*sin(theta_23)
    //return log(2 * sqrt(ssth23));
    return log(2) + 0.5*log(ssth23);
  }

}
