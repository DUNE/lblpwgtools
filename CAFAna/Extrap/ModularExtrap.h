#pragma once

#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Extrap/IExtrap.h"
#include "CAFAna/Extrap/ModularExtrapComponent.h"
#include <memory>

class TDirectory;

namespace ana
{
  class Loaders;
  class SpectrumLoader;
  class IDecomp;
  class OscillatableSpectrum;
  class FluxDecomp;

  // Extrapolate each component using a separate ModularExtrapComponent. This
  // class is not constructable directly, go via one of its dervied classes.
  class ModularExtrap: public IExtrap
  {

    public:
      // ModularExtrap owns objects on the heap through unique_ptr. We want to be able to transfer
      // ownership but not copy since this could lead to duplicate ownership of the same heap-allocated
      // objects.
      ModularExtrap(const ModularExtrap&) = delete;
      ModularExtrap& operator=(const ModularExtrap&) = delete;
      ModularExtrap(ModularExtrap&&) = default;
      ModularExtrap& operator=(ModularExtrap&&) = default;
      virtual ~ModularExtrap() = default;

      OscillatableSpectrum NueSurvComponent() override       {return fNueSurv->Return();};
      OscillatableSpectrum AntiNueSurvComponent() override   {return fNueSurvAnti->Return();}; 

      OscillatableSpectrum NumuSurvComponent() override      {return fNumuSurv->Return();};
      OscillatableSpectrum AntiNumuSurvComponent() override  {return fNumuSurvAnti->Return();};

      OscillatableSpectrum NueAppComponent() override        {return fNueApp->Return();};
      OscillatableSpectrum AntiNueAppComponent() override    {return fNueAppAnti->Return();};

      OscillatableSpectrum NumuAppComponent() override       {return fNumuApp->Return();};
      OscillatableSpectrum AntiNumuAppComponent() override   {return fNumuAppAnti->Return();};

      OscillatableSpectrum TauFromEComponent() override      {return fTauFromE->Return();};
      OscillatableSpectrum AntiTauFromEComponent() override  {return fTauFromEAnti->Return();};

      OscillatableSpectrum TauFromMuComponent() override     {return fTauFromMu->Return();};
      OscillatableSpectrum AntiTauFromMuComponent() override {return fTauFromMuAnti->Return();};

      Spectrum NCTotalComponent() override {return fNCTot->Return().Unoscillated();};
      Spectrum NCComponent() override      {return fNC->Return().Unoscillated();};
      Spectrum NCAntiComponent() override  {return fNCAnti->Return().Unoscillated();};

      void SaveTo(TDirectory* dir, const std::string& name) const override;

      static std::unique_ptr<ModularExtrap> LoadFrom(TDirectory* dir, const std::string& name);

      std::vector<ModularExtrapComponent*> GetModularExtrapComponents() const;

    protected:
      // Sets up all components to use FD MC, use a
      // derived class to create a ModularExtrap.
      ModularExtrap(IInteractionSource& farDetMCnonswapSrc,
                    IInteractionSource& farDetMCfluxswapSrc,
                    IInteractionSource& farDetMCtauswapSrc,
                    const HistAxis& axis,
                    const Cut& fdcut);

      std::unique_ptr<ModularExtrapComponent> fNueSurv;
      std::unique_ptr<ModularExtrapComponent> fNueSurvAnti;

      std::unique_ptr<ModularExtrapComponent> fNumuSurv;
      std::unique_ptr<ModularExtrapComponent> fNumuSurvAnti;

      std::unique_ptr<ModularExtrapComponent> fNueApp;
      std::unique_ptr<ModularExtrapComponent> fNueAppAnti;

      std::unique_ptr<ModularExtrapComponent> fNumuApp;
      std::unique_ptr<ModularExtrapComponent> fNumuAppAnti;

      std::unique_ptr<ModularExtrapComponent> fTauFromMu;
      std::unique_ptr<ModularExtrapComponent> fTauFromMuAnti;

      std::unique_ptr<ModularExtrapComponent> fTauFromE;
      std::unique_ptr<ModularExtrapComponent> fTauFromEAnti;

      std::unique_ptr<ModularExtrapComponent> fNCTot;
      std::unique_ptr<ModularExtrapComponent> fNC;
      std::unique_ptr<ModularExtrapComponent> fNCAnti;

    private:
      // Prevent user from default constructing.
      ModularExtrap(){};
  };
}
