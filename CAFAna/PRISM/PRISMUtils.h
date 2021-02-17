#pragma once

#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Vars/FitVars.h"

#include "CAFAna/Prediction/PredictionInterp.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"

#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"
#include "CAFAna/PRISM/PredictionPRISM.h"

#include "StandardRecord/StandardRecord.h"

#include "OscLib/func/IOscCalculator.h"

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

ana::Var NDSliceCorrection(double reference_width_cm, std::vector<double> const &Edges);

extern const ana::Var kMassCorrection;

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

struct PRISMStateBlob {
  std::unique_ptr<PredictionPRISM> PRISM;
  std::vector<std::unique_ptr<PredictionInterp>> MatchPredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> SelPredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> NDMatrixPredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> NDMatrixTruePredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> NDMatrixRecoPredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> FDMatrixPredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> FDMatrixTruePredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> FDMatrixRecoPredInterps;
  // For MC Eff Correction
  std::vector<std::unique_ptr<PredictionInterp>> NDUnselTruePredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> NDSelTruePredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> FDUnselTruePredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> FDSelTruePredInterps;
  //----------------------
  std::vector<std::unique_ptr<PredictionInterp>> FarDetPredInterps;
  std::vector<std::unique_ptr<OscillatableSpectrum>> FarDetData_nonswap;
  std::vector<std::unique_ptr<OscillatableSpectrum>> FarDetData_nueswap;

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
           bool(SelPredInterps[pc]) &&
           (IsND || (bool(FarDetPredInterps[fd_pc]) &&
                     bool(FarDetData_nonswap[fd_pc]) &&
                     bool(FarDetData_nueswap[fd_pc])));
  }

  void Init() {
    FillWithNulls(MatchPredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(SelPredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(NDMatrixPredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(NDMatrixTruePredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(NDMatrixRecoPredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(FDMatrixPredInterps, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FDMatrixTruePredInterps, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FDMatrixRecoPredInterps, PRISM::kNPRISMFDConfigs);
    FillWithNulls(NDUnselTruePredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(NDSelTruePredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(FDUnselTruePredInterps, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FDSelTruePredInterps, PRISM::kNPRISMConfigs);
    FillWithNulls(FarDetPredInterps, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FarDetData_nonswap, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FarDetData_nueswap, PRISM::kNPRISMFDConfigs);

    NDFluxPred_293kA_nu = nullptr;
    NDFluxPred_293kA_nub = nullptr;
    NDFluxPred_280kA_nu = nullptr;
    NDFluxPred_280kA_nub = nullptr;
    FDFluxPred_293kA_nu = nullptr;
    FDFluxPred_293kA_nub = nullptr;
  }
};

PRISMStateBlob LoadPRISMState(TFile &f, std::string const &varname);

template <typename T>
inline void SaveTo(TFile &f, std::string const &dirname, T *ty) {
  PRISMOUT("Saving " << ty << "to " << dirname);
  if (!ty) {
    abort();
  }
  ty->SaveTo(f.mkdir(dirname.c_str()));
}

template <typename T>
inline void SaveTo(TFile &f, std::string const &dirname,
                   std::unique_ptr<T> &ty) {
  PRISMOUT("Saving " << ty.get() << "to " << dirname);
  if (!ty) {
    abort();
  }
  ty->SaveTo(f.mkdir(dirname.c_str()));
}

osc::IOscCalculatorAdjustable *
ConfigureCalc(fhicl::ParameterSet const &ps,
              osc::IOscCalculatorAdjustable *icalc = nullptr);

double GetCalcValue(osc::IOscCalculatorAdjustable *icalc = nullptr,
                    std::string paramname = "");

std::vector<const ana::IFitVar *>
GetOscVars(std::vector<std::string> const &osc_vars, int hie = 1, int oct = 0);
void ScrubOscVars(std::vector<const IFitVar *> &oscvars,
                  std::vector<std::string> const &names_to_scrub);

SystShifts GetSystShifts(fhicl::ParameterSet const &ps);
std::vector<ana::ISyst const *>
GetListOfSysts(std::vector<std::string> const &);

inline ReweightableSpectrum
ToReweightableSpectrum(Spectrum const &spec, double POT, HistAxis const &axis) {
  TH2D *spec_h;  

  if (spec.NDimensions() == 2) {
    spec_h = dynamic_cast<TH2D *>(spec.ToTH2(POT));
  } else if (spec.NDimensions() == 3) {
    TH3 *spec3d_h = spec.ToTH3(POT);
    std::cout << "x title = " << spec3d_h->GetXaxis()->GetTitle()
              << "y title = " << spec3d_h->GetYaxis()->GetTitle()
              << "z title = " << spec3d_h->GetZaxis()->GetTitle() << std::endl;
    // Reweighting axis binning
    const Binning rwbins = Binning::FromTAxis(spec3d_h->GetZaxis());
    // analysis axis put on to 1D
    Binning xbins = axis.GetBinnings()[0]; 
    int n = 1;
    for (const Binning &b : axis.GetBinnings()) {
      n *= b.NBins();
      xbins = Binning::Simple(n, 0, n);
    }

    spec_h = HistCache::NewTH2D("", xbins, rwbins);
    for (int xit = 1; xit <= spec3d_h->GetYaxis()->GetNbins(); xit++) { // EHad

      for (int zit = 1; zit <= spec3d_h->GetZaxis()->GetNbins(); zit++) { // RWvar
        // Get projection of ELep axis
        TH1D *projY = spec3d_h->ProjectionY("", xit, xit, zit, zit);
        // fill 2D hist with ELep*EHad x-axis
        int NbinsY = projY->GetXaxis()->GetNbins();
        for (int yit = 1; yit <= NbinsY; yit++) {
          spec_h->SetBinContent(yit + ((xit - 1) * NbinsY), 
                                zit, 
                                projY->GetBinContent(yit));
        }
        HistCache::Delete(projY);
      }
    }
  } else {
    std::cout << "[ERROR] Not 2D or 3D, check input dimensions" << std::endl;
    abort();
  } 

  ReweightableSpectrum rwspec(ana::Constant(1), spec_h, axis.GetLabels(),
                              axis.GetBinnings(), POT, 0);

  HistCache::Delete(spec_h);

  return rwspec;
}

//-----------------------------------------------------
// Class for ND and FD detector extrapolation matrices:
//----------------------------------------------------
class NDFD_Matrix {
public:
  //NDFD_Matrix(Spectrum ND, Spectrum FD, double pot);
  NDFD_Matrix(std::unique_ptr<PredictionInterp> ND,
              std::unique_ptr<PredictionInterp> NDTrue,
              std::unique_ptr<PredictionInterp> NDReco, 
              std::unique_ptr<PredictionInterp> FD,
              std::unique_ptr<PredictionInterp> FDTrue,
              std::unique_ptr<PredictionInterp> FDReco,
              double pot);  

  // Normalise the ETrue column to 1 in ND and FD matrices
  void NormaliseETrue(osc::IOscCalculator *calc,
                      ana::SystShifts shift = kNoShift,
                      Flavors::Flavors_t NDflav = Flavors::kAll,
                      Flavors::Flavors_t FDflav = Flavors::kAll,
                      Current::Current_t curr = Current::kCC,
                      Sign::Sign_t NDsign = Sign::kBoth,
                      Sign::Sign_t FDsign = Sign::kBoth,
                      std::vector<double> NDefficiency = {},
                      std::vector<double> FDefficiency = {}) const;

  TH2D * GetNDMatrix() const;
  TH2D * GetFDMatrix() const;

  TH1 * GetPRISMExtrap() const;

  // Extrapolate ND PRISM pred to FD using Eigen
  void ExtrapolateNDtoFD(Spectrum NDPRISMLCComp) const;  

protected:

  mutable std::unique_ptr<TH2D> hMatrixND;
  mutable std::unique_ptr<TH2D> hMatrixFD;
  //mutable std::unique_ptr<TH1> hMatrixND;
  //mutable std::unique_ptr<TH1> hMatrixFD;
  std::unique_ptr<PredictionInterp> fMatrixND;
  std::unique_ptr<PredictionInterp> fMatrixTrueND;
  std::unique_ptr<PredictionInterp> fMatrixRecoND;
  std::unique_ptr<PredictionInterp> fMatrixFD;
  std::unique_ptr<PredictionInterp> fMatrixTrueFD;
  std::unique_ptr<PredictionInterp> fMatrixRecoFD;
  const double fPOT;
  mutable std::unique_ptr<TH1> fPRISMExtrap;

};
 
//----------------------------------------------------
// Class for ND and FD MC based efficiency correction:
//----------------------------------------------------
// brief / take unselected MC event rates at ND and FD
// calculate the efficiency of the cuts
// Might incorporate this into NDFD_Matrix class later
class MCEffCorrection {
public:
  MCEffCorrection(std::unique_ptr<PredictionInterp> NDunsel,
                  std::unique_ptr<PredictionInterp> NDsel,
                  std::unique_ptr<PredictionInterp> FDunsel,
                  std::unique_ptr<PredictionInterp> FDsel);
  // Fills NDefficiency and FDefficiency, taking selected
  // ND and FD event rates as argument
  void CalcEfficiency(osc::IOscCalculator *calc, 
                      HistAxis const &axis,
                      //ana::SystShifts shift = kNoShift,
                      Flavors::Flavors_t NDflav = Flavors::kAll,
                      Flavors::Flavors_t FDflav = Flavors::kAll,
                      Current::Current_t curr = Current::kCC,
                      Sign::Sign_t NDsign = Sign::kBoth,
                      Sign::Sign_t FDsign = Sign::kBoth) const;

  std::vector<double> GetNDefficiency() const { return NDefficiency; }
  std::vector<double> GetFDefficiency() const { return FDefficiency; }

protected:
  std::unique_ptr<PredictionInterp> fNDunselected;
  std::unique_ptr<PredictionInterp> fNDselected;
  std::unique_ptr<PredictionInterp> fFDunselected;
  std::unique_ptr<PredictionInterp> fFDselected;
  // ND and FD efficiency in each energy bin
  // Vec element [0] is 1st energy bin eff 
  mutable std::vector<double> NDefficiency;
  mutable std::vector<double> FDefficiency;

};

} // namespace ana
