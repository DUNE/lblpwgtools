#include "CAFAna/Prediction/PredictionInterpKernel.h"

// This trivial function has been split out into a separate file to make it
// easier to confirm it's being vectorized. Do something like
//
// g++ -S -fverbose-asm PredictionInterpKernel.cxx -Ofast -march=native -I../..
//
// and look at PredictionInterpKernel.s for xmm, ymm or zmm registers

#ifdef CAFANA_USE_STAN
#include "CAFAna/Core/Stan.h"
#endif

namespace ana
{
  namespace PredIntKern
  {
    void ShiftSpectrumKernel(const Coeffs* fits,
                             unsigned int N,
                             double x, double x2, double x3,
                             double* corr)
    {
      for(unsigned int n = 0; n < N; ++n){
        const Coeffs& f = fits[n];
        corr[n] *= f.a*x3 + f.b*x2 + f.c*x + f.d;
      } // end for n
    }

#ifdef CAFANA_USE_STAN
    void ShiftSpectrumKernel(const Coeffs* fits,
                             unsigned int N,
                             const stan::math::var& x,
                             const stan::math::var& x2,
                             const stan::math::var& x3,
                             stan::math::var* corr)
    {
      for(unsigned int n = 0; n < N; ++n)
      {
        const Coeffs& f = fits[n];
        corr[n] *= f.a*x3 + f.b*x2 + f.c*x + f.d;
      } // end for n

    }
#endif

  }
}
