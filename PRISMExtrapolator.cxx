#include "CAFAna/PRISM/PRISMExtrapolator.h"
#include "CAFAna/PRISM/EigenUtils.h"
#include "CAFAna/PRISM/PRISMUtils.h"

#include "CAFAna/Prediction/PredictionInterp.h"

#include "CAFAna/Core/Binning.h"

#include "CAFAna/Systs/DUNEFluxSysts.h"

#include "OscLib/func/IOscCalculator.h"

#include "TDirectory.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include <iostream>

using namespace PRISM;

namespace ana {

Flavors::Flavors_t GetFlavor(NuChan fps) {
  if ((fps & NuChan::kNumu) || (fps & NuChan::kNumuBar)) {
    return Flavors::kAllNuMu;
  } else if ((fps & NuChan::kNue) || (fps & NuChan::kNueBar)) {
    return Flavors::kAllNuE;
  } else {
    std::cout << "Invalid NuChan: " << fps << std::endl;
    abort();
  }
}

Sign::Sign_t GetSign(NuChan fps) {
  if ((fps & NuChan::kNumu) || (fps & NuChan::kNue)) {
    return Sign::kNu;
  } else if ((fps & NuChan::kNumuBar) || (fps & NuChan::kNueBar)) {
    return Sign::kAntiNu;
  } else {
    std::cout << "Invalid NuChan: " << fps << std::endl;
    abort();
  }
}

PRISMExtrapolator::PRISMExtrapolator()
    : fNDEventRateInterp_nu(nullptr), fNDEventRateInterp_nub(nullptr),
      fFDEventRateInterp_nu(nullptr), fFDEventRateInterp_nub(nullptr),
      fLastMatch(nullptr), fLastGaussMatch(nullptr), fStoreDebugMatches(false) {
}

void PRISMExtrapolator::Initialize(
    std::map<std::string, PredictionInterp const *> const &preds) {

  if (preds.find("FD_nu") != preds.end()) {
    fFDEventRateInterp_nu = preds.find("FD_nu")->second;
  }
  if (preds.find("FD_nub") != preds.end()) {
    fFDEventRateInterp_nub = preds.find("FD_nub")->second;
  }

  if (preds.find("ND_nu") != preds.end()) {
    fNDEventRateInterp_nu = preds.find("ND_nu")->second;
  }
  if (preds.find("ND_nub") != preds.end()) {
    fNDEventRateInterp_nub = preds.find("ND_nub")->second;
  }
}

PredictionInterp const *PRISMExtrapolator::GetNDPred(BeamMode bm) const {
  PredictionInterp const *rtn = nullptr;

  switch (bm) {
  case BeamMode::kNuMode: {
    rtn = fNDEventRateInterp_nu;
    break;
  }
  case BeamMode::kNuBarMode: {
    rtn = fNDEventRateInterp_nub;
    break;
  }
  default: {
    rtn = nullptr;
  }
  }

  if (!rtn) {
    std::cout << "[ERROR]: PRISMExtrapolator attempted to use beam mode for ND "
                 "spectra that we do not have."
              << std::endl;
    abort();
  }
  return rtn;
}

PredictionInterp const *PRISMExtrapolator::GetFDPred(BeamMode bm) const {
  PredictionInterp const *rtn = nullptr;

  switch (bm) {
  case BeamMode::kNuMode: {
    rtn = fFDEventRateInterp_nu;

    break;
  }
  case BeamMode::kNuBarMode: {
    rtn = fFDEventRateInterp_nub;

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

TH1 const *PRISMExtrapolator::GetFarMatchCoefficients(
    osc::IOscCalculator *osc, TH1 const *FDUnOscWeighted, double max_OffAxis_m,
    PRISM::MatchChan match_chan, SystShifts shift, double &soln_norm,
    double &resid_norm) const {

  static osc::NoOscillations no;

  shift = GetFluxSystShifts(shift);

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

  PRISMOUT("GetFarMatchCoefficients: "
           << match_chan.from.mode << ", " << match_chan.from.chan << ", "
           << match_chan.to.mode << ", " << match_chan.to.chan);

  PredictionInterp const *NDEventRateInterp = GetNDPred(match_chan.from.mode);

  Spectrum NDOffAxis_spec = NDEventRateInterp->PredictComponentSyst(
      &no, shift, flav_nd, Current::kCC, sgn_nd);

  std::unique_ptr<TH2> NDOffAxis(NDOffAxis_spec.ToTH2(1));
  NDOffAxis->SetDirectory(nullptr);

  // Get the oscillated numu rate (either with app/disp probabiliy applied, but
  // always the nonswap so that any xsec ratios don't affect the coefficients.)
  PredictionInterp const *FDEventRateInterp = GetFDPred(match_chan.to.mode);
  std::unique_ptr<TH1> FDOsc(
      FDEventRateInterp
          ->PredictComponentSyst(osc, shift, flav_fd, Current::kCC, sgn_fd)
          .ToTH1(1));
  FDOsc->SetDirectory(nullptr);

  assert(NDOffAxis->GetXaxis()->GetNbins() == FDOsc->GetXaxis()->GetNbins());

  int NEBins = FDOsc->GetXaxis()->GetNbins();

  int NCoeffs = NDOffAxis->GetYaxis()->FindFixBin(max_OffAxis_m);
  if (cond.CoeffRegVector.size() < size_t(NCoeffs)) {
    std::fill_n(std::back_inserter(cond.CoeffRegVector),
                NCoeffs - cond.CoeffRegVector.size(), 1);
  }

  std::vector<double> off_axis_bin_edges;
  off_axis_bin_edges.push_back(NDOffAxis->GetYaxis()->GetBinLowEdge(1));
  for (int i = 0; i < NDOffAxis->GetYaxis()->GetNbins(); ++i) {
    off_axis_bin_edges.push_back(NDOffAxis->GetYaxis()->GetBinUpEdge(i + 1));
  }

  Eigen::MatrixXd NDFluxMatrix = GetEigenMatrix(NDOffAxis.get(), NCoeffs);
  NDFluxMatrix.transposeInPlace();
  Eigen::MatrixXd RegMatrix = Eigen::MatrixXd::Zero(NCoeffs, NCoeffs);

  if (cond.RegFactor) {
    for (int row_it = 0; row_it < (NCoeffs - 1); ++row_it) {
      // Penalize neighbouring coefficient difference by cond.CoeffRegVector[it]
      RegMatrix(row_it, row_it) = cond.RegFactor;
      RegMatrix(row_it, row_it + 1) =
          -cond.RegFactor * cond.CoeffRegVector[row_it];
    }
    RegMatrix(NCoeffs - 1, NCoeffs - 1) = cond.RegFactor;
  }

  Eigen::VectorXd Target = GetEigenFlatVector(FDOsc.get());

  int EBinLow = FDOsc->GetXaxis()->FindFixBin(cond.ENuMin);
  int col_min = 0;
  if (EBinLow != 0) {
    col_min = EBinLow - 1;
  }
  int EBinUp = FDOsc->GetXaxis()->FindFixBin(cond.ENuMax);
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

  fLastMatch = std::unique_ptr<TH1>(new TH1D("soln", ";OffAxisSlice;Weight",
                                             off_axis_bin_edges.size() - 1,
                                             off_axis_bin_edges.data()));
  fLastMatch->SetDirectory(nullptr);
  FillHistFromEigenVector(fLastMatch.get(), OffAxisWeights);

  Eigen::MatrixXd reg_shape_matrix = RegMatrix / cond.RegFactor;

  soln_norm = (reg_shape_matrix * OffAxisWeights).norm();
  resid_norm = (P * (NDFluxMatrix * OffAxisWeights - Target)).norm();

  fLastResidual =
      std::unique_ptr<TH1>(static_cast<TH1 *>(FDOsc->Clone("LastResidual")));
  fLastResidual->SetDirectory(nullptr);
  fLastResidual->Clear();

  Eigen::VectorXd BestFit = NDFluxMatrix * OffAxisWeights;

  if (FDUnOscWeighted) {
    for (int bin_it = 0; bin_it < fLastResidual->GetXaxis()->GetNbins();
         ++bin_it) {
      double bc_uo = FDUnOscWeighted->GetBinContent(bin_it + 1);
      double bc_o = FDOsc->GetBinContent(bin_it + 1);
      double e = (bc_o - BestFit[bin_it]) / bc_uo;
      if (!std::isnormal(e)) {
        e = 0;
      }
      fLastResidual->SetBinContent(bin_it + 1, e);
    }
  }

  if (fStoreDebugMatches) {
    int NCoeffs = NDOffAxis->GetYaxis()->FindFixBin(max_OffAxis_m);

    Eigen::MatrixXd NDFluxMatrix = GetEigenMatrix(NDOffAxis.get(), NCoeffs);
    NDFluxMatrix.transposeInPlace();

    Eigen::VectorXd Target = GetEigenFlatVector(FDOsc.get());

    fDebugTarget["last_match"] = std::unique_ptr<TH1>(
        new TH1D("soln", ";enu_bin;norm", Target.size(), 0, Target.size()));
    fDebugTarget["last_match"]->SetDirectory(nullptr);
    FillHistFromEigenVector(fDebugTarget["last_match"].get(), Target);

    fDebugBF["last_match"] = std::unique_ptr<TH1>(
        new TH1D("soln", ";enu_bin;norm", Target.size(), 0, Target.size()));
    fDebugBF["last_match"]->SetDirectory(nullptr);
    FillHistFromEigenVector(fDebugBF["last_match"].get(), BestFit);

    fDebugND["last_match"] = std::move(NDOffAxis);
    fDebugND["last_match"]->SetDirectory(nullptr);

    fDebugResid["last_match"] =
        std::unique_ptr<TH1>(dynamic_cast<TH1 *>(fLastResidual->Clone()));
    fDebugResid["last_match"]->SetDirectory(nullptr);
  }

  return fLastMatch.get();
}

TH1 const *PRISMExtrapolator::GetGaussianCoefficients(double mean, double width,
                                                      double max_OffAxis_m,
                                                      PRISM::BeamChan NDbc,
                                                      SystShifts shift) const {

  shift = GetFluxSystShifts(shift);

  Sign::Sign_t sgn_nd = GetSign(NDbc.chan);
  Flavors::Flavors_t flav_nd = GetFlavor(NDbc.chan);

  PredictionInterp const *NDEventRateInterp = GetNDPred(NDbc.mode);

  assert(NDEventRateInterp);

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

  static osc::NoOscillations no;

  Spectrum NDOffAxis_spec = NDEventRateInterp->PredictComponentSyst(
      &no, shift, flav_nd, Current::kCC, sgn_nd);

  std::unique_ptr<TH2> NDOffAxis(NDOffAxis_spec.ToTH2(1));
  NDOffAxis->SetDirectory(nullptr);

  int NEBins = NDOffAxis->GetXaxis()->GetNbins();

  Eigen::VectorXd Target = Eigen::VectorXd::Zero(NEBins);

  for (int i = 0; i < NEBins; ++i) {
    double E = NDOffAxis->GetXaxis()->GetBinCenter(i + 1);
    Target(i) = 1.0 / (width * sqrt(2 * M_PI)) *
                exp(-0.5 * pow((E - mean) / (width), 2));
  }

  int NCoeffs = NDOffAxis->GetYaxis()->FindFixBin(max_OffAxis_m);
  if (cond.CoeffRegVector.size() < size_t(NCoeffs)) {
    std::fill_n(std::back_inserter(cond.CoeffRegVector),
                NCoeffs - cond.CoeffRegVector.size(), 1);
  }

  Eigen::MatrixXd NDFluxMatrix = GetEigenMatrix(NDOffAxis.get(), NCoeffs);
  NDFluxMatrix.transposeInPlace();
  Eigen::MatrixXd RegMatrix = Eigen::MatrixXd::Zero(NCoeffs, NCoeffs);

  if (cond.RegFactor) {
    for (int row_it = 0; row_it < (NCoeffs - 1); ++row_it) {
      // Penalize neighbouring coefficient difference by cond.CoeffRegVector[it]
      RegMatrix(row_it, row_it) = cond.RegFactor;
      RegMatrix(row_it, row_it + 1) =
          -cond.RegFactor * cond.CoeffRegVector[row_it];
    }
    RegMatrix(NCoeffs - 1, NCoeffs - 1) = cond.RegFactor;
  }

  int EBinLow = NDOffAxis->GetXaxis()->FindFixBin(cond.ENuMin);
  int col_min = 0;
  if (EBinLow != 0) {
    col_min = EBinLow - 1;
  }
  int EBinUp = NDOffAxis->GetXaxis()->FindFixBin(cond.ENuMax);
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

  // No longer want a hard cut on fit region
  // Use P matrix to downweight regions outside fit region
  /* // Apply energy cut
  Eigen::MatrixXd NDFluxMatrix_cut =
      NDFluxMatrix.topRows(col_max).bottomRows(col_max - col_min);
  Target = Target.topRows(col_max).bottomRows(col_max - col_min);
*/

  assert(NDFluxMatrix.rows() == Target.size());
  assert(NDFluxMatrix.rows() == P.rows());

  Eigen::VectorXd OffAxisWeights =
      ((NDFluxMatrix.transpose() * P * NDFluxMatrix) +
       RegMatrix.transpose() * RegMatrix)
          .inverse() *
      NDFluxMatrix.transpose() * P * Target;

  fLastGaussMatch = std::unique_ptr<TH1>(
      new TH1D("soln", ";OffAxisSlice;Weight", OffAxisWeights.size(), 0,
               OffAxisWeights.size()));
  fLastGaussMatch->SetDirectory(nullptr);
  FillHistFromEigenVector(fLastGaussMatch.get(), OffAxisWeights);

  std::vector<double> bins;

  for (int i = 0; i < NDOffAxis->GetXaxis()->GetNbins(); ++i) {
    bins.push_back(NDOffAxis->GetXaxis()->GetBinLowEdge(i + 1));
  }
  bins.push_back(
      NDOffAxis->GetXaxis()->GetBinUpEdge(NDOffAxis->GetXaxis()->GetNbins()));

  fLastGaussResidual = std::unique_ptr<TH1>(static_cast<TH1 *>(
      new TH1D("LastResidual", ";E;Count", bins.size() - 1, bins.data())));
  fLastGaussResidual->SetDirectory(nullptr);
  fLastGaussResidual->Clear();

  // Eigen::MatrixXd NDFluxMatrix_shift =
  //     GetEigenMatrix(NDOffAxis_shift.get(), NCoeffs);
  // NDFluxMatrix_shift.transposeInPlace();

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

  return fLastGaussMatch.get();
}

void PRISMExtrapolator::Write(TDirectory *dir) {
  if (fLastMatch) {
    dir->WriteTObject(fLastMatch.get(), "last_match");
  }

  if (fLastGaussMatch) {
    dir->WriteTObject(fLastGaussMatch.get(), "last_gauss_match");
  }

  if (fStoreDebugMatches) {

    for (auto &fit : fDebugTarget) {
      dir->WriteTObject(fit.second.get(), (fit.first + "_DebugTarget").c_str());
    }

    for (auto &fit : fDebugBF) {
      dir->WriteTObject(fit.second.get(), (fit.first + "_DebugMatch").c_str());
    }

    for (auto &fit : fDebugND) {
      dir->WriteTObject(fit.second.get(), (fit.first + "_DebugND").c_str());
    }

    for (auto &fit : fDebugND_shift) {
      dir->WriteTObject(fit.second.get(),
                        (fit.first + "_DebugND_shift").c_str());
    }

    for (auto &fit : fDebugResid) {
      dir->WriteTObject(fit.second.get(), (fit.first + "_DebugResid").c_str());
    }
  }
}

} // namespace ana
