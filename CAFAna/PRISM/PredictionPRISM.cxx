#include "CAFAna/PRISM/PredictionPRISM.h"
#include "CAFAna/PRISM/PRISMUtils.h"

#include "CAFAna/Prediction/PredictionsForPRISM.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/OscCurve.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "OscLib/func/IOscCalculator.h"

#include "TAxis.h"
#include "TDirectory.h"
#include "TH2.h"
#include "TObjString.h"

using namespace PRISM;

namespace ana {

static std::unique_ptr<ReweightableSpectrum> kNoSuchNDDataSpectrum{nullptr};
static std::unique_ptr<PredictionInterp> kNoSuchNDPredictionSpectrum{nullptr};
static std::unique_ptr<PredictionInterp> kNoSuchFDPredictionSpectrum{nullptr};

PredictionPRISM::PredictionPRISM(const HistAxis &AnalysisAxis,
                                 const HistAxis &NDOffAxis,
                                 const HistAxis &ND280kAAxis,
                                 const HistAxis &NDFDEnergyMatchAxis) {

  fSetNDErrorsFromRate = false;

  fAnalysisAxis = AnalysisAxis;
  PRISMOUT("PRISM analysis axis: "
           << fAnalysisAxis.GetLabels().front() << " with "
           << fAnalysisAxis.GetBinnings().front().Edges().size()
           << " bins from "
           << fAnalysisAxis.GetBinnings().front().Edges().front() << " to "
           << fAnalysisAxis.GetBinnings().front().Edges().back() << ".");

  fNDOffAxis = NDOffAxis;
  PRISMOUT("PRISM Off axis axis: "
           << fNDOffAxis.GetLabels().front() << " with "
           << fNDOffAxis.GetBinnings().front().Edges().size() << " bins from "
           << fNDOffAxis.GetBinnings().front().Edges().front() << " to "
           << fNDOffAxis.GetBinnings().front().Edges().back() << ".");

  fND280kAAxis = ND280kAAxis;
  PRISMOUT("PRISM 280 kA axis: "
           << fND280kAAxis.GetLabels().front() << " with "
           << fND280kAAxis.GetBinnings().front().Edges().size() << " bins from "
           << fND280kAAxis.GetBinnings().front().Edges().front() << " to "
           << fND280kAAxis.GetBinnings().front().Edges().back() << ".");

  fNDFDEnergyMatchAxis = NDFDEnergyMatchAxis;
  PRISMOUT("PRISM NDFDEnergyMatchAxis axis: "
           << fNDFDEnergyMatchAxis.GetLabels().front() << " with "
           << fNDFDEnergyMatchAxis.GetBinnings().front().Edges().size()
           << " bins from "
           << fNDFDEnergyMatchAxis.GetBinnings().front().Edges().front()
           << " to "
           << fNDFDEnergyMatchAxis.GetBinnings().front().Edges().back() << ".");

  fFluxMatcher = nullptr;

  fNCCorrection = true;
  fWSBCorrection = true;
  fWLBCorrection = true;
  fIntrinsicCorrection = true;

  std::vector<std::string> OffPrediction_Labels = fAnalysisAxis.GetLabels();
  std::vector<Binning> OffPrediction_Bins = fAnalysisAxis.GetBinnings();
  std::vector<Var> OffPrediction_Vars = fAnalysisAxis.GetVars();
  OffPrediction_Labels.push_back(fNDOffAxis.GetLabels().front());
  OffPrediction_Bins.push_back(fNDOffAxis.GetBinnings().front());
  OffPrediction_Vars.push_back(fNDOffAxis.GetVars().front());
  fOffPredictionAxis =
      HistAxis(OffPrediction_Labels, OffPrediction_Bins, OffPrediction_Vars);

  std::vector<std::string> _280kAPrediction_Labels = fAnalysisAxis.GetLabels();
  std::vector<Binning> _280kAPrediction_Bins = fAnalysisAxis.GetBinnings();
  std::vector<Var> _280kAPrediction_Vars = fAnalysisAxis.GetVars();
  _280kAPrediction_Labels.push_back(fND280kAAxis.GetLabels().front());
  _280kAPrediction_Bins.push_back(fND280kAAxis.GetBinnings().front());
  _280kAPrediction_Vars.push_back(fND280kAAxis.GetVars().front());
  f280kAPredictionAxis = HistAxis(_280kAPrediction_Labels,
                                  _280kAPrediction_Bins, _280kAPrediction_Vars);

  std::vector<double> offAxisBinEdges =
      fNDOffAxis.GetBinnings().front().Edges();

  // the center of the last bin
  fMaxOffAxis =
      (offAxisBinEdges.back() + offAxisBinEdges[offAxisBinEdges.size() - 2]) /
      2.0;

  // TODO we should make this correct every bin by its width, this currently
  // uses the second bin width only because a hack that uses the first bin to
  // mock up a special beam run
  double xslice_width_cm = (offAxisBinEdges[2] - offAxisBinEdges[1]) * 1E2;
  fDefaultOffAxisPOT = 1.0 / FD_ND_FVRatio(xslice_width_cm);

  std::vector<std::string> FluxMatcherCorrection_Labels =
      fAnalysisAxis.GetLabels();
  std::vector<Binning> FluxMatcherCorrection_Bins = fAnalysisAxis.GetBinnings();
  std::vector<Var> FluxMatcherCorrection_Vars = fAnalysisAxis.GetVars();
  FluxMatcherCorrection_Labels.push_back(
      fNDFDEnergyMatchAxis.GetLabels().front());
  FluxMatcherCorrection_Bins.push_back(
      fNDFDEnergyMatchAxis.GetBinnings().front());
  FluxMatcherCorrection_Vars.push_back(fNDFDEnergyMatchAxis.GetVars().front());
  fFluxMatcherCorrectionAxes =
      HistAxis(FluxMatcherCorrection_Labels, FluxMatcherCorrection_Bins,
               FluxMatcherCorrection_Vars);
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
                                      const Cut &cut, const Var &wei,
                                      ana::SystShifts shift,
                                      PRISM::BeamChan NDChannel) {
  std::unique_ptr<ReweightableSpectrum> &NDData = GetNDData(NDChannel);
  if (&NDData == &kNoSuchNDDataSpectrum) {
    std::cout << "ERROR: Invalid ND data type passed: " << NDChannel.chan << ":"
              << NDChannel.mode << std::endl;
    abort();
  }
  NDData = std::make_unique<ReweightableSpectrum>(
      ND_loader, fAnalysisAxis, fNDOffAxis, cut && kCut280kARun, shift, wei);

  std::unique_ptr<ReweightableSpectrum> &NDData_280kA =
      GetNDData_280kA(NDChannel);
  if (&NDData_280kA == &kNoSuchNDDataSpectrum) {
    std::cout << "ERROR: Invalid ND data type passed: " << NDChannel.chan << ":"
              << NDChannel.mode << std::endl;
    abort();
  }
  NDData_280kA = std::make_unique<ReweightableSpectrum>(
      ND_loader, fAnalysisAxis, fND280kAAxis, cut && kSel280kARun, shift,
      wei * kSpecHCRunWeight);
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
                                    const Var &wei,
                                    std::vector<ana::ISyst const *> systlist,
                                    PRISM::BeamChan NDChannel) {

  std::unique_ptr<PredictionInterp> &NDPrediction = GetNDPrediction(NDChannel);
  if (&NDPrediction == &kNoSuchNDPredictionSpectrum) {
    std::cout << "ERROR: Invalid ND MC type passed: " << NDChannel.chan << ":"
              << NDChannel.mode << std::endl;
    abort();
  }

  osc::NoOscillations kNoOsc;

  fPredGens.push_back(std::make_unique<NoOscPredictionGenerator>(
      fOffPredictionAxis, cut && kCut280kARun, wei));

  NDPrediction = std::make_unique<PredictionInterp>(systlist, &kNoOsc,
                                                    *fPredGens.back(), loaders);

  std::unique_ptr<PredictionInterp> &NDPrediction_280kA =
      GetNDPrediction_280kA(NDChannel);
  if (&NDPrediction_280kA == &kNoSuchNDPredictionSpectrum) {
    std::cout << "ERROR: Invalid ND MC type passed: " << NDChannel.chan << ":"
              << NDChannel.mode << std::endl;
    abort();
  }

  fPredGens.push_back(std::make_unique<NoOscPredictionGenerator>(
      f280kAPredictionAxis, cut && kSel280kARun, wei * kSpecHCRunWeight));

  NDPrediction_280kA = std::make_unique<PredictionInterp>(
      systlist, &kNoOsc, *fPredGens.back(), loaders);
}

///\brief Call to add a FD MC component
///
///\details This can be called a number of times to add various FD MC
/// components that might be used in the PRISM analysis. Components are
/// defined by the combination ofNuChan andBeamMode.
/// The list of valid components is given below:
/// * NumuNumuBar, NuMode (i.e. mu-like FHC)
/// * NumuNumuBar, NuBarMode (i.e. mu-like FHC)
/// * NueNueBar, NuMode (i.e. all e-like FHC)
/// * NueNueBar, NuBarMode (i.e. all e-like RHC)
///
/// The systlist add allowable systematic shifts to the constructed PredInterp
/// for use in fits and systematic studies.
void PredictionPRISM::AddFDMCLoader(Loaders &loaders, const Cut &cut,
                                    const Var &wei,
                                    std::vector<ana::ISyst const *> systlist,
                                    PRISM::BeamChan FDChannel) {

  osc::NoOscillations kNoOsc;

  std::unique_ptr<PredictionInterp> &FDPrediction = GetFDPrediction(FDChannel);
  if (&FDPrediction == &kNoSuchFDPredictionSpectrum) {
    std::cout << "ERROR: Invalid FD MC type passed: " << FDChannel.chan << ":"
              << FDChannel.mode << std::endl;
    abort();
  }

  fPredGens.push_back(
      std::make_unique<NoExtrapPredictionGenerator>(fAnalysisAxis, cut, wei));
  FDPrediction = std::make_unique<PredictionInterp>(systlist, &kNoOsc,
                                                    *fPredGens.back(), loaders);

  std::unique_ptr<PredictionInterp> &FDUnOscWeightedSigPrediction =
      GetFDUnOscWeightedSigPrediction(FDChannel);
  if (&FDUnOscWeightedSigPrediction == &kNoSuchFDPredictionSpectrum) {
    std::cout << "ERROR: Invalid FD MC type passed: " << FDChannel.chan << ":"
              << FDChannel.mode << std::endl;
    abort();
  }

  fPredGens.push_back(std::make_unique<FDNoOscPredictionGenerator>(
      fFluxMatcherCorrectionAxes, cut, wei));
  FDUnOscWeightedSigPrediction = std::make_unique<PredictionInterp>(
      systlist, &kNoOsc, *fPredGens.back(), loaders);
}

std::unique_ptr<ReweightableSpectrum> &
PredictionPRISM::GetNDData_right_sign_numu(BeamMode NDBM) const {
  if (NDBM == BeamMode::kNuMode) {
    return Measurements.ND.numu_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return Measurements.ND.numubar_ccinc_sel_nubmode;
  }
  return kNoSuchNDDataSpectrum;
}
std::unique_ptr<ReweightableSpectrum> &
PredictionPRISM::GetNDData_right_sign_nue(BeamMode NDBM) const {
  if (NDBM == BeamMode::kNuMode) {
    return Measurements.ND.nue_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return Measurements.ND.nuebar_ccinc_sel_nubmode;
  }
  return kNoSuchNDDataSpectrum;
}
std::unique_ptr<ReweightableSpectrum> &
PredictionPRISM::GetNDData_wrong_sign_numu(BeamMode NDBM) const {
  if (NDBM == BeamMode::kNuMode) {
    return Measurements.ND.numubar_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return Measurements.ND.numu_ccinc_sel_nubmode;
  }
  return kNoSuchNDDataSpectrum;
}
std::unique_ptr<ReweightableSpectrum> &
PredictionPRISM::GetNDData(PRISM::BeamChan NDChannel) const {
  if (NDChannel.chan &
      ((NDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuIntrinsic
                                             : NuChan::kNumuBarIntrinsic)) {
    return GetNDData_right_sign_numu(NDChannel.mode);
  } else if (NDChannel.chan & ((NDChannel.mode == BeamMode::kNuMode)
                                   ? NuChan::kNueIntrinsic
                                   : NuChan::kNueBarIntrinsic)) {
    return GetNDData_right_sign_nue(NDChannel.mode);
  } else if (NDChannel.chan &
             ((NDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuBarIntrinsic
                                                    : NuChan::kNumuIntrinsic)) {
    return GetNDData_wrong_sign_numu(NDChannel.mode);
  }
  PRISMOUT("Invalid ND Data request: " << NDChannel.mode << ", "
                                       << NDChannel.chan);
  return kNoSuchNDDataSpectrum;
}

bool PredictionPRISM::HaveNDData(PRISM::BeamChan NDChannel) const {
  return bool(GetNDData(NDChannel));
}

std::unique_ptr<ReweightableSpectrum> &
PredictionPRISM::GetNDData_280kA_right_sign_numu(BeamMode NDBM) const {
  if (NDBM == BeamMode::kNuMode) {
    return Measurements.ND.numu_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return Measurements.ND.numubar_ccinc_sel_nubmode;
  }
  return kNoSuchNDDataSpectrum;
}
std::unique_ptr<ReweightableSpectrum> &
PredictionPRISM::GetNDData_280kA_right_sign_nue(BeamMode NDBM) const {
  if (NDBM == BeamMode::kNuMode) {
    return Measurements.ND.nue_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return Measurements.ND.nuebar_ccinc_sel_nubmode;
  }
  return kNoSuchNDDataSpectrum;
}
std::unique_ptr<ReweightableSpectrum> &
PredictionPRISM::GetNDData_280kA_wrong_sign_numu(BeamMode NDBM) const {
  if (NDBM == BeamMode::kNuMode) {
    return Measurements.ND.numubar_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return Measurements.ND.numu_ccinc_sel_nubmode;
  }
  return kNoSuchNDDataSpectrum;
}
std::unique_ptr<ReweightableSpectrum> &
PredictionPRISM::GetNDData_280kA(PRISM::BeamChan NDChannel) const {
  if (NDChannel.chan &
      ((NDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuIntrinsic
                                             : NuChan::kNumuBarIntrinsic)) {
    return GetNDData_280kA_right_sign_numu(NDChannel.mode);
  } else if (NDChannel.chan & ((NDChannel.mode == BeamMode::kNuMode)
                                   ? NuChan::kNueIntrinsic
                                   : NuChan::kNueBarIntrinsic)) {
    return GetNDData_280kA_right_sign_nue(NDChannel.mode);
  } else if (NDChannel.chan &
             ((NDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuBarIntrinsic
                                                    : NuChan::kNumuIntrinsic)) {
    return GetNDData_280kA_wrong_sign_numu(NDChannel.mode);
  }
  PRISMOUT("Invalid ND Data request: " << NDChannel.mode << ", "
                                       << NDChannel.chan);
  return kNoSuchNDDataSpectrum;
}

bool PredictionPRISM::HaveNDData_280kA(PRISM::BeamChan NDChannel) const {
  return bool(GetNDData_280kA(NDChannel));
}

std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetNDPrediction_right_sign_numu(BeamMode NDBM) const {
  if (NDBM == BeamMode::kNuMode) {
    return Predictions.ND.numu_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return Predictions.ND.numubar_ccinc_sel_nubmode;
  }
  return kNoSuchNDPredictionSpectrum;
}
std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetNDPrediction_right_sign_nue(BeamMode NDBM) const {
  if (NDBM == BeamMode::kNuMode) {
    return Predictions.ND.nue_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return Predictions.ND.nuebar_ccinc_sel_nubmode;
  }
  return kNoSuchNDPredictionSpectrum;
}
std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetNDPrediction_wrong_sign_numu(BeamMode NDBM) const {
  if (NDBM == BeamMode::kNuMode) {
    return Predictions.ND.numubar_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return Predictions.ND.numu_ccinc_sel_nubmode;
  }
  return kNoSuchNDPredictionSpectrum;
}
std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetNDPrediction(PRISM::BeamChan NDChannel) const {
  if (NDChannel.chan &
      ((NDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuIntrinsic
                                             : NuChan::kNumuBarIntrinsic)) {
    return GetNDPrediction_right_sign_numu(NDChannel.mode);
  } else if (NDChannel.chan & ((NDChannel.mode == BeamMode::kNuMode)
                                   ? NuChan::kNueIntrinsic
                                   : NuChan::kNueBarIntrinsic)) {
    return GetNDPrediction_right_sign_nue(NDChannel.mode);
  } else if (NDChannel.chan &
             ((NDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuBarIntrinsic
                                                    : NuChan::kNumuIntrinsic)) {
    return GetNDPrediction_wrong_sign_numu(NDChannel.mode);
  }
  PRISMOUT("Invalid ND Prediction request: " << NDChannel.mode << ", "
                                             << NDChannel.chan);
  return kNoSuchNDPredictionSpectrum;
}

bool PredictionPRISM::HaveNDPrediction(PRISM::BeamChan NDChannel) const {
  return bool(GetNDPrediction(NDChannel));
}

std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetNDPrediction_280kA_right_sign_numu(BeamMode NDBM) const {
  if (NDBM == BeamMode::kNuMode) {
    return Predictions.ND.numu_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return Predictions.ND.numubar_ccinc_sel_nubmode;
  }
  return kNoSuchNDPredictionSpectrum;
}
std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetNDPrediction_280kA_right_sign_nue(BeamMode NDBM) const {
  if (NDBM == BeamMode::kNuMode) {
    return Predictions.ND.nue_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return Predictions.ND.nuebar_ccinc_sel_nubmode;
  }
  return kNoSuchNDPredictionSpectrum;
}
std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetNDPrediction_280kA_wrong_sign_numu(BeamMode NDBM) const {
  if (NDBM == BeamMode::kNuMode) {
    return Predictions.ND.numubar_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return Predictions.ND.numu_ccinc_sel_nubmode;
  }
  return kNoSuchNDPredictionSpectrum;
}
std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetNDPrediction_280kA(PRISM::BeamChan NDChannel) const {
  if (NDChannel.chan &
      ((NDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuIntrinsic
                                             : NuChan::kNumuBarIntrinsic)) {
    return GetNDPrediction_280kA_right_sign_numu(NDChannel.mode);
  } else if (NDChannel.chan & ((NDChannel.mode == BeamMode::kNuMode)
                                   ? NuChan::kNueIntrinsic
                                   : NuChan::kNueBarIntrinsic)) {
    return GetNDPrediction_280kA_right_sign_nue(NDChannel.mode);
  } else if (NDChannel.chan &
             ((NDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuBarIntrinsic
                                                    : NuChan::kNumuIntrinsic)) {
    return GetNDPrediction_280kA_wrong_sign_numu(NDChannel.mode);
  }
  PRISMOUT("Invalid ND Prediction request: " << NDChannel.mode << ", "
                                             << NDChannel.chan);
  return kNoSuchNDPredictionSpectrum;
}

bool PredictionPRISM::HaveNDPrediction_280kA(PRISM::BeamChan NDChannel) const {
  return bool(GetNDPrediction_280kA(NDChannel));
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

//----------------------------------------------------------------------
Spectrum PredictionPRISM::Predict(osc::IOscCalculator *calc) const {
  return PredictSyst(calc, kNoShift);
}

//----------------------------------------------------------------------
Spectrum PredictionPRISM::PredictSyst(osc::IOscCalculator *calc,
                                      SystShifts shift) const {
  std::map<PredictionPRISM::PRISMComponent, Spectrum> Comps =
      PredictPRISMComponents(calc, shift);

  assert(Comps.size());

  return Comps.at(kPRISMPred);
}

std::map<PredictionPRISM::PRISMComponent, Spectrum>
PredictionPRISM::PredictPRISMComponents(osc::IOscCalculator *calc,
                                        SystShifts shift,
                                        MatchChan match_chan) const {

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

  auto &NDData = GetNDData(match_chan.from);
  auto &NDPrediction = GetNDPrediction(match_chan.from);
  auto &FDPrediction = GetFDPrediction(match_chan.to);
  auto &FDUnOscWeightedSigPrediction =
      GetFDUnOscWeightedSigPrediction(match_chan.to);

  PRISMOUT("Making PRISM prediction for: \n\t("
           << match_chan.from.mode << ":" << match_chan.from.chan << ") -> ("
           << match_chan.to.mode << ":" << match_chan.to.chan << ")");

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
                         : Flavors::kNuMuToNuE;
  auto FDSigSign = ((match_chan.to.chan & NuChan::kNumu) ||
                    (match_chan.to.chan & NuChan::kNue))
                       ? Sign::kNu
                       : Sign::kAntiNu;

  auto FDWrongSign = (FDSigSign == Sign::kNu) ? Sign::kAntiNu : Sign::kNu;
  auto FDWrongFlavor = (FDSigFlavor == Flavors::kNuMuToNuMu)
                           ? Flavors::kAllNuE
                           : Flavors::kAllNuMu;
  auto FDIntrinsicFlavor = (FDSigFlavor == Flavors::kNuMuToNuMu)
                               ? Flavors::kNuEToNuMu
                               : Flavors::kNuEToNuE;

  PRISMOUT("\n\tNDSigFlavor: "
           << NDSigFlavor << "\n\tNDSigSign: " << NDSigSign
           << "\n\tNDWrongSign: " << NDWrongSign << "\n\tNDWrongFlavor: "
           << NDWrongFlavor << "\n\tFDSigFlavor: " << FDSigFlavor
           << "\n\tFDSigSign: " << FDSigSign << "\n\tFDWrongSign: "
           << FDWrongSign << "\n\tFDWrongFlavor: " << FDWrongFlavor
           << "\n\tFDIntrinsicFlavor: " << FDIntrinsicFlavor);

  // Using maps for non-default constructible classes is awful...
  std::map<PredictionPRISM::PRISMComponent, ReweightableSpectrum> NDComps;
  std::map<PredictionPRISM::PRISMComponent, Spectrum> Comps;

  RunPlan const &NDRunPlan = (match_chan.from.mode == PRISM::BeamMode::kNuMode)
                                 ? RunPlan_nu
                                 : RunPlan_nub;

  double NDPOT = NDRunPlan.GetPlanPOT();
  assert(NDPOT > 0);

  NDComps.emplace(kNDData, NDRunPlan.Weight(*NDData, fSetNDErrorsFromRate));
  NDComps.emplace(kNDDataCorr2D, NDComps.at(kNDData));

  // Start building MC components
  Spectrum NDSig_spec = NDRunPlan.Weight(NDPrediction->PredictComponentSyst(
      calc, shift, NDSigFlavor, Current::kCC, NDSigSign));

  std::unique_ptr<TH2> NDSig_h(NDSig_spec.ToTH2(NDPOT));
  NDSig_h->SetDirectory(nullptr);
  ReweightableSpectrum NDSig(ana::Constant(1), NDSig_h.get(),
                             fAnalysisAxis.GetLabels(),
                             fAnalysisAxis.GetBinnings(), NDPOT, 0);

  NDComps.emplace(kNDSig, NDSig);
  NDComps.emplace(kNDSig2D, NDSig);

  // ND Background subtraction
  if (fNCCorrection) {
    std::unique_ptr<TH2> NC_h(
        NDRunPlan
            .Weight(NDPrediction->PredictComponentSyst(
                calc, shift, Flavors::kAll, Current::kNC, Sign::kBoth))
            .ToTH2(NDPOT));
    NC_h->SetDirectory(nullptr);

    ReweightableSpectrum NC(ana::Constant(1), NC_h.get(),
                            fAnalysisAxis.GetLabels(),
                            fAnalysisAxis.GetBinnings(), NDPOT, 0);

    NDComps.emplace(kNDNCBkg, NC);
    NDComps.at(kNDDataCorr2D) -= NDComps.at(kNDNCBkg);
  }

  if (fWLBCorrection) {
    std::unique_ptr<TH2> WLB_h(
        NDRunPlan
            .Weight(NDPrediction->PredictComponentSyst(
                calc, shift, NDWrongFlavor, Current::kCC, Sign::kBoth))
            .ToTH2(NDPOT));
    WLB_h->SetDirectory(nullptr);
    ReweightableSpectrum Nue(ana::Constant(1), WLB_h.get(),
                             fAnalysisAxis.GetLabels(),
                             fAnalysisAxis.GetBinnings(), NDPOT, 0);

    NDComps.emplace(kNDWrongLepBkg, Nue);
    NDComps.at(kNDDataCorr2D) -= NDComps.at(kNDWrongLepBkg);
  }

  if (fWSBCorrection) {
    std::unique_ptr<TH2> WSB_h(
        NDRunPlan
            .Weight(NDPrediction->PredictComponentSyst(
                calc, shift, NDSigFlavor, Current::kCC, NDWrongSign))
            .ToTH2(NDPOT));
    WSB_h->SetDirectory(nullptr);

    ReweightableSpectrum WSB(ana::Constant(1), WSB_h.get(),
                             fAnalysisAxis.GetLabels(),
                             fAnalysisAxis.GetBinnings(), NDPOT, 0);

    NDComps.emplace(kNDWSBkg, WSB);
    NDComps.at(kNDDataCorr2D) -= NDComps.at(kNDWSBkg);
  }

  static osc::NoOscillations no;

  Spectrum FDUnWeightedSig_Spec =
      FDUnOscWeightedSigPrediction->PredictComponentSyst(
          &no, shift, FDSigFlavor, Current::kCC, FDSigSign);

  std::unique_ptr<TH2> FDUnOscWeightedSig_h(FDUnWeightedSig_Spec.ToTH2(NDPOT));
  std::unique_ptr<TH1> FDUnOscWeightedSig_TrueEnergy_h(
      FDUnOscWeightedSig_h->ProjectionY());

  FDUnOscWeightedSig_h->SetDirectory(nullptr);
  FDUnOscWeightedSig_TrueEnergy_h->SetDirectory(nullptr);

  // Linear Combination
  TH1 const *LinearCombination = fFluxMatcher->GetFarMatchCoefficients(
      calc, FDUnOscWeightedSig_TrueEnergy_h.get(), fMaxOffAxis, match_chan,
      shift);

  std::unique_ptr<TH1> UnRunPlannedLinearCombination =
      std::unique_ptr<TH1>(NDRunPlan.Unweight(LinearCombination));
  // We don't want the total POT of the runplan to affect the scale of the
  // coefficients, just the shape.
  UnRunPlannedLinearCombination->Scale(NDPOT);

  Spectrum UnRunPlannedLinearCombination_s(fNDOffAxis.GetLabels(),
                                           fNDOffAxis.GetBinnings());
  UnRunPlannedLinearCombination_s.FillFromHistogram(
      UnRunPlannedLinearCombination.get());
  UnRunPlannedLinearCombination_s.OverridePOT(NDPOT);
  UnRunPlannedLinearCombination_s.OverrideLivetime(0);
  Comps.emplace(kNDFDWeightings, UnRunPlannedLinearCombination_s);

  // for (auto &NDC : NDComps) {
  //   NDC.second.OverridePOT(fDefaultOffAxisPOT);
  // }

  if (NDComps.count(kNDSig)) {
    Comps.emplace(kNDSig, NDComps.at(kNDSig).WeightedByErrors(
                              UnRunPlannedLinearCombination.get()));
    Comps.emplace(kPRISMMC, Comps.at(kNDSig));
  }

  Comps.emplace(kNDDataCorr,
                NDComps.at(kNDDataCorr2D)
                    .WeightedByErrors(UnRunPlannedLinearCombination.get()));

  Comps.emplace(kPRISMPred, Comps.at(kNDDataCorr));

  // If we have the FD background predictions add them back in

  if (fNCCorrection) {
    Comps.emplace(kFDNCBkg,
                  FDPrediction->PredictComponentSyst(
                      calc, shift, Flavors::kAll, Current::kNC, Sign::kBoth));
    Comps.at(kPRISMPred) += Comps.at(kFDNCBkg);
    Comps.at(kPRISMMC) += Comps.at(kFDNCBkg);
  }

  if (fWLBCorrection) {
    Comps.emplace(kFDWrongLepBkg,
                  FDPrediction->PredictComponentSyst(
                      calc, shift, FDWrongFlavor, Current::kCC, Sign::kBoth));
    Comps.at(kPRISMPred) += Comps.at(kFDWrongLepBkg);
    Comps.at(kPRISMMC) += Comps.at(kFDWrongLepBkg);
  }

  if (fWSBCorrection) {
    Comps.emplace(kFDWSBkg,
                  FDPrediction->PredictComponentSyst(
                      calc, shift, FDSigFlavor, Current::kCC, FDWrongSign));
    Comps.at(kPRISMPred) += Comps.at(kFDWSBkg);
    Comps.at(kPRISMMC) += Comps.at(kFDWSBkg);
  }

  if (fIntrinsicCorrection) {
    Comps.emplace(kFDIntrinsicBkg,
                  FDPrediction->PredictComponentSyst(
                      calc, shift, FDIntrinsicFlavor, Current::kCC, FDSigSign));
    Comps.at(kPRISMPred) += Comps.at(kFDIntrinsicBkg);
    Comps.at(kPRISMMC) += Comps.at(kFDIntrinsicBkg);
  }

  Comps.emplace(kFDOscPred,
                FDPrediction->PredictComponentSyst(calc, shift, Flavors::kAll,
                                                   Current::kCC, Sign::kBoth));

  // this is given as a ratio to no weighting (no osc for numu disp, and usese
  // numu no-disp spectrum for nue) to stop explosions at maximal mixing
  ReweightableSpectrum FDUnOscWeightedSig(
      ana::Constant(1), FDUnOscWeightedSig_h.get(), fAnalysisAxis.GetLabels(),
      fAnalysisAxis.GetBinnings(), NDPOT, 0);

  Comps.emplace(kFDUnOscPred, FDUnOscWeightedSig.UnWeighted());

  std::unique_ptr<TH1> resid(static_cast<TH1 *>(
      fFluxMatcher->GetLastResidual()->Clone("weighted_residual")));
  resid->SetDirectory(nullptr);
  resid->Scale(NDPOT);

  Comps.emplace(kFDFluxCorr, FDUnOscWeightedSig.WeightedByErrors(resid.get()));

  std::cout << Comps.at(kPRISMPred).ToTH1(NDPOT)->GetMaximum() << ", "
            << Comps.at(kFDFluxCorr).ToTH1(NDPOT)->GetMaximum() << std::endl;

  Comps.at(kPRISMPred) += Comps.at(kFDFluxCorr);
  if (NDComps.count(kPRISMMC)) {
    Comps.at(kPRISMMC) += Comps.at(kFDFluxCorr);
  }

  for (auto const &NDC :
       NDComps) { // If you haven't been added, project to a 2D spectrum
    if (!Comps.count(NDC.first)) {
      Comps.emplace(NDC.first, NDC.second.ToSpectrum());
    }
  }
  return Comps;
}

std::map<PredictionPRISM::PRISMComponent, Spectrum>
PredictionPRISM::PredictGaussianFlux(double mean, double width,
                                     ana::SystShifts shift,
                                     BeamChan NDChannel) const {

  bool WeHaveNDData = HaveNDData(NDChannel);
  bool WeHaveNDPrediction = HaveNDPrediction(NDChannel);

  if (!WeHaveNDData || !WeHaveNDPrediction) {
    std::cout
        << "[ERROR]: Cannot make a PRISM prediction has we have: HaveNDData: "
        << (WeHaveNDData ? "have" : "do not have") << ", WeHaveNDPrediction: "
        << (WeHaveNDPrediction ? "have" : "do not have") << std::endl;
    abort();
  }

  DontAddDirectory guard;

  auto &NDData = GetNDData(NDChannel);
  auto &NDPrediction = GetNDPrediction(NDChannel);

  // Sort out the flavors and signs
  auto NDSigFlavor = (NDChannel.chan & NuChan::kNumuNumuBar) ? Flavors::kAllNuMu
                                                             : Flavors::kAllNuE;
  auto NDSigSign =
      ((NDChannel.chan & NuChan::kNumu) || (NDChannel.chan & NuChan::kNue))
          ? Sign::kNu
          : Sign::kAntiNu;
  auto NDWrongSign = (NDSigSign == Sign::kNu) ? Sign::kAntiNu : Sign::kNu;
  auto NDWrongFlavor =
      (NDSigFlavor == Flavors::kAllNuMu) ? Flavors::kAllNuE : Flavors::kAllNuMu;

  // Using maps for non-default constructible classes is awful...
  std::map<PredictionPRISM::PRISMComponent, ReweightableSpectrum> NDComps;
  std::map<PredictionPRISM::PRISMComponent, Spectrum> Comps;
  NDComps.emplace(kNDData, *NDData);
  NDComps.emplace(kNDDataCorr2D, NDComps.at(kNDData));

  double NDPOT = NDData->POT();

  static osc::NoOscillations no;

  // Start building MC components
  Spectrum NDSig_spec = NDPrediction->PredictComponentSyst(
      &no, shift, NDSigFlavor, Current::kCC, NDSigSign);

  std::unique_ptr<TH2> NDSig_h(NDSig_spec.ToTH2(NDPOT));

  ReweightableSpectrum NDSig(ana::Constant(1), NDSig_h.get(),
                             fAnalysisAxis.GetLabels(),
                             fAnalysisAxis.GetBinnings(), NDPOT, 0);

  NDComps.emplace(kNDSig2D, NDSig);

  // ND Background subtraction
  if (fNCCorrection) {
    std::unique_ptr<TH2> NC_h(
        NDPrediction
            ->PredictComponentSyst(&no, shift, Flavors::kAll, Current::kNC,
                                   Sign::kBoth)
            .ToTH2(NDPOT));
    ReweightableSpectrum NC(ana::Constant(1), NC_h.get(),
                            fAnalysisAxis.GetLabels(),
                            fAnalysisAxis.GetBinnings(), NDPOT, 0);

    NDComps.emplace(kNDNCBkg, NC);
    NDComps.at(kNDDataCorr2D) -= NDComps.at(kNDNCBkg);
  }

  if (fWLBCorrection) {
    std::unique_ptr<TH2> WLB_h(
        NDPrediction
            ->PredictComponentSyst(&no, shift, NDWrongFlavor, Current::kCC,
                                   Sign::kBoth)
            .ToTH2(NDPOT));
    ReweightableSpectrum Nue(ana::Constant(1), WLB_h.get(),
                             fAnalysisAxis.GetLabels(),
                             fAnalysisAxis.GetBinnings(), NDPOT, 0);

    NDComps.emplace(kNDWrongLepBkg, Nue);
    NDComps.at(kNDDataCorr2D) -= NDComps.at(kNDWrongLepBkg);
  }

  if (fWSBCorrection) {
    std::unique_ptr<TH2> WSB_h(
        NDPrediction
            ->PredictComponentSyst(&no, shift, NDSigFlavor, Current::kCC,
                                   NDWrongSign)
            .ToTH2(NDPOT));
    ReweightableSpectrum WSB(ana::Constant(1), WSB_h.get(),
                             fAnalysisAxis.GetLabels(),
                             fAnalysisAxis.GetBinnings(), NDPOT, 0);

    NDComps.emplace(kNDWSBkg, WSB);
    NDComps.at(kNDDataCorr2D) -= NDComps.at(kNDWSBkg);
  }

  // Linear Combination
  TH1 const *LinearCombination = fFluxMatcher->GetGaussianCoefficients(
      mean, width, fMaxOffAxis, NDChannel, shift);

  for (auto &NDC : NDComps) {
    NDC.second.OverridePOT(fDefaultOffAxisPOT);
  }

  Comps.emplace(kNDDataCorr,
                NDComps.at(kNDDataCorr2D).WeightedByErrors(LinearCombination));
  Comps.emplace(kPRISMPred, Comps.at(kNDDataCorr));

  Comps.emplace(kNDSig,
                NDComps.at(kNDSig2D).WeightedByErrors(LinearCombination));

  for (auto const &NDC :
       NDComps) { // If you haven't been added, project to a 2D spectrum
    if (!Comps.count(NDC.first)) {
      Comps.emplace(NDC.first, NDC.second.ToSpectrum());
    }
  }
  return Comps;
}

//----------------------------------------------------------------------
Spectrum PredictionPRISM::PredictComponent(osc::IOscCalculator *calc,
                                           Flavors::Flavors_t flav,
                                           Current::Current_t curr,
                                           Sign::Sign_t sign) const {

  throw;
}

// I know... but makes the SaveTo/LoadFrom a lot neater
using NamedReweightableSpectrumRefVect = std::vector<
    std::pair<std::string,
              std::reference_wrapper<std::unique_ptr<ReweightableSpectrum>>>>;
using NamedPredInterpRefVect = std::vector<std::pair<
    std::string, std::reference_wrapper<std::unique_ptr<PredictionInterp>>>>;
using NamedReweightableSpectrumCRefVect = std::vector<std::pair<
    std::string,
    std::reference_wrapper<std::unique_ptr<ReweightableSpectrum> const>>>;
using NamedPredInterpCRefVect = std::vector<
    std::pair<std::string,
              std::reference_wrapper<std::unique_ptr<PredictionInterp> const>>>;

//----------------------------------------------------------------------
void PredictionPRISM::SaveTo(TDirectory *dir) const {
  TDirectory *tmp = gDirectory;

  dir->cd();

  TObjString("PredictionPRISM").Write("type");

  for (auto &meas : NamedReweightableSpectrumCRefVect{
           {"M_ND_numu_ccinc_sel_numode",
            Measurements.ND.numu_ccinc_sel_numode},
           {"M_ND_numubar_ccinc_sel_numode",
            Measurements.ND.numubar_ccinc_sel_numode},
           {"M_ND_numu_ccinc_sel_nubmode",
            Measurements.ND.numu_ccinc_sel_nubmode},
           {"M_ND_numubar_ccinc_sel_nubmode",
            Measurements.ND.numubar_ccinc_sel_nubmode},
           {"M_ND_nue_ccinc_sel_numode", Measurements.ND.nue_ccinc_sel_numode},
           {"M_ND_nuebar_ccinc_sel_nubmode",
            Measurements.ND.nuebar_ccinc_sel_nubmode},
           {"M_ND_280kA_numu_ccinc_sel_numode",
            Measurements.ND_280kA.numu_ccinc_sel_numode},
           {"M_ND_280kA_numubar_ccinc_sel_numode",
            Measurements.ND_280kA.numubar_ccinc_sel_numode},
           {"M_ND_280kA_numu_ccinc_sel_nubmode",
            Measurements.ND_280kA.numu_ccinc_sel_nubmode},
           {"M_ND_280kA_numubar_ccinc_sel_nubmode",
            Measurements.ND_280kA.numubar_ccinc_sel_nubmode},
           {"M_ND_280kA_nue_ccinc_sel_numode",
            Measurements.ND_280kA.nue_ccinc_sel_numode},
           {"M_ND_280kA_nuebar_ccinc_sel_nubmode",
            Measurements.ND_280kA.nuebar_ccinc_sel_nubmode}}) {
    if (meas.second.get()) {
      meas.second.get()->OverridePOT(1);
      meas.second.get()->SaveTo(dir->mkdir(meas.first.c_str()));
    }
  }

  for (auto &meas : NamedPredInterpCRefVect{
           {"P_ND_numu_ccinc_sel_numode", Predictions.ND.numu_ccinc_sel_numode},
           {"P_ND_numubar_ccinc_sel_numode",
            Predictions.ND.numubar_ccinc_sel_numode},
           {"P_ND_numu_ccinc_sel_nubmode",
            Predictions.ND.numu_ccinc_sel_nubmode},
           {"P_ND_numubar_ccinc_sel_nubmode",
            Predictions.ND.numubar_ccinc_sel_nubmode},
           {"P_ND_nue_ccinc_sel_numode", Predictions.ND.nue_ccinc_sel_numode},
           {"P_ND_nuebar_ccinc_sel_nubmode",
            Predictions.ND.nuebar_ccinc_sel_nubmode},
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
      meas.second.get()->GetPredNomAs<PredictionNoOsc>()->OverridePOT(1);
      meas.second.get()->SaveTo(dir->mkdir(meas.first.c_str()));
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
      meas.second.get()->SaveTo(dir->mkdir(meas.first.c_str()));
    }
  }

  for (auto &meas :
       NamedPredInterpCRefVect{{"P_FD_numu_ccinc_sel_sig_numode",
                                Predictions.FD.numu_ccinc_sel_sig_numode},
                               {"P_FD_nue_ccinc_sel_sig_numode",
                                Predictions.FD.nue_ccinc_sel_sig_numode},
                               {"P_FD_numubar_ccinc_sel_sig_nubmode",
                                Predictions.FD.numubar_ccinc_sel_sig_nubmode},
                               {"P_FD_nuebar_ccinc_sel_sig_nubmode",
                                Predictions.FD.nuebar_ccinc_sel_sig_nubmode}}) {
    if (meas.second.get()) {
      meas.second.get()->SaveTo(dir->mkdir(meas.first.c_str()));
    }
  }

  for (unsigned int i = 0; i < fAnalysisAxis.GetBinnings().size(); ++i) {
    TObjString(fAnalysisAxis.GetLabels()[i].c_str())
        .Write(TString::Format("pred_label%d", i).Data());
    fAnalysisAxis.GetBinnings()[i].SaveTo(
        dir->mkdir(TString::Format("pred_bins%d", i)));
  }

  for (unsigned int i = 0; i < fNDOffAxis.GetBinnings().size(); ++i) {
    TObjString(fNDOffAxis.GetLabels()[i].c_str())
        .Write(TString::Format("offaxis_label%d", i).Data());
    fNDOffAxis.GetBinnings()[i].SaveTo(
        dir->mkdir(TString::Format("offaxis_bins%d", i)));
  }

  for (unsigned int i = 0; i < fND280kAAxis.GetBinnings().size(); ++i) {
    TObjString(fND280kAAxis.GetLabels()[i].c_str())
        .Write(TString::Format("280kAaxis_label%d", i).Data());
    fND280kAAxis.GetBinnings()[i].SaveTo(
        dir->mkdir(TString::Format("280kAaxis_bins%d", i)));
  }

  for (unsigned int i = 0; i < fNDFDEnergyMatchAxis.GetBinnings().size(); ++i) {
    TObjString(fNDFDEnergyMatchAxis.GetLabels()[i].c_str())
        .Write(TString::Format("ematch_label%d", i).Data());
    fNDFDEnergyMatchAxis.GetBinnings()[i].SaveTo(
        dir->mkdir(TString::Format("ematch_bins%d", i)));
  }

  tmp->cd();
}

//----------------------------------------------------------------------
std::unique_ptr<PredictionPRISM> PredictionPRISM::LoadFrom(TDirectory *dir) {

  std::vector<std::string> pred_labels;
  std::vector<Binning> pred_bins;
  std::vector<Var> pred_dummy_vars;

  for (int i = 0;; ++i) {
    TDirectory *subdir = dir->GetDirectory(TString::Format("pred_bins%d", i));
    if (!subdir) {
      break;
    }
    pred_bins.push_back(*Binning::LoadFrom(subdir));
    TObjString *label =
        (TObjString *)dir->Get(TString::Format("pred_label%d", i));
    pred_labels.push_back(label ? label->GetString().Data() : "");
    pred_dummy_vars.push_back(kUnweighted);
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
    offaxis_bins.push_back(*Binning::LoadFrom(subdir));
    TObjString *label =
        (TObjString *)dir->Get(TString::Format("offaxis_label%d", i));
    offaxis_labels.push_back(label ? label->GetString().Data() : "");
    offaxis_dummy_vars.push_back(kUnweighted);
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
    _280kAaxis_bins.push_back(*Binning::LoadFrom(subdir));
    TObjString *label =
        (TObjString *)dir->Get(TString::Format("280kAaxis_label%d", i));
    _280kAaxis_labels.push_back(label ? label->GetString().Data() : "");
    _280kAaxis_dummy_vars.push_back(kUnweighted);
  }

  std::vector<std::string> ematch_labels;
  std::vector<Binning> ematch_bins;
  std::vector<Var> ematch_dummy_vars;

  for (int i = 0;; ++i) {
    TDirectory *subdir = dir->GetDirectory(TString::Format("ematch_bins%d", i));
    if (!subdir) {
      break;
    }
    ematch_bins.push_back(*Binning::LoadFrom(subdir));
    TObjString *label =
        (TObjString *)dir->Get(TString::Format("ematch_label%d", i));
    ematch_labels.push_back(label ? label->GetString().Data() : "");
    ematch_dummy_vars.push_back(kUnweighted);
  }

  HistAxis const predictionAxis(pred_labels, pred_bins, pred_dummy_vars);
  HistAxis const offAxis(offaxis_labels, offaxis_bins, offaxis_dummy_vars);
  HistAxis const _280kAAxis(offaxis_labels, offaxis_bins, offaxis_dummy_vars);
  HistAxis const energyMatchAxis(ematch_labels, ematch_bins, ematch_dummy_vars);

  std::unique_ptr<PredictionPRISM> pred = std::make_unique<PredictionPRISM>(
      predictionAxis, offAxis, _280kAAxis, energyMatchAxis);

  for (auto &meas : NamedReweightableSpectrumRefVect{
           {"M_ND_numu_ccinc_sel_numode",
            pred->Measurements.ND.numu_ccinc_sel_numode},
           {"M_ND_numubar_ccinc_sel_numode",
            pred->Measurements.ND.numubar_ccinc_sel_numode},
           {"M_ND_numu_ccinc_sel_nubmode",
            pred->Measurements.ND.numu_ccinc_sel_nubmode},
           {"M_ND_numubar_ccinc_sel_nubmode",
            pred->Measurements.ND.numubar_ccinc_sel_nubmode},
           {"M_ND_nue_ccinc_sel_numode",
            pred->Measurements.ND.nue_ccinc_sel_numode},
           {"M_ND_nuebar_ccinc_sel_nubmode",
            pred->Measurements.ND.nuebar_ccinc_sel_nubmode},
           {"M_ND_280kA_numu_ccinc_sel_numode",
            pred->Measurements.ND_280kA.numu_ccinc_sel_numode},
           {"M_ND_280kA_numubar_ccinc_sel_numode",
            pred->Measurements.ND_280kA.numubar_ccinc_sel_numode},
           {"M_ND_280kA_numu_ccinc_sel_nubmode",
            pred->Measurements.ND_280kA.numu_ccinc_sel_nubmode},
           {"M_ND_280kA_numubar_ccinc_sel_nubmode",
            pred->Measurements.ND_280kA.numubar_ccinc_sel_nubmode},
           {"M_ND_280kA_nue_ccinc_sel_numode",
            pred->Measurements.ND_280kA.nue_ccinc_sel_numode},
           {"M_ND_280kA_nuebar_ccinc_sel_nubmode",
            pred->Measurements.ND_280kA.nuebar_ccinc_sel_nubmode}}) {
    if (dir->GetDirectory(meas.first.c_str())) {
      meas.second.get() =
          ReweightableSpectrum::LoadFrom(dir->GetDirectory(meas.first.c_str()));
      meas.second.get()->OverridePOT(1);
    }
  }

  for (auto &meas : NamedPredInterpRefVect{
           {"P_ND_numu_ccinc_sel_numode",
            pred->Predictions.ND.numu_ccinc_sel_numode},
           {"P_ND_numubar_ccinc_sel_numode",
            pred->Predictions.ND.numubar_ccinc_sel_numode},
           {"P_ND_numu_ccinc_sel_nubmode",
            pred->Predictions.ND.numu_ccinc_sel_nubmode},
           {"P_ND_numubar_ccinc_sel_nubmode",
            pred->Predictions.ND.numubar_ccinc_sel_nubmode},
           {"P_ND_nue_ccinc_sel_numode",
            pred->Predictions.ND.nue_ccinc_sel_numode},
           {"P_ND_nuebar_ccinc_sel_nubmode",
            pred->Predictions.ND.nuebar_ccinc_sel_nubmode},
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
      meas.second.get() =
          PredictionInterp::LoadFrom(dir->GetDirectory(meas.first.c_str()));
      meas.second.get()->GetPredNomAs<PredictionNoOsc>()->OverridePOT(1);
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
      meas.second.get() =
          PredictionInterp::LoadFrom(dir->GetDirectory(meas.first.c_str()));
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
            pred->Predictions.FD.nuebar_ccinc_sel_sig_nubmode}}) {
    if (dir->GetDirectory(meas.first.c_str())) {
      meas.second.get() =
          PredictionInterp::LoadFrom(dir->GetDirectory(meas.first.c_str()));
    }
  }

  return pred;
}

} // namespace ana
