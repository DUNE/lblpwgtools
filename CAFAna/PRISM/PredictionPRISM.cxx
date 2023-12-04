#include "CAFAna/PRISM/PredictionPRISM.h"

#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/PRISM/Cuts.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PredictionsForPRISM.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/LoadFromRegistry.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/OscCurve.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Stan.h"

#include "OscLib/IOscCalc.h"
#include "CAFAna/Core/StanUtils.h"

#include "CAFAna/Analysis/AnalysisVars.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/Systs/EnergySysts.h"
#include "CAFAna/Systs/RecoEnergyNDSysts.h"
#include "CAFAna/Systs/DUNEFluxSysts.h"

#include "TAxis.h"
#include "TDirectory.h"
#include "TH2.h"
#include "TObjString.h"

using namespace PRISM;

namespace ana {

REGISTER_LOADFROM("PredictionPRISM", IPrediction, PredictionPRISM);

static std::unique_ptr<PRISMReweightableSpectrum> kNoSuchNDDataSpectrum{nullptr};
static std::unique_ptr<PredictionInterp> kNoSuchNDPredictionSpectrum{nullptr};
static std::unique_ptr<PredictionInterp> kNoSuchFDPredictionSpectrum{nullptr};


PredictionPRISM::PredictionPRISM(
    const HistAxis &AnalysisAxisND, const HistAxis &AnalysisAxisFD_numu,
    const HistAxis &AnalysisAxisFD_nue,
    const HistAxis &NDOffAxis, const HistAxis &ND280kAAxis,
    const HistAxis &TrueAnalysisAxis, const HistAxis &NDFDEnergyMatchAxis,
    const HistAxis &CovarianceAxis_numu, const HistAxis &CovarianceAxis_nue,
    const HistAxis &OffPredictionAxis,
    const HistAxis &_280kAPredictionAxis,
    const HistAxis &FluxMatcherCorrectionAxes_numu,
    const HistAxis &FluxMatcherCorrectionAxes_nue)
    : fAnalysisAxisND(AnalysisAxisND), fAnalysisAxisFD_numu(AnalysisAxisFD_numu),
      fAnalysisAxisFD_nue(AnalysisAxisFD_nue),
      fNDOffAxis(NDOffAxis), fND280kAAxis(ND280kAAxis),
      fTrueAnalysisAxis(TrueAnalysisAxis),
      fNDFDEnergyMatchAxis(NDFDEnergyMatchAxis),
      fCovarianceAxis_numu(CovarianceAxis_numu), fCovarianceAxis_nue(CovarianceAxis_nue),
      fOffPredictionAxis(OffPredictionAxis),
      f280kAPredictionAxis(_280kAPredictionAxis),
      fFluxMatcherCorrectionAxes_numu(FluxMatcherCorrectionAxes_numu),
      fFluxMatcherCorrectionAxes_nue(FluxMatcherCorrectionAxes_nue) {}

void PredictionPRISM::Initialize() {
  fSetNDErrorsFromRate = true;
  fVaryNDFDMCData = false;
  fDebugPlots = false;
  fUseFakeData = false;

  fDefaultOffAxisPOT = 1.1E20;

  PRISMOUT("PRISM analysis axis: "
           << fAnalysisAxisFD_numu.GetLabels().front() << " with "
           << fAnalysisAxisFD_numu.GetBinnings().front().Edges().size()
           << " bins from "
           << fAnalysisAxisFD_numu.GetBinnings().front().Edges().front() << " to "
           << fAnalysisAxisFD_numu.GetBinnings().front().Edges().back() << ".");

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
  fFluxMatcherWSB = nullptr;

  fNDFD_Matrix = nullptr;
  fNDFD_Matrix_WSB = nullptr; //need a separate smearing matrix for the anti channel
  fMCEffCorrection = nullptr;
  fMCEffCorrection_WSB = nullptr; //need different efficiency correction for the anti channel

  fNCCorrection = true;
  fWSBCorrection = true;
  fWLBCorrection = true;
  fNuTauCCCorrection = true;
  fIntrinsicCorrection = true;

  fAxisAgreement = true;
  for (size_t bin = 0; bin < fAnalysisAxisFD_numu.GetBinnings().size(); bin++) {
    for (size_t ed = 0;
         ed < fAnalysisAxisFD_numu.GetBinnings().at(bin).Edges().size(); ed++) {
      double FDedge = fAnalysisAxisFD_numu.GetBinnings().at(bin).Edges().at(ed);
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
  if (&NDData == &kNoSuchNDDataSpectrum || &NDFakeDataBias == &kNoSuchNDDataSpectrum) {
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
/// * NuMode (i.e. wrong sign mu-like FHC), NuBarMode (i.e. right sign mu-like RHC)
/// * NuMode (i.e. wrong sign mu-like FHC), NuMode (i.e. wrong sign mu-like FHC)
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

  std::unique_ptr<PredictionInterp> &FDPrediction = GetFDPrediction(FDChannel);
  if (&FDPrediction == &kNoSuchFDPredictionSpectrum) {
    std::cout << "ERROR: Invalid FD MC type passed: " << FDChannel.chan << ":"
              << FDChannel.mode << std::endl;
    abort();
  }

  HistAxis &FDAxis = (FDChannel.chan == PRISM::NuChan::kNueNueBar) ?
                     fAnalysisAxisFD_nue :
                     fAnalysisAxisFD_numu;

  HistAxis &FDFluxCorrAxis = (FDChannel.chan == PRISM::NuChan::kNueNueBar) ?
                             fFluxMatcherCorrectionAxes_nue :
                             fFluxMatcherCorrectionAxes_numu;

  fPredGens.push_back(
      std::make_unique<NoExtrapPredictionGenerator>(FDAxis, cut, wei));
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
      FDFluxCorrAxis, cut, wei));
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

  //fPredGens.push_back(std::make_unique<NoExtrapPredictionGenerator>(
  fPredGens.push_back(std::make_unique<FDNoOscPredictionGenerator>(
      fTrueAnalysisAxis, kFDNueCut, wei));
  FDNueSwapAppOscPrediction = std::make_unique<PredictionInterp>(
      systlist, calc, *fPredGens.back(), loaders, kNoShift,
      PredictionInterp::kSplitBySign);

  // Numu/nue xsec corr in true neutrino energy used in including FD intrinsic nue in flux matching
  // Copied above and used fNDFDEnergyMatchAxis instead

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

  // Prediction to get oscillate nutau appearance spectrum.
  std::unique_ptr<PredictionInterp> &FDNutauSwapAppOscPrediction =
      GetFDNutauSwapAppOscPrediction(FDChannel.mode);
  // Always want unselected nutau cut for this.
  auto kFDNutauCut = (FDChannel.mode == BeamMode::kNuMode)
                      ? kPRISMFDSignal_True_nutau
                      : kPRISMFDSignal_True_nutaub;

  fPredGens.push_back( // fAnalysisAxisFD
      std::make_unique<NoExtrapPredictionGenerator>(fTrueAnalysisAxis,
                                                    kFDNutauCut, wei));
  FDNutauSwapAppOscPrediction = std::make_unique<PredictionInterp>(
      systlist, calc, *fPredGens.back(), loaders, kNoShift,
      PredictionInterp::kSplitBySign);
}

//-----------------------------------------------

std::unique_ptr<PRISMReweightableSpectrum> &
PredictionPRISM::GetNDData_right_sign_numu(BeamMode NDBM, int kA, bool shift) const {
  if (NDBM == BeamMode::kNuMode) {
    return (kA == 293) ? (shift ? Measurements.ND_293kA.numu_ccinc_sel_numode_fd :
                          Measurements.ND_293kA.numu_ccinc_sel_numode)
                       : (shift ? Measurements.ND_280kA.numu_ccinc_sel_numode_fd :
                          Measurements.ND_280kA.numu_ccinc_sel_numode);
  } else if (NDBM == BeamMode::kNuBarMode) {
    return (kA == 293) ? (shift ? Measurements.ND_293kA.numubar_ccinc_sel_nubmode_fd :
                          Measurements.ND_293kA.numubar_ccinc_sel_nubmode)
                       : (shift ? Measurements.ND_280kA.numubar_ccinc_sel_nubmode_fd :
                          Measurements.ND_280kA.numubar_ccinc_sel_nubmode);
  }
  return kNoSuchNDDataSpectrum;
}
std::unique_ptr<PRISMReweightableSpectrum> &
PredictionPRISM::GetNDData_right_sign_nue(BeamMode NDBM, int kA, bool shift) const {
  if (NDBM == BeamMode::kNuMode) {
    return (kA == 293) ? (shift ? Measurements.ND_293kA.nue_ccinc_sel_numode_fd :
                          Measurements.ND_293kA.nue_ccinc_sel_numode)
                       : (shift ? Measurements.ND_280kA.nue_ccinc_sel_numode_fd :
                          Measurements.ND_280kA.nue_ccinc_sel_numode);
  } else if (NDBM == BeamMode::kNuBarMode) {
    return (kA == 293) ? (shift ? Measurements.ND_293kA.nuebar_ccinc_sel_nubmode_fd :
                          Measurements.ND_293kA.nuebar_ccinc_sel_nubmode)
                       : (shift ? Measurements.ND_280kA.nuebar_ccinc_sel_nubmode_fd :
                          Measurements.ND_280kA.nuebar_ccinc_sel_nubmode);
  }
  return kNoSuchNDDataSpectrum;
}
std::unique_ptr<PRISMReweightableSpectrum> &
PredictionPRISM::GetNDData_wrong_sign_numu(BeamMode NDBM, int kA, bool shift) const {
  if (NDBM == BeamMode::kNuMode) {
    return (kA == 293) ? (shift ? Measurements.ND_293kA.numubar_ccinc_sel_numode_fd :
                          Measurements.ND_293kA.numubar_ccinc_sel_numode)
                       : (shift ? Measurements.ND_280kA.numubar_ccinc_sel_numode_fd :
                          Measurements.ND_280kA.numubar_ccinc_sel_numode);
  } else if (NDBM == BeamMode::kNuBarMode) {
    return (kA == 293) ? (shift ? Measurements.ND_293kA.numu_ccinc_sel_nubmode_fd :
                          Measurements.ND_293kA.numu_ccinc_sel_nubmode)
                       : (shift ? Measurements.ND_280kA.numu_ccinc_sel_nubmode_fd :
                         Measurements.ND_280kA.numu_ccinc_sel_nubmode);
  }
  return kNoSuchNDDataSpectrum;
}
std::unique_ptr<PRISMReweightableSpectrum> &
PredictionPRISM::GetNDData(PRISM::BeamChan NDChannel, int kA, bool shift) const {
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

bool PredictionPRISM::HaveNDData(PRISM::BeamChan NDChannel, int kA, bool shift) const {
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
                                   ? NuChan::kNueIntrinsic
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
PredictionPRISM::GetFDPrediction_right_sign_nutau(BeamMode FDBM) const {
  if (FDBM == BeamMode::kNuMode) {
    return Predictions.FD.nutau_ccinc_sel_numode;
  } else if (FDBM == BeamMode::kNuBarMode) {
    return Predictions.FD.nutaubar_ccinc_sel_nubmode;
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
  } else if (FDChannel.chan &
             ((FDChannel.mode == BeamMode::kNuMode) ? NuChan::kNutauApp
                                                    : NuChan::kNutauBarApp)) {
    return GetFDPrediction_right_sign_nutau(FDChannel.mode);
  }
  PRISMOUT("Invalid FD Prediction request: " << FDChannel.mode << ", "
                                             << FDChannel.chan);
  return kNoSuchFDPredictionSpectrum;
}

bool PredictionPRISM::HaveFDPrediction(PRISM::BeamChan FDChannel) const {
  return bool(GetFDPrediction(FDChannel));
}

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
PredictionPRISM::GetFDUnOscWeightedSigPrediction_right_sign_nutau(
    BeamMode FDBM) const {
  if (FDBM == BeamMode::kNuMode) {
    return Predictions.FD.nutau_ccinc_sel_sig_numode;
  } else if (FDBM == BeamMode::kNuBarMode) {
    return Predictions.FD.nutaubar_ccinc_sel_sig_nubmode;
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
  } else if (FDChannel.chan &
             ((FDChannel.mode == BeamMode::kNuMode) ? NuChan::kNutauApp
                                                    : NuChan::kNutauBarApp)) {
    return GetFDUnOscWeightedSigPrediction_right_sign_nutau(FDChannel.mode);
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
PredictionPRISM::GetFDNonSwapAppOscPredictionTrueEnu(PRISM::BeamMode FDBM) const {
  return (FDBM == BeamMode::kNuMode)
             ? Predictions.FD.numu_ccinc_sel_sig_apposc_numode_true_e_nu
             : Predictions.FD.numubar_ccinc_sel_sig_apposc_nubmode_true_e_nu;
}

std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetFDNueSwapAppOscPredictionTrueEnu(PRISM::BeamMode FDBM) const {
  return (FDBM == BeamMode::kNuMode)
             ? Predictions.FD.nue_ccinc_sel_sig_apposc_numode_true_e_nu
             : Predictions.FD.nuebar_ccinc_sel_sig_apposc_nubmode_true_e_nu;
}
std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetFDNutauSwapAppOscPrediction(PRISM::BeamMode FDBM) const {
  return (FDBM == BeamMode::kNuMode)
             ? Predictions.FD.nutau_ccinc_sel_sig_apposc_numode
             : Predictions.FD.nutaubar_ccinc_sel_sig_apposc_nubmode;
}

//----------------------------------------------------------------------
Spectrum PredictionPRISM::Predict(osc::IOscCalc *calc) const {
   return PredictSyst(calc, kNoShift);
}

//----------------------------------------------------------------------
Spectrum PredictionPRISM::PredictSyst(osc::IOscCalc *calc,
                                      const SystShifts &shift) const {
  std::map<PredictionPRISM::PRISMComponent, Spectrum> Comps =
      PredictPRISMComponents(calc, shift);

  assert(Comps.size());

  return Comps.at(kNDDataCorr_FDExtrap);
}

// Function to extract ND background -> further use in in PredictPRISMComponents
std::map<PredictionPRISM::PRISMComponent, PRISMReweightableSpectrum>
PredictionPRISM::NDDataMinusBackground(osc::IOscCalc *calc, SystShifts shift_nd, MatchChan match_chan) const {

  std::map<PredictionPRISM::PRISMComponent, PRISMReweightableSpectrum> NDComps;
  //define anti match channel (opposite channels than the on used in the prediction -> needed for WS backgroun prediction)

  auto &NDData = GetNDData(match_chan.from, 293, fUseFakeData);
  auto &NDPrediction = GetNDPrediction(match_chan.from);

  auto &NDData_280kA = GetNDData(match_chan.from, 280, fUseFakeData);
  auto &NDPrediction_280kA = GetNDPrediction(match_chan.from, 280);

  // Sort out the flavors and signs
  auto NDSigFlavor = (match_chan.from.chan & NuChan::kNumuNumuBar)
                         ? Flavors::kAllNuMu
                         : Flavors::kAllNuE;
  auto NDSigSign = ((match_chan.from.chan & NuChan::kNumu) ||
                    (match_chan.from.chan & NuChan::kNue))
                       ? Sign::kNu
                       : Sign::kAntiNu;
  auto NDWrongSign = (NDSigSign == Sign::kNu) ? Sign::kAntiNu : Sign::kNu;
  auto NDWrongFlavor =
      (NDSigFlavor == Flavors::kAllNuMu) ? Flavors::kAllNuE : Flavors::kAllNuMu;

  RunPlan const &NDRunPlan = (match_chan.from.mode == PRISM::BeamMode::kNuMode)
                                 ? RunPlan_nu
                                 : RunPlan_nub;
  // Unweighted ND data
  NDComps.emplace(kNDData_unweighted_293kA, *NDData);

  // Weight the data to mock-up the proposed run-plan
  NDComps.emplace(
      kNDData_293kA,
      NDRunPlan.Weight(*NDData, 293, fOffPredictionAxis, fSetNDErrorsFromRate));
  NDComps.emplace(kNDDataCorr2D_293kA, NDComps.at(kNDData_293kA));

  // Unweighted 280kA component
  NDComps.emplace(kNDData_unweighted_280kA, *NDData_280kA);

  // Weight 280kA component
  NDComps.emplace(
      kNDData_280kA,
      NDRunPlan.Weight(*NDData_280kA, 280, f280kAPredictionAxis, fSetNDErrorsFromRate));
  NDComps.emplace(kNDDataCorr2D_280kA, NDComps.at(kNDData_280kA));

  NDComps.emplace(kNDSigOnly2D_293kA,
                  NDRunPlan.Weight(SetSpectrumErrors(
                      NDPrediction->PredictComponentSyst(
                          calc, shift_nd,
                          NDSigFlavor, Current::kCC, NDSigSign),
                      fDefaultOffAxisPOT),
                      293, fOffPredictionAxis));

  // Start building MC components
  // Try doing background subtraction for MC as well, could be helpful for
  // 'fake data' studies.
  if (fDebugPlots) {
    PRISMReweightableSpectrum NDSig = NDRunPlan.Weight(
        SetSpectrumErrors(
            NDPrediction->PredictSyst(calc, (fVaryNDFDMCData ? shift_nd : kNoShift)),
            fDefaultOffAxisPOT),
        293, fOffPredictionAxis);

    NDComps.emplace(kNDSig_293kA, NDSig);
    NDComps.emplace(kNDSig2D_293kA, NDSig);

    PRISMReweightableSpectrum NDSig_280kA = NDRunPlan.Weight(
        SetSpectrumErrors(NDPrediction_280kA->PredictSyst(
                              calc, (fVaryNDFDMCData ? shift_nd : kNoShift)),
                          fDefaultOffAxisPOT),
        280, f280kAPredictionAxis);

    NDComps.emplace(kNDSig_280kA, NDSig_280kA);
    NDComps.emplace(kNDSig2D_280kA, NDSig_280kA);
  }

  // ND background subtraction:
  if (fNCCorrection) { // NC background subraction.
    PRISMReweightableSpectrum NC =
      NDRunPlan.Weight(SetSpectrumErrors(NDPrediction->PredictComponentSyst(
                                         calc, (fVaryNDFDMCData ? kNoShift : shift_nd),
                                         Flavors::kAll, Current::kNC, Sign::kBoth),
                                         fDefaultOffAxisPOT),
                       293, fOffPredictionAxis);

    NDComps.emplace(kNDNCBkg_293kA, NC);
    NDComps.at(kNDDataCorr2D_293kA) -= NDComps.at(kNDNCBkg_293kA);
    if (fDebugPlots) NDComps.at(kNDSig2D_293kA) -= NDComps.at(kNDNCBkg_293kA);

    PRISMReweightableSpectrum NC_280kA = NDRunPlan.Weight(
        SetSpectrumErrors(NDPrediction_280kA->PredictComponentSyst(
                              calc, (fVaryNDFDMCData ? kNoShift : shift_nd),
                              Flavors::kAll, Current::kNC, Sign::kBoth),
                          fDefaultOffAxisPOT),
        280, f280kAPredictionAxis);

    NDComps.emplace(kNDNCBkg_280kA, NC_280kA);
    NDComps.at(kNDDataCorr2D_280kA) -= NDComps.at(kNDNCBkg_280kA);
    if (fDebugPlots) NDComps.at(kNDSig2D_280kA) -= NDComps.at(kNDNCBkg_280kA);
  }

  if (fWLBCorrection) { // Wrong lepton background subraction.
    PRISMReweightableSpectrum WLB = NDRunPlan.Weight(
        SetSpectrumErrors(NDPrediction->PredictComponentSyst(
                              calc, (fVaryNDFDMCData ? kNoShift : shift_nd),
                              NDWrongFlavor, Current::kCC, Sign::kBoth),
                          fDefaultOffAxisPOT),
        293, fOffPredictionAxis);

    NDComps.emplace(kNDWrongLepBkg_293kA, WLB);
    NDComps.at(kNDDataCorr2D_293kA) -= NDComps.at(kNDWrongLepBkg_293kA);
    if (fDebugPlots) NDComps.at(kNDSig2D_293kA) -= NDComps.at(kNDWrongLepBkg_293kA);

    PRISMReweightableSpectrum WLB_280kA = NDRunPlan.Weight(
        SetSpectrumErrors(NDPrediction_280kA->PredictComponentSyst(
                              calc, (fVaryNDFDMCData ? kNoShift : shift_nd),
                              NDWrongFlavor, Current::kCC, Sign::kBoth),
                          fDefaultOffAxisPOT),
        280, f280kAPredictionAxis);

    NDComps.emplace(kNDWrongLepBkg_280kA, WLB_280kA);
    NDComps.at(kNDDataCorr2D_280kA) -= NDComps.at(kNDWrongLepBkg_280kA);
    if (fDebugPlots) NDComps.at(kNDSig2D_280kA) -= NDComps.at(kNDWrongLepBkg_280kA);
  }

  if (fWSBCorrection) { // Wrong sign background subraction.
    PRISMReweightableSpectrum WSB = NDRunPlan.Weight(
        SetSpectrumErrors(NDPrediction->PredictComponentSyst(
                              calc, (fVaryNDFDMCData ? kNoShift : shift_nd),
                              NDSigFlavor, Current::kCC, NDWrongSign),
                          fDefaultOffAxisPOT),
        293, fOffPredictionAxis);

    NDComps.emplace(kNDWSBkg_293kA, WSB);
    NDComps.at(kNDDataCorr2D_293kA) -= NDComps.at(kNDWSBkg_293kA);
    if (fDebugPlots) NDComps.at(kNDSig2D_293kA) -= NDComps.at(kNDWSBkg_293kA);

    PRISMReweightableSpectrum WSB_280kA = NDRunPlan.Weight(
        SetSpectrumErrors(NDPrediction_280kA->PredictComponentSyst(
                              calc, (fVaryNDFDMCData ? kNoShift : shift_nd),
                              NDSigFlavor, Current::kCC, NDWrongSign),
                          fDefaultOffAxisPOT),
        280, f280kAPredictionAxis);

    NDComps.emplace(kNDWSBkg_280kA, WSB_280kA);
    NDComps.at(kNDDataCorr2D_280kA) -= NDComps.at(kNDWSBkg_280kA);
    if (fDebugPlots) NDComps.at(kNDSig2D_280kA) -= NDComps.at(kNDWSBkg_280kA);
  }
  return NDComps;
}

// function to Scale relative size of the weights to account for the run-plan.
// E.g. more data taken on-axis means a smaller weight for the
// on-axis position weights.
Spectrum PredictionPRISM::LinearCombinationCoefficientsSpectrun(MatchChan match_chan,
                          std::pair<Eigen::ArrayXd, Eigen::ArrayXd> LinearCombinationFromExtrapolator,
                          LabelsAndBins oaAxisBins, int kA) const{

    RunPlan const &NDRunPlan = (match_chan.from.mode == PRISM::BeamMode::kNuMode)
                                   ? RunPlan_nu
                                   : RunPlan_nub;
    double NDPOT = NDRunPlan.GetPlanPOT();

    // Scale relative size of the weights to account for the run-plan.
    // E.g. more data taken on-axis means a smaller weight for the
    // on-axis position weights.
    Eigen::ArrayXd UnRunPlannedLinearCombination;
    if(kA == 293){
        UnRunPlannedLinearCombination =
          NDRunPlan.Unweight(LinearCombinationFromExtrapolator.first, kA, oaAxisBins);
    }
    else if(kA == 280){
      UnRunPlannedLinearCombination =
        NDRunPlan.Unweight(LinearCombinationFromExtrapolator.second, kA, oaAxisBins);
    }
    if(kA!=293 && kA!=280){
      std::cout<<" no Linear combination is calculated for this current: "<<kA<<std::endl;
      abort();
    }
    // We don't want the total POT of the runplan to affect the scale of the
    // coefficients, just the shape.
    UnRunPlannedLinearCombination *= NDPOT;

    Spectrum UnRunPlannedLinearCombination_s(
        std::move(UnRunPlannedLinearCombination), oaAxisBins, NDPOT, 0);

    return UnRunPlannedLinearCombination_s;
}


std::map<PredictionPRISM::PRISMComponent, Spectrum>
PredictionPRISM::PredictPRISMComponents(osc::IOscCalc *calc, SystShifts shift,
                                        MatchChan match_chan) const {
  // always want the WSBkg prediction to be off for the oscillated spectrum PRISMPrediction. it will be on for the separate WSBkg prediction
  bool flagPredictionWSBkg = false;
  // this flag is only to be passed to the GerFarMatchCoefficients function
  //and will always be false as desired for the PRISMPrediction
  // this is needed in order to be able to have the WSB flag (from fhicl ) on and still be able to make
  // both the PRSIMPrediction as well as PRISMWSBPrediction

  bool WeHaveNDData = HaveNDData(match_chan.from);
  bool WeHaveNDPrediction = HaveNDPrediction(match_chan.from);
  bool WeHaveFDPrediction = HaveFDPrediction(match_chan.to);
  bool WeHaveFDUnOscWeightedSigPrediction =
      HaveFDUnOscWeightedSigPrediction(match_chan.to);

  if (!WeHaveNDData || !WeHaveNDPrediction || !WeHaveFDPrediction ||
      !WeHaveFDUnOscWeightedSigPrediction || !fFluxMatcher) {
    std::cout
        << "[ERROR]: Cannot make a PRISM prediction has we have: HaveNDData: "
        << (WeHaveNDData ? "have" : "do not have") << ", HaveNDPrediction: "
        << (WeHaveNDPrediction ? "have" : "do not have")
        << ", HaveFDPrediction: "
        << (WeHaveFDPrediction ? "have" : "do not have")
        << ", HaveFDUnOscWeightedSigPrediction: "
        << (WeHaveFDUnOscWeightedSigPrediction ? "have" : "do not have")
        << ", FluxMatcher: " << (fFluxMatcher ? "have" : "do not have")
        << std::endl;
    abort();
  }

  DontAddDirectory guard;

  auto &FDPrediction = GetFDPrediction(match_chan.to);
  auto &FDNonSwapAppOscPrediction =
      GetFDNonSwapAppOscPrediction(match_chan.to.mode);
  auto &FDNueSwapAppOscPrediction =
      GetFDNueSwapAppOscPrediction(match_chan.to.mode);
  auto &FDNutauSwapAppOscPrediction =
      GetFDNutauSwapAppOscPrediction(match_chan.to.mode);
  auto &FDUnOscWeightedSigPrediction =
      GetFDUnOscWeightedSigPrediction(match_chan.to);
  auto &FDNonSwapAppOscPredictionTrueEnu =
      GetFDNonSwapAppOscPredictionTrueEnu(match_chan.to.mode);
  auto &FDNueSwapAppOscPredictionTrueEnu =
      GetFDNueSwapAppOscPredictionTrueEnu(match_chan.to.mode);

//define the anti beam channel here - only needed for anti channel cross section Ratio
MatchChan antimatch_chan = GetAntiChannel(match_chan);

  // now define the FDNonSwap and FDNueSwap
  auto &FDNueSwapAppOscPrediction_AntiChannel =
      GetFDNueSwapAppOscPrediction(antimatch_chan.to.mode);
  auto &FDNonSwapAppOscPrediction_AntiChannel =
      GetFDNonSwapAppOscPrediction(antimatch_chan.to.mode);
  auto &FDNonSwapAppOscPredictionTrueEnu_AntiChannel =
      GetFDNonSwapAppOscPredictionTrueEnu(antimatch_chan.to.mode);
  auto &FDNueSwapAppOscPredictionTrueEnu_AntiChannel =
      GetFDNueSwapAppOscPredictionTrueEnu(antimatch_chan.to.mode);

  // Sort out the flavors and signs
  auto NDSigFlavor = (match_chan.from.chan & NuChan::kNumuNumuBar)
                         ? Flavors::kAllNuMu
                         : Flavors::kAllNuE;
  auto NDSigSign = ((match_chan.from.chan & NuChan::kNumu) ||
                    (match_chan.from.chan & NuChan::kNue))
                       ? Sign::kNu
                       : Sign::kAntiNu;
  auto NDWrongSign = (NDSigSign == Sign::kNu) ? Sign::kAntiNu : Sign::kNu;
  auto NDWrongFlavor =
      (NDSigFlavor == Flavors::kAllNuMu) ? Flavors::kAllNuE : Flavors::kAllNuMu;

  auto FDSigFlavor = (match_chan.to.chan & NuChan::kNumuNumuBar)
                         ? Flavors::kNuMuToNuMu
                         : ( (match_chan.to.chan & NuChan::kNueNueBar) ? Flavors::kNuMuToNuE : Flavors::kNuMuToNuTau);
  auto FDSigSign = ((match_chan.to.chan & NuChan::kNumu) ||
                    (match_chan.to.chan & NuChan::kNue)  ||
                    (match_chan.to.chan & NuChan::kNutau))
                       ? Sign::kNu
                       : Sign::kAntiNu;

  auto FDWrongSign = (FDSigSign == Sign::kNu) ? Sign::kAntiNu : Sign::kNu;
  auto FDWrongFlavor = (FDSigFlavor == Flavors::kNuMuToNuMu)
                           ? (Flavors::kAllNuE)
                           : ( (FDSigFlavor == Flavors::kNuMuToNuE) ? ( Flavors::kAllNuMu ) : ( Flavors::kAllNuMu | Flavors::kAllNuE ) );
  auto FDIntrinsicFlavor = (FDSigFlavor == Flavors::kNuMuToNuMu)
                               ? Flavors::kNuEToNuMu
                               : ( (FDSigFlavor == Flavors::kNuMuToNuE) ? Flavors::kNuEToNuE : Flavors::kNuEToNuTau );

  // ND and FD detector systtematics are totally uncorrelated. Do not want to
  // try and minimise an ND prediction by varying an FD energy scale syst.
  // ----------------------------------------------------------------
  std::vector<ana::ISyst const *> nd_systs = FilterOutFDSysts(shift.ActiveSysts());
  std::vector<ana::ISyst const *> fd_systs = FilterOutNDSysts(shift.ActiveSysts());

  std::map <const ISyst*, double> nd_shift_map, fd_shift_map;
  for (auto const syst : nd_systs) {
    double shift_sig = shift.GetShift(syst);
    nd_shift_map[syst] = shift_sig;
  }
  for (auto const syst : fd_systs) {
    double shift_sig = shift.GetShift(syst);
    fd_shift_map[syst] = shift_sig;
  }

  SystShifts shift_nd(nd_shift_map);
  SystShifts shift_fd(fd_shift_map);
  //-----------------------------------------------------------------

  // Using maps for non-default constructible classes is awful...
  bool TempFlagSetTrueForCheck = true;
  //get the ND data after background has been subtracted
  std::map<PredictionPRISM::PRISMComponent, PRISMReweightableSpectrum> NDComps = NDDataMinusBackground(calc, shift_nd, match_chan);
  std::map<PredictionPRISM::PRISMComponent, Spectrum> Comps;

  RunPlan const &NDRunPlan = (match_chan.from.mode == PRISM::BeamMode::kNuMode)
                                 ? RunPlan_nu
                                 : RunPlan_nub;

  double NDPOT = NDRunPlan.GetPlanPOT();
  assert(NDPOT > 0);

  // FD MC spectrum with analysis axis/axes + Enu axis.
  Spectrum FDUnWeightedSig_Spec =
      FDUnOscWeightedSigPrediction->PredictComponentSyst(
          calc, (fVaryNDFDMCData ? kNoShift : shift_fd), FDSigFlavor, Current::kCC,
          FDSigSign);
  //FD MC spectrum with analysis axis/axes + Enu axis for Intrinsic RSBkg (this wil be used only in the case of fMatchIntrinsicBkg for flux correction )
  Spectrum FDUnWeightedSig_Spec_IntrinsicRSB =
      FDUnOscWeightedSigPrediction->PredictComponentSyst(
          calc, (fVaryNDFDMCData ? kNoShift : shift_fd), Flavors::kNuEToNuE, Current::kCC,
          FDSigSign);
  //FD MC spectrum with analysis axis/axes + Enu axis for Intrinsic WSBkg (this wil be used only in the case of WSintrisicBkgmatch for flux correction )
  Spectrum FDUnWeightedSig_Spec_IntrinsicWSB =
      FDUnOscWeightedSigPrediction->PredictComponentSyst(
          calc, (fVaryNDFDMCData ? kNoShift : shift_fd), Flavors::kNuEToNuE, Current::kCC,
          FDWrongSign);
  //FD MC spectrum with analysis axis/axes + Enu axis for wrong sign background (muons ) from beam contamination (this wil be used only in the case of WSintrisicBkgmatch for flux correction )
  Spectrum FDUnWeightedSig_Spec_WSB =
      FDUnOscWeightedSigPrediction->PredictComponentSyst(
          calc, (fVaryNDFDMCData ? kNoShift : shift_fd), FDSigFlavor, Current::kCC,
          FDWrongSign);


  // Convert FD MC to RWSpectrum with Enu as reweight variable.
  PRISMReweightableSpectrum FDUnOscWeightedSig =
      ToReweightableSpectrum(FDUnWeightedSig_Spec, NDPOT);
  PRISMReweightableSpectrum FDUnOscWeightedSig_IntrinsicRSB =
      ToReweightableSpectrum(FDUnWeightedSig_Spec_IntrinsicRSB, NDPOT);
  // Convert FD MC to RWSpectrum with Enu as reweight variable for intrinsic WSBkg .
  PRISMReweightableSpectrum FDUnOscWeightedSig_IntrinsicWSB =
      ToReweightableSpectrum(FDUnWeightedSig_Spec_IntrinsicWSB, NDPOT);
  // Convert FD MC to RWSpectrum with Enu as reweight variable for WSBkg (muons ) from beam contamination .
  PRISMReweightableSpectrum FDUnOscWeightedSig_WSB =
      ToReweightableSpectrum(FDUnWeightedSig_Spec_WSB, NDPOT);

  // FD unoscillated prediction.
  Comps.emplace(kFDUnOscPred, FDUnOscWeightedSig.UnWeighted());
  if(fMatchIntrinsicBkg)
    Comps.at(kFDUnOscPred) += FDUnOscWeightedSig_IntrinsicRSB.UnWeighted();
  // FD unoscillated prediction WSbkg.
  if(fMatchWSBkg){
  Comps.emplace(kFDUnOscPredWSB, FDUnOscWeightedSig_WSB.UnWeighted());
    if( (match_chan.to.chan & NuChan::kNueApp) || (match_chan.to.chan & NuChan::kNueBarApp) )
      Comps.at(kFDUnOscPredWSB) += FDUnOscWeightedSig_IntrinsicWSB.UnWeighted();
  }

  // Numu -> Nue x-section correction as a function of true energy.
  if (FDSigFlavor == Flavors::kNuMuToNuE && fMatchIntrinsicBkg) {
    // Copied from unfold and smear xsec corr below
    // for numu/nue xsec corr when including FD intrinsic nue in flux matching
    // Does it need to be FD spectrum? ND numu/nue spectrum should also work
    Spectrum FD_nueapp_spectrum_True_E_nu =
        FDNueSwapAppOscPredictionTrueEnu->PredictComponentSyst(
            calc, (fVaryNDFDMCData ? kNoShift : shift_fd), Flavors::kNuMuToNuE,
            Current::kCC, NDSigSign);

    Spectrum FD_numusurv_apposc_spectrum_True_E_nu =
        FDNonSwapAppOscPredictionTrueEnu->PredictComponentSyst(
            calc, (fVaryNDFDMCData ? kNoShift : shift_fd), Flavors::kNuMuToNuMu,
            Current::kCC, NDSigSign);

    Comps.emplace(kFD_NumuNueCorr_Nue_TrueEnu, FD_nueapp_spectrum_True_E_nu);
    Comps.emplace(kFD_NumuNueCorr_Numu_TrueEnu, FD_numusurv_apposc_spectrum_True_E_nu);

    Ratio FD_NumuNue_XsecRatio_TrueEnu(FD_numusurv_apposc_spectrum_True_E_nu,
                                   FD_nueapp_spectrum_True_E_nu,
                                   NDPOT); // numu/nue

    fFluxMatcher->SetIntrinsicNueXsecRatio(std::move(FD_NumuNue_XsecRatio_TrueEnu.GetEigen()));
  }

  //for WSB background we want the opposite ND sign == NDWrongSign
  if(fMatchWSBkg){
    Spectrum FD_nueapp_spectrum_True_E_nu_AntiChannel =
        FDNueSwapAppOscPredictionTrueEnu_AntiChannel->PredictComponentSyst(
            calc, (fVaryNDFDMCData ? kNoShift : shift_fd), Flavors::kNuMuToNuE,
            Current::kCC, NDWrongSign);

    Spectrum FD_numusurv_apposc_spectrum_True_E_nu_AntiChannel =
        FDNonSwapAppOscPredictionTrueEnu_AntiChannel->PredictComponentSyst(
            calc, (fVaryNDFDMCData ? kNoShift : shift_fd), Flavors::kNuMuToNuMu,
            Current::kCC, NDWrongSign);

    Ratio FD_NumuNue_XsecRatio_TrueEnu_AntiChannel(FD_numusurv_apposc_spectrum_True_E_nu_AntiChannel,
                                   FD_nueapp_spectrum_True_E_nu_AntiChannel,
                                   NDPOT); // numu/nue

    fFluxMatcherWSB->SetIntrinsicNueXsecRatioAntiChannel(std::move(FD_NumuNue_XsecRatio_TrueEnu_AntiChannel.GetEigen()));
  }



  // linear combination for PRSIM prediction - oscillated spectrum
  std::pair<Eigen::ArrayXd, Eigen::ArrayXd> LinearCombination =
      fFluxMatcher->GetFarMatchCoefficients(
          calc, match_chan, (fVaryNDFDMCData ? kNoShift : shift), flagPredictionWSBkg);

  // Get the residual from the event rate/flux matcher.
  Eigen::ArrayXd resid_arr = fFluxMatcher->GetLastResidual();
  // Calculate FD flux miss-matching correction.
  if(!fMatchIntrinsicBkg)
    Comps.emplace(kFDFluxCorr, FDUnOscWeightedSig.WeightedByErrors(resid_arr));
  else //flux correction including the RS intrinsic bckg
    Comps.emplace(kFDFluxCorr, (FDUnOscWeightedSig+FDUnOscWeightedSig_IntrinsicRSB).WeightedByErrors(resid_arr));


  LabelsAndBins oaAxis(fNDOffAxis.GetLabels().at(0),
                       fNDOffAxis.GetBinnings().at(0));
  LabelsAndBins oaAxis280(fND280kAAxis.GetLabels().at(0),
                          fND280kAAxis.GetBinnings().at(0));

  // save coefficients vs off axis position - PRISM prediction - osc spectrum
  Comps.emplace(kNDFDWeightings_293kA, LinearCombinationCoefficientsSpectrun(match_chan, LinearCombination, oaAxis, 293));
  Comps.emplace(kNDFDWeightings_280kA, LinearCombinationCoefficientsSpectrun(match_chan, LinearCombination, oaAxis280, 280));
  // Off axis coefficients for weighting:
  Eigen::ArrayXd LinearCombinationCoeffs_293kA_arr =
      LinearCombinationCoefficientsSpectrun(match_chan, LinearCombination, oaAxis, 293).GetEigen(NDPOT);
  Eigen::ArrayXd LinearCombinationCoeffs_280kA_arr =
      LinearCombinationCoefficientsSpectrun(match_chan, LinearCombination, oaAxis280, 280).GetEigen(NDPOT);


  // Perform linear combination on raw (i.e. not extrapolated) ND data/MC.
  // MC linear combination:
  if (NDComps.count(kNDSig_293kA) && fDebugPlots) {
    Comps.emplace(kNDSig_293kA,
                  NDComps.at(kNDSig2D_293kA)
                      .WeightedByErrors(LinearCombinationCoeffs_293kA_arr));
    Comps.emplace(kNDSig_280kA,
                  NDComps.at(kNDSig2D_280kA)
                      .WeightedByErrors(LinearCombinationCoeffs_280kA_arr));

    Comps.emplace(kPRISMMC, Comps.at(kNDSig_293kA));
    Comps.at(kPRISMMC) += Comps.at(kNDSig_280kA);
  }
  // Data linear combination:
  Comps.emplace(kNDDataCorr_293kA,
                NDComps.at(kNDDataCorr2D_293kA)
                    .WeightedByErrors(LinearCombinationCoeffs_293kA_arr));
  Comps.emplace(kNDDataCorr_280kA,
                NDComps.at(kNDDataCorr2D_280kA)
                    .WeightedByErrors(LinearCombinationCoeffs_280kA_arr));

  Comps.emplace(kPRISMPred, Comps.at(kNDDataCorr_293kA));
  Comps.at(kPRISMPred) += Comps.at(kNDDataCorr_280kA);

  Comps.emplace(kNDLinearComb, Comps.at(kPRISMPred));

  // Numu -> Nue x-section correction as a function of true energy.
  if (FDSigFlavor == Flavors::kNuMuToNuE) {
    Spectrum FD_nueapp_spectrum =
        FDNueSwapAppOscPrediction->PredictComponentSyst(
            calc, (fVaryNDFDMCData ? kNoShift : shift_fd), Flavors::kNuMuToNuE,
            Current::kCC, NDSigSign);

    Spectrum FD_numusurv_apposc_spectrum =
        FDNonSwapAppOscPrediction->PredictComponentSyst(
            calc, (fVaryNDFDMCData ? kNoShift : shift_fd), Flavors::kNuMuToNuMu,
            Current::kCC, NDSigSign);

    Comps.emplace(kFD_NumuNueCorr_Nue, FD_nueapp_spectrum);
    Comps.emplace(kFD_NumuNueCorr_Numu, FD_numusurv_apposc_spectrum);

    Ratio FD_NumuNueCorr_r(FD_nueapp_spectrum, FD_numusurv_apposc_spectrum,
                           NDPOT);

    // Give extrapolation method access to the nue/numu ratio
    // so this can be applied to the unfolded ND data.
    fNDFD_Matrix->SetNumuNueCorr(std::move(FD_NumuNueCorr_r.GetEigen()));

    //define the spectra for the anti ND channel (NDWrongSign)
    Spectrum FD_nueapp_spectrum_AntiChannel =
        FDNueSwapAppOscPrediction_AntiChannel->PredictComponentSyst(
            calc, (fVaryNDFDMCData ? kNoShift : shift_fd), Flavors::kNuMuToNuE,
            Current::kCC, NDWrongSign);

    Spectrum FD_numusurv_apposc_spectrum_AntiChannel =
        FDNonSwapAppOscPrediction_AntiChannel->PredictComponentSyst(
            calc, (fVaryNDFDMCData ? kNoShift : shift_fd), Flavors::kNuMuToNuMu,
            Current::kCC, NDWrongSign);

    Ratio FD_NumuNueCorr_r_AntiChannel(FD_nueapp_spectrum_AntiChannel, FD_numusurv_apposc_spectrum_AntiChannel,
                           NDPOT);

    if(fMatchWSBkg)
      fNDFD_Matrix_WSB->SetNumuNueCorr(std::move(FD_NumuNueCorr_r_AntiChannel.GetEigen()));
  }

  // Numu -> Nutau x-section correction as a function of true energy.
  if (FDSigFlavor == Flavors::kNuMuToNuTau) {
    Spectrum FD_nutauapp_spectrum =
        FDNutauSwapAppOscPrediction->PredictComponentSyst(
            calc, (fVaryNDFDMCData ? kNoShift : shift), Flavors::kNuMuToNuTau,
            Current::kCC, NDSigSign);

    Spectrum FD_numusurv_apposc_spectrum =
        FDNonSwapAppOscPrediction->PredictComponentSyst(
            calc, (fVaryNDFDMCData ? kNoShift : shift), Flavors::kNuMuToNuMu,
            Current::kCC, NDSigSign);

    Comps.emplace(kFD_NumuNutauCorr_Nutau, FD_nutauapp_spectrum);
    Comps.emplace(kFD_NumuNutauCorr_Numu, FD_numusurv_apposc_spectrum);

    Ratio FD_NumuNutauCorr_r(FD_nutauapp_spectrum, FD_numusurv_apposc_spectrum,
                           NDPOT);

    // Give extrapolation method access to the nue/numu ratio
    // so this can be applied to the unfolded ND data.
    fNDFD_Matrix->SetNumuNutauCorr(std::move(FD_NumuNutauCorr_r.GetEigen()));
  }

  //-------------------------------------------------------------
  // Procedure for near to far extrapolation of PRISM prediction:
  // ------------------------------------------------------------
  HistAxis const &FDAnaAxis = (FDSigFlavor == Flavors::kNuMuToNuE) ?
                        fAnalysisAxisFD_nue :
                        fAnalysisAxisFD_numu;
  HistAxis const &CovarAxis = (FDSigFlavor == Flavors::kNuMuToNuE) ?
                        fCovarianceAxis_nue :
                        fCovarianceAxis_numu;

  LabelsAndBins ExtrapAnaAxis(FDAnaAxis.GetLabels(),
                              FDAnaAxis.GetBinnings());
  LabelsAndBins ExtrapWeightAxis(fNDOffAxis.GetLabels(),
                                 fNDOffAxis.GetBinnings());
  LabelsAndBins Extrap280kAWeightAxis(fND280kAAxis.GetLabels(),
                                      fND280kAAxis.GetBinnings());
  //off-axis binnings is same for ND channel and ND anti channel -> no change needed for WSB case
  LabelsAndBins CovAnaAxis(CovarAxis.GetLabels(),
                           CovarAxis.GetBinnings());
  LabelsAndBins CovWeightAxis(CovarAxis.GetLabels(),
                              CovarAxis.GetBinnings());

  // 1. Calculate efficiency of selection.
  fMCEffCorrection->CalcEfficiency(
      calc, FDAnaAxis, (fVaryNDFDMCData ? kNoShift : shift_nd),
      (fVaryNDFDMCData ? kNoShift : shift_fd), NDSigFlavor,
      FDSigFlavor, Current::kCC, NDSigSign, FDSigSign);
  // Do ND to FD detector extrapolation here
  // Extrapolate just the LC ND, not the MC
  // 2. Extrapolate 293kA sample.
  fNDFD_Matrix->ExtrapolateNDtoFD(
      NDComps.at(kNDDataCorr2D_293kA), NDPOT, 293,
      LinearCombinationCoeffs_293kA_arr, calc,
      (fVaryNDFDMCData ? kNoShift : shift_nd),
      (fVaryNDFDMCData ? kNoShift : shift_fd), NDSigFlavor,
      FDSigFlavor, Current::kCC, NDSigSign, FDSigSign,
      fMCEffCorrection->GetNDefficiency(293),
      fMCEffCorrection->GetFDefficiency());
  // 3. Extrapolate 280kA sample.
  fNDFD_Matrix->ExtrapolateNDtoFD(
      NDComps.at(kNDDataCorr2D_280kA), NDPOT, 280,
      LinearCombinationCoeffs_280kA_arr, calc,
      (fVaryNDFDMCData ? kNoShift : shift_nd),
      (fVaryNDFDMCData ? kNoShift : shift_fd), NDSigFlavor,
      FDSigFlavor, Current::kCC, NDSigSign, FDSigSign,
      fMCEffCorrection->GetNDefficiency(280),
      fMCEffCorrection->GetFDefficiency());

  // 4. Get extrapolated 293kA sample.
  PRISMReweightableSpectrum sNDExtrap_293kA(
      std::move(fNDFD_Matrix->GetNDExtrap_293kA()),
      std::move(fNDFD_Matrix->GetErrorMat_293kA()), ExtrapAnaAxis,
      ExtrapWeightAxis, NDPOT, 0);
  NDComps.emplace(kNDDataExtrap2D_293kA, sNDExtrap_293kA);

  // 5. Weight extrapolated 293kA ND data by linear combination coeffiecients.
  Comps.emplace(kNDDataExtrap_293kA, sNDExtrap_293kA.WeightedByErrors(
                                         LinearCombinationCoeffs_293kA_arr));
  // 6. Get covariance matrix propagated through 293kA linear combination.
  ReweightableSpectrum sCovMat(std::move(fNDFD_Matrix->GetCovMat_293kA()),
                               CovAnaAxis, CovWeightAxis, NDPOT, 0);

  // 7. Get extrapolated 280kA sample.
  PRISMReweightableSpectrum sNDExtrap_280kA(
      std::move(fNDFD_Matrix->GetNDExtrap_280kA()),
      std::move(fNDFD_Matrix->GetErrorMat_280kA()), ExtrapAnaAxis,
      Extrap280kAWeightAxis, NDPOT, 0);

  NDComps.emplace(kNDDataExtrap2D_280kA, sNDExtrap_280kA);

  // 8. Weight extrapolated 280kA ND data by linear combination coeffiecient.
  Comps.emplace(kNDDataExtrap_280kA, sNDExtrap_280kA.WeightedByErrors(
                                         LinearCombinationCoeffs_280kA_arr));

  // 9. Get covariance matrix propagated through 280 linear combination.
  //    Only interested in the total covariance matrix, so no need to save a
  //    seperate 280kA covariance matrix.
  ReweightableSpectrum sCovMat_280kA(std::move(fNDFD_Matrix->GetCovMat_280kA()),
                                     CovAnaAxis, CovWeightAxis, NDPOT, 0);
  sCovMat += sCovMat_280kA;

  // 10. Add weighted and extrapolated 293kA and 280kA samples to get
  //     extrapolated PRISM prediction.
  Comps.emplace(kNDData_FDExtrap, Comps.at(kNDDataExtrap_293kA));
  Comps.at(kNDData_FDExtrap) += Comps.at(kNDDataExtrap_280kA);
  // 11. For adding in MC corrections:
  Comps.emplace(kNDDataCorr_FDExtrap, Comps.at(kNDData_FDExtrap));
  //------------------------------------------------------------
  // Repeat extrapolation for MC for debugging & 'fake data' studies --> still TODO for WSB
  if (NDComps.count(kNDSig_293kA) && fVaryNDFDMCData && fDebugPlots) {

    fNDFD_Matrix->ExtrapolateNDtoFD(
        NDComps.at(kNDSig2D_293kA), NDPOT, 293,
        LinearCombinationCoeffs_293kA_arr, calc,
        (fVaryNDFDMCData ? kNoShift : shift_nd),
        (fVaryNDFDMCData ? kNoShift : shift_fd), NDSigFlavor,
        FDSigFlavor, Current::kCC, NDSigSign, FDSigSign,
        fMCEffCorrection->GetNDefficiency(293),
        fMCEffCorrection->GetFDefficiency());

    fNDFD_Matrix->ExtrapolateNDtoFD(
        NDComps.at(kNDSig2D_280kA), NDPOT, 280,
        LinearCombinationCoeffs_280kA_arr, calc,
        (fVaryNDFDMCData ? kNoShift : shift_nd),
        (fVaryNDFDMCData ? kNoShift : shift_fd), NDSigFlavor,
        FDSigFlavor, Current::kCC, NDSigSign, FDSigSign,
        fMCEffCorrection->GetNDefficiency(280),
        fMCEffCorrection->GetFDefficiency());

    PRISMReweightableSpectrum sNDMCExtrap_293kA(
        std::move(fNDFD_Matrix->GetNDExtrap_293kA()),
        std::move(fNDFD_Matrix->GetErrorMat_293kA()), ExtrapAnaAxis,
        ExtrapWeightAxis, NDPOT, 0);

    NDComps.emplace(kNDMCExtrap2D_293kA, sNDMCExtrap_293kA);

    Comps.emplace(kNDMCExtrap_293kA, sNDMCExtrap_293kA.WeightedByErrors(
                                         LinearCombinationCoeffs_293kA_arr));

    PRISMReweightableSpectrum sNDMCExtrap_280kA(
        std::move(fNDFD_Matrix->GetNDExtrap_280kA()),
        std::move(fNDFD_Matrix->GetErrorMat_280kA()), ExtrapAnaAxis,
        Extrap280kAWeightAxis, NDPOT, 0);

    NDComps.emplace(kNDMCExtrap2D_280kA, sNDMCExtrap_280kA);

    Comps.emplace(kNDMCExtrap_280kA, sNDMCExtrap_280kA.WeightedByErrors(
                                         LinearCombinationCoeffs_280kA_arr));

    Comps.emplace(kNDMC_FDExtrap, Comps.at(kNDMCExtrap_293kA));
    Comps.at(kNDMC_FDExtrap) += Comps.at(kNDMCExtrap_280kA);
  }
  //----------------------------------------------------------------

  //if no WSB prediction then the prediction will be empty
  Eigen::ArrayXd EmptyArray = Eigen::ArrayXd::Zero(FDUnOscWeightedSig_IntrinsicWSB.GetEigen(1).matrix().size());
  //define an empty spectrum (could be any other spectrum weighted by the Empty array )
  Spectrum EmptySpectrum = FDUnOscWeightedSig_IntrinsicWSB.WeightedByErrors(EmptyArray);
  if(!fMatchWSBkg){
    Comps.emplace(kPRISMPredFinalWSB, EmptySpectrum);
  }
  else if(fMatchWSBkg){
    //get NDdata from anti channel for WSB prediction
    std::map<PredictionPRISM::PRISMComponent, PRISMReweightableSpectrum> NDCompsAntiChannel = NDDataMinusBackground(calc, shift_nd, antimatch_chan);

    //define the Linear Combination for WSB
    std::pair<Eigen::ArrayXd, Eigen::ArrayXd> LinearCombinationWSB =
        fFluxMatcherWSB->GetFarMatchCoefficients(
            calc, match_chan, (fVaryNDFDMCData ? kNoShift : shift), fMatchWSBkg);
    // save coefficients vs off axis position - WSB prediction
    Comps.emplace(kNDFDWeightings_293kAWSB, LinearCombinationCoefficientsSpectrun(match_chan, LinearCombinationWSB, oaAxis, 293));
    Comps.emplace(kNDFDWeightings_280kAWSB, LinearCombinationCoefficientsSpectrun(match_chan, LinearCombinationWSB, oaAxis280, 280));
    // Off axis coefficients for weighting:
    Eigen::ArrayXd LinearCombinationCoeffs_293kA_arr_WSB =
    LinearCombinationCoefficientsSpectrun(match_chan, LinearCombinationWSB, oaAxis, 293).GetEigen(NDPOT);;

    Eigen::ArrayXd LinearCombinationCoeffs_280kA_arr_WSB =
    LinearCombinationCoefficientsSpectrun(match_chan, LinearCombinationWSB, oaAxis280, 280).GetEigen(NDPOT);

   // Add the option of shifting the data for WSB as well
   // Perform linear combination on raw (i.e. not extrapolated) ND data/MC.
   // WSB MC linear combination:
    if (NDCompsAntiChannel.count(kNDSig_293kA) && fDebugPlots) {
      Comps.emplace(kNDSig_293kA,
                    NDCompsAntiChannel.at(kNDSig2D_293kA)
                        .WeightedByErrors(LinearCombinationCoeffs_293kA_arr_WSB));
      Comps.emplace(kNDSig_280kA,
                    NDCompsAntiChannel.at(kNDSig2D_280kA)
                        .WeightedByErrors(LinearCombinationCoeffs_280kA_arr_WSB));

      Comps.emplace(kPRISMMCWSB, Comps.at(kNDSig_293kA));
      Comps.at(kPRISMMCWSB) += Comps.at(kNDSig_280kA);
    }


   // Data linear combination WSB:
    Comps.emplace(kNDDataCorr_293kAWSB,
                  NDCompsAntiChannel.at(kNDDataCorr2D_293kA)
                      .WeightedByErrors(LinearCombinationCoeffs_293kA_arr_WSB));
    Comps.emplace(kNDDataCorr_280kAWSB,
                  NDCompsAntiChannel.at(kNDDataCorr2D_280kA)
                      .WeightedByErrors(LinearCombinationCoeffs_280kA_arr_WSB));
    //WSB PRISM prediction
    Comps.emplace(kPRISMPredWSB, Comps.at(kNDDataCorr_293kAWSB));
    Comps.at(kPRISMPredWSB) += Comps.at(kNDDataCorr_280kAWSB);

    //same as in the classic prediction but always interested in the opposite channel ->WrongSign
    fMCEffCorrection_WSB->CalcEfficiency(
        calc, FDAnaAxis, (fVaryNDFDMCData ? kNoShift : shift_nd),
        (fVaryNDFDMCData ? kNoShift : shift_fd), NDSigFlavor,
        FDSigFlavor, Current::kCC, NDWrongSign, FDWrongSign);

    //---this is for checking the results with no efficiency systematic shift for WSB -> just for testing
    //--->comment below and uncomment above for correct results
    //fMCEffCorrection_WSB->CalcEfficiency(
    //      calc, FDAnaAxis, kNoShift,
    //      kNoShift, NDSigFlavor,
    //      FDSigFlavor, Current::kCC, NDWrongSign, FDWrongSign);

    fNDFD_Matrix_WSB->ExtrapolateNDtoFD(
        NDCompsAntiChannel.at(kNDDataCorr2D_293kA), NDPOT, 293,
        LinearCombinationCoeffs_293kA_arr_WSB, calc,
        (fVaryNDFDMCData ? kNoShift : shift_nd),
        (fVaryNDFDMCData ? kNoShift : shift_fd), NDSigFlavor,
        FDSigFlavor, Current::kCC, NDWrongSign, FDWrongSign,
        fMCEffCorrection_WSB->GetNDefficiency(293),
        fMCEffCorrection_WSB->GetFDefficiency());

    fNDFD_Matrix_WSB->ExtrapolateNDtoFD(
        NDCompsAntiChannel.at(kNDDataCorr2D_280kA), NDPOT, 280,
        LinearCombinationCoeffs_280kA_arr_WSB, calc,
        (fVaryNDFDMCData ? kNoShift : shift_nd),
        (fVaryNDFDMCData ? kNoShift : shift_fd), NDSigFlavor,
        FDSigFlavor, Current::kCC, NDWrongSign, FDWrongSign,
        fMCEffCorrection_WSB->GetNDefficiency(280),
        fMCEffCorrection_WSB->GetFDefficiency());

    PRISMReweightableSpectrum sNDExtrap_293kA_AntiChannel(
      std::move(fNDFD_Matrix_WSB->GetNDExtrap_293kA()),
      std::move(fNDFD_Matrix_WSB->GetErrorMat_293kA()), ExtrapAnaAxis,
      ExtrapWeightAxis, NDPOT, 0);

    NDCompsAntiChannel.emplace(kNDDataExtrap2D_293kA, sNDExtrap_293kA_AntiChannel);
    // 5. Weight extrapolated 293kA ND data by linear combination coeffiecients.
    Comps.emplace(kNDDataExtrap_293kAWSB, sNDExtrap_293kA_AntiChannel.WeightedByErrors(
                                           LinearCombinationCoeffs_293kA_arr_WSB));
    // 6. Get covariance matrix propagated through 293kA linear combination.
    ReweightableSpectrum sCovMat_AntiChannel(std::move(fNDFD_Matrix_WSB->GetCovMat_293kA()),
                                CovAnaAxis, CovWeightAxis, NDPOT, 0);

    // 7. Get extrapolated 280kA sample.
    PRISMReweightableSpectrum sNDExtrap_280kA_AntiChannel(
        std::move(fNDFD_Matrix_WSB->GetNDExtrap_280kA()),
        std::move(fNDFD_Matrix_WSB->GetErrorMat_280kA()), ExtrapAnaAxis,
        Extrap280kAWeightAxis, NDPOT, 0);

    NDCompsAntiChannel.emplace(kNDDataExtrap2D_280kA, sNDExtrap_280kA_AntiChannel);
    // 8. Weight extrapolated 280kA ND data by linear combination coeffiecient.
    Comps.emplace(kNDDataExtrap_280kAWSB, sNDExtrap_280kA_AntiChannel.WeightedByErrors(
                                           LinearCombinationCoeffs_280kA_arr_WSB));

    // 9. Get covariance matrix propagated through 280 linear combination.
    //    Only interested in the total covariance matrix, so no need to save a
    //    seperate 280kA covariance matrix.
    ReweightableSpectrum sCovMat_280kA_AntiChannel(std::move(fNDFD_Matrix_WSB->GetCovMat_280kA()),
                                       CovAnaAxis, CovWeightAxis, NDPOT, 0);
    sCovMat_AntiChannel += sCovMat_280kA_AntiChannel;


    // 10. Add weighted and extrapolated 293kA and 280kA samples to get
    //     extrapolated PRISM prediction.
    Comps.emplace(kPRISMPredFinalWSB, Comps.at(kNDDataExtrap_293kAWSB));
    Comps.at(kPRISMPredFinalWSB) += Comps.at(kNDDataExtrap_280kAWSB);

    //11. Get the residual from the event rate/flux matcher. (do this before calling another linear combination)
    Eigen::ArrayXd resid_arr_WSB = fFluxMatcherWSB->GetLastResidual();
    // Calculate FD flux miss-matching correction.
    if( (match_chan.to.chan & NuChan::kNueApp) || (match_chan.to.chan & NuChan::kNueBarApp) ) //WS intrinsic background only added in coefficients calc for NuE/NuEbar appearance channels
      Comps.emplace(kFDFluxCorrWSB, (FDUnOscWeightedSig_IntrinsicWSB+FDUnOscWeightedSig_WSB).WeightedByErrors(resid_arr_WSB));
    else
      Comps.emplace(kFDFluxCorrWSB, FDUnOscWeightedSig_WSB.WeightedByErrors(resid_arr_WSB));
    // Add Flux correction to extrapolated PRISM.
    Comps.at(kPRISMPredFinalWSB) += Comps.at(kFDFluxCorrWSB);

    // Convert final covariance matrix into 2D spectrum
    Comps.emplace(kExtrapCovarMatrix_WSB, ToSpectrum(sCovMat_AntiChannel, NDPOT));

    //------------------------------------------------------------
    // Repeat extrapolation for MC for debugging & 'fake data' studies --> for WSB
    if (NDCompsAntiChannel.count(kNDSig_293kA) && fVaryNDFDMCData && fDebugPlots) {

      fNDFD_Matrix_WSB->ExtrapolateNDtoFD(
          NDCompsAntiChannel.at(kNDSig2D_293kA), NDPOT, 293,
          LinearCombinationCoeffs_293kA_arr_WSB, calc,
          (fVaryNDFDMCData ? kNoShift : shift_nd),
          (fVaryNDFDMCData ? kNoShift : shift_fd), NDSigFlavor,
          FDSigFlavor, Current::kCC, NDWrongSign, FDWrongSign,
          fMCEffCorrection_WSB->GetNDefficiency(293),
          fMCEffCorrection_WSB->GetFDefficiency());

      fNDFD_Matrix_WSB->ExtrapolateNDtoFD(
          NDCompsAntiChannel.at(kNDSig2D_280kA), NDPOT, 280,
          LinearCombinationCoeffs_280kA_arr_WSB, calc,
          (fVaryNDFDMCData ? kNoShift : shift_nd),
          (fVaryNDFDMCData ? kNoShift : shift_fd), NDSigFlavor,
          FDSigFlavor, Current::kCC, NDWrongSign, FDWrongSign,
          fMCEffCorrection_WSB->GetNDefficiency(280),
          fMCEffCorrection_WSB->GetFDefficiency());

      PRISMReweightableSpectrum sNDMCExtrap_293kA_AntiChannel(
        std::move(fNDFD_Matrix_WSB->GetNDExtrap_293kA()),
        std::move(fNDFD_Matrix_WSB->GetErrorMat_293kA()), ExtrapAnaAxis,
        ExtrapWeightAxis, NDPOT, 0);

      Comps.emplace(kNDMCExtrap_293kAWSB, sNDMCExtrap_293kA_AntiChannel.WeightedByErrors(
                                            LinearCombinationCoeffs_293kA_arr_WSB));

      PRISMReweightableSpectrum sNDMCExtrap_280kA_AntiChannel(
        std::move(fNDFD_Matrix_WSB->GetNDExtrap_280kA()),
        std::move(fNDFD_Matrix_WSB->GetErrorMat_280kA()), ExtrapAnaAxis,
        Extrap280kAWeightAxis, NDPOT, 0);

      Comps.emplace(kNDMCExtrap_280kAWSB, sNDMCExtrap_280kA_AntiChannel.WeightedByErrors(
                                           LinearCombinationCoeffs_280kA_arr_WSB));

      Comps.emplace(kNDMC_FDExtrapWSB, Comps.at(kNDMCExtrap_293kAWSB));
      Comps.at(kNDMC_FDExtrapWSB) += Comps.at(kNDMCExtrap_280kAWSB);
      Comps.at(kNDMC_FDExtrapWSB) += Comps.at(kFDFluxCorrWSB);
    }


 }

  // If we have the FD background predictions add them back in.
  // Add variances of background to diagonal of covariance matrix.
  if (fNCCorrection) { // Add in NC background.
    Comps.emplace(kFDNCBkg, FDPrediction->PredictComponentSyst(
                                calc, (fVaryNDFDMCData ? kNoShift : shift_fd),
                                Flavors::kAll, Current::kNC, Sign::kBoth));
    if (fAxisAgreement && fDebugPlots) {
      Comps.at(kPRISMPred) += Comps.at(kFDNCBkg);
      Comps.at(kPRISMMC) += Comps.at(kFDNCBkg);
    }
    Comps.at(kNDDataCorr_FDExtrap) += Comps.at(kFDNCBkg);
    if (Comps.count(kNDMC_FDExtrap) && fDebugPlots)
      Comps.at(kNDMC_FDExtrap) += Comps.at(kFDNCBkg);
  }

  if (fWLBCorrection) { // Add in wrong lepton background.
    Comps.emplace(kFDWrongLepBkg,
                  FDPrediction->PredictComponentSyst(
                      calc, (fVaryNDFDMCData ? kNoShift : shift_fd), FDWrongFlavor,
                      Current::kCC, Sign::kBoth));
    if (fAxisAgreement && fDebugPlots) {
      Comps.at(kPRISMPred) += Comps.at(kFDWrongLepBkg);
      Comps.at(kPRISMMC) += Comps.at(kFDWrongLepBkg);
    }
    Comps.at(kNDDataCorr_FDExtrap) += Comps.at(kFDWrongLepBkg);
    if (Comps.count(kNDMC_FDExtrap) && fDebugPlots)
      Comps.at(kNDMC_FDExtrap) += Comps.at(kFDWrongLepBkg);
  }

  if (fNuTauCCCorrection && (FDSigFlavor != Flavors::kNuMuToNuTau) ) {
    // Miss-identified CC nu-taus always a background for nue and numu signal
    Comps.emplace(kFDNuTauCCBkg,
                  FDPrediction->PredictComponentSyst(
                      calc, (fVaryNDFDMCData ? kNoShift : shift_fd),
                      Flavors::kAllNuTau, Current::kCC, Sign::kBoth));
    if (fAxisAgreement && fDebugPlots) {
      Comps.at(kPRISMPred) += Comps.at(kFDNuTauCCBkg);
      Comps.at(kPRISMMC) += Comps.at(kFDNuTauCCBkg);
    }
    Comps.at(kNDDataCorr_FDExtrap) += Comps.at(kFDNuTauCCBkg);
    if (Comps.count(kNDMC_FDExtrap) && fDebugPlots)
      Comps.at(kNDMC_FDExtrap) += Comps.at(kFDNuTauCCBkg);
  }

  if (fWSBCorrection) { // Add in wrong sign background. Nue: Numu->Nue only.
    // save the MC WSBkg (i.e from the WSmuon beam contamination )
    Comps.emplace(kFDWSBkgMC,
                  FDPrediction->PredictComponentSyst(
                       calc, (fVaryNDFDMCData ? kNoShift : shift_fd), FDSigFlavor,
                       Current::kCC, FDWrongSign));
    if(!fMatchWSBkg){
      Comps.emplace(kFDWSBkg,
                    FDPrediction->PredictComponentSyst(
                        calc, (fVaryNDFDMCData ? kNoShift : shift_fd), FDSigFlavor,
                        Current::kCC, FDWrongSign));
    }
    else{ //if the WSBkg correction is on ALL (including the WS intrinsic bkg) WS bkg will be added to FDWSBkg as obtained from the WSbkg prediction
      Comps.emplace(kFDWSBkg, Comps.at(kPRISMPredFinalWSB));
    }

    if (fAxisAgreement && fDebugPlots) {
      Comps.at(kPRISMPred) += Comps.at(kFDWSBkg);
      Comps.at(kPRISMMC) += Comps.at(kFDWSBkg);
    }
    Comps.at(kNDDataCorr_FDExtrap) += Comps.at(kFDWSBkg);
    if (Comps.count(kNDMC_FDExtrap) && fDebugPlots){
      if(!fMatchWSBkg)
         Comps.at(kNDMC_FDExtrap) += Comps.at(kFDWSBkg);
      else
         Comps.at(kNDMC_FDExtrap) += Comps.at(kNDMC_FDExtrapWSB);
    }
  }

  if (fIntrinsicCorrection) { // Add in intrinsic correction: here we only add the RS intrinsic, as the WS intrinsic is already handled by the WS flag and added in FDWSBKg)

    if( (match_chan.to.chan & NuChan::kNueApp) || (match_chan.to.chan & NuChan::kNueBarApp) ){ //intrinsic bkg data driven for appearance channels only (for now)
      if(fMatchIntrinsicBkg){ //RS intrinsic data driven
        if(fMatchWSBkg) //RS+WS intrinsic data driven
          Comps.emplace(kFDIntrinsicBkg, EmptySpectrum); //empty->don't add anything to the final pred
        else //no WS intrinsic data driven
          Comps.emplace(kFDIntrinsicBkg,
                        FDPrediction->PredictComponentSyst(
                            calc, (fVaryNDFDMCData ? kNoShift : shift_fd),
                            FDIntrinsicFlavor, Current::kCC, FDWrongSign)); //only add the WS MC
        }
        else{ // no RS intrinsic data driven
          if(fMatchWSBkg)
            Comps.emplace(kFDIntrinsicBkg,
                          FDPrediction->PredictComponentSyst(
                              calc, (fVaryNDFDMCData ? kNoShift : shift_fd),
                              FDIntrinsicFlavor, Current::kCC, FDSigSign)); //only add the RS MC
          else //no WS no RS
            Comps.emplace(kFDIntrinsicBkg,
                          FDPrediction->PredictComponentSyst(
                              calc, (fVaryNDFDMCData ? kNoShift : shift_fd),
                              FDIntrinsicFlavor, Current::kCC, Sign::kBoth)); // add MC IntrinsicBkg for both (RS and WS)
        }
    }
    else{ //use MC IntrinsicBkg for survival (NuMu->NuMu resp. AntiNuMu->AntiNuMu) channels
      Comps.emplace(kFDIntrinsicBkg,
                    FDPrediction->PredictComponentSyst(
                        calc, (fVaryNDFDMCData ? kNoShift : shift_fd),
                        FDIntrinsicFlavor, Current::kCC, Sign::kBoth));
    }
    if (fAxisAgreement && fDebugPlots) {
      Comps.at(kPRISMPred) += Comps.at(kFDIntrinsicBkg);
      Comps.at(kPRISMMC) += Comps.at(kFDIntrinsicBkg);
    }
    Comps.at(kNDDataCorr_FDExtrap) += Comps.at(kFDIntrinsicBkg);
    if (Comps.count(kNDMC_FDExtrap) && fDebugPlots)
      Comps.at(kNDMC_FDExtrap) += Comps.at(kFDIntrinsicBkg);
  }

// save the intrinisc MC nuE background
  Comps.emplace(kFDIntrinsicNuEBkgRightSign,  FDPrediction->PredictComponentSyst(
      calc, shift_fd, FDIntrinsicFlavor, Current::kCC, FDSigSign) );
  Comps.emplace(kFDIntrinsicNuEBkgWrongSign, FDPrediction->PredictComponentSyst(
      calc, shift_fd, FDIntrinsicFlavor, Current::kCC, FDWrongSign) );
//total intrinisc nuE background
  Comps.emplace(kFDIntrinsicNuEBkgMC, Comps.at(kFDIntrinsicNuEBkgRightSign));
  Comps.at(kFDIntrinsicNuEBkgMC) += Comps.at(kFDIntrinsicNuEBkgWrongSign);

  // Always shift FDOsc pred, as this acts as our 'shifted fd data' when
  // doing fake data shifts
  if (fDebugPlots) {
    
    Comps.emplace(kFDOscPred,
                  FDPrediction->PredictComponentSyst(
                      calc, shift_fd, Flavors::kAll, Current::kBoth, Sign::kBoth));
    //save FDOsc pred for WSbkg
    Comps.emplace(kFDOscPredWSB, FDPrediction->PredictComponentSyst(
                     calc, shift_fd, FDSigFlavor, Current::kCC, FDWrongSign));

    // Sometimes may want to look just at Numu CC FD prediction, if so, un-comment
    // below and comment-out above.
    //Comps.emplace(kFDOscPred,
    //              FDPrediction->PredictComponentSyst(
    //                  calc, shift_fd, Flavors::kNuMuToNuMu, Current::kCC, Sign::kNu));
  }


  if (fAxisAgreement && fDebugPlots)
    Comps.at(kPRISMPred) += Comps.at(kFDFluxCorr);

  // Add Flux correction to extrapolated PRISM.
  Comps.at(kNDDataCorr_FDExtrap) += Comps.at(kFDFluxCorr);
  if (Comps.count(kNDMC_FDExtrap) && fDebugPlots)
    Comps.at(kNDMC_FDExtrap) += Comps.at(kFDFluxCorr);

  // Convert final covariance matrix into 2D spectrum
  Comps.emplace(kExtrapCovarMatrixOscSpectrum, ToSpectrum(sCovMat, NDPOT)); // if WSB flag is false this will be the same as kExtrapCovarMatrix
  Comps.emplace(kExtrapCovarMatrix, ToSpectrum(sCovMat, NDPOT));
  if(fMatchWSBkg)
    Comps.at(kExtrapCovarMatrix) += Comps.at(kExtrapCovarMatrix_WSB);

  if (NDComps.count(kPRISMMC) && fAxisAgreement && fDebugPlots) {
    Comps.at(kPRISMMC) += Comps.at(kFDFluxCorr);
  }
  for (auto const &NDC :
       NDComps) { // If you haven't been added, project to a 2D spectrum
    if (!Comps.count(NDC.first)) {
      if (NDC.second.GetBinnings().size() < 2) {
        Comps.emplace(NDC.first, ToSpectrum(NDC.second, NDPOT));
      } else {
        // 2D analysis axis means 3D ND data spectrum
        // Quite annoying to do, and not particularly helpful
      }
    }
  }


  return Comps;
}

std::map<PredictionPRISM::PRISMComponent, Spectrum>
PredictionPRISM::PredictGaussianFlux(double mean, double width,
                                     ana::SystShifts shift,
                                     BeamChan NDChannel) const {

  // bool WeHaveNDData = HaveNDData(NDChannel);
  // bool WeHaveNDPrediction = HaveNDPrediction(NDChannel);

  // if (!WeHaveNDData || !WeHaveNDPrediction) {
  //   std::cout
  //       << "[ERROR]: Cannot make a PRISM prediction has we have: HaveNDData:
  //       "
  //       << (WeHaveNDData ? "have" : "do not have") << ", WeHaveNDPrediction:
  //       "
  //       << (WeHaveNDPrediction ? "have" : "do not have") << std::endl;
  //   abort();
  // }

  // DontAddDirectory guard;

  // auto &NDData = GetNDData(NDChannel);
  // auto &NDPrediction = GetNDPrediction(NDChannel);

  // // Sort out the flavors and signs
  // auto NDSigFlavor = (NDChannel.chan & NuChan::kNumuNumuBar) ?
  // Flavors::kAllNuMu
  //                                                            :
  //                                                            Flavors::kAllNuE;
  // auto NDSigSign =
  //     ((NDChannel.chan & NuChan::kNumu) || (NDChannel.chan & NuChan::kNue))
  //         ? Sign::kNu
  //         : Sign::kAntiNu;
  // auto NDWrongSign = (NDSigSign == Sign::kNu) ? Sign::kAntiNu : Sign::kNu;
  // auto NDWrongFlavor =
  //     (NDSigFlavor == Flavors::kAllNuMu) ? Flavors::kAllNuE :
  //     Flavors::kAllNuMu;

  // // Using maps for non-default constructible classes is awful...
  // std::map<PredictionPRISM::PRISMComponent, ReweightableSpectrum> NDComps;
  std::map<PredictionPRISM::PRISMComponent, Spectrum> Comps;
  // NDComps.emplace(kNDData, *NDData);
  // NDComps.emplace(kNDDataCorr2D, NDComps.at(kNDData));

  // double NDPOT = NDData->POT();

  // static osc::NoOscillations no;

  // // Start building MC components
  // Spectrum NDSig_spec = NDPrediction->PredictComponentSyst(
  //     &no, shift, NDSigFlavor, Current::kCC, NDSigSign);

  // std::unique_ptr<TH2> NDSig_h(NDSig_spec.ToTH2(NDPOT));

  // ReweightableSpectrum NDSig(ana::Constant(1), NDSig_h.get(),
  //                            fAnalysisAxis.GetLabels(),
  //                            fAnalysisAxis.GetBinnings(), NDPOT, 0);

  // NDComps.emplace(kNDSig2D, NDSig);

  // // ND Background subtraction
  // if (fNCCorrection) {
  //   std::unique_ptr<TH2> NC_h(
  //       NDPrediction
  //           ->PredictComponentSyst(&no, shift, Flavors::kAll, Current::kNC,
  //                                  Sign::kBoth)
  //           .ToTH2(NDPOT));
  //   ReweightableSpectrum NC(ana::Constant(1), NC_h.get(),
  //                           fAnalysisAxis.GetLabels(),
  //                           fAnalysisAxis.GetBinnings(), NDPOT, 0);

  //   NDComps.emplace(kNDNCBkg, NC);
  //   NDComps.at(kNDDataCorr2D) -= NDComps.at(kNDNCBkg);
  // }

  // if (fWLBCorrection) {
  //   std::unique_ptr<TH2> WLB_h(
  //       NDPrediction
  //           ->PredictComponentSyst(&no, shift, NDWrongFlavor, Current::kCC,
  //                                  Sign::kBoth)
  //           .ToTH2(NDPOT));
  //   ReweightableSpectrum Nue(ana::Constant(1), WLB_h.get(),
  //                            fAnalysisAxis.GetLabels(),
  //                            fAnalysisAxis.GetBinnings(), NDPOT, 0);

  //   NDComps.emplace(kNDWrongLepBkg, Nue);
  //   NDComps.at(kNDDataCorr2D) -= NDComps.at(kNDWrongLepBkg);
  // }

  // if (fWSBCorrection) {
  //   std::unique_ptr<TH2> WSB_h(
  //       NDPrediction
  //           ->PredictComponentSyst(&no, shift, NDSigFlavor, Current::kCC,
  //                                  NDWrongSign)
  //           .ToTH2(NDPOT));
  //   ReweightableSpectrum WSB(ana::Constant(1), WSB_h.get(),
  //                            fAnalysisAxis.GetLabels(),
  //                            fAnalysisAxis.GetBinnings(), NDPOT, 0);

  //   NDComps.emplace(kNDWSBkg, WSB);
  //   NDComps.at(kNDDataCorr2D) -= NDComps.at(kNDWSBkg);
  // }

  // // Linear Combination
  // TH1 const *LinearCombination = fFluxMatcher->GetGaussianCoefficients(
  //     mean, width, NDChannel, shift);

  // Comps.emplace(kNDDataCorr,
  //               NDComps.at(kNDDataCorr2D).WeightedByErrors(LinearCombination));
  // Comps.emplace(kPRISMPred, Comps.at(kNDDataCorr));

  // Comps.emplace(kNDSig,
  //               NDComps.at(kNDSig2D).WeightedByErrors(LinearCombination));

  // for (auto const &NDC :
  //      NDComps) { // If you haven't been added, project to a 2D spectrum
  //   if (!Comps.count(NDC.first)) {
  //     Comps.emplace(NDC.first, NDC.second.ToSpectrum());
  //   }
  // }
  return Comps;
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
           {"P_FD_nutau_ccinc_sel_numode", Predictions.FD.nutau_ccinc_sel_numode},
           {"P_FD_numubar_ccinc_sel_nubmode",Predictions.FD.numubar_ccinc_sel_nubmode},
           {"P_FD_nuebar_ccinc_sel_nubmode",Predictions.FD.nuebar_ccinc_sel_nubmode},
           {"P_FD_nutaubar_ccinc_sel_nubmode",Predictions.FD.nutaubar_ccinc_sel_nubmode}}) {
    if (meas.second.get()) {
      meas.second.get()->SaveTo(dir, meas.first.c_str());
    }
  }

  for (auto &meas : NamedPredInterpCRefVect{
           {"P_FD_numu_ccinc_sel_sig_numode",
            Predictions.FD.numu_ccinc_sel_sig_numode},
           {"P_FD_nue_ccinc_sel_sig_numode",
            Predictions.FD.nue_ccinc_sel_sig_numode},
           {"P_FD_nutau_ccinc_sel_sig_numode",
            Predictions.FD.nutau_ccinc_sel_sig_numode},
           {"P_FD_numubar_ccinc_sel_sig_nubmode",
            Predictions.FD.numubar_ccinc_sel_sig_nubmode},
           {"P_FD_nuebar_ccinc_sel_sig_nubmode",
            Predictions.FD.nuebar_ccinc_sel_sig_nubmode},
           {"P_FD_nutaubar_ccinc_sel_sig_nubmode",
            Predictions.FD.nutaubar_ccinc_sel_sig_nubmode},
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
            Predictions.FD.nuebar_ccinc_sel_sig_apposc_nubmode_true_e_nu},
           {"P_FD_nutau_ccinc_sel_sig_apposc_numode",
            Predictions.FD.nutau_ccinc_sel_sig_apposc_numode},
           {"P_FD_nutaubar_ccinc_sel_sig_apposc_nubmode",
            Predictions.FD.nutaubar_ccinc_sel_sig_apposc_nubmode}}) {

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

  for (unsigned int i = 0; i < fAnalysisAxisFD_numu.GetBinnings().size(); ++i) {
    TObjString(fAnalysisAxisFD_numu.GetLabels()[i].c_str())
        .Write(TString::Format("predFDnumu_label%d", i).Data());
    std::string str("predFDnumu_bins");
    str += std::to_string(i);
    fAnalysisAxisFD_numu.GetBinnings()[i].SaveTo(dir, str);
  }

  for (unsigned int i = 0; i < fAnalysisAxisFD_nue.GetBinnings().size(); ++i) {
    TObjString(fAnalysisAxisFD_nue.GetLabels()[i].c_str())
        .Write(TString::Format("predFDnue_label%d", i).Data());
    std::string str("predFDnue_bins");
    str += std::to_string(i);
    fAnalysisAxisFD_nue.GetBinnings()[i].SaveTo(dir, str);
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

  std::vector<std::string> predFDnumu_labels;
  std::vector<Binning> predFDnumu_bins;
  std::vector<Var> predFDnumu_dummy_vars;

  for (int i = 0;; ++i) {
    TDirectory *subdir = dir->GetDirectory(TString::Format("predFDnumu_bins%d", i));
    if (!subdir) {
      break;
    }
    std::string str("predFDnumu_bins");
    str += std::to_string(i);
    predFDnumu_bins.push_back(*Binning::LoadFrom(dir, str));
    TObjString *label =
        (TObjString *)dir->Get(TString::Format("predFDnumu_label%d", i));
    predFDnumu_labels.push_back(label ? label->GetString().Data() : "");
    predFDnumu_dummy_vars.push_back(Constant(1));
  }

  std::vector<std::string> predFDnue_labels;
  std::vector<Binning> predFDnue_bins;
  std::vector<Var> predFDnue_dummy_vars;

  for (int i = 0;; ++i) {
    TDirectory *subdir = dir->GetDirectory(TString::Format("predFDnue_bins%d", i));
    if (!subdir) {
      break;
    }
    std::string str("predFDnue_bins");
    str += std::to_string(i);
    predFDnue_bins.push_back(*Binning::LoadFrom(dir, str));
    TObjString *label =
        (TObjString *)dir->Get(TString::Format("predFDnue_label%d", i));
    predFDnue_labels.push_back(label ? label->GetString().Data() : "");
    predFDnue_dummy_vars.push_back(Constant(1));
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
  HistAxis const predictionAxisFD_numu(predFDnumu_labels, predFDnumu_bins,
                                       predFDnumu_dummy_vars);
  HistAxis const predictionAxisFD_nue(predFDnue_labels, predFDnue_bins,
                                       predFDnue_dummy_vars);
  HistAxis const offAxis(offaxis_labels, offaxis_bins, offaxis_dummy_vars);
  HistAxis const _280kAAxis(_280kAaxis_labels, _280kAaxis_bins,
                            _280kAaxis_dummy_vars);
  HistAxis const energyMatchAxis(ematch_labels, ematch_bins, ematch_dummy_vars);
  HistAxis const predictionAxisTrue(predTrue_labels, predTrue_bins,
                                    predTrue_dummy_vars);

  std::vector<HistAxis> AxisVec_numu = {predictionAxisFD_numu};
  std::vector<HistAxis> AxisVec_nue = {predictionAxisFD_nue};
  HistAxis const covarianceAxis_numu = GetMatrixAxis(AxisVec_numu);
  HistAxis const covarianceAxis_nue = GetMatrixAxis(AxisVec_nue);

  HistAxis const offPredictionAxis = GetTwoDAxis(predictionAxisND, offAxis);
  HistAxis const off280kAPredictionAxis =
      GetTwoDAxis(predictionAxisND, _280kAAxis);
  HistAxis const fluxMatcherCorrAxis_numu =
      GetTwoDAxis(predictionAxisFD_numu, energyMatchAxis);
  HistAxis const fluxMatcherCorrAxis_nue =
      GetTwoDAxis(predictionAxisFD_nue, energyMatchAxis);

  std::unique_ptr<PredictionPRISM> pred = std::make_unique<PredictionPRISM>(
      predictionAxisND, predictionAxisFD_numu, predictionAxisFD_nue, offAxis, _280kAAxis,
      predictionAxisTrue, energyMatchAxis, covarianceAxis_numu, covarianceAxis_nue,
      offPredictionAxis, off280kAPredictionAxis, fluxMatcherCorrAxis_numu, fluxMatcherCorrAxis_nue);
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
      pred->SetDefaultOffAxisPOT(MCPOT); // Save the default POT from ND MC file.
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
      meas.second.get()->MinimizeMemory();
    }
  }

  for (auto &meas : NamedPredInterpRefVect{
           {"P_FD_numu_ccinc_sel_numode",
            pred->Predictions.FD.numu_ccinc_sel_numode},
           {"P_FD_nue_ccinc_sel_numode",
            pred->Predictions.FD.nue_ccinc_sel_numode},
           {"P_FD_nutau_ccinc_sel_numode",
            pred->Predictions.FD.nutau_ccinc_sel_numode},
           {"P_FD_numubar_ccinc_sel_nubmode",
            pred->Predictions.FD.numubar_ccinc_sel_nubmode},
           {"P_FD_nuebar_ccinc_sel_nubmode",
            pred->Predictions.FD.nuebar_ccinc_sel_nubmode},
           {"P_FD_nutaubar_ccinc_sel_nubmode",
            pred->Predictions.FD.nutaubar_ccinc_sel_nubmode}}) {
    if (dir->GetDirectory(meas.first.c_str())) {
      meas.second.get() = PredictionInterp::LoadFrom(dir, meas.first.c_str());
      meas.second.get()->MinimizeMemory();
    }
  }

  for (auto &meas : NamedPredInterpRefVect{
           {"P_FD_numu_ccinc_sel_sig_numode",
            pred->Predictions.FD.numu_ccinc_sel_sig_numode},
           {"P_FD_nue_ccinc_sel_sig_numode",
            pred->Predictions.FD.nue_ccinc_sel_sig_numode},
           {"P_FD_nutau_ccinc_sel_sig_numode",
            pred->Predictions.FD.nutau_ccinc_sel_sig_numode},
           {"P_FD_numubar_ccinc_sel_sig_nubmode",
            pred->Predictions.FD.numubar_ccinc_sel_sig_nubmode},
           {"P_FD_nuebar_ccinc_sel_sig_nubmode",
            pred->Predictions.FD.nuebar_ccinc_sel_sig_nubmode},
           {"P_FD_nutaubar_ccinc_sel_sig_nubmode",
            pred->Predictions.FD.nutaubar_ccinc_sel_sig_nubmode},
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
            pred->Predictions.FD.numubar_ccinc_sel_sig_apposc_nubmode_true_e_nu},
           {"P_FD_nue_ccinc_sel_sig_apposc_numode_true_e_nu",
            pred->Predictions.FD.nue_ccinc_sel_sig_apposc_numode_true_e_nu},
           {"P_FD_nuebar_ccinc_sel_sig_apposc_nubmode_true_e_nu",
            pred->Predictions.FD.nuebar_ccinc_sel_sig_apposc_nubmode_true_e_nu},
           {"P_FD_nutau_ccinc_sel_sig_apposc_numode",
            pred->Predictions.FD.nutau_ccinc_sel_sig_apposc_numode},
           {"P_FD_nutaubar_ccinc_sel_sig_apposc_nubmode",
            pred->Predictions.FD.nutaubar_ccinc_sel_sig_apposc_nubmode}}) {
    if (dir->GetDirectory(meas.first.c_str())) {
      meas.second.get() = PredictionInterp::LoadFrom(dir, meas.first.c_str());
      meas.second.get()->MinimizeMemory();
    }
  }

  delete dir;

  return pred;
}

} // namespace ana
