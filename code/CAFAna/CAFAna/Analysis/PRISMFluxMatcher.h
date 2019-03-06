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
}

class TH1;
class TH2;
class TDirectory;

// Caching flux matcher.
class PRISMFluxMatcher {
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

  PRISMFluxMatcher(std::string const &FluxFilePath, int OffAxisBinMerge = 1,
                   int NDEnergyBinMerge = 1, int FDEnergyBinMerge = 1);

  /// Use to check whether the ND flux histograms are binned consistently with
  /// the proposed ND analysis off axis binning. Will not merge bins, but will
  /// ignore extra bins in the flux prediction
  bool CheckOffAxisBinningConsistency(ana::Binning const &);

  void SetStoreDebugMatches() { fStoreDebugMatches = true;}

  TH1 *GetFluxMatchCoefficients(
      osc::IOscCalculator *, double max_OffAxis_m,
      FluxPredSpecies NDMode = FluxPredSpecies::kNumu_numode,
      FluxPredSpecies FDMode = FluxPredSpecies::kNumu_numode);

  void Write(TDirectory *);

protected:
  std::vector<std::unique_ptr<TH2>> NDOffAxisPrediction;
  std::vector<std::unique_ptr<TH1>> FDUnOscPrediction;

  std::map<std::string, std::unique_ptr<TH1>> fMatchCache;

  bool fStoreDebugMatches;
  std::map<std::string, std::unique_ptr<TH1>> fDebugTarget;
  std::map<std::string, std::unique_ptr<TH1>> fDebugBF;

};
