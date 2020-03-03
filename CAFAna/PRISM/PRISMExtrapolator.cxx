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

namespace ana {

PRISMExtrapolator::PRISMExtrapolator()
    : fRegFactor(0), fENuMin(0xdeadbeef),
      fENuMax(0xdeadbeef), fCoeffRegVector{}, fLowEGaussFallOff(false),
      fStoreDebugMatches(false) {}

void PRISMExtrapolator::InitializeFluxMatcher(std::string const &FluxFilePath,
                                              int OffAxisBinMerge,
                                              int NDEnergyBinMerge,
                                              int FDEnergyBinMerge) {

  assert(FluxFilePath.size());

  std::unique_ptr<TFile> f(TFile::Open(FluxFilePath.c_str()));

  assert(f && !f->IsZombie());

  size_t is_ND = true; // ND

  // The order here will line up with the definition of
  // PRISMExtrapolator::FluxPredSpecies
  for (std::string const &det : {"ND", "FD"}) {
    for (std::string const &beammode : {"nu", "nubar"}) {
      for (std::string const &nu_name : {"numu", "nue", "numubar", "nuebar"}) {
        std::string hist_name =
            det + "_" + beammode + "_ppfx/LBNF_" + nu_name + "_flux_Nom";

        TH1 *h = dynamic_cast<TH1 *>(f->Get(hist_name.c_str()));
        assert(h);

        if (is_ND) {
          TH2 *h2 = dynamic_cast<TH2 *>(h);
          assert(h2);
          NDOffAxisPrediction.emplace_back(dynamic_cast<TH2 *>(h2->Clone()));
          NDOffAxisPrediction.back()->SetDirectory(nullptr);
          NDOffAxisPrediction.back()->RebinX(NDEnergyBinMerge);
          NDOffAxisPrediction.back()->RebinY(OffAxisBinMerge);
        } else {
          FDUnOscPrediction.emplace_back(dynamic_cast<TH1 *>(h->Clone()));
          FDUnOscPrediction.back()->SetDirectory(nullptr);
          FDUnOscPrediction.back()->RebinX(FDEnergyBinMerge);
        }
      }
    }
    is_ND = false; // move to FD
  }

  for (size_t i = 0; i < static_cast<size_t>(FluxPredSpecies::kUnhandled);
       ++i) {
    if (NDOffAxisPrediction[i]->GetXaxis()->GetNbins() !=
        FDUnOscPrediction[i]->GetXaxis()->GetNbins()) {
      std::cout << "[ERROR]: When loading flux predictions for PRISM flux "
                   "matching, ND hist("
                << i << ") had "
                << NDOffAxisPrediction[i]->GetXaxis()->GetNbins()
                << " energy bins, but the FD prediction had: "
                << FDUnOscPrediction[i]->GetXaxis()->GetNbins() << std::endl;
      abort();
    }
  }
  fMatchEventRates = false;
}

void PRISMExtrapolator::InitializeEventRateMatcher(
    PredictionInterp const *NDEventRateInterp,
    PredictionInterp const *FDEventRateInterp) {
  fNDEventRateInterp = NDEventRateInterp;
  fFDEventRateInterp = FDEventRateInterp;

  fMatchEventRates = true;
}

bool PRISMExtrapolator::CheckOffAxisBinningConsistency(
    Binning const &off_axis_binning) const {

  if (fMatchEventRates) { // Assume true...
    return true;
  }

  std::vector<double> binning_edges = off_axis_binning.Edges();
  std::vector<double> matcher_off_axis_edges;

  for (int bi_it = 0;
       bi_it < NDOffAxisPrediction.front()->GetYaxis()->GetNbins(); ++bi_it) {
    if (!bi_it) {
      matcher_off_axis_edges.push_back(
          NDOffAxisPrediction.front()->GetYaxis()->GetBinLowEdge(bi_it + 1));
    }
    matcher_off_axis_edges.push_back(
        NDOffAxisPrediction.front()->GetYaxis()->GetBinUpEdge(bi_it + 1));
  }

  for (size_t be_it = 0; be_it < binning_edges.size(); ++be_it) {
    if (fabs(binning_edges[be_it] - matcher_off_axis_edges[be_it]) > 1E-6) {
      std::cout << "In PRISMExtrapolator::CheckOffAxisBinningConsistency:  "
                   "Analysis bin("
                << be_it << ") edge @ " << binning_edges[be_it]
                << " but corresponding flux bin edge is @ "
                << matcher_off_axis_edges[be_it] << std::endl;
      return false;
    }
  }
  return true;
}

TH1 const *PRISMExtrapolator::GetMatchCoefficientsEventRate(
    osc::IOscCalculator *osc, double max_OffAxis_m, SystShifts shift) const {
  assert(fNDEventRateInterp);
  assert(fFDEventRateInterp);

  for (auto s : shift.ActiveSysts()) {
    if (s->ShortName().find("flux") == std::string::npos) {
      shift.SetShift(s, 0);
    }
  }

  Spectrum NDSpect = fNDEventRateInterp->PredictComponentSyst(
      osc, shift, Flavors::kAllNuMu, Current::kCC, Sign::kNu);

  Spectrum FDSpect = fFDEventRateInterp->PredictComponentSyst(
      osc, shift, Flavors::kAllNuMu, Current::kCC, Sign::kNu);

  std::unique_ptr<TH2> NDOffAxis(NDSpect.ToTH2(ana::FD_ND_FVRatio(50)));
  std::unique_ptr<TH1> FDOsc(FDSpect.ToTH1(1));

  assert(NDOffAxis->GetXaxis()->GetNbins() == FDOsc->GetXaxis()->GetNbins());

  int NEBins = FDOsc->GetXaxis()->GetNbins();

  int NCoeffs = NDOffAxis->GetYaxis()->FindFixBin(max_OffAxis_m);
  if (fCoeffRegVector.size() < size_t(NCoeffs)) {
    std::fill_n(std::back_inserter(fCoeffRegVector),
                NCoeffs - fCoeffRegVector.size(), 1);
  }

  Eigen::MatrixXd NDFluxMatrix = GetEigenMatrix(NDOffAxis.get(), NCoeffs);
  NDFluxMatrix.transposeInPlace();
  Eigen::MatrixXd RegMatrix = Eigen::MatrixXd::Zero(NCoeffs, NCoeffs);

  if (fRegFactor) {
    for (int row_it = 0; row_it < (NCoeffs - 1); ++row_it) {
      // Always penalize coefficient height;
      RegMatrix(row_it, row_it) = fRegFactor;
      // Penalize neighbouring coefficient difference by fCoeffRegVector[it]
      RegMatrix(row_it, row_it + 1) = -fRegFactor * fCoeffRegVector[row_it];
    }
    RegMatrix(NCoeffs - 1, NCoeffs - 1) = fRegFactor;
  }

  Eigen::VectorXd Target = GetEigenFlatVector(FDOsc.get());

  int EBinLow = FDOsc->GetXaxis()->FindFixBin(fENuMin);
  int col_min = 0;
  if (EBinLow != 0) {
    col_min = EBinLow - 1;
  }
  int EBinUp = FDOsc->GetXaxis()->FindFixBin(fENuMax);
  int col_max = NEBins - 1;
  if (EBinUp != NEBins) {
    col_max = EBinUp - 1;
  }

  // inverse covariance matrix for down weighting
  Eigen::MatrixXd P = Eigen::MatrixXd::Identity(NEBins, NEBins);
  for (int row = 0; row < NEBins; row++) {
    if (row <= col_min) {
      P(row, row) *= 0.05;
    }
    if (row >= col_max) {
      P(row, row) *= 0.4;
    }
  }                   

  /* // Apply energy cut
  Eigen::MatrixXd NDFluxMatrix_cut =
      NDFluxMatrix.topRows(col_max).bottomRows(col_max - col_min);
  Target = Target.topRows(col_max).bottomRows(col_max - col_min);
*/
  assert(NDFluxMatrix.rows() == Target.size());

  Eigen::VectorXd OffAxisWeights =
      ((NDFluxMatrix.transpose() * P * NDFluxMatrix) +
       RegMatrix.transpose() * RegMatrix)
          .inverse() *
      NDFluxMatrix.transpose() * P * Target;

  fMatchCache["last_match"] = std::unique_ptr<TH1>(
      new TH1D("soln", ";OffAxisSlice;Weight", OffAxisWeights.size(), 0,
               OffAxisWeights.size()));
  fMatchCache["last_match"]->SetDirectory(nullptr);
  FillHistFromEigenVector(fMatchCache["last_match"].get(), OffAxisWeights);

  fLastResidual =
      std::unique_ptr<TH1>(static_cast<TH1 *>(FDOsc->Clone("LastResidual")));
  fLastResidual->SetDirectory(nullptr);
  fLastResidual->Clear();

  Eigen::VectorXd BestFit = NDFluxMatrix * OffAxisWeights;

  for (int bin_it = 0; bin_it < fLastResidual->GetXaxis()->GetNbins();
       ++bin_it) {
    double bc = FDOsc->GetBinContent(bin_it + 1);
    double e = (bc - BestFit[bin_it]) / bc;
    if (!std::isnormal(e)) {
      e = 0;
    }
    fLastResidual->SetBinContent(bin_it + 1, e);
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

  return fMatchCache["last_match"].get();
}

TH1 const *PRISMExtrapolator::GetMatchCoefficientsFlux(
    osc::IOscCalculator *osc, double max_OffAxis_m,
    PRISMExtrapolator::FluxPredSpecies NDMode,
    PRISMExtrapolator::FluxPredSpecies FDMode) const {

  std::string uniq_soln_name =
      std::string(osc->GetParamsHash()->AsString()) + "_" +
      std::to_string(static_cast<size_t>(NDMode)) + "_" +
      std::to_string(static_cast<size_t>(FDMode)) + "_" +
      std::to_string(max_OffAxis_m);

  if (!fMatchCache.count(uniq_soln_name)) {

    int flavBefore = (FDMode < FluxPredSpecies::kNumu_nubarmode) ? 14 : -14;

    int flavAfter = 0;
    switch (FDMode) {
    case FluxPredSpecies::kNumu_numode: {
      flavAfter = 14;
      break;
    }
    case FluxPredSpecies::kNue_numode: {
      flavAfter = 12;
      break;
    }
    case FluxPredSpecies::kNumubar_numode: {
      flavAfter = -14;
      break;
    }
    case FluxPredSpecies::kNuebar_numode: {
      flavAfter = -12;
      break;
    }
    case FluxPredSpecies::kNumu_nubarmode: {
      flavAfter = -14;
      break;
    }
    case FluxPredSpecies::kNue_nubarmode: {
      flavAfter = -12;
      break;
    }
    case FluxPredSpecies::kNumubar_nubarmode: {
      flavAfter = 14;
      break;
    }
    case FluxPredSpecies::kNuebar_nubarmode: {
      flavAfter = -12;
      break;
    }
    default: {
      abort();
    }
    }

    TH2 *NDOffAxis = NDOffAxisPrediction[static_cast<size_t>(NDMode)].get();
    TH1 *FDUnosc = FDUnOscPrediction[static_cast<size_t>(FDMode)].get();

    assert(NDOffAxis->GetXaxis()->GetNbins() ==
           FDUnosc->GetXaxis()->GetNbins());

    int NEBins = FDUnosc->GetXaxis()->GetNbins();

    int NCoeffs = NDOffAxis->GetYaxis()->FindFixBin(max_OffAxis_m);

    Eigen::MatrixXd NDFluxMatrix = GetEigenMatrix(NDOffAxis, NCoeffs);
    NDFluxMatrix.transposeInPlace();
    Eigen::MatrixXd RegMatrix = Eigen::MatrixXd::Zero(NCoeffs, NCoeffs);

    if (fRegFactor) {
      for (int row_it = 0; row_it < (NCoeffs - 1); ++row_it) {
        RegMatrix(row_it, row_it) = fRegFactor;
        RegMatrix(row_it, row_it + 1) = -fRegFactor;
      }
      RegMatrix(NCoeffs - 1, NCoeffs - 1) = fRegFactor;
    }

    Eigen::VectorXd Target(FDUnosc->GetXaxis()->GetNbins());

    for (int ebin_it = 0; ebin_it < Target.size(); ++ebin_it) {
      Target[ebin_it] = FDUnosc->GetBinContent(ebin_it + 1) *
                        osc->P(flavBefore, flavAfter,
                               FDUnosc->GetXaxis()->GetBinCenter(ebin_it + 1));
    }

    std::unique_ptr<TH1> FDOsc(static_cast<TH1 *>(FDUnosc->Clone("target")));
    FDOsc->SetDirectory(nullptr);
    FillHistFromEigenVector(FDOsc.get(), Target);

    int EBinLow = FDUnosc->GetXaxis()->FindFixBin(fENuMin);
    int col_min = 0;
    if (EBinLow != 0) {
      col_min = EBinLow - 1;
    }
    int EBinUp = FDUnosc->GetXaxis()->FindFixBin(fENuMax);
    int col_max = NEBins - 1;
    if (EBinUp != NEBins) {
      col_max = EBinUp - 1;
    }

    // // Apply energy cut
    Eigen::MatrixXd NDFluxMatrix_cut =
        NDFluxMatrix.topRows(col_max).bottomRows(col_max - col_min);
    Target = Target.topRows(col_max).bottomRows(col_max - col_min);

    assert(NDFluxMatrix_cut.rows() == Target.size());

    Eigen::VectorXd OffAxisWeights =
        ((NDFluxMatrix_cut.transpose() * NDFluxMatrix_cut) +
         RegMatrix.transpose() * RegMatrix)
            .inverse() *
        NDFluxMatrix_cut.transpose() * Target;

    fMatchCache[uniq_soln_name] = std::unique_ptr<TH1>(
        new TH1D("soln", ";OffAxisSlice;Weight", OffAxisWeights.size(), 0,
                 OffAxisWeights.size()));
    fMatchCache[uniq_soln_name]->SetDirectory(nullptr);
    FillHistFromEigenVector(fMatchCache[uniq_soln_name].get(), OffAxisWeights);

    fLastResidual = std::unique_ptr<TH1>(
        static_cast<TH1 *>(FDUnosc->Clone("LastResidual")));
    fLastResidual->SetDirectory(nullptr);
    fLastResidual->Clear();
    Eigen::VectorXd BestFit = NDFluxMatrix * OffAxisWeights;

    for (int bin_it = 0; bin_it < fLastResidual->GetXaxis()->GetNbins();
         ++bin_it) {
      double bc = FDOsc->GetBinContent(bin_it + 1);
      fLastResidual->SetBinContent(bin_it + 1, (bc - BestFit[bin_it]) / bc);
    }

    if (fStoreDebugMatches) {
      fDebugTarget[uniq_soln_name] = std::move(FDOsc);

      fDebugBF[uniq_soln_name] =
          std::unique_ptr<TH1>(static_cast<TH1 *>(FDUnosc->Clone("best_fit")));
      fDebugBF[uniq_soln_name]->Clear();
      fDebugBF[uniq_soln_name]->SetDirectory(nullptr);
      FillHistFromEigenVector(fDebugBF[uniq_soln_name].get(), BestFit);
    }
  }

  return fMatchCache[uniq_soln_name].get();
}

TH1 const *PRISMExtrapolator::GetMatchCoefficients(
    osc::IOscCalculator *osc, double max_OffAxis_m,
    PRISMExtrapolator::FluxPredSpecies NDMode,
    PRISMExtrapolator::FluxPredSpecies FDMode, SystShifts const &shift) const {
  return fMatchEventRates
             ? GetMatchCoefficientsEventRate(osc, max_OffAxis_m, shift)
             : GetMatchCoefficientsFlux(osc, max_OffAxis_m, NDMode, FDMode);
}

void PRISMExtrapolator::Write(TDirectory *dir) {
  for (auto &fit : fMatchCache) {
    std::cout << "Writing Match: " << fit.first << std::endl;
    dir->WriteTObject(fit.second.get(), fit.first.c_str());
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

    for (auto &fit : fDebugResid) {
      dir->WriteTObject(fit.second.get(), (fit.first + "_DebugResid").c_str());
    }
  }
}

} // namespace ana
