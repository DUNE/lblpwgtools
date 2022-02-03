#pragma once

#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Experiment/IExperiment.h"

#include "CAFAna/Prediction/IPrediction.h"

#include "CAFAna/PRISM/EigenUtils.h"

namespace ana {
class SimpleChi2Experiment : public IExperiment {
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
    delete PredHist;

    return chi2;
  }
};


//----------------------------------------------------

// Another Chi2Experiment for Chi2 calculation using full
// PRISM covariance

class PRISMChi2CovarExperiment : public IExperiment {
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
    std::unique_ptr<TH1> hData = std::unique_ptr<TH1>(Data.ToTH1(fPOTFD));
    Eigen::VectorXd fData_vecraw = Data.GetEigen(fPOTFD).matrix();
    fData_vec = fData_vecraw.segment(1, fData_vecraw.size() - 2);

    fPOTND = NDPOT;

    fMatchChannel = match_chan;
  }

  PredictionPRISM const *fPred;
  Eigen::VectorXd fData_vec;
  double fPOTFD;
  double fPOTND;
  bool fUseCovariance;

  PRISM::MatchChan fMatchChannel;

  // Get the extrapolated PRISM prediction
  Eigen::VectorXd GetPred(osc::IOscCalc *osc,
                const SystShifts &syst = SystShifts::Nominal()) const {
    Eigen::VectorXd pred_v = fPred->PredictPRISMComponents(osc, syst, fMatchChannel)
                             .at(PredictionPRISM::kNDDataCorr_FDExtrap)
                             .GetEigen(fPOTFD).matrix();
    // Chop off the under and over flow bins.
    Eigen::VectorXd pred_v_seg = pred_v.segment(1, pred_v.size() - 2);
  
    return pred_v_seg;   
  }
  // Get the PRISM covariance matrix
  Eigen::MatrixXd GetCovariance(osc::IOscCalc *osc,
                                const SystShifts &syst = SystShifts::Nominal()) const {
    auto cov_s = fPred->PredictPRISMComponents(osc, syst, fMatchChannel)
                 .at(PredictionPRISM::kExtrapCovarMatrix);

    Eigen::MatrixXd cov_mat = ConvertArrayToMatrix(cov_s.GetEigen(std::pow(fPOTFD/fPOTND, 2)),
                                                   cov_s.GetBinnings());

    // Chop off the under and over flow bins.
    Eigen::MatrixXd cov_mat_block = cov_mat.block(1, 1, cov_mat.rows() - 2, 
                                                  cov_mat.cols() - 2);

    return cov_mat_block;
  }

  // Calculate Chi2 with the option to include the full covariance
  virtual double ChiSq(osc::IOscCalcAdjustable *osc,
                 const SystShifts &syst = kNoShift) const override {

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
