#pragma once

#include "CAFAna/Experiment/ICovarianceMatrix.h"

#include <vector>

namespace ana
{
  class CovMxLL: public ICovarianceMatrix
  {
  public:
    CovMxLL(const Eigen::MatrixXd& mat);

    double ChiSq(Eigen::ArrayXd apred, Eigen::ArrayXd adata) const override;

  protected:
    Eigen::MatrixXd fCovMxInv;

    mutable std::vector<double> fState;
  };
}
