#pragma once

#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/StanTypedefs.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Core/LoadFromFile.h"

#include "Utilities/func/Stan.h"

class TDirectory;

class TH1;

class TH1D;

namespace ana
{
  /// Base class defining interface for experiments
  class IExperiment
  {
    public:
      virtual ~IExperiment()
      {}

      virtual double ChiSq(osc::IOscCalculatorAdjustable *osc,
                           const SystShifts &syst = kNoShift) const
      {
        assert(false && "unimplemented");
        return 0;
      };

      virtual stan::math::var LogLikelihood(osc::IOscCalculatorAdjustableStan *osc,
                                            const SystShifts &syst = kNoShift) const
      {
        assert(false && "unimplemented");
        return 0;
      };

      virtual void SaveTo(TDirectory *dir, const std::string &name) const;

      // For MultiExperiment covariance matrix, need to be able to access prediction and data histograms
      virtual TH1D *PredHist(osc::IOscCalculator *calc,
                             const SystShifts &syst) const
      {
        // Only required if you want to use MultiExperiment covariance, so include
        // default implementation that returns null
        return nullptr;
      }

      virtual TH1D *DataHist() const
      {
        return nullptr;
      }

      // Bin masking of low stats bins
      virtual void ApplyMask(TH1 *a, TH1 *b) const
      {
      }
  };
}
