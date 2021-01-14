// TO DO:
// * Can I generalize SetMaskHist to IChiExperiment? It's so far defined in SingleSampleExperiment
//   experiments[expId]->SetMaskHist(0.5, (AnaV == kV4) ? 10 : 8); /// dont trust SetMaskHist. Check.
// * Write function to load predictions from fixed location
#pragma once

#include "CAFAna/Analysis/AnalysisVersion.h"
#include "CAFAna/Analysis/common_fit_definitions.h"
#include "CAFAna/Analysis/Calcs.h"
#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Experiment/CovarianceExperiment.h"
#include "CAFAna/Experiment/ReactorExperiment.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Experiment/SolarConstraints.h"

#include "CAFAna/Systs/AnaSysts.h"

using namespace ana;

bool mask = true;

struct expSpectrum
{
  Spectrum spectrum;
  std::string name;  
  double pot;
};

// This is a pain. There are 27 possible combinations of experiments.
// But no one asked for numu and nue only cases, or fhc and rhc only, or matrix or no matrix. 
MultiExperiment GetMultiExperimentLBL(std::vector<PredictionInterp*> predictions,
                                      std::vector<Spectrum> s_fakedata, 
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
    SingleSampleExperiment *temp_singleexpt = new SingleSampleExperiment(predictions[predId], s_fakedata[predId]);
    std::cout << thistag << std::endl;
    if(thistag.Contains("fd")){ // we will always at least have the fd
      experiments.Add(temp_singleexpt);
    }
    if(thistag.Contains("nd") && !UseNDCovMat){ // also simple experiments if no covariance matrix
      experiments.Add(temp_singleexpt);
    }
    if(thistag.Contains("nd") && UseNDCovMat){
      if(!TwoBeams){
        if(thistag.Contains("fhc")){
          CovarianceExperiment* temp_covexpt = new CovarianceExperiment({predictions[predId]}, {s_fakedata[predId]},
                                                                        this_fhcmatrix, kCovMxChiSqPreInvert);
          temp_covexpt->SetMaskHist(0, 0.5, 10, 0, -1); // also assume v4 for energy masking
          experiments.Add(temp_covexpt);
        }
        else{
          CovarianceExperiment* temp_covexpt = new CovarianceExperiment({predictions[predId]}, {s_fakedata[predId]},
                                                                        this_rhcmatrix, kCovMxChiSqPreInvert);
          temp_covexpt->SetMaskHist(0, 0.5, 10, 0, -1);
          experiments.Add(temp_covexpt);            
        }
      }
      else{
        CovarianceExperiment* temp_covexpt = new CovarianceExperiment({predictions[fhcnd_id], predictions[rhcnd_id]},
                                                                      {s_fakedata[fhcnd_id], s_fakedata[rhcnd_id]},
                                                                      this_jointmatrix, kCovMxChiSqPreInvert);
        temp_covexpt->SetMaskHist(0, 0.5, 10, 0, -1);
        temp_covexpt->SetMaskHist(1, 0.5, 10, 0, -1);
        if(thistag.Contains("fhc")){
          experiments.Add(temp_covexpt);
        }
        if(thistag.Contains("rhc")){
          continue; // avoid adding covariance experiment twice.
        }
      }
    }
  }

  return experiments;

}// end GetMultiExperimentLBL


// std::string fdir = "root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/dune/persistent/users/dmendez/CAFAnaInputs/";
// std::string fdir = "/pnfs/dune/persistent/users/dmendez/CAFAnaInputs/";

void get_sensitivity(std::string inPredDir = "/pnfs/dune/persistent/users/dmendez/CAFAnaInputs/",
		     std::string specialTag="test",
		     std::string systSet="nosyst",
		     double years_fhc = 3.5, double years_rhc = 3.5,
		     int nom_exp = 7, int hie=1,
		     TString penalty="nopen", std::string asimov_set="0",
		     TString detectors="fdnd", TString horns="fhcrhc", TString neutrinos="nuenumu"){

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
  bool use_nd = detectors.Contains("nd");

  ///// Predictions my way
  ///// The joint scripts load the preds at everypoint. Avoid that. Load once.
  std::vector<TString> tags; // I could maybe define a map to keep track of spectra types.
  std::vector<PredictionInterp*> predictions;
  std::string fdir = inPredDir;

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
  TString outputFname = "cpv_"+detectors+"_"+horns+"_"+neutrinos+"__"+penalty+"_"+systSet+(hie == 1 ? "__nh" : "__ih") + specialTag + ".root";
  TFile* fout = new TFile(outputFname, "RECREATE");
  fout->cd();
  double binwidth = 2*TMath::Pi()/36;
  TGraph* gCPV = new TGraph();

  for(double step = 0; step < 37; ++step) {
    
    double chisqmin = 99999;
    double thischisq;

    // fake data will be generated assuming these parameter values
    // osc::IOscCalcAdjustable* calc = DefaultOscCalc();
    double thisdcp = -TMath::Pi() + step*binwidth; // why start in -pi instead of zero... (t2k vs nova)
    osc::IOscCalcAdjustable* trueOsc = NuFitOscCalc(hie, 1, asimov_set); 
    trueOsc->SetdCP(thisdcp);

    // defining the predictions and fake data here might seem repetitive
    // but at least it's not reopening the prediction files at each steep
    std::cout << "\nfilling prediction" << std::endl;
    std::vector<Spectrum> s_predictions;
    std::vector<Spectrum> s_fakedata;
    // fill the fake data in a separate loop so the covexpt doesn't try to find sth that doesn't exist 
    for(unsigned int predId=0; predId<predictions.size(); predId++){
      s_predictions.push_back(predictions[predId]->Predict(trueOsc));
      s_fakedata.push_back(s_predictions[predId].MockData(pot[predId],0)); // second argument = 0 defaults to random throw number
    }

    // TO DO: Make this desition cleaner and shorter
    // Determine what experiment this is. Thank you Covariance Matrix.
    std::cout << "getting MultiExperiment" << std::endl;
    MultiExperiment experiments = GetMultiExperimentLBL(predictions, s_fakedata, tags, UseNDCovMat, TwoBeams);
    if(penalty.Contains("reactor")) experiments.Add(ReactorConstraintPDG2019());
    if(penalty.Contains("solar")) experiments.Add(&kSolarConstraintsPDG2019);

    // Still need to loop over dcp choices for the seeded Calc
    // why only have two options?
    // double idcplist = {0, 1/2, 1, 3/2}
    // for (double idcp : idcplist) {
    for (int idcp = 0; idcp < 2; ++idcp) {
      double dcptest = idcp*TMath::Pi();
      	
      for (int ioct = -1; ioct <= 1; ioct +=2) {
        osc::IOscCalcAdjustable* testOsc = NuFitOscCalc(hie, ioct, asimov_set);
      	testOsc->SetdCP(dcptest);
	
      	// Make a map of seed points to try (replaces the old loops)
      	std::map<const IFitVar*, std::vector<double>> oscSeeds = {};

        SystShifts trueSyst = kNoShift;
        SystShifts testSyst = kNoShift;
      	
        std::cerr << "[INFO]: Beginning fit. " << BuildLogInfoString();
        auto start_fit = std::chrono::system_clock::now();
        MinuitFitter this_fit(&experiments, oscVars, systlist, MinuitFitter::kNormal);
        double thischisq = this_fit.Fit(testOsc, testSyst, oscSeeds, {}, MinuitFitter::kVerbose)->EvalMetricVal();
        auto end_fit = std::chrono::system_clock::now();
        std::time_t end_fit_time = std::chrono::system_clock::to_time_t(end_fit);
        std::cerr << "[FIT]: Finished fit in "
        << std::chrono::duration_cast<std::chrono::seconds>(end_fit - start_fit).count()
        << " s after " << this_fit.GetNFCN() << " iterations "
        << BuildLogInfoString();

        chisqmin = TMath::Min(thischisq,chisqmin);
        std::cout << "chisqmin=" << chisqmin << std::endl;

      }
    }
    
    chisqmin = TMath::Max(chisqmin,1e-6);
    gCPV->SetPoint(gCPV->GetN(),thisdcp/TMath::Pi(),TMath::Sqrt(chisqmin));

    s_predictions.clear();
    s_fakedata.clear();

  } // step loop

  fout->cd();
  gCPV->Draw("ALP");
  gCPV->Write(hie > 0 ? "sens_cpv_nh" : "sens_cpv_ih");
  fout->Close();
}
