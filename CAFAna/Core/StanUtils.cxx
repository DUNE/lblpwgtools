#include "TH1.h"

#include "CAFAna/Core/StanUtils.h"
#include "CAFAna/Core/Utilities.h"

#include "Utilities/func/Stan.h"

namespace ana
{
  //----------------------------------------------------------------------
  stan::math::var LogLikelihood(const std::vector <stan::math::var> &exp, const TH1 *obs)
  {
    assert(exp.size() >= 2 && int(exp.size() - 2) == obs->GetNbinsX());  // exp includes under- and overflow

    stan::math::var chi = 0;

    for (std::size_t i = 1; i < exp.size() - 1; ++i)
    {
      auto e = exp[i];
      auto o = obs->GetBinContent(i);

      chi += LogLikelihood(e, o);
    }

    return chi;
  }
}
