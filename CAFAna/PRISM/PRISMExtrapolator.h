#pragma once

#include "CAFAna/Core/SystShifts.h"

#include "OscLib/func/IOscCalculator.h"

#include <limits>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace osc {
class IOscCalculator;
}
namespace ana {
class Binning;
class PredictionInterp;
} // namespace ana

class TH1;
class TH2;
class TDirectory;

namespace ana {

// Caching flux matcher.
class PRISMExtrapolator {
public:
  enum class FluxPredSpecies {
    kNumu_numode = 0,
    kNue_numode = 1,
    kNumubar_numode = 2,
    kNuebar_numode = 3,

    kNumu_nubarmode = 4,
    kNue_nubarmode = 5,
    kNumubar_nubarmode = 6,
    kNuebar_nubarmode = 7,

    kUnhandled = 8
  };

  PRISMExtrapolator();

  void InitializeFluxMatcher(std::string const &FluxFilePath,
                             int OffAxisBinMerge = 1, int NDEnergyBinMerge = 1,
                             int FDEnergyBinMerge = 1);

  void InitializeEventRateMatcher(PredictionInterp const *NDEventRateInterp,
                                  PredictionInterp const *FDEventRateInterp);

  double GetNDPOT() const;

  /// Use to check whether the ND flux histograms are binned consistently with
  /// the proposed ND analysis off axis binning. Will not merge bins, but will
  /// ignore extra bins in the flux prediction
  bool CheckOffAxisBinningConsistency(Binning const &) const;

  void SetStoreDebugMatches() { fStoreDebugMatches = true; }

  TH1 const *GetMatchCoefficientsEventRate(osc::IOscCalculator *osc,
                                           double max_OffAxis_m,
                                           SystShifts shift = kNoShift) const;
  TH1 const *GetMatchCoefficientsFlux(
      osc::IOscCalculator *osc, double max_OffAxis_m,
      FluxPredSpecies NDMode = FluxPredSpecies::kNumu_numode,
      FluxPredSpecies FDMode = FluxPredSpecies::kNumu_numode) const;
  TH1 const *
  GetMatchCoefficients(osc::IOscCalculator *osc, double max_OffAxis_m,
                       FluxPredSpecies NDMode = FluxPredSpecies::kNumu_numode,
                       FluxPredSpecies FDMode = FluxPredSpecies::kNumu_numode,
                       SystShifts const &shift = kNoShift) const;

  TH1 const *GetLastResidual() const { return fLastResidual.get(); }

  void Write(TDirectory *);

  void
  SetTargetConditioning(double RegFactor = 1E-8,
                        std::vector<double> CoeffRegVector = {},
                        double LowECutoff = 0,
                        double HighECutoff = std::numeric_limits<double>::max(),
                        bool LowEGaussTail = false) {
    fRegFactor = RegFactor;
    fENuMin = LowECutoff;
    fENuMax = HighECutoff;
    fCoeffRegVector = CoeffRegVector;
    fLowEGaussFallOff = LowEGaussTail;
  }

protected:
  std::vector<std::unique_ptr<TH2>> NDOffAxisPrediction;
  std::vector<std::unique_ptr<TH1>> FDUnOscPrediction;

  mutable std::unique_ptr<TH1> fLastResidual;

  PredictionInterp const *fFDEventRateInterp;
  PredictionInterp const *fNDEventRateInterp;

  bool fMatchEventRates;

  double fRegFactor;
  double fENuMin;
  double fENuMax;
  mutable std::vector<double> fCoeffRegVector;
  bool fLowEGaussFallOff;

  mutable std::map<std::string, std::unique_ptr<TH1>> fMatchCache;

  bool fStoreDebugMatches;
  mutable std::map<std::string, std::unique_ptr<TH1>> fDebugTarget;
  mutable std::map<std::string, std::unique_ptr<TH1>> fDebugBF;
  mutable std::map<std::string, std::unique_ptr<TH2>> fDebugND;
  mutable std::map<std::string, std::unique_ptr<TH2>> fDebugND_shift;
  mutable std::map<std::string, std::unique_ptr<TH1>> fDebugResid;
};

} // namespace ana
