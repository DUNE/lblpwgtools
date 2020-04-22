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

static double kdummydouble = 0;

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

  void SetStoreDebugMatches(bool v = true) { fStoreDebugMatches = v; }

  TH1 const *GetFarMatchCoefficients(
      osc::IOscCalculator *osc, TH1 const *FDUnOscWeighted,
      double max_OffAxis_m,
      PRISM::MatchChan chan = PRISM::kNumuDisappearance_Numode,
      SystShifts shift = kNoShift, double &soln_norm = kdummydouble,
      double &resid_norm = kdummydouble) const;

  TH1 const *GetGaussianCoefficients(double mean, double width,
                                     double max_OffAxis_m,
                                     PRISM::BeamChan NDbc = PRISM::kNumu_Numode,
                                     SystShifts shift = kNoShift) const;

  TH1 const *GetLastResidual() const { return fLastResidual.get(); }

  void Write(TDirectory *);

  struct Conditioning {
    double RegFactor;
    double ENuMin;
    double ENuMax;
    mutable std::vector<double> CoeffRegVector;
  };

  std::map<PRISM::MatchChan, Conditioning> fConditioning;

  void SetTargetConditioning(PRISM::MatchChan chan, double RegFactor,
                             std::vector<double> CoeffRegVector = {},
                             std::array<double, 2> erange = {
                                 0, std::numeric_limits<double>::max()}) {
    fConditioning[chan] =
        Conditioning{RegFactor, erange[0], erange[1], CoeffRegVector};
  }

  void SetChannelRegFactor(PRISM::MatchChan chan, double RegFactor) {
    fConditioning[chan].RegFactor = RegFactor;
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
