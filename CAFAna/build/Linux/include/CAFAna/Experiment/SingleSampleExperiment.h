#pragma once

#include "CAFAna/Experiment/IChiSqExperiment.h"
#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Core/Spectrum.h"

namespace ana
{
  /// Dummy syst to communicate with \ref SingleSampleExperiment
  class CosmicBkgScaleSyst: public ISyst
  {
  public:
    CosmicBkgScaleSyst() : ISyst("cosmicScale", "Cosmic background scale") {}
    void Shift(double, Restorer&, caf::StandardRecord*, double&) const {}
  };

  extern const CosmicBkgScaleSyst kCosmicBkgScaleSyst;

  enum ETestStatistic{
    kLogLikelihood, ///< No covariance matrix
    kCovMxChiSq,
    kCovMxChiSqPreInvert, ///< good approximation for ND
    kCovMxLogLikelihood ///< for FD
  };

  /// Compare a single data spectrum to the MC + cosmics expectation
  class SingleSampleExperiment: public IChiSqExperiment
  {
  public:
    /// \param pred   Source of oscillated MC beam predictions
    /// \param data   Data spectrum to compare to
    /// \param cosmic Cosmic ray background component
    /// \param cosmicScaleError fractional uncertainty on cosmic normalization
    SingleSampleExperiment(const IPrediction* pred,
                           const Spectrum& data,
                           const Spectrum& cosmic,
                           double cosmicScaleError = 0);

    /// \brief Fallback to manual cosmic scaling
    ///
    /// \a cosmic must be already scaled so that its bin contents can be
    /// directly summed onto \a data. If you're using the out-of-time part of
    /// the beam spill, the easiest thing to do is to pass \ref
    /// kTimingSidebandWeight as the weight argument when you fill it.
    SingleSampleExperiment(const IPrediction* pred,
                           const Spectrum& data,
                           const TH1D* cosmic,
                           double cosmicScaleError = 0);

    /// In MC studies you might not want to bother with cosmics
    SingleSampleExperiment(const IPrediction* pred,
                           const Spectrum& data)
      : fTestStatistic(kLogLikelihood), fMC(pred), fData(data), fCosmic(0), fMask(0), fCovMxInfo(0)
    {
    }

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

    virtual double ChiSq(osc::IOscCalculatorAdjustable* osc,
                         const SystShifts& syst = SystShifts::Nominal()) const override;

    virtual void Derivative(osc::IOscCalculator* calc,
                            const SystShifts& shift,
                            std::unordered_map<const ISyst*, double>& dch) const override;

    virtual void SaveTo(TDirectory* dir) const override;
    static std::unique_ptr<SingleSampleExperiment> LoadFrom(TDirectory* dir);

    // Didn't make provisions for copying fCosmic or fMC
    SingleSampleExperiment(const SingleSampleExperiment&) = delete;
    SingleSampleExperiment& operator=(const SingleSampleExperiment&) = delete;

    // need to explicitly declare move constructor since copy constructor is deleted
    SingleSampleExperiment(SingleSampleExperiment&& s)
      : fMC(s.fMC), fData(std::move(s.fData)), fCosmic(s.fCosmic), fCosmicScaleError(s.fCosmicScaleError)
    {
      s.fMC = nullptr;
      s.fCosmic = nullptr;
      s.fCosmicScaleError = 0;
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

    virtual void ApplyMask(TH1* a, TH1* b) const override;

    virtual TH1D* PredHist(osc::IOscCalculator* calc,
                           const SystShifts& syst) const override;
    virtual TH1D* DataHist() const override;

  protected:
    TMatrixD GetAbsInvCovMat(TH1D* hpred) const;

    ETestStatistic fTestStatistic;

    const IPrediction* fMC;
    Spectrum fData;
    TH1D* fCosmic;
    TH1* fMask;

    double fCosmicScaleError;


    TMatrixD* fCovMxInfo; ///< Represents different things depending on fTestStatistic


    mutable std::vector<double> fCovLLState;
  };
}
