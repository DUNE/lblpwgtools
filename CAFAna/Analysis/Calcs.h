#pragma once

#include "CAFAna/Core/OscCalcFwdDeclare.h"

namespace ana
{
  /// Reset calculator to default assumptions for all parameters
  void ResetOscCalcToDefault(osc::IOscCalcAdjustable* calc);
  void ResetOscCalcToDefaultIH(osc::IOscCalcAdjustable* calc);

  /// Create a new calculator with default assumptions for all parameters
  osc::IOscCalcAdjustable* DefaultOscCalc();
  osc::IOscCalcAdjustable* DefaultOscCalcIH();

  /// Reset calculator to default assumptions for all parameters
  void ResetSterileCalcToDefault(osc::IOscCalcSterile* calc);

  /// Create a sterile calculator with default assumptions for all parameters
  osc::IOscCalcSterile* DefaultSterileCalc(int nflavors);
}
