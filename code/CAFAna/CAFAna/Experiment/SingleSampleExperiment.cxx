#include "CAFAna/Experiment/SingleSampleExperiment.h"

#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Utilities.h"

#include "OscLib/func/IOscCalculator.h"

#include "TDirectory.h"
#include "TObjString.h"
#include "TH1.h"
#include "TH2.h"

namespace ana
{
  const CosmicBkgScaleSyst kCosmicBkgScaleSyst;

  //----------------------------------------------------------------------
  SingleSampleExperiment::SingleSampleExperiment(const IPrediction* pred,
                                                 const Spectrum& data,
                                                 const Spectrum& cosmic,
                                                 double cosmicScaleError)
    : fMC(pred), fData(data),
      fCosmic(cosmic.ToTH1(data.Livetime(), kLivetime)),
      fMask(0), fCosmicScaleError(cosmicScaleError),
      fCovMx(0), fCovMxInv(0), fPreInvert(0)
  {
  }

  //----------------------------------------------------------------------
  SingleSampleExperiment::SingleSampleExperiment(const IPrediction* pred,
                                                 const Spectrum& data,
                                                 const TH1D* cosmic,
                                                 double cosmicScaleError)
    : fMC(pred), fData(data), fCosmic(new TH1D(*cosmic)),
      fMask(0), fCosmicScaleError(cosmicScaleError),
      fCovMx(0), fCovMxInv(0), fPreInvert(0)
  {
  }

  //----------------------------------------------------------------------
  SingleSampleExperiment::SingleSampleExperiment(const IPrediction* pred,
                                                 const Spectrum& data,
                                                 const TMatrixD* cov,
                                                 const bool preInvert)
    : fMC(pred), fData(data), fCosmic(0), fMask(0), fCovMx(new TMatrixD(*cov)) 
  {
    fPreInvert = preInvert;
    if( fPreInvert ) InitInverseMatrix();
    else fCovMxInv = 0;
  }

  //----------------------------------------------------------------------
  SingleSampleExperiment::SingleSampleExperiment(const IPrediction* pred,
                                                 const Spectrum& data,
                                                 const std::string covMatFilename,
                                                 const std::string covMatName,
                                                 const bool preInvert)

    : fMC(pred), fData(data), fCosmic(0), fMask(0)
  {
    fPreInvert = preInvert;
    TDirectory *thisDir = gDirectory->CurrentDirectory();

    TFile covMatFile( covMatFilename.c_str() );
    fCovMx = (TMatrixD*) covMatFile.Get( covMatName.c_str() );

    if( !fCovMx ) {
      std::cout << "Could not obtain covariance matrix named "
                << covMatName << " from " << covMatFilename << std::endl;
      abort();
    }

    if( fPreInvert ) InitInverseMatrix();
    else fCovMxInv = 0;

    thisDir->cd();
  }

  //----------------------------------------------------------------------
  SingleSampleExperiment::~SingleSampleExperiment()
  {
    delete fCosmic;
    delete fMask;
    delete fCovMx;
    delete fCovMxInv;
  }

  //----------------------------------------------------------------------
  void SingleSampleExperiment::InitInverseMatrix()
  {
    TMatrixD toInvert( *fCovMx );

    TH1D* hist = fMC->Predict(0).ToTH1(fData.POT());
    for( int b = 0; b < hist->GetNbinsX(); ++b ) {
      // We add the squared fractional statistical errors to the diagonal. In
      // principle this should vary with the predicted number of events, but in
      // the ND using the no-syst-shifts number should be a pretty good
      // approximation, and it's much faster than needing to re-invert the
      // matrix every time.
      const double N = hist->GetBinContent(b+1);
      if(N > 0) toInvert(b, b) += 1/N;
    }
    HistCache::Delete(hist);

    fCovMxInv = new TMatrixD(TMatrixD::kInverted, toInvert);
  }

  //----------------------------------------------------------------------
  TH1D* SingleSampleExperiment::
  PredHistIncCosmics(osc::IOscCalculator* calc,
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
  double SingleSampleExperiment::ChiSq(osc::IOscCalculatorAdjustable* calc,
                                       const SystShifts& syst) const
  {
    TH1D* hpred = PredHistIncCosmics(calc, syst);
    TH1D* hdata = fData.ToTH1(fData.POT());

    // if there is a covariance matrix, use it
    double ll;
    if( fCovMxInv && fPreInvert ) { // Use pre-inverted covariance matrix
      TMatrixD absCovInv( *fCovMxInv );
      // Input covariance matrix is fractional; convert it to absolute by multiplying out the prediction
      double* array = hpred->GetArray();
      const int N = hpred->GetNbinsX();
      for( int b0 = 0; b0 < N; ++b0 ) {
        for( int b1 = 0; b1 < N; ++b1 ) {
          const double f = array[b0] * array[b1];
          if(f != 0) absCovInv(b0, b1) /= f;
        }
      }

      // Mask after the ND covariance is dealt with
      if (fMask){
        assert(hpred->GetNbinsX() == fMask->GetNbinsX());
        assert(hdata->GetNbinsX() == fMask->GetNbinsX());

        for(int i = 0; i < fMask->GetNbinsX()+2; ++i){
          if (fMask->GetBinContent(i+1) == 1) continue;
          hpred->SetBinContent(i+1, 0);
          hdata->SetBinContent(i+1, 0);
        }
      }

      ll = Chi2CovMx( hpred, hdata, absCovInv );
    } else if( fCovMx && !fPreInvert ) { // covariance matrix must be inverted each time

      TMatrixD absCov( *fCovMx );

      // Input covariance matrix is fractional; convert it to absolute by multiplying out the prediction
      double* array = hpred->GetArray();
      const int N = hpred->GetNbinsX();
      for( int b0 = 0; b0 < N; ++b0 ) {
        for( int b1 = 0; b1 < N; ++b1 ) {
          absCov(b0, b1) *= (array[b0] * array[b1]);
        }
        // Add statistical uncertainty in quadrature
        absCov(b0, b0) += array[b0];
      }

      // Mask after the ND covariance is dealt with
      if (fMask){
        assert(hpred->GetNbinsX() == fMask->GetNbinsX());
        assert(hdata->GetNbinsX() == fMask->GetNbinsX());

        for(int i = 0; i < fMask->GetNbinsX()+2; ++i){
          if (fMask->GetBinContent(i+1) == 1) continue;
          hpred->SetBinContent(i+1, 0);
          hdata->SetBinContent(i+1, 0);
        }
      }

      ll = Chi2CovMx( hpred, hdata, TMatrixD(TMatrixD::kInverted, absCov) );
    } else {
      // Still have to mask
      if (fMask){
        assert(hpred->GetNbinsX() == fMask->GetNbinsX());
        assert(hdata->GetNbinsX() == fMask->GetNbinsX());

        for(int i = 0; i < fMask->GetNbinsX()+2; ++i){
          if (fMask->GetBinContent(i+1) == 1) continue;
          hpred->SetBinContent(i+1, 0);
          hdata->SetBinContent(i+1, 0);
        }
      }

      ll = LogLikelihood(hpred, hdata);
    }

    HistCache::Delete(hpred);
    HistCache::Delete(hdata);

    return ll;
  }

  //----------------------------------------------------------------------
  void SingleSampleExperiment::
  Derivative(osc::IOscCalculator* calc,
             const SystShifts& shift,
             std::unordered_map<const ISyst*, double>& dchi) const
  {
    const double pot = fData.POT();

    std::unordered_map<const ISyst*, std::vector<double>> dp;
    for(auto it: dchi) dp[it.first] = {};
    fMC->Derivative(calc, shift, pot, dp);

    if(dp.empty()){ // prediction doesn't implement derivatives
      dchi.clear(); // pass on that info to our caller
      return;
    }

    TH1D* hpred = PredHistIncCosmics(calc, shift);
    TH1D* hdata = fData.ToTH1(pot);

    for(auto& it: dchi){
      if(it.first != &kCosmicBkgScaleSyst){
        it.second += LogLikelihoodDerivative(hpred, hdata, dp[it.first]);
      }
      else{
        const unsigned int N = fCosmic->GetNbinsX()+2;
        const double* ca = fCosmic->GetArray();
        std::vector<double> cosErr(N);
        for(unsigned int i = 0; i < N; ++i) cosErr[i] = ca[i]*fCosmicScaleError;
        it.second += LogLikelihoodDerivative(hpred, hdata, cosErr);
      }
    }

    HistCache::Delete(hpred);
    HistCache::Delete(hdata);
  }

  //----------------------------------------------------------------------
  void SingleSampleExperiment::SaveTo(TDirectory* dir) const
  {
    TDirectory* tmp = dir;

    dir->cd();
    TObjString("SingleSampleExperiment").Write("type");

    fMC->SaveTo(dir->mkdir("mc"));
    fData.SaveTo(dir->mkdir("data"));

    if(fCosmic) fCosmic->Write("cosmic");

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<SingleSampleExperiment> SingleSampleExperiment::LoadFrom(TDirectory* dir)
  {
    TObjString* ptag = (TObjString*)dir->Get("type");
    assert(ptag);
    assert(ptag->GetString() == "SingleSampleExperiment");

    assert(dir->GetDirectory("mc"));
    assert(dir->GetDirectory("data"));
    

    const IPrediction* mc = ana::LoadFrom<IPrediction>(dir->GetDirectory("mc")).release();
    const std::unique_ptr<Spectrum> data = Spectrum::LoadFrom(dir->GetDirectory("data"));

    TH1D* cosmic = 0;
    if(dir->Get("cosmic")) cosmic = (TH1D*)dir->Get("cosmic");

    auto ret = std::make_unique<SingleSampleExperiment>(mc, *data);
    if(cosmic) ret->fCosmic = cosmic;
    return ret;
  }

  void SingleSampleExperiment::SetMaskHist(double xmin, double xmax, double ymin, double ymax)
  {
    fMask = GetMaskHist(fData, xmin, xmax, ymin, ymax);
  }
}
