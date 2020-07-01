#include "CAFAna/Experiment/SingleSampleExperiment.h"

#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/StanUtils.h"
#include "CAFAna/Core/Utilities.h"

#include "OscLib/func/IOscCalculator.h"

#include "TDirectory.h"
#include "TObjString.h"
#include "TH2.h"

namespace ana
{
  const CosmicBkgScaleSyst kCosmicBkgScaleSyst;

  //----------------------------------------------------------------------
  SingleSampleExperiment::SingleSampleExperiment(const IPrediction* pred,
                                                 const Spectrum& data,
                                                 const Spectrum& cosmic,
                                                 double cosmicScaleError)
    : fTestStatistic(kLogLikelihood),
      fMC(pred), fData(data),
      fCosmic(cosmic.ToTH1(data.Livetime(), kLivetime)),
      fMask(0), fCosmicScaleError(cosmicScaleError),
      fCovMxInfo(0)
  {
  }

  //----------------------------------------------------------------------
  SingleSampleExperiment::SingleSampleExperiment(const IPrediction* pred,
                                                 const Spectrum& data,
                                                 const TH1D* cosmic,
                                                 double cosmicScaleError)
    : fTestStatistic(kLogLikelihood),
      fMC(pred), fData(data), fCosmic(new TH1D(*cosmic)),
      fMask(0), fCosmicScaleError(cosmicScaleError),
      fCovMxInfo(0)
  {
  }

  //----------------------------------------------------------------------
  SingleSampleExperiment::SingleSampleExperiment(const IPrediction* pred,
                                                 const Spectrum& data,
                                                 const TMatrixD* cov,
                                                 ETestStatistic stat)
    : fTestStatistic(stat),
      fMC(pred), fData(data), fCosmic(0), fMask(0),
      fCovMxInfo(0)
  {
    switch(stat){
    case kLogLikelihood:
      // No need for any matrix
      break;
    case kCovMxChiSq:
      // Store the covariance matrix as-is
      fCovMxInfo = new TMatrixD(*cov);
      break;
    case kCovMxChiSqPreInvert:
      {
        TMatrixD toInvert(*cov);

        TH1D* hist = fMC->Predict(static_cast<osc::IOscCalculator*>(nullptr)).ToTH1(fData.POT());
        for( int b = 0; b < hist->GetNbinsX(); ++b ) {
          // We add the squared fractional statistical errors to the
          // diagonal. In principle this should vary with the predicted number
          // of events, but in the ND using the no-syst-shifts number should be
          // a pretty good approximation, and it's much faster than needing to
          // re-invert the matrix every time.
          const double N = hist->GetBinContent(b+1);
          if(N > 0) toInvert(b, b) += 1/N;
        }
        HistCache::Delete(hist);

        fCovMxInfo = new TMatrixD(TMatrixD::kInverted, toInvert);
      }
      break;
    case kCovMxLogLikelihood:
      // Also pre-invert the matrix but with no stats
      fCovMxInfo = new TMatrixD(TMatrixD::kInverted, *cov);
      break;
    default:
      std::cout << "Unknown test statistic " << stat << std::endl;
      abort();
    }
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
    delete fCosmic;
    delete fMask;
    delete fCovMxInfo;
  }

  //----------------------------------------------------------------------
  TH1D* SingleSampleExperiment::PredHist(osc::IOscCalculator* calc,
                                         const SystShifts& syst) const
  {
    SystShifts systNoCosmic = syst;
    systNoCosmic.SetShift(&kCosmicBkgScaleSyst, 0);

    const Spectrum pred = fMC->PredictSyst(calc, systNoCosmic);

    TH1D* hpred = pred.ToTH1(fData.POT());

    if(fCosmic){
      if(fCosmicScaleError != 0){
        const double scale = 1 + syst.GetShift(&kCosmicBkgScaleSyst) * fCosmicScaleError;
        hpred->Add(fCosmic, scale);
      }
      else{
        hpred->Add(fCosmic);
      }
    }

    return hpred;
  }

  //----------------------------------------------------------------------
  TH1D* SingleSampleExperiment::DataHist() const
  {
    return fData.ToTH1(fData.POT());
  }

  //----------------------------------------------------------------------
  double SingleSampleExperiment::ChiSq(osc::IOscCalculatorAdjustable* calc,
                                       const SystShifts& syst) const
  {
    TH1D* hpred = PredHist(calc, syst);
    TH1D* hdata = fData.ToTH1(fData.POT());

    double ll;

    // if there is a covariance matrix, use it
    if(fCovMxInfo){
      const TMatrixD covInv = GetAbsInvCovMat(hpred);

      // Now the matrix is in order apply the mask to the two histograms
      if(fMask) ApplyMask(hpred, hdata);

      // Now it's absolute it's suitable for use in the chisq calculation
      if(fTestStatistic == kCovMxChiSq ||
         fTestStatistic == kCovMxChiSqPreInvert){
        ll = Chi2CovMx( hpred, hdata, covInv );
      }
      else{
        ll = LogLikelihoodCovMx(hpred, hdata, covInv, &fCovLLState);
      }
    }
    else{
      // No covariance matrix - use standard LL

      if(fMask) ApplyMask(hpred, hdata);

      // full namespace qualification to avoid degeneracy with method inherited from IExperiment
      ll = ana::LogLikelihood(hpred, hdata);
    }

    HistCache::Delete(hpred);
    HistCache::Delete(hdata);

    return ll;
  }

  //----------------------------------------------------------------------
  TMatrixD SingleSampleExperiment::GetAbsInvCovMat(TH1D* hpred) const
  {
    // The inverse relative covariance matrix comes from one of two sources
    // If you don't set the size the assignment operator won't do what you expect.
    TMatrixD covInv(fCovMxInfo->GetNrows(), fCovMxInfo->GetNcols());
    assert(fCovMxInfo->GetNrows() == fCovMxInfo->GetNcols());

    // Array contains the underflow too!
    double* array = hpred->GetArray();
    const int N = hpred->GetNbinsX();

    if(fTestStatistic == kCovMxChiSqPreInvert){
      // Either we precomputed it
      covInv = *fCovMxInfo;
    }
    else{
      // Or we have to manually add statistical uncertainty in quadrature
      TMatrixD cov = *fCovMxInfo;
      for( int b = 0; b < N; ++b ) {
        const double Nevt = array[b+1];
        if(Nevt > 0) cov(b, b) += 1/Nevt;
      }

      // And then invert
      covInv = TMatrixD(TMatrixD::kInverted, cov);
    }

    // In either case - covariance matrix is fractional; convert it to
    // absolute by multiplying out the prediction
    for( int b0 = 0; b0 < N; ++b0 ) {
      for( int b1 = 0; b1 < N; ++b1 ) {
        const double f = array[b0+1] * array[b1+1];
        if(f != 0) covInv(b0, b1) /= f;
        else covInv(b0, b1) = 0.;
      }
    }

    return covInv;
  }

  //----------------------------------------------------------------------
  void SingleSampleExperiment::ApplyMask(TH1* a, TH1* b) const
  {
    if(!fMask) return;

    assert(a->GetNbinsX() == fMask->GetNbinsX());
    assert(b->GetNbinsX() == fMask->GetNbinsX());

    for(int i = 0; i < fMask->GetNbinsX()+2; ++i){
      if(fMask->GetBinContent(i) == 0){
        a->SetBinContent(i, 0);
        b->SetBinContent(i, 0);
      }
    }
  }

  //----------------------------------------------------------------------
  void SingleSampleExperiment::AddCovarianceMatrix(const TMatrixD* cov,
						   ETestStatistic stat) 
  {
    // Check there is not a covariance matrix already associated with this experiment
    if (fCovMxInfo) {
      std::cout << "Error: trying to add a covariance matrix to a SingleSampleExperiment where one already exists" << std::endl;
      abort();
    }
    switch(stat){
    case kLogLikelihood:
      std::cout << "Trying to add a covariance matrix while specifying a Test Statistic method that does not allow it. Matrix not added." << std::endl;
      break;
    case kCovMxChiSq:
      // Store the covariance matrix as-is
      fCovMxInfo = new TMatrixD(*cov);
      break;
    case kCovMxChiSqPreInvert:
      {
        TMatrixD toInvert(*cov);

        TH1D* hist = fMC->Predict(static_cast<osc::IOscCalculator*>(nullptr)).ToTH1(fData.POT());
        for( int b = 0; b < hist->GetNbinsX(); ++b ) {
          // We add the squared fractional statistical errors to the
          // diagonal. In principle this should vary with the predicted number
          // of events, but in the ND using the no-syst-shifts number should be
          // a pretty good approximation, and it's much faster than needing to
          // re-invert the matrix every time.
          const double N = hist->GetBinContent(b+1);
          if(N > 0) toInvert(b, b) += 1/N;
        }
        HistCache::Delete(hist);

        fCovMxInfo = new TMatrixD(TMatrixD::kInverted, toInvert);
      }
      break;
    case kCovMxLogLikelihood:
      // Also pre-invert the matrix but with no stats
      fCovMxInfo = new TMatrixD(TMatrixD::kInverted, *cov);
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
  stan::math::var SingleSampleExperiment::LogLikelihood(osc::_IOscCalculatorAdjustable<stan::math::var> *osc,
                                                        const SystShifts &syst) const
  {
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

    if(fCosmic) fCosmic->Write("cosmic");

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

    TH1D* cosmic = 0;
    if(dir->Get("cosmic")) cosmic = (TH1D*)dir->Get("cosmic");

    delete dir;

    auto ret = std::make_unique<SingleSampleExperiment>(mc, *data);
    if(cosmic) ret->fCosmic = cosmic;
    return ret;
  }

  //----------------------------------------------------------------------
  void SingleSampleExperiment::SetMaskHist(double xmin, double xmax, double ymin, double ymax)
  {
    fMask = GetMaskHist(fData, xmin, xmax, ymin, ymax);
  }
}
