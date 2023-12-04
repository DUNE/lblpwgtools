#pragma once

#include "CAFAna/Experiment/ICovarianceMatrix.h"

namespace ana
{
  class CovMxChiSq: public ICovarianceMatrix
  {
  public:
    CovMxChiSq(const Eigen::MatrixXd& mat);

    double ChiSq(Eigen::ArrayXd apred, Eigen::ArrayXd adata) const override;

  protected:
    Eigen::MatrixXd GetAbsInvCovMat(const Eigen::ArrayXd& apred) const;

    Eigen::MatrixXd fCovMxFrac;
  };
}
