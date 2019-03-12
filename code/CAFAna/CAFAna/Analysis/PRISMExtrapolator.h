#pragma once

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
}

class TH1;
class TH2;
class TDirectory;

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

  void InitializeEventRateMatcher(ana::PredictionInterp const *NDEventRateInterp,
                             ana::PredictionInterp const *FDEventRateInterp);

  /// Use to check whether the ND flux histograms are binned consistently with
  /// the proposed ND analysis off axis binning. Will not merge bins, but will
  /// ignore extra bins in the flux prediction
  bool CheckOffAxisBinningConsistency(ana::Binning const &) const;

  void SetStoreDebugMatches() { fStoreDebugMatches = true; }

  TH1 *GetMatchCoefficientsEventRate(osc::IOscCalculator *osc,
                                 double max_OffAxis_m) const;
  TH1 *GetMatchCoefficientsFlux(
      osc::IOscCalculator *osc, double max_OffAxis_m,
      FluxPredSpecies NDMode = FluxPredSpecies::kNumu_numode,
      FluxPredSpecies FDMode = FluxPredSpecies::kNumu_numode) const;
  TH1 *GetMatchCoefficients(
      osc::IOscCalculator *osc, double max_OffAxis_m,
      FluxPredSpecies NDMode = FluxPredSpecies::kNumu_numode,
      FluxPredSpecies FDMode = FluxPredSpecies::kNumu_numode) const;

  void Write(TDirectory *);

  void SetTargetConditioning(double RegFactor = 1E-8,
                             double LowECutoff = 0xdeadbeef,
                             double HighECutoff = 0xdeadbeef,
                             bool LowEGaussTail = false) {
    fRegFactor = RegFactor;
    fENuMin = LowECutoff;
    fENuMax = HighECutoff;
    fLowEGaussFallOff = LowEGaussTail;
  }

protected:
  std::vector<std::unique_ptr<TH2>> NDOffAxisPrediction;
  std::vector<std::unique_ptr<TH1>> FDUnOscPrediction;

  ana::PredictionInterp const *fFDEventRateInterp;
  ana::PredictionInterp const *fNDEventRateInterp;

  bool fMatchEventRates;

  double fRegFactor;
  double fENuMin;
  double fENuMax;
  bool fLowEGaussFallOff;

  mutable std::map<std::string, std::unique_ptr<TH1>> fMatchCache;

  bool fStoreDebugMatches;
  mutable std::map<std::string, std::unique_ptr<TH1>> fDebugTarget;
  mutable std::map<std::string, std::unique_ptr<TH1>> fDebugBF;
};
