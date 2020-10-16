#pragma once

#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Experiment/IExperiment.h"

template<class T> class TMatrixT;
typedef TMatrixT<double> TMatrixD;

namespace ana
{
  class ICovarianceMatrix;

  enum ETestStatistic{
    kCovMxChiSq,
    kCovMxChiSqPreInvert, ///< good approximation for ND
    kCovMxLogLikelihood ///< for FD
  };

  /// Compare a single data spectrum to the MC expectation
  class CovarianceExperiment: public IExperiment
  {
  public:
    CovarianceExperiment(const std::vector<const IPrediction*>& preds,
                         const std::vector<Spectrum>& datas,
                         const TMatrixD* cov,
                         ETestStatistic stat);

    CovarianceExperiment(const IPrediction* pred,
                         const Spectrum& data,
                         const TMatrixD* cov,
                         ETestStatistic stat)
      : CovarianceExperiment(std::vector<const IPrediction*>(1, pred),
                             std::vector<Spectrum>(1, data),
                             cov, stat)
    {
    }

    /// Covariance matrix from file path
    CovarianceExperiment(const std::vector<const IPrediction*>& preds,
                         const std::vector<Spectrum>& datas,
                         const std::string& covMatFilename,
                         const std::string& covMatName,
                         ETestStatistic stat)
      : CovarianceExperiment(preds, datas, GetCov(covMatFilename, covMatName), stat)
    {
    }

    /// Covariance matrix from file path
    CovarianceExperiment(const IPrediction* pred,
                         const Spectrum& data,
                         const std::string& covMatFilename,
                         const std::string& covMatName,
                         ETestStatistic stat)
      : CovarianceExperiment(pred, data, GetCov(covMatFilename, covMatName), stat)
    {
    }

    virtual ~CovarianceExperiment();

    virtual double ChiSq(osc::IOscCalcAdjustable* osc,
                         const SystShifts& syst = kNoShift) const override;

    stan::math::var LogLikelihood(osc::_IOscCalcAdjustable<stan::math::var> *osc,
                                  const SystShifts &syst = kNoShift) const override;

    // Didn't make provisions for copying fMC
    CovarianceExperiment(const CovarianceExperiment&) = delete;
    CovarianceExperiment& operator=(const CovarianceExperiment&) = delete;

    // need to explicitly declare move constructor since copy constructor is deleted
    CovarianceExperiment(CovarianceExperiment&& s)
    {
      std::swap(fMCs, s.fMCs);
      std::swap(fDatas, s.fDatas);
    }

    void SetMaskHist(int idx,
                     double xmin=0, double xmax=-1, 
		     double ymin=0, double ymax=-1);

  protected:
    /// Helper for constructor
    static TMatrixD* GetCov(const std::string& fname,
                            const std::string& matname);

    /// NB handling of under/overflow here. Result of concatenating N M-bin
    /// arrays will have 2+N*(M-2) bins
    static Eigen::ArrayXd Concatenate(const std::vector<Eigen::ArrayXd>& arrs);

    Eigen::ArrayXd Predict(osc::IOscCalc* osc,
                           const SystShifts& syst = kNoShift) const;

    std::vector<const IPrediction*> fMCs;
    std::vector<Spectrum> fDatas;
    Eigen::ArrayXd fDataA;

    std::vector<Eigen::ArrayXd> fMasks;

    ICovarianceMatrix* fCov;
  };
}
