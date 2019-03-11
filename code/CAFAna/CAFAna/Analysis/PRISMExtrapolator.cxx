#include "CAFAna/Analysis/PRISMExtrapolator.h"

#include "CAFAna/Prediction/PredictionInterp.h"

#include "CAFAna/Core/Binning.h"

#include "OscLib/func/IOscCalculator.h"

#include "TDirectory.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include "Eigen/Dense"

#include <iostream>

void FillTH1FromEigenVector(TH1 *rh, Eigen::VectorXd const &vals) {
  Int_t dim = rh->GetDimension();
  if (dim == 1) {
    int idx = 0;
    for (Int_t x_it = 0; x_it < rh->GetXaxis()->GetNbins(); ++x_it) {
      double v = (idx >= vals.rows()) ? 0 : vals(idx);
      rh->SetBinContent(x_it + 1, v);
      rh->SetBinError(x_it + 1, 0);
      idx++;
    }
    // Reset flow bins
    rh->SetBinContent(0, 0);
    rh->SetBinError(0, 0);
    rh->SetBinContent(rh->GetXaxis()->GetNbins() + 1, 0);
    rh->SetBinError(rh->GetXaxis()->GetNbins() + 1, 0);
    return;
  }
  std::cout << "[ERROR]: FillHistFromstdvector cannot handle THND where N = "
            << dim << std::endl;
  abort();
}

Eigen::MatrixXd
FillEigenMatrixFromTH2(TH2 const *h2,
                       int MaxCols = std::numeric_limits<int>::max(),
                       bool swap_axes = false) {

  TAxis const *row_axis = swap_axes ? h2->GetYaxis() : h2->GetXaxis();
  TAxis const *col_axis = swap_axes ? h2->GetXaxis() : h2->GetYaxis();

  Int_t NCols = std::min(MaxCols, col_axis->GetNbins());
  Eigen::MatrixXd matrix = Eigen::MatrixXd::Zero(row_axis->GetNbins(), NCols);
  for (Int_t oabi_it = 0; oabi_it < NCols; ++oabi_it) {
    for (Int_t ebi_it = 0; ebi_it < row_axis->GetNbins(); ++ebi_it) {
      matrix(ebi_it, oabi_it) = h2->GetBinContent(ebi_it + 1, oabi_it + 1);
    }
  }
  return matrix;
}

Eigen::VectorXd FillEigenVectorFromTH1(TH1 const *h) {
  int nb = h->GetXaxis()->GetNbins();
  Eigen::VectorXd vec = Eigen::VectorXd::Zero(nb);
  for (Int_t bi_it = 0; bi_it < nb; ++bi_it) {
    vec[bi_it] = h->GetBinContent(bi_it + 1);
  }
  return vec;
}

PRISMExtrapolator::PRISMExtrapolator()
    : fRegFactor(0), fENuMin(0xdeadbeef), fENuMax(0xdeadbeef),
      fLowEGaussFallOff(false), fStoreDebugMatches(false) {}

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
    for (std::string const &beammode : {"numode", "nubarmode"}) {
      for (std::string const &nu_name : {"numu", "nue", "numubar", "nuebar"}) {
        std::string hist_name =
            det + "_" + beammode + "/LBNF_" + nu_name + "_flux";

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
    ana::PredictionInterp const *NDEventRateInterp,
    ana::PredictionInterp const *FDEventRateInterp) {
  fNDEventRateInterp = NDEventRateInterp;
  fFDEventRateInterp = FDEventRateInterp;

  fMatchEventRates = true;
}

bool PRISMExtrapolator::CheckOffAxisBinningConsistency(
    ana::Binning const &off_axis_binning) const {

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

Eigen::VectorXd SolveLinComb(TH2 const *NDPred, TH1 const *FDOsc,
                             double max_OffAxis_m, double reg_factor = 0) {
  assert(NDPred->GetXaxis()->GetNbins() == FDOsc->GetXaxis()->GetNbins());

  int NEBins = FDOsc->GetXaxis()->GetNbins();
  int NCoeffs = NDPred->GetYaxis()->FindFixBin(max_OffAxis_m);
  assert(NCoeffs);
  if (NCoeffs > NDPred->GetYaxis()->GetNbins()) {
    NCoeffs = NDPred->GetYaxis()->GetNbins();
  }

  Eigen::MatrixXd NDFluxMatrix = FillEigenMatrixFromTH2(NDPred, NCoeffs);
  Eigen::MatrixXd RegMatrix = Eigen::MatrixXd::Zero(NCoeffs, NCoeffs);

  if (reg_factor) {
    for (int row_it = 0; row_it < (NCoeffs - 1); ++row_it) {
      RegMatrix(row_it, row_it) = reg_factor;
      RegMatrix(row_it, row_it + 1) = -reg_factor;
    }
    RegMatrix(NCoeffs - 1, NCoeffs - 1) = reg_factor;
  }

  Eigen::VectorXd Target = FillEigenVectorFromTH1(FDOsc);

  assert(NDFluxMatrix.rows() == Target.size());

  return ((NDFluxMatrix.transpose() * NDFluxMatrix) +
          RegMatrix.transpose() * RegMatrix)
             .inverse() *
         NDFluxMatrix.transpose() * Target;
}

TH1 *PRISMExtrapolator::GetMatchCoefficientsEventRate(
    osc::IOscCalculator *osc, double max_OffAxis_m) const {
  assert(fNDEventRateInterp);
  assert(fFDEventRateInterp);

  std::unique_ptr<TH2> NDOffAxis(fNDEventRateInterp->Predict(nullptr).ToTH2(1));
  std::unique_ptr<TH1> FDOsc(fFDEventRateInterp->Predict(osc).ToTH1(1));

  Eigen::VectorXd soln =
      SolveLinComb(NDOffAxis.get(), FDOsc.get(), max_OffAxis_m, fRegFactor);

  fMatchCache["last_match"] = std::unique_ptr<TH1>(
      new TH1D("soln", ";OffAxisSlice;Weight", soln.size(), 0, soln.size()));
  fMatchCache["last_match"]->SetDirectory(nullptr);
  FillTH1FromEigenVector(fMatchCache["last_match"].get(), soln);

  if (fStoreDebugMatches) {
    int NCoeffs = NDOffAxis->GetYaxis()->FindFixBin(max_OffAxis_m);

    Eigen::MatrixXd NDFluxMatrix =
        FillEigenMatrixFromTH2(NDOffAxis.get(), NCoeffs);
    Eigen::VectorXd Target = FillEigenVectorFromTH1(FDOsc.get());

    fDebugTarget["last_match"] = std::unique_ptr<TH1>(
        new TH1D("soln", ";enu_bin;norm", Target.size(), 0, Target.size()));
    fDebugTarget["last_match"]->SetDirectory(nullptr);
    FillTH1FromEigenVector(fDebugTarget["last_match"].get(), Target);

    fDebugBF["last_match"] = std::unique_ptr<TH1>(
        new TH1D("soln", ";enu_bin;norm", Target.size(), 0, Target.size()));
    fDebugBF["last_match"]->SetDirectory(nullptr);
    FillTH1FromEigenVector(fDebugBF["last_match"].get(), NDFluxMatrix * soln);
  }

  return fMatchCache["last_match"].get();
}

TH1 *PRISMExtrapolator::GetMatchCoefficientsFlux(
    osc::IOscCalculator *osc, double max_OffAxis_m,
    PRISMExtrapolator::FluxPredSpecies NDMode,
    PRISMExtrapolator::FluxPredSpecies FDMode) const {

  std::string uniq_soln_name =
      std::string(osc->GetParamsHash()->AsString()) + "_" +
      std::to_string(static_cast<size_t>(NDMode)) + "_" +
      std::to_string(static_cast<size_t>(FDMode)) + "_" +
      std::to_string(max_OffAxis_m);

  std::cout << "In PRISMExtrapolator::GetMatchCoefficients Osc has hash = "
            << uniq_soln_name << std::endl;

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
    default: { abort(); }
    }

    TH2 *NDOffAxis = NDOffAxisPrediction[static_cast<size_t>(NDMode)].get();
    TH1 *FDUnosc = FDUnOscPrediction[static_cast<size_t>(FDMode)].get();

    assert(NDOffAxis->GetXaxis()->GetNbins() ==
           FDUnosc->GetXaxis()->GetNbins());

    int NEBins = FDUnosc->GetXaxis()->GetNbins();

    int NCoeffs = NDOffAxis->GetYaxis()->FindFixBin(max_OffAxis_m);

    Eigen::MatrixXd NDFluxMatrix = FillEigenMatrixFromTH2(NDOffAxis, NCoeffs);
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

    assert(NDFluxMatrix.rows() == Target.size());

    Eigen::VectorXd OffAxisWeights =
        ((NDFluxMatrix.transpose() * NDFluxMatrix) +
         RegMatrix.transpose() * RegMatrix)
            .inverse() *
        NDFluxMatrix.transpose() * Target;

    fMatchCache[uniq_soln_name] = std::unique_ptr<TH1>(
        new TH1D("soln", ";OffAxisSlice;Weight", OffAxisWeights.size(), 0,
                 OffAxisWeights.size()));
    fMatchCache[uniq_soln_name]->SetDirectory(nullptr);
    FillTH1FromEigenVector(fMatchCache[uniq_soln_name].get(), OffAxisWeights);

    if (fStoreDebugMatches) {
      fDebugTarget[uniq_soln_name] = std::unique_ptr<TH1>(
          new TH1D("soln", ";enu_bin;norm", Target.size(), 0, Target.size()));
      fDebugTarget[uniq_soln_name]->SetDirectory(nullptr);
      FillTH1FromEigenVector(fDebugTarget[uniq_soln_name].get(), Target);

      fDebugBF[uniq_soln_name] = std::unique_ptr<TH1>(
          new TH1D("soln", ";enu_bin;norm", Target.size(), 0, Target.size()));
      fDebugBF[uniq_soln_name]->SetDirectory(nullptr);
      FillTH1FromEigenVector(fDebugBF[uniq_soln_name].get(),
                             NDFluxMatrix * OffAxisWeights);
    }
  }

  return fMatchCache[uniq_soln_name].get();
}

TH1 *PRISMExtrapolator::GetMatchCoefficients(
    osc::IOscCalculator *osc, double max_OffAxis_m,
    PRISMExtrapolator::FluxPredSpecies NDMode,
    PRISMExtrapolator::FluxPredSpecies FDMode) const {
  return fMatchEventRates
             ? GetMatchCoefficientsEventRate(osc, max_OffAxis_m)
             : GetMatchCoefficientsFlux(osc, max_OffAxis_m, NDMode, FDMode);
}

void PRISMExtrapolator::Write(TDirectory *dir) {
  for (auto &fit : fMatchCache) {
    std::cout << "Writing Match: " << fit.first << std::endl;
    dir->WriteObject(fit.second.get(), fit.first.c_str());
  }

  if (fStoreDebugMatches) {

    for (auto &fit : fDebugTarget) {
      dir->WriteObject(fit.second.get(), (fit.first + "_DebugTarget").c_str());
    }

    for (auto &fit : fDebugBF) {
      dir->WriteObject(fit.second.get(), (fit.first + "_DebugMatch").c_str());
    }
  }
}
