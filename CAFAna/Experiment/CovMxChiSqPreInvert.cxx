#include "CAFAna/Experiment/CovMxChiSqPreInvert.h"

#include "CAFAna/Core/Utilities.h"

namespace ana
{
  //----------------------------------------------------------------------
  CovMxChiSqPreInvert::CovMxChiSqPreInvert(const Eigen::MatrixXd& mat,
                                           const Eigen::ArrayXd& pred)
  {
    fCovMxInv = mat;

    // We add the squared fractional statistical errors to the diagonal. In
    // principle this should vary with the predicted number of events, but in
    // the ND using the no-syst-shifts number should be a pretty good
    // approximation, and it's much faster than needing to re-invert the matrix
    // every time.
    for(int b = 0; b < pred.size(); ++b){
      if(pred[b] > 0) fCovMxInv.coeffRef(b, b) += 1/pred[b];
    }

    fCovMxInv = fCovMxInv.inverse();
  }

  //----------------------------------------------------------------------
  double CovMxChiSqPreInvert::ChiSq(Eigen::ArrayXd apred,
                                    Eigen::ArrayXd adata) const
  {
    Eigen::MatrixXd covInvM = GetAbsInvCovMat(apred);

    // Now the matrix is in order apply the mask to the two histograms
    ApplyMask(apred, adata);

    // Now it's absolute it's suitable for use in the chisq calculation
    return Chi2CovMx(apred, adata, fCovMxInv);
  }

  //----------------------------------------------------------------------
  Eigen::MatrixXd CovMxChiSqPreInvert::GetAbsInvCovMat(const Eigen::ArrayXd& apred) const
  {
    Eigen::MatrixXd covInv = fCovMxInv;

    const int N = apred.size()-2; // no under/overflow

    // Covariance matrix is fractional; convert it to absolute by multiplying
    // out the prediction
    //
    // TODO there is probably a nice Eigen expression that will do this
    for(int b0 = 0; b0 < N; ++b0){
      for(int b1 = 0; b1 < N; ++b1){
        const double f = apred[b0+1] * apred[b1+1];
        if(f != 0) covInv.coeffRef(b0, b1) /= f;
        else covInv.coeffRef(b0, b1) = 0.;
      }
    }

    return covInv;
  }
}
