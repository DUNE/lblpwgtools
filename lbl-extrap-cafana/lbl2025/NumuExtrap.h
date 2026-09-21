#pragma once

#include "CAFAna/Extrap/ModularExtrap.h"

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/IRecordSource.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Weight.h"

namespace ana
{
  class IDecomp;

  class NumuExtrap : public ModularExtrap
  {
    public:
      NumuExtrap(IInteractionSource &nearDetSrc,
                 IInteractionSource &farDetMCnonswapSrc,
                 IInteractionSource &farDetMCfluxswapSrc,
                 IInteractionSource &farDetMCtauswapSrc,
                 const IDecomp &numuDecomp,
                 const HistAxis &axis,
                 const Cut &fdcut,
                 const Cut &ndcut,
                 const SystShifts &shiftMC = kNoShift,
                 const Weight &weight = kUnweighted);

      NumuExtrap(Loaders &loaders,
                 const IDecomp &numuDecomp,
                 const HistAxis &axis,
                 const Cut &fdcut,
                 const Cut &ndcut,
                 ana::RecoType recoIxnType,
                 const SystShifts &shiftMC = kNoShift,
                 const Weight &weight = kUnweighted);
  };
}
