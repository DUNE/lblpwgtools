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

  static std::unique_ptr<PredictionNonSwapNoExtrap> LoadFrom(TDirectory *dir);
  virtual void SaveTo(TDirectory *dir, const std::string& name) const override;

private:
};

class NonSwapNoExtrapPredictionGenerator : public IPredictionGenerator {
public:
  NonSwapNoExtrapPredictionGenerator(HistAxis axis, Cut cut,
                                     Weight wei = kUnweighted)
      : fAxis(axis), fCut(cut), fWei(wei) {
    /*for (auto &v : fAxis.GetVars()) {
      assert(v.IsValid());
    }
    assert(fWei.IsValid());*/
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

  static std::unique_ptr<DataPredictionNoExtrap> LoadFrom(TDirectory *dir);

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
                    SpectrumLoaderBase &loader_nue, const HistAxis &axis,
                    const Cut &cut, const SystShifts &shift = kNoShift,
                    const Weight &wei = kUnweighted);

  PredictionFDNoOsc(SpectrumLoaderBase &loader_non,
                    SpectrumLoaderBase &loader_nue, const std::string &label,
                    const Binning &bins, const Var &var, const Cut &cut,
                    const SystShifts &shift = kNoShift,
                    const Weight &wei = kUnweighted);

  static std::unique_ptr<PredictionFDNoOsc> LoadFrom(TDirectory *dir);
  virtual void SaveTo(TDirectory *dir, const std::string& name) const override;

  void OverridePOT(double pot) {
    fSpectrumNonSwap.OverridePOT(pot);
    fSpectrumNueSwap.OverridePOT(pot);
  }

  virtual Spectrum Predict(osc::IOscCalc * /*calc*/) const override {
    return fSpectrumNonSwap + fSpectrumNueSwap;
  }
  virtual Spectrum PredictComponent(osc::IOscCalc *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

protected:
  PredictionFDNoOsc(const Spectrum &_fSpectrumNonSwap,
                    const Spectrum &_fSpectrumNueSwap,
                    const Spectrum &_fSpectrumRHCNonSwap,
                    const Spectrum &_fSpectrumRHCNueSwap) 
      : fSpectrumNonSwap(_fSpectrumNonSwap),
        fSpectrumNueSwap(_fSpectrumNueSwap),
        fSpectrumRHCNonSwap(_fSpectrumRHCNonSwap),
        fSpectrumRHCNueSwap(_fSpectrumRHCNueSwap) {} 

  Spectrum fSpectrumNonSwap;
  Spectrum fSpectrumNueSwap;
  Spectrum fSpectrumRHCNonSwap;
  Spectrum fSpectrumRHCNueSwap;
};

class FDNoOscPredictionGenerator : public IPredictionGenerator {
public:
  FDNoOscPredictionGenerator(HistAxis axis, Cut cut, Weight wei = kUnweighted)
      : fAxis(axis), fCut(cut), fWei(wei) {
    /*for (auto &v : fAxis.GetVars()) {
      assert(v.IsValid());
    }
    assert(fWei.IsValid());*/
  }

  virtual std::unique_ptr<IPrediction>
  Generate(Loaders &loaders,
           const SystShifts &shiftMC = kNoShift) const override {
    SpectrumLoaderBase &loader_non =
        loaders.GetLoader(caf::kFARDET, Loaders::kMC, Loaders::kNonSwap);
    SpectrumLoaderBase &loader_nue =
        loaders.GetLoader(caf::kFARDET, Loaders::kMC, Loaders::kNueSwap);
    return std::unique_ptr<IPrediction>(new PredictionFDNoOsc(
        loader_non, loader_nue, fAxis, fCut, shiftMC, fWei));
  }

protected:
  HistAxis fAxis;
  Cut fCut;
  Weight fWei;
};

/*
// A flux prediction that acts like an IPrediction
class FluxPrediction : public IPrediction {

public:
  static std::unique_ptr<FluxPrediction> LoadFrom(TDirectory *dir);
  virtual void SaveTo(TDirectory *dir, const std::string& name) const override;
  virtual Spectrum Predict(osc::IOscCalc *) const override {
    // no way to know which is the 'right' spectrum, so, don't use this.

    // std::cout << "[ERROR]: Cannot call Predict on FluxPrediction. Use "
    //              "PredictComponent instead."
    //           << std::endl;
    // abort();

    // required to get the binning in initfits
    return fSpectrumNumu;
  }

  virtual Spectrum PredictComponent(osc::IOscCalc *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

  FluxPrediction(const Spectrum &sNumu, const Spectrum &sNumubar,
                 const Spectrum &sNue, const Spectrum &sNuebar)
      : fSpectrumNumu(sNumu), fSpectrumNumubar(sNumubar), fSpectrumNue(sNue),
        fSpectrumNuebar(sNuebar) {}

protected:
  Spectrum fSpectrumNumu;
  Spectrum fSpectrumNumubar;
  Spectrum fSpectrumNue;
  Spectrum fSpectrumNuebar;
};

class FluxPredictionGenerator : public IPredictionGenerator {
public:
  FluxPredictionGenerator(HistAxis const &axis, bool isNuMode = true)
      : fAxis(axis), fIsNuMode(isNuMode) {}

  virtual std::unique_ptr<IPrediction>
  Generate(Loaders &loaders,
           const SystShifts &shiftMC = kNoShift) const override;

protected:
  HistAxis fAxis;
  bool fIsNuMode;
};

// A flux prediction that acts like an IPrediction
class OffAxisFluxPrediction : public IPrediction {

public:
  static std::unique_ptr<OffAxisFluxPrediction> LoadFrom(TDirectory *dir);
  virtual void SaveTo(TDirectory *dir, const std::string& name) const override;
  virtual Spectrum Predict(osc::IOscCalc *) const override {
    // no way to know which is the 'right' spectrum, so, don't use this.

    // std::cout << "[ERROR]: Cannot call Predict on OffAxisFluxPrediction. Use
    // "
    //              "PredictComponent instead."
    //           << std::endl;
    // abort();

    // required to get the binning in initfits
    return fSpectrumNumu;
  }

  virtual Spectrum PredictComponent(osc::IOscCalc *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

  OffAxisFluxPrediction(const Spectrum &sNumu, const Spectrum &sNumubar,
                        const Spectrum &sNue, const Spectrum &sNuebar)
      : fSpectrumNumu(sNumu), fSpectrumNumubar(sNumubar), fSpectrumNue(sNue),
        fSpectrumNuebar(sNuebar) {}

protected:
  Spectrum fSpectrumNumu;
  Spectrum fSpectrumNumubar;
  Spectrum fSpectrumNue;
  Spectrum fSpectrumNuebar;
};

class OffAxisFluxPredictionGenerator : public IPredictionGenerator {
public:
  OffAxisFluxPredictionGenerator(HistAxis const &axis, bool isNuMode = true,
                                 bool isSpecRun = false)
      : fAxis(axis), fIsNuMode(isNuMode), fIsSpecRun(isSpecRun) {}

  virtual std::unique_ptr<IPrediction>
  Generate(Loaders &loaders,
           const SystShifts &shiftMC = kNoShift) const override;

protected:
  HistAxis fAxis;
  bool fIsNuMode;
  bool fIsSpecRun;
};
*/
} // namespace ana
