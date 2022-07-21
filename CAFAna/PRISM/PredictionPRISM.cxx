ource build/Linux/CAFAnaEnv.sh
source support/support_software_env.sh
include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/PRISM/Cuts.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PredictionPRISM.h"

#include "CAFAna/Prediction/PredictionsForPRISM.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/OscCurve.h"
#include "CAFAna/Core/Spectrum.h"

#include "CAFAna/Analysis/AnalysisVars.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/Systs/RecoEnergyFDSysts.h"
#include "CAFAna/Systs/RecoEnergyNDSysts.h"

#include "TAxis.h"
#include "TDirectory.h"
#include "TH2.h"
#include "TObjString.h"

using namespace PRISM;

namespace ana {

static std::unique_ptr<PRISMReweightableSpectrum> kNoSuchNDDataSpectrum{
    nullptr};
static std::unique_ptr<PredictionInterp> kNoSuchNDPredictionSpectrum{nullptr};
static std::unique_ptr<PredictionInterp> kNoSuchFDPredictionSpectrum{nullptr};

PredictionPRISM::PredictionPRISM(
    const HistAxis &AnalysisAxisND, const HistAxis &AnalysisAxisFD,
    const HistAxis &NDOffAxis, const HistAxis &ND280kAAxis,
    const HistAxis &TrueAnalysisAxis, const HistAxis &NDFDEnergyMatchAxis,
    const HistAxis &CovarianceAxis, const HistAxis &OffPredictionAxis,
    const HistAxis &_280kAPredictionAxis,
    const HistAxis &FluxMatcherCorrectionAxes)
    : fAnalysisAxisND(AnalysisAxisND), fAnalysisAxisFD(AnalysisAxisFD),
      fNDOffAxis(NDOffAxis), fND280kAAxis(ND280kAAxis),
      fTrueAnalysisAxis(TrueAnalysisAxis),
      fNDFDEnergyMatchAxis(NDFDEnergyMatchAxis),
      fCovarianceAxis(CovarianceAxis), fOffPredictionAxis(OffPredictionAxis),
      f280kAPredictionAxis(_280kAPredictionAxis),
      fFluxMatcherCorrectionAxes(FluxMatcherCorrectionAxes) {}

void PredictionPRISM::Initialize() {
  fSetNDErrorsFromRate = true;
  fVaryNDFDMCData = false;
  fUseFakeData = false;

  fDefaultOffAxisPOT = 1.1E20;

  PRISMOUT("PRISM analysis axis: "
           << fAnalysisAxisFD.GetLabels().front() << " with "
           << fAnalysisAxisFD.GetBinnings().front().Edges().size()
           << " bins from "
           << fAnalysisAxisFD.GetBinnings().front().Edges().front() << " to "
           << fAnalysisAxisFD.GetBinnings().front().Edges().back() << ".");

  PRISMOUT("PRISM Off axis axis: "
           << fNDOffAxis.GetLabels().front() << " with "
           << fNDOffAxis.GetBinnings().front().Edges().size() << " bins from "
           << fNDOffAxis.GetBinnings().front().Edges().front() << " to "
           << fNDOffAxis.GetBinnings().front().Edges().back() << ".");

  PRISMOUT("PRISM 280 kA axis: "
           << fND280kAAxis.GetLabels().front() << " with "
           << fND280kAAxis.GetBinnings().front().Edges().size() << " bins from "
           << fND280kAAxis.GetBinnings().front().Edges().front() << " to "
           << fND280kAAxis.GetBinnings().front().Edges().back() << ".");

  PRISMOUT("PRISM NDFDEnergyMatchAxis axis: "
           << fNDFDEnergyMatchAxis.GetLabels().front() << " with "
           << fNDFDEnergyMatchAxis.GetBinnings().front().Edges().size()
           << " bins from "
           << fNDFDEnergyMatchAxis.GetBinnings().front().Edges().front()
           << " to "
           << fNDFDEnergyMatchAxis.GetBinnings().front().Edges().back() << ".");

  fFluxMatcher = nullptr;

  fNDFD_Matrix = nullptr;

  fNCCorrection = true;
  fWSBCorrection = true;
  fWLBCorrection = true;
  fNuTauCCCorrection = true;
  fIntrinsicCorrection = true;

  fAxisAgreement = true;
  for (size_t bin = 0; bin < fAnalysisAxisFD.GetBinnings().size(); bin++) {
    for (size_t ed = 0;
         ed < fAnalysisAxisFD.GetBinnings().at(bin).Edges().size(); ed++) {
      double FDedge = fAnalysisAxisFD.GetBinnings().at(bin).Edges().at(ed);
      double NDedge = fAnalysisAxisND.GetBinnings().at(bin).Edges().at(ed);
      if (FDedge != NDedge) {
        fAxisAgreement = false;
        break;
      }
    }
  }
}

///\brief Call to add a ND Data component
///
///\details This can be called a number of times to add various ND 'data'
/// components that might be used in the PRISM analysis. Components are
/// defined by the combination ofNuChan andBeamMode.
/// The list of valid components is given below:
/// * NumuIntrinsic, NuMode (i.e. right sign mu-like FHC)
/// * NumuBarIntrinsic, NuBarMode (i.e. right sign mu-like RHC)
/// * NumuBarIntrinsic, NuMode (i.e. wrong sign mu-like FHC)
/// * NumuIntrinsic, NuBarMode (i.e. wrong sign mu-like FHC)
/// * NueNueBar, NuMode (i.e. all e-like FHC)
/// * NueNueBar, NuBarMode (i.e. all e-like RHC)
///
/// The SystShifts instance allows the 'data' to be shifted relative to the
/// nominal
void PredictionPRISM::AddNDDataLoader(SpectrumLoaderBase &ND_loader,
                                      const Cut &cut, const Weight &wei,
                                      ana::SystShifts shift,
                                      PRISM::BeamChan NDChannel) {
  std::unique_ptr<PRISMReweightableSpectrum> &NDData =
      GetNDData(NDChannel, 293);
  std::unique_ptr<PRISMReweightableSpectrum> &NDFakeDataBias =
      GetNDData(NDChannel, 293, true);
  if (&NDData == &kNoSuchNDDataSpectrum ||
      &NDFakeDataBias == &kNoSuchNDDataSpectrum) {
    std::cout << "ERROR: Invalid ND data type passed: " << NDChannel.chan << ":"
              << NDChannel.mode << std::endl;
    abort();
  }

  // Corrects for non-uniform off-axis binning
  auto slice_width_weight =
      NDSliceCorrection(50, fNDOffAxis.GetBinnings().front().Edges());
  auto slice_width_weight_280kA =
      NDSliceCorrection(50, fND280kAAxis.GetBinnings().front().Edges());

  NDData = std::make_unique<PRISMReweightableSpectrum>(
      ND_loader, fAnalysisAxisND, fNDOffAxis, cut && kCut280kARun, kNoShift,
      wei * slice_width_weight);

  NDFakeDataBias = std::make_unique<PRISMReweightableSpectrum>(
      ND_loader, fAnalysisAxisND, fNDOffAxis, cut && kCut280kARun, shift,
      wei * slice_width_weight);

  std::unique_ptr<PRISMReweightableSpectrum> &NDData_280kA =
      GetNDData(NDChannel, 280);
  std::unique_ptr<PRISMReweightableSpectrum> &NDFakeDataBias_280kA =
      GetNDData(NDChannel, 280, true);
  if (&NDData_280kA == &kNoSuchNDDataSpectrum ||
      &NDFakeDataBias_280kA == &kNoSuchNDDataSpectrum) {
    std::cout << "ERROR: Invalid ND data type passed: " << NDChannel.chan << ":"
              << NDChannel.mode << std::endl;
    abort();
  }
  NDData_280kA = std::make_unique<PRISMReweightableSpectrum>(
      ND_loader, fAnalysisAxisND, fND280kAAxis, cut && kSel280kARun, kNoShift,
      wei * slice_width_weight_280kA);

  NDFakeDataBias_280kA = std::make_unique<PRISMReweightableSpectrum>(
      ND_loader, fAnalysisAxisND, fND280kAAxis, cut && kSel280kARun, shift,
      wei * slice_width_weight_280kA);
}

///\brief Call to add a ND MC component
///
///\details This can be called a number of times to add various ND MC
/// components that might be used in the PRISM analysis. Components are
/// defined by the combination ofNuChan andBeamMode.
/// The list of valid components is given below:
/// * NumuIntrinsic, NuMode (i.e. right sign mu-like FHC)
/// * NumuBarIntrinsic, NuBarMode (i.e. right sign mu-like RHC)
/// * NumuBarIntrinsic, NuMode (i.e. wrong sign mu-like FHC)
/// * NumuIntrinsic, NuBarMode (i.e. wrong sign mu-like FHC)
/// * NueNueBar, NuMode (i.e. all e-like FHC)
/// * NueNueBar, NuBarMode (i.e. all e-like RHC)
///
/// The systlist add allowable systematic shifts to the constructed PredInterp
/// for use in fits and systematic studies.
void PredictionPRISM::AddNDMCLoader(Loaders &loaders, const Cut &cut,
                                    const Weight &wei,
                                    std::vector<ana::ISyst const *> systlist,
                                    osc::IOscCalc *calc,
                                    PRISM::BeamChan NDChannel) {

  // Corrects for non-uniform off-axis binning
  auto slice_width_weight =
      NDSliceCorrection(50, fNDOffAxis.GetBinnings().front().Edges());
  auto slice_width_weight_280kA =
      NDSliceCorrection(50, fND280kAAxis.GetBinnings().front().Edges());

  // ana::SystShifts DataShift =
  // GetFakeDataGeneratorSystShift("MissingProtonFakeData_pos");

  std::unique_ptr<PredictionInterp> &NDPrediction = GetNDPrediction(NDChannel);
  if (&NDPrediction == &kNoSuchNDPredictionSpectrum) {
    std::cout << "ERROR: Invalid ND MC type passed: " << NDChannel.chan << ":"
              << NDChannel.mode << std::endl;
    abort();
  }

  fPredGens.push_back(std::make_unique<NoOscPredictionGenerator>(
      fOffPredictionAxis, cut && kCut280kARun, wei * slice_width_weight));

  NDPrediction = std::make_unique<PredictionInterp>(
      systlist, calc, *fPredGens.back(), loaders, kNoShift,
      PredictionInterp::kSplitBySign);

  std::unique_ptr<PredictionInterp> &NDPrediction_280kA =
      GetNDPrediction(NDChannel, 280);
  if (&NDPrediction_280kA == &kNoSuchNDPredictionSpectrum) {
    std::cout << "ERROR: Invalid ND MC type passed: " << NDChannel.chan << ":"
              << NDChannel.mode << std::endl;
    abort();
  }

  fPredGens.push_back(std::make_unique<NoOscPredictionGenerator>(
      f280kAPredictionAxis, cut && kSel280kARun,
      wei * slice_width_weight_280kA));

  NDPrediction_280kA = std::make_unique<PredictionInterp>(
      systlist, calc, *fPredGens.back(), loaders, kNoShift,
      PredictionInterp::kSplitBySign);
}

///\brief Call to add a FD MC component
///
///\details This can be called a number of times to add various FD MC
/// components that might be used in the PRISM analysis. Components are
/// defined by the combination ofNuChan andBeamMode.
/// The list of valid components is given below:
/// * NumuNumuBar, NuMode (i.e. mu-like FHC)
/// * NumuNumuBar, NuBarMode (i.e. mu-like RHC)
/// * NueNueBar, NuMode (i.e. all e-like FHC)
/// * NueNueBar, NuBarMode (i.e. all e-like RHC)
///
/// The systlist add allowable systematic shifts to the constructed PredInterp
/// for use in fits and systematic studies.
void PredictionPRISM::AddFDMCLoader(Loaders &loaders, const Cut &cut,
                                    const Weight &wei,
                                    std::vector<ana::ISyst const *> systlist,
                                    osc::IOscCalc *calc,
                                    PRISM::BeamChan FDChannel) {

  osc::NoOscillations kNoOsc;

  // ana::SystShifts DataShift =
  // GetFakeDataGeneratorSystShift("MissingProtonFakeData_pos");

  std::unique_ptr<PredictionInterp> &FDPrediction = GetFDPrediction(FDChannel);
  if (&FDPrediction == &kNoSuchFDPredictionSpectrum) {
    std::cout << "ERROR: Invalid FD MC type passed: " << FDChannel.chan << ":"
              << FDChannel.mode << std::endl;
    abort();
  }

  fPredGens.push_back(
      std::make_unique<NoExtrapPredictionGenerator>(fAnalysisAxisFD, cut, wei));
  FDPrediction = std::make_unique<PredictionInterp>(
      systlist, calc, *fPredGens.back(), loaders, kNoShift,
      PredictionInterp::kSplitBySign);

  std::unique_ptr<PredictionInterp> &FDUnOscWeightedSigPrediction =
      GetFDUnOscWeightedSigPrediction(FDChannel);
  if (&FDUnOscWeightedSigPrediction == &kNoSuchFDPredictionSpectrum) {
    std::cout << "ERROR: Invalid FD MC type passed: " << FDChannel.chan << ":"
              << FDChannel.mode << std::endl;
    abort();
  }

  // Prediction for MC correction. Make sure there is no WSB!
  fPredGens.push_back(std::make_unique<FDNoOscPredictionGenerator>(
      fFluxMatcherCorrectionAxes, cut, wei));
  FDUnOscWeightedSigPrediction = std::make_unique<PredictionInterp>(
      systlist, calc, *fPredGens.back(), loaders, kNoShift,
      PredictionInterp::kSplitBySign);

  // Prediction to oscillate numus by appearance probability.
  std::unique_ptr<PredictionInterp> &FDNonSwapAppOscPrediction =
      GetFDNonSwapAppOscPrediction(FDChannel.mode);
  // Always want unselected numu cut for this.
  auto kFDNumuCut = (FDChannel.mode == BeamMode::kNuMode)
                        ? kPRISMFDSignal_True_numu
                        : kPRISMFDSignal_True_numub;
  // Try FDNoOscPredictionGenerator
  // fPredGens.push_back(std::make_unique<NonSwapNoExtrapPredictionGenerator>(
  fPredGens.push_back(std::make_unique<FDNoOscPredictionGenerator>(
      fTrueAnalysisAxis, kFDNumuCut, wei));
  FDNonSwapAppOscPrediction = std::make_unique<PredictionInterp>(
      systlist, calc, *fPredGens.back(), loaders, kNoShift,
      PredictionInterp::kSplitBySign);

  // Prediction to get oscillate nue appearance spectrum.
  std::unique_ptr<PredictionInterp> &FDNueSwapAppOscPrediction =
      GetFDNueSwapAppOscPrediction(FDChannel.mode);
  // Always want unselected nue cut for this.
  auto kFDNueCut = (FDChannel.mode == BeamMode::kNuMode)
                       ? kPRISMFDSignal_True_nue
                       : kPRISMFDSignal_True_nueb;

  // fPredGens.push_back(std::make_unique<NoExtrapPredictionGenerator>(
  fPredGens.push_back(std::make_unique<FDNoOscPredictionGenerator>(
      fTrueAnalysisAxis, kFDNueCut, wei));
  FDNueSwapAppOscPrediction = std::make_unique<PredictionInterp>(
      systlist, calc, *fPredGens.back(), loaders, kNoShift,
      PredictionInterp::kSplitBySign);

  // Numu/nue xsec corr in true neutrino energy used in including FD intrinsic
  // nue in flux matching Copied above and used fNDFDEnergyMatchAxis instead

  // Prediction to oscillate numus by appearance probability.
  std::unique_ptr<PredictionInterp> &FDNonSwapAppOscPredictionTrueEnu =
      GetFDNonSwapAppOscPredictionTrueEnu(FDChannel.mode);
  fPredGens.push_back(std::make_unique<FDNoOscPredictionGenerator>(
      fNDFDEnergyMatchAxis, kFDNumuCut, wei)); // true neutrino energy axis
  FDNonSwapAppOscPredictionTrueEnu = std::make_unique<PredictionInterp>(
      systlist, calc, *fPredGens.back(), loaders, kNoShift,
      PredictionInterp::kSplitBySign);

  // Prediction to get oscillate nue appearance spectrum.
  std::unique_ptr<PredictionInterp> &FDNueSwapAppOscPredictionTrueEnu =
      GetFDNueSwapAppOscPredictionTrueEnu(FDChannel.mode);
  fPredGens.push_back(std::make_unique<FDNoOscPredictionGenerator>(
      fNDFDEnergyMatchAxis, kFDNueCut, wei));
  FDNueSwapAppOscPredictionTrueEnu = std::make_unique<PredictionInterp>(
      systlist, calc, *fPredGens.back(), loaders, kNoShift,
      PredictionInterp::kSplitBySign);
}

//-----------------------------------------------

std::unique_ptr<PRISMReweightableSpectrum> &
PredictionPRISM::GetNDData_right_sign_numu(BeamMode NDBM, int kA,
                                           bool shift) const {
  if (NDBM == BeamMode::kNuMode) {
    return (kA == 293) ? (shift ? Measurements.ND_293kA.numu_ccinc_sel_numode_fd
                                : Measurements.ND_293kA.numu_ccinc_sel_numode)
                       : (shift ? Measurements.ND_280kA.numu_ccinc_sel_numode_fd
                                : Measurements.ND_280kA.numu_ccinc_sel_numode);
  } else if (NDBM == BeamMode::kNuBarMode) {
    return (kA == 293)
               ? (shift ? Measurements.ND_293kA.numubar_ccinc_sel_nubmode_fd
                        : Measurements.ND_293kA.numubar_ccinc_sel_nubmode)
               : (shift ? Measurements.ND_280kA.numubar_ccinc_sel_nubmode_fd
                        : Measurements.ND_280kA.numubar_ccinc_sel_nubmode);
  }
  return kNoSuchNDDataSpectrum;
}
std::unique_ptr<PRISMReweightableSpectrum> &
PredictionPRISM::GetNDData_right_sign_nue(BeamMode NDBM, int kA,
                                          bool shift) const {
  if (NDBM == BeamMode::kNuMode) {
    return (kA == 293) ? (shift ? Measurements.ND_293kA.nue_ccinc_sel_numode_fd
                                : Measurements.ND_293kA.nue_ccinc_sel_numode)
                       : (shift ? Measurements.ND_280kA.nue_ccinc_sel_numode_fd
                                : Measurements.ND_280kA.nue_ccinc_sel_numode);
  } else if (NDBM == BeamMode::kNuBarMode) {
    return (kA == 293)
               ? (shift ? Measurements.ND_293kA.nuebar_ccinc_sel_nubmode_fd
                        : Measurements.ND_293kA.nuebar_ccinc_sel_nubmode)
               : (shift ? Measurements.ND_280kA.nuebar_ccinc_sel_nubmode_fd
                        : Measurements.ND_280kA.nuebar_ccinc_sel_nubmode);
  }
  return kNoSuchNDDataSpectrum;
}
std::unique_ptr<PRISMReweightableSpectrum> &
PredictionPRISM::GetNDData_wrong_sign_numu(BeamMode NDBM, int kA,
                                           bool shift) const {
  if (NDBM == BeamMode::kNuMode) {
    return (kA == 293)
               ? (shift ? Measurements.ND_293kA.numubar_ccinc_sel_numode_fd
                        : Measurements.ND_293kA.numubar_ccinc_sel_numode)
               : (shift ? Measurements.ND_280kA.numubar_ccinc_sel_numode_fd
                        : Measurements.ND_280kA.numubar_ccinc_sel_numode);
  } else if (NDBM == BeamMode::kNuBarMode) {
    return (kA == 293)
               ? (shift ? Measurements.ND_293kA.numu_ccinc_sel_nubmode_fd
                        : Measurements.ND_293kA.numu_ccinc_sel_nubmode)
               : (shift ? Measurements.ND_280kA.numu_ccinc_sel_nubmode_fd
                        : Measurements.ND_280kA.numu_ccinc_sel_nubmode);
  }
  return kNoSuchNDDataSpectrum;
}
std::unique_ptr<PRISMReweightableSpectrum> &
PredictionPRISM::GetNDData(PRISM::BeamChan NDChannel, int kA,
                           bool shift) const {
  if (NDChannel.chan &
      ((NDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuIntrinsic
                                             : NuChan::kNumuBarIntrinsic)) {
    return GetNDData_right_sign_numu(NDChannel.mode, kA, shift);
  } else if (NDChannel.chan & ((NDChannel.mode == BeamMode::kNuMode)
                                   ? NuChan::kNueIntrinsic
                                   : NuChan::kNueBarIntrinsic)) {
    return GetNDData_right_sign_nue(NDChannel.mode, kA, shift);
  } else if (NDChannel.chan &
             ((NDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuBarIntrinsic
                                                    : NuChan::kNumuIntrinsic)) {
    return GetNDData_wrong_sign_numu(NDChannel.mode, kA, shift);
  }
  PRISMOUT("Invalid ND Data request: " << NDChannel.mode << ", "
                                       << NDChannel.chan);
  return kNoSuchNDDataSpectrum;
}

bool PredictionPRISM::HaveNDData(PRISM::BeamChan NDChannel, int kA,
                                 bool shift) const {
  return bool(GetNDData(NDChannel, kA, shift));
}

std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetNDPrediction_right_sign_numu(BeamMode NDBM, int kA) const {
  if (NDBM == BeamMode::kNuMode) {
    return (kA == 293) ? Predictions.ND_293kA.numu_ccinc_sel_numode
                       : Predictions.ND_280kA.numu_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return (kA == 293) ? Predictions.ND_293kA.numubar_ccinc_sel_nubmode
                       : Predictions.ND_280kA.numubar_ccinc_sel_nubmode;
  }
  return kNoSuchNDPredictionSpectrum;
}
std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetNDPrediction_right_sign_nue(BeamMode NDBM, int kA) const {
  if (NDBM == BeamMode::kNuMode) {
    return (kA == 293) ? Predictions.ND_293kA.nue_ccinc_sel_numode
                       : Predictions.ND_280kA.nue_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return (kA == 293) ? Predictions.ND_293kA.nuebar_ccinc_sel_nubmode
                       : Predictions.ND_280kA.nuebar_ccinc_sel_nubmode;
  }
  return kNoSuchNDPredictionSpectrum;
}
std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetNDPrediction_wrong_sign_numu(BeamMode NDBM, int kA) const {
  if (NDBM == BeamMode::kNuMode) {
    return (kA == 293) ? Predictions.ND_293kA.numubar_ccinc_sel_numode
                       : Predictions.ND_280kA.numubar_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return (kA == 293) ? Predictions.ND_293kA.numu_ccinc_sel_nubmode
                       : Predictions.ND_280kA.numu_ccinc_sel_nubmode;
  }
  return kNoSuchNDPredictionSpectrum;
}
std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetNDPrediction(PRISM::BeamChan NDChannel, int kA) const {
  if (NDChannel.chan &
      ((NDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuIntrinsic
                                             : NuChan::kNumuBarIntrinsic)) {
    return GetNDPrediction_right_sign_numu(NDChannel.mode, kA);
  } else if (NDChannel.chan & ((NDChannel.mode == BeamMode::kNuMode)
                                   ? NuChan::kNueIntrinsic // eran
                                   : NuChan::kNueBarIntrinsic)) {
    return GetNDPrediction_right_sign_nue(NDChannel.mode, kA);
  } else if (NDChannel.chan &
             ((NDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuBarIntrinsic
                                                    : NuChan::kNumuIntrinsic)) {
    return GetNDPrediction_wrong_sign_numu(NDChannel.mode, kA);
  }
  PRISMOUT("Invalid ND Prediction request: " << NDChannel.mode << ", "
                                             << NDChannel.chan);
  return kNoSuchNDPredictionSpectrum;
}

bool PredictionPRISM::HaveNDPrediction(PRISM::BeamChan NDChannel,
                                       int kA) const {
  return bool(GetNDPrediction(NDChannel, kA));
}

std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetFDPrediction_right_sign_numu(BeamMode FDBM) const {
  if (FDBM == BeamMode::kNuMode) {
    return Predictions.FD.numu_ccinc_sel_numode;
  } else if (FDBM == BeamMode::kNuBarMode) {
    return Predictions.FD.numubar_ccinc_sel_nubmode;
  }
  return kNoSuchFDPredictionSpectrum;
}
std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetFDPrediction_right_sign_nue(BeamMode FDBM) const {
  if (FDBM == BeamMode::kNuMode) {
    return Predictions.FD.nue_ccinc_sel_numode;
  } else if (FDBM == BeamMode::kNuBarMode) {
    return Predictions.FD.nuebar_ccinc_sel_nubmode;
  }
  return kNoSuchFDPredictionSpectrum;
}
std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetFDPrediction(PRISM::BeamChan FDChannel) const {
  if (FDChannel.chan &
      ((FDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuIntrinsic
                                             : NuChan::kNumuBarIntrinsic)) {
    return GetFDPrediction_right_sign_numu(FDChannel.mode);
  } else if (FDChannel.chan &
             ((FDChannel.mode == BeamMode::kNuMode) ? NuChan::kNueApp
                                                    : NuChan::kNueBarApp)) {
    return GetFDPrediction_right_sign_nue(FDChannel.mode);
  }
  PRISMOUT("Invalid FD Prediction request: " << FDChannel.mode << ", "
                                             << FDChannel.chan);
  return kNoSuchFDPredictionSpectrum;
}

bool PredictionPRISM::HaveFDPrediction(PRISM::BeamChan FDChannel) const {
  return bool(GetFDPrediction(FDChannel));
}

// bool PredictionPRISM::HaveNDTargetPrediction(PRISM::BeamChan NDChannel) const
// {
//   return bool(GetNDPrediction(NDChannel));
// }   //eran

std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetFDUnOscWeightedSigPrediction_right_sign_numu(
    BeamMode FDBM) const {
  if (FDBM == BeamMode::kNuMode) {
    return Predictions.FD.numu_ccinc_sel_sig_numode;
  } else if (FDBM == BeamMode::kNuBarMode) {
    return Predictions.FD.numubar_ccinc_sel_sig_nubmode;
  }
  return kNoSuchFDPredictionSpectrum;
}
std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetFDUnOscWeightedSigPrediction_right_sign_nue(
    BeamMode FDBM) const {
  if (FDBM == BeamMode::kNuMode) {
    return Predictions.FD.nue_ccinc_sel_sig_numode;
  } else if (FDBM == BeamMode::kNuBarMode) {
    return Predictions.FD.nuebar_ccinc_sel_sig_nubmode;
  }
  return kNoSuchFDPredictionSpectrum;
}
std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetFDUnOscWeightedSigPrediction(
    PRISM::BeamChan FDChannel) const {
  if (FDChannel.chan &
      ((FDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuIntrinsic
                                             : NuChan::kNumuBarIntrinsic)) {
    return GetFDUnOscWeightedSigPrediction_right_sign_numu(FDChannel.mode);
  } else if (FDChannel.chan &
             ((FDChannel.mode == BeamMode::kNuMode) ? NuChan::kNueApp
                                                    : NuChan::kNueBarApp)) {
    return GetFDUnOscWeightedSigPrediction_right_sign_nue(FDChannel.mode);
  }
  PRISMOUT("Invalid FD UnOscPrediction request: " << FDChannel.mode << ", "
                                                  << FDChannel.chan);

  return kNoSuchFDPredictionSpectrum;
}

bool PredictionPRISM::HaveFDUnOscWeightedSigPrediction(
    PRISM::BeamChan FDChannel) const {
  return bool(GetFDUnOscWeightedSigPrediction(FDChannel));
}

std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetFDNonSwapAppOscPrediction(PRISM::BeamMode FDBM) const {
  return (FDBM == BeamMode::kNuMode)
             ? Predictions.FD.numu_ccinc_sel_sig_apposc_numode
             : Predictions.FD.numubar_ccinc_sel_sig_apposc_nubmode;
}

std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetFDNueSwapAppOscPrediction(PRISM::BeamMode FDBM) const {
  return (FDBM == BeamMode::kNuMode)
             ? Predictions.FD.nue_ccinc_sel_sig_apposc_numode
             : Predictions.FD.nuebar_ccinc_sel_sig_apposc_nubmode;
}

std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetFDNonSwapAppOscPredictionTrueEnu(
    PRISM::BeamMode FDBM) const {
  return (FDBM == BeamMode::kNuMode)
             ? Predictions.FD.numu_ccinc_sel_sig_apposc_numode_true_e_nu
             : Predictions.FD.numubar_ccinc_sel_sig_apposc_nubmode_true_e_nu;
}

std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetFDNueSwapAppOscPredictionTrueEnu(
    PRISM::BeamMode FDBM) const {
  return (FDBM == BeamMode::kNuMode)
             ? Predictions.FD.nue_ccinc_sel_sig_apposc_numode_true_e_nu
             : Predictions.FD.nuebar_ccinc_sel_sig_apposc_nubmode_true_e_nu;
}

//----------------------------------------------------------------------
Spectrum PredictionPRISM::Predict(osc::IOscCalc *calc) const {
  return PredictSyst(calc, kNoShift);
}

//----------------------------------------------------------------------
Spectrum PredictionPRISM::PredictSyst(osc::IOscCalc *calc,
                                      const SystShifts &shift) const {
  std::map<PredictionPRISM::PRISMComponent, Spectrum> Comps =
      PredictPRISMComponents_forNDtarget(calc,
                                         shift); // eran changing this to other

  assert(Comps.size());

  return Comps.at(kNDDataCorr_FDExtrap);
}

std::map<PredictionPRISM::PRISMComponent, Spectrum>
PredictionPRISM::PredictPRISMComponents(osc::IOscCalc *calc, SystShifts shift,
                                        MatchChan match_chan) const {

  return std::map<PredictionPRISM::PRISMComponent, Spectrum>{};
}

std::map<PredictionPRISM::PRISMComponent, Spectrum>
PredictionPRISM::PredictGaussianFlux_forNDtarget(double mean, double width,
                                                 ana::SystShifts shift,
                                                 BeamChan NDChannel) const {
  // fill this in
  return std::map<PredictionPRISM::PRISMComponent, Spectrum>{};
}

std::map<PredictionPRISM::PRISMComponent, Spectrum>
PredictionPRISM::PredictPRISMComponents_forNDtarget(osc::IOscCalc *calc,
                                   ana::SystShifts shift,
                                   PRISM::MatchChan match_chan) const {
  // fill this in

  bool WeHaveNDTargetPrediction = HaveNDPrediction(match_chan.to); // eran

  
  return std::map<PredictionPRISM::PRISMComponent, Spectrum>{};
}

//----------------------------------------------------------------------

ReweightableSpectrum
PredictionPRISM::GetDiagonalCovariance(Spectrum const &spec, double POT,
                                       HistAxis const &RecoAxis) const {
  LabelsAndBins reco_axis(RecoAxis.GetLabels().at(0),
                          RecoAxis.GetBinnings().at(0));
  std::unique_ptr<TH1> spec_h = std::unique_ptr<TH1>(spec.ToTH1(POT));
  // Create empty 2D matrix - this will be the new covariance matrix.
  Eigen::MatrixXd Cov_mat = Eigen::MatrixXd::Zero(
      spec_h->GetXaxis()->GetNbins() + 2, spec_h->GetXaxis()->GetNbins() + 2);
  // Set CovMat diagonal to be variances of prediction - MC error
  for (int ebin = 1; ebin <= spec_h->GetXaxis()->GetNbins(); ebin++) {
    Cov_mat(ebin, ebin) =
        std::pow(spec_h->GetBinError(ebin) * (POT / spec.POT()), 2);
  }

  ReweightableSpectrum rw(std::move(Cov_mat), reco_axis, reco_axis, POT,
                          0); // true_axis

  return rw;
}

//----------------------------------------------------------------------
Spectrum PredictionPRISM::PredictComponent(osc::IOscCalc *calc,
                                           Flavors::Flavors_t flav,
                                           Current::Current_t curr,
                                           Sign::Sign_t sign) const {

  throw;
}

// I know... but makes the SaveTo/LoadFrom a lot neater
using NamedReweightableSpectrumRefVect = std::vector<std::pair<
    std::string,
    std::reference_wrapper<std::unique_ptr<PRISMReweightableSpectrum>>>>;
using NamedPredInterpRefVect = std::vector<std::pair<
    std::string, std::reference_wrapper<std::unique_ptr<PredictionInterp>>>>;
using NamedReweightableSpectrumCRefVect = std::vector<std::pair<
    std::string,
    std::reference_wrapper<std::unique_ptr<PRISMReweightableSpectrum> const>>>;
using NamedPredInterpCRefVect = std::vector<
    std::pair<std::string,
              std::reference_wrapper<std::unique_ptr<PredictionInterp> const>>>;

//----------------------------------------------------------------------
void PredictionPRISM::SaveTo(TDirectory *dir, const std::string &name) const {
  TDirectory *tmp = gDirectory;

  dir = dir->mkdir(name.c_str()); // switch to subdir
  dir->cd();

  TObjString("PredictionPRISM").Write("type");

  for (auto &meas : NamedReweightableSpectrumCRefVect{
           {"M_ND_numu_ccinc_sel_numode",
            Measurements.ND_293kA.numu_ccinc_sel_numode},
           {"M_ND_numubar_ccinc_sel_numode",
            Measurements.ND_293kA.numubar_ccinc_sel_numode},
           {"M_ND_numu_ccinc_sel_numode_fd",
            Measurements.ND_293kA.numu_ccinc_sel_numode_fd},
           {"M_ND_numubar_ccinc_sel_numode_fd",
            Measurements.ND_293kA.numubar_ccinc_sel_numode_fd},
           {"M_ND_numu_ccinc_sel_nubmode",
            Measurements.ND_293kA.numu_ccinc_sel_nubmode},
           {"M_ND_numubar_ccinc_sel_nubmode",
            Measurements.ND_293kA.numubar_ccinc_sel_nubmode},
           {"M_ND_numu_ccinc_sel_nubmode_fd",
            Measurements.ND_293kA.numu_ccinc_sel_nubmode_fd},
           {"M_ND_numubar_ccinc_sel_nubmode_fd",
            Measurements.ND_293kA.numubar_ccinc_sel_nubmode_fd},
           {"M_ND_nue_ccinc_sel_numode",
            Measurements.ND_293kA.nue_ccinc_sel_numode},
           {"M_ND_nuebar_ccinc_sel_nubmode",
            Measurements.ND_293kA.nuebar_ccinc_sel_nubmode},
           {"M_ND_nue_ccinc_sel_numode_fd",
            Measurements.ND_293kA.nue_ccinc_sel_numode_fd},
           {"M_ND_nuebar_ccinc_sel_nubmode_fd",
            Measurements.ND_293kA.nuebar_ccinc_sel_nubmode_fd},
           {"M_ND_280kA_numu_ccinc_sel_numode",
            Measurements.ND_280kA.numu_ccinc_sel_numode},
           {"M_ND_280kA_numubar_ccinc_sel_numode",
            Measurements.ND_280kA.numubar_ccinc_sel_numode},
           {"M_ND_280kA_numu_ccinc_sel_numode_fd",
            Measurements.ND_280kA.numu_ccinc_sel_numode_fd},
           {"M_ND_280kA_numubar_ccinc_sel_numode_fd",
            Measurements.ND_280kA.numubar_ccinc_sel_numode_fd},
           {"M_ND_280kA_numu_ccinc_sel_nubmode",
            Measurements.ND_280kA.numu_ccinc_sel_nubmode},
           {"M_ND_280kA_numubar_ccinc_sel_nubmode",
            Measurements.ND_280kA.numubar_ccinc_sel_nubmode},
           {"M_ND_280kA_numu_ccinc_sel_nubmode_fd",
            Measurements.ND_280kA.numu_ccinc_sel_nubmode_fd},
           {"M_ND_280kA_numubar_ccinc_sel_nubmode_fd",
            Measurements.ND_280kA.numubar_ccinc_sel_nubmode_fd},
           {"M_ND_280kA_nue_ccinc_sel_numode",
            Measurements.ND_280kA.nue_ccinc_sel_numode},
           {"M_ND_280kA_nuebar_ccinc_sel_nubmode",
            Measurements.ND_280kA.nuebar_ccinc_sel_nubmode},
           {"M_ND_280kA_nue_ccinc_sel_numode_fd",
            Measurements.ND_280kA.nue_ccinc_sel_numode_fd},
           {"M_ND_280kA_nuebar_ccinc_sel_nubmode_fd",
            Measurements.ND_280kA.nuebar_ccinc_sel_nubmode_fd}}) {
    if (meas.second.get()) {
      meas.second.get()->SaveTo(dir, meas.first.c_str());
    }
  }

  for (auto &meas : NamedPredInterpCRefVect{
           {"P_ND_numu_ccinc_sel_numode",
            Predictions.ND_293kA.numu_ccinc_sel_numode},
           {"P_ND_numubar_ccinc_sel_numode",
            Predictions.ND_293kA.numubar_ccinc_sel_numode},
           {"P_ND_numu_ccinc_sel_nubmode",
            Predictions.ND_293kA.numu_ccinc_sel_nubmode},
           {"P_ND_numubar_ccinc_sel_nubmode",
            Predictions.ND_293kA.numubar_ccinc_sel_nubmode},
           {"P_ND_nue_ccinc_sel_numode",
            Predictions.ND_293kA.nue_ccinc_sel_numode},
           {"P_ND_nuebar_ccinc_sel_nubmode",
            Predictions.ND_293kA.nuebar_ccinc_sel_nubmode},
           {"P_ND_280kA_numu_ccinc_sel_numode",
            Predictions.ND_280kA.numu_ccinc_sel_numode},
           {"P_ND_280kA_numubar_ccinc_sel_numode",
            Predictions.ND_280kA.numubar_ccinc_sel_numode},
           {"P_ND_280kA_numu_ccinc_sel_nubmode",
            Predictions.ND_280kA.numu_ccinc_sel_nubmode},
           {"P_ND_280kA_numubar_ccinc_sel_nubmode",
            Predictions.ND_280kA.numubar_ccinc_sel_nubmode},
           {"P_ND_280kA_nue_ccinc_sel_numode",
            Predictions.ND_280kA.nue_ccinc_sel_numode},
           {"P_ND_280kA_nuebar_ccinc_sel_nubmode",
            Predictions.ND_280kA.nuebar_ccinc_sel_nubmode}}) {
    if (meas.second.get()) {
      meas.second.get()->SaveTo(dir, meas.first.c_str());
    }
  }

  for (auto &meas : NamedPredInterpCRefVect{
           {"P_FD_numu_ccinc_sel_numode", Predictions.FD.numu_ccinc_sel_numode},
           {"P_FD_nue_ccinc_sel_numode", Predictions.FD.nue_ccinc_sel_numode},
           {"P_FD_numubar_ccinc_sel_nubmode",
            Predictions.FD.numubar_ccinc_sel_nubmode},
           {"P_FD_nuebar_ccinc_sel_nubmode",
            Predictions.FD.nuebar_ccinc_sel_nubmode}}) {
    if (meas.second.get()) {
      meas.second.get()->SaveTo(dir, meas.first.c_str());
    }
  }

  for (auto &meas : NamedPredInterpCRefVect{
           {"P_FD_numu_ccinc_sel_sig_numode",
            Predictions.FD.numu_ccinc_sel_sig_numode},
           {"P_FD_nue_ccinc_sel_sig_numode",
            Predictions.FD.nue_ccinc_sel_sig_numode},
           {"P_FD_numubar_ccinc_sel_sig_nubmode",
            Predictions.FD.numubar_ccinc_sel_sig_nubmode},
           {"P_FD_nuebar_ccinc_sel_sig_nubmode",
            Predictions.FD.nuebar_ccinc_sel_sig_nubmode},
           {"P_FD_numu_ccinc_sel_sig_apposc_numode",
            Predictions.FD.numu_ccinc_sel_sig_apposc_numode},
           {"P_FD_numubar_ccinc_sel_sig_apposc_nubmode",
            Predictions.FD.numubar_ccinc_sel_sig_apposc_nubmode},
           {"P_FD_nue_ccinc_sel_sig_apposc_numode",
            Predictions.FD.nue_ccinc_sel_sig_apposc_numode},
           {"P_FD_nuebar_ccinc_sel_sig_apposc_nubmode",
            Predictions.FD.nuebar_ccinc_sel_sig_apposc_nubmode},
           {"P_FD_numu_ccinc_sel_sig_apposc_numode_true_e_nu",
            Predictions.FD.numu_ccinc_sel_sig_apposc_numode_true_e_nu},
           {"P_FD_numubar_ccinc_sel_sig_apposc_nubmode_true_e_nu",
            Predictions.FD.numubar_ccinc_sel_sig_apposc_nubmode_true_e_nu},
           {"P_FD_nue_ccinc_sel_sig_apposc_numode_true_e_nu",
            Predictions.FD.nue_ccinc_sel_sig_apposc_numode_true_e_nu},
           {"P_FD_nuebar_ccinc_sel_sig_apposc_nubmode_true_e_nu",
            Predictions.FD.nuebar_ccinc_sel_sig_apposc_nubmode_true_e_nu}}) {
    if (meas.second.get()) {
      meas.second.get()->SaveTo(dir, meas.first.c_str());
    }
  }

  for (unsigned int i = 0; i < fAnalysisAxisND.GetBinnings().size(); ++i) {
    TObjString(fAnalysisAxisND.GetLabels()[i].c_str())
        .Write(TString::Format("predND_label%d", i).Data());
    std::string str("predND_bins");
    str += std::to_string(i);
    fAnalysisAxisND.GetBinnings()[i].SaveTo(dir, str);
  }

  for (unsigned int i = 0; i < fAnalysisAxisFD.GetBinnings().size(); ++i) {
    TObjString(fAnalysisAxisFD.GetLabels()[i].c_str())
        .Write(TString::Format("predFD_label%d", i).Data());
    std::string str("predFD_bins");
    str += std::to_string(i);
    fAnalysisAxisFD.GetBinnings()[i].SaveTo(dir, str);
  }

  for (unsigned int i = 0; i < fNDOffAxis.GetBinnings().size(); ++i) {
    TObjString(fNDOffAxis.GetLabels()[i].c_str())
        .Write(TString::Format("offaxis_label%d", i).Data());
    std::string str("offaxis_bins");
    str += std::to_string(i);
    fNDOffAxis.GetBinnings()[i].SaveTo(dir, str);
  }

  for (unsigned int i = 0; i < fND280kAAxis.GetBinnings().size(); ++i) {
    TObjString(fND280kAAxis.GetLabels()[i].c_str())
        .Write(TString::Format("280kAaxis_label%d", i).Data());
    std::string str("280kAaxis_bins");
    str += std::to_string(i);
    fND280kAAxis.GetBinnings()[i].SaveTo(dir, str);
  }

  for (unsigned int i = 0; i < fNDFDEnergyMatchAxis.GetBinnings().size(); ++i) {
    TObjString(fNDFDEnergyMatchAxis.GetLabels()[i].c_str())
        .Write(TString::Format("ematch_label%d", i).Data());
    std::string str("ematch_bins");
    str += std::to_string(i);
    fNDFDEnergyMatchAxis.GetBinnings()[i].SaveTo(dir, str);
  }

  for (unsigned int i = 0; i < fTrueAnalysisAxis.GetBinnings().size(); ++i) {
    TObjString(fTrueAnalysisAxis.GetLabels()[i].c_str())
        .Write(TString::Format("predTrue_label%d", i).Data());
    std::string str("predTrue_bins");
    str += std::to_string(i);
    fTrueAnalysisAxis.GetBinnings()[i].SaveTo(dir, str);
  }

  tmp->cd();
}

//----------------------------------------------------------------------
std::unique_ptr<PredictionPRISM>
PredictionPRISM::LoadFrom(TDirectory *dir, const std::string &name) {

  std::cout << "Loading PredictionPRISM" << std::endl;
  dir = dir->GetDirectory(name.c_str());
  assert(dir);

  std::vector<std::string> predND_labels;
  std::vector<Binning> predND_bins;
  std::vector<Var> predND_dummy_vars;

  for (int i = 0;; ++i) {
    TDirectory *subdir = dir->GetDirectory(TString::Format("predND_bins%d", i));
    if (!subdir) {
      break;
    }
    std::string str("predND_bins");
    str += std::to_string(i);
    predND_bins.push_back(*Binning::LoadFrom(dir, str));
    TObjString *label =
        (TObjString *)dir->Get(TString::Format("predND_label%d", i));
    predND_labels.push_back(label ? label->GetString().Data() : "");
    predND_dummy_vars.push_back(Constant(1));
  }

  std::vector<std::string> predFD_labels;
  std::vector<Binning> predFD_bins;
  std::vector<Var> predFD_dummy_vars;

  for (int i = 0;; ++i) {
    TDirectory *subdir = dir->GetDirectory(TString::Format("predFD_bins%d", i));
    if (!subdir) {
      break;
    }
    std::string str("predFD_bins");
    str += std::to_string(i);
    predFD_bins.push_back(*Binning::LoadFrom(dir, str));
    TObjString *label =
        (TObjString *)dir->Get(TString::Format("predFD_label%d", i));
    predFD_labels.push_back(label ? label->GetString().Data() : "");
    predFD_dummy_vars.push_back(Constant(1));
  }

  std::vector<std::string> offaxis_labels;
  std::vector<Binning> offaxis_bins;
  std::vector<Var> offaxis_dummy_vars;

  for (int i = 0;; ++i) {
    TDirectory *subdir =
        dir->GetDirectory(TString::Format("offaxis_bins%d", i));
    if (!subdir) {
      break;
    }
    std::string str("offaxis_bins");
    str += std::to_string(i);
    offaxis_bins.push_back(*Binning::LoadFrom(dir, str));
    TObjString *label =
        (TObjString *)dir->Get(TString::Format("offaxis_label%d", i));
    offaxis_labels.push_back(label ? label->GetString().Data() : "");
    offaxis_dummy_vars.push_back(Constant(1));
  }

  std::vector<std::string> _280kAaxis_labels;
  std::vector<Binning> _280kAaxis_bins;
  std::vector<Var> _280kAaxis_dummy_vars;

  for (int i = 0;; ++i) {
    TDirectory *subdir =
        dir->GetDirectory(TString::Format("280kAaxis_bins%d", i));
    if (!subdir) {
      break;
    }
    std::string str("280kAaxis_bins");
    str += std::to_string(i);
    _280kAaxis_bins.push_back(*Binning::LoadFrom(dir, str));
    TObjString *label =
        (TObjString *)dir->Get(TString::Format("280kAaxis_label%d", i));
    _280kAaxis_labels.push_back(label ? label->GetString().Data() : "");
    _280kAaxis_dummy_vars.push_back(Constant(1));
  }

  std::vector<std::string> ematch_labels;
  std::vector<Binning> ematch_bins;
  std::vector<Var> ematch_dummy_vars;

  for (int i = 0;; ++i) {
    TDirectory *subdir = dir->GetDirectory(TString::Format("ematch_bins%d", i));
    if (!subdir) {
      break;
    }
    std::string str("280kAaxis_bins");
    str += std::to_string(i);
    ematch_bins.push_back(*Binning::LoadFrom(dir, str));
    TObjString *label =
        (TObjString *)dir->Get(TString::Format("ematch_label%d", i));
    ematch_labels.push_back(label ? label->GetString().Data() : "");
    ematch_dummy_vars.push_back(Constant(1));
  }

  std::vector<std::string> predTrue_labels;
  std::vector<Binning> predTrue_bins;
  std::vector<Var> predTrue_dummy_vars;

  for (int i = 0;; ++i) {
    TDirectory *subdir =
        dir->GetDirectory(TString::Format("predTrue_bins%d", i));
    if (!subdir) {
      break;
    }
    std::string str("predTrue_bins");
    str += std::to_string(i);
    predTrue_bins.push_back(*Binning::LoadFrom(dir, str));
    TObjString *label =
        (TObjString *)dir->Get(TString::Format("predTrue_label%d", i));
    predTrue_labels.push_back(label ? label->GetString().Data() : "");
    predTrue_dummy_vars.push_back(Constant(1));
  }

  HistAxis const predictionAxisND(predND_labels, predND_bins,
                                  predND_dummy_vars);
  HistAxis const predictionAxisFD(predFD_labels, predFD_bins,
                                  predFD_dummy_vars);
  HistAxis const offAxis(offaxis_labels, offaxis_bins, offaxis_dummy_vars);
  HistAxis const _280kAAxis(_280kAaxis_labels, _280kAaxis_bins,
                            _280kAaxis_dummy_vars);
  HistAxis const energyMatchAxis(ematch_labels, ematch_bins, ematch_dummy_vars);
  HistAxis const predictionAxisTrue(predTrue_labels, predTrue_bins,
                                    predTrue_dummy_vars);

  std::vector<HistAxis> AxisVec = {predictionAxisFD};
  HistAxis const covarianceAxis = GetMatrixAxis(AxisVec);

  HistAxis const offPredictionAxis = GetTwoDAxis(predictionAxisND, offAxis);
  HistAxis const off280kAPredictionAxis =
      GetTwoDAxis(predictionAxisND, _280kAAxis);
  HistAxis const fluxMatcherCorrAxis =
      GetTwoDAxis(predictionAxisFD, energyMatchAxis);

  std::unique_ptr<PredictionPRISM> pred = std::make_unique<PredictionPRISM>(
      predictionAxisND, predictionAxisFD, offAxis, _280kAAxis,
      predictionAxisTrue, energyMatchAxis, covarianceAxis, offPredictionAxis,
      off280kAPredictionAxis, fluxMatcherCorrAxis);
  // Important new function
  pred->Initialize();

  for (auto &meas : NamedReweightableSpectrumRefVect{
           {"M_ND_numu_ccinc_sel_numode",
            pred->Measurements.ND_293kA.numu_ccinc_sel_numode},
           {"M_ND_numubar_ccinc_sel_numode",
            pred->Measurements.ND_293kA.numubar_ccinc_sel_numode},
           {"M_ND_numu_ccinc_sel_numode_fd",
            pred->Measurements.ND_293kA.numu_ccinc_sel_numode_fd},
           {"M_ND_numubar_ccinc_sel_numode_fd",
            pred->Measurements.ND_293kA.numubar_ccinc_sel_numode_fd},
           {"M_ND_numu_ccinc_sel_nubmode",
            pred->Measurements.ND_293kA.numu_ccinc_sel_nubmode},
           {"M_ND_numubar_ccinc_sel_nubmode",
            pred->Measurements.ND_293kA.numubar_ccinc_sel_nubmode},
           {"M_ND_numu_ccinc_sel_nubmode_fd",
            pred->Measurements.ND_293kA.numu_ccinc_sel_nubmode_fd},
           {"M_ND_numubar_ccinc_sel_nubmode_fd",
            pred->Measurements.ND_293kA.numubar_ccinc_sel_nubmode_fd},
           {"M_ND_nue_ccinc_sel_numode",
            pred->Measurements.ND_293kA.nue_ccinc_sel_numode},
           {"M_ND_nuebar_ccinc_sel_nubmode",
            pred->Measurements.ND_293kA.nuebar_ccinc_sel_nubmode},
           {"M_ND_nue_ccinc_sel_numode_fd",
            pred->Measurements.ND_293kA.nue_ccinc_sel_numode_fd},
           {"M_ND_nuebar_ccinc_sel_nubmode_fd",
            pred->Measurements.ND_293kA.nuebar_ccinc_sel_nubmode_fd},
           {"M_ND_280kA_numu_ccinc_sel_numode",
            pred->Measurements.ND_280kA.numu_ccinc_sel_numode},
           {"M_ND_280kA_numubar_ccinc_sel_numode",
            pred->Measurements.ND_280kA.numubar_ccinc_sel_numode},
           {"M_ND_280kA_numu_ccinc_sel_numode_fd",
            pred->Measurements.ND_280kA.numu_ccinc_sel_numode_fd},
           {"M_ND_280kA_numubar_ccinc_sel_numode_fd",
            pred->Measurements.ND_280kA.numubar_ccinc_sel_numode_fd},
           {"M_ND_280kA_numu_ccinc_sel_nubmode",
            pred->Measurements.ND_280kA.numu_ccinc_sel_nubmode},
           {"M_ND_280kA_numubar_ccinc_sel_nubmode",
            pred->Measurements.ND_280kA.numubar_ccinc_sel_nubmode},
           {"M_ND_280kA_numu_ccinc_sel_nubmode_fd",
            pred->Measurements.ND_280kA.numu_ccinc_sel_nubmode_fd},
           {"M_ND_280kA_numubar_ccinc_sel_nubmode_fd",
            pred->Measurements.ND_280kA.numubar_ccinc_sel_nubmode_fd},
           {"M_ND_280kA_nue_ccinc_sel_numode",
            pred->Measurements.ND_280kA.nue_ccinc_sel_numode},
           {"M_ND_280kA_nuebar_ccinc_sel_nubmode",
            pred->Measurements.ND_280kA.nuebar_ccinc_sel_nubmode},
           {"M_ND_280kA_nue_ccinc_sel_numode_fd",
            pred->Measurements.ND_280kA.nue_ccinc_sel_numode_fd},
           {"M_ND_280kA_nuebar_ccinc_sel_nubmode_fd",
            pred->Measurements.ND_280kA.nuebar_ccinc_sel_nubmode_fd}}) {
    if (dir->GetDirectory(meas.first.c_str())) {
      meas.second.get() =
          PRISMReweightableSpectrum::LoadFrom(dir, meas.first.c_str());
      double MCPOT = meas.second.get()->POT();
      pred->SetDefaultOffAxisPOT(
          MCPOT); // Save the default POT from ND MC file.
      meas.second.get()->OverridePOT(1);
    }
  }

  for (auto &meas : NamedPredInterpRefVect{
           {"P_ND_numu_ccinc_sel_numode",
            pred->Predictions.ND_293kA.numu_ccinc_sel_numode},
           {"P_ND_numubar_ccinc_sel_numode",
            pred->Predictions.ND_293kA.numubar_ccinc_sel_numode},
           {"P_ND_numu_ccinc_sel_nubmode",
            pred->Predictions.ND_293kA.numu_ccinc_sel_nubmode},
           {"P_ND_numubar_ccinc_sel_nubmode",
            pred->Predictions.ND_293kA.numubar_ccinc_sel_nubmode},
           {"P_ND_nue_ccinc_sel_numode",
            pred->Predictions.ND_293kA.nue_ccinc_sel_numode},
           {"P_ND_nuebar_ccinc_sel_nubmode",
            pred->Predictions.ND_293kA.nuebar_ccinc_sel_nubmode},
           {"P_ND_280kA_numu_ccinc_sel_numode",
            pred->Predictions.ND_280kA.numu_ccinc_sel_numode},
           {"P_ND_280kA_numubar_ccinc_sel_numode",
            pred->Predictions.ND_280kA.numubar_ccinc_sel_numode},
           {"P_ND_280kA_numu_ccinc_sel_nubmode",
            pred->Predictions.ND_280kA.numu_ccinc_sel_nubmode},
           {"P_ND_280kA_numubar_ccinc_sel_nubmode",
            pred->Predictions.ND_280kA.numubar_ccinc_sel_nubmode},
           {"P_ND_280kA_nue_ccinc_sel_numode",
            pred->Predictions.ND_280kA.nue_ccinc_sel_numode},
           {"P_ND_280kA_nuebar_ccinc_sel_nubmode",
            pred->Predictions.ND_280kA.nuebar_ccinc_sel_nubmode}}) {
    if (dir->GetDirectory(meas.first.c_str())) {
      meas.second.get() = PredictionInterp::LoadFrom(dir, meas.first.c_str());
      meas.second.get()->GetPredNomAs<PredictionNoOsc>()->OverridePOT(1);
      // meas.second.get()->MinimizeMemory();
    }
  }

  for (auto &meas : NamedPredInterpRefVect{
           {"P_FD_numu_ccinc_sel_numode",
            pred->Predictions.FD.numu_ccinc_sel_numode},
           {"P_FD_nue_ccinc_sel_numode",
            pred->Predictions.FD.nue_ccinc_sel_numode},
           {"P_FD_numubar_ccinc_sel_nubmode",
            pred->Predictions.FD.numubar_ccinc_sel_nubmode},
           {"P_FD_nuebar_ccinc_sel_nubmode",
            pred->Predictions.FD.nuebar_ccinc_sel_nubmode}}) {
    if (dir->GetDirectory(meas.first.c_str())) {
      meas.second.get() = PredictionInterp::LoadFrom(dir, meas.first.c_str());
    }
  }

  for (auto &meas : NamedPredInterpRefVect{
           {"P_FD_numu_ccinc_sel_sig_numode",
            pred->Predictions.FD.numu_ccinc_sel_sig_numode},
           {"P_FD_nue_ccinc_sel_sig_numode",
            pred->Predictions.FD.nue_ccinc_sel_sig_numode},
           {"P_FD_numubar_ccinc_sel_sig_nubmode",
            pred->Predictions.FD.numubar_ccinc_sel_sig_nubmode},
           {"P_FD_nuebar_ccinc_sel_sig_nubmode",
            pred->Predictions.FD.nuebar_ccinc_sel_sig_nubmode},
           {"P_FD_numu_ccinc_sel_sig_apposc_numode",
            pred->Predictions.FD.numu_ccinc_sel_sig_apposc_numode},
           {"P_FD_numubar_ccinc_sel_sig_apposc_nubmode",
            pred->Predictions.FD.numubar_ccinc_sel_sig_apposc_nubmode},
           {"P_FD_nue_ccinc_sel_sig_apposc_numode",
            pred->Predictions.FD.nue_ccinc_sel_sig_apposc_numode},
           {"P_FD_nuebar_ccinc_sel_sig_apposc_nubmode",
            pred->Predictions.FD.nuebar_ccinc_sel_sig_apposc_nubmode},
           {"P_FD_numu_ccinc_sel_sig_apposc_numode_true_e_nu",
            pred->Predictions.FD.numu_ccinc_sel_sig_apposc_numode_true_e_nu},
           {"P_FD_numubar_ccinc_sel_sig_apposc_nubmode_true_e_nu",
            pred->Predictions.FD
                .numubar_ccinc_sel_sig_apposc_nubmode_true_e_nu},
           {"P_FD_nue_ccinc_sel_sig_apposc_numode_true_e_nu",
            pred->Predictions.FD.nue_ccinc_sel_sig_apposc_numode_true_e_nu},
           {"P_FD_nuebar_ccinc_sel_sig_apposc_nubmode_true_e_nu",
            pred->Predictions.FD
                .nuebar_ccinc_sel_sig_apposc_nubmode_true_e_nu}}) {
    if (dir->GetDirectory(meas.first.c_str())) {
      meas.second.get() = PredictionInterp::LoadFrom(dir, meas.first.c_str());
      // meas.second.get()->MinimizeMemory();
    }
  }

  delete dir;

  return pred;
}

} // namespace ana
