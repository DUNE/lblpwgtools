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
#include "CAFAna/PRISM/PRISMDetectorExtrapolation.h"
#include "CAFAna/PRISM/PRISMMCEffCorrection.h"

#include "TH3.h"

namespace ana {

// forward declare smearing matrix class
class NDFD_Matrix;
class MCEffCorrection;

/// Prediction that wraps a simple Spectrum
class PredictionPRISM : public IPrediction {
public:
  enum PRISMComponent {
    kNDData_293kA = 0,
    kNDDataCorr_293kA = 1,
    kNDDataCorr2D_293kA = 2,
    kNDSig_293kA = 3,
    kNDSig2D_293kA = 4,
    kNDWSBkg_293kA = 5,
    kNDNCBkg_293kA = 6,
    kNDWrongLepBkg_293kA = 7,
    kNDFDWeightings_293kA = 8,

    kNDData_280kA = 9,
    kNDDataCorr_280kA = 10,
    kNDDataCorr2D_280kA = 11,
    kNDSig_280kA = 12,
    kNDSig2D_280kA = 13,
    kNDWSBkg_280kA = 14,
    kNDNCBkg_280kA = 15,
    kNDWrongLepBkg_280kA = 16,
    kNDFDWeightings_280kA = 17,

    kNDLinearComb = 18,
    kFD_NumuNueCorr_Numu = 19,
    kFD_NumuNueCorr_Nue = 20,
    kFD_NumuNueCorr = 21,
    kFDFluxCorr = 22,
    kFDNCBkg = 23,
    kFDWSBkg = 24,
    kFDWrongLepBkg = 25,
    kFDNuTauCCBkg = 26,
    kFDIntrinsicBkg = 27,
    kFDUnOscPred = 28,
    kFDOscPred = 29,
    kPRISMPred = 30,
    kPRISMMC = 31,

    kNDData_unweighted_293kA = 32,
    kNDData_unweighted_280kA = 33,

    kNDDataExtrap2D_293kA = 34,
    kNDDataExtrap2D_280kA = 35,
    kNDDataExtrap_293kA = 36,
    kNDDataExtrap_280kA = 37,
    kNDData_FDExtrap = 38,
    kNDDataCorr_FDExtrap = 39,
    kExtrapCovarMatrix = 40,

    kNDMCExtrap2D_293kA = 41,
    kNDMCExtrap2D_280kA = 42,
    kNDMCExtrap_293kA = 43,
    kNDMCExtrap_280kA = 44,
    kNDMC_FDExtrap = 45,

  };

  static std::string GetComponentString(PRISMComponent pc) {
    switch (pc) {
    case kNDData_293kA: {
      return "NDData_293kA";
    }
    case kNDDataCorr_293kA: {
      return "NDDataCorr_293kA";
    }
    case kNDDataCorr2D_293kA: {
      return "NDDataCorr2D_293kA";
    }
    case kNDSig_293kA: {
      return "NDSigPred_293kA";
    }
    case kNDSig2D_293kA: {
      return "NDSigPred2D_293kA";
    }
    case kNDWSBkg_293kA: {
      return "NDWSBkg_293kA";
    }
    case kNDNCBkg_293kA: {
      return "NDNCBkg_293kA";
    }
    case kNDWrongLepBkg_293kA: {
      return "NDWrongLepBkg_293kA";
    }

    case kNDData_280kA: {
      return "NDData_280kA";
    }
    case kNDDataCorr_280kA: {
      return "NDDataCorr_280kA";
    }
    case kNDDataCorr2D_280kA: {
      return "NDDataCorr2D_280kA";
    }
    case kNDSig_280kA: {
      return "NDSigPred_280kA";
    }
    case kNDSig2D_280kA: {
      return "NDSigPred2D_280kA";
    }
    case kNDWSBkg_280kA: {
      return "NDWSBkg_280kA";
    }
    case kNDNCBkg_280kA: {
      return "NDNCBkg_280kA";
    }
    case kNDWrongLepBkg_280kA: {
      return "NDWrongLepBkg_280kA";
    }

    case kNDLinearComb: {
      return "NDLinearComb";
    }
    case kFD_NumuNueCorr_Numu: {
      return "FD_NumuNueCorr_Numu";
    }
    case kFD_NumuNueCorr_Nue: {
      return "FD_NumuNueCorr_Nue";
    }
    case kFD_NumuNueCorr: {
      return "FD_NumuNueCorr";
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
    case kFDNuTauCCBkg: {
      return "FDNuTauCCBkg";
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
    case kNDFDWeightings_293kA: {
      return "NDFDWeightings_293kA";
    }
    case kNDFDWeightings_280kA: {
      return "NDFDWeightings_280kA";
    }
    case kNDData_unweighted_293kA: {
      return "NDData_unweighted_293kA";
    }
    case kNDData_unweighted_280kA: {
      return "NDData_unweighted_280kA";
    }
    case kNDDataExtrap2D_293kA: {
      return "NDDataExtrap2D_293kA";
    }
    case kNDDataExtrap2D_280kA: {
      return "NDDataExtrap2D_280kA";
    }
    case kNDDataExtrap_293kA: {
      return "NDDataExtrap_293kA";
    }
    case kNDDataExtrap_280kA: {
      return "NDDataExtrap_280kA";
    }
    case kNDData_FDExtrap: {
      return "NDData_FDExtrap";
    }
    case kNDDataCorr_FDExtrap: {
      return "NDDataCorr_FDExtrap";
    }
    case kExtrapCovarMatrix: {
      return "ExtrapCovarMatrix";
    }
    case kNDMCExtrap2D_293kA: {
      return "NDMCExtrap2D_293kA";
    }
    case kNDMCExtrap2D_280kA: { 
      return "NDMCExtrap2D_280kA";
    }
    case kNDMCExtrap_293kA: {
      return "NDMCExtrap_293kA";
    }
    case kNDMCExtrap_280kA: {
      return "NDMCExtrap_280kA"; 
    }
    case kNDMC_FDExtrap: {
      return "NDMC_FDExtrap";
    }
    }
    return "";
  }

  HistAxis fAnalysisAxisND;
  HistAxis fAnalysisAxisFD;
  HistAxis fNDOffAxis;
  HistAxis fND280kAAxis;
  HistAxis fTrueAnalysisAxis;
  HistAxis fNDFDEnergyMatchAxis;
  HistAxis fCovarianceAxis;
  HistAxis fOffPredictionAxis;
  HistAxis f280kAPredictionAxis;
  HistAxis fFluxMatcherCorrectionAxes;

  PredictionPRISM(const HistAxis &AnalysisAxisND, 
                  const HistAxis &AnalysisAxisFD, 
                  const HistAxis &NDOffAxis,
                  const HistAxis &ND280kAAxis,
                  const HistAxis &TrueAnalysisAxis,
                  const HistAxis &NDFDEnergyMatchAxis,
                  const HistAxis &CovarianceAxis,
                  const HistAxis &OffPredictionAxis,
                  const HistAxis &_280kAPredictionAxis,
                  const HistAxis &FluxMatcherCorrectionAxes);

  void Initialize();

  static std::unique_ptr<PredictionPRISM> LoadFrom(TDirectory *dir, const std::string &name);
  virtual void SaveTo(TDirectory *dir, const std::string& name) const override;

  virtual Spectrum Predict(osc::IOscCalc *calc) const override;
  virtual Spectrum PredictSyst(osc::IOscCalc *calc,
                               ana::SystShifts shift = kNoShift) const;
  std::map<PRISMComponent, Spectrum> PredictPRISMComponents(
      osc::IOscCalc *calc, ana::SystShifts shift = kNoShift,
      PRISM::MatchChan match_chan = PRISM::kNumuDisappearance_Numode) const;

  std::map<PRISMComponent, Spectrum>
  PredictGaussianFlux(double mean, double width,
                      ana::SystShifts shift = kNoShift,
                      PRISM::BeamChan NDChannel = PRISM::kNumu_Numode) const;

  virtual Spectrum PredictComponent(osc::IOscCalc *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

  PRISMExtrapolator const *fFluxMatcher;
  void SetFluxMatcher(PRISMExtrapolator const *flux_matcher) {
    fFluxMatcher = flux_matcher;
  }

  // PredictionPRISM to own a pointer to a NDFD_Matrix object
  NDFD_Matrix const *fNDFD_Matrix;
  void SetNDFDDetExtrap(NDFD_Matrix const det_extrap) { 
    fNDFD_Matrix = &det_extrap; 
  }
  NDFD_Matrix const * Get_NDFD_Matrix() const {
    return fNDFD_Matrix;
  }

  // PredictionPRISM to own a pointer to a MCEffCorrection object
  MCEffCorrection const *fMCEffCorrection;
  void SetMC_NDFDEff(MCEffCorrection const eff_corr) { 
    fMCEffCorrection = &eff_corr; 
  }
  MCEffCorrection const * Get_MCEffCorrection() const {
    return fMCEffCorrection;
  } 

  void SetNDDataErrorsFromRate(bool v = true) { fSetNDErrorsFromRate = v; }
  void SetVaryNDFDMCData(bool v = true) { fVaryNDFDMCData = v; }

  //HistAxis fOffPredictionAxis;
  //HistAxis f280kAPredictionAxis;
  //HistAxis fFluxMatcherCorrectionAxes;
  double fDefaultOffAxisPOT;

  bool fNCCorrection = false;
  bool fWSBCorrection = false;
  bool fWLBCorrection = false;
  bool fNuTauCCCorrection = false;
  bool fIntrinsicCorrection = false;
  void SetNCCorrection(bool v = true) { fNCCorrection = v; }
  void SetWrongSignBackgroundCorrection(bool v = true) { fWSBCorrection = v; }
  void SetWrongLeptonBackgroundCorrection(bool v = true) { fWLBCorrection = v; }
  void SetNuTauCCBackgroundCorrection(bool v = true) { fNuTauCCCorrection = v; }
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
                       const Weight &wei = kUnweighted,
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
  void AddNDMCLoader(Loaders &, const Cut &cut, const Weight &wei = kUnweighted,
                     std::vector<ana::ISyst const *> systlist = {},
                     osc::IOscCalc* calc = (osc::IOscCalc*)0, 
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
  void AddFDMCLoader(Loaders &, const Cut &cut, const Weight &wei = kUnweighted,
                     std::vector<ana::ISyst const *> systlist = {},
                     osc::IOscCalc* calc = (osc::IOscCalc*)0,
                     PRISM::BeamChan FDChannel = PRISM::kNumu_Numode);

  void SetNDRunPlan(ana::RunPlan const &rp,
                    PRISM::BeamMode bm = PRISM::BeamMode::kNuMode) {
    ((bm == PRISM::BeamMode::kNuMode) ? RunPlan_nu : RunPlan_nub) = rp;
  }

  ReweightableSpectrum GetDiagonalCovariance(Spectrum const &spec, double POT, 
                                             HistAxis const &axis) const;

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
    _ND ND_293kA;
    _ND ND_280kA;
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
    _ND ND_293kA;
    _ND ND_280kA;

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

      std::unique_ptr<PredictionInterp> numu_ccinc_sel_sig_apposc_numode;
      std::unique_ptr<PredictionInterp> numubar_ccinc_sel_sig_apposc_nubmode;

      std::unique_ptr<PredictionInterp> nue_ccinc_sel_sig_apposc_numode;
      std::unique_ptr<PredictionInterp> nuebar_ccinc_sel_sig_apposc_nubmode;
    };
    _FD FD;
  };
  mutable _Predictions Predictions;

  std::unique_ptr<ReweightableSpectrum> &
  GetNDData_right_sign_numu(PRISM::BeamMode NDBM, int kA = 293) const;
  std::unique_ptr<ReweightableSpectrum> &
  GetNDData_right_sign_nue(PRISM::BeamMode NDBM, int kA = 293) const;
  std::unique_ptr<ReweightableSpectrum> &
  GetNDData_wrong_sign_numu(PRISM::BeamMode NDBM, int kA = 293) const;
  std::unique_ptr<ReweightableSpectrum> &
  GetNDData(PRISM::BeamChan NDChannel = PRISM::kNumu_Numode,
            int kA = 293) const;
  bool HaveNDData(PRISM::BeamChan NDChannel = PRISM::kNumu_Numode,
                  int kA = 293) const;

  std::unique_ptr<PredictionInterp> &
  GetNDPrediction_right_sign_numu(PRISM::BeamMode NDBM, int kA = 293) const;
  std::unique_ptr<PredictionInterp> &
  GetNDPrediction_right_sign_nue(PRISM::BeamMode NDBM, int kA = 293) const;
  std::unique_ptr<PredictionInterp> &
  GetNDPrediction_wrong_sign_numu(PRISM::BeamMode NDBM, int kA = 293) const;
  std::unique_ptr<PredictionInterp> &
  GetNDPrediction(PRISM::BeamChan NDChannel = PRISM::kNumu_Numode,
                  int kA = 293) const;
  bool HaveNDPrediction(PRISM::BeamChan NDChannel = PRISM::kNumu_Numode,
                        int kA = 293) const;

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

  // Prediction for unselected FD numus to be appearance oscillated.
  std::unique_ptr<PredictionInterp> &
  GetFDNonSwapAppOscPrediction(PRISM::BeamMode FDBM) const;
  // Prediction for unselected FD nues to be appearance oscillated.
  std::unique_ptr<PredictionInterp> &
  GetFDNueSwapAppOscPrediction(PRISM::BeamMode FDBM) const;

  // Need to keep a hold of these until the loader has gone.
  std::vector<std::unique_ptr<IPredictionGenerator>> fPredGens;

  // Whether to use the MC stats errors or the standard error on the ND data
  // sample to set the statistical errors.
  bool fSetNDErrorsFromRate;
  // Whether to apply shifts to MC ND & FD 'data' for debugging.
  // Will be false for 'standard' analysis where we have fixed data
  // and systematics are applied to 'MC components'.
  // Will be true if we want to do 'fake data' studies,
  // where we use nominal MC which is ignorant of shifts in the 'data'.
  bool fVaryNDFDMCData;
  
  // fAnalysisAxisFD and fAnalysisAxisND are not necessarily the same anymore,
  // so we only want to add MC corrections to PRISMPred (which has fAnalysisAxisND)
  // if the axes are the same.
  bool fAxisAgreement;

}; // namespace ana

} // namespace ana
