#pragma once

#include "CAFAna/Core/Loaders.h"

#include "CAFAna/Prediction/PredictionExtrap.h"

#include "CAFAna/Prediction/PredictionGenerator.h"

// Here are two specialized versions of the more commonly used Predictions with
// similar names. It was decided that making single use extensions was more
// sensible that adding generality that is unlikely to be used elsewhere, thus
// these 'hacks' have been contained in their own source file.
namespace ana {

/// Prediction that just uses FD MC, with no extrapolation
/// This version is used in the PRISM analysis to allow the NuMuSurv component
/// to be oscillated according to the appearance probability. This is done by
/// overriding the PredictionExtrap::Predict method to use a configurate
/// oscillation channel. This is used for the event rate matcher to avoid
/// trusting the generator on the numu->nue xsec ratio.
class PredictionNonSwapNoExtrap : public PredictionExtrap {
public:
  PredictionNonSwapNoExtrap(PredictionExtrap *pred);
  PredictionNonSwapNoExtrap(IExtrap *extrap);

  // This is the DUNE constructor
  PredictionNonSwapNoExtrap(SpectrumLoaderBase &loaderNonswap,
                            SpectrumLoaderBase &loaderNue,
                            SpectrumLoaderBase &loaderNuTau,
                            const std::string &label, const Binning &bins,
                            const Var &var, const Cut &cut,
                            const SystShifts &shift = kNoShift,
                            const Var &wei = kUnweighted);

  PredictionNonSwapNoExtrap(SpectrumLoaderBase &loaderNonswap,
                            SpectrumLoaderBase &loaderNue,
                            SpectrumLoaderBase &loaderNuTau,
                            const HistAxis &axis, const Cut &cut,
                            const SystShifts &shift = kNoShift,
                            const Var &wei = kUnweighted);

  PredictionNonSwapNoExtrap(Loaders &loaders, const std::string &label,
                            const Binning &bins, const Var &var, const Cut &cut,
                            const SystShifts &shift = kNoShift,
                            const Var &wei = kUnweighted);

  PredictionNonSwapNoExtrap(Loaders &loaders, const HistAxis &axis,
                            const Cut &cut, const SystShifts &shift = kNoShift,
                            const Var &wei = kUnweighted);

  virtual ~PredictionNonSwapNoExtrap();

  virtual Spectrum PredictComponent(osc::IOscCalculator *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

  static std::unique_ptr<PredictionNonSwapNoExtrap> LoadFrom(TDirectory *dir);
  virtual void SaveTo(TDirectory *dir) const override;

private:
};

class NonSwapNoExtrapPredictionGenerator : public IPredictionGenerator {
public:
  NonSwapNoExtrapPredictionGenerator(HistAxis axis, Cut cut,
                                     Var wei = kUnweighted)
      : fAxis(axis), fCut(cut), fWei(wei) {
    for (auto &v : fAxis.GetVars()) {
      assert(v.IsValid());
    }
    assert(fWei.IsValid());
  }

  virtual std::unique_ptr<IPrediction>
  Generate(Loaders &loaders,
           const SystShifts &shiftMC = kNoShift) const override {
    return std::unique_ptr<IPrediction>(
        new PredictionNonSwapNoExtrap(loaders, fAxis, fCut, shiftMC, fWei));
  }

protected:
  HistAxis fAxis;
  Cut fCut;
  Var fWei;
};

/// Prediction that wraps a simple Spectrum, makes unoscillatable FD
/// predictions, but keeping intrinsic and swap spectra separate. Needed
/// specifically in the PRISM analysis for the flux mis-match correction, which
/// may not use the same true energy binning as an OscillatableSpectrum
///
/// This just simply holds a spectrum component from each of the two input
/// loaders, so it is up to the user to set a sensible cut (i.e. looking for
/// just numu->numu events, rather than all numu events)
class PredictionFDNoOsc : public IPrediction {
public:
  PredictionFDNoOsc(SpectrumLoaderBase &loader_non,
                    SpectrumLoaderBase &loader_nue, const HistAxis &axis,
                    const Cut &cut, const SystShifts &shift = kNoShift,
                    const Var &wei = kUnweighted);

  PredictionFDNoOsc(SpectrumLoaderBase &loader_non,
                    SpectrumLoaderBase &loader_nue, const std::string &label,
                    const Binning &bins, const Var &var, const Cut &cut,
                    const SystShifts &shift = kNoShift,
                    const Var &wei = kUnweighted);

  static std::unique_ptr<PredictionFDNoOsc> LoadFrom(TDirectory *dir);
  virtual void SaveTo(TDirectory *dir) const override;

  void OverridePOT(double pot) {
    fSpectrumNonSwap.OverridePOT(pot);
    fSpectrumNueSwap.OverridePOT(pot);
  }

  virtual Spectrum Predict(osc::IOscCalculator * /*calc*/) const override {
    return fSpectrumNonSwap + fSpectrumNueSwap;
  }
  virtual Spectrum PredictComponent(osc::IOscCalculator *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

protected:
  PredictionFDNoOsc(const Spectrum &_fSpectrumNonSwap,
                    const Spectrum &_fSpectrumNueSwap)
      : fSpectrumNonSwap(_fSpectrumNonSwap), fSpectrumNueSwap(_fSpectrumNueSwap) {}

  Spectrum fSpectrumNonSwap;
  Spectrum fSpectrumNueSwap;
};

class FDNoOscPredictionGenerator : public IPredictionGenerator {
public:
  FDNoOscPredictionGenerator(HistAxis axis, Cut cut, Var wei = kUnweighted)
      : fAxis(axis), fCut(cut), fWei(wei) {
    for (auto &v : fAxis.GetVars()) {
      assert(v.IsValid());
    }
    assert(fWei.IsValid());
  }

  virtual std::unique_ptr<IPrediction>
  Generate(Loaders &loaders,
           const SystShifts &shiftMC = kNoShift) const override {
    SpectrumLoaderBase &loader_non = loaders.GetLoader(
        caf::kFARDET, Loaders::kMC, kBeam, Loaders::kNonSwap);
    SpectrumLoaderBase &loader_nue = loaders.GetLoader(
        caf::kFARDET, Loaders::kMC, kBeam, Loaders::kNueSwap);
    return std::unique_ptr<IPrediction>(new PredictionFDNoOsc(
        loader_non, loader_nue, fAxis, fCut, shiftMC, fWei));
  }

protected:
  HistAxis fAxis;
  Cut fCut;
  Var fWei;
};

} // namespace ana