#include "common_fit_definitions.C"

void fit_covar(std::string stateFname = "common_state_mcc11v3.root",
               std::string outputFname = "covar_various_asimov.root",
               std::string systSet = "flux", std::string sampleString = "ndfd",
               std::string penaltyString = "", std::string FakeDataSysts = "",
               double FakeDataValue = 1, std::string OtherSystsToExclude = "") {

  gROOT->SetBatch(1);
  gRandom->SetSeed(0);
  
  int hie = 1;
  
  // Get the systematics to use
  std::vector<const ISyst *> systlist = GetListOfSysts(systSet);

  // Oscillation parameters to use
  std::vector<const IFitVar *> oscVars = GetOscVars("alloscvars", hie);

  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");

  osc::IOscCalculatorAdjustable *trueOsc = NuFitOscCalc(hie);

  SystShifts trueSyst = kNoShift;

  // Can build fake data fits easily by passing syst parameter names to fix to 1
  // and then remove from the fit.
  if (FakeDataSysts.size()) {
    // Get all of the systs (notably including missing proton energy fake data)
    std::vector<const ISyst *> all_systs = GetListOfSysts();
    for (ISyst const *s : all_systs) {
      if (FakeDataSysts.find(s->ShortName()) != std::string::npos) {
        trueSyst.SetShift(s, FakeDataValue);
        auto it = std::find(systlist.begin(), systlist.end(), s);
        if (it != systlist.end()) {
          systlist.erase(it);
        }
      }
      if (OtherSystsToExclude.find(s->ShortName()) != std::string::npos) {
        auto it = std::find(systlist.begin(), systlist.end(), s);
        if (it != systlist.end()) {
          systlist.erase(it);
        }
      }
    }
  }

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

  double thischisq =
      RunFitPoint(stateFname, sampleString, trueOsc, trueSyst, false, oscVars,
                  systlist, testOsc, testSyst, oscSeeds, penalty,
                  Fitter::kNormal | Fitter::kIncludeHesse, fout);
  delete penalty;

  // Now close the file
  fout->Close();
}
