#pragma once

#include "CAFAna/Core/StanTypedefs.h"

namespace ana
{
  namespace PredIntKern
  {

    struct Coeffs{
      Coeffs(double _a, double _b, double _c, double _d)
        : a(_a), b(_b), c(_c), d(_d) {}
      double a, b, c, d;
    };

    void ShiftSpectrumKernel(const Coeffs* fits,
                             unsigned int N,
                             double x, double x2, double x3,
                             double* corr);

    /// Normally I'd make the <double> variant templated,
    /// but this function is so short that it makes more sense
    /// to leave the one with <double> arguments pass-by-value
    /// and do pass-by-ref on the Stan ones here
    void ShiftSpectrumKernel(const Coeffs* fits,
                             unsigned int N,
                             const stan::math::var& x, const stan::math::var& x2, const stan::math::var& x3,
                             stan::math::var* corr);
  }
}
