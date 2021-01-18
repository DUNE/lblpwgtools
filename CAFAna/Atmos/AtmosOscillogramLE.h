#pragma once

#include "CAFAna/Core/Ratio.h"

#include "OscLib/OscCalc.h"

namespace ana
{
  class AtmosOscillogramLE: public Ratio
  {
  public:
    AtmosOscillogramLE(osc::IOscCalc* calc, int from, int to);

  protected:
    int fFrom, fTo;
  };
}
