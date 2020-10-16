#include "CAFAna/Experiment/CovarianceExperiment.h"

#include "CAFAna/Experiment/CovMxChiSq.h"
#include "CAFAna/Experiment/CovMxChiSqPreInvert.h"
#include "CAFAna/Experiment/CovMxLL.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/StanUtils.h"
#include "CAFAna/Core/Utilities.h"

#include "OscLib/IOscCalc.h"

#include "TDirectory.h"
#include "TObjString.h"
#include "TH2.h"

namespace ana
{
  //----------------------------------------------------------------------
  TMatrixD* CovarianceExperiment::GetCov(const std::string& fname,
                                         const std::string& matname)
  {
    TDirectory* thisDir = gDirectory->CurrentDirectory();

    TFile f(fname.c_str());
    TMatrixD* ret = (TMatrixD*)f.Get(matname.c_str());

    if(!ret){
      std::cout << "Could not obtain covariance matrix named "
                << matname << " from " << fname << std::endl;
      abort();
    }

    thisDir->cd();

    return ret;
  }

  //----------------------------------------------------------------------
  CovarianceExperiment::CovarianceExperiment(const IPrediction* pred,
                                             const Spectrum& data,
                                             const TMatrixD* cov,
                                             ETestStatistic stat)
    : fMC(pred), fData(data)
  {
    switch(stat){
    case kCovMxChiSq:
      fCov = new CovMxChiSq(EigenMatrixXdFromTMatrixD(cov));
      break;

    case kCovMxChiSqPreInvert:
      fCov = new CovMxChiSqPreInvert(EigenMatrixXdFromTMatrixD(cov),
                                     pred->Predict((osc::IOscCalc*)0).GetEigen(data.POT()));
      break;

    case kCovMxLogLikelihood:
      fCov = new CovMxLL(EigenMatrixXdFromTMatrixD(cov));
      break;

    default:
      std::cout << "Unknown test statistic: " << stat << std::endl;
      abort();
    }
  }

  //----------------------------------------------------------------------
  CovarianceExperiment::CovarianceExperiment(const IPrediction* pred,
                                             const Spectrum& data,
                                             const std::string& covMatFilename,
                                             const std::string& covMatName,
                                             ETestStatistic stat)

    : CovarianceExperiment(pred, data, GetCov(covMatFilename, covMatName), stat)
  {
  }

  //----------------------------------------------------------------------
  CovarianceExperiment::~CovarianceExperiment()
  {
    delete fCov;
  }

  //----------------------------------------------------------------------
  double CovarianceExperiment::ChiSq(osc::IOscCalcAdjustable* calc,
                                     const SystShifts& syst) const
  {
    Eigen::ArrayXd apred = fMC->PredictSyst(calc, syst).GetEigen(fData.POT());
    Eigen::ArrayXd adata = fData.GetEigen(fData.POT());

    return fCov->ChiSq(apred, adata);
  }

  //----------------------------------------------------------------------
  void CovarianceExperiment::ApplyMask(Eigen::ArrayXd& a,
                                       Eigen::ArrayXd& b) const
  {
    if(fMaskA.size() == 0) return;

    assert(a.size() == fMaskA.size());
    assert(b.size() == fMaskA.size());

    // Arrays mean we get bin-by-bin operations
    a *= fMaskA;
    b *= fMaskA;
  }

  //----------------------------------------------------------------------
  stan::math::var CovarianceExperiment::LogLikelihood(osc::IOscCalcAdjustableStan *osc,
                                                      const SystShifts &syst) const
  {
    std::cout << "CovarianceExperiment doesn't yet support OscCalcStan" << std::endl;
    abort();
  }

  //----------------------------------------------------------------------
  void CovarianceExperiment::SetMaskHist(double xmin, double xmax, double ymin, double ymax)
  {
    fMaskA = GetMaskArray(fData, xmin, xmax, ymin, ymax);
    fCov->SetMask(fMaskA);
  }
}
