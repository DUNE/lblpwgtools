#pragma once
#include "CAFAna/Analysis/Fit.h"
#include "OscLib/func/IOscCalculator.h"

namespace osc{class IOscCalculatorAdjustable;}

namespace ana
{
  class Fitter;

  class Resolution{
  public:
    Resolution(Fitter myfit, osc::IOscCalculatorAdjustable* testOsc);
    double FitResult(double *thisparam, double *dummy);
 private:
  Fitter fmyfit;
  osc::IOscCalculatorAdjustable* ftestOsc;
  };
}
