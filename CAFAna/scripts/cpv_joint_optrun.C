// TO DO:
// * Can I generalize SetMaskHist to IChiExperiment? It's so far defined in SingleSampleExperiment
//   experiments[expId]->SetMaskHist(0.5, (AnaV == kV4) ? 10 : 8); /// dont trust SetMaskHist. Check.
// * Write function to load predictions from fixed location
#pragma once

#include "CAFAna/Analysis/AnalysisVersion.h"
#include "CAFAna/Analysis/common_fit_definitions.h"
#include "CAFAna/Analysis/Calcs.h"
#include "CAFAna/Analysis/Exposures.h"
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


void cpv_joint_optrun(std::string input_predictions="/pnfs/dune/persistent/users/dmendez/CAFAnaInputs/",
	std::string outputFname="cpv_opt_test3_mask.root",
	std::string systSet="nosyst",
  TString detectors="fdnd", TString horns="fhcrhc", TString neutrinos="nuenumu",
	TString penalty="nopen", std::string asimov_set="0", int hie=1,
	double years_fhc = 3.5, double years_rhc = 3.5, int nom_exp = 7){

  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  // get the analysis version
  auto AnaV = GetAnaVersion();

  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(systSet);

  // Oscillation parameters to use
  std::vector<const IFitVar*> oscVars = GetOscVars("th23:th13:dmsq32", hie);

  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();

  double binwidth = 2*TMath::Pi()/36;
  TGraph* gCPV = new TGraph();


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


  bool UseNDCovMat = true;
  if (getenv("CAFANA_USE_NDCOVMAT")) {
    UseNDCovMat = bool(atoi(getenv("CAFANA_USE_NDCOVMAT")));
  }
  bool UseV3NDCovMat = (AnaV == kV3);
  if (getenv("CAFANA_USE_UNCORRNDCOVMAT")) {
    UseV3NDCovMat = bool(atoi(getenv("CAFANA_USE_UNCORRNDCOVMAT")));
  }
  // Get ND covariance matrix
  TMatrixD *this_ndmatrix = new TMatrixD();
  bool use_nd = detectors.Contains("nd") && (years_rhc>0. && years_fhc>0.);
  if(use_nd && UseNDCovMat){
      bool TwoBeams = (nhorn == 2);
      bool isFHC = (horns.Contains("fhc"));
      this_ndmatrix = GetNDCovMat(UseV3NDCovMat, TwoBeams, isFHC);
  }

  ///// Predictions my way
  ///// The joint scripts load the preds at everypoint. Avoid that. Load once.
  std::vector<std::string> tags; // to keep track of spectra types. I could add a Title to Spectrum or define a structure but I rather no go there atm.
  std::vector<PredictionInterp*> predictions;
  std::string fdir = "/pnfs/dune/persistent/users/dmendez/CAFAnaInputs/";
  int this_idx=0;

  for(int detId=0; detId<ndetector; detId++){
    for(int hornId=0; hornId<nhorn; hornId++){
      for(int nuId=0; nuId<nneutrino; nuId++){

        if(detector[detId]=="nd" && neutrino[nuId]=="nue") continue; // no nd nue prediction

        std::string fname = fdir+Form("state_%s_%s.root", detector[detId].c_str(), horn[hornId].c_str());
        std::string din = Form("%s_interp_%s_%s", detector[detId].c_str(), neutrino[nuId].c_str(), horn[hornId].c_str());
        tags.push_back(din);
        TFile *fin = TFile::Open(fname.c_str(), "READ");
        predictions.push_back(LoadFrom<PredictionInterp>(fin, din).release());
        fin->Close();
        this_idx++;
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



  for(double step = 0; step < 37; ++step) {
    
    double chisqmin = 99999;
    double thischisq;

    // fake data will be generated assuming these parameter values
    // osc::IOscCalculatorAdjustable* calc = DefaultOscCalc();
    double thisdcp = -TMath::Pi() + step*binwidth; // why start in -pi instead of zero... (t2k vs nova)
    osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie, 1, asimov_set); 
    trueOsc->SetdCP(thisdcp);

    // defining the predictions and fake data here might seem repetitive
    // but at least it's not reopening the prediction files at each steep
    std::cout << "\nfilling prediction" << std::endl;
    std::vector<Spectrum> s_predictions;
    std::vector<Spectrum> s_fakedata;

    // define the experiments for the multiexperiment
    MultiExperiment experiments;
    for(unsigned int predId=0; predId<predictions.size(); predId++){
      s_predictions.push_back(predictions[predId]->Predict(trueOsc));
      s_fakedata.push_back(s_predictions[predId].MockData(pot[predId],0)); // second argument = 0 defaults to random throw number
      // Mask or you'll get different results to the official
      SingleSampleExperiment *temp_singleexpt = new SingleSampleExperiment(predictions[predId], s_fakedata[predId]);
      if(mask) temp_singleexpt->SetMaskHist(0.5, (AnaV == kV4) ? 10 : 8);
      experiments.Add(temp_singleexpt);
      // experiments.Add(new SingleSampleExperiment(predictions[predId], s_fakedata[predId]));
    }
    if(penalty.Contains("reactor")) experiments.Add(ReactorConstraintPDG2019());
    if(penalty.Contains("solar")) experiments.Add(&kSolarConstraintsPDG2019);

    // Add the ND covariance matrix
    // To properly add the nd matrix, the idx should match the nd experiments
    // TO DO: what happens in the FHC or RHC only case? Check and edit the GetNDCovMat function
    if(use_nd && UseNDCovMat){
      std::cout << "Adding covariance mat" << std::endl;
      experiments.AddCovarianceMatrix(this_ndmatrix, true, {0, 1});
    }

    // Still need to loop over dcp choices for the seeded calculator
    // why only have two options?
    // double idcplist = {0, 1/2, 1, 3/2}
    // for (double idcp : idcplist) {
    for (int idcp = 0; idcp < 2; ++idcp) {
      double dcptest = idcp*TMath::Pi();
      	
      for (int ioct = -1; ioct <= 1; ioct +=2) {
        osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie, ioct, asimov_set);
      	testOsc->SetdCP(dcptest);
	
      	// Make a map of seed points to try (replaces the old loops)
      	std::map<const IFitVar*, std::vector<double>> oscSeeds = {};
      	      	
        SystShifts trueSyst = kNoShift;
      	SystShifts testSyst = kNoShift;
      	
        std::cerr << "[INFO]: Beginning fit. " << BuildLogInfoString();
        auto start_fit = std::chrono::system_clock::now();
        MinuitFitter this_fit(&experiments, oscVars, systlist, MinuitFitter::kNormal);
        double thischisq = this_fit.Fit(testOsc, testSyst, oscSeeds, {}, MinuitFitter::kVerbose)->EvalMetricVal();
      	
      	chisqmin = TMath::Min(thischisq,chisqmin);
        std::cout << "chisqmin=" << chisqmin << std::endl;
        auto end_fit = std::chrono::system_clock::now();
        std::time_t end_fit_time = std::chrono::system_clock::to_time_t(end_fit);
        std::cerr << "[FIT]: Finished fit in "
        << std::chrono::duration_cast<std::chrono::seconds>(end_fit - start_fit).count()
        << " s after " << this_fit.GetNFCN() << " iterations "
        << BuildLogInfoString();

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

  predictions.clear();

}
