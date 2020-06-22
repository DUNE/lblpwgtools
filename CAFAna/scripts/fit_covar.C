#include "CAFAna/Analysis/common_fit_definitions.h"

using namespace ana;

char const *def_stateFname = "common_state_mcc11v3.root";
char const *def_outputFname = "covar_various_asimov.root";
char const *def_systSet = "nosyst";
char const *def_sampleString = "ndfd";
char const *def_fakeDataShift = "";
char const *def_penaltyString = "nopen";
int const def_hie = 1;
char const *def_asimov_set = "0";
char const *def_oscVarString = "alloscvars";

void fit_covar(std::string stateFname = def_stateFname,
               std::string outputFname = def_outputFname,
               std::string systSet = def_systSet,
               std::string sampleString = def_sampleString,
               std::string fakeDataShift = def_fakeDataShift,
               std::string penaltyString = def_penaltyString, int hie = def_hie,
               std::string asimov_set = def_asimov_set,
               std::string oscVarString = def_oscVarString) {

  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  // Get the systematics to use
  std::vector<const ISyst *> systlist = GetListOfSysts(systSet);
  std::cout << "[INFO]: Using " << systlist.size()
            << " systematic dials: " << std::endl;
  for (auto s : systlist) {
    std::cout << "[INFO]:\t" << s->ShortName() << std::endl;
  }

  // Oscillation parameters to use
  std::vector<const IFitVar *> oscVars = {};
  if (sampleString.find("fd") != std::string::npos) {
    oscVars = GetOscVars(oscVarString, hie);
  }

  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");

  FitTreeBlob ftb("fit_info","meta_tree");
  ftb.SetDirectory(fout);

  osc::IOscCalculatorAdjustable *trueOsc = NuFitOscCalc(hie, 1, asimov_set);

  SystShifts trueSyst = GetFakeDataGeneratorSystShift(fakeDataShift);

  // Move the input parameters a little, just to avoid asimov fit issues in
  // MINUIT
  osc::IOscCalculatorAdjustable *testOsc = NuFitOscCalc(hie, 1, asimov_set);
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
                  MinuitFitter::kNormal | MinuitFitter::kIncludeHesse, fout, &ftb);
  delete penalty;

  ftb.Fill();
  ftb.Write();
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
  std::string fakeDataShift = (argc > 5) ? argv[5] : def_fakeDataShift;
  std::string penaltyString = (argc > 6) ? argv[6] : def_penaltyString;
  int hie = (argc > 7) ? atoi(argv[7]) : def_hie;
  std::string asimov_set = (argc > 8) ? argv[8] : def_asimov_set;
  std::string oscVarString = (argc > 9) ? argv[9] : def_oscVarString;

  fit_covar(stateFname, outputFname, systSet, sampleString, fakeDataShift,
            penaltyString, hie, asimov_set, oscVarString);
}
#endif
