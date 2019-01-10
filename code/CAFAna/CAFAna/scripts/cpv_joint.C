// #ifdef __CINT__
// void cpv_joint(bool reload = false){}
// #else

#include "common_fit_definitions.C"

// Need to accept filename, ND/FD, systs and reload as arguments
void cpv_joint(std::string stateFname="common_state_mcc11v2.root",
	       std::string outputFname="cpv_sens_ndfd_allsyst.root",
	       std::string systSet = "flux", bool useND=true){
  
  gROOT->SetBatch(1);

  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(systSet);

  // Oscillation parameters to use
  std::vector<const IFitVar*> oscVars =
    {&kFitDmSq32Scaled, &kFitSinSqTheta23,
     &kFitTheta13, &kFitRho};
  
  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();
  for(int hie = -1; hie <= +1; hie += 2){

    const std::string hieStr = (hie > 0) ? "nh" : "ih";
    double dcpstep = 2*TMath::Pi()/36;
    TGraph* gCPV = new TGraph();
    double thisdcp;

    for(double idcp = 0; idcp < 37; ++idcp) {
      
      std::cout << "Trying idcp = " << idcp << std::endl;
      
      thisdcp = -TMath::Pi() + idcp*dcpstep;

      osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie);
      trueOsc->SetdCP(thisdcp);
      
      double chisqmin = 99999;
      double thischisq;
      
      // Still need to loop over dcp choices
      for (int idcp = 0; idcp < 2; ++idcp) {
	
	osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie);	
	double dcptest = idcp*TMath::Pi();
	testOsc->SetdCP(dcptest);
	
	// Make a map of seed points to try (replaces the old loops)
	std::map<const IFitVar*, std::vector<double>> oscSeeds = {};
	oscSeeds[&kFitSinSqTheta23] = {testOsc->GetTh23(), TMath::Pi()/2 - testOsc->GetTh23()}; // try both octants
	oscSeeds[&kFitDmSq32Scaled] = {testOsc->GetDmsq32()*1000., -1*testOsc->GetDmsq32()*1000.}; // try both hierarchies
	
	Penalizer_GlbLike penalty(hie);
	SystShifts trueSyst = kNoShift;
	SystShifts testSyst = kNoShift;
	
	thischisq = RunFitPoint(stateFname, useND ? pot_nd : 0, useND ? pot_nd : 0, pot_fd, pot_fd,
				trueOsc, trueSyst, false,
				oscVars, systlist,
				testOsc, testSyst,
				oscSeeds, &penalty);
	
	chisqmin = TMath::Min(thischisq,chisqmin);
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

//#endif
