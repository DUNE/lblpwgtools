#include "CAFAna/Core/PRISMReweightableSpectrum.h"

#include "CAFAna/PRISM/PRISMDetectorExtrapolation.h"

#include "OscLib/IOscCalc.h"

using namespace PRISM;

namespace ana {

//-----------------------------------------------------
// Class for ND and FD detector extrapolation matrices:
// ----------------------------------------------------

NDFD_Matrix::NDFD_Matrix() {
  fMatrixND = nullptr;
  fMatrixFD = nullptr;

  hMatrixND = Eigen::MatrixXd();
  hMatrixFD = Eigen::MatrixXd();
  fNDExtrap_293kA = Eigen::MatrixXd();
  fNDExtrap_280kA = Eigen::MatrixXd();
  fErrorMat_293kA = Eigen::MatrixXd();
  fErrorMat_280kA = Eigen::MatrixXd();
}

//-----------------------------------------------------

NDFD_Matrix::NDFD_Matrix(const NDFD_Matrix &MatPred) {
  fMatrixND = MatPred.fMatrixND;
  fMatrixFD = MatPred.fMatrixFD;

  hMatrixND = MatPred.hMatrixND;
  hMatrixFD = MatPred.hMatrixFD;
  fNDExtrap_293kA = MatPred.fNDExtrap_293kA;
  fNDExtrap_280kA = MatPred.fNDExtrap_280kA;
  fErrorMat_293kA = MatPred.fErrorMat_293kA;
  fErrorMat_280kA = MatPred.fErrorMat_280kA;
  vNumuNueCorr = MatPred.vNumuNueCorr;
} 

//-----------------------------------------------------

NDFD_Matrix::~NDFD_Matrix() {
  fMatrixND = nullptr;
  fMatrixFD = nullptr;
}

//-----------------------------------------------------

void NDFD_Matrix::Initialize(PredictionInterp const *ND,
                             PredictionInterp const *FD) {

  fMatrixND = ND;
  fMatrixFD = FD;
  IsNue = false;
}

//-----------------------------------------------------

Eigen::MatrixXd NDFD_Matrix::GetNDMatrix() const {
  return hMatrixND;
}

//-----------------------------------------------------

Eigen::MatrixXd NDFD_Matrix::GetFDMatrix() const {
  return hMatrixFD;
}

//-----------------------------------------------------

Eigen::MatrixXd NDFD_Matrix::GetNDExtrap_293kA() const {
  return fNDExtrap_293kA;
}
Eigen::MatrixXd NDFD_Matrix::GetNDExtrap_280kA() const {
  return fNDExtrap_280kA;
}

//-----------------------------------------------------

Eigen::MatrixXd NDFD_Matrix::GetErrorMat_293kA() const {
  return fErrorMat_293kA;
}
Eigen::MatrixXd NDFD_Matrix::GetErrorMat_280kA() const { 
  return fErrorMat_280kA;
}

//-----------------------------------------------------

Eigen::MatrixXd NDFD_Matrix::GetCovMat_293kA() const {
  return hCovMat_293kA;
}
Eigen::MatrixXd NDFD_Matrix::GetCovMat_280kA() const {
  return hCovMat_280kA;
}

//-----------------------------------------------------

void NDFD_Matrix::NormaliseETrue(Eigen::MatrixXd* MatrixND, Eigen::MatrixXd* MatrixFD,
                                 std::vector<double> NDefficiency,
                                 std::vector<double> FDefficiency) const {

  if (!MatrixND) {
    std::cout << "[ERROR] No fMatrixND." << std::endl; 
    abort();
  } else if (!MatrixFD) {
    std::cout << "[ERROR] No fMatrixFD." << std::endl; 
    abort();
  } 
 
  std::pair<Eigen::MatrixXd*, std::vector<double>> NDpair (MatrixND, NDefficiency);
  std::pair<Eigen::MatrixXd*, std::vector<double>> FDpair (MatrixFD, FDefficiency);

  std::vector<std::pair<Eigen::MatrixXd*, std::vector<double>>> matrix_pair = {NDpair, FDpair};

  for (auto &mat : matrix_pair) {
    for (int col_it = 1; col_it <= (mat.first->cols() - 2); col_it++) {
      Eigen::VectorXd projY = mat.first->col(col_it);
      if (std::isnormal(projY.sum())) {
        double integral = projY.sum();
        projY *= (1 / integral);
      }
      double eff = mat.second.at(col_it - 1);
      projY *= eff;
      for (int row_it = 1; row_it <= (mat.first->rows() - 2); row_it++) {
        (*mat.first)(row_it, col_it) = projY(row_it); 
      }
    }
  }
}

//-----------------------------------------------------

void NDFD_Matrix::ExtrapolateNDtoFD(PRISMReweightableSpectrum NDDataSpec, 
                                    double POT, const int kA, Eigen::ArrayXd&& weights,
                                    osc::IOscCalc *calc, ana::SystShifts shift,
                                    Flavors::Flavors_t NDflav, 
                                    Flavors::Flavors_t FDflav,
                                    Current::Current_t curr,
                                    Sign::Sign_t NDsign, Sign::Sign_t FDsign,
                                    std::vector<std::vector<double>> NDefficiency,
                                    std::vector<double> FDefficiency) const {
  Eigen::MatrixXd *fNDExtrap, *fErrorMat; 
  if (kA == 293) {
    fNDExtrap = &fNDExtrap_293kA;
    fErrorMat = &fErrorMat_293kA;
  } else if (kA == 280) {
    fNDExtrap = &fNDExtrap_280kA;
    fErrorMat = &fErrorMat_280kA;
  } else {
    std::cout << "[ERROR] Unknown HC." << std::endl;
    abort();
  }
 
  // Do not want to oscillate the MC in the FD matrix (ND is always un-oscillated).
  // The linear combination handles the oscillation, we just want to correct for the
  // different detector resolutions.
  // May need to revisit osc vs. no-osc FD smearing matrices.
  auto sMatrixND = fMatrixND->PredictComponentSyst(calc, shift, NDflav, curr, NDsign);
  hMatrixND = ConvertArrayToMatrix(sMatrixND.GetEigen(POT), sMatrixND.GetBinnings());
  auto sMatrixFD = fMatrixFD->PredictComponentSyst(calc, shift, FDflav, curr, FDsign); 
  hMatrixFD = ConvertArrayToMatrix(sMatrixFD.GetEigen(POT), sMatrixFD.GetBinnings());

  Eigen::MatrixXd PRISMND = NDDataSpec.GetEigen(POT);
  Eigen::MatrixXd PRISMND_block = PRISMND.block(1 ,1 , PRISMND.rows() - 2, PRISMND.cols() - 2); 
  
  Eigen::MatrixXd PRISMND_SumSq = NDDataSpec.GetSumSqEigen(POT);

  fNDExtrap->resize(PRISMND.rows(), PRISMND.cols());
  fErrorMat->resize(PRISMND.rows(), PRISMND.cols()); 

  Eigen::MatrixXd TotalLCCovMat = Eigen::MatrixXd::Zero(hMatrixFD.rows(),
                                                        hMatrixFD.rows());

  // Need a loop to go through each slice of off-axis ND data
  for (int slice = 0; slice < PRISMND_block.rows(); slice++) {
    // Normalise matrices to efficiency for particular OA stop
    NormaliseETrue(&hMatrixND, &hMatrixFD, NDefficiency.at(slice), FDefficiency);

    // Do Linear algebra without under/over-flow bins after normalisation.
    Eigen::MatrixXd MatrixND_block = hMatrixND.block(1, 1, hMatrixND.rows() - 2,
                                                     hMatrixND.cols() - 2);
    Eigen::MatrixXd MatrixFD_block = hMatrixFD.block(1, 1, hMatrixFD.rows() - 2,
                                                     hMatrixFD.cols() - 2);

    // Get a slice of ND data and place it a Eigen Vector
    Eigen::VectorXd NDERec = PRISMND_block.row(slice);

    // Build covariance matrix for this slice so we can propogate uncertainty:
    Eigen::MatrixXd CovMatRec = Eigen::MatrixXd::Zero(NDERec.size(), NDERec.size());
    for (int col = 0; col < CovMatRec.cols(); col++) {
      if (!std::isnormal(PRISMND_SumSq(slice + 1, col + 1))) {
        CovMatRec(col, col) = 1E-10;
      } else { 
        CovMatRec(col, col) = PRISMND_SumSq(slice + 1, col + 1);
      }
    }
    // Should* be fine to take the inverse of a purely diagonal matrix

    Eigen::MatrixXd invCovMatRec = CovMatRec.inverse();

    // Tikhonov regularisation is uneccessary, just least square unfold!
    Eigen::MatrixXd D = (MatrixND_block.transpose() * invCovMatRec * MatrixND_block).inverse() *
                        MatrixND_block.transpose() * invCovMatRec;
 
    Eigen::VectorXd NDETrue = D * NDERec;
    // Correct for nue/numu x-sec differences if doing appearance measurement.
    if (IsNue) { // If we are doing nue appearance...
      for (int bin = 0; bin < NDETrue.size(); bin++) {
        NDETrue(bin) *= vNumuNueCorr(bin + 1);
      }
    } 

    // Cov Mat for true energy, propogated through Tik reg
    Eigen::MatrixXd CovMatTrue = D * CovMatRec * D.transpose();

    Eigen::VectorXd FDERec = MatrixFD_block * NDETrue;
    Eigen::MatrixXd CovMatExtrap = MatrixFD_block * CovMatTrue * MatrixFD_block.transpose();

    // ** Get total covariance matrix of linear combination **
    TotalLCCovMat.block(1, 1, CovMatExtrap.rows(), CovMatExtrap.cols()) += 
        CovMatExtrap * std::pow(weights(slice + 1), 2);

    for (int ebin = 1; ebin <= (fNDExtrap->cols() - 2); ebin++) { 
      (*fNDExtrap)(slice + 1, ebin) = FDERec(ebin - 1);
      double varExtrap = CovMatExtrap(ebin - 1, ebin - 1);
      (*fErrorMat)(slice + 1, ebin) = varExtrap;
    }
  }

  if (kA == 293) {
    hCovMat_293kA = TotalLCCovMat;
  } else if (kA == 280) {
    hCovMat_280kA = TotalLCCovMat;
  }
}

//----------------------------------------------------

void NDFD_Matrix::Write(TDirectory *dir) const {
  Eigen::MatrixXd matND = hMatrixND;
  Eigen::MatrixXd matFD = hMatrixFD;

  osc::NoOscillations no; 
  auto sMND = fMatrixND->Predict(&no);

  std::vector<std::string> labels = sMND.GetLabels();
  std::vector<Binning> bins = sMND.GetBinnings();

  LabelsAndBins reco_axis(labels.at(0), bins.at(0));
  LabelsAndBins true_axis(labels.at(1), bins.at(1));

  ReweightableSpectrum rwND(std::move(matND), reco_axis, true_axis, 1, 0);
  ReweightableSpectrum rwFD(std::move(matFD), reco_axis, true_axis, 1, 0);

  dir->WriteTObject(rwND.ToTH2(1), "ND_SmearingMatrix");
  dir->WriteTObject(rwFD.ToTH2(1), "FD_SmearingMatrix");
}
 
} // namespace ana