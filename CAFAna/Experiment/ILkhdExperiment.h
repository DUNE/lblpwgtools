#pragma once

#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/StanTypedefs.h"
#include "CAFAna/Core/SystShifts.h"

class TDirectory;

namespace ana
{
  class IExperiment;

  template <typename T>
  class _ILkhdExperiment
  {
    public:
      virtual T LogLikelihood(osc::_IOscCalculatorAdjustable<T> *osc,
                              const SystShifts &syst = kNoShift) const = 0;

      virtual void SaveTo(TDirectory* dir) const
      {
        assert(0 && "Not implemented");
      }
  };

  typedef _ILkhdExperiment<stan::math::var> ILkhdExperiment;

  /// Create a likelihood experiment from a chisq one.
  /// Only a good idea if you want to throw away all the stan::math::var-ness
  /// of a LL experiment's vars (and thereby not use Stan's autodiff).
  class ChiSqToLklhdExptWrapper : public _ILkhdExperiment<double>
  {
    public:
      ChiSqToLklhdExptWrapper(const IExperiment& chisqExpt)
        : fChiSqExpt(chisqExpt)
      {}

      double LogLikelihood(osc::IOscCalculatorAdjustable* osc,
                           const SystShifts& syst = SystShifts::Nominal()) const override;

    protected:
      const IExperiment & fChiSqExpt;
  };
}

