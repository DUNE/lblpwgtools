#include "CAFAna/Experiment/CovMxChiSq.h"

#include "CAFAna/Core/Utilities.h"

namespace ana
{
  //----------------------------------------------------------------------
  CovMxChiSq::CovMxChiSq(const Eigen::MatrixXd& mat)
    : fCovMxFrac(mat)
  {
  }

  //----------------------------------------------------------------------
  double CovMxChiSq::ChiSq(Eigen::ArrayXd apred, Eigen::ArrayXd adata) const
  {
    const Eigen::MatrixXd covInvM = GetAbsInvCovMat(apred);

    // Now the matrix is in order apply the mask to the two histograms
    ApplyMask(apred, adata);

    // Now it's absolute it's suitable for use in the chisq calculation
    return Chi2CovMx(apred, adata, covInvM);
  }

  //----------------------------------------------------------------------
  Eigen::MatrixXd CovMxChiSq::GetAbsInvCovMat(const Eigen::ArrayXd& apred) const
  {
    const int N = apred.size()-2; // no under/overflow

    // We have to manually add statistical uncertainty in quadrature
    Eigen::MatrixXd cov = fCovMxFrac;
    for(int b = 0; b < N; ++b){
      const double Nevt = apred[b+1];
      if(Nevt > 0) cov(b, b) += 1/Nevt;
    }

    // And then invert
    Eigen::MatrixXd covInv = cov.inverse();

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
