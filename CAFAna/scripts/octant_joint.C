#include "CAFAna/Analysis/common_fit_definitions.h"

using namespace ana;

void octant_joint(std::string stateFname="common_state_mcc11v3.root",
		  std::string outputFname="octant_joint.root",
		  std::string systSet = "nosyst", std::string sampleString = "ndfd",
		  std::string penaltyString="", int hie=1, std::string asimov_joint="0"){
  
  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(systSet);

  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();
  
  // Deal with seeds once
  std::map<const IFitVar*, std::vector<double>> oscSeeds;
  oscSeeds[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};
  
  double minVal = 0.3;
  double maxVal = 0.7;
  int nsteps = 32;
  double ssth23_step = (maxVal - minVal)/nsteps;

  TGraph* gOct = new TGraph();

  // Loop over fit points
  for(int issth23 = 0; issth23 < nsteps; ++issth23) {

    double this_ssth23 = minVal + issth23*ssth23_step;
    double this_th23   = asin(sqrt(this_ssth23));    
    std::cout << "Trying issth23 = " << issth23 << "; ssth23 = " << this_ssth23 << std::endl;

    // Use limited osc vars (in the wrong octant!)
    int oct = 0;
    if (this_ssth23 < 0.5) oct = 1;
    else if (this_ssth23 > 0.5) oct = -1;
    
    std::vector<const IFitVar*> oscVars = GetOscVars("alloscvars", hie, oct);
    
    osc::IOscCalcAdjustable* trueOsc = NuFitOscCalc(hie, 1, asimov_joint);
    trueOsc->SetTh23(this_th23);
    
    osc::IOscCalcAdjustable* testOsc = NuFitOscCalc(hie, 1, asimov_joint);	
    testOsc->SetTh23(TMath::Pi()/2 - this_th23);

    IExperiment *penalty = GetPenalty(hie, 1, penaltyString, asimov_joint);
    SystShifts trueSyst = kNoShift;
    SystShifts testSyst = kNoShift;

    double thischisq = RunFitPoint(stateFname, sampleString,
				   trueOsc, trueSyst, false,
				   oscVars, systlist,
				   testOsc, testSyst,
				   oscSeeds, penalty);

    double chisqmin = TMath::Max(thischisq,1e-6);
    gOct->SetPoint(gOct->GetN(),this_ssth23,TMath::Sqrt(chisqmin));
    delete penalty;
  }

  fout->cd();
  gOct->Draw("ALP");
  gOct->Write(hie > 0 ? "sens_oct_nh" : "sens_oct_ih");
  fout->Close();
}

