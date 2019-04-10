#pragma once

#include "CAFAna/Prediction/PredictionExtrap.h"

#include "CAFAna/Prediction/PredictionGenerator.h"

namespace ana {
class Loaders;

/// Prediction that just uses FD MC, with no extrapolation
class PredictionNoExtrap : public PredictionExtrap {
public:
  PredictionNoExtrap(PredictionExtrap *pred);

  // This is the DUNE constructor
  PredictionNoExtrap(SpectrumLoaderBase &loaderNonswap,
                     SpectrumLoaderBase &loaderNue,
                     SpectrumLoaderBase &loaderNuTau, const std::string &label,
                     const Binning &bins, const Var &var, const Cut &cut,
                     const SystShifts &shift = kNoShift,
                     const Weight &wei = kUnweighted);

  PredictionNoExtrap(SpectrumLoaderBase &loaderNonswap,
                     SpectrumLoaderBase &loaderNue,
                     SpectrumLoaderBase &loaderNuTau, const HistAxis &axis,
                     const Cut &cut, const SystShifts &shift = kNoShift,
                     const Weight &wei = kUnweighted);

  PredictionNoExtrap(Loaders &loaders, const std::string &label,
                     const Binning &bins, const Var &var, const Cut &cut,
                     const SystShifts &shift = kNoShift,
                     const Weight &wei = kUnweighted);

  PredictionNoExtrap(Loaders &loaders, const HistAxis &axis, const Cut &cut,
                     const SystShifts &shift = kNoShift,
                     const Weight &wei = kUnweighted);

  virtual ~PredictionNoExtrap();

  static std::unique_ptr<PredictionNoExtrap> LoadFrom(TDirectory *dir, const std::string& name);
  virtual void SaveTo(TDirectory *dir, const std::string& name) const override;
};

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
} // namespace ana
