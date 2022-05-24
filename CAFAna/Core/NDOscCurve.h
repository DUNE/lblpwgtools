/////////////////////////////////////////////////////////////////////////////////
// NDOscCurve.h
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, January 2021
//
// Calculate ND oscillations.
// Adapted from NOvA sterile oscillation framework.
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CAFAna/Core/OscCalcFwdDeclare.h"
#include "CAFAna/Core/Ratio.h"

namespace ana {

  /// Transition probability for any one channel as a function of energy
  class NDOscCurve: public Ratio {
  public:

    NDOscCurve(osc::IOscCalc* calc, int from, int to);
    virtual ~NDOscCurve();

    NDOscCurve(const NDOscCurve& rhs) = default;
    NDOscCurve& operator=(const NDOscCurve& rhs) = default;

    TH1D* ToTH1(bool title = false) const;

  protected: 

    int fFrom, fTo;

  };

}
