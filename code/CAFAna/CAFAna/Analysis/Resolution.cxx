#include "CAFAna/Analysis/Resolution.h"

#include <iostream>
#include <functional>


namespace ana
{
  Resolution::Resolution(Fitter myfit, osc::IOscCalculatorAdjustable* testOsc)
    : fmyfit(myfit), ftestOsc(testOsc){}
  double Resolution::FitResult(double *thisparam, double*dummy) {
    ftestOsc->SetdCP(*thisparam);
    //std::cout << "DCP inside resolution function: " << ftestOsc->GetdCP() << std::endl;
    return fmyfit.Fit(ftestOsc, Fitter::kQuiet) - 1.0;
  }
}
