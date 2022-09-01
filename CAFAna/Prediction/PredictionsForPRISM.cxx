#include "CAFAna/Prediction/PredictionsForPRISM.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Extrap/IExtrap.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Extrap/TrivialExtrap.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/Systs/DUNEFluxSysts.h"
#include "CAFAna/Systs/OffAxisFluxUncertaintyHelper.h"

#include "TDirectory.h"
#include "TH1.h"
#include "TH2.h"
#include "TObjString.h"

namespace ana {

  // PredictionNonSwapNoExtrap Class:

  //----------------------------------------------------------------------
  PredictionNonSwapNoExtrap::PredictionNonSwapNoExtrap(
      SpectrumLoaderBase &loaderNonswap, SpectrumLoaderBase &loaderNue,
      SpectrumLoaderBase &loaderNuTau, const std::string &label,
      const Binning &bins, const Var &var, const Cut &cut,
      const SystShifts &shift, const Weight &wei)
      : PredictionExtrap(new TrivialExtrap(loaderNonswap, loaderNue, loaderNuTau,
                                         label, bins, var, cut, shift, wei)) {}

  //----------------------------------------------------------------------
  PredictionNonSwapNoExtrap::PredictionNonSwapNoExtrap(
      SpectrumLoaderBase &loaderNonswap, SpectrumLoaderBase &loaderNue,
      SpectrumLoaderBase &loaderNuTau, const HistAxis &axis, const Cut &cut,
      const SystShifts &shift, const Weight &wei)
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
      const Var &var, const Cut &cut, const SystShifts &shift, const Weight &wei)
      : PredictionNonSwapNoExtrap(loaders, HistAxis(label, bins, var), cut, shift,
                                  wei) {}

  //----------------------------------------------------------------------
  PredictionNonSwapNoExtrap::PredictionNonSwapNoExtrap(Loaders &loaders,
                                                       const HistAxis &axis,
                                                       const Cut &cut,
                                                       const SystShifts &shift,
                                                       const Weight &wei)
      : PredictionExtrap(new TrivialExtrap(loaders, axis, cut, shift, wei)) {}

  //----------------------------------------------------------------------
  Spectrum PredictionNonSwapNoExtrap::PredictComponent(osc::IOscCalc *calc,
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

    // Include intrinsic nue in flux matching
    if ((flav & Flavors::kNuEToNuE) && (sign & Sign::kNu)) {
      ret += fExtrap->NueSurvComponent().Oscillated(calc, +12, +12);
    }
    if ((flav & Flavors::kNuEToNuE) && (sign & Sign::kAntiNu)) {
      ret += fExtrap->AntiNueSurvComponent().Oscillated(calc, -12, -12);
    }

    if ((flav & Flavors::kNuMuToNuTau) && (sign & Sign::kNu)) {
      ret += fExtrap->NumuSurvComponent().Oscillated(calc, +14, +16);
    }
    if ((flav & Flavors::kNuMuToNuTau) && (sign & Sign::kAntiNu)) {
      ret += fExtrap->AntiNumuSurvComponent().Oscillated(calc, -14, -16);
    }
    return ret;
  }

  //----------------------------------------------------------------------
  void PredictionNonSwapNoExtrap::SaveTo(TDirectory *dir, const std::string& name) const {
    TDirectory *tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("PredictionNonSwapNoExtrap").Write("type");

    fExtrap->SaveTo(dir, "extrap");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<PredictionNonSwapNoExtrap>
  PredictionNonSwapNoExtrap::LoadFrom(TDirectory *dir, const std::string& name) {
    dir = dir->GetDirectory(name.c_str());
    assert(dir);

    IExtrap *extrap = ana::LoadFrom<IExtrap>(dir, "extrap").release();
    PredictionExtrap *pred = new PredictionExtrap(extrap);

    delete dir;

    return std::unique_ptr<PredictionNonSwapNoExtrap>(new PredictionNonSwapNoExtrap(pred));
  }

  //----------------------------------------------------------------------
  PredictionNonSwapNoExtrap::~PredictionNonSwapNoExtrap() {
    // We created this in the constructor so it's our responsibility
    delete fExtrap;
  }

  // DataPredictionNoExtrap Class:

  //----------------------------------------------------------------------
  DataPredictionNoExtrap::DataPredictionNoExtrap(Loaders &loaders,
                                                 const HistAxis &axis,
                                                 const Cut &cut,
                                                 const SystShifts &shift,
                                                 const Weight &wei)
      : PredictionExtrap(new TrivialExtrap(loaders, axis, cut, shift, wei)) {}

  //----------------------------------------------------------------------
  DataPredictionNoExtrap::DataPredictionNoExtrap(IExtrap *extrap)
      : PredictionExtrap(extrap) {}

  //----------------------------------------------------------------------
  DataPredictionNoExtrap::DataPredictionNoExtrap(PredictionExtrap *pred)
      : DataPredictionNoExtrap(pred->GetExtrap()) {}

  //----------------------------------------------------------------------
  DataPredictionNoExtrap::~DataPredictionNoExtrap() {
    // We created this in the constructor so it's our responsibility
    delete fExtrap;
  }

  //----------------------------------------------------------------------
  Spectrum DataPredictionNoExtrap::Predict(osc::IOscCalc *calc) const {
    return PredictComponent(calc, Flavors::kAll, Current::kBoth, Sign::kBoth);
  }

  //----------------------------------------------------------------------
  Spectrum DataPredictionNoExtrap::PredictComponent(osc::IOscCalc *calc,
                                           Flavors::Flavors_t flav,
                                           Current::Current_t curr,
                                           Sign::Sign_t sign) const {
    Spectrum ret = fExtrap->NCComponent(); // Get binning
    ret.Clear();

    if(curr & Current::kCC){
      if(flav & Flavors::kNuEToNuE && sign & Sign::kNu) ret += fExtrap->NueSurvComponent()
                                                               .Oscillated(calc, +12, +12);
      if(flav & Flavors::kNuEToNuE && sign & Sign::kAntiNu) ret += fExtrap->AntiNueSurvComponent()
                                                                   .Oscillated(calc, -12, -12);
      if(flav & Flavors::kNuEToNuMu && sign & Sign::kNu) ret += fExtrap->NumuAppComponent()
                                                                .Oscillated(calc, +12, +14);
      if(flav & Flavors::kNuEToNuMu && sign & Sign::kAntiNu) ret += fExtrap->AntiNumuAppComponent()
                                                                    .Oscillated(calc, -12, -14);
      if(flav & Flavors::kNuEToNuTau && sign & Sign::kNu) ret += fExtrap->TauFromEComponent()
                                                                 .Oscillated(calc, +12, +16);
      if(flav & Flavors::kNuEToNuTau && sign & Sign::kAntiNu) ret += fExtrap->AntiTauFromEComponent()
                                                                     .Oscillated(calc, -12, -16);
      if(flav & Flavors::kNuMuToNuE && sign & Sign::kNu) ret += fExtrap->NueAppComponent()
                                                                .Oscillated(calc, +14, +12);
      if(flav & Flavors::kNuMuToNuE && sign & Sign::kAntiNu) ret += fExtrap->AntiNueAppComponent()
                                                                    .Oscillated(calc, -14, -12);
      if(flav & Flavors::kNuMuToNuMu && sign & Sign::kNu) ret += fExtrap->NumuSurvComponent()
                                                                 .Oscillated(calc, +14, +14);
      if(flav & Flavors::kNuMuToNuMu && sign & Sign::kAntiNu) ret += fExtrap->AntiNumuSurvComponent()
                                                                     .Oscillated(calc, -14, -14);
      if(flav & Flavors::kNuMuToNuTau && sign & Sign::kNu) ret += fExtrap->TauFromMuComponent()
                                                                  .Oscillated(calc, +14, +16);
      if(flav & Flavors::kNuMuToNuTau && sign & Sign::kAntiNu) ret += fExtrap->AntiTauFromMuComponent()
                                                                      .Oscillated(calc, -14, -16);
    }
    if (curr & Current::kNC) {
      assert(flav == Flavors::kAll);
      assert(sign == Sign::kBoth); // Don't split NC 'data' by sign, not interpolating.
      ret += fExtrap->NCTotalComponent();
    }

    return ret;
  }

  void DataPredictionNoExtrap::SaveTo(TDirectory *dir, const std::string& name) const {
    TDirectory *tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("DataPredictionNoExtrap").Write("type");

    fExtrap->SaveTo(dir, "extrap");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  std::unique_ptr<DataPredictionNoExtrap> DataPredictionNoExtrap::LoadFrom(TDirectory *dir,
                                                                           const std::string& name) {
    dir = dir->GetDirectory(name.c_str());
    assert(dir);

    IExtrap *extrap = ana::LoadFrom<IExtrap>(dir, "extrap").release();
    PredictionExtrap *pred = new PredictionExtrap(extrap);

    delete dir;

    return std::unique_ptr<DataPredictionNoExtrap>(new DataPredictionNoExtrap(pred));
  }

  // PredictionFDNoOsc Class:

  //----------------------------------------------------------------------
  PredictionFDNoOsc::PredictionFDNoOsc(SpectrumLoaderBase &loader_non,
                                       SpectrumLoaderBase &loader_nue,
                                       SpectrumLoaderBase &loader_nutau,
                                       const std::string &label,
                                       const Binning &bins, const Var &var,
                                       const Cut &cut, const SystShifts &shift,
                                       const Weight &wei)
      : PredictionFDNoOsc(loader_non, loader_nue, loader_nutau, HistAxis(label, bins, var), cut,
                          shift, wei) {}

  //----------------------------------------------------------------------
  PredictionFDNoOsc::PredictionFDNoOsc(SpectrumLoaderBase &loader_non,
                                       SpectrumLoaderBase &loader_nue,
                                       SpectrumLoaderBase &loader_nutau,
                                       const HistAxis &axis, const Cut &cut,
                                       const SystShifts &shift, const Weight &wei)
      : fSpectrumNonSwap(loader_non, axis, cut && !kIsAntiNu && !kIsNC && kIsNumuCC, shift, wei),
        fSpectrumNueSwap(loader_nue, axis, cut && !kIsAntiNu && !kIsNC && kIsSig, shift, wei),
        fSpectrumNutauSwap(loader_nutau, axis, cut && !kIsAntiNu && !kIsNC && kIsTauFromMu, shift, wei),
        fSpectrumRHCNonSwap(loader_non, axis, cut && kIsAntiNu && !kIsNC && kIsNumuCC, shift, wei),
        fSpectrumRHCNueSwap(loader_nue, axis, cut && kIsAntiNu && !kIsNC && kIsSig, shift, wei),
        fSpectrumRHCNutauSwap(loader_nutau, axis, cut && kIsAntiNu && !kIsNC && kIsTauFromMu, shift, wei) {}

  //----------------------------------------------------------------------
  Spectrum PredictionFDNoOsc::PredictComponent(osc::IOscCalc * /*calc*/,
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
    if (flav & kNuMuToNuMu && sign & kNu) {
      ret += fSpectrumNonSwap;
    }
    if (flav & kNuMuToNuMu && sign & kAntiNu) {
      ret += fSpectrumRHCNonSwap;
    }
    if (flav & kNuMuToNuE && sign & kNu) {
      ret += fSpectrumNueSwap;
    }
    if (flav & kNuMuToNuE && sign & kAntiNu) {
      ret += fSpectrumRHCNueSwap;
    }
    if (flav & kNuMuToNuTau && sign & kNu) {
      ret += fSpectrumNutauSwap;
    }
    if (flav & kNuMuToNuTau && sign & kAntiNu) {
      ret += fSpectrumRHCNutauSwap;
    }
    return ret;
  }

  //----------------------------------------------------------------------
  void PredictionFDNoOsc::SaveTo(TDirectory *dir, const std::string& name) const {
    TDirectory *tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("PredictionFDNoOsc").Write("type");

    fSpectrumNonSwap.SaveTo(dir, "spect_nonswap");
    fSpectrumNueSwap.SaveTo(dir, "spect_nueswap");
    fSpectrumNutauSwap.SaveTo(dir, "spect_nutauswap");
    fSpectrumRHCNonSwap.SaveTo(dir, "spect_RHCnonswap");
    fSpectrumRHCNueSwap.SaveTo(dir, "spect_RHCnueswap");
    fSpectrumRHCNutauSwap.SaveTo(dir, "spect_RHCnutauswap");

    dir->Write();
    
    delete dir;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<PredictionFDNoOsc>PredictionFDNoOsc::LoadFrom(TDirectory *dir,
                                                                const std::string& name) {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    // Can't use make_unique because constructor is protected
    PredictionFDNoOsc *ret = new PredictionFDNoOsc(
        *ana::LoadFrom<Spectrum>(dir, "spect_nonswap"),
        *ana::LoadFrom<Spectrum>(dir, "spect_nueswap"),
        *ana::LoadFrom<Spectrum>(dir, "spect_nutauswap"),
        *ana::LoadFrom<Spectrum>(dir, "spect_RHCnonswap"),
        *ana::LoadFrom<Spectrum>(dir, "spect_RHCnueswap"),
        *ana::LoadFrom<Spectrum>(dir, "spect_RHCnutauswap"));

    delete dir;

    return std::unique_ptr<PredictionFDNoOsc>(ret);
  }
} // namespace ana
