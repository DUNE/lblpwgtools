#pragma once

#include "CAFAna/Extrap/IExtrap.h"

namespace ana
{
  class Loaders;

  class DUNERunPOTSpectrumLoader;

  /// "Extrapolation" that simply returns the FD MC prediction
  class TrivialExtrap: public IExtrap
  {
  public:
    // This is the DUNE constructor
    TrivialExtrap(DUNERunPOTSpectrumLoader& loaderBeam,
                  DUNERunPOTSpectrumLoader& loaderNue,
                  DUNERunPOTSpectrumLoader& loaderNuTau,
                  DUNERunPOTSpectrumLoader& loaderNC,
                  const HistAxis& axis,
                  const Cut& cut,
                  const SystShifts& shift,
                  const Var& wei);

    TrivialExtrap(DUNERunPOTSpectrumLoader& loaderBeam,
                  DUNERunPOTSpectrumLoader& loaderNue,
                  DUNERunPOTSpectrumLoader& loaderNuTau,
                  DUNERunPOTSpectrumLoader& loaderNC,
                  std::string label,
                  const Binning& bins,
                  const Var& var,
                  const Cut& cut,
                  const SystShifts& shift,
                  const Var& wei);

    TrivialExtrap(Loaders& loaders,
                  std::string label,
                  const Binning& bins,
                  const Var& var,
                  const Cut& cut,
                  const SystShifts& shift = kNoShift,
                  const Var& wei = kUnweighted);

    TrivialExtrap(Loaders& loaders,
                  const HistAxis& axis,
                  const Cut& cut,
                  const SystShifts& shift = kNoShift,
                  const Var& wei = kUnweighted);

    TrivialExtrap(SpectrumLoaderBase& loader,
                  SpectrumLoaderBase& loaderNue,
                  SpectrumLoaderBase& loaderTau,
                  SpectrumLoaderBase& loaderNC,
                  const HistAxis& axis,
                  const Cut& cut,
                  const SystShifts& shift,
                  const Var& wei);

    TrivialExtrap(SpectrumLoaderBase& loader,
                  SpectrumLoaderBase& loaderSwap,
                  SpectrumLoaderBase& loaderTau,
                  std::string label,
                  const Binning& bins,
                  const Var& var,
                  const Cut& cut,
                  const SystShifts& shift = kNoShift,
                  const Var& wei = kUnweighted);

    TrivialExtrap(SpectrumLoaderBase& loader,
                  SpectrumLoaderBase& loaderNue,
                  SpectrumLoaderBase& loaderTau,
                  SpectrumLoaderBase& loaderNC,
                  std::string label,
                  const Binning& bins,
                  const Var& var,
                  const Cut& cut,
                  const SystShifts& shift = kNoShift,
                  const Var& wei = kUnweighted);

    TrivialExtrap(SpectrumLoaderBase& loader,
                  SpectrumLoaderBase& loaderSwap,
                  std::string label,
                  const Binning& bins,
                  const Var& var,
                  const Cut& cut,
                  const SystShifts& shift = kNoShift,
                  const Var& wei = kUnweighted);

    virtual const OscillatableSpectrum & NueSurvComponent()       {return fNueSurv;}
    virtual const OscillatableSpectrum & AntiNueSurvComponent()   {return fNueSurvAnti;}

    virtual const OscillatableSpectrum & NumuSurvComponent()      {return fNumuSurv;}
    virtual const OscillatableSpectrum & AntiNumuSurvComponent()  {return fNumuSurvAnti;}

    virtual const OscillatableSpectrum & NueAppComponent()        {return fNueApp;}
    virtual const OscillatableSpectrum & AntiNueAppComponent()    {return fNueAppAnti;}

    virtual const OscillatableSpectrum & NumuAppComponent()       {return fNumuApp;}
    virtual const OscillatableSpectrum & AntiNumuAppComponent()   {return fNumuAppAnti;}

    virtual const OscillatableSpectrum & TauFromEComponent()      {return fTauFromE;}
    virtual const OscillatableSpectrum & AntiTauFromEComponent()  {return fTauFromEAnti;}

    virtual const OscillatableSpectrum & TauFromMuComponent()     {return fTauFromMu;}
    virtual const OscillatableSpectrum & AntiTauFromMuComponent() {return fTauFromMuAnti;}

    virtual const Spectrum& NCComponent() {return fNC;}

    virtual void SaveTo(TDirectory* dir) const;
    static std::unique_ptr<TrivialExtrap> LoadFrom(TDirectory* dir);

  protected:
    TrivialExtrap()
      : fNueApp(0, {}, {}, 0, 0),    fNueAppAnti(0, {}, {}, 0, 0),
        fNumuSurv(0, {}, {}, 0, 0),  fNumuSurvAnti(0, {}, {}, 0, 0),
        fNumuApp(0, {}, {}, 0, 0),   fNumuAppAnti(0, {}, {}, 0, 0),
        fNueSurv(0, {}, {}, 0, 0),   fNueSurvAnti(0, {}, {}, 0, 0),
        fTauFromE(0, {}, {}, 0, 0),  fTauFromEAnti(0, {}, {}, 0, 0),
        fTauFromMu(0, {}, {}, 0, 0), fTauFromMuAnti(0, {}, {}, 0, 0),
        fNC(0, {}, {}, 0, 0)
    {}

    OscillatableSpectrum fNueApp,    fNueAppAnti;
    OscillatableSpectrum fNumuSurv,  fNumuSurvAnti;
    OscillatableSpectrum fNumuApp,   fNumuAppAnti;
    OscillatableSpectrum fNueSurv,   fNueSurvAnti;
    OscillatableSpectrum fTauFromE,  fTauFromEAnti;
    OscillatableSpectrum fTauFromMu, fTauFromMuAnti;
    Spectrum fNC;
  };
}
