#include "CAFAna/Extrap/TrivialExtrap.h"

#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/SpectrumLoader.h"

#include "TDirectory.h"
#include "TObjString.h"

namespace ana
{
  //----------------------------------------------------------------------
  TrivialExtrap::TrivialExtrap(SpectrumLoaderBase& loaderNonswap,
                               SpectrumLoaderBase& loaderNue,
                               SpectrumLoaderBase& loaderNuTau,
                               const HistAxis& axis,
                               const Cut& cut,
                               const SystShifts& shift,
                               const Var& wei)
    :
    fNueApp       (loaderNue,     axis, cut && kIsSig       && !kIsAntiNu, shift, wei),
    fNueAppAnti   (loaderNue,     axis, cut && kIsSig       &&  kIsAntiNu, shift, wei),

    fNumuSurv     (loaderNonswap, axis, cut && kIsNumuCC    && !kIsAntiNu, shift, wei),
    fNumuSurvAnti (loaderNonswap, axis, cut && kIsNumuCC    &&  kIsAntiNu, shift, wei),

    fNumuApp      (loaderNuTau,   axis, cut && kIsNumuApp   && !kIsAntiNu, shift, wei),
    fNumuAppAnti  (loaderNuTau,   axis, cut && kIsNumuApp   &&  kIsAntiNu, shift, wei),

    fNueSurv      (loaderNonswap, axis, cut && kIsBeamNue   && !kIsAntiNu, shift, wei),
    fNueSurvAnti  (loaderNonswap, axis, cut && kIsBeamNue   &&  kIsAntiNu, shift, wei),

    fTauFromE     (loaderNue,     axis, cut && kIsTauFromE  && !kIsAntiNu, shift, wei),
    fTauFromEAnti (loaderNue,     axis, cut && kIsTauFromE  &&  kIsAntiNu, shift, wei),

    fTauFromMu    (loaderNuTau,   axis, cut && kIsTauFromMu && !kIsAntiNu, shift, wei),
    fTauFromMuAnti(loaderNuTau,   axis, cut && kIsTauFromMu &&  kIsAntiNu, shift, wei),

    fNC           (loaderNonswap, axis, cut && kIsNC,                      shift, wei)
  {
    // All swapped files are equally valid as a source of NCs. This
    // approximately doubles/triples our statistics. SpectrumLoader just adds
    // events and POT for both cases, which is the right thing to do.
    loaderNue  .AddSpectrum(fNC, axis.GetMultiDVar(), cut && kIsNC, shift, wei);
    loaderNuTau.AddSpectrum(fNC, axis.GetMultiDVar(), cut && kIsNC, shift, wei);
  }


  //----------------------------------------------------------------------
  TrivialExtrap::TrivialExtrap(SpectrumLoaderBase& loaderNonswap,
                               SpectrumLoaderBase& loaderNue,
                               SpectrumLoaderBase& loaderNuTau,
                               std::string label,
                               const Binning& bins,
                               const Var& var,
                               const Cut& cut,
                               const SystShifts& shift,
                               const Var& wei)
    :
    TrivialExtrap(loaderNonswap, loaderNue, loaderNuTau,
                  HistAxis(label, bins, var),
                  cut, shift, wei)
  {
  }

  //----------------------------------------------------------------------
  TrivialExtrap::TrivialExtrap(Loaders& loaders,
                               std::string label,
                               const Binning& bins,
                               const Var& var,
                               const Cut& cut,
                               const SystShifts& shift,
                               const Var& wei)
    : TrivialExtrap(loaders, HistAxis(label, bins, var), cut, shift, wei)
  {
  }

  //----------------------------------------------------------------------
  TrivialExtrap::TrivialExtrap(Loaders& loaders,
                               const HistAxis& axis,
                               const Cut& cut,
                               const SystShifts& shift,
                               const Var& wei)
    : TrivialExtrap(loaders.GetLoader(caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNonSwap),
                    loaders.GetLoader(caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNueSwap),
                    loaders.GetLoader(caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNuTauSwap),
                    axis, cut, shift, wei)
  {
  }

  //----------------------------------------------------------------------
  void TrivialExtrap::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("TrivialExtrap").Write("type");

    fNueApp.SaveTo(dir, "nue_app");
    fNueAppAnti.SaveTo(dir, "nue_app_anti");
    fNC.SaveTo(dir, "nc");
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

    ret->fNC = *Spectrum::LoadFrom(dir, "nc");

    delete dir;

    return ret;
  }
}
