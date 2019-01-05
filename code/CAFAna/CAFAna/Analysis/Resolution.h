#pragma once
#include "CAFAna/Analysis/Fit.h"
#include "OscLib/func/IOscCalculator.h"

namespace osc{class IOscCalculatorAdjustable;}

namespace ana
{
  class Fitter;

  class Resolution{
  public:
    Resolution(Fitter myfit, osc::IOscCalculatorAdjustable* testOsc, int which);
    double FitResult(double *thisparam, double *dummy);
 private:
  Fitter fmyfit;
  osc::IOscCalculatorAdjustable* ftestOsc;
  int fwhich; //defines projection 0=dcp, 1=th12, 2=th23, 3=dm2
  };
}
