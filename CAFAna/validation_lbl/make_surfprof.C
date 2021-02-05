// TO DO:
// * Can I generalize SetMaskHist to IChiExperiment? It's so far defined in SingleSampleExperiment
//   experiments[expId]->SetMaskHist(0.5, (AnaV == kV4) ? 10 : 8); /// dont trust SetMaskHist. Check.
// * Write function to load predictions from fixed location
#pragma once

#include "helper_validation_lbl.h"

using namespace ana;

void make_surfprof(std::string specialTag="test",
  std::string systSet="allsyst", std::string asimov_set="0",
  TString penalty="", TString detectors="fdnd",
  TString horns="rhcfhc", TString neutrinos="nuenumu",
  double years_fhc = 3.5, double years_rhc = 3.5,
  int nom_exp = 7, int hie=1){

  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  // get the analysis version
  auto AnaV = GetAnaVersion();

  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(systSet);

  // Oscillation parameters to use
  std::vector<const IFitVar*> oscVars = GetOscVars("th23:th13:dmsq32", hie);

  std::vector<std::string> neutrino = {};
  std::vector<std::string> detector = {};
  std::vector<std::string> horn;
  std::vector<float> pot = {};

  // Contains or use python inspired SplitString currently in AnaSysts.h
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
  if (getenv("CAFANA_USE_NDCOVMAT")) {
    UseNDCovMat = bool(atoi(getenv("CAFANA_USE_NDCOVMAT")));
  }
  bool useND = detectors.Contains("nd");
  //UseNDCovMat = false;
  ///// Predictions my way
  ///// The joint scripts load the preds at everypoint. Avoid that. Load once.
  std::vector<TString> tags; // to keep track of spectra types. I could add a Title to Spectrum or define a structure but I rather no go there atm.
  std::vector<PredictionInterp*> predictions;
  std::string fdir = "/pnfs/dune/persistent/users/dmendez/CAFAnaInputs/";

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
          if(detector[detId]=="nd") pot.push_back(kNDPOT[nom_exp] * years_fhc);
          else pot.push_back(kFDPOT[nom_exp] * years_fhc);
        } // fhc pot
        else{
          if(detector[detId]=="nd") pot.push_back(kNDPOT[nom_exp] * years_rhc);
          else pot.push_back(kFDPOT[nom_exp] * years_rhc);
        } // rhc pot

      } // nuId
    } // hornId
  } // detId

  // The output file
  TString outputFname = "surfprof_"+detectors+"_"+horns+"_"+neutrinos+"__"+penalty+"_"+systSet+(hie == 1 ? "__nh" : "__ih") + specialTag + ".root";
  TFile* fout = new TFile(outputFname, "RECREATE");
  fout->cd();

  osc::IOscCalcAdjustable* trueOsc = NuFitOscCalc(hie, 1, asimov_set); 
  osc::IOscCalcAdjustable* testOsc = NuFitOscCalc(hie, 1, asimov_set); 

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

  // TO DO: Make this desition cleaner and shorter
  // Determine what experiment this is. Thank you Covariance Matrix.
  std::cout << "getting MultiExperiment" << std::endl;
  MultiExperiment experiments = GetMultiExperimentLBL(predictions, s_fakedata, tags, UseNDCovMat, TwoBeams);
  if(penalty.Contains("reactor")) experiments.Add(ReactorConstraintPDG2019());
  if(penalty.Contains("solar")) experiments.Add(&kSolarConstraintsPDG2019);

  std::map<const IFitVar*, std::vector<double>> oscSeeds = {};
      	      	
  SystShifts trueSyst = kNoShift;
  SystShifts testSyst = kNoShift;
	
  std::cerr << "[INFO]: Beginning fit. " << BuildLogInfoString();
 //  auto start_fit = std::chrono::system_clock::now();
 //  MinuitFitter this_fit(&experiments, oscVars, systlist, MinuitFitter::kNormal);
 //  double thischisq = this_fit.Fit(testOsc, testSyst, oscSeeds, {}, MinuitFitter::kVerbose)->EvalMetricVal();

 // chisqmin = TMath::Min(thischisq,chisqmin);
 //  std::cout << "chisqmin=" << chisqmin << std::endl;
 //  auto end_fit = std::chrono::system_clock::now();
 //  std::time_t end_fit_time = std::chrono::system_clock::to_time_t(end_fit);
 //  std::cerr << "[FIT]: Finished fit in "
 //  << std::chrono::duration_cast<std::chrono::seconds>(end_fit - start_fit).count()
 //  << " s after " << this_fit.GetNFCN() << " iterations "
 //  << BuildLogInfoString();
    

  FrequentistSurface* surface = new FrequentistSurface(&experiments, testOsc, &kFitSinSqTheta23, 40, 0.3, 0.7, &kFitDmSq32Scaled, 40, 2.0, 3.0, {}, systlist);
  TH1* hprofile_delta32 = SqrtProfile(&experiments, testOsc, &kFitDmSq32Scaled, 40, 2.0, 3.0, -1, {}, systlist);
  TH1* hprofile_sin23   = SqrtProfile(&experiments, testOsc, &kFitSinSqTheta23, 40, 0.3, 0.7, -1, {}, systlist);

  surface->SaveTo(fout->mkdir("surface"), "mysurf");
  fout->cd();
  hprofile_sin23->Write("profile_sin23");
  fout->cd();
  hprofile_delta32->Write("profile_dm32");

  // gDirectory->WriteObject(hprofile_sin23,"profile_sin23");
  // gDirectory->WriteObject(hprofile_delta32,"profile_dm32");
  fout->Close();

  // s_predictions.clear();
  // s_fakedata.clear();
  // predictions.clear();
  //// experiments.clear();

}
