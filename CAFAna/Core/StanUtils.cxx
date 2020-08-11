#include "TH1.h"

#include "CAFAna/Core/StanUtils.h"
#include "CAFAna/Core/Utilities.h"

#include "Utilities/func/Stan.h"

namespace ana
{
  //----------------------------------------------------------------------
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
}
