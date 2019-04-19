#include "CAFAna/Prediction/PredictionNoExtrap.h"

#include "CAFAna/Prediction/PredictionGenerator.h"

namespace ana {
class Loaders;

/// Prediction that just uses FD MC, with no extrapolation
class PredictionModifiedNoExtrap : public PredictionNoExtrap {
public:
  PredictionModifiedNoExtrap(Loaders &loaders, const HistAxis &axis,
                             const Cut &cut, const SystShifts &shift = kNoShift,
                             const Var &wei = kUnweighted);

  virtual Spectrum PredictComponent(osc::IOscCalculator *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

  virtual ~PredictionModifiedNoExtrap();

  static std::unique_ptr<PredictionModifiedNoExtrap> LoadFrom(TDirectory *dir);
    virtual void SaveTo(TDirectory* dir) const override;

  void SetExtraWeighting(TH1 *h) { fExtraWeight = h; }
  void UnsetExtraWeighting() { fExtraWeight = nullptr; }

protected:
  PredictionModifiedNoExtrap(PredictionExtrap *pred);

  TH1 *fExtraWeight;
};

class ModifiedNoExtrapPredictionGenerator : public IPredictionGenerator {
public:
  ModifiedNoExtrapPredictionGenerator(HistAxis axis, Cut cut,
                                      Var wei = kUnweighted)
      : fAxis(axis), fCut(cut), fWei(wei) {}

  virtual std::unique_ptr<IPrediction>
  Generate(Loaders &loaders,
           const SystShifts &shiftMC = kNoShift) const override {
    return std::unique_ptr<IPrediction>(
        new PredictionModifiedNoExtrap(loaders, fAxis, fCut, shiftMC, fWei));
  }

protected:
  HistAxis fAxis;
  Cut fCut;
  Var fWei;
};
} // namespace ana
