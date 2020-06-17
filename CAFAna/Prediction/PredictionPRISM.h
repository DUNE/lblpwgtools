#pragma once

#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Prediction/PredictionGenerator.h"
#include "CAFAna/Prediction/PredictionInterp.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/OscillatableSpectrum.h"

#include "CAFAna/Analysis/PRISMExtrapolator.h"

#include "TH3.h"

namespace ana {
/// Prediction that wraps a simple Spectrum
class PredictionPRISM : public IPrediction {
public:
  PredictionPRISM(SpectrumLoaderBase &ND_loader, const HistAxis &recoAxis,
                  const HistAxis &offAxis, const Cut &cut,
                  const SystShifts &shift = kNoShift,
                  const Var &wei = kUnweighted,
                  PRISMExtrapolator const *flux_matcher = nullptr,
                  PRISMExtrapolator::FluxPredSpecies NDFluxSpecies =
                      PRISMExtrapolator::FluxPredSpecies::kNumu_numode,
                  PRISMExtrapolator::FluxPredSpecies FDFluxSpecies =
                      PRISMExtrapolator::FluxPredSpecies::kNumu_numode,
                  TH3 const *SelectedNumuCC = nullptr,
                  TH3 const *AllNumuCC = nullptr);

  void AddFDMCLoader(SpectrumLoaderBase &FD_loader, const Cut &cut,
                     const SystShifts &shift = kNoShift,
                     const Var &wei = kUnweighted);

  void AddNDMCLoader(SpectrumLoaderBase &ND_loader, const Cut &cut,
                     const SystShifts &shift = kNoShift,
                     const Var &wei = kUnweighted);

  static std::unique_ptr<PredictionPRISM>
  LoadFrom(TDirectory *dir, PRISMExtrapolator const *flux_matcher = nullptr,
           PRISMExtrapolator::FluxPredSpecies NDFluxSpecies =
               PRISMExtrapolator::FluxPredSpecies::kNumu_numode,
           PRISMExtrapolator::FluxPredSpecies FDFluxSpecies =
               PRISMExtrapolator::FluxPredSpecies::kNumu_numode);
  virtual void SaveTo(TDirectory *dir, const std::string& name) const override;

  virtual Spectrum Predict(osc::IOscCalculator *calc) const override;

  virtual Spectrum PredictComponent(osc::IOscCalculator *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

protected:
  PredictionPRISM(std::unique_ptr<ReweightableSpectrum> &&spec,
                  std::unique_ptr<ReweightableSpectrum> &&specNumu,
                  std::unique_ptr<ReweightableSpectrum> &&specNumubar,
                  PRISMExtrapolator const *flux_matcher = nullptr,
                  PRISMExtrapolator::FluxPredSpecies NDFluxSpecies =
                      PRISMExtrapolator::FluxPredSpecies::kNumu_numode,
                  PRISMExtrapolator::FluxPredSpecies FDFluxSpecies =
                      PRISMExtrapolator::FluxPredSpecies::kNumu_numode)
      : fPredictionAxis({}, {}, {}), fOffAxis({}, {}, {}) {

    fOffAxisSpectrum = std::move(spec);
    fOffAxisSpectrumNumu = std::move(specNumu);
    fOffAxisSpectrumNumu = std::move(specNumubar);
    fNDBkg = NDBackgroundSpectra{nullptr, nullptr, nullptr};
    fHaveNDBkgPred = false;
    fFarDetSpectrumNC = nullptr;
    fFarDetSpectrumNumu = nullptr;
    fFarDetSpectrumNumubar = nullptr;
    fHaveFDPred = false;
    fFluxMatcher = flux_matcher;
    fNDFluxSpecies = NDFluxSpecies;
    fFDFluxSpecies = FDFluxSpecies;
  }

  void
  SetNDBkgPredicitions(std::unique_ptr<ReweightableSpectrum> &&specNC,
                       std::unique_ptr<ReweightableSpectrum> &&specNue,
                       std::unique_ptr<ReweightableSpectrum> &&specNuebar) {
    fNDBkg.fOffAxisSpectrumNC = std::move(specNC);
    fNDBkg.fOffAxisSpectrumNue = std::move(specNue);
    fNDBkg.fOffAxisSpectrumNuebar = std::move(specNuebar);
    fHaveNDBkgPred = true;
  }

  void
  SetFDPredicitions(std::unique_ptr<Spectrum> &&specFDNC,
                    std::unique_ptr<OscillatableSpectrum> &&specFDNumu,
                    std::unique_ptr<OscillatableSpectrum> &&specFDNumubar) {
    fFarDetSpectrumNC = std::move(specFDNC);
    fFarDetSpectrumNumu = std::move(specFDNumu);
    fFarDetSpectrumNumubar = std::move(specFDNumubar);
    fHaveFDPred = true;
  }

  /// The 'data'
  std::unique_ptr<ReweightableSpectrum> fOffAxisSpectrum;

  // The 'MC Numu CC' -- Used for cheating when we don't fHaveNDBkgPred to
  // subtract
  std::unique_ptr<ReweightableSpectrum> fOffAxisSpectrumNumu;
  std::unique_ptr<ReweightableSpectrum> fOffAxisSpectrumNumubar;

  // Use for background subtraction
  struct NDBackgroundSpectra {
    std::unique_ptr<ReweightableSpectrum> fOffAxisSpectrumNC;
    std::unique_ptr<ReweightableSpectrum> fOffAxisSpectrumNue;
    std::unique_ptr<ReweightableSpectrum> fOffAxisSpectrumNuebar;
  };
  mutable NDBackgroundSpectra fNDBkg;
  bool fHaveNDBkgPred;

  // Use for background re-addition
  mutable std::unique_ptr<Spectrum> fFarDetSpectrumNC;

  // Use for flux correction & WSB
  mutable std::unique_ptr<OscillatableSpectrum> fFarDetSpectrumNumu;
  mutable std::unique_ptr<OscillatableSpectrum> fFarDetSpectrumNumubar;
  bool fHaveFDPred;

  PRISMExtrapolator const *fFluxMatcher;

  PRISMExtrapolator::FluxPredSpecies fNDFluxSpecies;
  PRISMExtrapolator::FluxPredSpecies fFDFluxSpecies;

  HistAxis fPredictionAxis;
  HistAxis fOffAxis;
};

class PredictionPRISMGenerator : public IPredictionGenerator {
public:
  PredictionPRISMGenerator(const HistAxis &recoAxis, const HistAxis &offAxis,
                           const Cut &cutND, const Cut &cutFD,
                           const Var &weiND = kUnweighted,
                           const Var &weiFD = kUnweighted,
                           PRISMExtrapolator const *flux_matcher = nullptr,
                           PRISMExtrapolator::FluxPredSpecies NDFluxSpecies =
                               PRISMExtrapolator::FluxPredSpecies::kNumu_numode,
                           PRISMExtrapolator::FluxPredSpecies FDFluxSpecies =
                               PRISMExtrapolator::FluxPredSpecies::kNumu_numode)
      : fPredictionAxis(recoAxis), fOffAxis(offAxis), fCutND(cutND),
        fWeiND(weiND), fCutFD(cutFD), fWeiFD(weiFD), fFluxMatcher(flux_matcher),
        fNDFluxSpecies(NDFluxSpecies), fFDFluxSpecies(FDFluxSpecies),
        fSelectedNumuCCInterp(nullptr), fAllNumuCCInterp(nullptr) {}

  void SetMCEfficiencyPredictors(PredictionInterp const *SelectedNumuCCInterp,
                                 PredictionInterp const *AllNumuCCInterp) {
    fSelectedNumuCCInterp = SelectedNumuCCInterp;
    fAllNumuCCInterp = AllNumuCCInterp;
  }

  virtual std::unique_ptr<IPrediction>
  Generate(Loaders &loaders,
           const SystShifts &shiftMC = kNoShift) const override {

    // When we have data, this becomes data.
    SpectrumLoaderBase &loader_ND =
        loaders.GetLoader(caf::kNEARDET, Loaders::kMC);

    bool HaveEff = (fSelectedNumuCCInterp && fAllNumuCCInterp);

    if (HaveEff) {
      SelectedNumuCCPredCache.emplace_back(
          fSelectedNumuCCInterp->PredictSyst(nullptr, shiftMC).ToTH3(1));
      AllNumuCCPredCache.emplace_back(
          fAllNumuCCInterp->PredictSyst(nullptr, shiftMC).ToTH3(1));
    }

    std::unique_ptr<PredictionPRISM> ppred = std::make_unique<PredictionPRISM>(
        loader_ND, fPredictionAxis, fOffAxis, fCutND, shiftMC, fWeiND,
        fFluxMatcher, fNDFluxSpecies, fFDFluxSpecies,
        HaveEff ? SelectedNumuCCPredCache.back().get() : nullptr,
        HaveEff ? AllNumuCCPredCache.back().get() : nullptr);

    SpectrumLoaderBase &loader_ND_corr =
        loaders.GetLoader(caf::kNEARDET, Loaders::kMC);
    ppred->AddNDMCLoader(loader_ND_corr, fCutND, shiftMC, fWeiND);

    SpectrumLoaderBase &loader_FD_corr =
        loaders.GetLoader(caf::kFARDET, Loaders::kMC);
    ppred->AddFDMCLoader(loader_FD_corr, fCutFD, shiftMC, fWeiFD);

    return ppred;
  }

protected:
  HistAxis fPredictionAxis;
  HistAxis fOffAxis;
  Cut fCutND;
  Var fWeiND;

  Cut fCutFD;
  Var fWeiFD;

  PRISMExtrapolator const *fFluxMatcher;
  PRISMExtrapolator::FluxPredSpecies fNDFluxSpecies;
  PRISMExtrapolator::FluxPredSpecies fFDFluxSpecies;

  bool fSubtractBackgrounds;
  bool fHaveFDPrediction;

  PredictionInterp const *fSelectedNumuCCInterp;
  PredictionInterp const *fAllNumuCCInterp;

  mutable std::vector<std::unique_ptr<TH3>> SelectedNumuCCPredCache;
  mutable std::vector<std::unique_ptr<TH3>> AllNumuCCPredCache;
};

} // namespace ana
