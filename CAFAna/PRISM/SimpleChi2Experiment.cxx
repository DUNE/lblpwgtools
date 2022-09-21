#include "CAFAna/PRISM/SimpleChi2Experiment.h"

#include "CAFAna/Core/Utilities.h"

#include "OscLib/IOscCalc.h"

namespace ana {

  // Chi2Experiment for Chi2 calculation using full
  // PRISM covariance

  PRISMChi2CovarExperiment::PRISMChi2CovarExperiment(
                           PredictionPRISM const *Pred, Spectrum const &Data,
                           bool UseCovariance, double NDPOT, double FDPOT,
                           PRISM::MatchChan match_chan)
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

  //-------------------------------------------------------
  // Get the extrapolated PRISM prediction
  Eigen::VectorXd PRISMChi2CovarExperiment::
                      GetPred(std::map<PredictionPRISM::PRISMComponent, Spectrum> &PRISMComps,
                              const SystShifts &syst) const {
    
    Eigen::VectorXd pred_v = PRISMComps.at(PredictionPRISM::kNDDataCorr_FDExtrap)
                             .GetEigen(fPOTFD).matrix(); 
    // Chop off the under and over flow bins.
    Eigen::VectorXd pred_v_seg = Eigen::VectorXd::Zero(pred_v.size() - 2);
    pred_v_seg = pred_v.segment(1, pred_v.size() - 2);
    
    return pred_v_seg;   
  }

  //-------------------------------------------------------
  // Get the PRISM covariance matrix
  Eigen::MatrixXd PRISMChi2CovarExperiment::
                      GetCovariance(std::map<PredictionPRISM::PRISMComponent, Spectrum> &PRISMComps,
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

  //-------------------------------------------------------
  // Calculate Chi2 with the option to include the full covariance
  double PRISMChi2CovarExperiment::ChiSq(osc::IOscCalcAdjustable *osc,
                                         const SystShifts &syst) const {

    auto PRISMComp_map = fPred->PredictPRISMComponents(osc, syst, fMatchChannel);

    Eigen::VectorXd PredVec = GetPred(PRISMComp_map, syst);
    Eigen::MatrixXd CovMat = Eigen::MatrixXd::Zero(PredVec.size(), PredVec.size());
    for (int diag = 0; diag < CovMat.rows(); diag++) {
      // Poisson error on number of events in bin
      // CovMat(diag, diag) = PredVec(diag);
      // Test alternative Chi2 covariance from [ref]
      // Nucl. Instrum. Meth. A, vol. 961, p. P163677, 2020.
      CovMat(diag, diag) = 3 / ((1 / fData_vec(diag)) + (2 / PredVec(diag))); 
    }
    // Add Poisson errors in quadrature to covariance (if you want to)
    if (fUseCovariance) {
      CovMat += GetCovariance(PRISMComp_map, syst);
    }
    double Chi2 = (PredVec - fData_vec).transpose() * CovMat.inverse() *
                  (PredVec - fData_vec);

    // For low-stats.
    //double Chi2 = ana::LogLikelihood(PredVec.array(), fData_vec.array());

    return Chi2;
  }

} // namespace ana
