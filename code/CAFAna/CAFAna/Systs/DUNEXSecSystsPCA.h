#pragma once

#include "CAFAna/Systs/DUNEXSecSysts.h"

namespace ana
{
  /// These systs should be orthogonal, ie no need for a correlation matrix
  class DUNEXSecSystPCA: public ISyst
  {
  public:
    virtual void Shift(double sigma,
                       Restorer& restore,
                       caf::StandardRecord* sr,
                       double& weight) const override;

  protected:
    friend const DUNEXSecSystPCA* GetDUNEXSecSystPCA(unsigned int);

    DUNEXSecSystPCA(int idx);

    int fIdx;
    double fCoeffs[nuenumu_dummy];
  };

  // Get a specific syst (lower number = more significant component)
  const DUNEXSecSystPCA* GetDUNEXSecSystPCA(unsigned int i);
  // Get the whole list up to some index
  SystVector<DUNEXSecSystPCA> GetDUNEXSecSystsPCA(int N = nuenumu_dummy);
}
