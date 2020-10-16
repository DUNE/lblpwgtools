#pragma once

#include "CAFAna/Experiment/ICovarianceMatrix.h"

namespace ana
{
  class CovMxChiSqPreInvert: public ICovarianceMatrix
  {
  public:
    CovMxChiSqPreInvert(const Eigen::MatrixXd& mat,
                        const Eigen::ArrayXd& pred);

    virtual double ChiSq(Eigen::ArrayXd apred, Eigen::ArrayXd adata) const override;

  protected:
    Eigen::MatrixXd GetAbsInvCovMat(const Eigen::ArrayXd& apred) const;

    Eigen::MatrixXd fCovMxInv;
  };
}
