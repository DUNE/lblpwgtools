#pragma once

#include "CAFAna/Prediction/PredictionNoExtrap.h"

#include "CAFAna/Prediction/PredictionGenerator.h"

namespace ana {
class Loaders;

///\brief Prediction that oscillated FD MC along with an extra weighting
/// histogram
///
/// Useful for adding FD MC corrections which are described as a function of
/// true energy. e.g. The PRISM 'flux matching' correction needs to add in FD MC
/// to correct the extrapolated ND data as a function of the true flux energy
/// spectrum.
class PredictionEnuWeightedNoExtrap : public PredictionNoExtrap {
public:
  PredictionEnuWeightedNoExtrap(Loaders &loaders, const HistAxis &axis,
                                const Cut &cut,
                                const SystShifts &shift = kNoShift,
                                const Var &wei = kUnweighted);

  virtual Spectrum PredictComponent(osc::IOscCalculator *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

  static std::unique_ptr<PredictionEnuWeightedNoExtrap>
  LoadFrom(TDirectory *dir);
  virtual void SaveTo(TDirectory *dir) const override;

  void SetEnuWeighting(TH1 const *h) { fEnuWeight = h; }
  void UnSetEnuWeighting() { fEnuWeight = nullptr; }

protected:
  PredictionEnuWeightedNoExtrap(PredictionExtrap *pred);

  TH1 const *fEnuWeight;
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
        new PredictionEnuWeightedNoExtrap(loaders, fAxis, fCut, shiftMC, fWei));
  }

protected:
  HistAxis fAxis;
  Cut fCut;
  Var fWei;
};
} // namespace ana
