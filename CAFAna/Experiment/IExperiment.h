#pragma once

#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/StanTypedefs.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Stan.h"

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

      virtual double ChiSq(osc::IOscCalcAdjustable *osc,
                           const SystShifts &syst = kNoShift) const
      {
        assert(false && "unimplemented");
        return 0;
      };

      virtual stan::math::var LogLikelihood(osc::IOscCalcAdjustableStan *osc,
                                            const SystShifts &syst = kNoShift) const
      {
        assert(false && "unimplemented");
        return 0;
      };

      virtual void SaveTo(TDirectory *dir, const std::string &name) const;
  };
}
