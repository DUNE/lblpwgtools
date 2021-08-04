#include "CAFAna/PRISM/PRISMUtils.h"

#include "CAFAna/PRISM/PredictionPRISM.h"
#include "CAFAna/PRISM/EigenUtils.h"
#include "CAFAna/PRISM/PRISMDetectorExtrapolation.h"

#include "CAFAna/Analysis/CalcsNuFit.h"
#include "CAFAna/Analysis/common_fit_definitions.h"
#include "CAFAna/Analysis/AnalysisVars.h"

#include "CAFAna/Cuts/TruthCuts.h"

using namespace PRISM;

namespace ana {

//-----------------------------------------------------
// Class for ND and FD detector extrapolation matrices:
// ----------------------------------------------------

NDFD_Matrix::NDFD_Matrix(PredictionInterp const * ND,
                         PredictionInterp const * FD,
                         double reg) : fRegFactor(reg), 
                         fNDExtrap_293kA(nullptr), fNDExtrap_280kA(nullptr),
                         hCovMat_293kA(nullptr), hCovMat_280kA(nullptr), 
                         hETrueUnfold(nullptr), hNumuNueCorr(nullptr), 
                         ETrueWriteOnce(true) {
  fMatrixND = ND;
  fMatrixFD = FD;
  if (!fMatrixND) {
    std::cout << "[WARNING] ND matrix not loaded." << std::endl;
    abort();
  } else if (!fMatrixFD) {
    std::cout << "[WARNING] FD matrix not loaded." << std::endl;
    abort();
  }
}

//-----------------------------------------------------

TH2D * NDFD_Matrix::GetNDMatrix() const {
  return hMatrixND.get();
}

//-----------------------------------------------------

TH2D * NDFD_Matrix::GetFDMatrix() const {
  return hMatrixFD.get();
}

//-----------------------------------------------------

TH2 * NDFD_Matrix::GetNDExtrap_293kA() const {
  return fNDExtrap_293kA.get();
}
TH2 * NDFD_Matrix::GetNDExtrap_280kA() const {
  return fNDExtrap_280kA.get();
}

//-----------------------------------------------------

TH2 * NDFD_Matrix::GetCovMat_293kA() const {
  return hCovMat_293kA.get();
}
TH2 * NDFD_Matrix::GetCovMat_280kA() const {
  return hCovMat_280kA.get();
}

//-----------------------------------------------------

void NDFD_Matrix::NormaliseETrue(std::unique_ptr<TH2D>* MatrixND, std::unique_ptr<TH2D>* MatrixFD,
                                 std::vector<double> NDefficiency,
                                 std::vector<double> FDefficiency) const {

  if (!hMatrixND) {
    std::cout << "[ERROR] No fMatrixND." << std::endl; 
    abort();
  } else if (!hMatrixFD) {
    std::cout << "[ERROR] No fMatrixFD." << std::endl; 
    abort();
  } 
 
  std::pair<std::unique_ptr<TH2D>*, std::vector<double>> NDpair (&hMatrixND, NDefficiency);
  std::pair<std::unique_ptr<TH2D>*, std::vector<double>> FDpair (&hMatrixFD, FDefficiency);

  std::vector<std::pair<std::unique_ptr<TH2D>*, std::vector<double>>> matrix_pair = 
      {NDpair, FDpair};   

  for (auto &mat : matrix_pair) {
    for (int col_it = 1; col_it <= mat.first->get()->GetXaxis()->GetNbins(); col_it++) {

      TH1D *projY = mat.first->get()->ProjectionY("_projY", col_it, col_it, "e");
      // Normalise integral of true bin to efficiency
      projY->Scale(1 / projY->Integral());
      projY->Scale(mat.second.at(col_it - 1)); 
      for (int row_it = 1; row_it <= mat.first->get()->GetYaxis()->GetNbins(); row_it++) {
        mat.first->get()->SetBinContent(col_it, row_it, projY->GetBinContent(row_it));
        mat.first->get()->SetBinError(col_it, row_it, projY->GetBinError(row_it));
      }
      HistCache::Delete(projY);
    }
  }
}

//-----------------------------------------------------

Eigen::MatrixXd NDFD_Matrix::GetL1NormReg(int truebins, TAxis *trueaxis) const {
  Eigen::MatrixXd RegMatrix = Eigen::MatrixXd::Zero(truebins, truebins);
  // Reg matrix with ... 1 -1 ... pattern
  // m cols and m-1 rows
  for (int row_it = 0; row_it < truebins - 1; row_it++) {
    RegMatrix(row_it, row_it) = fRegFactor;
    RegMatrix(row_it, row_it + 1) = -fRegFactor;
  }
  // weight by bin width to account for non-uniform binning
  for (int col_it = 0; col_it < truebins; col_it++) {
    double width = trueaxis->GetBinUpEdge(col_it + 1) - 
                   trueaxis->GetBinLowEdge(col_it + 1);
    for (int row_it = 0; row_it < truebins - 1; row_it++) {
      RegMatrix(row_it, col_it) *= (1/width);
    }
  } 
  return RegMatrix;
}

//-----------------------------------------------------

Eigen::MatrixXd NDFD_Matrix::GetL2NormReg(int truebins, TAxis *trueaxis) const {
  Eigen::MatrixXd RegMatrix = Eigen::MatrixXd::Zero(truebins, truebins);
  // Reg matrix with ... 1 -2 1 ... pattern
  // m cols and m-2 rows
  for (int row_it = 0; row_it < truebins - 2; row_it++) {
    RegMatrix(row_it, row_it) = fRegFactor;
    RegMatrix(row_it, row_it + 1) = (-2 * fRegFactor);
    RegMatrix(row_it, row_it + 2) = fRegFactor; 
  }
  // weight by bin width to account for non-uniform binning
  for (int col_it = 0; col_it < truebins; col_it++) {
    double width = trueaxis->GetBinUpEdge(col_it + 1) -
                   trueaxis->GetBinLowEdge(col_it + 1);
    for (int row_it = 0; row_it < truebins - 2; row_it++) {
      RegMatrix(row_it, col_it) *= (1/width);
    }
  }
  return RegMatrix;
}

//-----------------------------------------------------

void NDFD_Matrix::ExtrapolateNDtoFD(ReweightableSpectrum NDDataSpec, 
                                    double POT, const int kA, const TH1 *weights,
                                    osc::IOscCalculator *calc, ana::SystShifts shift,
                                    Flavors::Flavors_t NDflav, 
                                    Flavors::Flavors_t FDflav,
                                    Current::Current_t curr,
                                    Sign::Sign_t NDsign, Sign::Sign_t FDsign,
                                    std::vector<std::vector<double>> NDefficiency,
                                    std::vector<double> FDefficiency) const {
  std::unique_ptr<TH2>* fNDExtrap;
  
  if (kA == 293) {
    fNDExtrap = &fNDExtrap_293kA;
  } else if (kA == 280) {
    fNDExtrap = &fNDExtrap_280kA;
  } else {
    std::cout << "[ERROR] Unknown HC." << std::endl;
    abort();
  }
  
  auto sMatrixND = fMatrixND->PredictComponentSyst(calc, shift, NDflav, curr, NDsign);
  hMatrixND = std::unique_ptr<TH2D>(static_cast<TH2D*>(sMatrixND.ToTH2(1)));
  auto sMatrixFD = fMatrixFD->PredictComponentSyst(calc, shift, FDflav, curr, FDsign); 
  hMatrixFD = std::unique_ptr<TH2D>(static_cast<TH2D*>(sMatrixFD.ToTH2(1)));

  int NTrueBins = hMatrixND->GetXaxis()->GetNbins();
  // Use T-reg to calculate ETrue(ND)
  // Can use L1 or L2 norm reg here
  Eigen::MatrixXd RegMatrix = GetL1NormReg(NTrueBins, hMatrixND->GetXaxis()); 

  TH2D *PRISMND = static_cast<TH2D*>(NDDataSpec.ToTH2(POT));

  *fNDExtrap = std::unique_ptr<TH2>(static_cast<TH2*>(
                                    HistCache::NewTH2D("NDExtrap",
                                                       hMatrixFD->GetYaxis(), 
                                                       PRISMND->GetYaxis()))); 

  Eigen::MatrixXd TotalLCCovMat = Eigen::MatrixXd::Zero(hMatrixFD->GetYaxis()->GetNbins(),
                                                        hMatrixFD->GetYaxis()->GetNbins());

  // Need a loop to go through each slice of off-axis ND data
  for (int slice = 0; slice < PRISMND->GetYaxis()->GetNbins(); slice++) {
    // Normalise matrices to efficiency for particular OA stop
    NormaliseETrue(&hMatrixND, &hMatrixFD, NDefficiency.at(slice), FDefficiency);

    // Get a slice of ND data and place it a Eigen Vector
    TH1D *SliceProj = PRISMND->ProjectionX("slice", slice + 1, slice + 1, "e");
    Eigen::VectorXd NDERec = GetEigenFlatVector(SliceProj);

    // Build covariance matrix for this slice so we can propogate uncertainty:
    Eigen::MatrixXd CovMatRec = Eigen::MatrixXd::Zero(NDERec.size(), NDERec.size());
    for (int row_it = 0; row_it < CovMatRec.rows(); row_it++) {
      double error(0);
      if (std::isnormal(SliceProj->GetBinContent(row_it + 1))) { // Valid number
        error = SliceProj->GetBinError(row_it + 1);
      } else { // Is zero, need to change it
        error = 1E-5; // Arbitrarily small number to make calculation work. (Ugly.)
      }
      CovMatRec(row_it, row_it) = pow(error, 2);
    }
    // Should* be fine to take the inverse of a purely diagonal matrix
    Eigen::MatrixXd invCovMatRec = CovMatRec.inverse();

    std::unique_ptr<TH2D>* NDhist = &hMatrixND;
    std::unique_ptr<TH2D>* FDhist = &hMatrixFD;

    Eigen::MatrixXd NDmat = GetEigenMatrix(NDhist->get(),
                                           NDhist->get()->GetYaxis()->GetNbins(),
                                           NDhist->get()->GetXaxis()->GetNbins());
    Eigen::MatrixXd FDmat = GetEigenMatrix(FDhist->get(),
                                           FDhist->get()->GetYaxis()->GetNbins(),
                                           FDhist->get()->GetXaxis()->GetNbins());

    // Propogate uncertainty
    Eigen::MatrixXd D = ((NDmat.transpose() * invCovMatRec * NDmat) +
                         RegMatrix.transpose() * RegMatrix).inverse() *
                        NDmat.transpose() * invCovMatRec; 

    // ** WARNING ** If there are a lot of empty bins in ND data (as there can
    // be for 2D preds) then the above equation for D inc. the covariance matrix
    // might not work and you may need to use the below.
    // ------
    /* Eigen::MatrixXd D = ((NDmat.transpose() * NDmat) +
                         RegMatrix.transpose() * RegMatrix).inverse() *
                        NDmat.transpose(); */
    // ------
    
    Eigen::VectorXd NDETrue = D * NDERec;
    // Correct for nue/numu x-sec differences if doing appearance measurement.
    if (hNumuNueCorr) { // If we are doing nue appearance...
      for (int bin = 0; bin < NDETrue.size(); bin++) {
        NDETrue(bin) *= hNumuNueCorr->GetBinContent(bin + 1);
      }
    }

    // Cov Mat for true energy, propogated through Tik reg
    Eigen::MatrixXd CovMatTrue = D * CovMatRec * D.transpose();

    // Solution and residual norm for L-curve
    //std::cout << "regparam = " << fRegFactor << std::endl;
    Eigen::MatrixXd reg_shape_matrix = RegMatrix / fRegFactor;
    double soln_norm = (reg_shape_matrix * NDETrue).norm();
    double resid_norm = (invCovMatRec * (NDmat * NDETrue - NDERec)).norm();
    soln_norm_vector.push_back(soln_norm);
    resid_norm_vector.push_back(resid_norm);
    //std::cout << "resid_norm = " << resid_norm << " ; " << 
    //    "soln_norm = " << soln_norm << std::endl;

    Eigen::VectorXd FDERec = FDmat * NDETrue;

    Eigen::MatrixXd CovMatExtrap = FDmat * CovMatTrue * FDmat.transpose();

    // ** Get total covariance matrix of linear combination **
    TotalLCCovMat += CovMatExtrap * std::pow(weights->GetBinContent(slice + 1), 2);

    for (int ebin = 0; ebin < fNDExtrap->get()->GetXaxis()->GetNbins(); ebin++) {   
      fNDExtrap->get()->SetBinContent(ebin + 1, slice + 1, FDERec(ebin));
      double errorExtrap = pow(CovMatExtrap(ebin, ebin), 0.5); 
      fNDExtrap->get()->SetBinError(ebin + 1, slice + 1, errorExtrap);
    }
    HistCache::Delete(SliceProj);
  }

  // Full covariance matrix from linear combinations
  TAxis *covAxis = hMatrixFD->GetYaxis();
  if (kA == 293) {
    hCovMat_293kA = std::unique_ptr<TH2>(static_cast<TH2*>(
                                         HistCache::NewTH2D("CovMat_293kA", covAxis, covAxis)));
    FillHistFromEigenMatrix(hCovMat_293kA.get(), TotalLCCovMat);
  } else if (kA == 280) {
    hCovMat_280kA = std::unique_ptr<TH2>(static_cast<TH2*>(
                                         HistCache::NewTH2D("CovMat_280kA", covAxis, covAxis)));
    FillHistFromEigenMatrix(hCovMat_280kA.get(), TotalLCCovMat);
  }

  HistCache::Delete(PRISMND);
}

//----------------------------------------------------

void NDFD_Matrix::Write(TDirectory *dir) {
  dir->WriteTObject(hMatrixND.get(), "ND_SmearingMatrix");
  dir->WriteTObject(hMatrixFD.get(), "FD_SmearingMatrix");
  dir->WriteTObject(hETrueUnfold.get(), "Unfolded_ETrue");
  dir->WriteTObject(hCovMat_293kA.get(), "CovMatExtrap_293kA");
  dir->WriteTObject(hCovMat_280kA.get(), "CovMatExtrap_280kA");
}
 
} // namespace ana
