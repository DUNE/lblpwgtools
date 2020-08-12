#include "CAFAna/Experiment/SingleSampleExperiment.h"

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
  SingleSampleExperiment::SingleSampleExperiment(const IPrediction* pred,
                                                 const Spectrum& data)
    : fTestStatistic(kLogLikelihood),
      fMC(pred), fData(data)
  {
  }

  //----------------------------------------------------------------------
  SingleSampleExperiment::SingleSampleExperiment(const IPrediction* pred,
                                                 const Spectrum& data,
                                                 const TMatrixD* cov,
                                                 ETestStatistic stat)
    : fTestStatistic(stat),
      fMC(pred), fData(data)
  {
    AddCovarianceMatrix(cov, stat);
  }

  // Helper for constructor
  TMatrixD* GetCov(const std::string& fname, const std::string& matname)
  {
   
    TDirectory *thisDir = gDirectory->CurrentDirectory();

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
  SingleSampleExperiment::SingleSampleExperiment(const IPrediction* pred,
                                                 const Spectrum& data,
                                                 const std::string covMatFilename,
                                                 const std::string covMatName,
                                                 ETestStatistic stat)

    : SingleSampleExperiment(pred, data,
                             GetCov(covMatFilename, covMatName),
                             stat)
  {
  }

  //----------------------------------------------------------------------
  SingleSampleExperiment::~SingleSampleExperiment()
  {
  }

  //----------------------------------------------------------------------
  double SingleSampleExperiment::ChiSq(osc::IOscCalcAdjustable* calc,
                                       const SystShifts& syst) const
  {
    Eigen::ArrayXd apred = fMC->PredictSyst(calc, syst).GetEigen(fData.POT());
    Eigen::ArrayXd adata = fData.GetEigen(fData.POT());

    double ll = 0;

    // if there is a covariance matrix, use it
    if(fCovMxInfoM.size() != 0){
      const Eigen::MatrixXd covInvM = GetAbsInvCovMat(apred);

      // Now the matrix is in order apply the mask to the two histograms
      ApplyMask(apred, adata);

      // Now it's absolute it's suitable for use in the chisq calculation
      if(fTestStatistic == kCovMxChiSq ||
         fTestStatistic == kCovMxChiSqPreInvert){
        ll = Chi2CovMx(apred, adata, covInvM);
      }
      else{
        ll = LogLikelihoodCovMx(apred, adata, covInvM, &fCovLLState);
      }
    }
    else{
      // No covariance matrix - use standard LL
      ApplyMask(apred, adata);

      // full namespace qualification to avoid degeneracy with method inherited from IExperiment
      ll = ana::LogLikelihood(apred, adata);
    }

    return ll;
  }

  //----------------------------------------------------------------------
  Eigen::MatrixXd SingleSampleExperiment::GetAbsInvCovMat(const Eigen::ArrayXd& apred) const
  {
    Eigen::MatrixXd covInv(fCovMxInfoM.rows(), fCovMxInfoM.cols());

    const int N = apred.size()-2; // no under/overflow

    // The inverse relative covariance matrix comes from one of two sources
    if(fTestStatistic == kCovMxChiSqPreInvert){
      // Either we precomputed it
      covInv = fCovMxInfoM;
    }
    else{
      // Or we have to manually add statistical uncertainty in quadrature
      Eigen::MatrixXd cov = fCovMxInfoM;
      for( int b = 0; b < N; ++b ) {
        const double Nevt = apred[b+1];
        if(Nevt > 0) cov(b, b) += 1/Nevt;
      }

      // And then invert
      covInv = cov.inverse();
    }

    // In either case - covariance matrix is fractional; convert it to
    // absolute by multiplying out the prediction
    //
    // TODO there is probably a nice Eigen expression that will do this
    for( int b0 = 0; b0 < N; ++b0 ) {
      for( int b1 = 0; b1 < N; ++b1 ) {
        const double f = apred[b0+1] * apred[b1+1];
        if(f != 0) covInv.coeffRef(b0, b1) /= f;
        else covInv.coeffRef(b0, b1) = 0.;
      }
    }

    return covInv;
  }

  //----------------------------------------------------------------------
  void SingleSampleExperiment::ApplyMask(Eigen::ArrayXd& a,
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
  void SingleSampleExperiment::AddCovarianceMatrix(const TMatrixD* cov,
						   ETestStatistic stat) 
  {
    if(fCovMxInfoM.size() > 0) {
      std::cout << "Error: trying to add a covariance matrix to a SingleSampleExperiment where one already exists" << std::endl;
      abort();
    }

    switch(stat){
    case kLogLikelihood:
      // No need for any matrix
      break;
    case kCovMxChiSq:
      // Store the covariance matrix as-is
      fCovMxInfoM = EigenMatrixXdFromTMatrixD(cov);
      break;
    case kCovMxChiSqPreInvert:
      {
      fCovMxInfoM = EigenMatrixXdFromTMatrixD(cov);

      const Eigen::ArrayXd pred = fMC->Predict((osc::IOscCalc*)0).GetEigen(fData.POT());

      // We add the squared fractional statistical errors to the diagonal. In
      // principle this should vary with the predicted number of events, but in
      // the ND using the no-syst-shifts number should be a pretty good
      // approximation, and it's much faster than needing to re-invert the
      // matrix every time.
      for(int b = 0; b < pred.size(); ++b){
        if(pred[b] > 0) fCovMxInfoM.coeffRef(b, b) += 1/pred[b];
      }

      fCovMxInfoM = fCovMxInfoM.inverse();
      }
      break;
    case kCovMxLogLikelihood:
      // Also pre-invert the matrix but with no stats
      fCovMxInfoM = EigenMatrixXdFromTMatrixD(cov).inverse();
      break;
    default:
      std::cout << "Unknown test statistic " << stat << std::endl;
      abort();
    }
  }

  //----------------------------------------------------------------------
  void SingleSampleExperiment::AddCovarianceMatrix(const std::string covMatFilename,
						   const std::string covMatName,
						   ETestStatistic stat) 
  {
    AddCovarianceMatrix(GetCov(covMatFilename, covMatName), stat);
  }

  //----------------------------------------------------------------------
  stan::math::var SingleSampleExperiment::LogLikelihood(osc::IOscCalcAdjustableStan *osc,
                                                        const SystShifts &syst) const
  {
    if(fCovMxInfoM.size() != 0){
      std::cout << "SingleSampleExperiment doesn't yet support the combination of covariance matrix and OscCalcStan" << std::endl;
      abort();
    }

    const Spectrum pred = fMC->PredictSyst(osc, syst);

    const Eigen::ArrayXd data = fData.GetEigen(fData.POT());

    // It's possible to have a non-stan prediction. e.g. from a NoOsc
    // prediction with no systs.
    if(pred.HasStan()){
      // fully-qualified so that we get the one in StanUtils.h
      //
      // LogLikelihood(), confusingly, returns chi2=-2*LL
      return ana::LogLikelihood(pred.GetEigenStan(fData.POT()), data) / -2.;
    }
    else{
      return ana::LogLikelihood(pred.GetEigen(fData.POT()), data) / -2.;
    }
  }


  //----------------------------------------------------------------------
  void SingleSampleExperiment::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = dir;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("SingleSampleExperiment").Write("type");

    fMC->SaveTo(dir, "mc");
    fData.SaveTo(dir, "data");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<SingleSampleExperiment> SingleSampleExperiment::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    TObjString* ptag = (TObjString*)dir->Get("type");
    assert(ptag);
    assert(ptag->GetString() == "SingleSampleExperiment");
    delete ptag;

    const IPrediction* mc = ana::LoadFrom<IPrediction>(dir, "mc").release();
    const std::unique_ptr<Spectrum> data = Spectrum::LoadFrom(dir, "data");

    delete dir;

    auto ret = std::make_unique<SingleSampleExperiment>(mc, *data);
    return ret;
  }

  //----------------------------------------------------------------------
  void SingleSampleExperiment::SetMaskHist(double xmin, double xmax, double ymin, double ymax)
  {
    fMaskA = GetMaskArray(fData, xmin, xmax, ymin, ymax);
  }
}
