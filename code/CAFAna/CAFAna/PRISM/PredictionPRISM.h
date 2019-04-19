#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Prediction/PredictionGenerator.h"
#include "CAFAna/Prediction/PredictionInterp.h"
#include "CAFAna/Prediction/PredictionModifiedNoExtrap.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Prediction/PredictionNoOsc.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/OscillatableSpectrum.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/PRISM/PRISMExtrapolator.h"

#include "TH3.h"

namespace ana {

/// Prediction that wraps a simple Spectrum
class PredictionPRISM : public IPrediction {
public:
  enum PRISMComponent {
    kNDData = (1 << 0),
    kNDDataCorr = (1 << 1),
    kNDDataCorr2D = (1 << 2),
    kNDSig = (1 << 3),
    kNDWSBkg = (1 << 4),
    kNDNCBkg = (1 << 5),
    kNDNueBkg = (1 << 6),
    kFDFluxCorr = (1 << 7),
    kFDNCBkg = (1 << 8),
    kFDWSBkg = (1 << 9)
  };

  static std::string GetComponentString(PRISMComponent pc) {
    switch (pc) {
    case kNDData: {
      return "kNDData";
    }
    case kNDDataCorr: {
      return "kNDDataCorr";
    }
    case kNDDataCorr2D: {
      return "kNDDataCorr2D";
    }
    case kNDSig: {
      return "kNDSig";
    }
    case kNDWSBkg: {
      return "kNDWSBkg";
    }
    case kNDNCBkg: {
      return "kNDNCBkg";
    }
    case kNDNueBkg: {
      return "kNDNueBkg";
    }
    case kFDFluxCorr: {
      return "kFDFluxCorr";
    }
    case kFDNCBkg: {
      return "kFDNCBkg";
    }
    case kFDWSBkg: {
      return "kFDWSBkg";
    }
    }
    return "";
  }

  // Use to instantiate an empty prediction.
  PredictionPRISM(const HistAxis &recoAxis, const HistAxis &offAxis);

  // Use to instantiate a data-like Prediction, where the ND 'data' is not
  // varied systematic shifts (but any corrections are)
  PredictionPRISM(SpectrumLoaderBase &, const HistAxis &recoAxis,
                  const HistAxis &offAxis, const Cut &cut,
                  const Var &wei = kUnweighted);

  void AddNDMCLoader(Loaders &, const Cut &cut = kNoCut,
                     const Var &wei = kUnweighted,
                     std::vector<ana::ISyst const *> systlist = {});

  void AddFDMCLoader(Loaders &, const Cut &cut = kNoCut,
                     const Var &wei = kUnweighted,
                     std::vector<ana::ISyst const *> systlist = {});

  static std::unique_ptr<PredictionPRISM> LoadFrom(TDirectory *dir);
  virtual void SaveTo(TDirectory *dir) const override;

  virtual Spectrum Predict(osc::IOscCalculator *calc) const override;
  virtual Spectrum PredictSyst(osc::IOscCalculator *calc,
                               ana::SystShifts shift = kNoShift) const;
  virtual std::map<PRISMComponent, Spectrum>
  PredictPRISMComponents(osc::IOscCalculator *calc,
                         ana::SystShifts shift = kNoShift,
                         bool AllComps = false) const;

  virtual Spectrum PredictComponent(osc::IOscCalculator *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

  void SetFluxMatcher(PRISMExtrapolator const *flux_matcher) {
    assert(flux_matcher->CheckOffAxisBinningConsistency(
        fOffAxis.GetBinnings().front()));
    fFluxMatcher = flux_matcher;
  }

protected:
  /// The 'data'
  std::unique_ptr<ReweightableSpectrum> fOffAxisData;
  bool fHaveData;

  // The 'MC Numu CC' -- Used for cheating when we don't fHaveNDBkgPred to
  // subtract
  std::unique_ptr<NoOscPredictionGenerator> fOffAxisPredGen;
  std::unique_ptr<PredictionInterp> fOffAxisPrediction;
  bool fHaveNDPred;

  // Use for background re-addition
  std::unique_ptr<ModifiedNoExtrapPredictionGenerator> fFDPredGen;
  mutable std::unique_ptr<PredictionInterp> fFarDetPrediction;
  bool fHaveFDPred;

  PRISMExtrapolator const *fFluxMatcher;

  PRISMExtrapolator::FluxPredSpecies fNDFluxSpecies;
  PRISMExtrapolator::FluxPredSpecies fFDFluxSpecies;

  HistAxis fPredictionAxis;
  HistAxis fOffAxis;
  std::unique_ptr<HistAxis> fOffPredictionAxis;
  mutable std::vector<double> fFDTrueEnergyBins;
};

} // namespace ana
