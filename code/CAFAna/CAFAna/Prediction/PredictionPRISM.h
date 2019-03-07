#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Prediction/PredictionGenerator.h"

#include "CAFAna/Core/Loaders.h"

#include "CAFAna/Analysis/PRISMFluxMatcher.h"

namespace ana {
/// Prediction that wraps a simple Spectrum
class PredictionPRISM : public IPrediction {
public:
  PredictionPRISM(SpectrumLoaderBase &loader, const HistAxis &recoAxis,
                  const HistAxis &offAxis, const Cut &cut,
                  const SystShifts &shift = kNoShift,
                  const Var &wei = kUnweighted,
                  PRISMFluxMatcher *flux_matcher = nullptr,
                  PRISMFluxMatcher::FluxPredSpecies NDFluxSpecies =
                      PRISMFluxMatcher::FluxPredSpecies::kNumu_numode,
                  PRISMFluxMatcher::FluxPredSpecies FDFluxSpecies =
                      PRISMFluxMatcher::FluxPredSpecies::kNumu_numode);

  static std::unique_ptr<PredictionPRISM>
  LoadFrom(TDirectory *dir, PRISMFluxMatcher *flux_matcher = nullptr,
           PRISMFluxMatcher::FluxPredSpecies NDFluxSpecies =
               PRISMFluxMatcher::FluxPredSpecies::kNumu_numode,
           PRISMFluxMatcher::FluxPredSpecies FDFluxSpecies =
               PRISMFluxMatcher::FluxPredSpecies::kNumu_numode);
  virtual void SaveTo(TDirectory *dir) const override;

  virtual Spectrum Predict(osc::IOscCalculator *calc) const override;

  virtual Spectrum PredictComponent(osc::IOscCalculator *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

protected:
  PredictionPRISM(const ReweightableSpectrum &s,
                  const ReweightableSpectrum &sNC,
                  const ReweightableSpectrum &sNumu,
                  const ReweightableSpectrum &sNumubar,
                  const ReweightableSpectrum &sNue,
                  const ReweightableSpectrum &sNuebar,
                  PRISMFluxMatcher *flux_matcher = nullptr,
                  PRISMFluxMatcher::FluxPredSpecies NDFluxSpecies =
                      PRISMFluxMatcher::FluxPredSpecies::kNumu_numode,
                  PRISMFluxMatcher::FluxPredSpecies FDFluxSpecies =
                      PRISMFluxMatcher::FluxPredSpecies::kNumu_numode)
      : fOffAxisSpectrum(s), fOffAxisSpectrumNC(sNC),
        fOffAxisSpectrumNumu(sNumu), fOffAxisSpectrumNumubar(sNumubar),
        fOffAxisSpectrumNue(sNue), fOffAxisSpectrumNuebar(sNuebar),
        fFluxMatcher(flux_matcher), fNDFluxSpecies(NDFluxSpecies),
        fFDFluxSpecies(FDFluxSpecies) {}

  TH1 *fOffAxisWeighter;
  ReweightableSpectrum fOffAxisSpectrum;

  ReweightableSpectrum fOffAxisSpectrumNC;
  ReweightableSpectrum fOffAxisSpectrumNumu;
  ReweightableSpectrum fOffAxisSpectrumNumubar;
  ReweightableSpectrum fOffAxisSpectrumNue;
  ReweightableSpectrum fOffAxisSpectrumNuebar;

  // Add unselected predictions for eff correction

  PRISMFluxMatcher *fFluxMatcher;

  PRISMFluxMatcher::FluxPredSpecies fNDFluxSpecies;
  PRISMFluxMatcher::FluxPredSpecies fFDFluxSpecies;
};

class PredictionPRISMGenerator : public IPredictionGenerator {
public:
  PredictionPRISMGenerator(const HistAxis &recoAxis, const HistAxis &offAxis,
                           const Cut &cut, const Var &wei = kUnweighted,
                           PRISMFluxMatcher *flux_matcher = nullptr,
                           PRISMFluxMatcher::FluxPredSpecies NDFluxSpecies =
                               PRISMFluxMatcher::FluxPredSpecies::kNumu_numode,
                           PRISMFluxMatcher::FluxPredSpecies FDFluxSpecies =
                               PRISMFluxMatcher::FluxPredSpecies::kNumu_numode)
      : fPredictionAxis(recoAxis), fOffAxis(offAxis), fCut(cut), fWei(wei),
        fFluxMatcher(flux_matcher), fNDFluxSpecies(NDFluxSpecies),
        fFDFluxSpecies(FDFluxSpecies) {}

  virtual std::unique_ptr<IPrediction>
  Generate(Loaders &loaders,
           const SystShifts &shiftMC = kNoShift) const override {

    SpectrumLoaderBase &loader = loaders.GetLoader(caf::kNEARDET, Loaders::kMC);

    return std::unique_ptr<IPrediction>(new PredictionPRISM(
        loader, fPredictionAxis, fOffAxis, fCut, shiftMC, fWei, fFluxMatcher,
        fNDFluxSpecies, fFDFluxSpecies));
  }

protected:
  HistAxis fPredictionAxis;
  HistAxis fOffAxis;
  Cut fCut;
  Var fWei;

  PRISMFluxMatcher *fFluxMatcher;
  PRISMFluxMatcher::FluxPredSpecies fNDFluxSpecies;
  PRISMFluxMatcher::FluxPredSpecies fFDFluxSpecies;
};

} // namespace ana
