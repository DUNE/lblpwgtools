#pragma once

#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Prediction/PredictionGenerator.h"

#include "CAFAna/Core/Loaders.h"

namespace ana
{
  /// Prediction that wraps a simple Spectrum
  class PredictionNoOsc: public IPrediction
  {
  public:
    PredictionNoOsc(SpectrumLoaderBase& loader,
                    const HistAxis& axis,
                    const Cut& cut,
                    const SystShifts& shift = kNoShift,
                    const Var& wei = kUnweighted);

    PredictionNoOsc(SpectrumLoaderBase& loader,
                    const std::string& label,
                    const Binning& bins,
                    const Var& var,
                    const Cut& cut,
                    const SystShifts& shift = kNoShift,
                    const Var& wei = kUnweighted);

    static std::unique_ptr<PredictionNoOsc> LoadFrom(TDirectory* dir);
    virtual void SaveTo(TDirectory* dir) const override;

    virtual Spectrum Predict(osc::IOscCalculator* /*calc*/) const override
    {
      return fSpectrum;
    }

    void OverridePOT(double pot){
      fSpectrum.OverridePOT(1);
      fSpectrumNC.OverridePOT(1);
      fSpectrumNumu.OverridePOT(1);
      fSpectrumNumubar.OverridePOT(1);
      fSpectrumNue.OverridePOT(1);
      fSpectrumNuebar.OverridePOT(1);
    }

    virtual Spectrum PredictComponent(osc::IOscCalculator* calc,
                                      Flavors::Flavors_t flav,
                                      Current::Current_t curr,
                                      Sign::Sign_t sign) const override;

  protected:
    PredictionNoOsc(const Spectrum& s,
                    const Spectrum& sNC,
                    const Spectrum& sNumu, const Spectrum& sNumubar,
                    const Spectrum& sNue, const Spectrum& sNuebar)
      : fSpectrum(s),
        fSpectrumNC(sNC),
        fSpectrumNumu(sNumu), fSpectrumNumubar(sNumubar),
        fSpectrumNue(sNue), fSpectrumNuebar(sNuebar)
    {
    }

    Spectrum fSpectrum;

    Spectrum fSpectrumNC;
    Spectrum fSpectrumNumu;
    Spectrum fSpectrumNumubar;
    Spectrum fSpectrumNue;
    Spectrum fSpectrumNuebar;
  };


  class NoOscPredictionGenerator: public IPredictionGenerator
  {
  public:
    NoOscPredictionGenerator(HistAxis axis,
                             Cut cut,
			     Var wei = kUnweighted)
      : fAxis(axis), fCut(cut), fWei(wei)
    {
    }

    virtual std::unique_ptr<IPrediction>
    Generate(Loaders& loaders, const SystShifts& shiftMC = kNoShift) const override
    {
      SpectrumLoaderBase& loader = loaders.GetLoader(caf::kNEARDET,
                                                     Loaders::kMC);
      return std::unique_ptr<IPrediction>(new PredictionNoOsc(loader,
                                                              fAxis,
                                                              fCut,
                                                              shiftMC, fWei));
    }
  protected:
    HistAxis fAxis;
    Cut fCut;
    Var fWei;
  };

}
