#pragma once

#include "CAFAna/Extrap/IExtrap.h"

#include "CAFAna/Core/IRecordSource.h"

namespace ana
{
  class Loaders;

  /// "Extrapolation" that simply returns the FD MC prediction
  class TrivialExtrap: public IExtrap
  {
  public:
    TrivialExtrap(IRecordSource& nonswapSrc,
                  IRecordSource& nueSrc,
                  IRecordSource& tauSrc,
                  const HistAxis& axis);

    //    TrivialExtrap(SliceSources& srcs, const HistAxis& axis);

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
