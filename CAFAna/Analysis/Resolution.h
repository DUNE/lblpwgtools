#pragma once

#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Fit/MinuitFitter.h"
#include "OscLib/func/IOscCalculator.h"

namespace ana
{
  class Resolution{
  public:
    Resolution(MinuitFitter myfit, osc::IOscCalculatorAdjustable* testOsc, int which);
    double FitResult(double *thisparam, double *dummy);
 private:
  MinuitFitter fmyfit;
  osc::IOscCalculatorAdjustable* ftestOsc;
  int fwhich; //defines projection 0=dcp, 1=th12, 2=th23, 3=dm2
  };
}
