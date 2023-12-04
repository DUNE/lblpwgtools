#pragma once

#include "CAFAna/Experiment/IExperiment.h"

#include <memory>

namespace ana
{
  /// Very simple model allowing inclusion of reactor constraints
  class ReactorExperiment : public IExperiment
  {
  public:
    ReactorExperiment(double bestFit, double sigma)
      : fBestFit(bestFit), fSigma(sigma)
    {
    }

    virtual double ChiSq(osc::IOscCalcAdjustable* osc,
                         const SystShifts& shift = SystShifts::Nominal()) const override;

    void SaveTo(TDirectory* dir, const std::string& name) const override;
    static std::unique_ptr<ReactorExperiment> LoadFrom(TDirectory* dir, const std::string& name);
  protected:
    virtual double SSTh13(osc::IOscCalcAdjustable* osc) const;

    double fBestFit, fSigma;
  };


  /// Reactor constraint from NuFit 2018 
  const ReactorExperiment* ReactorConstraintNuFit2018(bool normal_ordering, bool sk_data);

  /// Reactor constraint from NuFit 2019 
  const ReactorExperiment* ReactorConstraintNuFit2019(bool normal_ordering, bool sk_data);

  /// Reactor constraint from the PDG 2019
  const ReactorExperiment* ReactorConstraintPDG2019();
}
