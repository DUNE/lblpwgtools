#pragma once

#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/OscCalcFwdDeclare.h"

#include "CAFAna/Vars/FitVars.h"

#include "CAFAna/Prediction/PredictionInterp.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Prediction/PredictionsForPRISM.h"

#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"
#include "CAFAna/PRISM/PredictionPRISM.h"
#include "CAFAna/PRISM/EigenUtils.h"

#include "StandardRecord/StandardRecord.h"

#include "OscLib/IOscCalc.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

#include "fhiclcpp/ParameterSet.h"

#include <memory>
#include <vector>

#define PRISMDEBUG

#ifdef PRISMDEBUG
#define PRISMOUT(a) std::cout << a << std::endl;
#else
#define PRISMOUT(a)
#endif

namespace ana {

class FVMassCorrection {
  size_t fOverflow;

public:
  TH1D *fvmasscor;

  FVMassCorrection();
  double GetWeight(double vtx_x_cm);
};

inline double FD_ND_FVRatio(double x_slice_cm) {
  double NDSliceFV = x_slice_cm * 200 * (350 - 50);
  double FDFV = 620 * 1100 * (1244 - 50);

  return FDFV / NDSliceFV;
}

ana::Weight NDSliceCorrection(double reference_width_cm, std::vector<double> const &Edges);

extern const ana::Weight kMassCorrection;

template <typename T>
inline void FillWithNulls(std::vector<std::unique_ptr<T>> &v, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    v.emplace_back(nullptr);
  }
}

template <typename T>
inline void FillWithNulls(std::vector<std::shared_ptr<T>> &v, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    v.emplace_back(nullptr);
  }
}

template <typename T>
inline void FillWithNulls(std::vector<std::vector<std::shared_ptr<T>>> &v, 
                          size_t n1, size_t n2) {
  for (size_t i = 0; i < n1; ++i) {
    std::vector<std::shared_ptr<T>> tmp;
    for (size_t j = 0; j < n2; ++j) {
      tmp.emplace_back(nullptr);
    }
    v.emplace_back(tmp);
  }
}

struct PRISMStateBlob {
  std::unique_ptr<PredictionPRISM> PRISM;
  std::vector<std::unique_ptr<PredictionInterp>> MatchPredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> NDMatrixPredInterps; 
  std::vector<std::unique_ptr<PredictionInterp>> FDMatrixPredInterps; 
  // For MC Eff Correction
  std::vector<std::unique_ptr<PredictionInterp>> NDUnselTruePredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> NDSelTruePredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> FDUnselTruePredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> FDSelTruePredInterps;
  //----------------------
  std::vector<std::unique_ptr<PredictionInterp>> FarDetPredInterps;
  std::vector<std::unique_ptr<OscillatableSpectrum>> FarDetData_nonswap;
  std::vector<std::unique_ptr<OscillatableSpectrum>> FarDetData_nueswap;
  std::vector<std::unique_ptr<DataPredictionNoExtrap>> FarDetDataPreds;

  std::unique_ptr<PredictionInterp> NDFluxPred_293kA_nu;
  std::unique_ptr<PredictionInterp> NDFluxPred_293kA_nub;
  std::unique_ptr<PredictionInterp> NDFluxPred_280kA_nu;
  std::unique_ptr<PredictionInterp> NDFluxPred_280kA_nub;
  std::unique_ptr<PredictionInterp> FDFluxPred_293kA_nu;
  std::unique_ptr<PredictionInterp> FDFluxPred_293kA_nub;

  bool Have(size_t pc) {
    bool IsNu = PRISM::IsNuConfig(pc);
    bool IsND = PRISM::IsNDConfig(pc);
    size_t fd_pc = 0;
    size_t IsNue = PRISM::IsNueConfig(pc);
    if (!IsND) {
      fd_pc = PRISM::GetFDConfig(pc);
    }

    // Don't need MatchPredInterps for Nue (they aren't made/used)
    return PRISM && (IsNue || bool(MatchPredInterps[pc])) &&
           (IsND || (bool(FarDetPredInterps[fd_pc]) &&
                     bool(FarDetData_nonswap[fd_pc]) &&
                     bool(FarDetData_nueswap[fd_pc])));
  }

  void Init() {
    FillWithNulls(MatchPredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(NDMatrixPredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(FDMatrixPredInterps, PRISM::kNPRISMFDConfigs);
    FillWithNulls(NDUnselTruePredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(NDSelTruePredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(FDUnselTruePredInterps, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FDSelTruePredInterps, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FarDetPredInterps, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FarDetData_nonswap, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FarDetData_nueswap, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FarDetDataPreds, PRISM::kNPRISMFDConfigs);

    NDFluxPred_293kA_nu = nullptr;
    NDFluxPred_293kA_nub = nullptr;
    NDFluxPred_280kA_nu = nullptr;
    NDFluxPred_280kA_nub = nullptr;
    FDFluxPred_293kA_nu = nullptr;
    FDFluxPred_293kA_nub = nullptr;
  }
};

PRISMStateBlob LoadPRISMState(TFile &f, std::string const &varname);
void DumpLoadedSpectra(PRISMStateBlob const &blob);

template <typename T>
inline void SaveTo(TFile &f, std::string const &dirname, T *ty) {
  PRISMOUT("Saving " << ty << "to " << dirname);
  if (!ty) {
    abort();
  }

  f.cd();
  TDirectory *dir = gDirectory;
  
  ty->SaveTo(dir, dirname.c_str());

}

template <typename T>
inline void SaveTo(TFile &f, std::string const &dirname,
                   std::unique_ptr<T> &ty) {
  PRISMOUT("Saving " << ty.get() << "to " << dirname);
  if (!ty) {
    abort();
  }

  f.cd();
  TDirectory *dir = gDirectory;

  ty->SaveTo(dir, dirname.c_str());
}

osc::IOscCalcAdjustable *
ConfigureCalc(fhicl::ParameterSet const &ps,
              osc::IOscCalcAdjustable *icalc = nullptr);

double GetCalcValue(osc::IOscCalcAdjustable *icalc = nullptr,
                    std::string paramname = "");

std::vector<const ana::IFitVar *>
GetOscVars(std::vector<std::string> const &osc_vars, int hie = 1, int oct = 0);
void ScrubOscVars(std::vector<const IFitVar *> &oscvars,
                  std::vector<std::string> const &names_to_scrub);

SystShifts GetSystShifts(fhicl::ParameterSet const &ps);
std::vector<ana::ISyst const *>
GetListOfSysts(std::vector<std::string> const &);

// Function to take a HistAxis with potentially multiple binnings
// and convert e.g. 2 binnings into 1 binning. Useful for the smearing matrix
// axes and covariance matrix axes, where each axis could be for multiple variables.
HistAxis GetMatrixAxis(const std::vector<HistAxis> &axisvec);

HistAxis GetTwoDAxis(const HistAxis &axis1, const HistAxis &axis2);

//---------------------------------
inline PRISMReweightableSpectrum ToReweightableSpectrum(Spectrum const &spec, 
                                                   double POT) {
  Eigen::MatrixXd spec_mat = ConvertArrayToMatrix(spec.GetEigen(POT),
                                                  spec.GetBinnings());

  // Sadly think I need to do some stuff with ROOT to get error matrix.
  std::unique_ptr<TH1> h_err = std::unique_ptr<TH1>(spec.ToTH1(POT));
  h_err->SetDirectory(nullptr);
  Eigen::ArrayXd ErrorArr = spec.GetEigen(1);
  ErrorArr.setZero();
  for (int el = 1; el <= ErrorArr.size() - 2; ++el) {
    ErrorArr(el) = h_err->GetBinError(el);
  }
  Eigen::ArrayXXd Errors_mat = ConvertArrayToMatrix(ErrorArr,
                                                    spec.GetBinnings()).array();

  Eigen::MatrixXd SumSq_mat = (Errors_mat.pow(2) * 
                               std::pow(POT/spec.POT(), 2)).matrix();
  /*Eigen::MatrixXd err_mat = Eigen::MatrixXd::Zero(spec_mat.rows(), spec_mat.cols());
  if (spec.GetBinnings().size() == 2) {
    //for (int col = 1; col <= (spec_mat.cols() - 2); col++) {
    //  for (int row = 1; row <= (spec_mat.rows() - 2); row++) {
    //    double err = h_err->GetBinError(row + (col - 1) * (spec_mat.rows() - 2));
        err_mat(row, col) = std::pow(err * (POT/spec.POT()), 2); // want error from MC
      }
    }
  } else if (spec.GetBinnings().size() == 3) {
    abort();
  }*/

  std::vector<std::string> anaLabels = { spec.GetLabels().at(0) };
  std::vector<Binning> anaBins = { spec.GetBinnings().at(0) };
  if (spec.GetBinnings().size() == 3) {
    anaLabels.push_back(spec.GetLabels().at(1));
    anaBins.push_back(spec.GetBinnings().at(1)); 
  }
  LabelsAndBins anaAxis = LabelsAndBins(anaLabels, anaBins);
  LabelsAndBins weightAxis = (spec.GetBinnings().size() == 2) ?
    LabelsAndBins(spec.GetLabels().at(1), spec.GetBinnings().at(1)) :
    LabelsAndBins(spec.GetLabels().at(2), spec.GetBinnings().at(2)); // Is 3D.

  PRISMReweightableSpectrum rwspec(std::move(spec_mat), std::move(SumSq_mat), 
                                   anaAxis, weightAxis, POT, 0); // POT

  return rwspec;
}
//---------------------------------
inline Spectrum ToSpectrum(ReweightableSpectrum const &rwspec, double pot) {
  Eigen::MatrixXd mat = rwspec.GetEigen(pot);
  int NCols = mat.cols();
  int NRows = mat.rows();

  Eigen::ArrayXd arr((NCols-2) * (NRows-2) + 2);
  arr.setZero();
  for (int col = 1; col <= (NCols - 2); col++ ) {
    for (int row = 1; row <= (NRows - 2); row++ ) {
      arr(row + (col - 1) * (NRows - 2)) = mat(row, col);
    }
  }

  std::vector<std::string> labels = rwspec.GetLabels();
  std::vector<Binning> bins = rwspec.GetBinnings();
  for (auto const &tb : rwspec.GetTrueBinnings()) {
    bins.push_back(tb);
    labels.push_back("");
  } 

  LabelsAndBins spec_LBs(labels, bins);

  Spectrum ret(std::move(arr), spec_LBs, pot, 0);
  return ret;  
}

//-----------------------------------
inline Spectrum SetSpectrumErrors(Spectrum const &spec, double mcpot) {

  Eigen::ArrayXd spec_arr = spec.GetEigen(spec.POT()); // arr for POT = 1
  Eigen::ArrayXd spec_arr_mc = spec.GetEigen(mcpot); // arr at original pot
  Eigen::ArrayXd spec_sumsq(spec_arr.size());
  spec_sumsq.setZero();

  for (int bin = 1; bin <= spec_arr.size() - 2; bin++) {
    double spec_err = std::sqrt(spec_arr_mc(bin));
    double frac_err;
    if (std::isnormal(spec_arr_mc(bin))) {
      frac_err = spec_err / spec_arr_mc(bin);
    } else { frac_err = 0; }
    spec_sumsq(bin) = std::pow(frac_err * spec_arr(bin), 2);
  }

  LabelsAndBins spec_LBs(spec.GetLabels(), spec.GetBinnings());
  return Spectrum(std::move(spec_arr), std::move(spec_sumsq), spec_LBs, spec.POT(), 0); 

}

} // namespace ana
