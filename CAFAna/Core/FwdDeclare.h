/// \file FwdDeclare.h -- consolidate forward declarations of template types
///                       and associated typdefs into one place so that they
///                       can be easily updated if needed.

#pragma once

namespace osc
{
  template<class T> class _IOscCalc;
  template<class T> class _IOscCalcAdjustable;

  typedef _IOscCalc<double> IOscCalc;
  typedef _IOscCalcAdjustable<double> IOscCalcAdjustable;
}
