#include "CAFAna/Prediction/PredictionsForPRISM.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Extrap/IExtrap.h"

#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Extrap/TrivialExtrap.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/Systs/DUNEFluxSysts.h"
#include "CAFAna/Systs/OffAxisFluxUncertaintyHelper.h"

#include "OscLib/func/IOscCalculator.h"

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

// DataPredictionNoExtrap Class:

//----------------------------------------------------------------------
DataPredictionNoExtrap::DataPredictionNoExtrap(Loaders &loaders, 
                                               const HistAxis &axis,
                                               const Cut &cut, 
                                               const SystShifts &shift,
                                               const Var &wei)
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
Spectrum DataPredictionNoExtrap::Predict(osc::IOscCalculator *calc) const {
  return PredictComponent(calc, Flavors::kAll, Current::kBoth, Sign::kBoth);
}

//----------------------------------------------------------------------
Spectrum DataPredictionNoExtrap::PredictComponent(osc::IOscCalculator *calc,
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
    assert(sign == Sign::kBoth);
    ret += fExtrap->NCComponent();
  }

  return ret;
}

void DataPredictionNoExtrap::SaveTo(TDirectory *dir) const {
  TDirectory *tmp = gDirectory;

  dir->cd();

  TObjString("DataPredictionNoExtrap").Write("type"); 

  fExtrap->SaveTo(dir->mkdir("extrap"));

  tmp->cd();
}

std::unique_ptr<DataPredictionNoExtrap> DataPredictionNoExtrap::LoadFrom(TDirectory *dir) {
  assert(dir->GetDirectory("extrap"));
  IExtrap *extrap =
      ana::LoadFrom<IExtrap>(dir->GetDirectory("extrap")).release();
  PredictionExtrap *pred = new PredictionExtrap(extrap);

  return std::unique_ptr<DataPredictionNoExtrap>(
      new DataPredictionNoExtrap(pred));           

}

// PredictionFDNoOsc Class:

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
    : fSpectrumNonSwap(loader_non, axis, cut && !kIsAntiNu && !kIsNC, shift, wei),
      fSpectrumNueSwap(loader_nue, axis, cut && !kIsAntiNu && !kIsNC, shift, wei),
      fSpectrumRHCNonSwap(loader_non, axis, cut && kIsAntiNu && !kIsNC, shift, wei),
      fSpectrumRHCNueSwap(loader_nue, axis, cut && kIsAntiNu && !kIsNC, shift, wei) {}

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
  return ret;
}

//----------------------------------------------------------------------
void PredictionFDNoOsc::SaveTo(TDirectory *dir) const {
  TDirectory *tmp = gDirectory;

  dir->cd();

  TObjString("PredictionFDNoOsc").Write("type");

  fSpectrumNonSwap.SaveTo(dir->mkdir("spect_nonswap"));
  fSpectrumNueSwap.SaveTo(dir->mkdir("spect_nueswap"));
  fSpectrumRHCNonSwap.SaveTo(dir->mkdir("spect_RHCnonswap"));
  fSpectrumRHCNueSwap.SaveTo(dir->mkdir("spect_RHCnueswap"));

  tmp->cd();
}

//----------------------------------------------------------------------
std::unique_ptr<PredictionFDNoOsc>
PredictionFDNoOsc::LoadFrom(TDirectory *dir) {

  // Can't use make_unique because constructor is protected
  return std::unique_ptr<PredictionFDNoOsc>(new PredictionFDNoOsc(
      *ana::LoadFrom<Spectrum>(dir->GetDirectory("spect_nonswap")),
      *ana::LoadFrom<Spectrum>(dir->GetDirectory("spect_nueswap")),
      *ana::LoadFrom<Spectrum>(dir->GetDirectory("spect_RHCnonswap")),
      *ana::LoadFrom<Spectrum>(dir->GetDirectory("spect_RHCnueswap"))));
}

struct PRISMFluxHelper {
  std::unique_ptr<TH2> fNDNumu_293kA_nu;
  std::unique_ptr<TH2> fNDNumubar_293kA_nu;
  std::unique_ptr<TH2> fNDNue_293kA_nu;
  std::unique_ptr<TH2> fNDNuebar_293kA_nu;
  std::unique_ptr<TH2> fNDNumu_293kA_nub;
  std::unique_ptr<TH2> fNDNumubar_293kA_nub;
  std::unique_ptr<TH2> fNDNue_293kA_nub;
  std::unique_ptr<TH2> fNDNuebar_293kA_nub;

  std::unique_ptr<TH2> fNDNumu_280kA_nu;
  std::unique_ptr<TH2> fNDNumubar_280kA_nu;
  std::unique_ptr<TH2> fNDNue_280kA_nu;
  std::unique_ptr<TH2> fNDNuebar_280kA_nu;
  std::unique_ptr<TH2> fNDNumu_280kA_nub;
  std::unique_ptr<TH2> fNDNumubar_280kA_nub;
  std::unique_ptr<TH2> fNDNue_280kA_nub;
  std::unique_ptr<TH2> fNDNuebar_280kA_nub;

  std::unique_ptr<TH1> fFDNumu_nu;
  std::unique_ptr<TH1> fFDNumubar_nu;
  std::unique_ptr<TH1> fFDNue_nu;
  std::unique_ptr<TH1> fFDNuebar_nu;
  std::unique_ptr<TH1> fFDNumu_nub;
  std::unique_ptr<TH1> fFDNumubar_nub;
  std::unique_ptr<TH1> fFDNue_nub;
  std::unique_ptr<TH1> fFDNuebar_nub;

  bool fRead;

  PRISMFluxHelper() : fRead(false) {}

  void Read() {

    if (fRead) {
      return;
    }

    TH2 *NDNumu_293kA_nu;
    TH2 *NDNumubar_293kA_nu;
    TH2 *NDNue_293kA_nu;
    TH2 *NDNuebar_293kA_nu;
    TH2 *NDNumu_293kA_nub;
    TH2 *NDNumubar_293kA_nub;
    TH2 *NDNue_293kA_nub;
    TH2 *NDNuebar_293kA_nub;

    TH2 *NDNumu_280kA_nu;
    TH2 *NDNumubar_280kA_nu;
    TH2 *NDNue_280kA_nu;
    TH2 *NDNuebar_280kA_nu;
    TH2 *NDNumu_280kA_nub;
    TH2 *NDNumubar_280kA_nub;
    TH2 *NDNue_280kA_nub;
    TH2 *NDNuebar_280kA_nub;

    TH1 *FDNumu_nu;
    TH1 *FDNumubar_nu;
    TH1 *FDNue_nu;
    TH1 *FDNuebar_nu;
    TH1 *FDNumu_nub;
    TH1 *FDNumubar_nub;
    TH1 *FDNue_nub;
    TH1 *FDNuebar_nub;

    TFile fin(
        (FindCAFAnaDir() + "/PRISM/flux/LBNF_TDRFlux_Nov19.root").c_str());
    assert(fin.IsOpen());

    fin.GetObject("ND_293kA_nu_numu", NDNumu_293kA_nu);
    assert(bool(NDNumu_293kA_nu) && (NDNumu_293kA_nu->GetDimension() == 2));
    fin.GetObject("ND_293kA_nu_numubar", NDNumubar_293kA_nu);
    assert(bool(NDNumubar_293kA_nu) &&
           (NDNumubar_293kA_nu->GetDimension() == 2));
    fin.GetObject("ND_293kA_nu_nue", NDNue_293kA_nu);
    assert(bool(NDNue_293kA_nu) && (NDNue_293kA_nu->GetDimension() == 2));
    fin.GetObject("ND_293kA_nu_nuebar", NDNuebar_293kA_nu);
    assert(bool(NDNuebar_293kA_nu) && (NDNuebar_293kA_nu->GetDimension() == 2));
    fin.GetObject("ND_293kA_nub_numu", NDNumu_293kA_nub);
    assert(bool(NDNumu_293kA_nub) && (NDNumu_293kA_nub->GetDimension() == 2));
    fin.GetObject("ND_293kA_nub_numubar", NDNumubar_293kA_nub);
    assert(bool(NDNumubar_293kA_nub) &&
           (NDNumubar_293kA_nub->GetDimension() == 2));
    fin.GetObject("ND_293kA_nub_nue", NDNue_293kA_nub);
    assert(bool(NDNue_293kA_nub) && (NDNue_293kA_nub->GetDimension() == 2));
    fin.GetObject("ND_293kA_nub_nuebar", NDNuebar_293kA_nub);
    assert(bool(NDNuebar_293kA_nub) &&
           (NDNuebar_293kA_nub->GetDimension() == 2));

    fin.GetObject("ND_280kA_nu_numu", NDNumu_280kA_nu);
    assert(bool(NDNumu_280kA_nu) && (NDNumu_280kA_nu->GetDimension() == 2));
    fin.GetObject("ND_280kA_nu_numubar", NDNumubar_280kA_nu);
    assert(bool(NDNumubar_280kA_nu) &&
           (NDNumubar_280kA_nu->GetDimension() == 2));
    fin.GetObject("ND_280kA_nu_nue", NDNue_280kA_nu);
    assert(bool(NDNue_280kA_nu) && (NDNue_280kA_nu->GetDimension() == 2));
    fin.GetObject("ND_280kA_nu_nuebar", NDNuebar_280kA_nu);
    assert(bool(NDNuebar_280kA_nu) && (NDNuebar_280kA_nu->GetDimension() == 2));
    fin.GetObject("ND_280kA_nub_numu", NDNumu_280kA_nub);
    assert(bool(NDNumu_280kA_nub) && (NDNumu_280kA_nub->GetDimension() == 2));
    fin.GetObject("ND_280kA_nub_numubar", NDNumubar_280kA_nub);
    assert(bool(NDNumubar_280kA_nub) &&
           (NDNumubar_280kA_nub->GetDimension() == 2));
    fin.GetObject("ND_280kA_nub_nue", NDNue_280kA_nub);
    assert(bool(NDNue_280kA_nub) && (NDNue_280kA_nub->GetDimension() == 2));
    fin.GetObject("ND_280kA_nub_nuebar", NDNuebar_280kA_nub);
    assert(bool(NDNuebar_280kA_nub) &&
           (NDNuebar_280kA_nub->GetDimension() == 2));

    fin.GetObject("FD_nu_numu", FDNumu_nu);
    assert(bool(FDNumu_nu) && (FDNumu_nu->GetDimension() == 1));
    fin.GetObject("FD_nu_numubar", FDNumubar_nu);
    assert(bool(FDNumubar_nu) && (FDNumubar_nu->GetDimension() == 1));
    fin.GetObject("FD_nu_nue", FDNue_nu);
    assert(bool(FDNue_nu) && (FDNue_nu->GetDimension() == 1));
    fin.GetObject("FD_nu_nuebar", FDNuebar_nu);
    assert(bool(FDNuebar_nu) && (FDNuebar_nu->GetDimension() == 1));
    fin.GetObject("FD_nub_numu", FDNumu_nub);
    assert(bool(FDNumu_nub) && (FDNumu_nub->GetDimension() == 1));
    fin.GetObject("FD_nub_numubar", FDNumubar_nub);
    assert(bool(FDNumubar_nub) && (FDNumubar_nub->GetDimension() == 1));
    fin.GetObject("FD_nub_nue", FDNue_nub);
    assert(bool(FDNue_nub) && (FDNue_nub->GetDimension() == 1));
    fin.GetObject("FD_nub_nuebar", FDNuebar_nub);
    assert(bool(FDNuebar_nub) && (FDNuebar_nub->GetDimension() == 1));

    fNDNumu_293kA_nu = std::unique_ptr<TH2>(NDNumu_293kA_nu);
    fNDNumu_293kA_nu->SetDirectory(nullptr);
    fNDNumubar_293kA_nu = std::unique_ptr<TH2>(NDNumubar_293kA_nu);
    fNDNumubar_293kA_nu->SetDirectory(nullptr);
    fNDNue_293kA_nu = std::unique_ptr<TH2>(NDNue_293kA_nu);
    fNDNue_293kA_nu->SetDirectory(nullptr);
    fNDNuebar_293kA_nu = std::unique_ptr<TH2>(NDNuebar_293kA_nu);
    fNDNuebar_293kA_nu->SetDirectory(nullptr);
    fNDNumu_293kA_nub = std::unique_ptr<TH2>(NDNumu_293kA_nub);
    fNDNumu_293kA_nub->SetDirectory(nullptr);
    fNDNumubar_293kA_nub = std::unique_ptr<TH2>(NDNumubar_293kA_nub);
    fNDNumubar_293kA_nub->SetDirectory(nullptr);
    fNDNue_293kA_nub = std::unique_ptr<TH2>(NDNue_293kA_nub);
    fNDNue_293kA_nub->SetDirectory(nullptr);
    fNDNuebar_293kA_nub = std::unique_ptr<TH2>(NDNuebar_293kA_nub);
    fNDNuebar_293kA_nub->SetDirectory(nullptr);

    fNDNumu_280kA_nu = std::unique_ptr<TH2>(NDNumu_280kA_nu);
    fNDNumu_280kA_nu->SetDirectory(nullptr);
    fNDNumubar_280kA_nu = std::unique_ptr<TH2>(NDNumubar_280kA_nu);
    fNDNumubar_280kA_nu->SetDirectory(nullptr);
    fNDNue_280kA_nu = std::unique_ptr<TH2>(NDNue_280kA_nu);
    fNDNue_280kA_nu->SetDirectory(nullptr);
    fNDNuebar_280kA_nu = std::unique_ptr<TH2>(NDNuebar_280kA_nu);
    fNDNuebar_280kA_nu->SetDirectory(nullptr);
    fNDNumu_280kA_nub = std::unique_ptr<TH2>(NDNumu_280kA_nub);
    fNDNumu_280kA_nub->SetDirectory(nullptr);
    fNDNumubar_280kA_nub = std::unique_ptr<TH2>(NDNumubar_280kA_nub);
    fNDNumubar_280kA_nub->SetDirectory(nullptr);
    fNDNue_280kA_nub = std::unique_ptr<TH2>(NDNue_280kA_nub);
    fNDNue_280kA_nub->SetDirectory(nullptr);
    fNDNuebar_280kA_nub = std::unique_ptr<TH2>(NDNuebar_280kA_nub);
    fNDNuebar_280kA_nub->SetDirectory(nullptr);

    fFDNumu_nu = std::unique_ptr<TH1>(FDNumu_nu);
    fFDNumu_nu->SetDirectory(nullptr);
    fFDNumubar_nu = std::unique_ptr<TH1>(FDNumubar_nu);
    fFDNumubar_nu->SetDirectory(nullptr);
    fFDNue_nu = std::unique_ptr<TH1>(FDNue_nu);
    fFDNue_nu->SetDirectory(nullptr);
    fFDNuebar_nu = std::unique_ptr<TH1>(FDNuebar_nu);
    fFDNuebar_nu->SetDirectory(nullptr);
    fFDNumu_nub = std::unique_ptr<TH1>(FDNumu_nub);
    fFDNumu_nub->SetDirectory(nullptr);
    fFDNumubar_nub = std::unique_ptr<TH1>(FDNumubar_nub);
    fFDNumubar_nub->SetDirectory(nullptr);
    fFDNue_nub = std::unique_ptr<TH1>(FDNue_nub);
    fFDNue_nub->SetDirectory(nullptr);
    fFDNuebar_nub = std::unique_ptr<TH1>(FDNuebar_nub);
    fFDNuebar_nub->SetDirectory(nullptr);

    fRead = true;
  }

  std::unique_ptr<TH2> const &GetNDFluxPred(bool IsNuMode, int nu_pdg,
                                            int isSpecHCRun) {
    switch (nu_pdg) {
    case 12: {
      return IsNuMode ? (isSpecHCRun ? fNDNue_280kA_nu : fNDNue_293kA_nu)
                      : (isSpecHCRun ? fNDNue_280kA_nub : fNDNue_293kA_nub);
    }
    case -12: {
      return IsNuMode
                 ? (isSpecHCRun ? fNDNuebar_280kA_nu : fNDNuebar_293kA_nu)
                 : (isSpecHCRun ? fNDNuebar_280kA_nub : fNDNuebar_293kA_nub);
    }
    case 14: {
      return IsNuMode ? (isSpecHCRun ? fNDNumu_280kA_nu : fNDNumu_293kA_nu)
                      : (isSpecHCRun ? fNDNumu_280kA_nub : fNDNumu_293kA_nub);
    }
    case -14: {
      return IsNuMode
                 ? (isSpecHCRun ? fNDNumubar_280kA_nu : fNDNumubar_293kA_nu)
                 : (isSpecHCRun ? fNDNumubar_280kA_nub : fNDNumubar_293kA_nub);
    }
    default: {
      std::cout << "[ERROR]: Invalid nu pdg: " << nu_pdg << std::endl;
      abort();
    }
    }
  }
  std::unique_ptr<TH1> const &GetFDFluxPred(bool IsNuMode, int nu_pdg) {
    switch (nu_pdg) {
    case 12: {
      return IsNuMode ? fFDNue_nu : fFDNue_nub;
    }
    case -12: {
      return IsNuMode ? fFDNuebar_nu : fFDNuebar_nub;
    }
    case 14: {
      return IsNuMode ? fFDNumu_nu : fFDNumu_nub;
    }
    case -14: {
      return IsNuMode ? fFDNumubar_nu : fFDNumubar_nub;
    }
    default: {
      std::cout << "[ERROR]: Invalid nu pdg: " << nu_pdg << std::endl;
      abort();
    }
    }
  }
};

std::unique_ptr<FluxPrediction> FluxPrediction::LoadFrom(TDirectory *dir) {
  return std::unique_ptr<FluxPrediction>(new FluxPrediction(
      *ana::LoadFrom<Spectrum>(dir->GetDirectory("spect_numu")),
      *ana::LoadFrom<Spectrum>(dir->GetDirectory("spect_numubar")),
      *ana::LoadFrom<Spectrum>(dir->GetDirectory("spect_nue")),
      *ana::LoadFrom<Spectrum>(dir->GetDirectory("spect_nuebar"))));
}
void FluxPrediction::SaveTo(TDirectory *dir) const {
  TDirectory *tmp = gDirectory;

  dir->cd();

  TObjString("FluxPrediction").Write("type");

  fSpectrumNumu.SaveTo(dir->mkdir("spect_numu"));
  fSpectrumNumubar.SaveTo(dir->mkdir("spect_numubar"));
  fSpectrumNue.SaveTo(dir->mkdir("spect_nue"));
  fSpectrumNuebar.SaveTo(dir->mkdir("spect_nuebar"));

  tmp->cd();
}

namespace {
void OscHist(osc::IOscCalculator *calc, TH1 *h, int from, int to) {
  for (int bi_it = 0; bi_it < h->GetXaxis()->GetNbins(); ++bi_it) {
    double bl = h->GetXaxis()->GetBinLowEdge(bi_it + 1);
    double bu = h->GetXaxis()->GetBinUpEdge(bi_it + 1);
    double step = ((bu - bl) / 10.0);

    double avg_weight = 0;
    double avger = 0;
    for (int it = 0; it < 10; ++it) {
      double e = bl + (double(it) + 0.5) * step;
      if (calc->P(from, to, e) > 1) {
        continue;
      }
      avg_weight += calc->P(from, to, e);
      avger++;
    }

    if (avger == 0) {
      avg_weight = 1;
    } else {
      avg_weight /= avger;
    }

    h->SetBinContent(bi_it + 1, h->GetBinContent(bi_it + 1) * avg_weight);
    h->SetBinError(bi_it + 1, h->GetBinError(bi_it + 1) * avg_weight);
  }
}
} // namespace

Spectrum FluxPrediction::PredictComponent(osc::IOscCalculator *calc,
                                          Flavors::Flavors_t flav,
                                          Current::Current_t,
                                          Sign::Sign_t sign) const {

  TH1D *total = fSpectrumNumu.ToTH1(1);
  total->Reset();

  using namespace Flavors;
  int nu_nubar = (sign == Sign::kNu) ? 1 : -1;

  if (flav & kNuEToNuE) {
    TH1D *nue = ((nu_nubar > 0) ? fSpectrumNue : fSpectrumNuebar).ToTH1(1);
    OscHist(calc, nue, nu_nubar * 12, nu_nubar * 12);

    total->Add(nue);
    HistCache::Delete(nue);
  }
  if (flav & kNuEToNuMu) {
    TH1D *nue = ((nu_nubar > 0) ? fSpectrumNue : fSpectrumNuebar).ToTH1(1);
    OscHist(calc, nue, nu_nubar * 12, nu_nubar * 14);

    total->Add(nue);
    HistCache::Delete(nue);
  }

  if (flav & kNuMuToNuE) {
    TH1D *numu = ((nu_nubar > 0) ? fSpectrumNumu : fSpectrumNumubar).ToTH1(1);
    OscHist(calc, numu, nu_nubar * 14, nu_nubar * 12);

    total->Add(numu);
    HistCache::Delete(numu);
  }
  if (flav & kNuMuToNuMu) {
    TH1D *numu = ((nu_nubar > 0) ? fSpectrumNumu : fSpectrumNumubar).ToTH1(1);
    OscHist(calc, numu, nu_nubar * 14, nu_nubar * 14);

    total->Add(numu);
    HistCache::Delete(numu);
  }

  Spectrum rtn = fSpectrumNumu;
  rtn.Clear();
  rtn.FillFromHistogram(total);

  HistCache::Delete(total);

  return rtn;
}

Spectrum FillFluxSpectrum(HistAxis const &axis, std::unique_ptr<TH1> const &fl,
                          SystShifts shiftMC, int nu_pdg, bool IsNuMode) {
  Spectrum spec(axis.GetLabels(), axis.GetBinnings());
  spec.Clear();

  TH1 *spec_h = spec.ToTH1(1);

  shiftMC = FilterFluxSystShifts(shiftMC);
  std::vector<std::pair<size_t, double>> FluxShifts;

  for (auto s : shiftMC.ActiveSysts()) {
    size_t idx = GetFluxSystIndex(s);
    if (idx == ana::kNotValidFluxSyst) {
      continue;
    }
    double shift = shiftMC.GetShift(s);
    FluxShifts.push_back(std::make_pair(idx, shift));
  }

  for (int i = 0; i < spec_h->GetXaxis()->GetNbins(); ++i) {

    double bl = spec_h->GetXaxis()->GetBinLowEdge(i + 1);
    double bu = spec_h->GetXaxis()->GetBinUpEdge(i + 1);
    double step = ((bu - bl) / 10.0);

    // std::cout << "E bin = " << i << ", " << bl << " -> " << bu << std::endl;

    double avg_flux = 0;
    for (int it = 0; it < 10; ++it) {
      double e = bl + (double(it) + 0.5) * step;

      // std::cout << "\tE Substep: " << e << std::endl;

      int xbi = fl->GetXaxis()->FindFixBin(e);

      double shift_weight = 1;
      for (size_t si = 0; si < FluxShifts.size(); ++si) {
        shift_weight *= OffAxisFluxUncertaintyHelper::Get().GetFluxWeight(
            FluxShifts[si].first, FluxShifts[si].second, e, 0, nu_pdg,
            false /*IsND*/, IsNuMode, false /*IsSpecRun*/);
      }

      avg_flux += fl->GetBinContent(xbi) * shift_weight;
    }

    // std::cout << "Avg flux = " << (avg_flux / 10.0) << std::endl;

    avg_flux /= 10.0;
    spec_h->SetBinContent(i + 1, avg_flux);
  }
  spec.FillFromHistogram(spec_h);
  spec.OverridePOT(1);
  return spec;
}

std::unique_ptr<IPrediction>
FluxPredictionGenerator::Generate(Loaders &loaders,
                                  const SystShifts &shiftMC) const {

  static PRISMFluxHelper fluxhelper;
  static bool first = true;
  if (first) {
    fluxhelper.Read();
    first = false;
  }

  return std::unique_ptr<IPrediction>(new FluxPrediction(
      FillFluxSpectrum(fAxis, fluxhelper.GetFDFluxPred(fIsNuMode, 14), shiftMC,
                       14, fIsNuMode),
      FillFluxSpectrum(fAxis, fluxhelper.GetFDFluxPred(fIsNuMode, -14), shiftMC,
                       -14, fIsNuMode),
      FillFluxSpectrum(fAxis, fluxhelper.GetFDFluxPred(fIsNuMode, 12), shiftMC,
                       12, fIsNuMode),
      FillFluxSpectrum(fAxis, fluxhelper.GetFDFluxPred(fIsNuMode, -12), shiftMC,
                       -12, fIsNuMode)));
}

std::unique_ptr<OffAxisFluxPrediction>
OffAxisFluxPrediction::LoadFrom(TDirectory *dir) {
  return std::unique_ptr<OffAxisFluxPrediction>(new OffAxisFluxPrediction(
      *ana::LoadFrom<Spectrum>(dir->GetDirectory("spect_numu")),
      *ana::LoadFrom<Spectrum>(dir->GetDirectory("spect_numubar")),
      *ana::LoadFrom<Spectrum>(dir->GetDirectory("spect_nue")),
      *ana::LoadFrom<Spectrum>(dir->GetDirectory("spect_nuebar"))));
}
void OffAxisFluxPrediction::SaveTo(TDirectory *dir) const {
  TDirectory *tmp = gDirectory;

  dir->cd();

  TObjString("OffAxisFluxPrediction").Write("type");

  fSpectrumNumu.SaveTo(dir->mkdir("spect_numu"));
  fSpectrumNumubar.SaveTo(dir->mkdir("spect_numubar"));
  fSpectrumNue.SaveTo(dir->mkdir("spect_nue"));
  fSpectrumNuebar.SaveTo(dir->mkdir("spect_nuebar"));

  tmp->cd();
}

Spectrum OffAxisFluxPrediction::PredictComponent(osc::IOscCalculator *calc,
                                                 Flavors::Flavors_t flav,
                                                 Current::Current_t,
                                                 Sign::Sign_t sign) const {
  using namespace Flavors;
  int nu_nubar = (sign == Sign::kNu) ? 1 : -1;

  if ((flav & kNuEToNuE) || (flav & kNuEToNuMu)) {
    return ((nu_nubar > 0) ? fSpectrumNue : fSpectrumNuebar);
  }

  if ((flav & kNuMuToNuE) || (flav & kNuMuToNuMu)) {
    return ((nu_nubar > 0) ? fSpectrumNumu : fSpectrumNumubar);
  }

  std::cout << "[ERROR]: Invalid flavor requested for "
               "OffAxisFluxPrediction::PredictComponent: "
            << flav << std::endl;
  abort();
}

Spectrum FillOffAxisFluxSpectrum(HistAxis const &axis,
                                 std::unique_ptr<TH2> const &fl,
                                 SystShifts shiftMC, int nu_pdg, bool IsNuMode,
                                 bool isSpecHCRun) {
  Spectrum spec(axis.GetLabels(), axis.GetBinnings());
  spec.Clear();

  TH2 *spec_h = spec.ToTH2(1);

  // std::cout << "[INFO]: Filling off axis flux from histogram: " <<
  // fl->GetName()
  //           << std::endl;
  // std::cout << "\t " << fl->GetXaxis()->GetNbins()
  //           << " x bins from: " << fl->GetXaxis()->GetBinLowEdge(1) << " -> "
  //           << fl->GetXaxis()->GetBinUpEdge(fl->GetXaxis()->GetNbins())
  //           << std::endl;
  // std::cout << "\t " << fl->GetYaxis()->GetNbins()
  //           << " y bins from: " << fl->GetYaxis()->GetBinLowEdge(1) << " -> "
  //           << fl->GetYaxis()->GetBinUpEdge(fl->GetYaxis()->GetNbins())
  //           << std::endl;
  // std::cout << "[INFO]: Flux binning: " << spec_h->GetName() << std::endl;
  // std::cout << "\t " << spec_h->GetXaxis()->GetNbins()
  //           << " x bins from: " << spec_h->GetXaxis()->GetBinLowEdge(1)
  //           << " -> "
  //           <<
  //           spec_h->GetXaxis()->GetBinUpEdge(spec_h->GetXaxis()->GetNbins())
  //           << std::endl;
  // std::cout << "\t " << spec_h->GetYaxis()->GetNbins()
  //           << " y bins from: " << spec_h->GetYaxis()->GetBinLowEdge(1)
  //           << " -> "
  //           <<
  //           spec_h->GetYaxis()->GetBinUpEdge(spec_h->GetYaxis()->GetNbins())
  //           << std::endl;

  size_t nallshifts = shiftMC.ActiveSysts().size();
  shiftMC = FilterFluxSystShifts(shiftMC);
  size_t nfluxshifts = shiftMC.ActiveSysts().size();
  // std::cout << "With " << nfluxshifts << "/" << nallshifts << " flux shifts"
  //           << std::endl;

  std::vector<std::pair<size_t, double>> FluxShifts;

  for (auto s : shiftMC.ActiveSysts()) {
    size_t idx = GetFluxSystIndex(s);
    if (idx == ana::kNotValidFluxSyst) {
      continue;
    }
    double shift = shiftMC.GetShift(s);
    FluxShifts.push_back(std::make_pair(idx, shift));
  }

  bool isneg = std::abs(spec_h->GetYaxis()->GetBinLowEdge(1)) >=
               std::abs(spec_h->GetYaxis()->GetBinUpEdge(
                   spec_h->GetYaxis()->GetNbins()));

  for (int j = 0; j < spec_h->GetYaxis()->GetNbins(); ++j) {
    double offaxis_m =
        (isneg ? -1.0 : 1.0) * spec_h->GetYaxis()->GetBinCenter(j + 1);

    // std::cout << "Reading off axis bin: " << j << ", " << offaxis_m << " -> "
    //           << fl->GetYaxis()->FindFixBin(offaxis_m) << " flux bin"
    //           << std::endl;

    for (int i = 0; i < spec_h->GetXaxis()->GetNbins(); ++i) {

      double bl = spec_h->GetXaxis()->GetBinLowEdge(i + 1);
      double bu = spec_h->GetXaxis()->GetBinUpEdge(i + 1);
      double step = ((bu - bl) / 10.0);

      // std::cout << "\tE bin = " << i << ", " << bl << " -> " << bu <<
      // std::endl;

      double avg_flux = 0;

      for (int it = 0; it < 10; ++it) {
        double e = bl + (double(it) + 0.5) * step;
        int xbi = fl->GetXaxis()->FindFixBin(e);
        int ybi = fl->GetYaxis()->FindFixBin(offaxis_m);
        // if (isSpecHCRun) {
        //   std::cout << "\t\tE Substep: " << e << "(" << bl << "+"
        //             << (double(it) + 0.5) << "*" << step << "), flux xbin "
        //             << xbi << ", flux ybin: " << ybi << "(" << offaxis_m
        //             << " m)" << std::endl;
        // }

        double shift_weight = 1;
        for (size_t si = 0; si < FluxShifts.size(); ++si) {
          shift_weight *= OffAxisFluxUncertaintyHelper::Get().GetFluxWeight(
              FluxShifts[si].first, FluxShifts[si].second, e, offaxis_m, nu_pdg,
              true /*IsND*/, IsNuMode, isSpecHCRun);
        }

        avg_flux += fl->GetBinContent(xbi, ybi) * shift_weight;
      }

      // std::cout << "\tAvg flux = " << (avg_flux / 10.0) << std::endl;

      avg_flux /= 10.0;
      spec_h->SetBinContent(i + 1, j + 1, avg_flux);
    }
  }
  spec.FillFromHistogram(spec_h);
  spec.OverridePOT(1);

  return spec;
}

std::unique_ptr<IPrediction>
OffAxisFluxPredictionGenerator::Generate(Loaders &loaders,
                                         const SystShifts &shiftMC) const {

  static PRISMFluxHelper fluxhelper;
  static bool first = true;
  if (first) {
    fluxhelper.Read();
    first = false;
  }

  return std::unique_ptr<IPrediction>(new OffAxisFluxPrediction(
      FillOffAxisFluxSpectrum(
          fAxis, fluxhelper.GetNDFluxPred(fIsNuMode, 14, fIsSpecRun), shiftMC,
          14, fIsNuMode, fIsSpecRun),
      FillOffAxisFluxSpectrum(
          fAxis, fluxhelper.GetNDFluxPred(fIsNuMode, -14, fIsSpecRun), shiftMC,
          -14, fIsNuMode, fIsSpecRun),
      FillOffAxisFluxSpectrum(
          fAxis, fluxhelper.GetNDFluxPred(fIsNuMode, 12, fIsSpecRun), shiftMC,
          12, fIsNuMode, fIsSpecRun),
      FillOffAxisFluxSpectrum(
          fAxis, fluxhelper.GetNDFluxPred(fIsNuMode, -12, fIsSpecRun), shiftMC,
          -12, fIsNuMode, fIsSpecRun)));
}

} // namespace ana
