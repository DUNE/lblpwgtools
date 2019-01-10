// ETW May 2018
// Standard script for DUNE CPV sensitivity
// Input files use TensorFlow CVN training from May 2018 

#ifdef __CINT__
void cpv_joint(bool reload = false){}
#else

#include "common_fit_definitions.C"

std::string stateFname  = "common_state_mcc11v2.root";
std::string outputFname = "cpv_sens_ndfd_allsyst.root";

//Set systematics style by hand for now
bool detsyst  = false;
bool fluxsyst = true;
bool xsecsyst = false;

// Need to accept filename, ND/FD, systs and reload as arguments
void cpv_joint(bool reload = false){
  
  gROOT->SetBatch(1);

  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(fluxsyst, xsecsyst, detsyst);

  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();
  for(int hie = -1; hie <= +1; hie += 2){

    const std::string hieStr = (hie > 0) ? "nh" : "ih";
    bool oscvar = true;
    double dcpstep = 2*TMath::Pi()/36;
    TGraph* gCPV_NH = new TGraph();
    TGraph* gCPV_IH = new TGraph();
    double thisdcp;

    for(double idcp = 0; idcp < 37; ++idcp) {
	
      thisdcp = -TMath::Pi() + idcp*dcpstep;

      osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie);
      trueOsc->SetdCP(thisdcp);

      MultiExperiment my_expt = GetMultiExperiment(stateFname, pot_nd, pot_nd, pot_fd, pot_fd, trueOsc);
      
      std::vector<const IFitVar*> oscVars =
	{&kFitDmSq32Scaled, &kFitSinSqTheta23,
	 &kFitTheta13, &kFitRho};
      
      double chisqmin = 99999;
      double thischisq;
      
      for(int ihie = -1; ihie <= +1; ihie += 2) {
	for (int idcp = 0; idcp < 2; ++idcp) {
	  for (int ioct = -1; ioct <= 1; ioct +=2) {
	    osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie);	
	    double dcptest = idcp*TMath::Pi();
	    testOsc->SetdCP(dcptest);

	    if (ihie < 0) testOsc->SetDmsq32(-1*testOsc->GetDmsq32());
	    if (ioct < 0) testOsc->SetTh23(TMath::Pi()/2 - testOsc->GetTh23());

	    osc::IOscCalculatorAdjustable* cvcalc = testOsc->Copy();	  
	    Penalizer_GlbLike penalty(cvcalc,hie);

	    // Got to do something about the penalizer here...
	    MultiExperiment this_expt({&my_expt, &penalty});	    
	    Fitter this_fit(&this_expt, oscVars, systlist);

	    std::map<const IFitVar*, std::vector<double>> oscSeeds;
	    oscSeeds[&kFitSinSqTheta23] = {.4, .6}; // try both octants
	    SystShifts testSyst = kNoShift;
	    thischisq = this_fit.Fit(testOsc, testSyst, {}, {}, Fitter::kVerbose);
	    
	    chisqmin = TMath::Min(thischisq,chisqmin);
	  }
	}
      }
      
      chisqmin = TMath::Max(chisqmin,1e-6);
      if (hie > 0) gCPV_NH->SetPoint(gCPV_NH->GetN(),thisdcp/TMath::Pi(),TMath::Sqrt(chisqmin));
      else gCPV_IH->SetPoint(gCPV_IH->GetN(),thisdcp/TMath::Pi(),TMath::Sqrt(chisqmin));
    }
    fout->cd();
    if (hie > 0) {
      gCPV_NH->Draw("ALP");
      gCPV_NH->Write("sens_cpv_nh");
    } else {
      gCPV_IH->Draw("ALP");
      gCPV_IH->Write("sens_cpv_ih");
    }
  }
  fout->Close();
}

#endif
