#pragma once

#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Experiment/IExperiment.h"
#include "CAFAna/PRISM/EigenUtils.h"
#include "CAFAna/PRISM/PredictionPRISM.h"

namespace ana {

  // Chi2Experiment for Chi2 calculation using full
  // PRISM covariance

  class PRISMChi2CovarExperiment: public IExperiment
  {
  public:
    /// \param pred   Source of oscillated MC beam predictions
    /// \param data   Data spectrum to compare to
    PRISMChi2CovarExperiment(const PredictionPRISM* pred,
                             const Spectrum& data, bool UseCovariance = false,
                             double NDPOT = 0, double FDPOT = 0,
                             PRISM::MatchChan match_chan = PRISM::kNumuDisappearance_Numode);

    virtual ~PRISMChi2CovarExperiment();

    virtual double ChiSq(osc::IOscCalcAdjustable* osc,
                         const SystShifts& syst = kNoShift) const override;

    double ValidChiSq(Eigen::VectorXd &obs, Eigen::VectorXd &exp, Eigen::MatrixXd &cov, const SystShifts& syst = kNoShift) const;
    virtual void SaveTo(TDirectory* dir, const std::string& name) const override;
    static std::unique_ptr<PRISMChi2CovarExperiment> LoadFrom(TDirectory* dir, const std::string& name);

    // Didn't make provisions for copying fPred
    PRISMChi2CovarExperiment(const PRISMChi2CovarExperiment&) = delete;
    PRISMChi2CovarExperiment& operator=(const PRISMChi2CovarExperiment&) = delete;

    // need to explicitly declare move constructor since copy constructor is deleted
    PRISMChi2CovarExperiment(PRISMChi2CovarExperiment&& s)
      : fPred(s.fPred), fData(std::move(s.fData))
    {
      s.fPred = nullptr;
    }

    // Set boundaries of fit. We remove bins we don't want rather than
    // setting them to zero. This prevents us calculating nonsense Chi2
    void SetFitBoundaries(double xmin=0, double xmax=-1, 
		          double ymin=0, double ymax=-1);

  protected:
    const PredictionPRISM* fPred;
    Spectrum fData;
    bool fUseCovariance;
    double fPOTND;
    double fPOTFD;
    PRISM::MatchChan fMatchChannel;

    std::pair<double, double> pFitBoundry;
  };

} // namespace ana
