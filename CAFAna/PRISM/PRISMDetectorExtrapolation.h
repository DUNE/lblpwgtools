#pragma once

#include "CAFAna/Core/SystShifts.h"

#include "CAFAna/Prediction/PredictionInterp.h"

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

    NDFD_Matrix();

    NDFD_Matrix(const NDFD_Matrix &MatPreds);

    ~NDFD_Matrix();

    void Initialize(std::pair<PredictionInterp const *, size_t> ND,
                    std::pair<PredictionInterp const *, size_t> FD);

    // Normalise the ETrue column to efficiency in ND and FD matrices
    void NormaliseETrue(Eigen::MatrixXd* MatrixND, Eigen::MatrixXd* MatrixFD,
                        Eigen::ArrayXd const &NDefficiency,
                        Eigen::ArrayXd const &FDefficiency) const;


    Eigen::MatrixXd GetNDMatrix() const;
    Eigen::MatrixXd GetFDMatrix() const;

    Eigen::MatrixXd GetNDExtrap_293kA() const;
    Eigen::MatrixXd GetNDExtrap_280kA() const;

    Eigen::MatrixXd GetErrorMat_293kA() const;
    Eigen::MatrixXd GetErrorMat_280kA() const;

    Eigen::MatrixXd GetCovMat_293kA() const;
    Eigen::MatrixXd GetCovMat_280kA() const;

    void SetNumuNueCorr(const Eigen::ArrayXd&& ratio) const {
      vNumuNueCorr = std::move(ratio);
      IsNue = true;
    }

    void SetNumuNutauCorr(const Eigen::ArrayXd&& ratio) const {
      vNumuNutauCorr = std::move(ratio);
      IsNutau = true;
    }

    // Extrapolate ND PRISM pred to FD using Eigen
    // This function is becoming slightly monsterous...
    void ExtrapolateNDtoFD(PRISMReweightableSpectrum NDDataSpec,
                           double POT, const int kA, Eigen::ArrayXd& weights,
                           osc::IOscCalc *calc, ana::SystShifts shift_nd = kNoShift,
                           ana::SystShifts shift_fd = kNoShift,
                           Flavors::Flavors_t NDflav = Flavors::kAll,
                           Flavors::Flavors_t FDflav = Flavors::kAll,
                           Current::Current_t curr = Current::kCC,
                           Sign::Sign_t NDsign = Sign::kBoth,
                           Sign::Sign_t FDsign = Sign::kBoth,
                           Eigen::ArrayXXd NDefficiency = {{}},
                           Eigen::ArrayXd FDefficiency = {}) const;

    size_t GetNDConfigFromPred(Flavors::Flavors_t NDflav, Sign::Sign_t NDsign,
                               bool is280kA = false) const;

    size_t GetFDConfigFromPred(Flavors::Flavors_t FDflav, Sign::Sign_t FDsign) const;

    void Write(TDirectory *dir, PRISM::MatchChan match_chan) const;

  protected:

    std::vector<PredictionInterp const *> NDPredInterps;
    std::vector<PredictionInterp const *> FDPredInterps;

    mutable bool IsNue;
    mutable bool IsNutau;
    mutable Eigen::MatrixXd hMatrixND;
    mutable Eigen::MatrixXd hMatrixFD;
    mutable Eigen::MatrixXd fNDExtrap_293kA;
    mutable Eigen::MatrixXd fNDExtrap_280kA;
    mutable Eigen::MatrixXd fErrorMat_293kA; // Not a covariance matrix, just a map of sq errors.
    mutable Eigen::MatrixXd fErrorMat_280kA; // Not a covariance matrix, just a map of sq errors.
    mutable Eigen::MatrixXd hCovMat_293kA;
    mutable Eigen::MatrixXd hCovMat_280kA;
    mutable Eigen::ArrayXd vNumuNueCorr;
    mutable Eigen::ArrayXd vNumuNutauCorr;
  };

} // namespace ana
