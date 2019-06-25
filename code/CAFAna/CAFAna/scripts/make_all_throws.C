// One script to bring them all and in the darkness bind them...

#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/Analysis/CheckPointHelper.h"

using namespace ana;

char const *def_stateFname = "common_state_mcc11v3.root";
char const *def_outputFname = "sens_ndfd_nosyst.root";
int const def_nthrows = 100;
char const *def_systSet = "nosyst";
char const *def_sampleString = "ndfd";
char const *def_throwString = "stat:fake:start";
char const *def_penaltyString = "nopen";
int const def_hie = 1;

void make_all_throws(std::string stateFname = def_stateFname,
                     std::string outputFname = def_outputFname,
                     int nthrows = def_nthrows,
                     std::string systSet = def_systSet,
                     std::string sampleString = def_sampleString,
                     std::string throwString = def_throwString,
                     std::string penaltyString = def_penaltyString,
                     int hie = def_hie) {

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

  // The global tree for all throw types
  FitTreeBlob global_tree("global_fit_info", "global_params");
  double globalmin;
  double thisdcp;
  double this_th23;
  double this_ssth23;

  // std::stringstream CLI_ss("");
  // CLI_ss << stateFname << " " << outputFname << " " << nthrows << " " <<
  // systSet
  //        << " " << sampleString << " " << throwString << " " << penaltyString
  //        << " " << hie;
  //
  // std::string *CLIArgs = nullptr;
  // global_tree.meta_tree->Branch("CLI", &CLIArgs);
  // (*CLIArgs) = CLI_ss.str();
  // std::cerr << "[CLI]: " << *CLIArgs << std::endl;

  // Fit in the correct hierarchy and octant for global throw
  std::vector<const IFitVar *> oscVarsGlobal = GetOscVars("alloscvars", hie);

  std::cerr << "[FIT]: Global Osc Parameters: \n\t";
  for (auto s : oscVarsGlobal) {
    std::cerr << s->ShortName() << " ";
  }
  std::cerr << std::endl;

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

  std::map<const IFitVar *, std::vector<double>> oscSeedsGlobal;
  oscSeedsGlobal[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};
  oscSeedsGlobal[&kFitSinSqTheta23] = {0.4, 0.6};

  // MH specific
  FitTreeBlob mh_tree("mh_fit_info", "mh_params");
  double mh_chisqmin;
  double mh_dchi2;
  double mh_significance = 0;
  mh_tree.throw_tree->Branch("chisqmin", &mh_chisqmin);
  mh_tree.throw_tree->Branch("globalmin", &globalmin);
  mh_tree.throw_tree->Branch("hie", &hie);
  mh_tree.throw_tree->Branch("dcp", &thisdcp);
  mh_tree.throw_tree->Branch("dchi2", &mh_dchi2);
  mh_tree.throw_tree->Branch("significance", &mh_significance);
  // mh_tree.meta_tree->Branch("CLI", &CLIArgs);

  // Fit in the incorrect hierarchy for the exclusion
  std::vector<const IFitVar *> oscVarsMH = GetOscVars("alloscvars", -1 * hie);

  // CPV specific
  FitTreeBlob cpv_tree("cpv_fit_info", "cpv_params");
  double cpv_chisqmin;
  double cpv_dchi2;
  double cpv_significance = 0;
  // Add the variables of interest to the tree
  cpv_tree.throw_tree->Branch("chisqmin", &cpv_chisqmin);
  cpv_tree.throw_tree->Branch("globalmin", &globalmin);
  cpv_tree.throw_tree->Branch("hie", &hie);
  cpv_tree.throw_tree->Branch("dcp", &thisdcp);
  cpv_tree.throw_tree->Branch("dchi2", &cpv_dchi2);
  cpv_tree.throw_tree->Branch("significance", &cpv_significance);
  // cpv_tree.meta_tree->Branch("CLI", &CLIArgs);

  // CP-conserving specific (no dCP)
  std::vector<const IFitVar *> oscVarsCPV =
      GetOscVars("dmsq32:th23:th13:th12:dmsq21:rho", hie);

  std::map<const IFitVar *, std::vector<double>> oscSeedsCPV = {};
  oscSeedsCPV[&kFitSinSqTheta23] = {.4, .6}; // try both octants

  // Octant specific
  FitTreeBlob oct_tree("oct_fit_info", "oct_params");
  double oct_chisqmin;
  double oct_dchi2;
  double oct_significance = 0;
  oct_tree.throw_tree->Branch("chisqmin", &oct_chisqmin);
  oct_tree.throw_tree->Branch("globalmin", &globalmin);
  oct_tree.throw_tree->Branch("hie", &hie);
  oct_tree.throw_tree->Branch("ssth23", &this_ssth23);
  oct_tree.throw_tree->Branch("th23", &this_th23);
  oct_tree.throw_tree->Branch("dchi2", &oct_dchi2);
  oct_tree.throw_tree->Branch("significance", &oct_significance);
  // oct_tree.meta_tree->Branch("CLI", &CLIArgs);

  std::map<const IFitVar *, std::vector<double>> oscSeedsOct;
  oscSeedsOct[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};

  for (int i = 0; i < nthrows; ++i) {

    std::cerr << "[THW]: Starting throw " << i << " " << BuildLogInfoString();
    // Set up throws for the starting value
    SystShifts fakeThrowSyst;

    // Always throw OA parameters now!
    osc::IOscCalculatorAdjustable *fakeThrowOsc =
        ThrownWideOscCalc(hie, oscVarsGlobal);

    thisdcp = fakeThrowOsc->GetdCP() / TMath::Pi();
    this_th23 = fakeThrowOsc->GetTh23();
    this_ssth23 = sin(this_th23) * sin(this_th23);
    int oct = (this_ssth23 > 0.5) ? 1 : -1;

    // This is very very dumb, but unavoidable given how the parameters work...
    std::vector<const IFitVar *> oscVarsOct =
        GetOscVars("alloscvars", hie, -1 * oct);

    // Now deal with systematics
    if (fakenuis_throw) {
      for (auto s : systlist)
        fakeThrowSyst.SetShift(
            s, GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
    } else
      fakeThrowSyst = kNoShift;

    // Prefit throws etc
    SystShifts fitThrowSyst;
    osc::IOscCalculatorAdjustable *fitThrowOsc;
    if (start_throw) {
      for (auto s : systlist)
        fitThrowSyst.SetShift(
            s, GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
      fitThrowOsc = ThrownWideOscCalc(hie, oscVarsGlobal);
    } else {
      fitThrowSyst = kNoShift;
      fitThrowOsc = NuFitOscCalc(hie, 1);
    }

    // Somebody stop him, the absolute madman!
    // Keep the same stats/syst/OA throw for all fits
    std::vector<std::unique_ptr<Spectrum>> mad_spectra_yo = {};

    auto fit_type = Fitter::kNormal;

    // -------------------------------------
    // --------- Do the global fit ---------
    // -------------------------------------

    // This actually doesn't matter unless we apply a theta23 constraint, which
    // I think we shouldn't anyway...
    IExperiment *gpenalty = GetPenalty(hie, 1, penaltyString);

    globalmin =
        RunFitPoint(stateFname, sampleString, fakeThrowOsc, fakeThrowSyst,
                    stats_throw, oscVarsGlobal, systlist, fitThrowOsc,
                    SystShifts(fitThrowSyst), oscSeedsGlobal, gpenalty,
                    fit_type, nullptr, &global_tree, &mad_spectra_yo);
    global_tree.Fill();
    delete gpenalty;
    delete fitThrowOsc;

    std::cerr << "[THW]: Global throw " << i
              << " fit found minimum chi2 = " << globalmin << " "
              << BuildLogInfoString();

    // -------------------------------------
    // --------- Now do CPV fits -----------
    // -------------------------------------
    {
      // Now fit several times to find the best fit when dCP = 0, pi
      double cpv_chisqmin = 99999;
      double cpv_thischisq;

      for (int tdcp = 0; tdcp < 2; ++tdcp) {
        double dcptest = tdcp * TMath::Pi();

        // Now testOsc is restricted to CP conservation
        osc::IOscCalculatorAdjustable *testOscCPV = NuFitOscCalc(hie, 1);
        testOscCPV->SetdCP(dcptest);

        IExperiment *cpv_penalty = GetPenalty(hie, 1, penaltyString);

        cpv_thischisq =
            RunFitPoint(stateFname, sampleString, fakeThrowOsc, fakeThrowSyst,
                        stats_throw, oscVarsCPV, systlist, testOscCPV,
                        SystShifts(fitThrowSyst), oscSeedsCPV, cpv_penalty,
                        fit_type, nullptr, &cpv_tree, &mad_spectra_yo);

        cpv_chisqmin = TMath::Min(cpv_thischisq, cpv_chisqmin);
        delete cpv_penalty;
        delete testOscCPV;
      }

      std::cerr << "[THW]: CPV throw " << i
                << " fit found minimum chi2 = " << cpv_chisqmin << " "
                << BuildLogInfoString();

      cpv_dchi2 = cpv_chisqmin - globalmin;
      if (cpv_dchi2 > 0) {
        cpv_significance = sqrt(cpv_dchi2);
      } else if (cpv_dchi2 < -1E-4) {
        std::cerr << "[WARN]: CPV fit dchi2 of " << cpv_dchi2 << "; "
                  << cpv_chisqmin << " - " << globalmin << std::endl;
      }

      cpv_tree.Fill();
    }

    // -------------------------------------
    // --------- Now octant fits -----------
    // -------------------------------------
    osc::IOscCalculatorAdjustable *testOscOct = NuFitOscCalc(hie, -1 * oct);

    // No penalty on the octant, so ignore it...
    IExperiment *oct_penalty = GetPenalty(hie, 1, penaltyString);

    double oct_chisqmin = RunFitPoint(
        stateFname, sampleString, fakeThrowOsc, fakeThrowSyst, stats_throw,
        oscVarsOct, systlist, testOscOct, SystShifts(fitThrowSyst), oscSeedsOct,
        oct_penalty, fit_type, nullptr, &oct_tree, &mad_spectra_yo);

    std::cerr << "[THW]: Oct. throw " << i
              << " fit found minimum chi2 = " << oct_chisqmin << " "
              << BuildLogInfoString();

    oct_dchi2 = oct_chisqmin - globalmin;
    if (oct_dchi2 > 0) {
      oct_significance = sqrt(oct_dchi2);
    } else if (oct_dchi2 < -1E-4) {
      std::cerr << "[WARN]: Octant fit dchi2 of " << oct_dchi2 << "; "
                << oct_chisqmin << " - " << globalmin << std::endl;
    }

    oct_tree.Fill();
    delete oct_penalty;
    delete testOscOct;

    // -------------------------------------
    // --------- Now the MH fits -----------
    // -------------------------------------

    // Force the testOsc to be in the wrong hierarchy
    osc::IOscCalculatorAdjustable *testOscMH = NuFitOscCalc(-1 * hie, 1);

    // Wrong hierarchy remember
    IExperiment *mh_penalty = GetPenalty(-1 * hie, 1, penaltyString);

    double mh_chisqmin = RunFitPoint(
        stateFname, sampleString, fakeThrowOsc, fakeThrowSyst, stats_throw,
        oscVarsMH, systlist, testOscMH, SystShifts(fitThrowSyst),
        oscSeedsGlobal, mh_penalty, fit_type, // same seeds as a global fit
        nullptr, &mh_tree, &mad_spectra_yo);

    std::cerr << "[THW]: MH. throw " << i
              << " fit found minimum chi2 = " << mh_chisqmin << " "
              << BuildLogInfoString();

    mh_dchi2 = mh_chisqmin - globalmin;
    if (mh_dchi2 > 0) {
      mh_significance = sqrt(mh_dchi2);
    } else if (mh_dchi2 < -1E-4) {
      std::cerr << "[WARN]: MH fit dchi2 of " << mh_dchi2 << "; " << mh_chisqmin
                << " - " << globalmin << std::endl;
    }

    mh_tree.Fill();

    delete mh_penalty;
    delete testOscMH;

    delete fakeThrowOsc;

    if (chk.ShouldCheckpoint()) {
      chk.WaitForSemaphore();
      std::cerr << "[OUT]: Writing output file:" << outputFname << std::endl;
      TDirectory *odir = gDirectory;
      TFile fout(outputFname.c_str(), "RECREATE");
      global_tree.SetDirectoryClone(&fout);
      mh_tree.SetDirectoryClone(&fout);
      cpv_tree.SetDirectoryClone(&fout);
      oct_tree.SetDirectoryClone(&fout);
      fout.Write();
      fout.Close();
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
  global_tree.SetDirectoryClone(&fout);
  mh_tree.SetDirectoryClone(&fout);
  cpv_tree.SetDirectoryClone(&fout);
  oct_tree.SetDirectoryClone(&fout);
  fout.Write();
  fout.Close();

  std::cout << "[INFO]: Done " << BuildLogInfoString();
}

#ifndef __CINT__
#ifndef NO_MAT_MAIN
void Usage(char const *argv[]) {
  std::cout << "[USAGE]: " << argv[0]
            << " [/path/to/state_stub] [output_filename.root] [NThrows] "
               "[SystSet: e.g. noxsec:nodet] [Sample: e.g. ndfd] [Throw: e.g. "
               "stat:fake:start] [Penalties: e.g. nopen] [Hierarchy: e.g. 1]"
            << std::endl;
}

int main(int argc, char const *argv[]) {

  gROOT->SetMustClean(false);

  if ((argc >= 2) &&
      ((std::string("-?") == argv[1]) || (std::string("--help") == argv[1]) ||
       (std::string("-h") == argv[1]))) {
    Usage(argv);
    return 0;
  }

  std::string stateFname = (argc > 1) ? argv[1] : def_stateFname;
  std::string outputFname = (argc > 2) ? argv[2] : def_outputFname;
  int nthrows = (argc > 3) ? atoi(argv[3]) : def_nthrows;
  std::string systSet = (argc > 4) ? argv[4] : def_systSet;
  std::string sampleString = (argc > 5) ? argv[5] : def_sampleString;
  std::string throwString = (argc > 6) ? argv[6] : def_throwString;
  std::string penaltyString = (argc > 7) ? argv[7] : def_penaltyString;
  int hie = (argc > 8) ? atoi(argv[8]) : def_hie;

  make_all_throws(stateFname, outputFname, nthrows, systSet, sampleString,
                  throwString, penaltyString, hie);
}
#endif
#endif
