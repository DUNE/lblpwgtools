#pragma once

namespace osc
{
  template<class T> class _IOscCalc;
  template<class T> class _IOscCalcAdjustable;

  typedef _IOscCalc<double> IOscCalc;
  typedef _IOscCalcAdjustable<double> IOscCalcAdjustable;

  class IOscCalcSterile;
}
