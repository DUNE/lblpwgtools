#include "CAFAna/Analysis/common_fit_definitions.h"

#include <omp.h>

using namespace ana;

int main(int argc, char const *argv[]) {

  std::string stateFname = argv[1];
  int const nthrows = 2;
  std::string systSet = "nodet:noxsec:nflux=10";
  std::string sampleString = "ndfd";
  std::string throwString = "stat:fake:start";
  std::string penaltyString = "nopen";
  int const hie = 1;
  std::string asimov_set = "0";
  std::string oscVarString = "alloscvars";

  gROOT->SetBatch(1);
  gROOT->SetMustClean(false);

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

  FitTreeBlob tree_1;
  FitTreeBlob tree_1_rp;
  FitTreeBlob tree_4;

  for (int i = 0; i < nthrows; ++i) {
    std::cerr << "[THW]: Starting throw " << i << " " << BuildLogInfoString();

    // Set up throws for the starting value
    SystShifts fakeThrowSyst;
    osc::IOscCalculatorAdjustable *fakeThrowOsc;

    // First deal with OA parameters
    if (fakeoa_throw || central_throw) {
      fakeThrowOsc = ThrownWideOscCalc(hie, oscVars);
    } else {
      fakeThrowOsc = NuFitOscCalc(hie, 1, asimov_set);
    }

    // Now deal with systematics
    if (fakenuis_throw and not central_throw) {
      for (auto s : systlist) {
        fakeThrowSyst.SetShift(
            s, GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
      }
    } else {
      fakeThrowSyst = kNoShift;
    }

    if (central_throw) {
      for (auto s : systlist)
        s->SetCentral(GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
    }

    // Prefit
    SystShifts fitThrowSyst;
    osc::IOscCalculatorAdjustable *fitThrowOsc_1, *fitThrowOsc_1_rp,
        *fitThrowOsc_4;
    if (start_throw) {
      for (auto s : systlist)
        fitThrowSyst.SetShift(
            s, GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
      fitThrowOsc_1 = ThrownWideOscCalc(hie, oscVars);
    } else {
      fitThrowSyst = kNoShift;
      fitThrowOsc_1 = NuFitOscCalc(hie, 1, asimov_set);
    }
    fitThrowOsc_1_rp = fitThrowOsc_1->Copy();
    fitThrowOsc_4 = fitThrowOsc_1->Copy();

    MinuitFitter::FitOpts fit_type = MinuitFitter::kNormal;
    if (getenv("CAFANA_FIT_PRECISION")) {
      switch (atoi(getenv("CAFANA_FIT_PRECISION"))) {
      case 0:
        fit_type = MinuitFitter::kFast;
        break;
      case 1:
        fit_type = MinuitFitter::kNormal;
        break;
      case 2:
        fit_type = MinuitFitter::kCareful;
        break;
      }
    }
    if (getenv("CAFANA_FIT_FORCE_HESSE") &&
        bool(atoi(getenv("CAFANA_FIT_FORCE_HESSE")))) {
      fit_type = fit_type | MinuitFitter::kIncludeHesse;
    }

    IExperiment *penalty = GetPenalty(hie, 1, penaltyString);

    auto start_1 = std::chrono::system_clock::now();
    omp_set_num_threads(1);

    // Make sure we run the same fit each time
    std::vector<seeded_spectra> mad_spectra_yo = {};

    double thischisq = RunFitPoint(
        stateFname, sampleString, fakeThrowOsc, fakeThrowSyst, stats_throw,
        oscVars, systlist, fitThrowOsc_1, SystShifts(fitThrowSyst), oscSeeds,
        penalty, fit_type, nullptr, &tree_1, &mad_spectra_yo);
    auto end_1 = std::chrono::system_clock::now();

    std::cerr << "[THW]: Throw " << i << " found minimum chi2 = " << thischisq
              << " with 1 threads in"
              << std::chrono::duration_cast<std::chrono::seconds>(end_1 -
                                                                  start_1)
                     .count()
              << " s. " << BuildLogInfoString();

    thischisq = RunFitPoint(
        stateFname, sampleString, fakeThrowOsc, fakeThrowSyst, stats_throw,
        oscVars, systlist, fitThrowOsc_1_rp, SystShifts(fitThrowSyst), oscSeeds,
        penalty, fit_type, nullptr, &tree_1_rp, &mad_spectra_yo);

    for (size_t p_it = 0; p_it < tree_4.fPostFitValues->size(); ++p_it) {
      if ((fabs(tree_1.fPostFitValues->at(p_it) -
                tree_1_rp.fPostFitValues->at(p_it)) > 1E-8) ||
          (fabs(tree_1.fPostFitErrors->at(p_it) -
                tree_1_rp.fPostFitErrors->at(p_it)) > 1E-8)) {
        std::cout << "[WARN]: Fit found differing values for "
                  << tree_1_rp.fParamNames->at(p_it)
                  << ", 1 thread = " << tree_1.fPostFitValues->at(p_it)
                  << " +/- " << tree_1.fPostFitErrors->at(p_it)
                  << ", 1 thread (retry) = "
                  << tree_1_rp.fPostFitValues->at(p_it) << " +/- "
                  << tree_1_rp.fPostFitErrors->at(p_it) << std::endl;
      }
    }

    auto start_4 = std::chrono::system_clock::now();
    omp_set_num_threads(4);

    thischisq = RunFitPoint(
        stateFname, sampleString, fakeThrowOsc, fakeThrowSyst, stats_throw,
        oscVars, systlist, fitThrowOsc_4, SystShifts(fitThrowSyst), oscSeeds,
        penalty, fit_type, nullptr, &tree_4, &mad_spectra_yo);
    auto end_4 = std::chrono::system_clock::now();

    std::cerr << "[THW]: Throw " << i << " found minimum chi2 = " << thischisq
              << " with 4 threads in "
              << std::chrono::duration_cast<std::chrono::seconds>(end_4 -
                                                                  start_4)
                     .count()
              << " s, " << BuildLogInfoString();

    for (size_t p_it = 0; p_it < tree_4.fPostFitValues->size(); ++p_it) {
      if ((fabs(tree_1.fPostFitValues->at(p_it) -
                tree_4.fPostFitValues->at(p_it)) > 1E-8) ||
          (fabs(tree_1.fPostFitErrors->at(p_it) -
                tree_4.fPostFitErrors->at(p_it)) > 1E-8)) {
        std::cout << "[WARN]: Fit found differing values for "
                  << tree_4.fParamNames->at(p_it)
                  << ", 1 thread = " << tree_1.fPostFitValues->at(p_it)
                  << " +/- " << tree_1.fPostFitErrors->at(p_it)
                  << ", 4 threads = " << tree_4.fPostFitValues->at(p_it)
                  << " +/- " << tree_4.fPostFitErrors->at(p_it) << std::endl;
      }
    }

    // Done with this systematic throw
    delete penalty;
    delete fitThrowOsc_1;
    delete fitThrowOsc_1_rp;
    delete fitThrowOsc_4;
  }
}
