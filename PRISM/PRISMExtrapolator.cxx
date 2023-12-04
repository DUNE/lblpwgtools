#include "CAFAna/PRISM/PRISMExtrapolator.h"
#include "CAFAna/PRISM/EigenUtils.h"

#include "CAFAna/Prediction/PredictionInterp.h"
#include "CAFAna/Prediction/PredictionNoOsc.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/MathUtil.h"

#include "CAFAna/Systs/DUNEFluxSysts.h"
#include "CAFAna/Systs/RecoEnergyNDSysts.h"

#include "OscLib/IOscCalc.h"

#include "TDirectory.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include <iostream>

using namespace PRISM;

namespace ana {

//--------------------------------------------------------------------------------
Flavors::Flavors_t GetFlavor(NuChan fps) {
  if ((fps & NuChan::kNumu) || (fps & NuChan::kNumuBar)) {
    return Flavors::kNuMuToNuMu;
  } else if ((fps & NuChan::kNue) || (fps & NuChan::kNueBar)) {
    return Flavors::kNuMuToNuE;
  } else if ((fps & NuChan::kNutau) || (fps & NuChan::kNutauBar)) {
    return Flavors::kAllNuTau;
  } else {
    std::cout << "Invalid NuChan: " << fps << std::endl;
    abort();
  }
}

//--------------------------------------------------------------------------------
Sign::Sign_t GetSign(NuChan fps) {
  if ( (fps & NuChan::kNumu) || (fps & NuChan::kNue) || (fps & NuChan::kNutau) ) {
    return Sign::kNu;
  } else if ( (fps & NuChan::kNumuBar) || (fps & NuChan::kNueBar) || (fps & NuChan::kNutauBar) ) {
    return Sign::kAntiNu;
  } else {
    std::cout << "Invalid NuChan: " << fps << std::endl;
    abort();
  }
}

//--------------------------------------------------------------------------------
PRISMExtrapolator::PRISMExtrapolator()
    : fNDPredInterp_293kA_nu(nullptr), fNDPredInterp_293kA_nub(nullptr),
      fNDPredInterp_280kA_nu(nullptr), fNDPredInterp_280kA_nub(nullptr),
      fFDPredInterp_nu(nullptr), fFDPredInterp_nub(nullptr),
      fLastMatch_293kA(nullptr), fLastMatch_280kA(nullptr),
      fLastGaussMatch_293kA(nullptr), fLastGaussMatch_280kA(nullptr),
      fStoreDebugMatches(false), fMatchIntrinsicNue(false), fMatchWSBkg(false) {}

//--------------------------------------------------------------------------------
PRISMExtrapolator::PRISMExtrapolator(const PRISMExtrapolator &ExtrapPreds) {
  vNumuNueXsecRatioTrueEnu = ExtrapPreds.vNumuNueXsecRatioTrueEnu;
  vNumuNueXsecRatioTrueEnuAntiChannel = ExtrapPreds.vNumuNueXsecRatioTrueEnuAntiChannel;
}

//--------------------------------------------------------------------------------
void PRISMExtrapolator::Initialize(
    std::map<std::string, PredictionInterp *> const &preds) {

  if (preds.find("FD_nu") != preds.end()) {
    fFDPredInterp_nu = preds.find("FD_nu")->second;
  }
  if (preds.find("FD_nub") != preds.end()) {
    fFDPredInterp_nub = preds.find("FD_nub")->second;
  }

  if (preds.find("ND_293kA_nu") != preds.end()) {
    fNDPredInterp_293kA_nu = preds.find("ND_293kA_nu")->second;
  }
  if (preds.find("ND_293kA_nub") != preds.end()) {
    fNDPredInterp_293kA_nub = preds.find("ND_293kA_nub")->second;
  }
  if (preds.find("ND_280kA_nu") != preds.end()) {
    fNDPredInterp_280kA_nu = preds.find("ND_280kA_nu")->second;
  }
  if (preds.find("ND_280kA_nub") != preds.end()) {
    fNDPredInterp_280kA_nub = preds.find("ND_280kA_nub")->second;
  }
}

//--------------------------------------------------------------------------------
PredictionInterp const *PRISMExtrapolator::GetNDPred(BeamMode bm,
                                                     int kA) const {
  PredictionInterp const *rtn = nullptr;

  switch (bm) {
  case BeamMode::kNuMode: {
    rtn = (kA == 293) ? fNDPredInterp_293kA_nu : fNDPredInterp_280kA_nu;
    break;
  }
  case BeamMode::kNuBarMode: {
    rtn = (kA == 293) ? fNDPredInterp_293kA_nub : fNDPredInterp_280kA_nub;
    break;
  }
  default: {
    rtn = nullptr;
  }
  }

  if (!rtn) {
    std::cout << "[ERROR]: PRISMExtrapolator attempted to use beam mode(" << bm << ") for ND "
                 "spectra that we do not have."
              << std::endl;
    abort();
  }
  return rtn;
}

//--------------------------------------------------------------------------------
PredictionInterp const *PRISMExtrapolator::GetFDPred(BeamMode bm) const {
  PredictionInterp const *rtn = nullptr;

  switch (bm) {
  case BeamMode::kNuMode: {
    rtn = fFDPredInterp_nu;

    break;
  }
  case BeamMode::kNuBarMode: {
    rtn = fFDPredInterp_nub;

    break;
  }
  default: {
    rtn = nullptr;
  }
  }

  if (!rtn) {
    std::cout << "[ERROR]: PRISMExtrapolator attempted to use beam mode for FD "
                 "spectra that we do not have."
              << std::endl;
    abort();
  }
  return rtn;
}

//--------------------------------------------------------------------------------
std::pair<Eigen::ArrayXd, Eigen::ArrayXd> PRISMExtrapolator::GetFarMatchCoefficients(
    osc::IOscCalc *calc, PRISM::MatchChan match_chan, SystShifts shift,
    double &soln_norm, double &resid_norm, bool MatchWSBkg) const {

  static osc::NoOscillations no;

  // Only apply flux systematics when calculating LC weights
  shift = FilterFluxSystShifts(shift);

  if (!fConditioning.count(match_chan)) {
    std::cout
        << "[ERROR]: No ND->FD matching conditioning set for this channel: "
        << match_chan << std::endl;
    abort();
  }

  Conditioning const &cond = fConditioning.at(match_chan);

  Sign::Sign_t sgn_nd = GetSign(match_chan.from.chan),
               sgn_fd = GetSign(match_chan.to.chan);
  Flavors::Flavors_t flav_nd = GetFlavor(match_chan.from.chan);
  Flavors::Flavors_t flav_fd = GetFlavor(match_chan.to.chan);
  //define wrong sign for both ND and FD
  Sign::Sign_t wrong_sgn_nd = (sgn_nd == Sign::kNu) ? Sign::kAntiNu : Sign::kNu;
  Sign::Sign_t wrong_sgn_fd = (sgn_fd == Sign::kNu) ? Sign::kAntiNu : Sign::kNu;
  /*PRISMOUT("GetFarMatchCoefficients: "
           << match_chan.from.mode << ", " << match_chan.from.chan << ", "
           << match_chan.to.mode << ", " << match_chan.to.chan);*/

// define anti channel -> for WSbkg pred
  MatchChan antimatch_chan = GetAntiChannel(match_chan);// for the wrong sign background we always used ND data the opposite/anti channel

//usual ND prediction from the channel of interest -> for FD oscillated spectrum
  PredictionInterp const *NDPredInterp_293kA =
      GetNDPred(match_chan.from.mode, 293); // Can be flux OR ev rate
  Spectrum NDOffAxis_293kA_spec = NDPredInterp_293kA->PredictComponentSyst(
      &no, shift, flav_nd, Current::kCC, sgn_nd);
  // Get 293kA sample at ND.

  PredictionInterp const *NDPredInterp_280kA =
      GetNDPred(match_chan.from.mode, 280); // Can be flux OR ev rate
  Spectrum NDOffAxis_280kA_spec = NDPredInterp_280kA->PredictComponentSyst(
      &no, shift, flav_nd, Current::kCC, sgn_nd);
  // Get 280kA sample at ND.

//get ND prediction from the anti channel
  PredictionInterp const *NDPredInterp_293kA_AntiChannel =
      GetNDPred(antimatch_chan.from.mode, 293); // Can be flux OR ev rate
  Spectrum NDOffAxis_293kA_spec_AntiChannel = NDPredInterp_293kA_AntiChannel->PredictComponentSyst(
      &no, shift, flav_nd, Current::kCC, wrong_sgn_nd);
  // Get 293kA sample at ND.

  PredictionInterp const *NDPredInterp_280kA_AntiChannel =
      GetNDPred(antimatch_chan.from.mode, 280); // Can be flux OR ev rate
  Spectrum NDOffAxis_280kA_spec_AntiChannel = NDPredInterp_280kA_AntiChannel->PredictComponentSyst(
      &no, shift, flav_nd, Current::kCC, wrong_sgn_nd);
  // Get 280kA sample

  // Need to remove underflow and overflow elements.
  Eigen::MatrixXd FlowNDFluxMatrix_293kA;
  Eigen::MatrixXd FlowNDFluxMatrix_280kA;

  if(!MatchWSBkg){
    FlowNDFluxMatrix_293kA = ConvertArrayToMatrix(NDOffAxis_293kA_spec.GetEigen(1),
                                                  NDOffAxis_293kA_spec.GetBinnings());
    FlowNDFluxMatrix_280kA = ConvertArrayToMatrix(NDOffAxis_280kA_spec.GetEigen(1),
                                                  NDOffAxis_280kA_spec.GetBinnings());
  }
  else if(MatchWSBkg){
    FlowNDFluxMatrix_293kA = ConvertArrayToMatrix(NDOffAxis_293kA_spec_AntiChannel.GetEigen(1),
                                                  NDOffAxis_293kA_spec_AntiChannel.GetBinnings());
    FlowNDFluxMatrix_280kA = ConvertArrayToMatrix(NDOffAxis_280kA_spec_AntiChannel.GetEigen(1),
                                                  NDOffAxis_280kA_spec_AntiChannel.GetBinnings());
  }

  Eigen::MatrixXd NDFluxMatrix_293kA =
      FlowNDFluxMatrix_293kA.block(1, 1, FlowNDFluxMatrix_293kA.rows() - 2,
                                   FlowNDFluxMatrix_293kA.cols() - 2);
  Eigen::MatrixXd NDFluxMatrix_280kA =
      FlowNDFluxMatrix_280kA.block(1, 1, FlowNDFluxMatrix_280kA.rows() - 2,
                                   FlowNDFluxMatrix_280kA.cols() - 2);

  // Get the oscillated numu rate (either with app/disp probabiliy applied, but
  // always the nonswap so that any xsec ratios don't affect the coefficients.)
  PredictionInterp const *FDPredInterp = GetFDPred(match_chan.to.mode); // Can be flux OR ev rate
  Spectrum FDOsc_spec = FDPredInterp->PredictComponentSyst(
      calc, shift, flav_fd, Current::kCC, sgn_fd);
  // Include right sign intrinsic nue bkg in target flux matching
  Spectrum FDOsc_intrinsic_nue_spec = FDPredInterp->PredictComponentSyst(
      calc, shift, Flavors::kNuEToNuE, Current::kCC, sgn_fd);
  //1. define wrong sing intrinsic nue bkg spectrum
  Spectrum FDOsc_intrinsic_WS_nue_spec = FDPredInterp->PredictComponentSyst(
      calc, shift, Flavors::kNuEToNuE, Current::kCC, wrong_sgn_fd);
  //2.define wrong sign background (muons ) from beam contamination spectrum;
  Spectrum FDOsc_WSBkg = FDPredInterp->PredictComponentSyst(
      calc, shift, flav_fd, Current::kCC, wrong_sgn_fd);
  Eigen::VectorXd FlowTarget;

  if(!MatchWSBkg){ // the matchWSB flag is always off for the standard prediction
    if ( fMatchIntrinsicNue && ( (match_chan.to.chan & NuChan::kNueApp) || (match_chan.to.chan & NuChan::kNueBarApp) ) ) {

      FlowTarget = FDOsc_intrinsic_nue_spec.GetEigen(1).matrix();
      for (int bin = 0; bin < FlowTarget.size(); bin++) {
        FlowTarget(bin) *= vNumuNueXsecRatioTrueEnu(bin); // numu/nue xsec ratio vs true nu E applied
      }

      FlowTarget += FDOsc_spec.GetEigen(1).matrix();
    } else {
        FlowTarget = FDOsc_spec.GetEigen(1).matrix();
    }
  }
  else if(MatchWSBkg == true){ //MatchWSBkg prediction
    if((match_chan.to.chan & NuChan::kNueApp) || (match_chan.to.chan & NuChan::kNueBarApp) ){

      FlowTarget = FDOsc_intrinsic_WS_nue_spec.GetEigen(1).matrix();
      for (int bin = 0; bin < FlowTarget.size(); bin++) {
        FlowTarget(bin) *= vNumuNueXsecRatioTrueEnuAntiChannel(bin); // numu/nue xsec ratio vs true nu E applied for anti channel
      }
      FlowTarget += FDOsc_WSBkg.GetEigen(1).matrix(); //no cross section correction since we deal with mu_nu here
    }
    else{ //if disappearance channel only WSBkg from nu_mu beam contamination -> intrinsic WS from MC
      FlowTarget = FDOsc_WSBkg.GetEigen(1).matrix();
    }
  }

  Eigen::VectorXd Target = FlowTarget.segment(1, FlowTarget.size() - 2);

  Spectrum FDUnOsc_spec = FDPredInterp->PredictComponentSyst(
        &no, shift, Flavors::kNuMuToNuMu, Current::kCC, sgn_fd);
  Spectrum FDUnOsc_intrinsic_RS_nue_spec =  FDPredInterp->PredictComponentSyst(
        &no, shift, Flavors::kNuEToNuE, Current::kCC, sgn_fd);
  Spectrum FDUnOsc_intrinsic_WS_nue_spec =  FDPredInterp->PredictComponentSyst(
        &no, shift, Flavors::kNuEToNuE, Current::kCC, wrong_sgn_fd);
  Spectrum FDUnosc_WSBkg = FDPredInterp->PredictComponentSyst(
        &no, shift, Flavors::kNuMuToNuMu, Current::kCC, wrong_sgn_fd);

  Eigen::VectorXd FlowFDUnOsc_vec;
  if(!MatchWSBkg ){
    FlowFDUnOsc_vec = FDUnOsc_spec.GetEigen(1).matrix();
    if( fMatchIntrinsicNue && ( (match_chan.to.chan & NuChan::kNueApp) || (match_chan.to.chan & NuChan::kNueBarApp) ) )
      FlowFDUnOsc_vec += FDUnOsc_intrinsic_RS_nue_spec.GetEigen(1).matrix();  //include the intrinsic RS to residual calculation as well
  }
  else if(MatchWSBkg){
    FlowFDUnOsc_vec = FDUnosc_WSBkg.GetEigen(1).matrix();
    if( (match_chan.to.chan & NuChan::kNueApp) || (match_chan.to.chan & NuChan::kNueBarApp) )
      FlowFDUnOsc_vec += FDUnOsc_intrinsic_WS_nue_spec.GetEigen(1).matrix();
  }
  Eigen::VectorXd FDUnOsc_vec = FlowFDUnOsc_vec.segment(1, FlowFDUnOsc_vec.size() - 2);

  // Make sure we have the same number of energy bins at ND and FD
  assert(NDFluxMatrix_293kA.cols() == Target.size());
  assert(NDFluxMatrix_280kA.cols() == Target.size());

  // Number of energy bins
  int NEBins = Target.size();
  // Number of off-axis position bins
  int NCoeffs_293kA = NDFluxMatrix_293kA.rows();
  int NCoeffs_280kA = NDFluxMatrix_280kA.rows();
  int NCoeffs = NCoeffs_293kA + NCoeffs_280kA;

  std::vector<double> off_axis_bin_edges_293kA = NDOffAxis_293kA_spec.GetBinnings().at(1).Edges();

  std::vector<double> off_axis_bin_edges_280kA = NDOffAxis_280kA_spec.GetBinnings().at(1).Edges();

  // Total ND flux matrix: combines 293kA and 280kA
  Eigen::MatrixXd NDFluxMatrix = Eigen::MatrixXd::Zero(NCoeffs, NEBins);
  // Top rows of the total ND matrix is the 293kA matrix
  NDFluxMatrix.topRows(NCoeffs_293kA) = NDFluxMatrix_293kA;
  // Bottom rows of the total ND matrix is the 280kA matrix
  NDFluxMatrix.bottomRows(NCoeffs_280kA) = NDFluxMatrix_280kA;

  NDFluxMatrix.transposeInPlace();

  Eigen::MatrixXd RegMatrix = Eigen::MatrixXd::Zero(NCoeffs, NCoeffs);

  if (cond.RegFactor_293kA || cond.RegFactor_280kA) {

    for (int row_it = 0; row_it < (NCoeffs_293kA - 1); ++row_it) {
      // Penalize neighbouring coefficient difference by cond.CoeffRegVector[it]
      RegMatrix(row_it, row_it) = cond.RegFactor_293kA;
      RegMatrix(row_it, row_it + 1) = -cond.RegFactor_293kA;
    }
    RegMatrix(NCoeffs_293kA - 1, NCoeffs_293kA - 1) = cond.RegFactor_293kA;

    for (int row_it = NCoeffs_293kA; row_it < (NCoeffs - 1); ++row_it) {
      // Penalize neighbouring coefficient difference by cond.CoeffRegVector[it]
      RegMatrix(row_it, row_it) = cond.RegFactor_280kA;
      RegMatrix(row_it, row_it + 1) = -cond.RegFactor_280kA;
    }
    RegMatrix(NCoeffs - 1, NCoeffs - 1) = cond.RegFactor_280kA;
  }

  int EBinLow = FDOsc_spec.GetBinnings().at(0).FindBin(cond.ENuMin);
  int col_min = 0;
  if (EBinLow != 0) {
    col_min = EBinLow - 1;
  }
  int EBinUp = FDOsc_spec.GetBinnings().at(0).FindBin(cond.ENuMax);
  int col_max = NEBins - 1;
  if (EBinUp != NEBins) {
    col_max = EBinUp - 1;
  }

  // inverse covariance matrix for down weighting
  Eigen::MatrixXd P = Eigen::MatrixXd::Identity(NEBins, NEBins);
  for (int row = 0; row < NEBins; row++) {
    if (row <= col_min) { // low energy bin(s) weight
      P(row, row) *= 0.8;
    }
    if (row >= col_max) { // high energy bin(s) weight
      P(row, row) *= 0.0;
    }
  }

  assert(NDFluxMatrix.rows() == Target.size());
  assert(NDFluxMatrix.rows() == P.rows());

  // Do the maths
  Eigen::VectorXd OffAxisWeights =
      ((NDFluxMatrix.transpose() * P * NDFluxMatrix) +
       RegMatrix.transpose() * RegMatrix)
          .inverse() *
      NDFluxMatrix.transpose() * P * Target;

  Eigen::VectorXd OffAxisWeights_293kA = OffAxisWeights.head(off_axis_bin_edges_293kA.size() - 1);
  Eigen::VectorXd OffAxisWeights_280kA = OffAxisWeights.tail(off_axis_bin_edges_280kA.size() - 1);

  Eigen::MatrixXd reg_shape_matrix = RegMatrix / cond.RegFactor_293kA;

  soln_norm = (reg_shape_matrix * OffAxisWeights).norm();
  resid_norm = (P * (NDFluxMatrix * OffAxisWeights - Target)).norm();

  Eigen::VectorXd Residual = Eigen::VectorXd::Zero(Target.size());

  Eigen::VectorXd BestFit = NDFluxMatrix * OffAxisWeights;

  // Get the residual for flux miss-matching correction.
  // Normalise by unoscillated numu event rate.
  for (int bin_it = 0; bin_it < Target.size(); ++bin_it) {
    double bc_o = Target(bin_it);
    double bc_u = FDUnOsc_vec(bin_it);
    double e = (bc_o - BestFit(bin_it)) / bc_u;
    if (!std::isnormal(e)) {
      e = 0;
    }
    Residual(bin_it) = e;
  }

  // Residual to be outputted must have the underflow and overflow elements.
  fLastResidual = Eigen::ArrayXd::Zero(Residual.size() + 2);
  fLastResidual.segment(1, Residual.size()) += Residual.array();

  if (fStoreDebugMatches) {

    fLastMatch_293kA = std::unique_ptr<TH1>(new TH1D(
        "soln_293kA", ";OffAxisSlice;Weight", off_axis_bin_edges_293kA.size() - 1,
        off_axis_bin_edges_293kA.data()));
    fLastMatch_293kA->SetDirectory(nullptr);
    FillHistFromEigenVector(fLastMatch_293kA.get(), OffAxisWeights_293kA);

    fLastMatch_280kA = std::unique_ptr<TH1>(new TH1D(
        "soln_280kA", ";OffAxisSlice;Weight", off_axis_bin_edges_280kA.size() - 1,
        off_axis_bin_edges_280kA.data()));
    fLastMatch_280kA->SetDirectory(nullptr);
    FillHistFromEigenVector(fLastMatch_280kA.get(), OffAxisWeights_280kA);

    std::unique_ptr<TH1> FDOscTarget_h(FDOsc_spec.ToTH1(1));
    fDebugOscTarget["last_match"] = std::move(FDOscTarget_h);
    fDebugOscTarget["last_match"]->SetDirectory(nullptr);
    fDebugTarget["last_match"] = std::unique_ptr<TH1>(
      static_cast<TH1 *>(fDebugOscTarget["last_match"]->Clone()));
    fDebugTarget["last_match"]->SetDirectory(nullptr);
    for (int i = 0; i < fDebugTarget["last_match"]->GetXaxis()->GetNbins(); i++) {
      fDebugTarget["last_match"]->SetBinContent(i + 1, Target(i));
    }

    std::unique_ptr<TH1> FDUnOsc_h(FDUnOsc_spec.ToTH1(1));
    fDebugUnOsc["last_match"] = std::move(FDUnOsc_h);
    fDebugUnOsc["last_match"]->SetDirectory(nullptr);

    fDebugBF["last_match"] = std::unique_ptr<TH1>(
      static_cast<TH1 *>(fDebugOscTarget["last_match"]->Clone()));
    fDebugBF["last_match"]->SetDirectory(nullptr);
    for (int i = 0; i < fDebugBF["last_match"]->GetXaxis()->GetNbins(); i++) {
      fDebugBF["last_match"]->SetBinContent(i + 1, BestFit(i));
    }

    std::unique_ptr<TH2> NDOffAxis_293kA(NDOffAxis_293kA_spec.ToTH2(1));
    fDebugND_293kA["last_match"] = std::move(NDOffAxis_293kA);
    fDebugND_293kA["last_match"]->SetDirectory(nullptr);

    fDebugND_293kA["last_match_weighted"] = std::unique_ptr<TH2>(
        static_cast<TH2 *>(fDebugND_293kA["last_match"]->Clone()));
    fDebugND_293kA["last_match_weighted"]->SetDirectory(nullptr);

    for (int i = 0;
         i < fDebugND_293kA["last_match_weighted"]->GetXaxis()->GetNbins();
         ++i) {
      for (int j = 0;
           j < fDebugND_293kA["last_match_weighted"]->GetYaxis()->GetNbins();
           ++j) {
        fDebugND_293kA["last_match_weighted"]->SetBinContent(
            i + 1, j + 1,
            fDebugND_293kA["last_match_weighted"]->GetBinContent(i + 1, j + 1) *
                fLastMatch_293kA->GetBinContent(j + 1));
        fDebugND_293kA["last_match_weighted"]->SetBinError(
            i + 1, j + 1,
            fDebugND_293kA["last_match_weighted"]->GetBinError(i + 1, j + 1) *
                fLastMatch_293kA->GetBinContent(j + 1));
      }
    }

    std::unique_ptr<TH2> NDOffAxis_280kA(NDOffAxis_280kA_spec.ToTH2(1));
    fDebugND_280kA["last_match"] = std::move(NDOffAxis_280kA);
    fDebugND_280kA["last_match"]->SetDirectory(nullptr);

    fDebugND_280kA["last_match_weighted"] = std::unique_ptr<TH2>(
        static_cast<TH2 *>(fDebugND_280kA["last_match"]->Clone()));
    fDebugND_280kA["last_match_weighted"]->SetDirectory(nullptr);

    for (int i = 0;
         i < fDebugND_280kA["last_match_weighted"]->GetXaxis()->GetNbins();
         ++i) {
      for (int j = 0;
           j < fDebugND_280kA["last_match_weighted"]->GetYaxis()->GetNbins();
           ++j) {
        fDebugND_280kA["last_match_weighted"]->SetBinContent(
            i + 1, j + 1,
            fDebugND_280kA["last_match_weighted"]->GetBinContent(i + 1, j + 1) *
                fLastMatch_280kA->GetBinContent(j + 1));
        fDebugND_280kA["last_match_weighted"]->SetBinError(
            i + 1, j + 1,
            fDebugND_280kA["last_match_weighted"]->GetBinError(i + 1, j + 1) *
                fLastMatch_280kA->GetBinContent(j + 1));
      }
    }

    fDebugResid["last_match"] = std::unique_ptr<TH1>(
      static_cast<TH1 *>(fDebugOscTarget["last_match"]->Clone()));
    fDebugResid["last_match"]->SetDirectory(nullptr);
    for (int i = 0; i < fDebugResid["last_match"]->GetXaxis()->GetNbins(); i++) {
      fDebugResid["last_match"]->SetBinContent(i + 1, Residual(i));
    }

    fDebugFitMatrix["last_match"] = std::unique_ptr<TH2>(new TH2D(
        "ndmatrix", ";component;enu_bin;content", NDFluxMatrix.cols(), 0,
        NDFluxMatrix.cols(), NDFluxMatrix.rows(), 0, NDFluxMatrix.rows()));
    fDebugFitMatrix["last_match"]->SetDirectory(nullptr);
    FillHistFromEigenMatrix(fDebugFitMatrix["last_match"].get(), NDFluxMatrix);

    fDebugFitMatrix["last_match_covmat"] =
        std::unique_ptr<TH2>(new TH2D("ndmatrix", ";component;flux", P.cols(),
                                      0, P.cols(), P.rows(), 0, P.rows()));
    fDebugFitMatrix["last_match_covmat"]->SetDirectory(nullptr);
    FillHistFromEigenMatrix(fDebugFitMatrix["last_match_covmat"].get(), P);

    fDebugFitMatrix["last_match_regmatrix"] = std::unique_ptr<TH2>(
        new TH2D("ndmatrix", ";component;flux", RegMatrix.cols(), 0,
                 RegMatrix.cols(), RegMatrix.rows(), 0, RegMatrix.rows()));
    fDebugFitMatrix["last_match_regmatrix"]->SetDirectory(nullptr);
    FillHistFromEigenMatrix(fDebugFitMatrix["last_match_regmatrix"].get(),
                            RegMatrix);
  }
  return {OffAxisWeights_293kA.array(), OffAxisWeights_280kA.array()};
}

//--------------------------------------------------------------------------------
std::pair<TH1 const *, TH1 const *> PRISMExtrapolator::GetGaussianCoefficients(
    double mean, double width, PRISM::BeamChan NDbc, SystShifts shift) const {

  static osc::NoOscillations no;

  shift = FilterFluxSystShifts(shift);

  Sign::Sign_t sgn_nd = GetSign(NDbc.chan);
  Flavors::Flavors_t flav_nd = GetFlavor(NDbc.chan);

  PredictionInterp const *NDPredInterp = GetNDPred(NDbc.mode);

  assert(NDPredInterp);

  MatchChan fake_match_chan{
      NDbc,
      (NDbc.mode == BeamMode::kNuMode ? kNumu_Numode : kNumuBar_NuBarmode)};

  if (!fConditioning.count(fake_match_chan)) {
    std::cout << "[ERROR]: No matching conditioning set for gaussian flux fits "
                 "for channel: "
              << NDbc
              << " (N.B. The gaussian flux fits use the disappearance channel "
                 "for the relevant beam mode)"
              << std::endl;
    abort();
  }

  Conditioning const &cond = fConditioning.at(fake_match_chan);

  PredictionInterp const *NDPredInterp_293kA =
      GetNDPred(fake_match_chan.from.mode, 293);

  Spectrum NDOffAxis_293kA_spec = NDPredInterp_293kA->PredictComponentSyst(
      &no, shift, flav_nd, Current::kCC, sgn_nd);

  std::unique_ptr<TH2> NDOffAxis_293kA(NDOffAxis_293kA_spec.ToTH2(1));
  NDOffAxis_293kA->SetDirectory(nullptr);

  PredictionInterp const *NDPredInterp_280kA =
      GetNDPred(fake_match_chan.from.mode, 280);

  Spectrum NDOffAxis_280kA_spec = NDPredInterp_280kA->PredictComponentSyst(
      &no, shift, flav_nd, Current::kCC, sgn_nd);

  std::unique_ptr<TH2> NDOffAxis_280kA(NDOffAxis_280kA_spec.ToTH2(1));
  NDOffAxis_280kA->SetDirectory(nullptr);

  assert(NDOffAxis_293kA->GetXaxis()->GetNbins() ==
         NDOffAxis_280kA->GetXaxis()->GetNbins());

  int NEBins = NDOffAxis_280kA->GetXaxis()->GetNbins();

  int NCoeffs_293kA = NDOffAxis_293kA->GetYaxis()->GetNbins();
  int NCoeffs_280kA = NDOffAxis_280kA->GetYaxis()->GetNbins();
  int NCoeffs = NCoeffs_293kA + NCoeffs_280kA;

  Eigen::MatrixXd NDFluxMatrix_293kA =
      GetEigenMatrix(NDOffAxis_293kA.get(), NCoeffs_293kA);
  Eigen::MatrixXd NDFluxMatrix_280kA =
      GetEigenMatrix(NDOffAxis_280kA.get(), NCoeffs_280kA);
  Eigen::MatrixXd NDFluxMatrix = Eigen::MatrixXd::Zero(NCoeffs, NEBins);
  NDFluxMatrix.topRows(NCoeffs_293kA) = NDFluxMatrix_293kA;
  NDFluxMatrix.bottomRows(NCoeffs_280kA) = NDFluxMatrix_280kA;
  NDFluxMatrix.transposeInPlace();

  Eigen::VectorXd Target = Eigen::VectorXd::Zero(NEBins);

  for (int i = 0; i < NEBins; ++i) {
    double E = NDOffAxis_293kA->GetXaxis()->GetBinCenter(i + 1);
    Target(i) = 1.0 / (width * sqrt(2 * M_PI)) *
                exp(-0.5 * pow((E - mean) / (width), 2));
  }

  Eigen::MatrixXd RegMatrix = Eigen::MatrixXd::Zero(NCoeffs, NCoeffs);

  if (cond.RegFactor_293kA || cond.RegFactor_280kA) {
    for (int row_it = 0; row_it < (NCoeffs_293kA - 1); ++row_it) {
      // Penalize neighbouring coefficient difference by cond.CoeffRegVector[it]
      RegMatrix(row_it, row_it) = cond.RegFactor_293kA;
      RegMatrix(row_it, row_it + 1) = -cond.RegFactor_293kA;
    }
    RegMatrix(NCoeffs_293kA - 1, NCoeffs_293kA - 1) = cond.RegFactor_293kA;

    for (int row_it = NCoeffs_293kA; row_it < (NCoeffs - 1); ++row_it) {
      // Penalize neighbouring coefficient difference by cond.CoeffRegVector[it]
      RegMatrix(row_it, row_it) = cond.RegFactor_280kA;
      RegMatrix(row_it, row_it + 1) = -cond.RegFactor_280kA;
    }
    RegMatrix(NCoeffs - 1, NCoeffs - 1) = cond.RegFactor_280kA;
  }

  int EBinLow = NDOffAxis_293kA->GetXaxis()->FindFixBin(cond.ENuMin);
  int col_min = 0;
  if (EBinLow != 0) {
    col_min = EBinLow - 1;
  }
  int EBinUp = NDOffAxis_293kA->GetXaxis()->FindFixBin(cond.ENuMax);
  int col_max = NEBins - 1;
  if (EBinUp != NEBins) {
    col_max = EBinUp - 1;
  }

  // inverse covariance matrix for down weighting
  Eigen::MatrixXd P = Eigen::MatrixXd::Identity(NEBins, NEBins);
  for (int row = 0; row < NEBins; row++) {
    if (row <= col_min) { // low energy bin(s) weight
      P(row, row) *= 0.8;
    }
    if (row >= col_max) { // high energy bin(s) weight
      P(row, row) *= 0.0;
    }
  }

  assert(NDFluxMatrix.rows() == Target.size());
  assert(NDFluxMatrix.rows() == P.rows());

  Eigen::VectorXd OffAxisWeights =
      ((NDFluxMatrix.transpose() * P * NDFluxMatrix) +
       RegMatrix.transpose() * RegMatrix)
          .inverse() *
      NDFluxMatrix.transpose() * P * Target;

  fLastGaussMatch_293kA = std::unique_ptr<TH1>(
      new TH1D("soln_293kA", ";OffAxisSlice;Weight", OffAxisWeights.size(), 0,
               OffAxisWeights.size()));
  fLastGaussMatch_293kA->SetDirectory(nullptr);
  FillHistFromEigenVector(fLastGaussMatch_293kA.get(), OffAxisWeights);

  fLastGaussMatch_280kA = std::unique_ptr<TH1>(
      new TH1D("soln_280kA", ";OffAxisSlice;Weight", OffAxisWeights.size(), 0,
               OffAxisWeights.size()));
  fLastGaussMatch_280kA->SetDirectory(nullptr);
  FillHistFromEigenVector(fLastGaussMatch_280kA.get(), OffAxisWeights);

  std::vector<double> bins;

  for (int i = 0; i < NDOffAxis_293kA->GetXaxis()->GetNbins(); ++i) {
    bins.push_back(NDOffAxis_293kA->GetXaxis()->GetBinLowEdge(i + 1));
  }
  bins.push_back(NDOffAxis_293kA->GetXaxis()->GetBinUpEdge(
      NDOffAxis_293kA->GetXaxis()->GetNbins()));

  fLastGaussResidual = std::unique_ptr<TH1>(static_cast<TH1 *>(
      new TH1D("LastResidual", ";E;Count", bins.size() - 1, bins.data())));
  fLastGaussResidual->SetDirectory(nullptr);
  fLastGaussResidual->Clear();

  Eigen::VectorXd BestFit = NDFluxMatrix * OffAxisWeights;

  for (int bin_it = 0; bin_it < NEBins; ++bin_it) {
    double e = (Target[bin_it] - BestFit[bin_it]);
    if (!std::isnormal(e)) {
      e = 0;
    }
    fLastGaussResidual->SetBinContent(bin_it + 1, e);
  }

  if (fStoreDebugMatches) {
    fDebugTarget["last_gauss_match"] = std::unique_ptr<TH1>(
        static_cast<TH1 *>(fLastGaussResidual->Clone("target")));
    fDebugTarget["last_gauss_match"]->Clear();
    fDebugTarget["last_gauss_match"]->SetDirectory(nullptr);
    FillHistFromEigenVector(fDebugTarget["last_gauss_match"].get(), Target);

    fDebugBF["last_match"] = std::unique_ptr<TH1>(
        static_cast<TH1 *>(fLastGaussResidual->Clone("best_fit")));
    fDebugBF["last_match"]->SetDirectory(nullptr);
    FillHistFromEigenVector(fDebugBF["last_match"].get(), BestFit);
  }

  return {fLastGaussMatch_293kA.get(), fLastGaussMatch_280kA.get()};
}

//--------------------------------------------------------------------------------
void PRISMExtrapolator::Write(TDirectory *dir) {
  if (fLastMatch_293kA) {
    dir->WriteTObject(fLastMatch_293kA.get(), "last_match_293kA");
  }
  if (fLastMatch_293kA) {
    dir->WriteTObject(fLastMatch_280kA.get(), "last_match_280kA");
  }

  if (fLastGaussMatch_293kA) {
    dir->WriteTObject(fLastGaussMatch_293kA.get(), "last_gauss_match_293kA");
  }
  if (fLastGaussMatch_280kA) {
    dir->WriteTObject(fLastGaussMatch_280kA.get(), "last_gauss_match_280kA");
  }

  if (fStoreDebugMatches) {

    for (auto &fit : fDebugOscTarget) {
      dir->WriteTObject(fit.second.get(), (fit.first + "_DebugOscTarget").c_str());
    }

    for (auto &fit : fDebugUnOsc) {
      dir->WriteTObject(fit.second.get(), (fit.first + "_DebugUnOsc").c_str());
    }

    for (auto &fit : fDebugTarget) {
      dir->WriteTObject(fit.second.get(), (fit.first + "_DebugTarget").c_str());
    }

    for (auto &fit : fDebugBF) {
      dir->WriteTObject(fit.second.get(), (fit.first + "_DebugMatch").c_str());
    }

    for (auto &fit : fDebugND_293kA) {
      dir->WriteTObject(fit.second.get(),
                        (fit.first + "_DebugND_293kA").c_str());
    }

    for (auto &fit : fDebugND_280kA) {
      dir->WriteTObject(fit.second.get(),
                        (fit.first + "_DebugND_280kA").c_str());
    }

    for (auto &fit : fDebugResid) {
      dir->WriteTObject(fit.second.get(), (fit.first + "_DebugResid").c_str());
    }

    for (auto &fit : fDebugFitMatrix) {
      dir->WriteTObject(fit.second.get(),
                        (fit.first + "_DebugFitMatrix").c_str());
    }
  }
}

} // namespace ana
