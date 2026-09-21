#include "lbl2025/PredictionGenerator3Flavor.h"

namespace lbl2025
{

  NumuExtrapGenerator::NumuExtrapGenerator(const ana::HistAxis axis,
                                           const ana::Cut cutFD,
                                           const ana::Cut cutND,
                                           const ana::Weight wei)
                                           : fAxis(axis), fCutFD(cutFD), fCutND(cutND), fWei(wei)
  {}

  std::unique_ptr<ana::IPrediction> NumuExtrapGenerator::Generate(ana::Loaders& loaders, const ana::RecoType& ixnRecoType, const ana::SystShifts& shiftMC) const
  {
    std::cout << "before decomp" << std::endl;
    // NB: NumuDecomp's shiftData is left at its default (kNoShift) for now;
    // ND data shifts are not yet wired up here.
    auto decomp = new ana::NumuDecomp(loaders, fAxis, fCutND, ixnRecoType, shiftMC, ana::kNoShift, fWei);
    std::cout << "after decomp" << std::endl;
    auto extrap = std::make_unique<ana::NumuExtrap>(loaders, *decomp, fAxis, fCutFD, fCutND, ixnRecoType, shiftMC, fWei);
    return std::make_unique<ana::PredictionExtrap>(extrap.release());
  }
}
