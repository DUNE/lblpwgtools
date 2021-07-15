#pragma once

#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Prediction/PredictionGenerator.h"

#include "CAFAna/Core/Loaders.h"

namespace ana {
/// Prediction that wraps a simple Spectrum
class PredictionNoOsc : public IPrediction {
public:
  PredictionNoOsc(SpectrumLoaderBase &loader, const HistAxis &axis,
                  const Cut &cut, const SystShifts &shift = kNoShift,
                  const Weight &wei = kUnweighted);

  PredictionNoOsc(SpectrumLoaderBase &loader, const std::string &label,
                  const Binning &bins, const Var &var, const Cut &cut,
                  const SystShifts &shift = kNoShift,
                  const Weight &wei = kUnweighted);

  static std::unique_ptr<PredictionNoOsc> LoadFrom(TDirectory *dir, const std::string& name);
  virtual void SaveTo(TDirectory *dir, const std::string& name) const override;

  virtual Spectrum Predict(osc::IOscCalc * /*calc*/) const override {
    return fSpectrum;
  }

  virtual Spectrum Predict(osc::IOscCalcStan* /*calc*/) const override {
    return fSpectrum;
  }

  virtual Spectrum PredictComponent(osc::IOscCalc *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

  virtual Spectrum PredictComponent(osc::IOscCalcStan* /*calc*/,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override
  {
    return PredictComponent((osc::IOscCalc*)0, flav, curr, sign);
  }

protected:
  PredictionNoOsc(const Spectrum &s, const Spectrum &sNC, const Spectrum &sNumu,
                  const Spectrum &sNumubar, const Spectrum &sNue,
                  const Spectrum &sNuebar)
      : fSpectrum(s), fSpectrumNC(sNC), fSpectrumNumu(sNumu),
        fSpectrumNumubar(sNumubar), fSpectrumNue(sNue),
        fSpectrumNuebar(sNuebar) {}

  Spectrum fSpectrum;

  Spectrum fSpectrumNC;
  Spectrum fSpectrumNumu;
  Spectrum fSpectrumNumubar;
  Spectrum fSpectrumNue;
  Spectrum fSpectrumNuebar;
};

class NoOscPredictionGenerator : public IPredictionGenerator {
public:
  NoOscPredictionGenerator(HistAxis axis, Cut cut, Weight wei = kUnweighted)
      : fAxis(axis), fCut(cut), fWei(wei) {
  }

  virtual std::unique_ptr<IPrediction>
  Generate(Loaders &loaders,
           const SystShifts &shiftMC = kNoShift) const override {
    SpectrumLoaderBase &loader = loaders.GetLoader(caf::kNEARDET, Loaders::kMC);
    return std::unique_ptr<IPrediction>(
        new PredictionNoOsc(loader, fAxis, fCut, shiftMC, fWei));
  }

protected:
  HistAxis fAxis;
  Cut fCut;
  Weight fWei;
};

} // namespace ana
