#pragma once

#include "CAFAnaCore/CAFAna/Core/ReweightableSpectrum.h"
#include "CAFAna/Core/Weight.h"

#include "TDirectory.h"
#include "TH2.h"
#include "TObjString.h"

#include <iostream>

namespace ana {

  /// \brief: Class derived from ReweightableSpectrum which can perform
  /// all the function of ReweightableSpectrum as well as properly 
  /// propagating statistical uncertainties using a matrix of the sqrd errors.
  class PRISMReweightableSpectrum : public ReweightableSpectrum {
  public:
 
    PRISMReweightableSpectrum(SpectrumLoaderBase& loader,
                              const HistAxis& recoAxis,
                              const HistAxis& trueAxis,
                              const Cut& cut,
                              const SystShifts& shift = kNoShift,
                              const Weight& wei = kUnweighted);      

    PRISMReweightableSpectrum(const Eigen::MatrixXd&& mat,
                              const Eigen::MatrixXd&& err_mat,
                              const LabelsAndBins& recoAxis,
                              const LabelsAndBins& trueAxis,
                              double pot, double livetime);

    // Function to weight fMat by some array whilst propagating statistical
    // error correctly.
    Spectrum WeightedByErrors(Eigen::ArrayXd &w_arr) const;

    // Reimplementing these function upsets me, but it will do.
    PRISMReweightableSpectrum& PlusEqualsHelper(const PRISMReweightableSpectrum& rhs, int sign);
    PRISMReweightableSpectrum& operator+=(const PRISMReweightableSpectrum& rhs);
    PRISMReweightableSpectrum operator+(const PRISMReweightableSpectrum& rhs) const;
    PRISMReweightableSpectrum& operator-=(const PRISMReweightableSpectrum& rhs);
    PRISMReweightableSpectrum operator-(const PRISMReweightableSpectrum& rhs) const;

    static std::unique_ptr<PRISMReweightableSpectrum> LoadFrom(TDirectory* dir, 
                                                               const std::string& name);

    // Necessary for getting the POT scaling for PRISM correct.
    void OverridePOT(double newpot) { fPOT = newpot; }

  protected:
    // Matrix of the errors in each element of fMat squared.
    Eigen::MatrixXd fMatSumSq;
  };
}
