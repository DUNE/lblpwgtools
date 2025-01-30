#include "CAFAna/Analysis/common_fit_definitions.h"

using namespace ana;

void cpv_joint(std::string stateFname="common_state_mcc11v3.root",
	       std::string outputFname="cpv_sens_ndfd_nosyst.root",
	       std::string systSet = "nosyst", std::string sampleString="ndfd",
	       std::string penaltyString="nopen", int hie=1, std::string asimov_set="0",
	       int isetCP = 0,
	       std::string fakeDataShift = "", int fitBias = 0){

  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  // Allow a fake data bias
  SystShifts trueSyst = GetFakeDataSystShift(fakeDataShift);

  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(systSet);

  // Should the fake data dials be removed from the systlist?
  if (!fitBias){

    std::vector<std::string> bias_syst_names;
    // Loop over all systs set to a non-nominal value and remove
    for (auto syst : trueSyst.ActiveSysts()){
      std::cout << "Removing " << syst->ShortName() <<std::endl;
      bias_syst_names.push_back(syst->ShortName());
    }
    RemoveSysts(systlist, bias_syst_names);
  }

  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();

  // First find the minimum for dcp = 0
  std::map<const IFitVar*, std::vector<double>> oscSeeds = {};
  oscSeeds[&kFitSinSqTheta23] = {0.4, 0.6};

  osc::IOscCalculatorAdjustable* trueOscGlob = NuFitOscCalc(hie, 1, asimov_set);
  trueOscGlob->SetdCP(0);

  double glob_chisqmin = 99999;
  double thischisq;

  // Now loop over all true values
  int nsteps = 36;
  double dcpstep = 2*TMath::Pi()/nsteps;
  TGraph* gCPV = new TGraph();


  // Have to loop because I need to change the testOsc dcp value
  for (int idcp = 0; idcp < 2; ++idcp) {
  //for (int idcp = 0; idcp < 1; ++idcp) {
    for(int ihie = -1; ihie <= +1; ihie += 2) {
      double dcptest = idcp*TMath::Pi();
      //double dcptest = -TMath::Pi() + isetCP*dcpstep;

      std::vector<const IFitVar*> oscVars = GetOscVars("th23:th13:dmsq32", ihie);      
      osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(ihie, 1, asimov_set);
      testOsc->SetdCP(dcptest);
      
      IExperiment *penalty = GetPenalty(ihie, 1, penaltyString, asimov_set);
      SystShifts testSyst = kNoShift;
      
      thischisq = RunFitPoint(stateFname, sampleString,
			      trueOscGlob, trueSyst, false,
			      oscVars, systlist,
			      testOsc, testSyst,
			      oscSeeds, penalty, 
			      Fitter::kNormal, nullptr);
      
      glob_chisqmin = TMath::Min(thischisq,glob_chisqmin);
      delete penalty;
      delete testOsc;
    }
  }
  delete trueOscGlob;
  std::cout << "Global chi2 at dCP=0,pi: " << glob_chisqmin << std::endl;


  for(double idcp = 0; idcp < nsteps+1; ++idcp) {
    
    std::cout << "Trying idcp = " << idcp << std::endl;
    
    double thisdcp = -TMath::Pi() + idcp*dcpstep;
    osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie, 1, asimov_set);
    trueOsc->SetdCP(thisdcp);

    double chisqmin = 99999;
    
    // Still need to loop over dcp choices
    //for (int idcp = 0; idcp < 2; ++idcp) {
    for (int idcp = 0; idcp < 1; ++idcp) {
      //double dcptest = idcp*TMath::Pi();
      double dcptest = -TMath::Pi() + isetCP*dcpstep;
      std::cout<<" ---- the true and test dcp values: "<<thisdcp<<" "<<dcptest<<std::endl;

      for(int ihie = -1; ihie <= +1; ihie += 2) {

	std::vector<const IFitVar*> oscVars = GetOscVars("th23:th13:dmsq32", ihie);	
	osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(ihie, 1, asimov_set);
	testOsc->SetdCP(dcptest);
	
	IExperiment *penalty = GetPenalty(ihie, 1, penaltyString, asimov_set);
	SystShifts testSyst = kNoShift;
	
	thischisq = RunFitPoint(stateFname, sampleString,
				trueOsc, trueSyst, false,
				oscVars, systlist,
				testOsc, testSyst,
				oscSeeds, penalty, Fitter::kNormal, nullptr);
	
	chisqmin = TMath::Min(thischisq,chisqmin);
	delete penalty;
	delete testOsc;
      }
    }
    delete trueOsc;
    std::cout<<" ---- current chi2 "<<chisqmin<<std::endl;   
 
    chisqmin = TMath::Max(chisqmin,1e-6);
    double diff = chisqmin-glob_chisqmin;
    diff = TMath::Max(diff, 0.);
    gCPV->SetPoint(gCPV->GetN(),thisdcp/TMath::Pi(),sqrt(diff));
  }

  fout->cd();
  gCPV->Draw("ALP");
  gCPV->Write(hie > 0 ? "sens_cpv_nh" : "sens_cpv_ih");
  fout->Close();
}

int main(int argc, char* argv[]){
  const char* stateFname = argv[1];
  const char* outputFname = argv[2];
  const char* systSet = argv[3];
  int icp = atoi(argv[4]);
  cpv_joint(stateFname, outputFname,
            systSet, "ndfd:10year",
            "nopen", 1, "0", icp);
}

