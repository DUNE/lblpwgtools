#include "common_fit_definitions.C"

void fit_covar(std::string stateFname  = "common_state_mcc11v3.root",
	       std::string outputFname = "covar_various_asimov.root",
	       std::string systSet = "flux", std::string sampleString="ndfd",
	       std::string penaltyString="", double NDStatsFactor = 1){

  gROOT->SetBatch(1);

  // Get the systematics to use
  std::vector<const ISyst *> systlist = GetListOfSysts(systSet);

  // Oscillation parameters to use
  std::vector<const IFitVar *> oscVars = {
      &kFitDmSq32Scaled,  &kFitSinSqTheta23, &kFitTheta13, &kFitDeltaInPiUnits,
      &kFitSinSq2Theta12, &kFitDmSq21,       &kFitRho};

  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");

  int hie = 1;

  // No fake data throw here
  osc::IOscCalculatorAdjustable *trueOsc = NuFitOscCalc(hie);
  SystShifts trueSyst = kNoShift;

  // Move the input parameters a little, just to avoid asimov fit issues in
  // MINUIT
  osc::IOscCalculatorAdjustable *testOsc = NuFitOscCalc(hie);
  SystShifts testSyst;
  for (auto s : systlist)
    testSyst.SetShift(s, GetBoundedGausThrow(s->Min() * 0.05, s->Max() * 0.05));

  // Make a map of seed points to try (replaces the old loops)
  std::map<const IFitVar *, std::vector<double>> oscSeeds = {};

  // Add a penalty term (maybe)
  IExperiment *penalty = GetPenalty(hie, 1, penaltyString);

  double thischisq = RunFitPoint(stateFname, sampleString,
				 trueOsc, trueSyst, false,
				 oscVars, systlist,
				 testOsc, testSyst,
				 oscSeeds, penalty,
				 Fitter::kNormal|Fitter::kIncludeHesse, fout, 0,
	       kAddSampleTypeToFilename);
  delete penalty;

  // Now close the file
  fout->Close();
}
