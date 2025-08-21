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

  /// \brief Extrapolate each component using a separate ModularExtrapComponent
  ///
  /// This class is not constructable directly.  Use one of its subclasses.
  /// N.b.: only extrapolates one sample.
  class ModularExtrap: public IExtrap
  {

    public:

      // Prevent copying because we own objects on the free store.
      ModularExtrap(const ModularExtrap&) = delete;
      ModularExtrap& operator=(const ModularExtrap&) = delete;
      ModularExtrap(ModularExtrap&&) = default;
      ModularExtrap& operator=(ModularExtrap&&) = default;
      virtual ~ModularExtrap() = default;

      void SaveTo(TDirectory* dir, const std::string& name) const override;
      void SavePlotsNue( TDirectory* dir, double potFD ) const;
      void SavePlotsNueRHC( TDirectory* dir, double potFD ) const;
      void SavePlotsNumu( TDirectory* dir, double potFD ) const;
      static std::unique_ptr<ModularExtrap> LoadFrom(TDirectory* dir, const std::string& name);

      // Override abstract methods.
      OscillatableSpectrum NueSurvComponent()       override;
      OscillatableSpectrum AntiNueSurvComponent()   override;
      OscillatableSpectrum NumuSurvComponent()      override;
      OscillatableSpectrum AntiNumuSurvComponent()  override;
      OscillatableSpectrum NueAppComponent()        override;
      OscillatableSpectrum AntiNueAppComponent()    override;
      OscillatableSpectrum NumuAppComponent()       override;
      OscillatableSpectrum AntiNumuAppComponent()   override;
      OscillatableSpectrum TauFromMuComponent()     override;
      OscillatableSpectrum AntiTauFromMuComponent() override;
      OscillatableSpectrum TauFromEComponent()      override;
      OscillatableSpectrum AntiTauFromEComponent()  override;
      // nc:
      Spectrum             NCTotalComponent()       override;
      Spectrum             NCComponent()            override;
      Spectrum             NCAntiComponent()        override;
      //end nc

      std::vector<ModularExtrapComponent*> GetModExtrapComponents() const
      {
        return {
            fEEextrap.get(), fEEAntiextrap.get(),
            fMMextrap.get(), fMMAntiextrap.get(),
            fMEextrap.get(), fMEAntiextrap.get(),
            fEMextrap.get(), fEMAntiextrap.get(),
            //nc
            fNCTotalextrap.get(), fNCextrap.get(), fNCAntiextrap.get(),
            //end nc
            fMTextrap.get(), fMTAntiextrap.get(),
            fETextrap.get(), fETAntiextrap.get()
        };
      }

    protected:

      /// Sets up all components to use FD MC--internal use only.
      /// Use a derived class to create a ModularExtrap.
      /// This function is protected.
      ModularExtrap(
        SpectrumLoader& farMCswapLoader,
        SpectrumLoader& farMCnonswapLoader,
        SpectrumLoader& farMCtauswapLoader,
        const HistAxis& axis,
        const Cut& fdcut,
        ana::RecoType recoIxnType,
        const SystShifts& shiftMC,
        const Weight& weight
      );

      std::unique_ptr<ModularExtrapComponent> fEEextrap;
      std::unique_ptr<ModularExtrapComponent> fEEAntiextrap;
      std::unique_ptr<ModularExtrapComponent> fMMextrap;
      std::unique_ptr<ModularExtrapComponent> fMMAntiextrap;
      std::unique_ptr<ModularExtrapComponent> fMEextrap;
      std::unique_ptr<ModularExtrapComponent> fMEAntiextrap;
      std::unique_ptr<ModularExtrapComponent> fEMextrap;
      std::unique_ptr<ModularExtrapComponent> fEMAntiextrap;
      //nc
      std::unique_ptr<ModularExtrapComponent> fNCTotalextrap;
      std::unique_ptr<ModularExtrapComponent> fNCextrap;
      std::unique_ptr<ModularExtrapComponent> fNCAntiextrap;
      //end nc
      std::unique_ptr<ModularExtrapComponent> fMTextrap;
      std::unique_ptr<ModularExtrapComponent> fMTAntiextrap;
      std::unique_ptr<ModularExtrapComponent> fETextrap;
      std::unique_ptr<ModularExtrapComponent> fETAntiextrap;

    private:

      ModularExtrap(){};

  };

}
