#include "common_fit_definitions.C"

void octant_joint(std::string stateFname="common_state_mcc11v3.root",
	      std::string outputFname="octant_test.root",
	      std::string systSet = "nosyst", std::string sampleString = "ndfd",
	      std::string penaltyString="", int hie=1, std::string asimov_joint="0"){
  
  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(systSet);
  // Dummy no systematics
  std::vector<const ISyst*> systlist_none;
  
  // For the global fit
  std::vector<const IFitVar*> oscVars = GetOscVars("alloscvars", hie) ;
  // hardcode this b/c I am dumb, but it's true as long as passing alloscvars
  int kdelta = 3;

  // For the wrong sign fit
  std::vector<const IFitVar*> oscVarsWrong = GetOscVars("alloscvars", -1*hie) ;
  
  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();

  double th23max = 0.991;
  double th23min = 0.580;
  int th23steps = 10; //should be ~40 can optimize
  double th23step = (th23max-th23min)/th23steps;
  TGraph* gOct = new TGraph();

  // 
  for(int ith23 = 0; ith23 < th23steps; ++ith23) {
    double chisqmin = 99999;
	
    std::cout << "Trying ith23 = " << ith23 << std::endl;
    
    double thisth23 = th23min + ith23*th23step;	
    std::cout << "th23 this step = " << thisth23 << std::endl;

    osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie, 1, asimov_joint);
    trueOsc->SetTh23(thisth23);
    
    double thischisq;
    std::vector<double> fBestSeedValues;

    osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie, 1, asimov_joint);	
    testOsc->SetTh23(TMath::Pi()/2 - thisth23);

    std::string this_asimov = "th23:"+to_string(thisth23);
    IExperiment *penalty_oct = GetPenalty(hie, 1, penaltyString, this_asimov, true, true);

    SystShifts trueSyst = kNoShift;
    SystShifts testSyst = kNoShift;

    //Prescan in finely binned delta space without full systs
    std::map<const IFitVar*, std::vector<double>> tmposcSeeds = {};
    int prescan_steps = 5; //nominally 80 but doesn't seem to be helping much
    for (int idcp=0; idcp < prescan_steps+1; ++idcp){
      tmposcSeeds[&kFitDeltaInPiUnits].push_back(-1. + idcp*2./prescan_steps);
    }
    thischisq = RunFitPoint(stateFname, "fd",
			    trueOsc, trueSyst, false,
			    oscVars, systlist_none,
			    testOsc, testSyst,
			    fBestSeedValues,
			    tmposcSeeds, penalty_oct,
			    Fitter::kNormal,
			    NULL, nullptr,
			    nullptr, junkShifts);

    //Use minimum seed values from previous prescan to seed real fit which happens here:
    std::map<const IFitVar*, std::vector<double>> oscSeeds = {};
    oscSeeds[&kFitDeltaInPiUnits] = {fBestSeedValues[kdelta]};
    

    thischisq = RunFitPoint(stateFname, sampleString,
			    trueOsc, trueSyst, false,
			    oscVars, systlist,
			    testOsc, testSyst,
			    fBestSeedValues,
			    oscSeeds, penalty_oct);
      
    chisqmin = TMath::Min(thischisq,chisqmin);
    delete penalty_oct;
    tmposcSeeds.clear();
    oscSeeds.clear();
    
    chisqmin = TMath::Max(chisqmin,1e-6);
    gOct->SetPoint(gOct->GetN(),sin(thisth23)*sin(thisth23),TMath::Sqrt(chisqmin));

  }

  fout->cd();
  gOct->Draw("ALP");
  gOct->Write(hie > 0 ? "sens_oct_nh" : "sens_oct_ih");
  fout->Close();
}

