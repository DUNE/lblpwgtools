#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PRISMMCEffCorrection.h"

using namespace PRISM;

namespace ana {

  //----------------------------------------------------
  // Class for ND and FD MC based efficiency correction:
  //----------------------------------------------------

  MCEffCorrection::MCEffCorrection() : fNDunselected_293kA(nullptr),
                                       fNDselected_293kA(nullptr),
                                       fNDunselected_280kA(nullptr),
                                       fNDselected_280kA(nullptr),
                                       fFDunselected(nullptr),
                                       fFDselected(nullptr) 
    {
    }
                                

  //----------------------------------------------------

  MCEffCorrection::MCEffCorrection(const MCEffCorrection &EffCorr) {
    fNDunselected_293kA = EffCorr.fNDunselected_293kA;
    fNDselected_293kA = EffCorr.fNDselected_293kA;
    fNDunselected_280kA = EffCorr.fNDunselected_280kA;
    fNDselected_280kA = EffCorr.fNDselected_280kA;
    fFDunselected = EffCorr.fFDunselected;
    fFDselected = EffCorr.fFDselected;
  }

  //----------------------------------------------------

  MCEffCorrection::~MCEffCorrection() {
    fNDunselected_293kA = nullptr;
    fNDselected_293kA = nullptr;
    fNDunselected_280kA = nullptr; 
    fNDselected_280kA = nullptr;
    fFDunselected = nullptr;
    fFDselected = nullptr;
  }

  //----------------------------------------------------

  void MCEffCorrection::Initialize(PredictionInterp const * NDunsel_293kA,
                                   PredictionInterp const * NDsel_293kA,
                                   PredictionInterp const * NDunsel_280kA,
                                   PredictionInterp const * NDsel_280kA,
                                   PredictionInterp const * FDunsel,
                                   PredictionInterp const * FDsel) {

    fNDunselected_293kA = NDunsel_293kA;
    fNDselected_293kA = NDsel_293kA;
    fNDunselected_280kA = NDunsel_280kA;
    fNDselected_280kA = NDsel_280kA;
    fFDunselected = FDunsel;
    fFDselected = FDsel;

    osc::NoOscillations no;
    auto NDPred = fNDunselected_293kA->Predict(&no); 

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

    ana::SystShifts syst = shift;
  
    osc::NoOscillations no; 
  
    if (!fNDunselected_293kA || !fNDselected_293kA || !fNDunselected_280kA || !fNDselected_280kA) {
      std::cout << "[WARNING] No NDunselected and/or NDselected Pred" << std::endl;
      abort();
    }

    auto sNDunselected_293kA = fNDunselected_293kA->PredictComponentSyst(&no, syst, 
                                                                         NDflav, curr, NDsign);
    auto sNDunselected_280kA = fNDunselected_280kA->PredictComponentSyst(&no, syst,
                                                                         NDflav, curr, NDsign);
  
    // Analysis axis could be 2D, so put into RWSpec so we can have it projected into 1D trueaxis
    ReweightableSpectrum rwsNDunselected_293kA = ToReweightableSpectrum(sNDunselected_293kA, 
                                                                        1);
    ReweightableSpectrum rwsNDunselected_280kA = ToReweightableSpectrum(sNDunselected_280kA,
                                                                        1);
    Eigen::MatrixXd NDunsel_293kA = rwsNDunselected_293kA.GetEigen(1);
    Eigen::MatrixXd NDunsel_280kA = rwsNDunselected_280kA.GetEigen(1);  
  
    // Selected ND MC
    Spectrum sNDselected_293kA = fNDselected_293kA->PredictComponentSyst(&no, syst, 
                                                                         NDflav, curr, NDsign);
    Spectrum sNDselected_280kA = fNDselected_280kA->PredictComponentSyst(&no, syst, 
                                                                         NDflav, curr, NDsign);
    ReweightableSpectrum rwsNDselected_293kA = ToReweightableSpectrum(sNDselected_293kA, 
                                                                      1);
    ReweightableSpectrum rwsNDselected_280kA = ToReweightableSpectrum(sNDselected_280kA, 
                                                                      1);
    Eigen::MatrixXd NDsel_293kA = rwsNDselected_293kA.GetEigen(1);
    Eigen::MatrixXd NDsel_280kA = rwsNDselected_280kA.GetEigen(1);
  
    if (!fFDunselected || !fFDselected) {
      std::cout << "[WARNING] No FDunselected and or FDselected Pred" << std::endl; 
      abort();
    }
    // FD unselected // calc!!
    Spectrum sFDunselected = fFDunselected->PredictComponentSyst(calc, syst, FDflav, curr, FDsign);
    Eigen::ArrayXd vFDunselected = sFDunselected.GetEigen(1);
    // FD selected // calc!!
    Spectrum sFDselected = fFDselected->PredictComponentSyst(calc, syst, FDflav, curr, FDsign);
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

  //-----------------------------------------------------------
  
  void MCEffCorrection::Write(TDirectory *dir) const {
    // Don't care how ugly this is, just getting the plots.
    // Just to get the axes.
    osc::NoOscillations no;
    auto NDPred = fNDunselected_293kA->Predict(&no);
  
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
