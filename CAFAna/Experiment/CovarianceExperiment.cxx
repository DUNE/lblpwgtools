#include "CAFAna/Experiment/CovarianceExperiment.h"

#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Experiment/CovMxChiSq.h"
#include "CAFAna/Experiment/CovMxChiSqPreInvert.h"
#include "CAFAna/Experiment/CovMxLL.h"

#include "OscLib/IOscCalc.h"

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
  CovarianceExperiment::
  CovarianceExperiment(const std::vector<const IPrediction*>& preds,
                       const std::vector<Spectrum>& datas,
                       const TMatrixD* cov,
                       ETestStatistic stat)
    : fMCs(preds), fDatas(datas)
  {
    assert(preds.size() == datas.size());

    switch(stat){
    case kCovMxChiSq:
      fCov = new CovMxChiSq(EigenMatrixXdFromTMatrixD(cov));
      break;

    case kCovMxChiSqPreInvert:
      fCov = new CovMxChiSqPreInvert(EigenMatrixXdFromTMatrixD(cov), Predict(0));
      break;

    case kCovMxLogLikelihood:
      fCov = new CovMxLL(EigenMatrixXdFromTMatrixD(cov));
      break;

    default:
      std::cout << "Unknown test statistic: " << stat << std::endl;
      abort();
    }

    std::vector<Eigen::ArrayXd> adatas;
    adatas.reserve(fDatas.size());
    for(const Spectrum& s: fDatas) adatas.push_back(s.GetEigen(s.POT()));
    fDataA = Concatenate(adatas);

    // To begin with all bins are included
    fMasks.resize(datas.size());
    // Default arguments evaluate to all unmasked
    for(unsigned int i = 0; i < datas.size(); ++i) SetMaskHist(i);
  }

  //----------------------------------------------------------------------
  CovarianceExperiment::~CovarianceExperiment()
  {
    delete fCov;
  }

  //----------------------------------------------------------------------
  Eigen::ArrayXd CovarianceExperiment::Concatenate(const std::vector<Eigen::ArrayXd>& arrs)
  {
    // Drop under/overflow from each argument, but include dummy
    // under/overflows as expected by fCov
    int N = 2;
    for(const Eigen::ArrayXd& arr: arrs) N += arr.size()-2;
    Eigen::ArrayXd ret(N);
    ret[0] = 0;
    ret[N-1] = 0;

    double* p = &ret.data()[1]; // start writing at first non-underflow bin

    for(const Eigen::ArrayXd& arr: arrs){
      for(int i = 1; i < arr.size()-1; ++i) *p++ = arr[i];
    }

    return ret;
  }

  //----------------------------------------------------------------------
  Eigen::ArrayXd CovarianceExperiment::Predict(osc::IOscCalc* calc,
                                               const SystShifts& syst) const
  {
    std::vector<Eigen::ArrayXd> apreds(fMCs.size());

    for(unsigned int i = 0; i < fMCs.size(); ++i){
      apreds[i] = fMCs[i]->PredictSyst(calc, syst).GetEigen(fDatas[i].POT());
    }

    return Concatenate(apreds);
  }

  //----------------------------------------------------------------------
  double CovarianceExperiment::ChiSq(osc::IOscCalcAdjustable* calc,
                                     const SystShifts& syst) const
  {
    return fCov->ChiSq(Predict(calc, syst), fDataA);
  }

  //----------------------------------------------------------------------
  stan::math::var CovarianceExperiment::LogLikelihood(osc::IOscCalcAdjustableStan *osc,
                                                      const SystShifts &syst) const
  {
    std::cout << "CovarianceExperiment doesn't yet support OscCalcStan" << std::endl;
    abort();
  }

  //----------------------------------------------------------------------
  void CovarianceExperiment::SetMaskHist(int idx, double xmin, double xmax, double ymin, double ymax)
  {
    fMasks[idx] = GetMaskArray(fDatas[idx], xmin, xmax, ymin, ymax);

    fCov->SetMask(Concatenate(fMasks));
  }
}
