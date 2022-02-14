#pragma once

#include "CAFAna/Core/SystShifts.h"

#include "CAFAna/Prediction/PredictionInterp.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

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
    MCEffCorrection();
  
    // Copy constructor
    MCEffCorrection(const MCEffCorrection &EffCorr);
  
    ~MCEffCorrection();
  
    void Initialize(std::pair<PredictionInterp const *, size_t> NDunsel_293kA,
                    std::pair<PredictionInterp const *, size_t> NDsel_293kA,
                    std::pair<PredictionInterp const *, size_t> NDunsel_280kA,
                    std::pair<PredictionInterp const *, size_t> NDsel_280kA,
                    std::pair<PredictionInterp const *, size_t> FDunsel,
                    std::pair<PredictionInterp const *, size_t> FDsel);

    // Fills NDefficiency and FDefficiency, taking selected
    // ND and FD event rates as argument
    void CalcEfficiency(osc::IOscCalc *calc, 
                        HistAxis const &axis,
                        ana::SystShifts shift = kNoShift,
                        Flavors::Flavors_t NDflav = Flavors::kAll,
                        Flavors::Flavors_t FDflav = Flavors::kAll,
                        Current::Current_t curr = Current::kCC,
                        Sign::Sign_t NDsign = Sign::kBoth,
                        Sign::Sign_t FDsign = Sign::kBoth) const;

    Eigen::ArrayXXd GetNDefficiency(int kA) const {
      if (kA == 293) {
        return NDefficiency_293kA;
      } else if (kA == 280) {
        return NDefficiency_280kA;
      } else {
        std::cout << "[ERROR] unrecognised HC." << std::endl;
        abort();
      }
    }
    Eigen::ArrayXd GetFDefficiency() const { return FDefficiency; } 

    size_t GetNDConfigFromPred(Flavors::Flavors_t NDflav, Sign::Sign_t NDsign, 
                               bool is280kA = false) const;

    size_t GetFDConfigFromPred(Flavors::Flavors_t FDflav, Sign::Sign_t FDsign) const;
 
    void Write(TDirectory *dir) const;

  protected:
    std::vector<PredictionInterp const *> NDUnselPredInterps;
    std::vector<PredictionInterp const *> NDSelPredInterps;
    std::vector<PredictionInterp const *> FDUnselPredInterps;
    std::vector<PredictionInterp const *> FDSelPredInterps;

    // ND and FD efficiency in each energy bin
    // vector of vectors to hold ND eff at each OA stop.
    mutable Eigen::ArrayXXd NDefficiency_293kA;
    mutable Eigen::ArrayXXd NDefficiency_280kA;
    mutable Eigen::ArrayXd FDefficiency;
  };

} // namespace ana
