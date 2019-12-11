#include "CAFAna/Experiment/MultiExperiment.h"
#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Core/LoadFromFile.h"

#include "OscLib/func/IOscCalculator.h"

#include "TDirectory.h"
#include "TH1D.h"
#include "TObjString.h"
#include "TVectorD.h"

#include <cassert>

namespace ana
{

  //----------------------------------------------------------------------
  // Add covariance matrix corresponding to one or more experiments
  void MultiExperiment::AddCovarianceMatrix(const std::string covMatFileName,
                                            const std::string covMatName,
                                            const bool preInvert,
                                            const std::vector<int> &expt_idxes)
  {
        TDirectory *thisDir = gDirectory->CurrentDirectory();

        // std::cout << "Adding covariance matrix " << covMatName << std::endl;
        // std::cout << "From file: " << covMatFileName << std::endl;

        // Get the covariance matrix from a file
        TFile covMatFile( covMatFileName.c_str() );
        TMatrixD* covMx = (TMatrixD*) covMatFile.Get(covMatName.c_str());

        if( !covMx ) {
          std::cout << "Could not obtain covariance matrix named "
                    << covMatName << " from " << covMatFileName << std::endl;
          abort();
        }

        AddCovarianceMatrix(covMx, preInvert, expt_idxes);

        thisDir->cd();

  }
    void MultiExperiment::AddCovarianceMatrix(TMatrixD *covMx,
                                              const bool preInvert,
                                              const std::vector<int> &expt_idxes)
    {

    if( !covMx ) {
      std::cout << "Passed invalid covariance matrix." << std::endl;
      abort();
    }
    fCovMx.push_back( covMx );

    // Which experiments does this cov matrix corresponds to?
    fCovMxExpIdx.push_back( expt_idxes );

    // Ensure that associated Experiment exists; must call Add() before AddCovarianceMatrix()
    for( unsigned int i = 0; i < expt_idxes.size(); ++i ) {
      unsigned int idx = expt_idxes[i];
      if( fExpts.size() <= idx || !fExpts[idx] ) {
        std::cout << "Added covariance for Experiment with index " << idx
                  << " but no such Experiment exists" << std::endl;
        abort();
      } else {
        // std::cout << "  Adding experiment " << idx << " to covariance matrix" << std::endl;
        fUseCovMx[idx] = true;
      }
    }

    // Invert the fractional matrix in advance for performance
    if( preInvert ) {
      // std::cout << "Pre-inverting covariance matrix" << std::endl;
      // Figure out how many bins there are
      int n_bins = 0;
      TVectorD pred( covMx->GetNcols() );
      for( unsigned int i = 0; i < expt_idxes.size(); ++i ) {
        int idx = expt_idxes[i];
        TH1D * hist = fExpts[idx]->PredHist(0, kNoShift);
        for( int b = 0; b < hist->GetNbinsX(); ++b ) {
          pred[n_bins++] = hist->GetBinContent(b+1);
        }
        HistCache::Delete(hist);
      }

      // make a matrix to invert
      TMatrixD toInvert(*covMx);
      assert( toInvert.GetNcols() == n_bins );

      // We add the squared fractional statistical errors to the diagonal. In
      // principle this should vary with the predicted number of events, but in
      // the ND using the no-syst-shifts number should be a pretty good
      // approximation, and it's much faster than needing to re-invert the
      // matrix every time.
      for( int b = 0; b < n_bins; ++b ) {
        if( pred[b] > 0. ) toInvert(b, b) += 1./pred[b];
      }

      fCovMxInv.push_back( new TMatrixD(TMatrixD::kInverted, toInvert) );
    } else {
      fCovMxInv.push_back(0);
    }

    fPreInvert.push_back(preInvert);
  }


  //----------------------------------------------------------------------
  double MultiExperiment::ChiSq(osc::IOscCalculatorAdjustable* osc,
                                const SystShifts& syst) const
  {
    double ret = 0.;
    for( unsigned int nCov = 0; nCov < fCovMx.size(); ++nCov ) {
      // Build single array with all experiments associated with this covariance matrix
      TVectorD pred( fCovMx[nCov]->GetNcols() );
      TVectorD data( fCovMx[nCov]->GetNcols() );
      int n_bins = 0;
      for( unsigned int i = 0; i < fCovMxExpIdx[nCov].size(); ++i ) {
        int idx = fCovMxExpIdx[nCov][i];

        // Make a local copy we're going to rewrite into the terms this
        // sub-experiment will accept.
        SystShifts localShifts = syst;
        for(auto it: fSystCorrelations[idx]){
          // We're mapping prim -> sec
          const ISyst* prim = it.first;
          const ISyst* sec = it.second;
          if(syst.GetShift(prim) != 0){
            // sec can be unset, which means there's no representation needed
            // of prim in the sub-experiment.
            if(sec) localShifts.SetShift(sec, syst.GetShift(prim));
            // We've either translated or discarded prim, so drop it here.
            localShifts.SetShift(prim, 0);
          }
        }

        TH1D * histMC = fExpts[idx]->PredHist(osc, localShifts);
        TH1D * histData = fExpts[idx]->DataHist();
        // Mask bins with too low statistics
        fExpts[idx]->ApplyMask( histMC, histData );
        for( int b = 0; b < histMC->GetNbinsX(); ++b ) {
          pred[n_bins] = histMC->GetBinContent(b+1);
          data[n_bins++] = histData->GetBinContent(b+1);
        }
        HistCache::Delete(histMC);
        HistCache::Delete(histData);
      }

      TMatrixD covInv(fCovMx[nCov]->GetNrows(), fCovMx[nCov]->GetNcols());

      if( fPreInvert[nCov] ) {
        // Use pre-computed CovMxInv with stat error already added
        assert(fCovMxInv[nCov]);
        covInv = *(fCovMxInv[nCov]);
      } else {
        // Invert the matrix here, first add the statistical uncertainty
        TMatrixD cov = *(fCovMx[nCov]);
        for( int b = 0; b < n_bins; ++b ) {
          const double Nevt = pred[b];
          if(Nevt > 0.) cov(b, b) += 1./Nevt;
        }

        // And then invert
        covInv = TMatrixD(TMatrixD::kInverted, cov);
      }

      // In either case - covariance matrix is fractional; convert it to
      // absolute by multiplying out the prediction
      for( int b0 = 0; b0 < n_bins; ++b0 ) {
        for( int b1 = 0; b1 < n_bins; ++b1 ) {
          const double f = pred[b0] * pred[b1];
          if(f != 0) covInv(b0, b1) /= f;
          else covInv(b0, b1) = 0.;
        }
      }

      // Now it's absolute it's suitable for use in the chisq calculation
      double chi2 = Chi2CovMx( pred, data, covInv );
      ret += chi2;
    }

    // Add chi2 from experiments that don't use covariance matrix
    // penalty term, for example
    double ret2 = 0.;
    for(unsigned int n = 0; n < fExpts.size(); ++n){

      // check if we already counted this experiment with a covariance matrix
      //if( fUseCovMx[n] ) continue;

      // Don't waste time fiddling with the systematics if for sure there's
      // nothing to do.
      if(fSystCorrelations[n].empty()){
        double thischi2 = fExpts[n]->ChiSq(osc, syst);
        ret2 += thischi2;
        if( !fUseCovMx[n] ) ret += thischi2;
      }
      else{
        // Make a local copy we're going to rewrite into the terms this
        // sub-experiment will accept.
        SystShifts localShifts = syst;
        for(auto it: fSystCorrelations[n]){
          // We're mapping prim -> sec
          const ISyst* prim = it.first;
          const ISyst* sec = it.second;
          if(syst.GetShift(prim) != 0){
            // sec can be unset, which means there's no representation needed
            // of prim in the sub-experiment.
            if(sec) localShifts.SetShift(sec, syst.GetShift(prim));
            // We've either translated or discarded prim, so drop it here.
            localShifts.SetShift(prim, 0);
          }
        }
        double thischi2 = fExpts[n]->ChiSq(osc, localShifts);
        ret2 += thischi2;
        if( !fUseCovMx[n] ) ret += thischi2;
      }
    }

    return ret;
  }

  //----------------------------------------------------------------------
  void MultiExperiment::
  Derivative(osc::IOscCalculator* calc,
             const SystShifts& shift,
             std::unordered_map<const ISyst*, double>& dchi) const
  {
    if(!fCovMx.empty()){
      // std::cout << "MultiExperiment::Derivative() not implemented for case with covariance matrices" << std::endl;
      dchi.clear();
      return;
    }

    // Each one should sum into the total so far
    for(unsigned int n = 0; n < fExpts.size(); ++n){
      if(fSystCorrelations[n].empty()){
        // If there are no adjustments needed it's easy
        fExpts[n]->Derivative(calc, shift, dchi);
        if(dchi.empty()) return;
        continue;
      }

      auto dchiLocal = dchi;
      for(auto& it: dchiLocal) it.second = 0;
      SystShifts localShifts = shift;
      std::unordered_map<const ISyst*, const ISyst*> reverseMap;
      for(auto it: fSystCorrelations[n]){
        // We're mapping prim -> sec
        const ISyst* prim = it.first;
        const ISyst* sec = it.second;
        if(dchi.count(prim)){
          dchiLocal.erase(dchiLocal.find(prim));
          if(sec){
            dchiLocal.emplace(sec, 0);
            reverseMap[sec] = prim;
          }
        }
        if(sec) localShifts.SetShift(sec, shift.GetShift(prim));
        // We've either translated or discarded prim, so drop it here.
        localShifts.SetShift(prim, 0);
      }

      fExpts[n]->Derivative(calc, localShifts, dchiLocal);

      // One of the components doesn't support derivatives, don't bother asking
      // the rest.
      if(dchiLocal.empty()){
        dchi.clear();
        return;
      }

      // And translate back
      for(auto it: dchiLocal){
        if(reverseMap.count(it.first) > 0){
          dchi[reverseMap[it.first]] += it.second;
        }
        else{
          dchi[it.first] += it.second;
        }
      }
    }
  }

  //----------------------------------------------------------------------
  void MultiExperiment::
  SetSystCorrelations(int idx,
                      const std::vector<std::pair<const ISyst*, const ISyst*>>& corrs)
  {
    // Sanity-check the mapping
    std::map<const ISyst*, const ISyst*> already;
    for(auto it: corrs){
      assert(it.first != it.second);

      // Don't worry if second element is null pointer
      if (!it.second) continue;
      if(already.find(it.second) == already.end()){
        already[it.second] = it.first;
      }
      else{
        std::cout << "MultiExperiment::SetSystCorrelations(): Warning!\n"
                  << "In experiment " << idx << " both "
                  << already[it.second]->ShortName() << " and "
                  << it.first->ShortName()
                  << " are configured to map to " << it.second->ShortName()
                  << ". That's probably not what you want." << std::endl;
      }
    }

    // Apply it
    fSystCorrelations[idx] = corrs;
  }

  //----------------------------------------------------------------------
  void MultiExperiment::SaveTo(TDirectory* dir) const
  {
    bool hasCorr = false;
    for(auto it: fSystCorrelations) if(!it.empty()) hasCorr = true;

    if(hasCorr){
      std::cerr << "Warning in MultiExperiment: systematic correlations are set and will not be serialized by this call to SaveTo(). You will have to re-set them once you load the experiment back in." << std::endl;
    }

    TDirectory* tmp = dir;

    dir->cd();
    TObjString("MultiExperiment").Write("type");

    for(unsigned int i = 0; i < fExpts.size(); ++i){
      fExpts[i]->SaveTo(dir->mkdir(TString::Format("expt%d", i)));
    }

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<MultiExperiment> MultiExperiment::LoadFrom(TDirectory* dir)
  {
    TObjString* ptag = (TObjString*)dir->Get("type");
    assert(ptag);
    assert(ptag->GetString() == "MultiExperiment");

    std::vector<const IChiSqExperiment*> expts;

    for(int i = 0; ; ++i){
      TDirectory* subdir = dir->GetDirectory(TString::Format("expt%d", i));
      if(!subdir) break;

      expts.push_back(ana::LoadFrom<IChiSqExperiment>(subdir).release());
    }

    assert(!expts.empty());

    return std::unique_ptr<MultiExperiment>(new MultiExperiment(expts));
  }
}
