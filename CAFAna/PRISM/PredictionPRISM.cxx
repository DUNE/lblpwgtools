#include "CAFAna/PRISM/PredictionPRISM.h"
#include "CAFAna/PRISM/PRISMUtils.h"

#include "CAFAna/Prediction/PredictionsForPRISM.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/OscCurve.h"
#include "CAFAna/Core/Spectrum.h"

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

  fNDFD_Matrix = nullptr;

  fNCCorrection = false;
  fWSBCorrection = false;
  fWLBCorrection = false;
  fIntrinsicCorrection = false;

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

  double xslice_width_cm = (offAxisBinEdges[1] - offAxisBinEdges[0]) * 1E2;
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

  // Corrects for non-uniform off-axis binning
  auto slice_width_weight =
      NDSliceCorrection(50, fNDOffAxis.GetBinnings().front().Edges());
  auto slice_width_weight_280kA =
      NDSliceCorrection(50, fND280kAAxis.GetBinnings().front().Edges()); 

  NDData = std::make_unique<ReweightableSpectrum>(
      ND_loader, fAnalysisAxis, fNDOffAxis, cut && kCut280kARun, shift,
      wei * slice_width_weight); 

  std::unique_ptr<ReweightableSpectrum> &NDData_280kA =
      GetNDData(NDChannel, 280);
  if (&NDData_280kA == &kNoSuchNDDataSpectrum) {
    std::cout << "ERROR: Invalid ND data type passed: " << NDChannel.chan << ":"
              << NDChannel.mode << std::endl;
    abort();
  }
  NDData_280kA = std::make_unique<ReweightableSpectrum>(
      ND_loader, fAnalysisAxis, fND280kAAxis, cut && kSel280kARun, shift,
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
                                    const Var &wei,
                                    std::vector<ana::ISyst const *> systlist,
                                    PRISM::BeamChan NDChannel) {

  // Corrects for non-uniform off-axis binning
  auto slice_width_weight =
      NDSliceCorrection(50, fNDOffAxis.GetBinnings().front().Edges());
  auto slice_width_weight_280kA =
      NDSliceCorrection(50, fND280kAAxis.GetBinnings().front().Edges()); // CHANGE 50 to 200

  std::unique_ptr<PredictionInterp> &NDPrediction = GetNDPrediction(NDChannel);
  if (&NDPrediction == &kNoSuchNDPredictionSpectrum) {
    std::cout << "ERROR: Invalid ND MC type passed: " << NDChannel.chan << ":"
              << NDChannel.mode << std::endl;
    abort();
  }

  osc::NoOscillations kNoOsc;

  fPredGens.push_back(std::make_unique<NoOscPredictionGenerator>(
      fOffPredictionAxis, cut && kCut280kARun, wei * slice_width_weight));

  NDPrediction = std::make_unique<PredictionInterp>(systlist, &kNoOsc,
                                                    *fPredGens.back(), loaders);

  std::unique_ptr<PredictionInterp> &NDPrediction_280kA =
      GetNDPrediction(NDChannel, 280);
  if (&NDPrediction_280kA == &kNoSuchNDPredictionSpectrum) {
    std::cout << "ERROR: Invalid ND MC type passed: " << NDChannel.chan << ":"
              << NDChannel.mode << std::endl;
    abort();
  }

  fPredGens.push_back(std::make_unique<NoOscPredictionGenerator>(
      f280kAPredictionAxis, cut && kSel280kARun,
      wei * slice_width_weight_280kA)); // kSpecHCRunWeight

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

  std::unique_ptr<PredictionInterp> &FDNonSwapAppOscPrediction =
      GetFDNonSwapAppOscPrediction(FDChannel.mode);

  fPredGens.push_back(std::make_unique<NonSwapNoExtrapPredictionGenerator>(
      fFluxMatcherCorrectionAxes, cut, wei));
  FDNonSwapAppOscPrediction = std::make_unique<PredictionInterp>(
      systlist, &kNoOsc, *fPredGens.back(), loaders);
}

std::unique_ptr<ReweightableSpectrum> &
PredictionPRISM::GetNDData_right_sign_numu(BeamMode NDBM, int kA) const {
  if (NDBM == BeamMode::kNuMode) {
    return (kA == 293) ? Measurements.ND_293kA.numu_ccinc_sel_numode
                       : Measurements.ND_280kA.numu_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return (kA == 293) ? Measurements.ND_293kA.numubar_ccinc_sel_nubmode
                       : Measurements.ND_280kA.numubar_ccinc_sel_nubmode;
  }
  return kNoSuchNDDataSpectrum;
}
std::unique_ptr<ReweightableSpectrum> &
PredictionPRISM::GetNDData_right_sign_nue(BeamMode NDBM, int kA) const {
  if (NDBM == BeamMode::kNuMode) {
    return (kA == 293) ? Measurements.ND_293kA.nue_ccinc_sel_numode
                       : Measurements.ND_280kA.nue_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return (kA == 293) ? Measurements.ND_293kA.nuebar_ccinc_sel_nubmode
                       : Measurements.ND_280kA.nuebar_ccinc_sel_nubmode;
  }
  return kNoSuchNDDataSpectrum;
}
std::unique_ptr<ReweightableSpectrum> &
PredictionPRISM::GetNDData_wrong_sign_numu(BeamMode NDBM, int kA) const {
  if (NDBM == BeamMode::kNuMode) {
    return (kA == 293) ? Measurements.ND_293kA.numubar_ccinc_sel_numode
                       : Measurements.ND_280kA.numubar_ccinc_sel_numode;
  } else if (NDBM == BeamMode::kNuBarMode) {
    return (kA == 293) ? Measurements.ND_293kA.numu_ccinc_sel_nubmode
                       : Measurements.ND_280kA.numu_ccinc_sel_nubmode;
  }
  return kNoSuchNDDataSpectrum;
}
std::unique_ptr<ReweightableSpectrum> &
PredictionPRISM::GetNDData(PRISM::BeamChan NDChannel, int kA) const {
  if (NDChannel.chan &
      ((NDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuIntrinsic
                                             : NuChan::kNumuBarIntrinsic)) {
    return GetNDData_right_sign_numu(NDChannel.mode, kA);
  } else if (NDChannel.chan & ((NDChannel.mode == BeamMode::kNuMode)
                                   ? NuChan::kNueIntrinsic
                                   : NuChan::kNueBarIntrinsic)) {
    return GetNDData_right_sign_nue(NDChannel.mode, kA);
  } else if (NDChannel.chan &
             ((NDChannel.mode == BeamMode::kNuMode) ? NuChan::kNumuBarIntrinsic
                                                    : NuChan::kNumuIntrinsic)) {
    return GetNDData_wrong_sign_numu(NDChannel.mode, kA);
  }
  PRISMOUT("Invalid ND Data request: " << NDChannel.mode << ", "
                                       << NDChannel.chan);
  return kNoSuchNDDataSpectrum;
}

bool PredictionPRISM::HaveNDData(PRISM::BeamChan NDChannel, int kA) const {
  return bool(GetNDData(NDChannel, kA));
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

std::unique_ptr<PredictionInterp> &
PredictionPRISM::GetFDNonSwapAppOscPrediction(PRISM::BeamMode FDBM) const {
  return (FDBM == BeamMode::kNuMode)
             ? Predictions.FD.numu_ccinc_sel_sig_apposc_numode
             : Predictions.FD.numubar_ccinc_sel_sig_apposc_nubmode;
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

  auto &NDData_280kA = GetNDData(match_chan.from, 280);
  auto &NDPrediction_280kA = GetNDPrediction(match_chan.from, 280);

  auto &FDPrediction = GetFDPrediction(match_chan.to);
  auto &FDUnOscWeightedSigPrediction =
      GetFDUnOscWeightedSigPrediction(match_chan.to);

  /*PRISMOUT("Making PRISM prediction for: \n\t("
           << match_chan.from.mode << ":" << match_chan.from.chan << ") -> ("
           << match_chan.to.mode << ":" << match_chan.to.chan << ")");*/

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

  /*PRISMOUT("\n\tNDSigFlavor: "
           << NDSigFlavor << "\n\tNDSigSign: " << NDSigSign
           << "\n\tNDWrongSign: " << NDWrongSign << "\n\tNDWrongFlavor: "
           << NDWrongFlavor << "\n\tFDSigFlavor: " << FDSigFlavor
           << "\n\tFDSigSign: " << FDSigSign << "\n\tFDWrongSign: "
           << FDWrongSign << "\n\tFDWrongFlavor: " << FDWrongFlavor
           << "\n\tFDIntrinsicFlavor: " << FDIntrinsicFlavor);*/

  // Using maps for non-default constructible classes is awful...
  std::map<PredictionPRISM::PRISMComponent, ReweightableSpectrum> NDComps;
  std::map<PredictionPRISM::PRISMComponent, Spectrum> Comps;

  RunPlan const &NDRunPlan = (match_chan.from.mode == PRISM::BeamMode::kNuMode)
                                 ? RunPlan_nu
                                 : RunPlan_nub;

  double NDPOT = NDRunPlan.GetPlanPOT();
  assert(NDPOT > 0);

  NDComps.emplace(kNDData_unweighted_293kA, *NDData);
  //std::cout << "!!!!!! Weighting: 293kA." << std::endl;
  NDComps.emplace(kNDData_293kA,
                  NDRunPlan.Weight(*NDData, 293, fSetNDErrorsFromRate));
  NDComps.emplace(kNDDataCorr2D_293kA, NDComps.at(kNDData_293kA));

  NDComps.emplace(kNDData_unweighted_280kA, *NDData_280kA);
  //std::cout << "!!!!!! Weighting: 280kA" << std::endl;
  NDComps.emplace(kNDData_280kA,
                  NDRunPlan.Weight(*NDData_280kA, 280, fSetNDErrorsFromRate));
  NDComps.emplace(kNDDataCorr2D_280kA, NDComps.at(kNDData_280kA));

  // Start building MC components
  ReweightableSpectrum NDSig = ToReweightableSpectrum(
      NDRunPlan.Weight(NDPrediction->PredictComponentSyst(
                           calc, shift, NDSigFlavor, Current::kCC, NDSigSign),
                       293),
      NDPOT, fAnalysisAxis);

  NDComps.emplace(kNDSig_293kA, NDSig);
  NDComps.emplace(kNDSig2D_293kA, NDSig);

  ReweightableSpectrum NDSig_280kA = ToReweightableSpectrum(
      NDRunPlan.Weight(NDPrediction_280kA->PredictComponentSyst(
                           calc, shift, NDSigFlavor, Current::kCC, NDSigSign),
                       280),
      NDPOT, fAnalysisAxis);

  NDComps.emplace(kNDSig_280kA, NDSig_280kA);
  NDComps.emplace(kNDSig2D_280kA, NDSig_280kA);

  // ND Background subtraction
  if (fNCCorrection) {
    ReweightableSpectrum NC = ToReweightableSpectrum(
        NDRunPlan.Weight(
            NDPrediction->PredictComponentSyst(calc, shift, Flavors::kAll,
                                               Current::kNC, Sign::kBoth),
            293),
        NDPOT, fAnalysisAxis);

    NDComps.emplace(kNDNCBkg_293kA, NC);
    NDComps.at(kNDDataCorr2D_293kA) -= NDComps.at(kNDNCBkg_293kA);

    ReweightableSpectrum NC_280kA = ToReweightableSpectrum(
        NDRunPlan.Weight(
            NDPrediction_280kA->PredictComponentSyst(calc, shift, Flavors::kAll,
                                                     Current::kNC, Sign::kBoth),
            280),
        NDPOT, fAnalysisAxis);

    NDComps.emplace(kNDNCBkg_280kA, NC_280kA);
    NDComps.at(kNDDataCorr2D_280kA) -= NDComps.at(kNDNCBkg_280kA);
  }

  if (fWLBCorrection) {
    ReweightableSpectrum WLB = ToReweightableSpectrum(
        NDRunPlan.Weight(
            NDPrediction->PredictComponentSyst(calc, shift, NDWrongFlavor,
                                               Current::kCC, Sign::kBoth),
            293),
        NDPOT, fAnalysisAxis);

    NDComps.emplace(kNDWrongLepBkg_293kA, WLB);
    NDComps.at(kNDDataCorr2D_293kA) -= NDComps.at(kNDWrongLepBkg_293kA);

    ReweightableSpectrum WLB_280kA = ToReweightableSpectrum(
        NDRunPlan.Weight(
            NDPrediction_280kA->PredictComponentSyst(calc, shift, NDWrongFlavor,
                                                     Current::kCC, Sign::kBoth),
            280),
        NDPOT, fAnalysisAxis);

    NDComps.emplace(kNDWrongLepBkg_280kA, WLB_280kA);
    NDComps.at(kNDDataCorr2D_280kA) -= NDComps.at(kNDWrongLepBkg_280kA);
  }

  if (fWSBCorrection) {
    ReweightableSpectrum WSB = ToReweightableSpectrum(
        NDRunPlan.Weight(
            NDPrediction->PredictComponentSyst(calc, shift, NDSigFlavor,
                                               Current::kCC, NDWrongSign),
            293),
        NDPOT, fAnalysisAxis);

    NDComps.emplace(kNDWSBkg_293kA, WSB);
    NDComps.at(kNDDataCorr2D_293kA) -= NDComps.at(kNDWSBkg_293kA);

    ReweightableSpectrum WSB_280kA = ToReweightableSpectrum(
        NDRunPlan.Weight(
            NDPrediction_280kA->PredictComponentSyst(calc, shift, NDSigFlavor,
                                                     Current::kCC, NDWrongSign),
            280),
        NDPOT, fAnalysisAxis);

    NDComps.emplace(kNDWSBkg_280kA, WSB_280kA);
    NDComps.at(kNDDataCorr2D_280kA) -= NDComps.at(kNDWSBkg_280kA);
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
  std::pair<TH1 const *, TH1 const *> LinearCombination =
      fFluxMatcher->GetFarMatchCoefficients(calc, match_chan, shift);

  std::unique_ptr<TH1> UnRunPlannedLinearCombination_293kA =
      std::unique_ptr<TH1>(NDRunPlan.Unweight(LinearCombination.first, 293));

  std::unique_ptr<TH1> UnRunPlannedLinearCombination_280kA =
      std::unique_ptr<TH1>(NDRunPlan.Unweight(LinearCombination.second, 280));
  // We don't want the total POT of the runplan to affect the scale of the
  // coefficients, just the shape.
  UnRunPlannedLinearCombination_293kA->Scale(NDPOT);
  UnRunPlannedLinearCombination_280kA->Scale(NDPOT); 

  /*PRISMOUT("PRISM analysis axis: "
           << fNDOffAxis.GetLabels().front() << " with "
           << fNDOffAxis.GetBinnings().front().Edges().size() << " bins from "
           << fNDOffAxis.GetBinnings().front().Edges().front() << " to "
           << fNDOffAxis.GetBinnings().front().Edges().back() << ".");

  PRISMOUT("PRISM analysis axis: "
           << fND280kAAxis.GetLabels().front() << " with "
           << fND280kAAxis.GetBinnings().front().Edges().size() << " bins from "
           << fND280kAAxis.GetBinnings().front().Edges().front() << " to "
           << fND280kAAxis.GetBinnings().front().Edges().back() << ".");*/

  Spectrum UnRunPlannedLinearCombination_293kA_s(fNDOffAxis.GetLabels(),
                                                 fNDOffAxis.GetBinnings());
  UnRunPlannedLinearCombination_293kA_s.Clear();
  UnRunPlannedLinearCombination_293kA_s.FillFromHistogram(
      UnRunPlannedLinearCombination_293kA.get());
  UnRunPlannedLinearCombination_293kA_s.OverridePOT(NDPOT);
  UnRunPlannedLinearCombination_293kA_s.OverrideLivetime(0);
  Comps.emplace(kNDFDWeightings_293kA, UnRunPlannedLinearCombination_293kA_s);

  Spectrum UnRunPlannedLinearCombination_280kA_s(fND280kAAxis.GetLabels(),
                                                 fND280kAAxis.GetBinnings());
  UnRunPlannedLinearCombination_280kA_s.Clear();
  UnRunPlannedLinearCombination_280kA_s.FillFromHistogram(
      UnRunPlannedLinearCombination_280kA.get());
  UnRunPlannedLinearCombination_280kA_s.OverridePOT(NDPOT); 
  UnRunPlannedLinearCombination_280kA_s.OverrideLivetime(0);
  Comps.emplace(kNDFDWeightings_280kA, UnRunPlannedLinearCombination_280kA_s);

  if (NDComps.count(kNDSig_293kA)) {
    Comps.emplace(
        kNDSig_293kA,
        NDComps.at(kNDSig_293kA)
            .WeightedByErrors(UnRunPlannedLinearCombination_293kA.get()));
    Comps.emplace(
        kNDSig_280kA,
        NDComps.at(kNDSig_280kA)
            .WeightedByErrors(UnRunPlannedLinearCombination_280kA.get()));

    Comps.emplace(kPRISMMC, Comps.at(kNDSig_293kA));
    Comps.at(kPRISMMC) += Comps.at(kNDSig_280kA);
  }

  //std::cout << "WEIGHTING: kNDDataCorr_293kA" << std::endl;
  Comps.emplace(
      kNDDataCorr_293kA,
      NDComps.at(kNDDataCorr2D_293kA)
          .WeightedByErrors(UnRunPlannedLinearCombination_293kA.get()));


  /*TH2 *bla = NDComps.at(kNDDataCorr2D_293kA).ToTH2(NDPOT);
  TH1D *test293kA = HistCache::Copy(Comps.at(kNDDataCorr_293kA).ToTH1(NDPOT));
  gFile->WriteTObject(test293kA, "regularLC");

  for (int i = 0; i < bla->GetXaxis()->GetNbins(); ++i) {
    double sumOA(0), sumOAVar(0);
    for (int j = 0; j < bla->GetYaxis()->GetNbins(); ++j) {
      double bc = bla->GetBinContent(i + 1, j + 1);
      double be = bla->GetBinError(i + 1, j + 1);
      double bcw = bc * UnRunPlannedLinearCombination_293kA->GetBinContent(j + 1);
      double bew = be * UnRunPlannedLinearCombination_293kA->GetBinContent(j + 1);
      double varw = pow(UnRunPlannedLinearCombination_293kA->GetBinContent(j + 1), 2) * bc;
      if (i == 6 && j == 40) {
        std::cout << "[BC] = " << bc << ", [BE] = " << be << std::endl
                  << "[BCW] = " << bcw << ", [BEW] = " << bew << std::endl
                  << "[VARW] = " << varw << ", [VARBEW] = " << pow(bew,2) << std::endl;
      }
      sumOA += bcw;
      sumOAVar += varw;
    }
    double sumOAErr = sqrt(sumOAVar);
    test293kA->SetBinContent(i + 1, sumOA);
    test293kA->SetBinError(i + 1, sumOAErr);
  }

  //gFile->WriteObject(bla, "weightednotsummed_293kA");
  gFile->WriteTObject(test293kA, "manualLC");*/
  //HistCache::Delete(test293kA);
  //std::cout << "WEIGHTING: kNDDataCorr_280kA" << std::endl;
  Comps.emplace(
      kNDDataCorr_280kA,
      NDComps.at(kNDDataCorr2D_280kA)
          .WeightedByErrors(UnRunPlannedLinearCombination_280kA.get()));

  /*TH2 *bla280 = NDComps.at(kNDDataCorr2D_280kA).ToTH2(NDPOT);
  TH1D *test280kA = HistCache::Copy(Comps.at(kNDDataCorr_280kA).ToTH1(NDPOT));
  gFile->WriteTObject(test280kA, "regularLC280");
  for (int i = 0; i < bla280->GetXaxis()->GetNbins(); ++i) {
    double sumOA(0), sumOAVar(0);
    for (int j = 0; j < bla280->GetYaxis()->GetNbins(); ++j) {
      double bc = bla280->GetBinContent(i + 1, j + 1);
      double be = bla280->GetBinError(i + 1, j + 1);
      double bcw = bc * UnRunPlannedLinearCombination_280kA->GetBinContent(j + 1);
      double varw = pow(UnRunPlannedLinearCombination_280kA->GetBinContent(j + 1), 2) * bc;
    
      sumOA += bcw;
      sumOAVar += varw;
    }
    double sumOAErr = sqrt(sumOAVar);
    test280kA->SetBinContent(i + 1, sumOA);
    test280kA->SetBinError(i + 1, sumOAErr);
  } 
  gFile->WriteTObject(test280kA, "manualLC280");
  TH1D *testPRISM = HistCache::Copy(Comps.at(kNDDataCorr_293kA).ToTH1(NDPOT));
  for (int i = 0; i < test293kA->GetXaxis()->GetNbins(); i++) {
    double bc = test293kA->GetBinContent(i+1) + test280kA->GetBinContent(i+1);
    double be = sqrt(pow(test293kA->GetBinError(i+1), 2) +
                     pow(test280kA->GetBinError(i+1), 2));
    testPRISM->SetBinContent(i+1, bc);
    testPRISM->SetBinError(i+1, be);
  }
  gFile->WriteTObject(testPRISM, "testPRISMPred");
  HistCache::Delete(test293kA);
  HistCache::Delete(test280kA);
  HistCache::Delete(testPRISM);
  */

  Comps.emplace(kPRISMPred, Comps.at(kNDDataCorr_293kA));
  Comps.at(kPRISMPred) += Comps.at(kNDDataCorr_280kA);
  
  Comps.emplace(kNDLinearComb, Comps.at(kPRISMPred));
  //gFile->WriteTObject(Comps.at(kNDLinearComb).ToTH1(NDPOT), "regularPRISMPred");

  //-----------------------------
  // Get Efficiency from MC and fold into 
  // detector extrapolation from ND to FD
  // Set shift to kNoShift as we don't want systs affecting the MC efficiency correction
  // at the moment
  fMCEffCorrection->CalcEfficiency(calc, NDSigFlavor, FDSigFlavor, 
                                   Current::kCC, NDSigSign, FDSigSign);

  // Do ND to FD detector extrapolation here
  // Normalise the ERec v ETrue ND and FD matrices
  fNDFD_Matrix->NormaliseETrue(calc, shift, NDSigFlavor, FDSigFlavor, //kNoShift
                               Current::kCC, NDSigSign, FDSigSign,
                               fMCEffCorrection->GetNDefficiency(),
                               fMCEffCorrection->GetFDefficiency());
  // Extrapolate just the LC ND, not the MC
  fNDFD_Matrix->ExtrapolateNDtoFD(Comps.at(kNDLinearComb));
 
  Spectrum PRISMExtrapSpec = Spectrum(fNDFD_Matrix->GetPRISMExtrap(), 
                                      Comps.at(kPRISMPred).GetLabels(),
                                      Comps.at(kPRISMPred).GetBinnings(), 
                                      1, 0); // NDPOT
  // Keep PRISM extrap prediction wihtout corrections
  Comps.emplace(kNDData_FDExtrap, PRISMExtrapSpec);
  // For adding in MC corrections
  Comps.emplace(kNDDataCorr_FDExtrap, PRISMExtrapSpec);
  //-----------------------------

  // If we are doing numu -> nue propagation, need to correct for xsec
  if (FDSigFlavor == Flavors::kNuMuToNuE) {

    // These both contain the selection cuts currently... May or may not be what
    // we want, depends on the efficiency correction.
    Spectrum FD_nueapp_spectrum = FDPrediction->PredictComponentSyst(
        calc, shift, Flavors::kNuMuToNuE, Current::kCC, NDSigSign);

    TH1D *FD_nueapp_h = FD_nueapp_spectrum.ToTH1(NDPOT);

    Spectrum FD_numusurv_apposc_spectrum =
        GetFDNonSwapAppOscPrediction(match_chan.to.mode)
            ->PredictComponentSyst(calc, shift, Flavors::kNuMuToNuE,
                                   Current::kCC, NDSigSign);

    Comps.emplace(kFD_NumuNueCorr_Nue, FD_nueapp_spectrum);
    Comps.emplace(kFD_NumuNueCorr_Numu, FD_numusurv_apposc_spectrum);

    TH1D *FD_numusurv_apposc_h = FD_numusurv_apposc_spectrum.ToTH1(NDPOT);
    FD_nueapp_h->Divide(FD_numusurv_apposc_h);

    Spectrum FD_NumuNueCorr = Comps.at(kPRISMPred);
    FD_NumuNueCorr.Clear();
    FD_NumuNueCorr.FillFromHistogram(FD_nueapp_h);
    FD_NumuNueCorr.OverridePOT(NDPOT);
    FD_NumuNueCorr.OverrideLivetime(0);
    Comps.emplace(kFD_NumuNueCorr, FD_NumuNueCorr);

    TH1D *PRISMPred_h = Comps.at(kPRISMPred).ToTH1(NDPOT);
    PRISMPred_h->Multiply(FD_nueapp_h);

    Comps.at(kPRISMPred).Clear();
    Comps.at(kPRISMPred).FillFromHistogram(PRISMPred_h);
    Comps.at(kPRISMPred).OverridePOT(NDPOT);
    Comps.at(kPRISMPred).OverrideLivetime(0);

    HistCache::Delete(FD_nueapp_h);
    HistCache::Delete(FD_nueapp_h);
    HistCache::Delete(PRISMPred_h);
  }

  // If we have the FD background predictions add them back in

  if (fNCCorrection) {
    Comps.emplace(kFDNCBkg,
                  FDPrediction->PredictComponentSyst(
                      calc, shift, Flavors::kAll, Current::kNC, Sign::kBoth));
    Comps.at(kPRISMPred) += Comps.at(kFDNCBkg);
    Comps.at(kPRISMPred) += Comps.at(kFDNCBkg);
    Comps.at(kNDDataCorr_FDExtrap) += Comps.at(kFDNCBkg);
  }

  if (fWLBCorrection) {
    Comps.emplace(kFDWrongLepBkg,
                  FDPrediction->PredictComponentSyst(
                      calc, shift, FDWrongFlavor, Current::kCC, Sign::kBoth));
    Comps.at(kPRISMPred) += Comps.at(kFDWrongLepBkg);
    Comps.at(kPRISMMC) += Comps.at(kFDWrongLepBkg);
    Comps.at(kNDDataCorr_FDExtrap) += Comps.at(kFDWrongLepBkg);
  }

  if (fWSBCorrection) {
    Comps.emplace(kFDWSBkg,
                  FDPrediction->PredictComponentSyst(
                      calc, shift, FDSigFlavor, Current::kCC, FDWrongSign));
    Comps.at(kPRISMPred) += Comps.at(kFDWSBkg);
    Comps.at(kPRISMMC) += Comps.at(kFDWSBkg);
    Comps.at(kNDDataCorr_FDExtrap) += Comps.at(kFDWSBkg);
  }

  if (fIntrinsicCorrection) {
    Comps.emplace(kFDIntrinsicBkg,
                  FDPrediction->PredictComponentSyst(
                      calc, shift, FDIntrinsicFlavor, Current::kCC, FDSigSign));
    Comps.at(kPRISMPred) += Comps.at(kFDIntrinsicBkg);
    Comps.at(kPRISMMC) += Comps.at(kFDIntrinsicBkg);
    Comps.at(kNDDataCorr_FDExtrap) += Comps.at(kFDIntrinsicBkg);
  }

  Comps.emplace(kFDOscPred,
                FDPrediction->PredictComponentSyst(calc, shift, Flavors::kAll,
                                                   Current::kCC, Sign::kBoth));

  ReweightableSpectrum FDUnOscWeightedSig(
      ana::Constant(1), FDUnOscWeightedSig_h.get(), fAnalysisAxis.GetLabels(),
      fAnalysisAxis.GetBinnings(), NDPOT, 0);

  Comps.emplace(kFDUnOscPred, FDUnOscWeightedSig.UnWeighted());

  std::unique_ptr<TH1> resid(static_cast<TH1 *>(
      fFluxMatcher->GetLastResidual()->Clone("weighted_residual")));
  resid->SetDirectory(nullptr);

  Comps.emplace(kFDFluxCorr, FDUnOscWeightedSig.WeightedByErrors(resid.get()));

  //std::cout << Comps.at(kPRISMPred).ToTH1(NDPOT)->GetMaximum() << ", "
  //          << Comps.at(kFDFluxCorr).ToTH1(NDPOT)->GetMaximum() << std::endl;
  
  Comps.at(kPRISMPred) += Comps.at(kFDFluxCorr);

  // At Flux correction to extrapolated PRISM
  Comps.at(kNDDataCorr_FDExtrap) += Comps.at(kFDFluxCorr);

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
            Measurements.ND_293kA.numu_ccinc_sel_numode},
           {"M_ND_numubar_ccinc_sel_numode",
            Measurements.ND_293kA.numubar_ccinc_sel_numode},
           {"M_ND_numu_ccinc_sel_nubmode",
            Measurements.ND_293kA.numu_ccinc_sel_nubmode},
           {"M_ND_numubar_ccinc_sel_nubmode",
            Measurements.ND_293kA.numubar_ccinc_sel_nubmode},
           {"M_ND_nue_ccinc_sel_numode",
            Measurements.ND_293kA.nue_ccinc_sel_numode},
           {"M_ND_nuebar_ccinc_sel_nubmode",
            Measurements.ND_293kA.nuebar_ccinc_sel_nubmode},
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
       }) {
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
  HistAxis const _280kAAxis(_280kAaxis_labels, _280kAaxis_bins,
                            _280kAaxis_dummy_vars);
  HistAxis const energyMatchAxis(ematch_labels, ematch_bins, ematch_dummy_vars);

  std::unique_ptr<PredictionPRISM> pred = std::make_unique<PredictionPRISM>(
      predictionAxis, offAxis, _280kAAxis, energyMatchAxis);

  for (auto &meas : NamedReweightableSpectrumRefVect{
           {"M_ND_numu_ccinc_sel_numode",
            pred->Measurements.ND_293kA.numu_ccinc_sel_numode},
           {"M_ND_numubar_ccinc_sel_numode",
            pred->Measurements.ND_293kA.numubar_ccinc_sel_numode},
           {"M_ND_numu_ccinc_sel_nubmode",
            pred->Measurements.ND_293kA.numu_ccinc_sel_nubmode},
           {"M_ND_numubar_ccinc_sel_nubmode",
            pred->Measurements.ND_293kA.numubar_ccinc_sel_nubmode},
           {"M_ND_nue_ccinc_sel_numode",
            pred->Measurements.ND_293kA.nue_ccinc_sel_numode},
           {"M_ND_nuebar_ccinc_sel_nubmode",
            pred->Measurements.ND_293kA.nuebar_ccinc_sel_nubmode},
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
            pred->Predictions.FD.nuebar_ccinc_sel_sig_nubmode},
           {"P_FD_numu_ccinc_sel_sig_apposc_numode",
            pred->Predictions.FD.numu_ccinc_sel_sig_apposc_numode},
           {"P_FD_numubar_ccinc_sel_sig_apposc_nubmode",
            pred->Predictions.FD.numubar_ccinc_sel_sig_apposc_nubmode},
       }) {
    if (dir->GetDirectory(meas.first.c_str())) {
      meas.second.get() =
          PredictionInterp::LoadFrom(dir->GetDirectory(meas.first.c_str()));
    }
  }

  return pred;
}

} // namespace ana
