#include "CAFAna/Core/IFitVar.h"

#include "Utilities/func/MathUtil.h"
#include "Utilities/func/Stan.h"

#include <cmath>

namespace ana
{
  //----------------------------------------------------------------------
  double IConstrainedFitVar::Penalty(double val,
                                     osc::IOscCalculatorAdjustable*) const
  {
    const double lo = LowLimit();
    const double hi = HighLimit();

    if(val >= lo && val <= hi) return 0;

    // Try to direct fit back towards centre of the space. Engineer penalty to
    // be zero at the limits.
    const double mean = (lo+hi)/2;
    const double rad = (hi-lo)/2;
    return util::sqr((val-mean)/rad)-1;


    //    if(val < lo) return util::sqr(lo-val);
    //    if(val > hi) return util::sqr(val-hi);
    //    return 0;
  }

  //----------------------------------------------------------------------
  stan::math::var StanExp(const stan::math::var& x){return exp(x);}
  stan::math::var StanLog(const stan::math::var& x){return log(x);}
} // namespace
