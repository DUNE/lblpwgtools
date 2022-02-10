#pragma once

#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Experiment/IExperiment.h"

#include "CAFAna/Prediction/IPrediction.h"

#include "CAFAna/PRISM/EigenUtils.h"
#include "CAFAna/PRISM/PredictionPRISM.h"

namespace ana {
  class SimpleChi2Experiment : public IExperiment {
  public:
    SimpleChi2Experiment(
        IPrediction const *Pred, Spectrum const &Data, bool UseHistError = false,
        double POT = 0,
        std::pair<double, double> erange = {-std::numeric_limits<double>::max(),
                                            std::numeric_limits<double>::max()});

    IPrediction const *fPred;
    TH1 const *fData;
    double fPOT;
    bool fUseHistError;
  
    std::pair<int, int> fBinRange;

    TH1D *GetPred(osc::IOscCalcAdjustable *osc,
                  const SystShifts &syst = SystShifts::Nominal()) const;

    double ChiSq(osc::IOscCalcAdjustable *osc,
                 const SystShifts &syst = SystShifts::Nominal()) const;
  };


  //----------------------------------------------------

  // Another Chi2Experiment for Chi2 calculation using full
  // PRISM covariance

  class PRISMChi2CovarExperiment : public IExperiment {
  public:
    PRISMChi2CovarExperiment(PredictionPRISM const *Pred, Spectrum const &Data,
                             bool UseCovariance = false, double NDPOT = 0, double FDPOT = 0,
                             PRISM::MatchChan match_chan = PRISM::kNumuDisappearance_Numode,
                             std::pair<double, double> erange = {-std::numeric_limits<double>::max(),
                                                                 std::numeric_limits<double>::max()});

    // Get the extrapolated PRISM prediction
    Eigen::VectorXd GetPred(osc::IOscCalc *osc,
                            std::map<PredictionPRISM::PRISMComponent, Spectrum> &PRISMComps,
                            const SystShifts &syst = SystShifts::Nominal()) const;
    // Get the PRISM covariance matrix
    Eigen::MatrixXd GetCovariance(osc::IOscCalc *osc,
                                  std::map<PredictionPRISM::PRISMComponent, Spectrum> &PRISMComps,
                                  const SystShifts &syst = SystShifts::Nominal()) const;

    // Calculate Chi2 with the option to include the full covariance
    virtual double ChiSq(osc::IOscCalcAdjustable *osc,
                         const SystShifts &syst = kNoShift) const override;
  protected:
    PredictionPRISM const *fPred;
    Eigen::VectorXd fData_vec;
    double fPOTFD;
    double fPOTND;
    bool fUseCovariance;

    PRISM::MatchChan fMatchChannel;
};



} // namespace ana
