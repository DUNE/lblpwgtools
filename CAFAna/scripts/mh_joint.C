#include "CAFAna/Analysis/common_fit_definitions.h"

using namespace ana;

// void mh_joint(std::string stateFname="common_state_mcc11v3.root",
// 	      std::string outputFname="mh_sens_ndfd_nosyst.root",
// 	      std::string systSet = "nosyst", std::string sampleString = "ndfd",
// 	      std::string penaltyString="", int hie=1, std::string asimov_joint="0"){
void mh_joint(std::string stateFname="common_state_mcc11v3.root",
        std::string tagFname="suffix",
        std::string systSet = "nosyst", std::string sampleString = "ndfd:7years",
        std::string penaltyString="", int hie=1, std::string asimov_joint="0",
        float years_fhc=3.5, float years_rhc=3.5){

  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(systSet);

  // For the global fit
  std::vector<const IFitVar*> oscVars = GetOscVars("alloscvars", hie) ;

  // For the wrong sign fit
  std::vector<const IFitVar*> oscVarsWrong = GetOscVars("alloscvars", -1*hie) ;

  std::string expfhc = Form("%0.f",years_fhc*100);
  std::string exprhc = Form("%0.f",years_rhc*100);
  std::string hierarchy = (hie==1 ? "nh":"ih");
  std::string outputFname = "mh_"+sampleString+"_"+systSet+"_"+penaltyString+"_"+hierarchy+"_asimov"+asimov_joint;
  outputFname += "__fhc"+exprhc+"_rhc"+exprhc+".root";
  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();

  double dcpstep = 2*TMath::Pi()/36;
  TGraph* gMH = new TGraph();

  for(double idcp = 0; idcp < 37; ++idcp) {

    std::cout << "Trying idcp = " << idcp << std::endl;

    double thisdcp = -TMath::Pi() + idcp*dcpstep;
    osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie, 1, asimov_joint);
    trueOsc->SetdCP(thisdcp);

    double chisqmin = 99999;
    double thischisq;

    for(int ioct = -1; ioct <= 1; ioct += 2) {

      osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie, ioct, asimov_joint);
      testOsc->SetdCP(thisdcp);
      // Force it into the wrong hierarchy
      testOsc->SetDmsq32(-1*testOsc->GetDmsq32());

      IExperiment *penalty = GetPenalty(hie, ioct, penaltyString, asimov_joint);
      SystShifts trueSyst = kNoShift;
      SystShifts testSyst = kNoShift;

      std::map<const IFitVar*, std::vector<double>> oscSeeds = {};
      oscSeeds[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};

      // thischisq = RunFitPoint(stateFname, sampleString,
			   //    trueOsc, trueSyst, false,
			   //    oscVarsWrong, systlist,
			   //    testOsc, testSyst,
			   //    oscSeeds, penalty);
      thischisq = RunFitPoint(years_fhc, years_fhc
            stateFname, sampleString,
            trueOsc, trueSyst, false,
            oscVarsWrong, systlist,
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
  fout->Close();
}
