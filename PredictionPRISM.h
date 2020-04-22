#pragma once

#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Prediction/PredictionGenerator.h"
#include "CAFAna/Prediction/PredictionInterp.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Prediction/PredictionNoOsc.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/OscillatableSpectrum.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/PRISM/PRISMExtrapolator.h"
#include "CAFAna/PRISM/RunPlan.h"

#include "TH3.h"

namespace ana {

/// Prediction that wraps a simple Spectrum
class PredictionPRISM : public IPrediction {
public:
  enum PRISMComponent {
    kNDData = 0,
    kNDDataCorr = 1,
    kNDDataCorr2D = 2,
    kNDSig = 3,
    kNDSig2D = 4,
    kNDWSBkg = 5,
    kNDNCBkg = 6,
    kNDWrongLepBkg = 7,
    kFDFluxCorr = 8,
    kFDNCBkg = 9,
    kFDWSBkg = 10,
    kFDWrongLepBkg = 11,
    kFDIntrinsicBkg = 12,
    kFDUnOscPred = 13,
    kFDOscPred = 14,
    kPRISMPred = 15,
    kPRISMMC = 16,
    kNDFDWeightings = 17
  };

  static std::string GetComponentString(PRISMComponent pc) {
    switch (pc) {
    case kNDData: {
      return "NDData";
    }
    case kNDDataCorr: {
      return "NDDataCorr";
    }
    case kNDDataCorr2D: {
      return "NDDataCorr2D";
    }
    case kNDSig: {
      return "NDSigPred";
    }
    case kNDSig2D: {
      return "NDSigPred2D";
    }
    case kNDWSBkg: {
      return "NDWSBkg";
    }
    case kNDNCBkg: {
      return "NDNCBkg";
    }
    case kNDWrongLepBkg: {
      return "NDWrongLepBkg";
    }
    case kFDFluxCorr: {
      return "FDFluxCorr";
    }
    case kFDNCBkg: {
      return "FDNCBkg";
    }
    case kFDWSBkg: {
      return "FDWSBkg";
    }
    case kFDWrongLepBkg: {
      return "FDWrongLepBkg";
    }
    case kFDIntrinsicBkg: {
      return "FDIntrinsicBkg";
    }
    case kFDUnOscPred: {
      return "FDUnOscPred";
    }
    case kFDOscPred: {
      return "FDOscPred";
    }
    case kPRISMPred: {
      return "PRISMPred";
    }
    case kPRISMMC: {
      return "PRISMMC";
    }
    case kNDFDWeightings: {
      return "NDFDWeightings";
    }
    }
    return "";
  }

  HistAxis fAnalysisAxis;
  HistAxis fNDOffAxis;
  HistAxis fNDFDEnergyMatchAxis;

  PredictionPRISM(const HistAxis &AnalysisAxis, const HistAxis &NDOffAxis,
                  const HistAxis &NDFDEnergyMatchAxis);

  static std::unique_ptr<PredictionPRISM> LoadFrom(TDirectory *dir);
  virtual void SaveTo(TDirectory *dir) const override;

  virtual Spectrum Predict(osc::IOscCalculator *calc) const override;
  virtual Spectrum PredictSyst(osc::IOscCalculator *calc,
                               ana::SystShifts shift = kNoShift) const;
  std::map<PRISMComponent, Spectrum>
  PredictPRISMComponents(osc::IOscCalculator *calc,
                         ana::SystShifts shift = kNoShift,
                         PRISM::BeamChan NDChannel = PRISM::kNumu_Numode,
                         PRISM::BeamChan FDChannel = PRISM::kNumu_Numode) const;

  std::map<PRISMComponent, Spectrum>
  PredictGaussianFlux(double mean, double width,
                      ana::SystShifts shift = kNoShift,
                      PRISM::BeamChan NDChannel = PRISM::kNumu_Numode) const;

  virtual Spectrum PredictComponent(osc::IOscCalculator *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

  PRISMExtrapolator const *fFluxMatcher;
  void SetFluxMatcher(PRISMExtrapolator const *flux_matcher) {
    fFluxMatcher = flux_matcher;
  }

  void SetNDDataErrorsFromRate(bool v = true) { fSetNDErrorsFromRate = v; }

  HistAxis fOffPredictionAxis;
  HistAxis fFluxMatcherCorrectionAxes;
  double fMaxOffAxis;
  double fDefaultOffAxisPOT;

  bool fNCCorrection;
  bool fWSBCorrection;
  bool fWLBCorrection;
  bool fIntrinsicCorrection;
  void SetNCCorrection(bool v = true) { fNCCorrection = v; }
  void SetWrongSignBackgroundCorrection(bool v = true) { fWSBCorrection = v; }
  void SetWrongLeptonBackgroundCorrection(bool v = true) { fWLBCorrection = v; }
  void SetIntrinsicBackgroundCorrection(bool v = true) {
    fIntrinsicCorrection = v;
  }

  ///\brief Call to add a ND Data component
  ///
  ///\details This can be called a number of times to add various ND 'data'
  /// components that might be used in the PRISM analysis. Components are
  /// defined by the combination of PRISM::NuChan and PRISM::BeamMode.
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
  void AddNDDataLoader(SpectrumLoaderBase &, const Cut &cut,
                       const Var &wei = kUnweighted,
                       ana::SystShifts shift = kNoShift,
                       PRISM::BeamChan NDChannel = PRISM::kNumu_Numode);

  ///\brief Call to add a ND MC component
  ///
  ///\details This can be called a number of times to add various ND MC
  /// components that might be used in the PRISM analysis. Components are
  /// defined by the combination of PRISM::NuChan and PRISM::BeamMode.
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
  void AddNDMCLoader(Loaders &, const Cut &cut, const Var &wei = kUnweighted,
                     std::vector<ana::ISyst const *> systlist = {},
                     PRISM::BeamChan NDChannel = PRISM::kNumu_Numode);

  ///\brief Call to add a FD MC component
  ///
  ///\details This can be called a number of times to add various FD MC
  /// components that might be used in the PRISM analysis. Components are
  /// defined by the combination of PRISM::NuChan and PRISM::BeamMode.
  /// The list of valid components is given below:
  /// * NumuNumuBar, NuMode (i.e. mu-like FHC)
  /// * NumuNumuBar, NuBarMode (i.e. mu-like FHC)
  /// * NueNueBar, NuMode (i.e. all e-like FHC)
  /// * NueNueBar, NuBarMode (i.e. all e-like RHC)
  ///
  /// The systlist add allowable systematic shifts to the constructed PredInterp
  /// for use in fits and systematic studies.
  void AddFDMCLoader(Loaders &, const Cut &cut, const Var &wei = kUnweighted,
                     std::vector<ana::ISyst const *> systlist = {},
                     PRISM::BeamChan FDChannel = PRISM::kNumu_Numode);

  void SetNDRunPlan(ana::RunPlan const &rp,
                    PRISM::BeamMode bm = PRISM::BeamMode::kNuMode) {
    ((bm == PRISM::BeamMode::kNuMode) ? RunPlan_nu : RunPlan_nub) = rp;
  }

protected:
  ana::RunPlan RunPlan_nu, RunPlan_nub;

  // Contains 'measurements' that go into the PRISM extrapolation, should be
  // Spectra or ReweightableSpectra. Should not react to systematics.
  struct _Measurements {

    struct _ND {
      std::unique_ptr<ReweightableSpectrum> numu_ccinc_sel_numode;
      std::unique_ptr<ReweightableSpectrum> numubar_ccinc_sel_numode;

      std::unique_ptr<ReweightableSpectrum> numu_ccinc_sel_nubmode;
      std::unique_ptr<ReweightableSpectrum> numubar_ccinc_sel_nubmode;

      std::unique_ptr<ReweightableSpectrum> nue_ccinc_sel_numode;
      std::unique_ptr<ReweightableSpectrum> nuebar_ccinc_sel_nubmode;
    };
    _ND ND;
  };

  mutable _Measurements Measurements;

  // Contains 'predictions' used for testing the PRISM procedure or for applying
  // corrections to data
  struct _Predictions {

    struct _ND {
      // Used to make PRISM Prediction Prediction, and to calculate backgrounds
      // to subtract
      std::unique_ptr<PredictionInterp> numu_ccinc_sel_numode;
      std::unique_ptr<PredictionInterp> numubar_ccinc_sel_numode;

      std::unique_ptr<PredictionInterp> numu_ccinc_sel_nubmode;
      std::unique_ptr<PredictionInterp> numubar_ccinc_sel_nubmode;

      std::unique_ptr<PredictionInterp> nue_ccinc_sel_numode;
      std::unique_ptr<PredictionInterp> nuebar_ccinc_sel_nubmode;
    };
    _ND ND;

    struct _FD {
      // Used for FD background re-addition, they keep track of WSB, NC, nue
      // contamination themselves
      std::unique_ptr<PredictionInterp> numu_ccinc_sel_numode;
      std::unique_ptr<PredictionInterp> nue_ccinc_sel_numode;

      std::unique_ptr<PredictionInterp> numubar_ccinc_sel_nubmode;
      std::unique_ptr<PredictionInterp> nuebar_ccinc_sel_nubmode;

      std::unique_ptr<PredictionInterp> numu_ccinc_sel_sig_numode;
      std::unique_ptr<PredictionInterp> nue_ccinc_sel_sig_numode;

      std::unique_ptr<PredictionInterp> numubar_ccinc_sel_sig_nubmode;
      std::unique_ptr<PredictionInterp> nuebar_ccinc_sel_sig_nubmode;
    };
    _FD FD;
  };
  mutable _Predictions Predictions;

  std::unique_ptr<ReweightableSpectrum> &
  GetNDData_right_sign_numu(PRISM::BeamMode NDBM) const;
  std::unique_ptr<ReweightableSpectrum> &
  GetNDData_right_sign_nue(PRISM::BeamMode NDBM) const;
  std::unique_ptr<ReweightableSpectrum> &
  GetNDData_wrong_sign_numu(PRISM::BeamMode NDBM) const;
  std::unique_ptr<ReweightableSpectrum> &
  GetNDData(PRISM::BeamChan NDChannel = PRISM::kNumu_Numode) const;
  bool HaveNDData(PRISM::BeamChan NDChannel = PRISM::kNumu_Numode) const;

  std::unique_ptr<PredictionInterp> &
  GetNDPrediction_right_sign_numu(PRISM::BeamMode NDBM) const;
  std::unique_ptr<PredictionInterp> &
  GetNDPrediction_right_sign_nue(PRISM::BeamMode NDBM) const;
  std::unique_ptr<PredictionInterp> &
  GetNDPrediction_wrong_sign_numu(PRISM::BeamMode NDBM) const;
  std::unique_ptr<PredictionInterp> &
  GetNDPrediction(PRISM::BeamChan NDChannel = PRISM::kNumu_Numode) const;
  bool HaveNDPrediction(PRISM::BeamChan NDChannel = PRISM::kNumu_Numode) const;

  std::unique_ptr<PredictionInterp> &
  GetFDPrediction_right_sign_numu(PRISM::BeamMode FDBM) const;
  std::unique_ptr<PredictionInterp> &
  GetFDPrediction_right_sign_nue(PRISM::BeamMode FDBM) const;
  std::unique_ptr<PredictionInterp> &
  GetFDPrediction(PRISM::BeamChan NDChannel = PRISM::kNumu_Numode) const;

  bool HaveFDPrediction(PRISM::BeamChan FDChannel = PRISM::kNumu_Numode) const;

  std::unique_ptr<PredictionInterp> &
  GetFDUnOscWeightedSigPrediction_right_sign_numu(PRISM::BeamMode FDBM) const;
  std::unique_ptr<PredictionInterp> &
  GetFDUnOscWeightedSigPrediction_right_sign_nue(PRISM::BeamMode FDBM) const;
  std::unique_ptr<PredictionInterp> &GetFDUnOscWeightedSigPrediction(
      PRISM::BeamChan FDChannel = PRISM::kNumu_Numode) const;

  bool HaveFDUnOscWeightedSigPrediction(
      PRISM::BeamChan FDChannel = PRISM::kNumu_Numode) const;

  // Need to keep a hold of these until the loader has gone.
  std::vector<std::unique_ptr<IPredictionGenerator>> fPredGens;

  // Whether to use the MC stats errors or the standard error on the ND data
  // sample to set the statistical errors.
  bool fSetNDErrorsFromRate;
}; // namespace ana

} // namespace ana
