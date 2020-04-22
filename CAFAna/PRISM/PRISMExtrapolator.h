#pragma once

#include "CAFAna/Core/SystShifts.h"

#include "OscLib/func/IOscCalculator.h"

#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"

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
class Spectrum;
} // namespace ana

class TH1;
class TH2;
class TDirectory;

namespace ana {

// Caching flux matcher.
class PRISMExtrapolator {
public:
  PRISMExtrapolator();

  ///\brief Initialize the matcher with PredInterps that can
  /// predict relevant event rates.
  ///
  /// Expect a std::map initializer list/literal like:
  /// ```
  ///   extrap->Initialize({
  ///       {"ND_nu", ND_interp_FHC.get()},
  ///       {"FD_nub", FD_interp_RHC.get()},
  ///       //...
  ///   });
  /// ```
  ///
  /// Any can be left out, which will limit the
  /// allowed combinations of PRISM::NuChan that can be passed to
  /// GetMatchCoefficients
  void Initialize(std::map<std::string, PredictionInterp const *> const &);

  PredictionInterp const *
  GetNDPred(PRISM::BeamMode bm = PRISM::BeamMode::kNuMode) const;
  PredictionInterp const *
  GetFDPred(PRISM::BeamMode bm = PRISM::BeamMode::kNuMode) const;

  void SetStoreDebugMatches() { fStoreDebugMatches = true; }

  TH1 const *GetFarMatchCoefficients(osc::IOscCalculator *osc,
                                     TH1 const *FDUnOscWeighted,
                                     double max_OffAxis_m,
                                     PRISM::BeamChan NDbc = PRISM::kNumu_Numode,
                                     PRISM::BeamChan FDbc = PRISM::kNumu_Numode,
                                     SystShifts shift = kNoShift) const;

  TH1 const *GetGaussianCoefficients(double mean, double width,
                                     double max_OffAxis_m,
                                     PRISM::BeamChan NDbc = PRISM::kNumu_Numode,
                                     SystShifts shift = kNoShift) const;

  TH1 const *GetLastResidual() const { return fLastResidual.get(); }

  void Write(TDirectory *);

  void SetTargetConditioning(
      double RegFactor = 1E-8, std::vector<double> CoeffRegVector = {},
      double LowECutoff = 0,
      double HighECutoff = std::numeric_limits<double>::max()) {
    fRegFactor = RegFactor;
    fENuMin = LowECutoff;
    fENuMax = HighECutoff;
    fCoeffRegVector = CoeffRegVector;
  }

protected:
  std::vector<std::unique_ptr<TH2>> NDOffAxisPrediction;
  std::vector<std::unique_ptr<TH1>> FDUnOscPrediction;

  mutable std::unique_ptr<TH1> fLastResidual;
  mutable std::unique_ptr<TH1> fLastGaussResidual;

  PredictionInterp const *fNDEventRateInterp_nu;
  PredictionInterp const *fNDEventRateInterp_nub;

  PredictionInterp const *fFDEventRateInterp_nu;
  PredictionInterp const *fFDEventRateInterp_nub;

  double fRegFactor;
  double fENuMin;
  double fENuMax;
  mutable std::vector<double> fCoeffRegVector;

  mutable std::unique_ptr<TH1> fLastMatch;
  mutable std::unique_ptr<TH1> fLastGaussMatch;

  bool fStoreDebugMatches;
  mutable std::map<std::string, std::unique_ptr<TH1>> fDebugTarget;
  mutable std::map<std::string, std::unique_ptr<TH1>> fDebugBF;
  mutable std::map<std::string, std::unique_ptr<TH2>> fDebugND;
  mutable std::map<std::string, std::unique_ptr<TH2>> fDebugND_shift;
  mutable std::map<std::string, std::unique_ptr<TH1>> fDebugResid;
};

} // namespace ana
