#include "CAFAna/Systs/SystComponentScale.h"

#include <cmath>

namespace ana
{
  //----------------------------------------------------------------------
  SystComponentScale::~SystComponentScale()
  {
  }

  //----------------------------------------------------------------------
  void SystComponentScale::Shift(double sigma,
                                  Restorer& /*restore*/,
                                  caf::StandardRecord* sr,
                                  double& weight) const
  {
    if(!fCut(sr)) return;

    weight *= pow(1+fOneSigma, sigma);
  }
}
