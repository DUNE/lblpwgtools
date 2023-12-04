#pragma once

#include "CAFAna/Extrap/IExtrap.h"

namespace ana
{
  class Loaders;

  /// "Extrapolation" that simply returns the FD MC prediction
  class TrivialExtrap: public IExtrap
  {
  public:
    // This is the DUNE constructor
    TrivialExtrap(SpectrumLoaderBase& loaderNonswap,
                  SpectrumLoaderBase& loaderNue,
                  SpectrumLoaderBase& loaderNuTau,
                  const HistAxis& axis,
                  const Cut& cut,
                  const SystShifts& shift,
                  const Weight& wei);

    TrivialExtrap(SpectrumLoaderBase& loaderNonswap,
                  SpectrumLoaderBase& loaderNue,
                  SpectrumLoaderBase& loaderNuTau,
                  std::string label,
                  const Binning& bins,
                  const Var& var,
                  const Cut& cut,
                  const SystShifts& shift,
                  const Weight& wei);

    TrivialExtrap(Loaders& loaders,
                  std::string label,
                  const Binning& bins,
                  const Var& var,
                  const Cut& cut,
                  const SystShifts& shift = kNoShift,
                  const Weight& wei = kUnweighted);

    TrivialExtrap(Loaders& loaders,
                  const HistAxis& axis,
                  const Cut& cut,
                  const SystShifts& shift = kNoShift,
                  const Weight& wei = kUnweighted);

    virtual OscillatableSpectrum NueSurvComponent() override       {return fNueSurv;}
    virtual OscillatableSpectrum AntiNueSurvComponent() override   {return fNueSurvAnti;}

    virtual OscillatableSpectrum NumuSurvComponent() override      {return fNumuSurv;}
    virtual OscillatableSpectrum AntiNumuSurvComponent() override  {return fNumuSurvAnti;}

    virtual OscillatableSpectrum NueAppComponent() override        {return fNueApp;}
    virtual OscillatableSpectrum AntiNueAppComponent() override    {return fNueAppAnti;}

    virtual OscillatableSpectrum NumuAppComponent() override       {return fNumuApp;}
    virtual OscillatableSpectrum AntiNumuAppComponent() override   {return fNumuAppAnti;}

    virtual OscillatableSpectrum TauFromEComponent() override      {return fTauFromE;}
    virtual OscillatableSpectrum AntiTauFromEComponent() override  {return fTauFromEAnti;}

    virtual OscillatableSpectrum TauFromMuComponent() override     {return fTauFromMu;}
    virtual OscillatableSpectrum AntiTauFromMuComponent() override {return fTauFromMuAnti;}

    virtual Spectrum NCTotalComponent() override {return fNCTot;}
    virtual Spectrum NCComponent()  override{return fNC;}
    virtual Spectrum NCAntiComponent()  override {return fNCAnti;}

    virtual void SaveTo(TDirectory* dir, const std::string& name) const override;
    static std::unique_ptr<TrivialExtrap> LoadFrom(TDirectory* dir, const std::string& name);

  protected:
    TrivialExtrap()
      : fNueApp    (OscillatableSpectrum::Uninitialized()),
        fNueAppAnti(OscillatableSpectrum::Uninitialized()),
        fNumuSurv    (OscillatableSpectrum::Uninitialized()),
        fNumuSurvAnti(OscillatableSpectrum::Uninitialized()),
        fNumuApp    (OscillatableSpectrum::Uninitialized()),
        fNumuAppAnti(OscillatableSpectrum::Uninitialized()),
        fNueSurv    (OscillatableSpectrum::Uninitialized()),
        fNueSurvAnti(OscillatableSpectrum::Uninitialized()),
        fTauFromE    (OscillatableSpectrum::Uninitialized()),
        fTauFromEAnti(OscillatableSpectrum::Uninitialized()),
        fTauFromMu    (OscillatableSpectrum::Uninitialized()),
        fTauFromMuAnti(OscillatableSpectrum::Uninitialized()),
        fNCTot(Spectrum::Uninitialized()),
        fNC(Spectrum::Uninitialized()),
        fNCAnti(Spectrum::Uninitialized())
    {}

    OscillatableSpectrum fNueApp,    fNueAppAnti;
    OscillatableSpectrum fNumuSurv,  fNumuSurvAnti;
    OscillatableSpectrum fNumuApp,   fNumuAppAnti;
    OscillatableSpectrum fNueSurv,   fNueSurvAnti;
    OscillatableSpectrum fTauFromE,  fTauFromEAnti;
    OscillatableSpectrum fTauFromMu, fTauFromMuAnti;
    Spectrum fNCTot;
    Spectrum fNC, fNCAnti;
  };
}
