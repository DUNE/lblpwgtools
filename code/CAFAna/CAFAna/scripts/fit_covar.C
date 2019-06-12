#include "common_fit_definitions.C"

char const *def_stateFname = "common_state_mcc11v3.root";
char const *def_outputFname = "covar_various_asimov.root";
char const *def_systSet = "nosyst";
char const *def_sampleString = "ndfd";
char const *def_penaltyString = "nopen";
int const def_hie = 1;
char const *def_asimov_set = "0";
char const *def_oscVarString = "th13:deltapi:th23:dmsq32";

void fit_covar(std::string stateFname = def_stateFname,
               std::string outputFname = def_outputFname,
               std::string systSet = def_systSet,
               std::string sampleString = def_sampleString,
               std::string penaltyString = def_penaltyString, int hie = def_hie,
               std::string asimov_set = def_asimov_set,
               std::string oscVarString = def_oscVarString) {

  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  // Get the systematics to use
  std::vector<const ISyst *> systlist = GetListOfSysts(systSet);

  // Oscillation parameters to use
  std::vector<const IFitVar *> oscVars = GetOscVars(oscVarString, hie);

  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");

  osc::IOscCalculatorAdjustable *trueOsc = NuFitOscCalc(hie, 1, asimov_set);

  // Move the input parameters a little, just to avoid asimov fit issues in
  // MINUIT
  osc::IOscCalculatorAdjustable *testOsc = NuFitOscCalc(hie, 1, asimov_set);
  SystShifts testSyst;
  for (auto s : systlist)
    testSyst.SetShift(s, GetBoundedGausThrow(s->Min() * 0.05, s->Max() * 0.05));

  // Make a map of seed points to try (replaces the old loops)
  std::map<const IFitVar *, std::vector<double>> oscSeeds = {};

  // Add a penalty term (maybe)
  IExperiment *penalty = GetPenalty(hie, 1, penaltyString);

  double thischisq =
      RunFitPoint(stateFname, sampleString, trueOsc, kNoShift, false, oscVars,
                  systlist, testOsc, testSyst, oscSeeds, penalty,
                  Fitter::kNormal | Fitter::kIncludeHesse, fout);
  delete penalty;

  // Now close the file
  fout->Close();
}

#ifndef __CINT__
int main(int argc, char const *argv[]) {

  gROOT->SetMustClean(false);

  std::string stateFname = (argc > 1) ? argv[1] : def_stateFname;
  std::string outputFname = (argc > 2) ? argv[2] : def_outputFname;
  std::string systSet = (argc > 3) ? argv[3] : def_systSet;
  std::string sampleString = (argc > 4) ? argv[4] : def_sampleString;
  std::string penaltyString = (argc > 5) ? argv[5] : def_penaltyString;
  int hie = (argc > 6) ? atoi(argv[6]) : def_hie;
  std::string asimov_set = (argc > 7) ? argv[7] : def_asimov_set;
  std::string oscVarString = (argc > 8) ? argv[8] : def_oscVarString;

  fit_covar(stateFname, outputFname, systSet, sampleString, penaltyString, hie,
            asimov_set, oscVarString);
}
#endif
