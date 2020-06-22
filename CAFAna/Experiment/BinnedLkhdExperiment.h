#pragma once

#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/StanTypedefs.h"
#include "CAFAna/Experiment/ILkhdExperiment.h"

namespace ana
{
  class IPrediction;

  /// The Bayesian fit analog to SingleSampleExperiment.
  class BinnedLkhdExperiment : public ILkhdExperiment
  {
    public:
      BinnedLkhdExperiment(const IPrediction *MC, const Spectrum& data);

      stan::math::var LogLikelihood(osc::_IOscCalculatorAdjustable<stan::math::var> *osc,
                                    const SystShifts &syst) const override;

    private:
      const IPrediction* fMC;
      Spectrum fData;

  };

}
