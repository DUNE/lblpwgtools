#include "common_fit_definitions.C"

void mh_joint(std::string stateFname="common_state_mcc11v3.root",
	      std::string outputFname="mh_sens_ndfd_nosyst.root",
	      std::string systSet = "nosyst", bool useND=true,
	      std::string penaltyString=""){
  
  gROOT->SetBatch(1);

  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(systSet);
  RemoveSysts(systlist, {"MFP_N", "MFP_pi"});
  
  // Oscillation parameters to use
  std::vector<const IFitVar*> oscVars = {&kFitDmSq32Scaled, &kFitSinSqTheta23, &kFitTheta13,
					 &kFitSinSq2Theta12, &kFitDmSq21,
					 &kFitDeltaInPiUnits, &kFitRho};
  
  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();
  for(int hie = -1; hie <= +1; hie += 2){

    double dcpstep = 2*TMath::Pi()/36;
    TGraph* gMH = new TGraph();
    double thisdcp;

    for(double idcp = 0; idcp < 37; ++idcp) {
	
      std::cout << "Trying idcp = " << idcp << std::endl;

      thisdcp = -TMath::Pi() + idcp*dcpstep;
	
      osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie);
      trueOsc->SetdCP(thisdcp);

      double chisqmin = 99999;
      double thischisq;

      for(int ioct = -1; ioct <= 1; ioct += 2) {
	
	osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie, ioct);	
	testOsc->SetdCP(thisdcp);
	// Force it into the wrong hierarchy
	testOsc->SetDmsq32(-1*testOsc->GetDmsq32());

	IExperiment *penalty = GetPenalty(hie, ioct, penaltyString);
	SystShifts trueSyst = kNoShift;
	SystShifts testSyst = kNoShift;

	std::map<const IFitVar*, std::vector<double>> oscSeeds = {};

	thischisq = RunFitPoint(stateFname, (useND) ? pot_nd : 0, (useND) ? pot_nd : 0, pot_fd, pot_fd,
				trueOsc, trueSyst, false,
				oscVars, systlist,
				testOsc, testSyst,
				oscSeeds, penalty);
	
	chisqmin = TMath::Min(thischisq,chisqmin);
	delete penalty;
      }
      
      chisqmin = TMath::Max(chisqmin,1e-6);
      gMH->SetPoint(gMH->GetN(),thisdcp/TMath::Pi(),TMath::Sqrt(chisqmin));
    }

    fout->cd();
    gMH->Draw("ALP");
    gMH->Write(hie > 0 ? "sens_mh_nh" : "sens_mh_ih");
  }
  fout->Close();
}

