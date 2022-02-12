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
    auto NDPred = NDunsel_293kA.first->Predict(&no);

    // WARNING: Not set up for 2D preds!
    std::vector<Binning> bins = NDPred.GetBinnings();
    int NAnaElements, NOAElements; 
    if (NDPred.NDimensions() == 2) {
      NAnaElements = bins.at(0).NBins();
      NOAElements = bins.at(1).NBins();
    } else { // is 2D pred, not done yet. 
      abort();
    }
    std::vector<double> NDE_293kA, NDE_280kA;
    int it(0);
    while (it < NAnaElements) {
      NDE_293kA.push_back(0);
      NDE_280kA.push_back(0);
      FDefficiency.push_back(0);
      it++;
    }
    it = 0;
    while (it < NOAElements) {
      NDefficiency_293kA.push_back(NDE_293kA);
      NDefficiency_280kA.push_back(NDE_280kA);
      it++;
    }
  }

  //----------------------------------------------------
  
  void MCEffCorrection::CalcEfficiency(osc::IOscCalc *calc,
                                       HistAxis const &axis,
                                       ana::SystShifts shift,
                                       Flavors::Flavors_t NDflav,
                                       Flavors::Flavors_t FDflav,
                                       Current::Current_t curr,
                                       Sign::Sign_t NDsign, 
                                       Sign::Sign_t FDsign) const {

    osc::NoOscillations no; 

    Spectrum sNDunselected_293kA = 
        NDUnselPredInterps.at(GetNDConfigFromPred(NDflav, NDsign, false))
        ->PredictComponentSyst(&no, kNoShift, NDflav, curr, NDsign);
    Spectrum sNDunselected_280kA =
        NDUnselPredInterps.at(GetNDConfigFromPred(NDflav, NDsign, true))
        ->PredictComponentSyst(&no, kNoShift, NDflav, curr, NDsign);  


    // Analysis axis could be 2D, so put into RWSpec so we can have it projected into 1D trueaxis
    //ReweightableSpectrum rwsNDunselected_293kA = ToReweightableSpectrum(sNDunselected_293kA, 
    //                                                                    1);
    //ReweightableSpectrum rwsNDunselected_280kA = ToReweightableSpectrum(sNDunselected_280kA,
    //                                                                    1);
    Eigen::MatrixXd NDunsel_293kA = ConvertArrayToMatrix(sNDunselected_293kA.GetEigen(1),
                                                         sNDunselected_293kA.GetBinnings());
    Eigen::MatrixXd NDunsel_280kA = ConvertArrayToMatrix(sNDunselected_280kA.GetEigen(1),
                                                         sNDunselected_280kA.GetBinnings());  
  
    // Selected ND MC
    Spectrum sNDselected_293kA = 
        NDSelPredInterps.at(GetNDConfigFromPred(NDflav, NDsign, false))
        ->PredictComponentSyst(&no, kNoShift, NDflav, curr, NDsign);
    Spectrum sNDselected_280kA = 
         NDSelPredInterps.at(GetNDConfigFromPred(NDflav, NDsign, true))
         ->PredictComponentSyst(&no, kNoShift, NDflav, curr, NDsign);

    //ReweightableSpectrum rwsNDselected_293kA = ToReweightableSpectrum(sNDselected_293kA, 
    //                                                                  1);
    //ReweightableSpectrum rwsNDselected_280kA = ToReweightableSpectrum(sNDselected_280kA, 
    //                                                                  1);
    Eigen::MatrixXd NDsel_293kA = ConvertArrayToMatrix(sNDselected_293kA.GetEigen(1),
                                                       sNDselected_293kA.GetBinnings());
    Eigen::MatrixXd NDsel_280kA = ConvertArrayToMatrix(sNDselected_280kA.GetEigen(1),
                                                       sNDselected_280kA.GetBinnings());
  
    Spectrum sFDunselected = 
        FDUnselPredInterps.at(GetFDConfigFromPred(FDflav, FDsign))
        ->PredictComponentSyst(calc, shift, FDflav, curr, FDsign);  //calc
    Eigen::ArrayXd vFDunselected = sFDunselected.GetEigen(1);
    Spectrum sFDselected = 
        FDSelPredInterps.at(GetFDConfigFromPred(FDflav, FDsign))
        ->PredictComponentSyst(calc, shift, FDflav, curr, FDsign);  //calc
    Eigen::ArrayXd vFDselected = sFDselected.GetEigen(1);
  
    // Calculate ND efficiency
    // efficiency fluctuates slightly with OA position
    // Efficiency for 293kA sample:
    for (int slice = 1; slice <= (NDsel_293kA.rows() - 2); slice++) { // OA slice
      std::vector<double> SliceEfficiency;
      for (int ebin = 1; ebin <= (NDsel_293kA.cols() - 2); ebin++) { // Energy bins
        double NDbin_eff = NDsel_293kA(slice, ebin) / NDunsel_293kA(slice, ebin);
        if (std::isnormal(NDbin_eff)) {
          SliceEfficiency.push_back(NDbin_eff);
        } else {
          SliceEfficiency.push_back(1E-6); // arbitrarily small instead of 0
        }
      }
      NDefficiency_293kA.at(slice - 1) = SliceEfficiency;
    }
  
    // Efficiency for 280kA sample:
    for (int slice = 1; slice <= (NDsel_280kA.rows() - 2); slice++) { // OA slice
      std::vector<double> SliceEfficiency;
      for (int ebin = 1; ebin <= (NDsel_280kA.cols() - 2); ebin++) { // Energy bins
        double NDbin_eff = NDsel_280kA(slice, ebin) / NDunsel_280kA(slice, ebin);
        if (std::isnormal(NDbin_eff)) {
          SliceEfficiency.push_back(NDbin_eff);
        } else {
          SliceEfficiency.push_back(1E-6); // arbitrarily small instead of 0 
        }
      }
      NDefficiency_280kA.at(slice - 1) = SliceEfficiency;
    }
  
    // Calculate FD efficiency
    for (int ebin = 1; ebin <= (vFDselected.size() - 2); ebin++) { 
      double FDbin_eff = vFDselected(ebin) / vFDunselected(ebin);
      if (std::isnormal(FDbin_eff)) {
        FDefficiency.at(ebin - 1) = FDbin_eff;
      } else {
        FDefficiency.at(ebin - 1) = 1E-6;
      }
    }
  }

  //----------------------------------------------------------
  size_t MCEffCorrection::GetNDConfigFromPred(Flavors::Flavors_t NDflav, Sign::Sign_t NDsign, 
                                           bool is280kA) const {
    size_t conf;
    assert(NDflav == Flavors::kAllNumu); // Only considering numu at ND.
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
    } else { abort(); }
                                                                                  
    return GetFDConfig(conf);
  }                                                                                                

  //-----------------------------------------------------------
  
  void MCEffCorrection::Write(TDirectory *dir) const {
    // Don't care how ugly this is, just getting the plots.
    // Just to get the axes.
    osc::NoOscillations no;
    auto NDPred = NDUnselPredInterps.at(kND_293kA_nu)->Predict(&no);  

    std::vector<std::string> labels = NDPred.GetLabels();
    std::vector<Binning> bins = NDPred.GetBinnings();  
  
    LabelsAndBins ana_axis(labels.at(0), bins.at(0));
    LabelsAndBins oa_axis(labels.at(1), bins.at(1));
  
    int NAnaElements = bins.at(0).NBins() + 2;
    int NOAElements = bins.at(1).NBins() + 2;
  
    Eigen::MatrixXd NDEff293kA_mat = Eigen::MatrixXd::Zero(NOAElements,
                                                         NAnaElements);
   
    for (int y = 1; y <= NDEff293kA_mat.rows() - 2; y++) {
      for (int x = 1; x <= NDEff293kA_mat.cols() - 2; x++) {
        NDEff293kA_mat(y, x) = NDefficiency_293kA.at(y - 1).at(x - 1);
      }
    }
  
    ReweightableSpectrum NDEff293kA_rws(std::move(NDEff293kA_mat), ana_axis, oa_axis, 1, 0);
  
    Eigen::ArrayXd FDEff_arr = Eigen::ArrayXd::Zero(NAnaElements);
  
    for (int x = 1; x <= FDEff_arr.size() - 2; x++) {
      FDEff_arr(x) = FDefficiency.at(x - 1);
    }
  
    Spectrum FDEff_s(std::move(FDEff_arr), ana_axis, 1, 0);
  
    dir->WriteTObject(NDEff293kA_rws.ToTH2(1), "NDEff_293kA");
    dir->WriteTObject(FDEff_s.ToTH1(1), "FDEff");
  }

} // namespace ana
