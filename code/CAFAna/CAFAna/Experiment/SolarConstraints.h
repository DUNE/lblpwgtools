#pragma once

#include "CAFAna/Experiment/IExperiment.h"

#include <memory>

namespace ana
{
  /// \brief Constraints on the parameters \f$ \Delta m^2_{21} \f$ and
  /// \f$ \sin^22\theta_{12} \f$ from solar experiments
  class SolarConstraints: public IExperiment
  {
  public:
    SolarConstraints();

    virtual double ChiSq(osc::IOscCalculatorAdjustable* osc,
                         const SystShifts& syst = SystShifts::Nominal()) const;

    virtual void SaveTo(TDirectory* dir) const;
    static std::unique_ptr<SolarConstraints> LoadFrom(TDirectory* dir);
  protected:
    double fCentralDmsq, fErrorDmsq;
    double fCentralAngle, fErrorAngle;
  };
}
