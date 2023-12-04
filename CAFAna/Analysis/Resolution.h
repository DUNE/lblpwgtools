#pragma once

#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Fit/MinuitFitter.h"
#include "OscLib/IOscCalc.h"

namespace ana
{
  class Resolution{
  public:
    Resolution(MinuitFitter& myfit, osc::IOscCalcAdjustable* testOsc, int which);
    double FitResult(double *thisparam, double *dummy);
 private:
  MinuitFitter& fmyfit;
  osc::IOscCalcAdjustable* ftestOsc;
  int fwhich; //defines projection 0=dcp, 1=th12, 2=th23, 3=dm2
  };
}
