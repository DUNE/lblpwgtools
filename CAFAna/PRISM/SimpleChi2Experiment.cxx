#include "CAFAna/PRISM/SimpleChi2Experiment.h"

//#include "CAFAna/PRISM/PredictionPRISM.h"

#include "OscLib/IOscCalc.h"

namespace ana {

  SimpleChi2Experiment::SimpleChi2Experiment(
      IPrediction const *Pred, Spectrum const &Data, bool UseHistError,
      double POT, std::pair<double, double> erange)
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

  TH1D *SimpleChi2Experiment::GetPred(osc::IOscCalcAdjustable *osc, const SystShifts &syst) const {
    return fPred->PredictSyst(osc, syst).ToTH1(fPOT);
  }

  double SimpleChi2Experiment::ChiSq(osc::IOscCalcAdjustable *osc, const SystShifts &syst) const {

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


//----------------------------------------------------

// Another Chi2Experiment for Chi2 calculation using full
// PRISM covariance

  PRISMChi2CovarExperiment::PRISMChi2CovarExperiment(
                           PredictionPRISM const *Pred, Spectrum const &Data,
                           bool UseCovariance, double NDPOT, double FDPOT,
                           PRISM::MatchChan match_chan,
                           std::pair<double, double> erange)
                           : fPred(Pred), fUseCovariance(UseCovariance) {
    fPOTFD = FDPOT;
    if (fPOTFD == 0) {
      fPOTFD = Data.POT();
    }
    Eigen::VectorXd fData_vecraw = Data.GetEigen(fPOTFD).matrix();
    fData_vec = fData_vecraw.segment(1, fData_vecraw.size() - 2);

    fPOTND = NDPOT;

    fMatchChannel = match_chan;
  }

  // Get the extrapolated PRISM prediction
  Eigen::VectorXd PRISMChi2CovarExperiment::
                      GetPred(osc::IOscCalc *osc,
                              std::map<PredictionPRISM::PRISMComponent, Spectrum> &PRISMComps,
                              const SystShifts &syst) const {
    
    Eigen::VectorXd pred_v = PRISMComps.at(PredictionPRISM::kNDDataCorr_FDExtrap)
                             .GetEigen(fPOTFD).matrix();
    // Chop off the under and over flow bins.
    Eigen::VectorXd pred_v_seg = Eigen::VectorXd::Zero(pred_v.size() - 2);
    pred_v_seg = pred_v.segment(1, pred_v.size() - 2);
    
    return pred_v_seg;   
  }

  // Get the PRISM covariance matrix
  Eigen::MatrixXd PRISMChi2CovarExperiment::
                      GetCovariance(osc::IOscCalc *osc,
                                    std::map<PredictionPRISM::PRISMComponent, Spectrum> &PRISMComps,
                                    const SystShifts &syst) const {
    auto cov_s = PRISMComps.at(PredictionPRISM::kExtrapCovarMatrix);

    Eigen::MatrixXd cov_mat = ConvertArrayToMatrix(cov_s.GetEigen(fPOTND),
                                                   cov_s.GetBinnings());
    cov_mat *= std::pow(fPOTFD/fPOTND, 2);

    // Chop off the under and over flow bins.
    Eigen::MatrixXd cov_mat_block = cov_mat.block(1, 1, cov_mat.rows() - 2, 
                                                  cov_mat.cols() - 2);

    return cov_mat_block;
  }

  // Calculate Chi2 with the option to include the full covariance
  double PRISMChi2CovarExperiment::ChiSq(osc::IOscCalcAdjustable *osc,
                                         const SystShifts &syst) const {

    auto PRISMComp_map = fPred->PredictPRISMComponents(osc, syst, fMatchChannel);

    Eigen::VectorXd PredVec = GetPred(osc, PRISMComp_map, syst);
    Eigen::MatrixXd CovMat = Eigen::MatrixXd::Zero(PredVec.size(), PredVec.size());
    for (int diag = 0; diag < CovMat.rows(); diag++) {
      // Poisson error on number of events in bin
      CovMat(diag, diag) = PredVec(diag);
    }
    // Add Poisson errors in quadrature to covariance (if you want to)
    if (fUseCovariance) {
      CovMat += GetCovariance(osc, PRISMComp_map, syst);
    }
    double Chi2 = (PredVec - fData_vec).transpose() * CovMat.inverse() *
                  (PredVec - fData_vec);

    return Chi2;
  }

} // namespace ana
