#pragma once

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
  }
}
