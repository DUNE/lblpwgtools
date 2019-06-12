#include "common_fit_definitions.C"

void fit_covar(std::string stateFname = "common_state_mcc11v3.root",
               std::string outputFname = "covar_various_asimov.root",
               std::string systSet = "flux", std::string sampleString = "ndfd",
               std::string penaltyString = "", std::string fakeDataShift="") {

  gROOT->SetBatch(1);
  gRandom->SetSeed(0);
  
  int hie = 1;
  
  bool useND = false;
  bool useFD = false;

  if (sampleString.find("nd") != std::string::npos) useND = true;
  if (sampleString.find("fd") != std::string::npos) useFD = true;
  
  // Get the systematics to use
  std::vector<const ISyst *> systlist = GetListOfSysts(systSet, useND, useFD);

  // Oscillation parameters to use
  std::vector<const IFitVar*> oscVars = {};
  if (sampleString.find("fd") != std::string::npos) {
    oscVars = GetOscVars("alloscvars", hie);
  }

  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");

  osc::IOscCalculatorAdjustable *trueOsc = NuFitOscCalc(hie);

  SystShifts trueSyst = GetFakeDataGeneratorSystShift(fakeDataShift);

  // Move the input parameters a little, just to avoid asimov fit issues in
  // MINUIT
  osc::IOscCalculatorAdjustable *testOsc = NuFitOscCalc(hie);
  SystShifts testSyst;
  for (auto s : systlist)
    testSyst.SetShift(s, GetBoundedGausThrow(s->Min() * 0.05, s->Max() * 0.05));

  std::map<const IFitVar *, std::vector<double>> oscSeeds = {};
  if (sampleString.find("fd") != std::string::npos) {
    oscSeeds[&kFitSinSqTheta23] = {.4, .6}; // try both octants
    oscSeeds[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};
  }

  // Add a penalty term (maybe)
  IExperiment *penalty = GetPenalty(hie, 1, penaltyString);

  double thischisq =
      RunFitPoint(stateFname, sampleString, trueOsc, trueSyst, false, oscVars,
                  systlist, testOsc, testSyst, oscSeeds, penalty,
                  Fitter::kNormal | Fitter::kIncludeHesse, fout);
  delete penalty;

  // Now close the file
  fout->Close();
}
