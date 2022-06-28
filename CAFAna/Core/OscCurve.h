#pragma once

#include "CAFAna/Core/OscCalcFwdDeclare.h"
#include "CAFAna/Core/StanTypedefs.h"

#include "CAFAna/Core/Ratio.h"

namespace ana
{
  /// Transition probability for any one channel as a function of energy
  class OscCurve : public Ratio
  {
  public:
    OscCurve(osc::IOscCalc* calc, int from, int to);
    OscCurve(osc::IOscCalcStan* calc, int from, int to);
    virtual ~OscCurve();

    OscCurve(const OscCurve& rhs) = default;
    OscCurve& operator=(const OscCurve& rhs) = default;

    TH1D* ToTH1(bool title = false) const;

  protected:
    int fFrom, fTo;
  };
}
