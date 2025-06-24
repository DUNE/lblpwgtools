#include "CAFAna/Core/StanUtils.h"

#include <iostream>

#ifdef CAFANA_USE_STAN
#include "CAFAna/Core/Stan.h"
#endif

#include "CAFAna/Core/Utilities.h"

namespace util
{
  //----------------------------------------------------------------------

#ifdef CAFANA_USE_STAN
  stan::math::var LogLikelihood(const Eigen::ArrayXstan& exp,
                                const Eigen::ArrayXd& obs)
  {
    assert(exp.size() >= 2 && exp.size() == obs.size());

    stan::math::var chi = 0;

    for (int i = 1; i < exp.size() - 1; ++i)
    {
      chi += LogLikelihood(exp[i], obs[i]);
    }

    return chi;
  }
#endif

  void NoStanError()
  {
    const auto err = "PredictionInterp::ShiftSpectrum(): Attempt to use Stan-aware Spectrum or Syst without Stan support built into CAFAna!  Abort.";
    std::cerr << err << "\n";
    throw std::runtime_error(err);
  }

}
