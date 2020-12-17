#pragma once

#include "CAFAna/Core/ReweightableSpectrum.h"

#include "CAFAna/Core/OscillatableSpectrum.h" // borrow their OscCache struct

#include "CAFAna/Core/ThreadLocal.h"

#include "OscLib/OscCalc.h"

namespace ana
{
  class AtmosOscSpect: public ReweightableSpectrum
  {
  public:
    AtmosOscSpect(const LabelsAndBins& recoAxis);

    // These under a different name
    Spectrum Unoscillated() const {return UnWeighted();}
    Spectrum TrueEnergy() const {return WeightingVariable();}

    Spectrum Oscillated(osc::IOscCalc* calc, int from, int to) const;

  protected:
    mutable ThreadLocal<OscCache> fCache;
  };
}
