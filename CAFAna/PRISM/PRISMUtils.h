#pragma once

#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Vars/FitVars.h"

#include "CAFAna/Prediction/PredictionInterp.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"

#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"

#include "StandardRecord/StandardRecord.h"

#include "OscLib/func/IOscCalculator.h"

#include "TFile.h"
#include "TH1D.h"

#include "fhiclcpp/ParameterSet.h"

#include <memory>
#include <vector>

// #define PRISMDEBUG

#ifdef PRISMDEBUG
#define PRISMOUT(a) std::cout << a << std::endl;
#else
#define PRISMOUT(a)
#endif

namespace ana {

class PredictionPRISM;

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

extern const ana::Var kMassCorrection;

template <typename T>
inline void FillWithNulls(std::vector<std::unique_ptr<T>> &v, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    v.emplace_back(nullptr);
  }
}

struct PRISMStateBlob {
  std::unique_ptr<PredictionPRISM> PRISM;
  std::vector<std::unique_ptr<PredictionInterp>> MatchPredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> SelPredInterps;
  std::vector<std::unique_ptr<PredictionInterp>> FarDetPredInterps;
  std::vector<std::unique_ptr<OscillatableSpectrum>> FarDetData_nonswap;
  std::vector<std::unique_ptr<OscillatableSpectrum>> FarDetData_nueswap;

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
    FillWithNulls(FarDetPredInterps, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FarDetData_nonswap, PRISM::kNPRISMFDConfigs);
    FillWithNulls(FarDetData_nueswap, PRISM::kNPRISMFDConfigs);
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
std::vector<ana::ISyst const *> GetListOfSysts(std::vector<std::string> const &);

SystShifts GetFluxSystShifts(SystShifts);

} // namespace ana
