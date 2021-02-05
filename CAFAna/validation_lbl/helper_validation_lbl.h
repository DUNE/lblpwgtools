#pragma once

#include "includes_validation_lbl.h"

using namespace ana;

struct expSpectrum
{
  Spectrum spectrum;
  std::string name;  
  double pot;
};

// This is a pain. There are 27 possible combinations of experiments.
// But no one asked for numu and nue only cases, or fhc and rhc only, or matrix or no matrix. 
MultiExperiment GetMultiExperimentLBL(std::vector<PredictionInterp*> predictions,
	std::vector<Spectrum> fakedata,
	std::vector<TString> tags,
	bool UseNDCovMat, bool TwoBeams){

  // First, make sure to get the possition of the fhc nd and rhc nd predictions
  int fhcnd_id = 0;
  int rhcnd_id = 0;
  for(unsigned int tagId=0; tagId<tags.size(); tagId++){
    if(tags[tagId].Contains("nd") && tags[tagId].Contains("fhc"))
      fhcnd_id = int(tagId);
    if(tags[tagId].Contains("nd") && tags[tagId].Contains("rhc"))
      rhcnd_id = int(tagId);
  }
  TMatrixD *this_jointmatrix = new TMatrixD();
  TMatrixD *this_fhcmatrix   = new TMatrixD();
  TMatrixD *this_rhcmatrix   = new TMatrixD();
  if(UseNDCovMat){ // assume v4 is the right matrix
    this_jointmatrix = GetNDCovMat(false, true, true);
    this_fhcmatrix   = GetNDCovMat(false, false, true);
    this_rhcmatrix   = GetNDCovMat(false, false, false);
  }

  MultiExperiment experiments;

  for(unsigned int predId=0; predId<predictions.size(); predId++){
  	TString thistag = tags[predId];
  	SingleSampleExperiment *temp_singleexpt = new SingleSampleExperiment(predictions[predId], fakedata[predId]);
  	if(thistag.Contains("fd")){ // we will always at least have the fd
  		temp_singleexpt->SetMaskHist(0.5, 10, 0, -1);
  		experiments.Add(temp_singleexpt);
  	}

  	if(thistag.Contains("nd") && !UseNDCovMat){ // also simple experiments if no covariance matrix
  		experiments.Add(temp_singleexpt);
  	} // nd but dont use cov matrix

    if(thistag.Contains("nd") && UseNDCovMat){
      if(!TwoBeams){
        if(thistag.Contains("fhc")){
          CovarianceExperiment* temp_covexpt = new CovarianceExperiment(predictions[predId], fakedata[predId],
                                                                        this_fhcmatrix, kCovMxChiSqPreInvert);
          temp_covexpt->SetMaskHist(0, 0.5, 10, 0, -1); // also assume v4 for energy masking
          experiments.Add(temp_covexpt);
        }
        else{
          CovarianceExperiment* temp_covexpt = new CovarianceExperiment(predictions[predId], fakedata[predId],
                                                                        this_rhcmatrix, kCovMxChiSqPreInvert);
          temp_covexpt->SetMaskHist(0, 0.5, 10, 0, -1);
          experiments.Add(temp_covexpt);            
        }
      }
      else{
    	CovarianceExperiment* temp_covexpt = new CovarianceExperiment({predictions[fhcnd_id], predictions[rhcnd_id]},
                                                                      {fakedata[fhcnd_id], fakedata[rhcnd_id]},
                                                                      this_jointmatrix, kCovMxChiSqPreInvert);
        temp_covexpt->SetMaskHist(0, 0.5, 10, 0, -1);
        temp_covexpt->SetMaskHist(1, 0.5, 10, 0, -1);
        if(thistag.Contains("rhc")){
          experiments.Add(temp_covexpt);
        }
        if(thistag.Contains("fhc")){
          continue; // avoid adding covariance experiment twice.
        }
      }
    } // nd and use matrix

  } // loop through predictions

  return experiments;

}// end GetMultiExperimentLBL