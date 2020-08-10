#pragma once

#include "CAFAna/Experiment/IExperiment.h"
#include "CAFAna/Core/Spectrum.h"


namespace ana
{
  class IPrediction;

  /// Compare a data spectrum to MC expectation using only the event count
  class CountingExperiment: public IExperiment
  {
  public:
    CountingExperiment(const IPrediction* p, const Spectrum& d);
    ~CountingExperiment();
    virtual double ChiSq(osc::IOscCalcAdjustable* osc,
                         const SystShifts& syst = SystShifts::Nominal()) const override;

    virtual void SaveTo(TDirectory* dir, const std::string& name) const override;
    static std::unique_ptr<CountingExperiment> LoadFrom(TDirectory* dir, const std::string& name);
  protected:
    const IPrediction* fMC;
    Spectrum fData;
  };
}
