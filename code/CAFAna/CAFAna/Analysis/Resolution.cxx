#include "CAFAna/Analysis/Resolution.h"

#include <iostream>
#include <functional>


namespace ana
{
  Resolution::Resolution(Fitter myfit, osc::IOscCalculatorAdjustable* testOsc, int which)
    : fmyfit(myfit), ftestOsc(testOsc), fwhich(which) {}
  double Resolution::FitResult(double *thisparam, double*dummy) {
    if (fwhich == 0) {
      ftestOsc->SetdCP(*thisparam);
      //std::cout << "DCP inside resolution function: " << ftestOsc->GetdCP() << std::endl;
    }
    else if (fwhich == 1) {
      ftestOsc->SetTh13(*thisparam);
      std::cout << "Th13 inside resolution function: " << ftestOsc->GetTh13() << std::endl;
    }
    else if (fwhich == 2) {
      ftestOsc->SetTh23(*thisparam);
      std::cout << "Th23 inside resolution function: " << ftestOsc->GetTh23() << std::endl;
    }
    else if (fwhich == 3) {
      ftestOsc->SetDmsq32(*thisparam);
      std::cout << "Dmsq inside resolution function: " << ftestOsc->GetDmsq32() << std::endl;
    }

    
    return fmyfit.Fit(ftestOsc, Fitter::kQuiet) - 1.0;
  }
}
