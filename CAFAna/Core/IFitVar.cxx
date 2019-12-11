#include "CAFAna/Core/IFitVar.h"

#include "Utilities/func/MathUtil.h"
#include "Utilities/func/Stan.h"

#include <cmath>

namespace ana
{
  //----------------------------------------------------------------------
  stan::math::var StanExp(const stan::math::var& x){return exp(x);}
  stan::math::var StanLog(const stan::math::var& x){return log(x);}
} // namespace
