#pragma once

#include "CAFAna/Core/SystShifts.h"

#include "CAFAna/Prediction/PredictionInterp.h"

#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"
#include "CAFAna/PRISM/EigenUtils.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

#include <memory>
#include <vector>

#define PRISMDEBUG

#ifdef PRISMDEBUG
#define PRISMOUT(a) std::cout << a << std::endl;
#else
#define PRISMOUT(a)
#endif

namespace ana {

//-----------------------------------------------------
// Class for ND and FD detector extrapolation matrices:
//----------------------------------------------------
// brief / Extrapolation procedure for correcting for 
// ND/FD detector differences. Unfolds and then re-smears
// each slice of off-axis ND data according to the 
// selection efficiency at that position.
class NDFD_Matrix {

public:
  
  NDFD_Matrix(double reg, bool optreg = false);

  NDFD_Matrix(const NDFD_Matrix &MatPreds);
  NDFD_Matrix(NDFD_Matrix &&MatPreds) noexcept;

  ~NDFD_Matrix();

  void Initialize(PredictionInterp const *ND,
                  PredictionInterp const *FD);

  // Normalise the ETrue column to efficiency in ND and FD matrices
  void NormaliseETrue(Eigen::MatrixXd* MatrixND, Eigen::MatrixXd* MatrixFD,
                      std::vector<double> NDefficiency,
                      std::vector<double> FDefficiency) const;


  Eigen::MatrixXd GetNDMatrix() const;
  Eigen::MatrixXd GetFDMatrix() const;

  Eigen::MatrixXd GetNDExtrap_293kA() const;
  Eigen::MatrixXd GetNDExtrap_280kA() const;

  Eigen::MatrixXd GetErrorMat_293kA() const;
  Eigen::MatrixXd GetErrorMat_280kA() const;

  Eigen::MatrixXd GetCovMat_293kA() const;
  Eigen::MatrixXd GetCovMat_280kA() const;

  // Get regularisation matrices
  Eigen::MatrixXd GetL1NormReg(int truebins, TAxis *trueaxis) const;
  Eigen::MatrixXd GetL2NormReg(int truebins, TAxis *trueaxis) const;

  void SetRegFactor(double reg) { fRegFactor = reg; }

  void SetNumuNueCorr(TH1 *h) const { hNumuNueCorr = std::unique_ptr<TH1>(h); }

  // Extrapolate ND PRISM pred to FD using Eigen
  // This function is becoming slightly monsterous...
  void ExtrapolateNDtoFD(ReweightableSpectrum NDDataSpec, 
                         double POT, const int kA, Eigen::ArrayXd&& weights,
                         osc::IOscCalc *calc, ana::SystShifts shift = kNoShift,
                         Flavors::Flavors_t NDflav = Flavors::kAll,
                         Flavors::Flavors_t FDflav = Flavors::kAll,
                         Current::Current_t curr = Current::kCC,
                         Sign::Sign_t NDsign = Sign::kBoth,
                         Sign::Sign_t FDsign = Sign::kBoth,
                         std::vector<std::vector<double>> NDefficiency = {{}},
                         std::vector<double> FDefficiency = {}) const;

  void Write(TDirectory *dir) const;

protected:

  double fRegFactor;
  mutable Eigen::MatrixXd hMatrixND;
  mutable Eigen::MatrixXd hMatrixFD;

  PredictionInterp const *fMatrixND;
  PredictionInterp const *fMatrixFD;
  mutable Eigen::MatrixXd fNDExtrap_293kA;
  mutable Eigen::MatrixXd fNDExtrap_280kA;
  mutable Eigen::MatrixXd fErrorMat_293kA; // Not a covariance matrix, just a map of errors.
  mutable Eigen::MatrixXd fErrorMat_280kA; // Not a covariance matrix, just a map of errors.
  mutable Eigen::MatrixXd hCovMat_293kA;
  mutable Eigen::MatrixXd hCovMat_280kA;
  mutable std::unique_ptr<TH1> hNumuNueCorr;
  mutable bool fOptimizeReg;
};

} // namespace ana
