#pragma once

#include "CAFAna/Core/Loaders.h"

#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Prediction/PredictionExtrap.h"
#include "CAFAna/Prediction/PredictionGenerator.h"

// Here are two specialized versions of the more commonly used Predictions with
// similar names. It was decided that making single use extensions was more
// sensible than adding generality that is unlikely to be used elsewhere, thus
// these 'hacks' have been contained in their own source file.
namespace ana {

/// Prediction that just uses FD MC, with no extrapolation
/// This version is used in the PRISM analysis to allow the NuMuSurv component
/// to be oscillated according to the appearance probability. This is done by
/// overriding the PredictionExtrap::Predict method to use a configurable
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
                            const Weight &wei = kUnweighted);

  PredictionNonSwapNoExtrap(SpectrumLoaderBase &loaderNonswap,
                            SpectrumLoaderBase &loaderNue,
                            SpectrumLoaderBase &loaderNuTau,
                            const HistAxis &axis, const Cut &cut,
                            const SystShifts &shift = kNoShift,
                            const Weight &wei = kUnweighted);

  PredictionNonSwapNoExtrap(Loaders &loaders, const std::string &label,
                            const Binning &bins, const Var &var, const Cut &cut,
                            const SystShifts &shift = kNoShift,
                            const Weight &wei = kUnweighted);

  PredictionNonSwapNoExtrap(Loaders &loaders, const HistAxis &axis,
                            const Cut &cut, const SystShifts &shift = kNoShift,
                            const Weight &wei = kUnweighted);

  virtual ~PredictionNonSwapNoExtrap();

  virtual Spectrum PredictComponent(osc::IOscCalc *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

  static std::unique_ptr<PredictionNonSwapNoExtrap> LoadFrom(TDirectory *dir,
                                                             const std::string& name);
  virtual void SaveTo(TDirectory *dir, const std::string& name) const override;

private:
};

class NonSwapNoExtrapPredictionGenerator : public IPredictionGenerator {
public:
  NonSwapNoExtrapPredictionGenerator(HistAxis axis, Cut cut,
                                     Weight wei = kUnweighted)
      : fAxis(axis), fCut(cut), fWei(wei) {}

  virtual std::unique_ptr<IPrediction>
  Generate(Loaders &loaders,
           const SystShifts &shiftMC = kNoShift) const override {
    return std::unique_ptr<IPrediction>(
        new PredictionNonSwapNoExtrap(loaders, fAxis, fCut, shiftMC, fWei));
  }

protected:
  HistAxis fAxis;
  Cut fCut;
  Weight fWei;
};


/// Class to correctly produce the FD mock 'data'. This makes sure the different
/// components of the 'data' are being oscillated correctly, as we would see in
/// real data. Previously just used OscillatableSpectrum for whole nonswap or
/// nueswap file. This led to components of FD data being oscillated incorrectly.
///
/// Do not need an associated IPredictionGenerator class as I never anticipate
/// this being used as an input to PredictionInterp objects.
/// (This class just re-implements the old PredictionNoExtrap prediction method.)
class DataPredictionNoExtrap : public PredictionExtrap {
public:

  DataPredictionNoExtrap(PredictionExtrap *pred);
  DataPredictionNoExtrap(IExtrap *extrap);

  DataPredictionNoExtrap(Loaders &loaders, const HistAxis &axis,
                         const Cut &cut, const SystShifts &shift = kNoShift,
                         const Weight &wei = kUnweighted);

  virtual ~DataPredictionNoExtrap();

  virtual Spectrum Predict(osc::IOscCalc *calc) const override;

  virtual Spectrum PredictComponent(osc::IOscCalc *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

  static std::unique_ptr<DataPredictionNoExtrap> LoadFrom(TDirectory *dir, const std::string& name);

  virtual void SaveTo(TDirectory *dir, const std::string& name) const override;

private:
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
                    SpectrumLoaderBase &loader_nue,
                    SpectrumLoaderBase &loader_nutau, const HistAxis &axis,
                    const Cut &cut, const SystShifts &shift = kNoShift,
                    const Weight &wei = kUnweighted);

  PredictionFDNoOsc(SpectrumLoaderBase &loader_non,
                    SpectrumLoaderBase &loader_nue,
                    SpectrumLoaderBase &loader_nutau, const std::string &label,
                    const Binning &bins, const Var &var, const Cut &cut,
                    const SystShifts &shift = kNoShift,
                    const Weight &wei = kUnweighted);

  static std::unique_ptr<PredictionFDNoOsc> LoadFrom(TDirectory *dir, const std::string& name);
  virtual void SaveTo(TDirectory *dir, const std::string& name) const override;

  void OverridePOT(double pot) {
    fSpectrumNonSwap.OverridePOT(pot);
    fSpectrumNueSwap.OverridePOT(pot);
    fSpectrumNutauSwap.OverridePOT(pot);
    fSpectrumRHCNonSwap.OverridePOT(pot);
    fSpectrumRHCNueSwap.OverridePOT(pot);
    fSpectrumRHCNutauSwap.OverridePOT(pot);
  }

  virtual Spectrum Predict(osc::IOscCalc * /*calc*/) const override {
    return fSpectrumNonSwap + fSpectrumNueSwap + fSpectrumNutauSwap;
  }
  virtual Spectrum PredictComponent(osc::IOscCalc *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

protected:
  PredictionFDNoOsc(const Spectrum &_fSpectrumNonSwap,
                    const Spectrum &_fSpectrumNueSwap,
                    const Spectrum &_fSpectrumNutauSwap,
                    const Spectrum &_fSpectrumRHCNonSwap,
                    const Spectrum &_fSpectrumRHCNueSwap,
                    const Spectrum &_fSpectrumRHCNutauSwap,
                    const Spectrum &_fSpectrumNonSwapNues,
                    const Spectrum &_fSpectrumRHCNonSwapNues)
      : fSpectrumNonSwap(_fSpectrumNonSwap),
        fSpectrumNueSwap(_fSpectrumNueSwap),
        fSpectrumNutauSwap(_fSpectrumNutauSwap),
        fSpectrumRHCNonSwap(_fSpectrumRHCNonSwap),
        fSpectrumRHCNueSwap(_fSpectrumRHCNueSwap),
        fSpectrumRHCNutauSwap(_fSpectrumRHCNutauSwap),
        fSpectrumNonSwapNues(_fSpectrumNonSwapNues),
        fSpectrumRHCNonSwapNues(_fSpectrumRHCNonSwapNues) {}

  Spectrum fSpectrumNonSwap;
  Spectrum fSpectrumNueSwap;
  Spectrum fSpectrumNutauSwap;
  Spectrum fSpectrumRHCNonSwap;
  Spectrum fSpectrumRHCNueSwap;
  Spectrum fSpectrumRHCNutauSwap;
  Spectrum fSpectrumNonSwapNues;
  Spectrum fSpectrumRHCNonSwapNues;
};

class FDNoOscPredictionGenerator : public IPredictionGenerator {
public:
  FDNoOscPredictionGenerator(HistAxis axis, Cut cut, Weight wei = kUnweighted)
      : fAxis(axis), fCut(cut), fWei(wei) {}

  virtual std::unique_ptr<IPrediction>
  Generate(Loaders &loaders,
           const SystShifts &shiftMC = kNoShift) const override {
    SpectrumLoaderBase &loader_non =
        loaders.GetLoader(caf::kFARDET, Loaders::kMC, Loaders::kNonSwap);
    SpectrumLoaderBase &loader_nue =
        loaders.GetLoader(caf::kFARDET, Loaders::kMC, Loaders::kNueSwap);
    SpectrumLoaderBase &loader_nutau =
        loaders.GetLoader(caf::kFARDET, Loaders::kMC, Loaders::kNuTauSwap);
    return std::unique_ptr<IPrediction>(new PredictionFDNoOsc(
        loader_non, loader_nue, loader_nutau, fAxis, fCut, shiftMC, fWei));
  }

protected:
  HistAxis fAxis;
  Cut fCut;
  Weight fWei;
};

} // namespace ana
