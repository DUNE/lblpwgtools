#pragma once

#include "CAFAna/Prediction/PredictionExtrap.h"

#include "CAFAna/Prediction/PredictionGenerator.h"

#include "CAFAna/Core/IRecordSource.h"

namespace ana {
class Loaders;

/// Prediction that just uses FD MC, with no extrapolation
class PredictionNoExtrap : public PredictionExtrap {
public:
  PredictionNoExtrap(PredictionExtrap *pred);
  PredictionNoExtrap(IExtrap *extrap);

  PredictionNoExtrap(IInteractionSource& loaderNonswap,
                     IInteractionSource& loaderNue,
                     IInteractionSource& loaderNuTau,
                     const HistAxis& axis);

  //  PredictionNoExtrap(Loaders &loaders, const HistAxis &axis);

  virtual ~PredictionNoExtrap();

  static std::unique_ptr<PredictionNoExtrap> LoadFrom(TDirectory *dir, const std::string& name);
  virtual void SaveTo(TDirectory *dir, const std::string& name) const override;
};

  /* TODO think about generators
class NoExtrapPredictionGenerator : public IPredictionGenerator {
public:
  NoExtrapPredictionGenerator(HistAxis axis, Cut cut, Weight wei = kUnweighted)
      : fAxis(axis), fCut(cut), fWei(wei) {
  }

  virtual std::unique_ptr<IPrediction>
  Generate(Loaders &loaders,
           const SystShifts &shiftMC = kNoShift) const override {
    return std::unique_ptr<IPrediction>(
        new PredictionNoExtrap(loaders, fAxis, fCut, shiftMC, fWei));
  }

protected:
  HistAxis fAxis;
  Cut fCut;
  Weight fWei;
};
  */

} // namespace ana
