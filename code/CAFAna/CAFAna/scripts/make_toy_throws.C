#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/Analysis/CheckPointHelper.h"

using namespace ana;

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

  if (gRNGSeed == 0) { // if we have a time based seed its still useful for
                       // things to be reproducible, so use the time to seed a
                       // new RNG seed and spin the wheels for 100k throws;
    gRandom->SetSeed(0);
    for (size_t i = 0; i < 1E5; ++i) {
      gRNGSeed = gRandom->Integer(std::numeric_limits<unsigned>::max());
    }
  }

  std::cerr << "[RNG]: gRNGSeed = " << gRNGSeed << std::endl;

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

    std::cerr << "[FIT]: Osc Parameters: \n\t";
    for (auto s : oscVars) {
      std::cerr << s->ShortName() << " ";
    }
    std::cerr << std::endl;
  }
  std::vector<const ISyst *> flux_systs = systlist;
  KeepSysts(flux_systs, GetListOfSysts("nodet:noxsec"));
  if (flux_systs.size()) {
    std::cerr << "[FIT]: Flux Parameters: \n\t";
    for (auto s : flux_systs) {
      std::cerr << s->ShortName() << " ";
    }
    std::cerr << std::endl;
  }
  std::vector<const ISyst *> xsec_systs = systlist;
  KeepSysts(xsec_systs, GetListOfSysts("noflux:nodet"));
  if (xsec_systs.size()) {
    std::cerr << "[FIT]: XSec Parameters: \n\t";
    for (auto s : xsec_systs) {
      std::cerr << s->ShortName() << " ";
    }
    std::cerr << std::endl;
  }
  std::vector<const ISyst *> det_systs = systlist;
  KeepSysts(det_systs, GetListOfSysts("noflux:noxsec"));
  if (det_systs.size()) {
    std::cerr << "[FIT]: Det Parameters: \n\t";
    for (auto s : det_systs) {
      std::cerr << s->ShortName() << " ";
    }
    std::cerr << std::endl;
  }

  // Deal with seeds once
  std::map<const IFitVar *, std::vector<double>> oscSeeds;
  if (sampleString.find("fd") != std::string::npos) {
    oscSeeds[&kFitSinSqTheta23] = {.4, .6}; // try both octants
    oscSeeds[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};
  }

  FitTreeBlob pftree("fit_info", "param_info");

  std::stringstream CLI_ss("");
  CLI_ss << stateFname << " " << outputFname << " " << nthrows << " " << systSet
         << " " << sampleString << " " << throwString << " " << penaltyString
         << " " << hie << " " << asimov_set << " " << oscVarString;
  std::string *CLIArgs = new std::string(CLI_ss.str());
  pftree.meta_tree->Branch("CLI", &CLIArgs);

  std::cerr << "[CLI]: " << *CLIArgs << std::endl;

  for (int i = 0; i < nthrows; ++i) {

    std::cerr << "[THW]: Starting throw " << i << " " << BuildLogInfoString();

    // Set up throws for the starting value
    SystShifts fakeThrowSyst;
    osc::IOscCalculatorAdjustable *fakeThrowOsc;

    // First deal with OA parameters
    if (fakeoa_throw || central_throw)
      fakeThrowOsc = ThrownWideOscCalc(hie, oscVars);
    else
      fakeThrowOsc = NuFitOscCalc(hie, 1, asimov_set);

    // Now deal with systematics
    if (fakenuis_throw and not central_throw) {
      for (auto s : systlist) {
        fakeThrowSyst.SetShift(
            s, GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
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
    Fitter::Precision fitStrategy = Fitter::kNormal|Fitter::kIncludeHesse;

    IExperiment *penalty = GetPenalty(hie, 1, penaltyString);

    double thischisq =
        RunFitPoint(stateFname, sampleString, fakeThrowOsc, fakeThrowSyst,
                    stats_throw, oscVars, systlist, fitThrowOsc, fitThrowSyst,
                    oscSeeds, penalty, fitStrategy, nullptr, &pftree);

    pftree.Fill();

    std::cerr << "[THW]: Throw " << i << " found minimum chi2 = " << thischisq
              << " " << BuildLogInfoString();
    // Done with this systematic throw
    delete penalty;

    if (chk.ShouldCheckpoint()) {
      chk.WaitForSemaphore();
      std::cerr << "[OUT]: Writing output file:" << outputFname << std::endl;
      TDirectory *odir = gDirectory;
      TFile fout(outputFname.c_str(), "RECREATE");
      pftree.Write();
      pftree.SetDirectory(nullptr);
      if (odir) {
        odir->cd();
      }
      chk.NotifyCheckpoint();
    }

    if (!chk.IsSafeToStartNewUnit()) {
      std::cerr
          << "[CHK]: Do not have time to finish another fit, exiting early."
          << std::endl;
      break;
    }
  }

  std::cerr << "[OUT]: Writing output file:" << outputFname << std::endl;
  TFile fout(outputFname.c_str(), "RECREATE");
  pftree.Write();

  std::cout << "[INFO]: Done " << BuildLogInfoString();
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
