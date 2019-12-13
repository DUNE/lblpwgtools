// Standard script for DUNE MH sensitivity
// Now uses state file created by spec.C


#include "CAFAna/Analysis/Fit.h"

#include "CAFAna/Analysis/CalcsNuFit.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Experiment/MultiExperiment.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
//#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Prediction/PredictionInterp.h"
#include "StandardRecord/StandardRecord.h"
#include "CAFAna/Systs/Systs.h"
#include "CAFAna/Systs/DUNEFluxSysts.h"
#include "CAFAna/Systs/XSecSysts.h"
#include "CAFAna/Systs/EnergySysts.h"
#include "CAFAna/Systs/NDRecoSysts.h"
#include "CAFAna/Systs/FDRecoSysts.h"
#include "CAFAna/Systs/NuOnESysts.h"
#include "CAFAna/Systs/MissingProtonFakeData.h"

#include "CAFAna/Vars/FitVars.h"
#include "CAFAna/Analysis/Exposures.h"

using namespace ana;

#include "Utilities/rootlogon.C"

#include "OscLib/func/IOscCalculator.h"


#include "TCanvas.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH2.h"

#include "CAFAna/Analysis/common_fit_definitions.h"

//Set up some options for testing
bool globesscalefactor = false; // scale POT down by 0.95
bool prescan = false; //option to prescan for MH fits
bool th13penalty = true;

// POT/yr * 3.5yrs * mass correction
double potFD = 3.5 * POT120 * 40/1.13;

const char* stateFname = "spec_state_v3_wt.root";
//const char* stateFname = "/nashome/c/callumw/OA_studies/analysis_scripts/common_state_mcc11v3.root";
const char* outputFname = "mh_sens.root";

//Set systematics style by hand for now
bool nosyst = false;
bool normsyst = true;
bool fullsyst = false;

std::vector<const ISyst*> systlist;
std::vector<const ISyst*> normlist_sig = {&kNueFHCSyst, &kNumuFHCSyst, &kNueRHCSyst, &kNumuRHCSyst};
std::vector<const ISyst*> normlist_bg = {&kNueBeamFHCSyst, &kNueBeamRHCSyst, &kNCAppSyst, &kNCDisSyst, &kNutauSyst};

void mh()
{
  rootlogon(); // style

  if (globesscalefactor) potFD = potFD * 0.95;
  
  if(TFile(stateFname).IsZombie()){
    std::cout << "No state available, please run spec.C true" << std::endl;
    }
  else{    
    if (normsyst) {
      systlist.insert(systlist.end(), normlist_sig.begin(), normlist_sig.end()); 
      systlist.insert(systlist.end(), normlist_bg.begin(), normlist_bg.end()); 
    }

    // List all of the systematics we'll be using
    std::cout << "Systematics in this fit: " << std::endl;
    for(const ISyst* s: systlist) std::cout << s->ShortName() << "\t\t" << s->LatexName() << std::endl;
    if (systlist.size()==0) {std::cout << "None" << std::endl;}

    std::cout << "Loading state from " << stateFname << std::endl; 
  }

  TFile fin(stateFname);
  //GetListOfSysts();
  PredictionInterp& predInt_FDNumuFHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_numu_fhc")).release();
  PredictionInterp& predInt_FDNueFHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_nue_fhc")).release();
  PredictionInterp& predInt_FDNumuRHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_numu_rhc")).release();
  PredictionInterp& predInt_FDNueRHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_nue_rhc")).release();

  //PredictionInterp& predInt_FDNumuFHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("fd_interp_numu_fhc")).release();
  //PredictionInterp& predInt_FDNueFHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("fd_interp_nue_fhc")).release();
  //PredictionInterp& predInt_FDNumuRHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("fd_interp_numu_rhc")).release();
  //PredictionInterp& predInt_FDNueRHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("fd_interp_nue_rhc")).release();

  fin.Close();
  std::cout << "Done loading state" << std::endl;

  TFile* fout = new TFile(outputFname, "RECREATE");

  TGraph* gMH_NH = new TGraph();
  TGraph* gMH_IH = new TGraph();
  //for(int hie = -1; hie <= +1; hie += 2){
  for(int hie = 1; hie <= +1; hie += 2){

    bool oscvar = true;
    double dcpstep = 2*TMath::Pi()/36;
    double thisdcp;

    for(double idcp = 0; idcp < 37; ++idcp) {
	
      thisdcp = -TMath::Pi() + idcp*dcpstep;

      osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie);

      trueOsc->SetdCP(thisdcp);

      const Spectrum data_nue_fhc_syst = predInt_FDNueFHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment app_expt_fhc_syst(&predInt_FDNueFHC, data_nue_fhc_syst);
      app_expt_fhc_syst.SetMaskHist(0.5,8.0);

      const Spectrum data_nue_rhc_syst = predInt_FDNueRHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment app_expt_rhc_syst(&predInt_FDNueRHC, data_nue_rhc_syst);
      app_expt_rhc_syst.SetMaskHist(0.5,8.0);

      const Spectrum data_numu_fhc_syst = predInt_FDNumuFHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment dis_expt_fhc_syst(&predInt_FDNumuFHC, data_numu_fhc_syst);
      dis_expt_fhc_syst.SetMaskHist(0.5,8.0);

      const Spectrum data_numu_rhc_syst = predInt_FDNumuRHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment dis_expt_rhc_syst(&predInt_FDNumuRHC, data_numu_rhc_syst);
      dis_expt_rhc_syst.SetMaskHist(0.5,8.0);

      std::vector<const IFitVar*> oscVars =
	{&kFitDmSq32Scaled, &kFitSinSqTheta23, &kFitTheta13, 
	 &kFitSinSq2Theta12, &kFitDmSq21,
	 &kFitDeltaInPiUnits, &kFitRho};

      std::vector<const IFitVar*> oscVars_scan = {};

      double chisqmin = 99999;
      double thischisq;

      for(int ioct = -1; ioct <= 1; ioct += 2) {
	//osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalcCDR(hie);	
	osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie,ioct);	
	testOsc->SetDmsq32(-1*testOsc->GetDmsq32());

	//if (ioct < 0) {
	//testOsc->SetTh23(TMath::PiOver2() - testOsc->GetTh23());
	//}

	if (prescan) {
	  double scanmin = 99999;
	  for (int idcp_scan = 0; idcp_scan < 41; ++idcp_scan) {
	    double dcp_scan = -TMath::Pi() + idcp_scan*2*TMath::Pi()/40;
	    for (int ith_scan = 0; ith_scan < 41; ++ith_scan) {
	      double th13_scan = (2.5 + ith_scan*9.0/40)*TMath::Pi()/180;
	      osc::IOscCalculatorAdjustable* scanOsc = testOsc->Copy();
	      scanOsc->SetdCP(dcp_scan);
	      scanOsc->SetTh13(th13_scan);
	      osc::IOscCalculatorAdjustable* cvcalc = scanOsc->Copy();	  
	      MultiExperiment full_expt_syst({&app_expt_fhc_syst, &app_expt_rhc_syst, &dis_expt_fhc_syst, &dis_expt_rhc_syst});
	      Fitter fit_scan(&full_expt_syst, oscVars_scan, {});
	      double scanchisq = fit_scan.Fit(scanOsc, MinuitFitter::kQuiet);
	      if (scanchisq < scanmin) {
		scanmin = scanchisq;
		testOsc->SetdCP(dcp_scan);
		testOsc->SetTh13(th13_scan);
	      }
	      //std::cout << "scanning: " << thisdcp << " " << dcp_scan << " " << th13_scan << " " << scanchisq << " " << scanmin << std::endl;
	    }
	  }
	}
	else {
	  testOsc->SetdCP(thisdcp);
	}

	//osc::IOscCalculatorAdjustable* cvcalc = testOsc->Copy();	  
	//Penalizer_GlbLikeCDR penalty(cvcalc,hie); 
	Penalizer_GlbLike penalty(hie,ioct,th13penalty,false,false);

	MultiExperiment full_expt_syst({&app_expt_fhc_syst, &app_expt_rhc_syst, &dis_expt_fhc_syst, &dis_expt_rhc_syst, &penalty});

	MinuitFitter fit_syst(&full_expt_syst, oscVars, systlist);

	//std::cout << "before: " << thisdcp << " " << testOsc->GetdCP() << " " << testOsc->GetTh13()*180/TMath::Pi() << std::endl;
	thischisq = fit_syst.Fit(testOsc, IFitter::kQuiet)->EvalMetricVal();
	//std::cout << "after: " << thisdcp << " " << testOsc->GetdCP() << " " << testOsc->GetTh13()*180/TMath::Pi() << " " << thischisq << std::endl;

	chisqmin = TMath::Min(thischisq,chisqmin);
      }

      chisqmin = TMath::Max(chisqmin,1e-6);
      std::cout << thisdcp << " " << TMath::Sqrt(chisqmin) << std::endl;
      if (hie > 0) {
	gMH_NH->SetPoint(gMH_NH->GetN(),thisdcp/TMath::Pi(),TMath::Sqrt(chisqmin));
      }
      else {
	gMH_IH->SetPoint(gMH_IH->GetN(),thisdcp/TMath::Pi(),TMath::Sqrt(chisqmin));
      }
    }
  }
  gMH_NH->Write("sens_mh_nh");
  gMH_IH->Write("sens_mh_ih");
  fout->Close();
}
