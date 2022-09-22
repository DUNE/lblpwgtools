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
                           : fPred(Pred), fData(Data), fUseCovariance(UseCovariance) {
    fPOTFD = FDPOT;
    if (fPOTFD == 0) {
      fPOTFD = Data.POT();
    }

    fPOTND = NDPOT;

    fMatchChannel = match_chan;
  }

  //-------------------------------------------------------
  // Get the extrapolated PRISM prediction
  Eigen::ArrayXd PRISMChi2CovarExperiment::GetPred(
      std::map<PredictionPRISM::PRISMComponent, Spectrum> &PRISMComps) const {
    
    Eigen::ArrayXd pred_arr = PRISMComps.at(PredictionPRISM::kNDDataCorr_FDExtrap)
                              .GetEigen(fPOTFD); 
    // Chop off the under and over flow bins.
    //Eigen::ArrayXd pred_v_seg = Eigen::ArrayXd::Zero(pred_v.size() - 2);
    //pred_v_seg = pred_v.segment(1, pred_v.size() - 2);
    return pred_arr;
    //return pred_v_seg;   
  }

  //-------------------------------------------------------
  // Get the PRISM covariance matrix
  Eigen::ArrayXd PRISMChi2CovarExperiment::GetCovariance(
      std::map<PredictionPRISM::PRISMComponent, Spectrum> &PRISMComps) const {
    auto cov_s = PRISMComps.at(PredictionPRISM::kExtrapCovarMatrix);

    //Eigen::ArrayXXd cov_mat = ConvertArrayTo2DArray(cov_s.GetEigen(fPOTND), 
    //                                                cov_s.GetBinnings());
    Eigen::ArrayXd cov_mat = cov_s.GetEigen(fPOTND);
    cov_mat *= std::pow(fPOTFD/fPOTND, 2);

    // Chop off the under and over flow bins.
    //Eigen::MatrixXd cov_mat_block = cov_mat.block(1, 1, cov_mat.rows() - 2, 
    //                                              cov_mat.cols() - 2);
    return cov_mat;
    //return cov_mat_block;
  }

  //-------------------------------------------------------
  Eigen::ArrayXd PRISMChi2CovarExperiment::Concatenate(const std::vector<Eigen::ArrayXd>& arrs) {
    // Drop under/overflow from each argument, but include dummy
    // under/overflows as expected by fCov
    int N = 2;
    for(const Eigen::ArrayXd& arr: arrs) N += arr.size()-2;
    Eigen::ArrayXd ret(N);
    ret[0] = 0;
    ret[N-1] = 0;

    double* p = &ret.data()[1]; // start writing at first non-underflow bin

    for(const Eigen::ArrayXd& arr: arrs){
      for(int i = 1; i < arr.size()-1; ++i) *p++ = arr[i];
    }

    return ret;
  }

  //-------------------------------------------------------
  void PRISMChi2CovarExperiment::SetMaskArray(double xmin, double xmax, 
                                              double ymin, double ymax) {
    fMaskA = GetMaskArray(fData, xmin, xmax, ymin, ymax);
    fMaskCov = Concatenate({fMaskA, fMaskA});
  }

  //-------------------------------------------------------
  void PRISMChi2CovarExperiment::ApplyMask(Eigen::ArrayXd& a,
                                           Eigen::ArrayXd& b,
                                           Eigen::ArrayXd& cov) const {
    if (fMaskA.size() == 0) return;
    if (fMaskCov.size() == 0) return;

    a *= fMaskA;
    b *= fMaskA;
    cov *= fMaskCov;
  }

  //-------------------------------------------------------
  // Calculate Chi2 with the option to include the full covariance
  double PRISMChi2CovarExperiment::ChiSq(osc::IOscCalcAdjustable *osc,
                                         const SystShifts &syst) const {

    auto PRISMComp_map = fPred->PredictPRISMComponents(osc, syst, fMatchChannel);

    Eigen::ArrayXd pred = GetPred(PRISMComp_map);
    Eigen::ArrayXd data = fData.GetEigen(fPOTFD);
    
    //Eigen::ArrayXXd CovMat = Eigen::ArrayXXd::Zero(data.size(), data.size());
    Eigen::ArrayXd CovMat = Eigen::ArrayXd::Zero(((data.size() - 2) * (data.size() - 2)) + 2);
    //for (int diag = 0; diag < CovMat.rows(); diag++) {
    for (int x = 1; x < data.size() - 2; x++) {
      //for (int y = 1; y < data.size() - 2; y++) {
      // Poisson error on number of events in bin
      // CovMat(diag, diag) = PredVec(diag);
      // Test alternative Chi2 covariance from [ref]
      // Nucl. Instrum. Meth. A, vol. 961, p. P163677, 2020.
      //CovMat(diag, diag) = 3 / ((1 / fData_vec(diag)) + (2 / PredVec(diag))); 
      //CovMat(y + (x - 1) * data.size()) = 3 / ((1 / data(x)) + (2 / pred(x)));
      CovMat(x * x) = 3 / ((1 / data(x)) + (2 / pred(x)));
    }
    // Add Poisson errors in quadrature to covariance (if you want to)
    if (fUseCovariance) {
      CovMat += GetCovariance(PRISMComp_map);
    }

    ApplyMask(pred, data, CovMat);

    Eigen::MatrixXd CovMat_mat = 
        ConvertArrayToMatrix(CovMat,
                             PRISMComp_map.at(PredictionPRISM::kExtrapCovarMatrix).GetBinnings());

    //double Chi2 = (PredVec - fData_vec).transpose() * CovMat.inverse() *
    //              (PredVec - fData_vec);
    double Chi2 = (pred.matrix() = data.matrix()).transpose() * CovMat_mat.inverse() *
                  (pred.matrix() = data.matrix());

    // For low-stats.
    //double Chi2 = ana::LogLikelihood(PredVec.array(), fData_vec.array());

    return Chi2;
  }

} // namespace ana
