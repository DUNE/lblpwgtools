#pragma once

#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Experiment/IChiSqExperiment.h"

#include "CAFAna/Prediction/IPrediction.h"

#include "CAFAna/PRISM/EigenUtils.h"

namespace ana {
class SimpleChi2Experiment : public IChiSqExperiment {
public:
  SimpleChi2Experiment(
      IPrediction const *Pred, Spectrum const &Data, bool UseHistError = false,
      double POT = 0,
      std::pair<double, double> erange = {-std::numeric_limits<double>::max(),
                                          std::numeric_limits<double>::max()})
      : fPred(Pred), fUseHistError(UseHistError) {
    fPOT = POT;
    if (fPOT == 0) {
      fPOT = Data.POT();
    }
    fData = Data.ToTH1(fPOT);

    if (erange.first != -std::numeric_limits<double>::max()) {
      fBinRange.first = fData->GetXaxis()->FindFixBin(erange.first);
      if (fBinRange.first < 1) {
        fBinRange.first = 1;
      }
    } else {
      fBinRange.first = 1;
    }

    if (erange.second != std::numeric_limits<double>::max()) {
      fBinRange.second = fData->GetXaxis()->FindFixBin(erange.second);
      if (fBinRange.second > (fData->GetXaxis()->GetNbins() + 1)) {
        fBinRange.second = (fData->GetXaxis()->GetNbins() + 1);
      }
    } else {
      fBinRange.second = (fData->GetXaxis()->GetNbins() + 1);
    }
  }

  IPrediction const *fPred;
  TH1 const *fData;
  double fPOT;
  bool fUseHistError;

  std::pair<int, int> fBinRange;

  TH1D *GetPred(osc::IOscCalc *osc,
                const SystShifts &syst = SystShifts::Nominal()) const {
    return fPred->PredictSyst(osc, syst).ToTH1(fPOT);
  }

  double ChiSq(osc::IOscCalc *osc,
               const SystShifts &syst = SystShifts::Nominal()) const {

    TH1D *PredHist = GetPred(osc, syst);
    double chi2 = 0;
    for (int bi = fBinRange.first; bi < fBinRange.second; ++bi) {
      double pbc = PredHist->GetBinContent(bi + 1);
      double pbe =
          (fUseHistError ? pow(PredHist->GetBinError(bi + 1), 2) : 0) + pbc;

      if (!std::isnormal(pbe)) {
        continue;
      }

      double dbc = fData->GetBinContent(bi + 1);

      double contrib = pow((pbc - dbc), 2) / pbe;

      chi2 += contrib;
    }
    HistCache::Delete(PredHist);

    return chi2;
  }
};

class PRISMChi2Experiment : public IChiSqExperiment {
public:
  PRISMChi2Experiment(
      PredictionPRISM const *Pred, Spectrum const &Data,
      bool UseHistError = false, double POT = 0,
      PRISM::MatchChan match_chan = PRISM::kNumuDisappearance_Numode,
      std::pair<double, double> erange = {-std::numeric_limits<double>::max(),
                                          std::numeric_limits<double>::max()})
      : fPred(Pred), fUseHistError(UseHistError) {
    fPOT = POT;
    if (fPOT == 0) {
      fPOT = Data.POT();
    }
    fData = Data.ToTH1(fPOT);

    fMatchChannel = match_chan;

    if (erange.first != -std::numeric_limits<double>::max()) {
      fBinRange.first = fData->GetXaxis()->FindFixBin(erange.first);
      if (fBinRange.first < 1) {
        fBinRange.first = 1;
      }
    } else {
      fBinRange.first = 1;
    }

    if (erange.second != std::numeric_limits<double>::max()) {
      fBinRange.second = fData->GetXaxis()->FindFixBin(erange.second);
      if (fBinRange.second > (fData->GetXaxis()->GetNbins() + 1)) {
        fBinRange.second = (fData->GetXaxis()->GetNbins() + 1);
      }
    } else {
      fBinRange.second = (fData->GetXaxis()->GetNbins() + 1);
    }
  }

  PredictionPRISM const *fPred;
  TH1 const *fData;
  double fPOT;
  bool fUseHistError;

  PRISM::MatchChan fMatchChannel;
  std::pair<int, int> fBinRange;

  TH1D *GetPred(osc::IOscCalc *osc,
                const SystShifts &syst = SystShifts::Nominal()) const {
    return fPred->PredictPRISMComponents(osc, syst, fMatchChannel)
        .at(PredictionPRISM::kNDDataCorr_FDExtrap) 
        .ToTH1(fPOT);
  }

  double ChiSq(osc::IOscCalc *osc,
               const SystShifts &syst = SystShifts::Nominal()) const {

    TH1D *PredHist = GetPred(osc, syst);

    double chi2 = 0;
    for (int bi = fBinRange.first; bi < fBinRange.second; ++bi) {
      double pbc = PredHist->GetBinContent(bi + 1);
      double pbe =
          (fUseHistError ? pow(PredHist->GetBinError(bi + 1), 2) : 0) + pbc;

      if (!std::isnormal(pbe)) {
        continue;
      }

      double dbc = fData->GetBinContent(bi + 1);

      double contrib = pow((pbc - dbc), 2) / pbe;

      chi2 += contrib;
    }
    HistCache::Delete(PredHist);

    return chi2;
  }
};

//----------------------------------------------------

// Another Chi2Experiment for Chi2 calculation using full
// PRISM covariance

class PRISMChi2CovarExperiment : public IChiSqExperiment {
public:
  PRISMChi2CovarExperiment(PredictionPRISM const *Pred, Spectrum const &Data,
                           bool UseCovariance = false, double NDPOT = 0, double FDPOT = 0,
                           PRISM::MatchChan match_chan = PRISM::kNumuDisappearance_Numode,
                           std::pair<double, double> erange = {-std::numeric_limits<double>::max(),
                                                               std::numeric_limits<double>::max()})
                           : fPred(Pred), fUseCovariance(UseCovariance) {
    fPOTFD = FDPOT;
    if (fPOTFD == 0) {
      fPOTFD = Data.POT();
    }
    fData = Data.ToTH1(fPOTFD);
    fData_vec = GetEigenFlatVector(fData);

    fPOTND = NDPOT;

    fMatchChannel = match_chan;

    if (erange.first != -std::numeric_limits<double>::max()) {
      fBinRange.first = fData->GetXaxis()->FindFixBin(erange.first);
      if (fBinRange.first < 1) {
        fBinRange.first = 1;
      }
    } else {
      fBinRange.first = 1;
    }

    if (erange.second != std::numeric_limits<double>::max()) {
      fBinRange.second = fData->GetXaxis()->FindFixBin(erange.second);
      if (fBinRange.second > (fData->GetXaxis()->GetNbins() + 1)) {
        fBinRange.second = (fData->GetXaxis()->GetNbins() + 1);
      }
    } else {
      fBinRange.second = (fData->GetXaxis()->GetNbins() + 1);
    }
  }

  PredictionPRISM const *fPred;
  TH1 const *fData;
  Eigen::VectorXd fData_vec;
  double fPOTFD;
  double fPOTND;
  bool fUseCovariance;

  PRISM::MatchChan fMatchChannel;
  std::pair<int, int> fBinRange;
  // Get the extrapolated PRISM prediction
  Eigen::VectorXd GetPred(osc::IOscCalc *osc,
                const SystShifts &syst = SystShifts::Nominal()) const {
    TH1D *pred_h = fPred->PredictPRISMComponents(osc, syst, fMatchChannel)
                   .at(PredictionPRISM::kNDDataCorr_FDExtrap)
                   .ToTH1(fPOTFD);
    pred_h->SetDirectory(nullptr);
    Eigen::VectorXd pred_vec = GetEigenFlatVector(pred_h);
    HistCache::Delete(pred_h);
    return pred_vec;   
  }
  // Get the PRISM covariance matrix
  Eigen::MatrixXd GetCovariance(osc::IOscCalc *osc,
                                const SystShifts &syst = SystShifts::Nominal()) const {
    TH2D *cov_h = dynamic_cast<TH2D*>(fPred->PredictPRISMComponents(osc, syst, fMatchChannel)
                  .at(PredictionPRISM::kExtrapCovarMatrix)
                  .ToTH2(fPOTND));
    cov_h->Scale(std::pow(fPOTFD/fPOTND, 2));
    cov_h->SetDirectory(nullptr);
    Eigen::MatrixXd cov_mat = GetEigenMatrix(cov_h, 
                                             cov_h->GetYaxis()->GetNbins(),
                                             cov_h->GetXaxis()->GetNbins());
    HistCache::Delete(cov_h);
    return cov_mat;
  }

  // Calculate Chi2 with the option to include the full covariance
  double ChiSq(osc::IOscCalc *osc,
               const SystShifts &syst = SystShifts::Nominal()) const {

    Eigen::VectorXd PredVec = GetPred(osc, syst);
   
    Eigen::MatrixXd CovMat = Eigen::MatrixXd::Zero(PredVec.size(), PredVec.size());
    for (int diag = 0; diag < CovMat.rows(); diag++) {
      // Poisson error on number of events in bin
      CovMat(diag, diag) = PredVec(diag);
    }
    // Add Poisson errors in quadrature to covariance (if you want to)
    if (fUseCovariance) {
      CovMat += GetCovariance(osc, syst);
    }
    double Chi2 = (PredVec - fData_vec).transpose() * CovMat.inverse() *
                  (PredVec - fData_vec);

    return Chi2;
  }
};



} // namespace ana
