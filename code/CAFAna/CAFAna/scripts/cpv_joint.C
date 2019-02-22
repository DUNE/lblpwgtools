#include "common_fit_definitions.C"

void cpv_joint(std::string stateFname="common_state_mcc11v3.root",
	       std::string outputFname="cpv_sens_ndfd_nosyst.root",
	       std::string systSet = "nosyst", std::string sampleString="ndfd",
	       std::string penaltyString="nopen", int asimov_set=0){
  
  gROOT->SetBatch(1);

  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(systSet);
  RemoveSysts(systlist, {"MFP_N", "MFP_pi"});
  
  // Oscillation parameters to use
  std::vector<const IFitVar*> oscVars = {&kFitDmSq32Scaled, &kFitSinSqTheta23, &kFitTheta13,
					 &kFitSinSq2Theta12, &kFitDmSq21, &kFitRho};
  
  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();
  for(int hie = -1; hie <= +1; hie += 2){

    double dcpstep = 2*TMath::Pi()/36;
    TGraph* gCPV = new TGraph();
    double thisdcp;

    for(double idcp = 0; idcp < 37; ++idcp) {
      
      std::cout << "Trying idcp = " << idcp << std::endl;
      
      thisdcp = -TMath::Pi() + idcp*dcpstep;

      osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie, 1, asimov_set);
      trueOsc->SetdCP(thisdcp);
      
      double chisqmin = 99999;
      double thischisq;
      
      // Still need to loop over dcp choices
      for (int idcp = 0; idcp < 2; ++idcp) {
	double dcptest = idcp*TMath::Pi();

	for(int ihie = -1; ihie <= +1; ihie += 2) {

	  for (int ioct = -1; ioct <= 1; ioct +=2) {

	    osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(ihie, ioct, asimov_set);	
	    testOsc->SetdCP(dcptest);
	  
	    // Make a map of seed points to try (replaces the old loops)
	    std::map<const IFitVar*, std::vector<double>> oscSeeds = {};

	    IExperiment *penalty = GetPenalty(ihie, ioct, penaltyString);
	    SystShifts trueSyst = kNoShift;
	    SystShifts testSyst = kNoShift;
	    
	    thischisq = RunFitPoint(stateFname, sampleString,
				    trueOsc, trueSyst, false,
				    oscVars, systlist,
				    testOsc, testSyst,
				    oscSeeds, penalty);
	    
	    chisqmin = TMath::Min(thischisq,chisqmin);
	    delete penalty;
	  }
	}
      }

      chisqmin = TMath::Max(chisqmin,1e-6);      
      gCPV->SetPoint(gCPV->GetN(),thisdcp/TMath::Pi(),TMath::Sqrt(chisqmin));
    }

    fout->cd();
    gCPV->Draw("ALP");
    gCPV->Write(hie > 0 ? "sens_cpv_nh" : "sens_cpv_ih");
  }
  fout->Close();
}

