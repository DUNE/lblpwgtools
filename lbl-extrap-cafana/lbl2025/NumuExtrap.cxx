#include "lbl2025/NumuExtrap.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/SpectrumLoaderBase.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Decomp/IDecomp.h"

namespace ana
{
  // TODO: Add back in SystShifts and Weights
  NumuExtrap::NumuExtrap(IInteractionSource &nearDetSrc,
                         IInteractionSource &farDetMCnonswapSrc,
                         IInteractionSource &farDetMCfluxswapSrc,
                         IInteractionSource &farDetMCtauswapSrc,
                         const IDecomp& numuDecomp,
                         const HistAxis& axis,
                         const Cut& fdcut,
                         const Cut& ndcut)
    : ModularExtrap(farDetMCnonswapSrc,
                    farDetMCfluxswapSrc,
                    farDetMCtauswapSrc,
                    axis,
                    fdcut)
  {
    // mu -> mu ----
    fNumuSurv = std::unique_ptr<ModularExtrapComponent>(
       new TruthReweight(nearDetSrc, axis, axis, fdcut, kNoShift, kUnweighted,
            "mu -> mu", "#nu_{#mu} #rightarrow #nu_{#mu}",
            ndcut, numuDecomp,                           // numu selection in ND
            DecompResult::numu, kIsNumuCC && !kIsAntiNu, // numu truth in ND
            farDetMCnonswapSrc, kIsNumuCC && !kIsAntiNu) // mu->mu in FD
    );
    // mubar -> mubar ---
    fNumuSurvAnti = std::unique_ptr<ModularExtrapComponent>(
       new TruthReweight(nearDetSrc, axis, axis, fdcut, kNoShift, kUnweighted,
                         "mubar -> mubar", "#bar{#nu}_{#mu} #rightarrow #bar{#nu}_{#mu}",
                         ndcut, numuDecomp,                             // numu selection in ND
                         DecompResult::numubar, kIsNumuCC && kIsAntiNu, // numubar truth in ND
                         farDetMCnonswapSrc, kIsNumuCC && kIsAntiNu)    // mubar->mubar in FD
    );
  }

  //---------------------------------------------------------------------------
  NumuExtrap::NumuExtrap(Loaders& loaders,
                         const IDecomp& numuDecomp,
                         const HistAxis& axis,
                         const Cut& fdcut,
                         const Cut& ndcut,
                         ana::RecoType recoIxnType)
      : NumuExtrap(loaders.GetSource(DataMC::kMC, caf::Det_t::kNEARDET, SwappingConfig::kNonSwap).Interactions(ana::RecoType::kDLP),
                   loaders.GetSource(DataMC::kMC, caf::Det_t::kFARDET, SwappingConfig::kNonSwap).Interactions(recoIxnType),
                   loaders.GetSource(DataMC::kMC, caf::Det_t::kFARDET, SwappingConfig::kNueSwap).Interactions(recoIxnType),
                   loaders.GetSource(DataMC::kMC, caf::Det_t::kFARDET, SwappingConfig::kNuTauSwap).Interactions(recoIxnType),
                   numuDecomp,
                   axis,
                   fdcut,
                   ndcut)
  {
  }
}
