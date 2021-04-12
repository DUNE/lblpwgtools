#include "CAFAna/Analysis/common_fit_definitions.h"

using namespace ana;

void cpv_joint_optrun(std::string stateFname="/pnfs/dune/persistent/users/dmendez/CAFAnaInputs/State",
          std::string tagFname="suffix",
          std::string systSet = "nosyst", std::string sampleString="ndfd:7years",
          std::string penaltyString="nopen", int hie=1, std::string asimov_set="0",
          float years_fhc=3.5, float years_rhc=3.5){

  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(systSet);

  // Oscillation parameters to use
  std::vector<const IFitVar*> oscVars = GetOscVars("th23:th13:dmsq32", hie);

  std::string expfhc = Form("%0.f",years_fhc*100);
  std::string exprhc = Form("%0.f",years_rhc*100);
  std::string hierarchy = (hie==1 ? "nh":"ih");
  std::string outputFname = "cpv_"+sampleString+"_"+systSet+"_"+penaltyString+"_"+hierarchy+"_asimov"+asimov_set;
  outputFname += "__fhc"+exprhc+"_rhc"+exprhc+".root";
  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();

  double dcpstep = 2*TMath::Pi()/36;
  TGraph* gCPV = new TGraph();

  for(double idcp = 0; idcp < 37; ++idcp) {
    
    std::cout << "Trying idcp = " << idcp << std::endl;
    
    double thisdcp = -TMath::Pi() + idcp*dcpstep;
    osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie, 1, asimov_set);
    trueOsc->SetdCP(thisdcp);

    double chisqmin = 99999;
    double thischisq;
    
    // Still need to loop over dcp choices
    for (int idcp = 0; idcp < 2; ++idcp) {
      double dcptest = idcp*TMath::Pi();
      
      //for(int ihie = -1; ihie <= +1; ihie += 2) {
	
      for (int ioct = -1; ioct <= 1; ioct +=2) {
	
	osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie, ioct, asimov_set);
	testOsc->SetdCP(dcptest);
	
	// Make a map of seed points to try (replaces the old loops)
	std::map<const IFitVar*, std::vector<double>> oscSeeds = {};
	
	IExperiment *penalty = GetPenalty(hie, ioct, penaltyString, asimov_set);
	SystShifts trueSyst = kNoShift;
	SystShifts testSyst = kNoShift;
	
	// thischisq = RunFitPoint(stateFname, sampleString,
	// 			trueOsc, trueSyst, false,
	// 			oscVars, systlist,
	// 			testOsc, testSyst,
	// 			oscSeeds, penalty, Fitter::kNormal, nullptr);
  thischisq = RunFitPoint(years_fhc, years_rhc,
         stateFname, sampleString,
         trueOsc, trueSyst, false,
         oscVars, systlist,
         testOsc, testSyst,
         oscSeeds, penalty, Fitter::kNormal, nullptr);
	
	chisqmin = TMath::Min(thischisq,chisqmin);
	delete penalty;
      }
    }
    //    }
    
    chisqmin = TMath::Max(chisqmin,1e-6);
    gCPV->SetPoint(gCPV->GetN(),thisdcp/TMath::Pi(),TMath::Sqrt(chisqmin));
  }

  fout->cd();
  gCPV->Draw("ALP");
  gCPV->Write(hie > 0 ? "sens_cpv_nh" : "sens_cpv_ih");
  fout->Close();
}
