#pragma once

#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Vars/FitVars.h"

#include "CAFAna/Prediction/PredictionInterp.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"

#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"
#include "CAFAna/PRISM/PredictionPRISM.h"
#include "CAFAna/PRISM/EigenUtils.h"

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

//----------------------------------------------------
// Class for ND and FD MC based efficiency correction:
//----------------------------------------------------
// brief / take unselected MC event rates at ND and FD
// calculate the efficiency of the cuts
// Might incorporate this into NDFD_Matrix class later
class MCEffCorrection {

public:
  MCEffCorrection(PredictionInterp const * NDunsel_293kA,
                  PredictionInterp const * NDsel_293kA,
                  PredictionInterp const * NDunsel_280kA,
                  PredictionInterp const * NDsel_280kA,
                  PredictionInterp const * FDunsel,
                  PredictionInterp const * FDsel);

  // Copy constructor
  MCEffCorrection(const MCEffCorrection &EffCorr);

  ~MCEffCorrection();

  // Fills NDefficiency and FDefficiency, taking selected
  // ND and FD event rates as argument
  void CalcEfficiency(osc::IOscCalculator *calc, 
                      HistAxis const &axis,
                      ana::SystShifts shift = kNoShift,
                      Flavors::Flavors_t NDflav = Flavors::kAll,
                      Flavors::Flavors_t FDflav = Flavors::kAll,
                      Current::Current_t curr = Current::kCC,
                      Sign::Sign_t NDsign = Sign::kBoth,
                      Sign::Sign_t FDsign = Sign::kBoth) const;

  std::vector<std::vector<double>> GetNDefficiency(int kA) const { 
    if (kA == 293) {
      return NDefficiency_293kA; 
    } else if (kA == 280) {
      return NDefficiency_280kA;
    } else {
      std::cout << "[ERROR] unrecognised HC." << std::endl;
      abort();
    }
  } 
  std::vector<double> GetFDefficiency() const { return FDefficiency; }

  void Write(TDirectory *dir);

protected:
  PredictionInterp const * fNDunselected_293kA;
  PredictionInterp const * fNDselected_293kA;
  PredictionInterp const * fNDunselected_280kA;
  PredictionInterp const * fNDselected_280kA;
  PredictionInterp const * fFDunselected;
  PredictionInterp const * fFDselected;

  mutable TH2D *hNDunselected_293kA;
  mutable TH2D *hNDselected_293kA;
  mutable TH2D *hNDunselected_280kA;
  mutable TH2D *hNDselected_280kA;
  mutable TH1D *hFDunselected;
  mutable TH1D *hFDselected;
  // ND and FD efficiency in each energy bin
  // vector of vectors to hold ND eff at each OA stop.
  mutable std::vector<std::vector<double>> NDefficiency_293kA;
  mutable std::vector<std::vector<double>> NDefficiency_280kA;
  mutable std::vector<double> FDefficiency;
  // bool to state whether the calculation has been done before
  mutable bool fDoneOnce;

};

} // namespace ana
