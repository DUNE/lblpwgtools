#pragma once

#include <Eigen/Dense>

namespace ana
{
  class ICovarianceMatrix
  {
  public:
    virtual ~ICovarianceMatrix() {}

    virtual double ChiSq(Eigen::ArrayXd apred, Eigen::ArrayXd adata) const = 0;

    void SetMask(const Eigen::ArrayXd& mask){fMaskA = mask;}

  protected:
    ICovarianceMatrix() {}

    void ApplyMask(Eigen::ArrayXd& a, Eigen::ArrayXd& b) const;

    Eigen::ArrayXd fMaskA;
  };
}
