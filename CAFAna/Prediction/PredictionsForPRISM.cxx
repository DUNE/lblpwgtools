#include "CAFAna/Prediction/PredictionsForPRISM.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Extrap/IExtrap.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Extrap/TrivialExtrap.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "TDirectory.h"
#include "TObjString.h"

namespace ana {

//----------------------------------------------------------------------
PredictionNonSwapNoExtrap::PredictionNonSwapNoExtrap(
    SpectrumLoaderBase &loaderNonswap, SpectrumLoaderBase &loaderNue,
    SpectrumLoaderBase &loaderNuTau, const std::string &label,
    const Binning &bins, const Var &var, const Cut &cut,
    const SystShifts &shift, const Var &wei)
    : PredictionExtrap(new TrivialExtrap(loaderNonswap, loaderNue, loaderNuTau,
                                         label, bins, var, cut, shift, wei)) {}

//----------------------------------------------------------------------
PredictionNonSwapNoExtrap::PredictionNonSwapNoExtrap(
    SpectrumLoaderBase &loaderNonswap, SpectrumLoaderBase &loaderNue,
    SpectrumLoaderBase &loaderNuTau, const HistAxis &axis, const Cut &cut,
    const SystShifts &shift, const Var &wei)
    : PredictionExtrap(new TrivialExtrap(loaderNonswap, loaderNue, loaderNuTau,
                                         axis, cut, shift, wei)) {}

//----------------------------------------------------------------------
PredictionNonSwapNoExtrap::PredictionNonSwapNoExtrap(IExtrap *extrap)
    : PredictionExtrap(extrap) {}

//----------------------------------------------------------------------
PredictionNonSwapNoExtrap::PredictionNonSwapNoExtrap(PredictionExtrap *pred)
    : PredictionNonSwapNoExtrap(pred->GetExtrap()) {}

//----------------------------------------------------------------------
PredictionNonSwapNoExtrap::PredictionNonSwapNoExtrap(
    Loaders &loaders, const std::string &label, const Binning &bins,
    const Var &var, const Cut &cut, const SystShifts &shift, const Var &wei)
    : PredictionNonSwapNoExtrap(loaders, HistAxis(label, bins, var), cut, shift,
                                wei) {}

//----------------------------------------------------------------------
PredictionNonSwapNoExtrap::PredictionNonSwapNoExtrap(Loaders &loaders,
                                                     const HistAxis &axis,
                                                     const Cut &cut,
                                                     const SystShifts &shift,
                                                     const Var &wei)
    : PredictionExtrap(new TrivialExtrap(loaders, axis, cut, shift, wei)) {}

//----------------------------------------------------------------------
Spectrum PredictionNonSwapNoExtrap::PredictComponent(osc::IOscCalculator *calc,
                                                     Flavors::Flavors_t flav,
                                                     Current::Current_t curr,
                                                     Sign::Sign_t sign) const {

  Spectrum ret = fExtrap->NCComponent(); // Get binning
  ret.Clear();

  if (!(curr & Current::kCC)) {
    return ret;
  }

  if ((flav & Flavors::kNuMuToNuMu) && (sign & Sign::kNu)) {
    ret += fExtrap->NumuSurvComponent().Oscillated(calc, +14, +14);
  }
  if ((flav & Flavors::kNuMuToNuMu) && (sign & Sign::kAntiNu)) {
    ret += fExtrap->AntiNumuSurvComponent().Oscillated(calc, -14, -14);
  }

  if ((flav & Flavors::kNuMuToNuE) && (sign & Sign::kNu)) {
    ret += fExtrap->NumuSurvComponent().Oscillated(calc, +14, +12);
  }
  if ((flav & Flavors::kNuMuToNuE) && (sign & Sign::kAntiNu)) {
    ret += fExtrap->AntiNumuSurvComponent().Oscillated(calc, -14, -12);
  }

  return ret;
}

//----------------------------------------------------------------------
void PredictionNonSwapNoExtrap::SaveTo(TDirectory *dir) const {
  TDirectory *tmp = gDirectory;

  dir->cd();

  TObjString("PredictionNonSwapNoExtrap").Write("type");

  fExtrap->SaveTo(dir->mkdir("extrap"));

  tmp->cd();
}

//----------------------------------------------------------------------
std::unique_ptr<PredictionNonSwapNoExtrap>
PredictionNonSwapNoExtrap::LoadFrom(TDirectory *dir) {
  assert(dir->GetDirectory("extrap"));
  IExtrap *extrap =
      ana::LoadFrom<IExtrap>(dir->GetDirectory("extrap")).release();
  PredictionExtrap *pred = new PredictionExtrap(extrap);
  return std::unique_ptr<PredictionNonSwapNoExtrap>(
      new PredictionNonSwapNoExtrap(pred));
}

//----------------------------------------------------------------------
PredictionNonSwapNoExtrap::~PredictionNonSwapNoExtrap() {
  // We created this in the constructor so it's our responsibility
  delete fExtrap;
}

//----------------------------------------------------------------------
PredictionFDNoOsc::PredictionFDNoOsc(SpectrumLoaderBase &loader_non,
                                     SpectrumLoaderBase &loader_nue,
                                     const std::string &label,
                                     const Binning &bins, const Var &var,
                                     const Cut &cut, const SystShifts &shift,
                                     const Var &wei)
    : PredictionFDNoOsc(loader_non, loader_nue, HistAxis(label, bins, var), cut,
                        shift, wei) {}

//----------------------------------------------------------------------
PredictionFDNoOsc::PredictionFDNoOsc(SpectrumLoaderBase &loader_non,
                                     SpectrumLoaderBase &loader_nue,
                                     const HistAxis &axis, const Cut &cut,
                                     const SystShifts &shift, const Var &wei)
    : fSpectrumNonSwap(loader_non, axis, cut, shift, wei),
      fSpectrumNueSwap(loader_nue, axis, cut, shift, wei) {}

//----------------------------------------------------------------------
Spectrum PredictionFDNoOsc::PredictComponent(osc::IOscCalculator * /*calc*/,
                                             Flavors::Flavors_t flav,
                                             Current::Current_t curr,
                                             Sign::Sign_t sign) const {
  if (flav == Flavors::kAll && curr == Current::kBoth && sign == Sign::kBoth) {
    return Predict(0); // Faster
  }

  Spectrum ret = fSpectrumNonSwap;
  ret.Clear();

  if (curr & Current::kNC) {
    return ret;
  }

  assert(curr == Current::kCC);

  using namespace Flavors;
  using namespace Current;
  using namespace Sign;

  // Safe to assume by this point that it's 100% CC
  if (flav & kNuMuToNuMu) {
    ret += fSpectrumNonSwap;
  }
  if (flav & kNuMuToNuE) {
    ret += fSpectrumNueSwap;
  }
  return ret;
}

//----------------------------------------------------------------------
void PredictionFDNoOsc::SaveTo(TDirectory *dir) const {
  TDirectory *tmp = gDirectory;

  dir->cd();

  TObjString("PredictionFDNoOsc").Write("type");

  fSpectrumNonSwap.SaveTo(dir->mkdir("spect_nonswap"));
  fSpectrumNueSwap.SaveTo(dir->mkdir("spect_nueswap"));

  tmp->cd();
}

//----------------------------------------------------------------------
std::unique_ptr<PredictionFDNoOsc>
PredictionFDNoOsc::LoadFrom(TDirectory *dir) {

  // Can't use make_unique because constructor is protected
  return std::unique_ptr<PredictionFDNoOsc>(new PredictionFDNoOsc(
      *ana::LoadFrom<Spectrum>(dir->GetDirectory("spect_nonswap")),
      *ana::LoadFrom<Spectrum>(dir->GetDirectory("spect_nueswap"))));
}
} // namespace ana