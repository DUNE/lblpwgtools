#include "CAFAna/Analysis/common_fit_definitions.h"
#include "TGraph2D.h"

using namespace ana;

void cpv_joint(std::string stateFname="common_state_mcc11v3.root",
               std::string outputFname="cpv_sens_ndfd_nosyst_2D.root",
               std::string systSet = "nosyst", std::string sampleString="ndfd",
               std::string penaltyString="nopen", int hie=1, std::string asimov_set="0",
               int isetCP = 0, int isetTh13 = 0,
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
      std::cout << "Removing " << syst->ShortName() << std::endl;
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

  // Now loop over all true values for dcp
  int nsteps_dcp = 18;
  double dcpstep = 2 * TMath::Pi() / nsteps_dcp;

  // Loop for theta13: Range from 0 to 20 degrees (in radians, this is from 0 to ~0.349 radians)
  int nsteps_th13 = 18;
  double th13_min = 0;  // 0 degrees in radians
  double th13_max = 20 * TMath::Pi() / 180;  // 20 degrees in radians
  double th13step = (th13_max - th13_min) / nsteps_th13;  // Step size for theta13

  // Test theta13 value from isetTh13 (the fixed value for testOsc)
  double testTh13 = th13_min + isetTh13 * th13step;

  TGraph2D* gCPV_Th13 = new TGraph2D();

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


  for(double ith13 = 0; ith13 < nsteps_th13 + 1; ++ith13) {
    double trueTh13 = th13_min + ith13 * th13step;  // Looping over trueOsc theta13 values

    for(double idcp = 0; idcp < nsteps_dcp + 1; ++idcp) {

      std::cout << "Trying idcp = " << idcp << " and ith13 = " << ith13 << std::endl;

      double thisdcp = -TMath::Pi() + idcp * dcpstep;

      // Create the trueOsc (looped over in theta13 and dcp)
      osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie, 1, asimov_set);
      trueOsc->SetdCP(thisdcp);
      trueOsc->SetTh13(trueTh13);  // Setting theta13 value for this scan point in trueOsc

      double chisqmin = 99999;

      for (int idcp_test = 0; idcp_test < 1; ++idcp_test) {
        double dcptest = -TMath::Pi() + isetCP * dcpstep;
        std::cout << " ---- the true and test dcp values: " << thisdcp << " " << dcptest << std::endl;

        for(int ihie = -1; ihie <= +1; ihie += 2) {

          // Create the testOsc (fixed at isetTh13 and isetCP)
          std::vector<const IFitVar*> oscVars = GetOscVars("th23:dmsq32", ihie);
          osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(ihie, 1, asimov_set);
          testOsc->SetdCP(dcptest);
          testOsc->SetTh13(testTh13);  // Fixed theta13 value for testOsc based on isetTh13

          IExperiment *penalty = GetPenalty(ihie, 1, penaltyString, asimov_set);
          SystShifts testSyst = kNoShift;

          thischisq = RunFitPoint(stateFname, sampleString,
                                  trueOsc, trueSyst, false,
                                  oscVars, systlist,
                                  testOsc, testSyst,
                                  oscSeeds, penalty, Fitter::kNormal, nullptr);

          chisqmin = TMath::Min(thischisq, chisqmin);
          delete penalty;
          delete testOsc;
        }
      }
      delete trueOsc;
      std::cout << " ---- current chi2 " << chisqmin << std::endl;

      chisqmin = TMath::Max(chisqmin, 1e-6);
      double diff = chisqmin - glob_chisqmin;
      diff = TMath::Max(diff, 0.);
      gCPV_Th13->SetPoint(gCPV_Th13->GetN(), thisdcp / TMath::Pi(), trueTh13, sqrt(diff));
    }
  }

  fout->cd();
  gCPV_Th13->Write("sens_cpv_th13_vs_dcp");
  fout->Close();
}


int main(int argc, char* argv[]){
  const char* stateFname = argv[1];
  const char* outputFname = argv[2];
  const char* systSet = argv[3];
  int icp = atoi(argv[4]);
  int ith13 = atoi(argv[5]);
  cpv_joint(stateFname, outputFname,
            systSet, "ndfd:10year",
            "nopen", 1, "0", icp, ith13);
}

