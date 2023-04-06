#pragma once

#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Core/OscCalcFwdDeclare.h"

#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"

#include <limits>
#include <map>
#include <memory>
#include <string>
#include <vector>

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

    PRISMExtrapolator(const PRISMExtrapolator &ExtrapPreds);

    ///\brief Initialize the matcher with PredInterps that can
    /// predict relevant event rates.
    ///
    /// Expect a std::map initializer list/literal like:
    /// ```
    ///   extrap->Initialize({
    ///       {"ND_293kA_nu", ND_interp_FHC.get()},
    ///       {"ND_280kA_nu", ND_interp_FHC_280kA.get()},
    ///       {"FD_nub", FD_interp_RHC.get()},
    ///       //...
    ///   });
    /// ```
    ///
    /// Any can be left out, which will limit the
    /// allowed combinations of PRISM::NuChan that can be passed to
    /// GetMatchCoefficients
    void Initialize(std::map<std::string, PredictionInterp *> const &);

    PredictionInterp const *
    GetNDPred(PRISM::BeamMode bm = PRISM::BeamMode::kNuMode, int kA = 293) const;
    PredictionInterp const *
    GetFDPred(PRISM::BeamMode bm = PRISM::BeamMode::kNuMode) const;

    void SetStoreDebugMatches(bool v = true) { fStoreDebugMatches = v; }
    void SetMatchIntrinsicNue(bool v = true) { fMatchIntrinsicNue = v; }

    void SetIntrinsicNueXsecRatio(const Eigen::ArrayXd&& ratiotrueE) const {
      vNumuNueXsecRatioTrueEnu = std::move(ratiotrueE);
    }

    void SetIntrinsicNueXsecRatioAntiChannel(const Eigen::ArrayXd&& ratiotrueE) const {
      vNumuNueXsecRatioTrueEnuAntiChannel = std::move(ratiotrueE);
    }

    std::pair<Eigen::ArrayXd, Eigen::ArrayXd>
    GetFarMatchCoefficients(osc::IOscCalc *osc, PRISM::MatchChan chan,
                            SystShifts shift, double &soln_norm,
                            double &resid_norm, bool MatchWSBkg) const;

    std::pair<Eigen::ArrayXd, Eigen::ArrayXd>
    GetFarMatchCoefficients(osc::IOscCalc *osc, PRISM::MatchChan chan,
                            SystShifts shift, bool MatchWSBkg) const {
      double dummy1, dummy2;
      return GetFarMatchCoefficients(osc, chan, shift, dummy1, dummy2, MatchWSBkg);
    }

    std::pair<TH1 const *, TH1 const *>
    GetGaussianCoefficients(double mean, double width, PRISM::BeamChan NDbc,
                            SystShifts shift) const;

    Eigen::ArrayXd GetLastResidual() const { return fLastResidual; }

    void Write(TDirectory *);

    struct Conditioning {
      double RegFactor_293kA;
      double RegFactor_280kA;
      double ENuMin;
      double ENuMax;
    };

    std::map<PRISM::MatchChan, Conditioning> fConditioning;

    void SetTargetConditioning(PRISM::MatchChan chan, double RegFactor_293kA,
                               double RegFactor_280kA,
                               std::array<double, 2> erange = {
                                   0, std::numeric_limits<double>::max()}) {
      fConditioning[chan] =
          Conditioning{RegFactor_293kA, RegFactor_280kA, erange[0], erange[1]};
    }

    void SetChannelRegFactor(PRISM::MatchChan chan, int kA, double RegFactor) {

      switch (kA) {
      case 280: {
        fConditioning[chan].RegFactor_280kA = RegFactor;
        break;
      }
      case 293: {
        fConditioning[chan].RegFactor_293kA = RegFactor;
        break;
      }
      default: {
        std::cout << "[ERROR]: Invalid Horn Current for reg factor: " << kA
                  << std::endl;
        abort();
      }
      }
    }

  protected:
    std::vector<std::unique_ptr<TH2>> NDOffAxisPrediction;
    std::vector<std::unique_ptr<TH1>> FDUnOscPrediction;

    mutable Eigen::ArrayXd fLastResidual;
    mutable Eigen::ArrayXd vNumuNueXsecRatioTrueEnu;
    mutable Eigen::ArrayXd vNumuNueXsecRatioTrueEnuAntiChannel;
    mutable std::unique_ptr<TH1> fLastGaussResidual;

    PredictionInterp const *fNDPredInterp_293kA_nu;
    PredictionInterp const *fNDPredInterp_293kA_nub;

    PredictionInterp const *fNDPredInterp_280kA_nu;
    PredictionInterp const *fNDPredInterp_280kA_nub;

    PredictionInterp const *fFDPredInterp_nu;
    PredictionInterp const *fFDPredInterp_nub;

    mutable std::unique_ptr<TH1> fLastMatch_293kA;
    mutable std::unique_ptr<TH1> fLastMatch_280kA;
    mutable std::unique_ptr<TH1> fLastGaussMatch_293kA;
    mutable std::unique_ptr<TH1> fLastGaussMatch_280kA;

    bool fStoreDebugMatches;

    // Whether to include intrinsic nue bkg at FD in target flux matching
    // false: use FD MC for this bkg estimation
    // true: include in flux matching
    // numu/nue cross section ratio correction will be needed with true option
    bool fMatchIntrinsicNue;
    //this is for including the wrong sign background in FD target flux
    bool fMatchWSBkg;
    mutable std::map<std::string, std::unique_ptr<TH1>> fDebugTarget;
    mutable std::map<std::string, std::unique_ptr<TH1>> fDebugBF;
    mutable std::map<std::string, std::unique_ptr<TH2>> fDebugFitMatrix;
    mutable std::map<std::string, std::unique_ptr<TH2>> fDebugND_293kA;
    mutable std::map<std::string, std::unique_ptr<TH2>> fDebugND_280kA;
    mutable std::map<std::string, std::unique_ptr<TH1>> fDebugResid;
  };

} // namespace ana
