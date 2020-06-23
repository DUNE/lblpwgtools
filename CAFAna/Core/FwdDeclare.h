/// \file FwdDeclare.h -- consolidate forward declarations of template types
///                       and associated typdefs into one place so that they
///                       can be easily updated if needed.

#pragma once

namespace osc
{
  template<class T> class _IOscCalculator;
  template<class T> class _IOscCalculatorAdjustable;

  typedef _IOscCalculator<double> IOscCalculator;
  typedef _IOscCalculatorAdjustable<double> IOscCalculatorAdjustable;
}
