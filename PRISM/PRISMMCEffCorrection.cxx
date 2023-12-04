#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PRISMMCEffCorrection.h"
#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"

using namespace PRISM;

namespace ana {

  //----------------------------------------------------
  // Class for ND and FD MC based efficiency correction:
  //----------------------------------------------------

  MCEffCorrection::MCEffCorrection() {
    // Fill PredInterp vectors with nulls.
    for (size_t conf = 0; conf < kNPRISMConfigs; conf++) {
      NDUnselPredInterps.push_back(nullptr);
      NDSelPredInterps.push_back(nullptr);
    }
    for (size_t conf = 0; conf < kNPRISMFDConfigs; conf++) {
      FDUnselPredInterps.push_back(nullptr);
      FDSelPredInterps.push_back(nullptr);
    }
    hNDUnselected_293kA = nullptr;
    hNDSelected_293kA = nullptr;
  }


  //----------------------------------------------------

  MCEffCorrection::MCEffCorrection(const MCEffCorrection &EffCorr) {
    for (size_t conf = 0; conf < kNPRISMConfigs; conf++) {
      NDUnselPredInterps.at(conf) = EffCorr.NDUnselPredInterps.at(conf);
      NDSelPredInterps.at(conf) = EffCorr.NDSelPredInterps.at(conf);
    }
    for (size_t conf = 0; conf < kNPRISMFDConfigs; conf++) {
      FDUnselPredInterps.at(conf) = EffCorr.FDUnselPredInterps.at(conf);
      FDSelPredInterps.at(conf) = EffCorr.FDSelPredInterps.at(conf);
    }
  }

  //----------------------------------------------------

  MCEffCorrection::~MCEffCorrection() {
    for (auto &pred : NDUnselPredInterps) pred = nullptr;
    for (auto &pred : NDSelPredInterps) pred = nullptr;
    for (auto &pred : FDUnselPredInterps) pred = nullptr;
    for (auto &pred : FDSelPredInterps) pred = nullptr;
  }

  //----------------------------------------------------

  void MCEffCorrection::Initialize(std::pair<PredictionInterp const *, size_t> NDunsel_293kA,
                                   std::pair<PredictionInterp const *, size_t> NDsel_293kA,
                                   std::pair<PredictionInterp const *, size_t> NDunsel_280kA,
                                   std::pair<PredictionInterp const *, size_t> NDsel_280kA,
                                   std::pair<PredictionInterp const *, size_t> FDunsel,
                                   std::pair<PredictionInterp const *, size_t> FDsel) {

    NDUnselPredInterps.at(NDunsel_293kA.second) = NDunsel_293kA.first;
    NDUnselPredInterps.at(NDunsel_280kA.second) = NDunsel_280kA.first;
    NDSelPredInterps.at(NDsel_293kA.second) = NDsel_293kA.first;
    NDSelPredInterps.at(NDsel_280kA.second) = NDsel_280kA.first;

    FDUnselPredInterps.at(FDunsel.second) = FDunsel.first;
    FDSelPredInterps.at(FDsel.second) = FDsel.first;

    osc::NoOscillations no;
    auto NDPred_293kA = NDunsel_293kA.first->Predict(&no);
    auto NDPred_280kA = NDunsel_280kA.first->Predict(&no);

    std::vector<Binning> bins_293kA = NDPred_293kA.GetBinnings();
    int NAnaElements, NOAElements_293kA;
    if (NDPred_293kA.NDimensions() == 2) {
      NAnaElements = bins_293kA.at(0).NBins();
      NOAElements_293kA = bins_293kA.at(1).NBins();
    } else { // is 3D pred
      NAnaElements = bins_293kA.at(0).NBins() * bins_293kA.at(1).NBins();
      NOAElements_293kA = bins_293kA.at(2).NBins();
    }
    std::vector<Binning> bins_280kA = NDPred_280kA.GetBinnings();
    int NOAElements_280kA;
    if (NDPred_280kA.NDimensions() == 2) {
      NOAElements_280kA = bins_280kA.at(1).NBins();
    } else {
      NOAElements_280kA = bins_280kA.at(2).NBins();
    }

    NDefficiency_293kA = Eigen::ArrayXXd::Zero(NOAElements_293kA, NAnaElements);
    NDefficiency_280kA = Eigen::ArrayXXd::Zero(NOAElements_280kA, NAnaElements);

    FDefficiency = Eigen::ArrayXd::Zero(NAnaElements);
  }

  //----------------------------------------------------

  void MCEffCorrection::CalcEfficiency(osc::IOscCalc *calc,
                                       HistAxis const &axis,
                                       ana::SystShifts shift_nd,
                                       ana::SystShifts shift_fd,
                                       Flavors::Flavors_t NDflav,
                                       Flavors::Flavors_t FDflav,
                                       Current::Current_t curr,
                                       Sign::Sign_t NDsign,
                                       Sign::Sign_t FDsign) const {

    osc::NoOscillations no;

    Spectrum sNDunselected_293kA =
        NDUnselPredInterps.at(GetNDConfigFromPred(NDflav, NDsign, false))
        ->PredictComponentSyst(&no, shift_nd, NDflav, curr, NDsign); // shift_nd
    Spectrum sNDunselected_280kA =
        NDUnselPredInterps.at(GetNDConfigFromPred(NDflav, NDsign, true))
        ->PredictComponentSyst(&no, shift_nd, NDflav, curr, NDsign); // shift_nd

    // Analysis axis could be 2D, so put into RWSpec so we can have it projected into 1D trueaxis
    Eigen::ArrayXXd NDunsel_293kA = ConvertArrayToMatrix(sNDunselected_293kA.GetEigen(1),
                                                         sNDunselected_293kA.GetBinnings())
                                                        .array()
                                                        .block(1, 1, NDefficiency_293kA.rows(),
                                                               NDefficiency_293kA.cols());
    Eigen::ArrayXXd NDunsel_280kA = ConvertArrayToMatrix(sNDunselected_280kA.GetEigen(1),
                                                         sNDunselected_280kA.GetBinnings())
                                                        .array()
                                                        .block(1, 1, NDefficiency_280kA.rows(),
                                                               NDefficiency_280kA.cols());

    // Selected ND MC
    Spectrum sNDselected_293kA =
        NDSelPredInterps.at(GetNDConfigFromPred(NDflav, NDsign, false))
        ->PredictComponentSyst(&no, shift_nd, NDflav, curr, NDsign); // shift_nd
    Spectrum sNDselected_280kA =
         NDSelPredInterps.at(GetNDConfigFromPred(NDflav, NDsign, true))
         ->PredictComponentSyst(&no, shift_nd, NDflav, curr, NDsign); // shift_nd

    Eigen::ArrayXXd NDsel_293kA = ConvertArrayToMatrix(sNDselected_293kA.GetEigen(1),
                                                       sNDselected_293kA.GetBinnings())
                                                      .array()
                                                      .block(1, 1, NDefficiency_293kA.rows(),
                                                             NDefficiency_293kA.cols());
    Eigen::ArrayXXd NDsel_280kA = ConvertArrayToMatrix(sNDselected_280kA.GetEigen(1),
                                                       sNDselected_280kA.GetBinnings())
                                                      .array()
                                                      .block(1, 1, NDefficiency_280kA.rows(),
                                                             NDefficiency_280kA.cols());

    Spectrum sFDunselected =
        FDUnselPredInterps.at(GetFDConfigFromPred(FDflav, FDsign))
        ->PredictComponentSyst(calc, shift_fd, FDflav, curr, FDsign); // shift_fd
    Spectrum sFDselected =
        FDSelPredInterps.at(GetFDConfigFromPred(FDflav, FDsign))
        ->PredictComponentSyst(calc, shift_fd, FDflav, curr, FDsign); // shift_fd

    Eigen::ArrayXd vFDunselected = sFDunselected.GetEigen(1)
                                   .segment(1, FDefficiency.size());
    Eigen::ArrayXd vFDselected = sFDselected.GetEigen(1)
                                 .segment(1, FDefficiency.size());

    // Calculate ND efficiency
    // efficiency fluctuates slightly with OA position
    // Use coefficient-wise opterations of Array objects.
    // Efficiency for 293kA sample:
    NDefficiency_293kA = NDsel_293kA / NDunsel_293kA;
    // Efficiency for 280kA sample:
    NDefficiency_280kA = NDsel_280kA / NDunsel_280kA;
    // Calculate FD efficiency
    FDefficiency = vFDselected / vFDunselected;
  }

  //----------------------------------------------------------
  size_t MCEffCorrection::GetNDConfigFromPred(Flavors::Flavors_t NDflav, Sign::Sign_t NDsign,
                                           bool is280kA) const {
    size_t conf;
    assert(NDflav == Flavors::kAllNuMu); // Only considering numu at ND.
    if (!is280kA) conf = (NDsign == Sign::kNu) ? kND_293kA_nu : kND_293kA_nub;
    else conf = (NDsign == Sign::kNu) ? kND_280kA_nu : kND_280kA_nub;
    return conf;
  }

  size_t MCEffCorrection::GetFDConfigFromPred(Flavors::Flavors_t FDflav, Sign::Sign_t FDsign) const {
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

  //-----------------------------------------------------------

  void MCEffCorrection::Write(TDirectory *dir) const {
    // Don't care how ugly this is, just getting the plots.
    // Just to get the axes.
    osc::NoOscillations no;
    size_t conf = kND_293kA_nu;
    if (!NDUnselPredInterps.at(conf)) conf = kND_293kA_nub;
    if (!NDUnselPredInterps.at(conf)) {
      std::cout << "[ERROR] Cannot write efficiency." << std::endl;
      abort();
    }
    auto NDPred = NDUnselPredInterps.at(conf)->Predict(&no);

    size_t conf_280kA = kND_280kA_nu;
    if (!NDUnselPredInterps.at(conf_280kA)) conf_280kA = kND_280kA_nub;
    if (!NDUnselPredInterps.at(conf_280kA)) {
      std::cout << "[ERROR] Cannot write efficiency." << std::endl;
      abort();
    }
    auto NDPred_280kA = NDUnselPredInterps.at(conf_280kA)->Predict(&no);

    std::vector<std::string> anaLabels = { NDPred.GetLabels().at(0) };
    std::vector<Binning> anaBins = { NDPred.GetBinnings().at(0) };
    if (NDPred.GetBinnings().size() == 3) {
      anaLabels.push_back(NDPred.GetLabels().at(1));
      anaBins.push_back(NDPred.GetBinnings().at(1));
    }
    LabelsAndBins ana_axis(anaLabels, anaBins);

    LabelsAndBins oa_axis = (NDPred.GetBinnings().size() == 2) ?
      LabelsAndBins(NDPred.GetLabels().at(1), NDPred.GetBinnings().at(1)) :
      LabelsAndBins(NDPred.GetLabels().at(2), NDPred.GetBinnings().at(2));
    LabelsAndBins oa280kA_axis = (NDPred_280kA.GetBinnings().size() == 2) ?
      LabelsAndBins(NDPred_280kA.GetLabels().at(1), NDPred_280kA.GetBinnings().at(1)) :
      LabelsAndBins(NDPred_280kA.GetLabels().at(2), NDPred_280kA.GetBinnings().at(2));

    Eigen::MatrixXd NDEff293kA_mat = Eigen::MatrixXd::Zero(NDefficiency_293kA.rows() + 2,
                                                           NDefficiency_293kA.cols() + 2);
    NDEff293kA_mat.block(1, 1, NDefficiency_293kA.rows(),
                         NDefficiency_293kA.cols()) = NDefficiency_293kA.matrix();

    ReweightableSpectrum NDEff293kA_rws(std::move(NDEff293kA_mat), ana_axis, oa_axis, 1, 0);

    Eigen::MatrixXd NDEff280kA_mat = Eigen::MatrixXd::Zero(NDefficiency_280kA.rows() + 2,
                                                           NDefficiency_280kA.cols() + 2);
    NDEff280kA_mat.block(1, 1, NDefficiency_280kA.rows(),
                         NDefficiency_280kA.cols()) = NDefficiency_280kA.matrix();

    ReweightableSpectrum NDEff280kA_rws(std::move(NDEff280kA_mat), ana_axis, oa280kA_axis, 1, 0);

    Eigen::ArrayXd FDEff_arr = Eigen::ArrayXd::Zero(FDefficiency.size() + 2);

    FDEff_arr.segment(1, FDefficiency.size()) = FDefficiency;

    Spectrum FDEff_s(std::move(FDEff_arr), ana_axis, 1, 0);

    if (hNDUnselected_293kA.get()) dir->WriteTObject(hNDUnselected_293kA.get(), "NDUnsel_293kA");
    if (hNDSelected_293kA.get()) dir->WriteTObject(hNDSelected_293kA.get(), "NDSel_293kA");
    if (hFDunselected.get()) dir->WriteTObject(hFDunselected.get(), "FDUnsel");
    if (hFDselected.get()) dir->WriteTObject(hFDselected.get(), "FDSel");
    dir->WriteTObject(NDEff293kA_rws.ToTH2(1), "NDEff_293kA");
    dir->WriteTObject(NDEff280kA_rws.ToTH2(1), "NDEff_280kA");
    dir->WriteTObject(FDEff_s.ToTH1(1), "FDEff");
  }

} // namespace ana
