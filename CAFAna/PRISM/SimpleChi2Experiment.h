#pragma once

#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Experiment/IExperiment.h"
#include "CAFAna/PRISM/EigenUtils.h"
#include "CAFAna/PRISM/PredictionPRISM.h"

namespace ana {

  // Chi2Experiment for Chi2 calculation using full
  // PRISM covariance

  class PRISMChi2CovarExperiment : public IExperiment {
  public:
    PRISMChi2CovarExperiment(PredictionPRISM const *Pred, Spectrum const &Data,
                             bool UseCovariance = false, double NDPOT = 0, double FDPOT = 0,
                             PRISM::MatchChan match_chan = PRISM::kNumuDisappearance_Numode);

    // Get the extrapolated PRISM prediction
    Eigen::ArrayXd GetPred(
        std::map<PredictionPRISM::PRISMComponent, Spectrum> &PRISMComps) const;
    // Get the PRISM covariance matrix
    Eigen::ArrayXd GetCovariance(
        std::map<PredictionPRISM::PRISMComponent, Spectrum> &PRISMComps) const;

    Eigen::ArrayXd Concatenate(const std::vector<Eigen::ArrayXd>& arrs);
    void SetMaskArray(double xmin=0, double xmax=-1, double ymin=0, double ymax=-1);
    void ApplyMask(Eigen::ArrayXd& a, Eigen::ArrayXd& b, Eigen::ArrayXd& cov) const;

    // Calculate Chi2 with the option to include the full covariance
    virtual double ChiSq(osc::IOscCalcAdjustable *osc,
                         const SystShifts &syst = kNoShift) const override;
  protected:
    PredictionPRISM const *fPred;
    Spectrum fData;
    double fPOTFD;
    double fPOTND;
    bool fUseCovariance;
    Eigen::ArrayXd fMaskA;
    Eigen::ArrayXd fMaskCov;

    PRISM::MatchChan fMatchChannel;
  };



} // namespace ana
