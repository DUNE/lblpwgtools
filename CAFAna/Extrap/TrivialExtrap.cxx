#include "CAFAna/Extrap/TrivialExtrap.h"

#include "CAFAna/Core/LoadFromRegistry.h"

#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/SpectrumLoader.h"

#include "TDirectory.h"
#include "TObjString.h"

namespace ana
{
  REGISTER_LOADFROM("TrivialExtrap", IExtrap, TrivialExtrap);

  //----------------------------------------------------------------------
  TrivialExtrap::TrivialExtrap(IInteractionSource& nonswapSrc,
                               IInteractionSource& nueSrc,
                               IInteractionSource& tauSrc,
                               const HistAxis& axis)
    :
    fNueApp       (nueSrc    [kIsNueApp    && !kIsAntiNu], axis),
    fNueAppAnti   (nueSrc    [kIsNueApp    &&  kIsAntiNu], axis),
    fNumuSurv     (nonswapSrc[kIsNumuCC    && !kIsAntiNu], axis),
    fNumuSurvAnti (nonswapSrc[kIsNumuCC    &&  kIsAntiNu], axis),
    fNumuApp      (tauSrc    [kIsNumuApp   && !kIsAntiNu], axis),
    fNumuAppAnti  (tauSrc    [kIsNumuApp   &&  kIsAntiNu], axis),
    fNueSurv      (nonswapSrc[kIsBeamNue   && !kIsAntiNu], axis),
    fNueSurvAnti  (nonswapSrc[kIsBeamNue   &&  kIsAntiNu], axis),
    fTauFromE     (nueSrc    [kIsTauFromE  && !kIsAntiNu], axis),
    fTauFromEAnti (nueSrc    [kIsTauFromE  &&  kIsAntiNu], axis),
    fTauFromMu    (tauSrc    [kIsTauFromMu && !kIsAntiNu], axis),
    fTauFromMuAnti(tauSrc    [kIsTauFromMu &&  kIsAntiNu], axis),
    fNCTot        (nonswapSrc[kIsNC                     ], axis),
    fNC           (nonswapSrc[kIsNC        && !kIsAntiNu], axis),
    fNCAnti       (nonswapSrc[kIsNC        &&  kIsAntiNu], axis)
  {
    // All swapped files are equally valid as a source of NCs. This
    // approximately doubles/triples our statistics. SpectrumLoader just adds
    // events and POT for both cases, which is the right thing to do.

    nueSrc[kIsNC].GetVar(axis.GetVar1D()).Register(&fNCTot);
    nueSrc[kIsNC && !kIsAntiNu].GetVar(axis.GetVar1D()).Register(&fNC);
    nueSrc[kIsNC &&  kIsAntiNu].GetVar(axis.GetVar1D()).Register(&fNCAnti);

    tauSrc[kIsNC].GetVar(axis.GetVar1D()).Register(&fNCTot);
    tauSrc[kIsNC && !kIsAntiNu].GetVar(axis.GetVar1D()).Register(&fNC);
    tauSrc[kIsNC &&  kIsAntiNu].GetVar(axis.GetVar1D()).Register(&fNCAnti);
  }

  /*
  //----------------------------------------------------------------------
  TrivialExtrap::TrivialExtrap(Loaders& loaders,
                               const HistAxis& axis,
                               const Cut& cut,
                               const SystShifts& shift,
                               const Weight& wei)
    : TrivialExtrap(loaders.GetLoader(caf::kFARDET, Loaders::kMC, Loaders::kNonSwap),
                    loaders.GetLoader(caf::kFARDET, Loaders::kMC, Loaders::kNueSwap),
                    loaders.GetLoader(caf::kFARDET, Loaders::kMC, Loaders::kNuTauSwap),
                    axis)
  {
  }
  */

  //----------------------------------------------------------------------
  void TrivialExtrap::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("TrivialExtrap").Write("type");

    fNueApp.SaveTo(dir, "nue_app");
    fNueAppAnti.SaveTo(dir, "nue_app_anti");
    fNCTot.SaveTo(dir, "nc_tot");
    fNC.SaveTo(dir, "nc");
    fNCAnti.SaveTo(dir, "nc_anti");
    fNumuSurv.SaveTo(dir, "numu_surv");
    fNumuSurvAnti.SaveTo(dir, "numu_surv_anti");
    fNumuApp.SaveTo(dir, "numu_app");
    fNumuAppAnti.SaveTo(dir, "numu_app_anti");
    fNueSurv.SaveTo(dir, "nue_surv");
    fNueSurvAnti.SaveTo(dir, "nue_surv_anti");
    fTauFromE.SaveTo(dir, "nutau_from_nue");
    fTauFromEAnti.SaveTo(dir, "nutau_from_nue_anti");
    fTauFromMu.SaveTo(dir, "nutau_from_numu");
    fTauFromMuAnti.SaveTo(dir, "nutau_from_numu_anti");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<TrivialExtrap> TrivialExtrap::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    std::unique_ptr<TrivialExtrap> ret(new TrivialExtrap);

    ret->fNueApp        = *OscillatableSpectrum::LoadFrom(dir, "nue_app");
    ret->fNueAppAnti    = *OscillatableSpectrum::LoadFrom(dir, "nue_app_anti");
    ret->fNumuSurv      = *OscillatableSpectrum::LoadFrom(dir, "numu_surv");
    ret->fNumuSurvAnti  = *OscillatableSpectrum::LoadFrom(dir, "numu_surv_anti");
    ret->fNumuApp       = *OscillatableSpectrum::LoadFrom(dir, "numu_app");
    ret->fNumuAppAnti   = *OscillatableSpectrum::LoadFrom(dir, "numu_app_anti");
    ret->fNueSurv       = *OscillatableSpectrum::LoadFrom(dir, "nue_surv");
    ret->fNueSurvAnti   = *OscillatableSpectrum::LoadFrom(dir, "nue_surv_anti");
    ret->fTauFromE      = *OscillatableSpectrum::LoadFrom(dir, "nutau_from_nue");
    ret->fTauFromEAnti  = *OscillatableSpectrum::LoadFrom(dir, "nutau_from_nue_anti");
    ret->fTauFromMu     = *OscillatableSpectrum::LoadFrom(dir, "nutau_from_numu");
    ret->fTauFromMuAnti = *OscillatableSpectrum::LoadFrom(dir, "nutau_from_numu_anti");

    ret->fNCTot  = *Spectrum::LoadFrom(dir, "nc_tot");
    ret->fNC     = *Spectrum::LoadFrom(dir, "nc");
    ret->fNCAnti = *Spectrum::LoadFrom(dir, "nc_anti");

    delete dir;

    return ret;
  }
}
