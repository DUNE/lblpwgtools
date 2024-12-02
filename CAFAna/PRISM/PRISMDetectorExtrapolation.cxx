#include "CAFAna/PRISM/PRISMReweightableSpectrum.h"
#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"
#include "CAFAna/PRISM/PRISMDetectorExtrapolation.h"

//#include "CAFAna/Core/SystShifts.h"
//#include "CAFAna/Systs/AnaSysts.h"

#include "OscLib/IOscCalc.h"

using namespace PRISM;

namespace ana {

  //-----------------------------------------------------
  // Class for ND and FD detector extrapolation matrices:
  // ----------------------------------------------------

  NDFD_Matrix::NDFD_Matrix() {
    for (size_t conf = 0; conf < kNPRISMConfigs; conf++) NDPredInterps.push_back(nullptr);
    for (size_t conf = 0; conf < kNPRISMFDConfigs; conf++) FDPredInterps.push_back(nullptr);

    hMatrixND = Eigen::MatrixXd();
    hMatrixNDEfficiency1 = Eigen::MatrixXd();
    hMatrixFD = Eigen::MatrixXd();
    fNDExtrap_293kA = Eigen::MatrixXd();
    fNDExtrap_280kA = Eigen::MatrixXd();
    fErrorMat_293kA = Eigen::MatrixXd();
    fErrorMat_280kA = Eigen::MatrixXd();
  }

  //-----------------------------------------------------

  NDFD_Matrix::NDFD_Matrix(const NDFD_Matrix &MatPred) {
    for (size_t conf = 0; conf < kNPRISMConfigs; conf++)
      NDPredInterps.at(conf) = MatPred.NDPredInterps.at(conf);
    for (size_t conf = 0; conf < kNPRISMFDConfigs; conf++)
      FDPredInterps.at(conf) = MatPred.FDPredInterps.at(conf);
    hMatrixND = MatPred.hMatrixND;
    hMatrixNDEfficiency1 = MatPred.hMatrixNDEfficiency1;
    hMatrixFD = MatPred.hMatrixFD;
    fNDExtrap_293kA = MatPred.fNDExtrap_293kA;
    fNDExtrap_280kA = MatPred.fNDExtrap_280kA;
    fErrorMat_293kA = MatPred.fErrorMat_293kA;
    fErrorMat_280kA = MatPred.fErrorMat_280kA;
    vNumuNueCorr = MatPred.vNumuNueCorr;
    vNumuNutauCorr = MatPred.vNumuNutauCorr;
  }

  //-----------------------------------------------------

  NDFD_Matrix::~NDFD_Matrix() {
    for (auto &pred : NDPredInterps) pred = nullptr;
    for (auto &pred : FDPredInterps) pred = nullptr;
  }

  //-----------------------------------------------------

  void NDFD_Matrix::Initialize(std::pair<PredictionInterp const *, size_t> ND,
                               std::pair<PredictionInterp const *, size_t> FD) {
    NDPredInterps.at(ND.second) = ND.first;
    FDPredInterps.at(FD.second) = FD.first;
    IsNue = false;
    IsNutau = false;
    reg_param = 0.0001;
  }

  //-----------------------------------------------------

  Eigen::MatrixXd NDFD_Matrix::GetNDMatrix() const {
    return hMatrixND;
  }
  Eigen::MatrixXd NDFD_Matrix::GetNDMatrixFor100perCentEfficiency() const {
    return hMatrixNDEfficiency1;
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
                                   Eigen::ArrayXd const &NDefficiency,
                                   Eigen::ArrayXd const &FDefficiency) const {

    if (!MatrixND) {
      std::cout << "[ERROR] No fMatrixND." << std::endl;
      abort();
    } else if (!MatrixFD) {
      std::cout << "[ERROR] No fMatrixFD." << std::endl;
      abort();
    }

    std::pair<Eigen::MatrixXd*, Eigen::ArrayXd> NDpair (MatrixND, NDefficiency);
    std::pair<Eigen::MatrixXd*, Eigen::ArrayXd> FDpair (MatrixFD, FDefficiency);

    std::vector<std::pair<Eigen::MatrixXd*, Eigen::ArrayXd>> matrix_pair = {NDpair, FDpair};

    for (auto &mat : matrix_pair) {
      for (int col_it = 1; col_it <= (mat.first->cols() - 2); col_it++) {
        Eigen::VectorXd projY = mat.first->col(col_it);
        if (std::isnormal(projY.sum())) {
          double integral = projY.sum();
          projY *= (1 / integral);
        }
        double eff = 0.01;
        if (std::isnormal(mat.second(col_it - 1))) eff = mat.second(col_it - 1);
        projY *= eff;
        for (int row_it = 1; row_it <= (mat.first->rows() - 2); row_it++) {
          (*mat.first)(row_it, col_it) = projY(row_it);
        }
      }
    }
  }


  // normalize to Etrue with 100% efficiency -> to transform from Generated(Etrue) to Generated(Erec)
  void NDFD_Matrix::NormaliseETrue100PerCentEfficiency(Eigen::MatrixXd* MatrixND) const {

    if (!MatrixND) {
      std::cout << "[ERROR] No fMatrixND." << std::endl;
      abort();
    }

    for (int col_it = 1; col_it <= (MatrixND->cols() - 2); col_it++) {
      Eigen::VectorXd projY = MatrixND->col(col_it);
        if (std::isnormal(projY.sum())) {
          double integral = projY.sum();
          projY *= (1 / integral);
        }
        for (int row_it = 1; row_it <= (MatrixND->rows() - 2); row_it++) {
          (*MatrixND)(row_it, col_it) = projY(row_it);
        }
    }

  }

  //-----------------------------------------------------

  void NDFD_Matrix::ExtrapolateNDtoFD(PRISMReweightableSpectrum NDDataSpec,
                                      double POT, const int kA, Eigen::ArrayXd& weights,
                                      osc::IOscCalc *calc, ana::SystShifts shift_nd,
                                      ana::SystShifts shift_fd,
                                      Flavors::Flavors_t NDflav,
                                      Flavors::Flavors_t FDflav,
                                      Current::Current_t curr,
                                      Sign::Sign_t NDsign, Sign::Sign_t FDsign,
                                      Eigen::ArrayXXd NDefficiency,
                                      Eigen::ArrayXd FDefficiency, bool UseNDFDExtrapPred) const {
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

    osc::NoOscillations no;

    //Freeze systematics for unfolding for NDFDExtrapolation-> would eventually have no unfolding(no systs) for NDFDExtrap + NDEffCorr
    if(UseNDFDExtrapPred){
      shift_nd = kNoShift;
      shift_fd = kNoShift;
    }

    // Do not want to oscillate the MC in the FD matrix (ND is always un-oscillated).
    // The linear combination handles the oscillation, we just want to correct for the
    // different detector resolutions.
    // May need to revisit osc vs. no-osc FD smearing matrices.
    auto sMatrixND = NDPredInterps.at(GetNDConfigFromPred(NDflav, NDsign))
                     ->PredictComponentSyst(calc, shift_nd, NDflav, curr, NDsign); // shift_nd
    hMatrixND = ConvertArrayToMatrix(sMatrixND.GetEigen(POT), sMatrixND.GetBinnings());
    hMatrixNDEfficiency1 = ConvertArrayToMatrix(sMatrixND.GetEigen(POT), sMatrixND.GetBinnings());
    
    auto sMatrixFD = FDPredInterps.at(GetFDConfigFromPred(FDflav, FDsign))
                     ->PredictComponentSyst(calc, shift_fd, FDflav, curr, FDsign);
    hMatrixFD = ConvertArrayToMatrix(sMatrixFD.GetEigen(POT), sMatrixFD.GetBinnings());

    Eigen::MatrixXd PRISMND = NDDataSpec.GetEigen(POT);
    Eigen::MatrixXd PRISMND_block = PRISMND.block(1 ,1 , PRISMND.rows() - 2, PRISMND.cols() - 2);

    Eigen::MatrixXd PRISMND_SumSq = NDDataSpec.GetSumSqEigen(POT);
    Eigen::MatrixXd TotalLCCovMat;

    if(UseNDFDExtrapPred){
      fNDExtrap->resize(PRISMND.rows(), hMatrixND.rows()); //FD Energy axis same as hmatixND.rows() for the NDFDExtrapolation from Pred
      fErrorMat->resize(PRISMND.rows(), hMatrixND.rows()); // FD energy axis same as hmatixND.rows() for the NDFDExtrapolation from Pred

      TotalLCCovMat = Eigen::MatrixXd::Zero(hMatrixND.rows(),
                                          hMatrixND.rows()); //for NDFDExtrapolation from Pred

    }
    else{ //standard PRISM NDFD Extrapolation
      fNDExtrap->resize(PRISMND.rows(), hMatrixFD.rows()); // FD energy axis
      fErrorMat->resize(PRISMND.rows(), hMatrixFD.rows()); // FD energy axis

      TotalLCCovMat = Eigen::MatrixXd::Zero(hMatrixFD.rows(),
                                          hMatrixFD.rows());
    }
    //initialize matrices
    fNDExtrap->setZero();
    fErrorMat->setZero();

    auto binnings = NDDataSpec.GetBinnings().at(0);
    auto edges = binnings.Edges();
    std::vector<double> BinWidthVector;
    std::vector<double> BinCentersVector;
    for(int iEdge = 1; iEdge < (int) edges.size(); iEdge++){
      BinWidthVector.push_back(edges[iEdge] - edges[iEdge-1]);
      BinCentersVector.push_back(0.5* (edges[iEdge] - edges[iEdge-1]) + edges[iEdge-1]);
    }

    double AverageBinWidth = 0;
    for(unsigned int iEdge = 0; iEdge<BinWidthVector.size()-1; iEdge++){
      AverageBinWidth += BinWidthVector[iEdge] / BinWidthVector.size();
    }
    //AverageBinWidth = 0.04;

    // Need a loop to go through each slice of off-axis ND data
    for (int slice = 0; slice < PRISMND_block.rows(); slice++) {
      // Normalise matrices to efficiency for particular OA stop
      NormaliseETrue(&hMatrixND, &hMatrixFD, NDefficiency.row(slice), FDefficiency);
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

      // Regularisation matrix for unfolding - this makes unfolding more stable in fits with detector
      // systematics. Use L2 reg, penalising large changes in curvature.
      Eigen::MatrixXd RegMatrix = Eigen::MatrixXd::Zero(MatrixND_block.rows(), MatrixND_block.rows());
      //====Ciaran's reg: first derviative for equal bins
      for (int row_it = 0; row_it < (RegMatrix.rows() - 2); ++row_it) {
        RegMatrix(row_it, row_it) = reg_param;
        RegMatrix(row_it, row_it + 1) = -2 * reg_param;
        RegMatrix(row_it, row_it + 2) = reg_param;
      }
      RegMatrix(RegMatrix.rows() - 2, RegMatrix.rows() - 2) = reg_param;
      RegMatrix(RegMatrix.rows() - 2, RegMatrix.rows() - 1) = -2 * reg_param;
      //====best data-pred match for variable binning (equal bins 0.4-20) - doesn't change for default binning -> uncomment below and comment above if you want this improved unfolding
      //==first derivative regularization for first Etrue bin
      /*for (int row_it = 0; row_it < 1; ++row_it) {
        RegMatrix(row_it, row_it) = -1* reg_param * 1/( (BinCentersVector[row_it+1] - BinCentersVector[row_it])) *AverageBinWidth ;
        RegMatrix(row_it, row_it + 1) = 1 * reg_param * 1/( (BinCentersVector[row_it+1] - BinCentersVector[row_it])) *AverageBinWidth;
      }
      //regularization matrix - 2nd derivative - for variable bin widtths
      for (int row_it = 1; row_it < (RegMatrix.rows() - 2); ++row_it) {
        RegMatrix(row_it, row_it) = reg_param * pow(AverageBinWidth,2) * 1/ BinWidthVector[row_it] *
                       ( 1/( (BinCentersVector[row_it+1] - BinCentersVector[row_it]) * (BinCentersVector[row_it+1] - BinCentersVector[row_it] + BinCentersVector[row_it+2] -BinCentersVector[row_it+1] ) ) );
        RegMatrix(row_it, row_it + 1) = -1 * reg_param * pow(AverageBinWidth,2) * 1/ BinWidthVector[row_it+1] *
                       1/( (BinCentersVector[row_it+1] - BinCentersVector[row_it]) * (BinCentersVector[row_it+2] - BinCentersVector[row_it+1]) ) ;

        RegMatrix(row_it, row_it + 2) = reg_param * pow(AverageBinWidth,2) * 1/ BinWidthVector[row_it+2] *
                       1 / ( (BinCentersVector[row_it+2] - BinCentersVector[row_it+1]) * (BinCentersVector[row_it+2] - BinCentersVector[row_it+1] + BinCentersVector[row_it+1] - BinCentersVector[row_it] )  );
      }*/
      // Final two rows are zero when doing curvature reg.
      Eigen::MatrixXd D = (MatrixND_block.transpose() * MatrixND_block + 
                           RegMatrix.transpose() * RegMatrix).inverse() * 
                          MatrixND_block.transpose();

      Eigen::VectorXd NDETrue = D * NDERec; //Generated Events Vs Etrue
      hArrayGenNDETrue = NDETrue.array();

      //normalize smear mat ND to ETrue(efficieny = 1)
      NormaliseETrue100PerCentEfficiency(&hMatrixNDEfficiency1);
      Eigen::MatrixXd MatrixNDForEfficinecyOne_block = hMatrixNDEfficiency1.block(1, 1, hMatrixNDEfficiency1.rows() - 2,
                                                                             hMatrixNDEfficiency1.cols() - 2);


      //get GenEvents Vs NDErec -> for this one needs efficiency =1
      Eigen::VectorXd GenNDErec =  MatrixNDForEfficinecyOne_block * NDETrue;
      hArrayGenNDErec = GenNDErec.array();
      // Correct for nue/numu x-sec differences if doing appearance measurement.
      if (IsNue) { // If we are doing nue appearance...
        for (int bin = 0; bin < NDETrue.size(); bin++) {
          NDETrue(bin) *= vNumuNueCorr(bin + 1);
        }
      }
      // Correct for nutau/numu x-sec differences if doing appearance measurement.
      if (IsNutau) { // If we are doing nutau appearance...
        for (int bin = 0; bin < NDETrue.size(); bin++) {
          NDETrue(bin) *= vNumuNutauCorr(bin + 1);
        }
      }

      // Cov Mat for true energy, propogated through unfold.
      Eigen::MatrixXd CovMatTrue = D * CovMatRec * D.transpose();

      Eigen::VectorXd FDERec = MatrixFD_block * NDETrue;

      Eigen::MatrixXd CovMatExtrap;
      if(UseNDFDExtrapPred){
        CovMatExtrap = MatrixNDForEfficinecyOne_block  * CovMatTrue * MatrixNDForEfficinecyOne_block.transpose(); //for NDFDExtrapolation from Pred
      }
      else{
        CovMatExtrap = MatrixFD_block * CovMatTrue * MatrixFD_block.transpose();
      }

      // ** Get total covariance matrix of linear combination **
      TotalLCCovMat.block(1, 1, CovMatExtrap.rows(), CovMatExtrap.cols()) +=
          CovMatExtrap * std::pow(weights(slice + 1), 2);

      for (int ebin = 1; ebin <= (fNDExtrap->cols() - 2); ebin++) {
        if(UseNDFDExtrapPred){
	  (*fNDExtrap)(slice + 1, ebin) = GenNDErec(ebin - 1); //for NDFDExtrapolation from Pred
          (*fNDExtrap)(slice + 1, ebin) *= FDefficiency(ebin - 1);
        }
        else{
          (*fNDExtrap)(slice + 1, ebin) = FDERec(ebin - 1);
        }
        double varExtrap = CovMatExtrap(ebin - 1, ebin - 1);
        (*fErrorMat)(slice + 1, ebin) = varExtrap;
      }

    }

    if (kA == 293) {
      hCovMat_293kA = TotalLCCovMat;
    } else if (kA == 280) {
      hCovMat_280kA = TotalLCCovMat;
    }
 // std::cout<<" end of NDFDExtrapolation "<<std::endl;
  }

  //----------------------------------------------------

  size_t NDFD_Matrix::GetNDConfigFromPred(Flavors::Flavors_t NDflav, Sign::Sign_t NDsign,
                                          bool is280kA) const {
    size_t conf;
    assert(NDflav == Flavors::kAllNuMu); // Only considering numu at ND.
    if (!is280kA) conf = (NDsign == Sign::kNu) ? kND_293kA_nu : kND_293kA_nub;
    else conf = (NDsign == Sign::kNu) ? kND_280kA_nu : kND_280kA_nub;
    return conf;
  }

  //----------------------------------------------------

  size_t NDFD_Matrix::GetFDConfigFromPred(Flavors::Flavors_t FDflav, Sign::Sign_t FDsign) const {
    size_t conf;
    if (FDflav == Flavors::kNuMuToNuMu) {
      conf = (FDsign == Sign::kNu) ? kFD_nu_numu : kFD_nub_numu;
    } else if (FDflav == Flavors::kNuMuToNuE) {
      conf = (FDsign == Sign::kNu) ? kFD_nu_nue : kFD_nub_nue;
    } else if (FDflav == Flavors::kNuMuToNuTau) {
      conf = (FDsign == Sign::kNu) ? kFD_nu_nutau : kFD_nub_nutau;
    } else { abort(); }

    return GetFDConfig(conf);
  }

  //----------------------------------------------------

  void NDFD_Matrix::Write(TDirectory *dir, PRISM::MatchChan match_chan) const {
    Eigen::MatrixXd matND = hMatrixND;
    Eigen::MatrixXd matFD = hMatrixFD;
    Eigen::MatrixXd matNDEff1 = hMatrixNDEfficiency1;

    // Sort out the flavors and signs
    auto NDSigFlavor = (match_chan.from.chan & NuChan::kNumuNumuBar)
                           ? Flavors::kAllNuMu
                           : Flavors::kAllNuE;
    auto NDSigSign = ((match_chan.from.chan & NuChan::kNumu) ||
                      (match_chan.from.chan & NuChan::kNue))
                         ? Sign::kNu
                         : Sign::kAntiNu;
    auto FDSigFlavor = (match_chan.to.chan & NuChan::kNumuNumuBar)
                          ? Flavors::kNuMuToNuMu
                          : ( (match_chan.to.chan & NuChan::kNueNueBar) ? Flavors::kNuMuToNuE : Flavors::kNuMuToNuTau);
    auto FDSigSign = ((match_chan.to.chan & NuChan::kNumu) ||
                     (match_chan.to.chan & NuChan::kNue)  ||
                     (match_chan.to.chan & NuChan::kNutau))
                        ? Sign::kNu
                        : Sign::kAntiNu;
    size_t confnd = GetNDConfigFromPred(NDSigFlavor, NDSigSign);
    size_t conf = GetFDConfigFromPred(FDSigFlavor, FDSigSign);

    osc::NoOscillations no;
 
    auto sMND = NDPredInterps.at(confnd)->Predict(&no);
    auto sMFD = FDPredInterps.at(conf)->Predict(&no);

    std::vector<std::string> labelsND = sMND.GetLabels();
    std::vector<Binning> binsND = sMND.GetBinnings();

    std::vector<std::string> labelsFD = sMFD.GetLabels();
    std::vector<Binning> binsFD = sMFD.GetBinnings();

    LabelsAndBins reco_axisND(labelsND.at(0), binsND.at(0));
    LabelsAndBins true_axisND(labelsND.at(1), binsND.at(1));

    LabelsAndBins reco_axisFD(labelsFD.at(0), binsFD.at(0));
    LabelsAndBins true_axisFD(labelsFD.at(1), binsFD.at(1));

    Spectrum GenETrueSpectrum(std::move(hArrayGenNDETrue), reco_axisND, 1, 0);
    Spectrum GenErecSpectrum(std::move(hArrayGenNDErec), reco_axisND, 1, 0);

    ReweightableSpectrum rwND(std::move(matND), reco_axisND, true_axisND, 1, 0);
    ReweightableSpectrum rwNDEff1(std::move(matNDEff1), reco_axisND, true_axisND, 1, 0);
    ReweightableSpectrum rwFD(std::move(matFD), reco_axisFD, true_axisFD, 1, 0);

    dir->WriteTObject(rwND.ToTH2(1), "ND_SmearingMatrix");
    dir->WriteTObject(rwNDEff1.ToTH2(1), "ND_SmearingMatrixEff1");
    dir->WriteTObject(rwFD.ToTH2(1), "FD_SmearingMatrix");
    dir->WriteTObject(GenETrueSpectrum.ToTH1(1), "GenNDETrue");
    dir->WriteTObject(GenErecSpectrum.ToTH1(1), "GenNDErec");
  }

} // namespace ana
