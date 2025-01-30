#include "CAFAna/Analysis/common_fit_definitions.h"

using namespace ana;

void mh_joint(std::string stateFname="common_state_mcc11v3.root",
	      std::string outputFname="mh_sens_ndfd_nosyst.root",
	      std::string systSet = "nosyst", std::string sampleString = "ndfd",
	      std::string penaltyString="", int hie=1, std::string asimov_joint="0",
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
      RemoveSysts(systlist, bias_syst_names);
    }
  }

  // For the global fit
  std::vector<const IFitVar*> oscVars = GetOscVars("alloscvars", hie) ;

  // For the wrong sign fit
  std::vector<const IFitVar*> oscVarsWrong = GetOscVars("alloscvars", -1*hie) ;

  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();

  std::map<const IFitVar*, std::vector<double>> oscSeeds = {};
  oscSeeds[&kFitDeltaInPiUnits] = {-0.66, 0, 0.66};

  int nsteps = 36;
  double dcpstep = 2*TMath::Pi()/nsteps;
  TGraph* gMH = new TGraph();

  for(double idcp = 0; idcp < nsteps+1; ++idcp) {

    std::cout << "Trying idcp = " << idcp << std::endl;
    double thisdcp = -TMath::Pi() + idcp*dcpstep;

    osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie, 1, asimov_joint);
    trueOsc->SetdCP(thisdcp);
    
    double correct_chisqmin = 99999;
    double wrong_chisqmin = 99999;
    double thischisq;

    // First fit in the correct hierarchy
    for(int ioct = -1; ioct <= 1; ioct += 2) {

      osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie, ioct, asimov_joint);
      testOsc->SetdCP(thisdcp);


      IExperiment *penalty = GetPenalty(hie, ioct, penaltyString, asimov_joint);
      SystShifts testSyst = kNoShift;

      thischisq = RunFitPoint(stateFname, sampleString,
                              trueOsc, trueSyst, false,
                              oscVars, systlist,
                              testOsc, testSyst,
                              oscSeeds, penalty);

      correct_chisqmin = TMath::Min(thischisq,correct_chisqmin);
      delete penalty;
      delete testOsc;
    }

    // Now the incorrect hierarchy
    for(int ioct = -1; ioct <= 1; ioct += 2) {

      osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie, ioct, asimov_joint);
      testOsc->SetdCP(thisdcp);
      // Force it into the wrong hierarchy
      testOsc->SetDmsq32(-1*testOsc->GetDmsq32());

      IExperiment *penalty = GetPenalty(hie, ioct, penaltyString, asimov_joint);
      SystShifts testSyst = kNoShift;

      thischisq = RunFitPoint(stateFname, sampleString,
			      trueOsc, trueSyst, false,
			      oscVarsWrong, systlist,
			      testOsc, testSyst,
			      oscSeeds, penalty);

      wrong_chisqmin = TMath::Min(thischisq,wrong_chisqmin);
      delete penalty;
      delete testOsc;
    }

    double dchisq = wrong_chisqmin - correct_chisqmin;
    std::cout << "dchisq = " << dchisq << " " << wrong_chisqmin << " - " << correct_chisqmin << std::endl;
    dchisq = TMath::Max(dchisq,1e-6);
    gMH->SetPoint(gMH->GetN(),thisdcp/TMath::Pi(),TMath::Sqrt(dchisq));
    
    delete trueOsc;
  }

  fout->cd();
  gMH->Draw("ALP");
  gMH->Write(hie > 0 ? "sens_mh_nh" : "sens_mh_ih");
  fout->Close();
}
