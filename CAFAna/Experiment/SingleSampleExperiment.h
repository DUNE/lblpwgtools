#pragma once

#include "CAFAna/Experiment/IExperiment.h"
#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Spectrum.h"

namespace ana
{
  enum ETestStatistic{
    kLogLikelihood, ///< No covariance matrix
    kCovMxChiSq,
    kCovMxChiSqPreInvert, ///< good approximation for ND
    kCovMxLogLikelihood ///< for FD
  };

  /// Compare a single data spectrum to the MC expectation
  class SingleSampleExperiment: public IExperiment
  {
  public:
    /// \param pred   Source of oscillated MC beam predictions
    /// \param data   Data spectrum to compare to
    SingleSampleExperiment(const IPrediction* pred,
                           const Spectrum& data);

    /// Include a covariance matrix
    SingleSampleExperiment(const IPrediction* pred,
                           const Spectrum& data,
                           const TMatrixD* cov,
                           ETestStatistic stat);

    /// Include a covariance matrix file path
    SingleSampleExperiment(const IPrediction* pred,
                           const Spectrum& data,
                           const std::string covMatFilename,
                           const std::string covMatName,
                           ETestStatistic stat);

    virtual ~SingleSampleExperiment();

    virtual double ChiSq(osc::IOscCalcAdjustable* osc,
                         const SystShifts& syst = kNoShift) const override;

    stan::math::var LogLikelihood(osc::_IOscCalcAdjustable<stan::math::var> *osc,
                                  const SystShifts &syst = kNoShift) const override;

      virtual void SaveTo(TDirectory* dir, const std::string& name) const override;
    static std::unique_ptr<SingleSampleExperiment> LoadFrom(TDirectory* dir, const std::string& name);

    // Didn't make provisions for copying fMC
    SingleSampleExperiment(const SingleSampleExperiment&) = delete;
    SingleSampleExperiment& operator=(const SingleSampleExperiment&) = delete;

    // need to explicitly declare move constructor since copy constructor is deleted
    SingleSampleExperiment(SingleSampleExperiment&& s)
      : fMC(s.fMC), fData(std::move(s.fData))
    {
      s.fMC = nullptr;
    };

    // Add in a covariance matrix to an existing SingleSampleExperiment 
    // Only works with the uncorrelated matrices
    void AddCovarianceMatrix(const TMatrixD* cov,
			     ETestStatistic stat);

    void AddCovarianceMatrix(const std::string covMatFilename,
			     const std::string covMatName,
			     ETestStatistic stat);

    void SetMaskHist(double xmin=0, double xmax=-1, 
		     double ymin=0, double ymax=-1);
  protected:
    virtual void ApplyMask(Eigen::ArrayXd& a, Eigen::ArrayXd& b) const;

    Eigen::MatrixXd GetAbsInvCovMat(const Eigen::ArrayXd& apred) const;

    ETestStatistic fTestStatistic;

    const IPrediction* fMC;
    Spectrum fData;
    Eigen::ArrayXd fMaskA;

    Eigen::MatrixXd fCovMxInfoM; ///< Represents different things depending on fTestStatistic

    mutable std::vector<double> fCovLLState;
  };
}
