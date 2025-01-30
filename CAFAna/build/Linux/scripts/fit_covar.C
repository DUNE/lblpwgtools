#include "CAFAna/Analysis/common_fit_definitions.h"

using namespace ana;

char const *def_stateFname = "common_state_mcc11v3.root";
char const *def_outputFname = "covar_various_asimov.root";
char const *def_systSet = "nosyst";
char const *def_sampleString = "ndfd";
char const *def_fakeDataShift = "";
int const def_fitBias = 1;
char const *def_penaltyString = "nopen";
int const def_hie = 1;
char const *def_asimov_set = "0";

void fit_covar(std::string stateFname = def_stateFname,
               std::string outputFname = def_outputFname,
               std::string systSet = def_systSet,
               std::string sampleString = def_sampleString,
               std::string fakeDataShift = def_fakeDataShift,
	       int fitBias = def_fitBias,
               std::string penaltyString = def_penaltyString, int hie = def_hie,
               std::string asimov_set = def_asimov_set) {
  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  // This function allows any dial to be set to non-nominal
  SystShifts trueSyst = GetFakeDataSystShift(fakeDataShift);

  // Get the systematics to use
  std::vector<const ISyst *> systlist = GetListOfSysts(systSet);

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


  std::cout << "[INFO]: Using " << systlist.size()
            << " systematic dials: " << std::endl;
  for (auto s : systlist) {
    std::cout << "[INFO]:\t" << s->ShortName() << std::endl;
  }

  // Oscillation parameters to use
  std::vector<const IFitVar *> oscVars = {};
  if (sampleString.find("fd") != std::string::npos) {
    std::cout << "Adding all osc. vars to the fit" << std::endl;
    oscVars = GetOscVars("alloscvars", hie);
  }

  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");

  FitTreeBlob ftb("fit_info","meta_tree");
  ftb.SetDirectory(fout);

  osc::IOscCalculatorAdjustable *trueOsc = NuFitOscCalc(hie, 1, asimov_set);

  // Move the input parameters a little, just to avoid asimov fit issues in
  // MINUIT
  osc::IOscCalculatorAdjustable *testOsc = NuFitOscCalc(hie, 1, asimov_set);
  SystShifts testSyst;
  for (auto s : systlist)
    testSyst.SetShift(s, GetBoundedGausThrow(s->Min() * 0.05, s->Max() * 0.05));

  std::map<const IFitVar *, std::vector<double>> oscSeeds = {};
  if (sampleString.find("fd") != std::string::npos) {
    std::cout << "Trying 8 test points..." << std::endl;
    oscSeeds[&kFitSinSqTheta23] = {.4, .6}; // try both octants
    oscSeeds[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};
  }

  // Add a penalty term (maybe)
  IExperiment *penalty = GetPenalty(hie, 1, penaltyString);

  double thischisq =
      RunFitPoint(stateFname, sampleString, trueOsc, trueSyst, false, oscVars,
                  systlist, testOsc, testSyst, oscSeeds, penalty,
                  Fitter::kNormal | Fitter::kIncludeHesse, fout, &ftb);

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
  int fitBias = (argc > 6) ? atoi(argv[6]) : def_fitBias;
  std::string penaltyString = (argc > 7) ? argv[7] : def_penaltyString;
  int hie = (argc > 8) ? atoi(argv[8]) : def_hie;
  std::string asimov_set = (argc > 9) ? argv[9] : def_asimov_set;

  fit_covar(stateFname, outputFname, systSet, sampleString, fakeDataShift,
            fitBias, penaltyString, hie, asimov_set);
}
#endif
