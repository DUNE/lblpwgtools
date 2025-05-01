#pragma once

#include <cassert>

#include "CAFAna/Core/OscCalcFwdDeclare.h"

#ifdef CAFANA_USE_STAN
#include "CAFAna/Core/StanTypedefs.h"
#endif

#include "CAFAna/Core/SystShifts.h"

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

#ifdef CAFANA_USE_STAN
      virtual stan::math::var LogLikelihood(osc::IOscCalcAdjustableStan *osc,
                                            const SystShifts &syst = kNoShift) const
      {
        assert(false && "unimplemented");
        return 0;
      };
#endif

      virtual void SaveTo(TDirectory *dir, const std::string &name) const;
  };
}
