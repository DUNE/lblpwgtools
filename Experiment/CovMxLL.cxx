#include "CAFAna/Experiment/CovMxLL.h"

#include "CAFAna/Core/Utilities.h"

namespace ana
{
  //----------------------------------------------------------------------
  CovMxLL::CovMxLL(const Eigen::MatrixXd& mat)
    : fCovMxInv(mat.inverse())
  {
  }

  //----------------------------------------------------------------------
  double CovMxLL::ChiSq(Eigen::ArrayXd apred, Eigen::ArrayXd adata) const
  {
    ApplyMask(apred, adata);

    return LogLikelihoodCovMx(apred, adata, fCovMxInv, &fState);
  }
}
