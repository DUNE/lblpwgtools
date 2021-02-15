#pragma once

#include "includes_validation_lbl.h"

using namespace ana;

struct expSpectrum
{
  Spectrum spectrum;
  std::string name;  
  double pot;
};

osc::IOscCalcAdjustable* GetTestCalculator()
{
  osc::IOscCalcAdjustable* ret = new osc::OscCalcPMNSOpt;
  ret->SetL(1284.9);
  ret->SetRho(2.848);
  ret->SetDmsq21(7.53e-5);
  ret->SetTh12(asin(sqrt(0.307)));
  ret->SetDmsq32(2.444e-3);
  ret->SetTh23(asin(sqrt(0.512)));
  ret->SetTh13(asin(sqrt(2.18e-2)));
  ret->SetdCP(215 * TMath::Pi()/180);

  return ret;
}

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

void make_lblbasefit(
  TString detectors="fdnd",
  TString horns="rhcfhc",
  TString neutrinos="nuenumu",
  bool systs = false)
{

  // Systematics to use: 1 or 2 for quick test
  std::string systtag = "stats";
  std::vector<const ISyst*> systlist = {};
  if(systs){
    systtag  = "systs";
    systlist = {&kEnergyScaleFD,&kFDRecoNumuSyst};
  }

  std::vector<std::string> neutrino = {};
  std::vector<std::string> detector = {};
  std::vector<std::string> horn     = {};
  std::vector<float> pot = {};

  if(neutrinos.Contains("nue")) neutrino.push_back("nue");
  if(neutrinos.Contains("numu")) neutrino.push_back("numu");

  if(detectors.Contains("nd")) detector.push_back("nd");
  if(detectors.Contains("fd")) detector.push_back("fd");

  if(horns.Contains("fhc")) horn.push_back("fhc");
  if(horns.Contains("rhc")) horn.push_back("rhc");

  const int nhorn = horn.size();
  const int ndetector = detector.size();
  const int nneutrino = neutrino.size();

  bool TwoBeams = (nhorn == 2);
  bool IsFHC = (horns.Contains("fhc"));


  // Get ND covariance matrix or not
  bool UseNDCovMat = true;
  bool useND = detectors.Contains("nd");

  ///// Load Predictions once
  std::vector<TString> tags; // to keep track of spectra types. I could add a Title to Spectrum or define a structure but I rather no go there atm.
  std::vector<PredictionInterp*> predictions;
  std::string fdir = "/pnfs/dune/persistent/users/dmendez/lbl_fit_validation/";

  for(int detId=0; detId<ndetector; detId++){
    for(int hornId=0; hornId<nhorn; hornId++){
      for(int nuId=0; nuId<nneutrino; nuId++){

        if(detector[detId]=="nd" && neutrino[nuId]=="nue") continue; // no nd nue prediction

        std::string fname = fdir+Form("state_%s_%s.root", detector[detId].c_str(), horn[hornId].c_str());
        std::string din = Form("%s_interp_%s_%s", detector[detId].c_str(), neutrino[nuId].c_str(), horn[hornId].c_str());
        tags.push_back(detector[detId]+neutrino[nuId]+horn[hornId]);
        TFile *fin = TFile::Open(fname.c_str(), "READ");
        predictions.push_back(LoadFrom<PredictionInterp>(fin, din).release());
        fin->Close();
        // Get nominal exposure per year from Analysis/Exposures.h
        if(horn[hornId]=="fhc"){
          if(detector[detId]=="nd") pot.push_back(kNDPOT[15] * 7.5);
          else pot.push_back(kFDPOT[15] * 7.5);
        } // fhc pot
        else{
          if(detector[detId]=="nd") pot.push_back(kNDPOT[15] * 7.5);
          else pot.push_back(kFDPOT[15] * 7.5);
        } // rhc pot

      } // nuId
    } // hornId
  } // detId

  // We could add many validation layers but let's not add one for the calculators
  // Fix the values here instead of setting them with NuFitOscCalc in case it changes.
  osc::IOscCalcAdjustable* trueOsc = GetTestCalculator();
  osc::IOscCalcAdjustable* testOsc = GetTestCalculator(); 

  // defining the predictions and fake data here might seem repetitive
  // but at least it's not reopening the prediction files at each steep
  std::cout << "\nfilling prediction" << std::endl;
  std::vector<Spectrum> s_predictions;
  std::vector<Spectrum> s_fakedata;
  // fill the fake data in a separate loop so the covexpt doesn't try to find sth that doesn't exist 
  for(unsigned int predId=0; predId<predictions.size(); predId++){
    std::cout << "POT " << pot[predId] << std::endl;
    s_predictions.push_back(predictions[predId]->Predict(trueOsc));
    s_fakedata.push_back(s_predictions[predId].FakeData(pot[predId])); // second argument = 0 defaults to random throw number
  }

  // Determine what experiment this is
  std::cout << "getting MultiExperiment" << std::endl;
  MultiExperiment experiments = GetMultiExperimentLBL(predictions, s_fakedata, tags, UseNDCovMat, TwoBeams);

  std::map<const IFitVar*, std::vector<double>> oscSeeds = {};
                
  SystShifts trueSyst = kNoShift;
  SystShifts testSyst = kNoShift;
  
  std::cerr << "[INFO]: Beginning fit. " << BuildLogInfoString();
    
  TH1* hprofile_dcp  = SqrtProfile(&experiments, testOsc, &kFitDeltaInPiUnits, 4, 0.0, 2.0, -1, {}, systlist);
  TH1* hprofile_ss23 = SqrtProfile(&experiments, testOsc, &kFitSinSqTheta23,   4, 0.4, 0.6, -1, {}, systlist);

  // open the file that contains the profiles to compare with
  TString fname_comp = "test_fhcrhc_ndfd_"+systtag+"_15years.root";
  TFile* fcomp = new TFile(fname_comp, "RECREATE");

  fcomp->cd();
  hprofile_dcp->Write("sqrtprof_dcp");
  fcomp->cd();
  hprofile_ss23->Write("sqrtprof_ss23");

  fcomp->Close();

  // s_predictions.clear();
  // s_fakedata.clear();
  // predictions.clear();
  //// experiments.clear();

}