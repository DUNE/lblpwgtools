#include "common_fit_definitions.C"

#include "CheckPointHelper.h"

char const *def_stateFname = "common_state_mcc11v3.root";
char const *def_outputFname = "throws_ndfd_nosyst.root";
int const def_nthrows = 100;
char const *def_systSet = "nosyst";
char const *def_sampleString = "ndfd";
char const *def_throwString = "stat:fake:start";
char const *def_penaltyString = "nopen";
int const def_hie = 1;
char const *def_asimov_set = "0";
char const *def_oscVarString = "alloscvars";

// Need to accept filename, ND/FD, systs and reload as arguments
void make_toy_throws(std::string stateFname = def_stateFname,
                     std::string outputFname = def_outputFname,
                     int nthrows = def_nthrows,
                     std::string systSet = def_systSet,
                     std::string sampleString = def_sampleString,
                     std::string throwString = def_throwString,
                     std::string penaltyString = def_penaltyString,
                     int hie = def_hie, std::string asimov_set = def_asimov_set,
                     std::string oscVarString = def_oscVarString) {

  gROOT->SetBatch(1);
  gRandom->SetSeed(gRNGSeed);

  CheckPointHelper chk;
  if (chk.IsCounting()) {
    nthrows = std::numeric_limits<int>::max();
  }

  // Decide what is to be thrown
  bool stats_throw, fakeoa_throw, fakenuis_throw, start_throw, central_throw;
  ParseThrowInstructions(throwString, stats_throw, fakeoa_throw, fakenuis_throw,
                         start_throw, central_throw);

  // Get the systematics to use
  std::vector<const ISyst *> systlist = GetListOfSysts(systSet);

  // Oscillation parameters to use
  std::vector<const IFitVar *> oscVars = {};

  if (sampleString.find("fd") != std::string::npos) {
    oscVars = GetOscVars(oscVarString, hie);
  }

  // Deal with seeds once
  std::map<const IFitVar *, std::vector<double>> oscSeeds;
  if (sampleString.find("fd") != std::string::npos) {
    oscSeeds[&kFitSinSqTheta23] = {.4, .6}; // try both octants
    oscSeeds[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};
  }

  // Setup output file
  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");

  FitTreeBlob pftree("fit_info");
  pftree.SetDirectory(fout);

  for (int i = 0; i < nthrows; ++i) {

    std::cout << "Starting throw " << i << std::endl;

    // Set up throws for the starting value
    SystShifts fakeThrowSyst;
    osc::IOscCalculatorAdjustable *fakeThrowOsc;

    // First deal with OA parameters
    if (fakeoa_throw || central_throw)
      fakeThrowOsc = ThrownWideOscCalc(hie, oscVars);
    else
      fakeThrowOsc = NuFitOscCalc(hie, 1, asimov_set);

#ifdef PROFILE_COUTS
    std::cout << "PROFILE: Throw " << i << " start: " << std::endl;
    std::cout << "PROFILE: \tL: " << fakeThrowOsc->GetL() << std::endl;
    std::cout << "PROFILE: \tRho: " << fakeThrowOsc->GetRho() << std::endl;
    std::cout << "PROFILE: \tDmsq21: " << fakeThrowOsc->GetDmsq21()
              << std::endl;
    std::cout << "PROFILE: \tDmsq32: " << fakeThrowOsc->GetDmsq32()
              << std::endl;
    std::cout << "PROFILE: \tTh12: " << fakeThrowOsc->GetTh12() << std::endl;
    std::cout << "PROFILE: \tTh13: " << fakeThrowOsc->GetTh13() << std::endl;
    std::cout << "PROFILE: \tTh23: " << fakeThrowOsc->GetTh23() << std::endl;
    std::cout << "PROFILE: \tdCP: " << fakeThrowOsc->GetdCP() << std::endl;
#endif

    // Now deal with systematics
    if (fakenuis_throw and not central_throw) {
      for (auto s : systlist) {
        fakeThrowSyst.SetShift(
            s, GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
#ifdef PROFILE_COUTS
        std::cout << "PROFILE: \t" << s->ShortName() << ": "
                  << fakeThrowSyst.GetShift(s) << std::endl;
#endif
      }
    } else
      fakeThrowSyst = kNoShift;

    if (central_throw) {
      for (auto s : systlist)
        s->SetCentral(GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
    }

    // Prefit
    SystShifts fitThrowSyst;
    osc::IOscCalculatorAdjustable *fitThrowOsc;
    if (start_throw) {
      for (auto s : systlist)
        fitThrowSyst.SetShift(
            s, GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
      fitThrowOsc = ThrownWideOscCalc(hie, oscVars);
    } else {
      fitThrowSyst = kNoShift;
      fitThrowOsc = NuFitOscCalc(hie, 1, asimov_set);
    }
    Fitter::Precision fitStrategy = Fitter::kNormal; //|Fitter::kIncludeHesse;

    IExperiment *penalty = GetPenalty(hie, 1, penaltyString);

    double thischisq =
        RunFitPoint(stateFname, sampleString, fakeThrowOsc, fakeThrowSyst,
                    stats_throw, oscVars, systlist, fitThrowOsc, fitThrowSyst,
                    oscSeeds, penalty, fitStrategy, nullptr, &pftree);

#ifdef PROFILE_COUTS
    size_t NRelParams = oscVars.size() + systlist.size();
    std::cout << "PROFILE: Post fit values: " << std::endl;
    for (size_t pn_it = 0; pn_it < NRelParams; ++pn_it) {
      std::cout << "PROFILE: \t" << pftree.fParamNames->at(pn_it) << ": "
                << pftree.fPostFitValues->at(pn_it) << std::endl;
    }
#endif

    pftree.Fill();
    std::cout << "Throw " << i << ": found minimum chi2 = " << thischisq
              << std::endl;
    // Done with this systematic throw
    delete penalty;

    if (chk.ShouldCheckpoint()) {
      std::cout << "[INFO]: Writing output file..." << std::endl;
      fout->Write();
      chk.NotifyCheckpoint();
    }

    if (!chk.IsSafeToStartNewUnit()) {
      std::cout
          << "[INFO]: Do not have time to finish another fit, exiting early."
          << std::endl;
      break;
    }
  }

  pftree.Write();
  // Now close the file
  fout->Write();
  fout->Close();
}

#ifndef NO_MTT_MAIN
#ifndef __CINT__
int main(int argc, char const *argv[]) {

  gROOT->SetMustClean(false);

  std::string stateFname = (argc > 1) ? argv[1] : def_stateFname;
  std::string outputFname = (argc > 2) ? argv[2] : def_outputFname;
  int nthrows = (argc > 3) ? atoi(argv[3]) : def_nthrows;
  std::string systSet = (argc > 4) ? argv[4] : def_systSet;
  std::string sampleString = (argc > 5) ? argv[5] : def_sampleString;
  std::string throwString = (argc > 6) ? argv[6] : def_throwString;
  std::string penaltyString = (argc > 7) ? argv[7] : def_penaltyString;
  int hie = (argc > 8) ? atoi(argv[8]) : def_hie;
  std::string asimov_set = (argc > 9) ? argv[9] : def_asimov_set;
  std::string oscVarString = (argc > 10) ? argv[10] : def_oscVarString;

  make_toy_throws(stateFname, outputFname, nthrows, systSet, sampleString,
                  throwString, penaltyString, hie, asimov_set, oscVarString);
}
#endif
#endif
