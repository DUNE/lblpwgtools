#pragma once

#include "CAFAna/Core/Ratio.h"

#include "OscLib/OscCalc.h"

namespace ana
{
  class AtmosOscillogram: public Ratio
  {
  public:
    AtmosOscillogram(osc::IOscCalc* calc, int from, int to);

  protected:
    int fFrom, fTo;
  };
}
