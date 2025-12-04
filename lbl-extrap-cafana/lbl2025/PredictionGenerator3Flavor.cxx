#include "lbl2025/PredictionGenerator3Flavor.h"

namespace lbl2025
{

  NumuExtrapGenerator::NumuExtrapGenerator(const ana::HistAxis axis,
                                           const ana::Cut cutFD,
                                           const ana::Cut cutND)
                                           : fAxis(axis), fCutFD(cutFD), fCutND(cutND)
  {}

  std::unique_ptr<ana::IPrediction> NumuExtrapGenerator::Generate(ana::Loaders& loaders, const ana::RecoType& ixnRecoType, const ana::SystShifts& shiftMC) const
  {
    auto decomp = new ana::NumuDecomp(loaders, fAxis, fCutND, ixnRecoType);
    auto extrap = std::make_unique<ana::NumuExtrap>(loaders, *decomp, fAxis, fCutFD, fCutND, ixnRecoType);
    return std::make_unique<ana::PredictionExtrap>(extrap.release());
  }
}
