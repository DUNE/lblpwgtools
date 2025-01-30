// One script to bring them all and in the darkness bind them...

#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/Analysis/CheckPointHelper.h"

#include "TFormula.h"

using namespace ana;

// This is actually a copy of ParseAsimovSet...
std::vector<std::pair<std::string, double> > ParseFixPar(std::string noApologies){
  // Bad things happen now
  std::vector<std::pair<std::string, double> > ret;
  std::vector<std::string> output;
  std::stringstream ss(noApologies);
  std::string token;

  while (std::getline(ss, token, ',')) output.push_back(token);

  for (auto & pair : output){
    std::stringstream sss(pair);
    std::getline(sss, token, ':');
    std::string name = token;
    std::getline(sss, token, ':');
    TFormula thisForm("", token.c_str());
    double value = thisForm.Eval(0);
    ret.push_back(std::make_pair(name, value));
  }
  return ret;
}

// Function to decide which oscillation parameters to throw

char const *def_stateFname = "common_state_mcc11v3.root";
char const *def_outputFname = "sens_ndfd_nosyst.root";
int const def_nthrows = 100;
char const *def_systSet = "nosyst";
char const *def_fixVar = "deltapi:0";
char const *def_sampleString = "ndfd";
char const *def_throwString = "stat:fake:start";
char const *def_penaltyString = "nopen";
int const def_hie = 1;
char const *def_fakeDataShift = "";

void make_FC_throws(std::string stateFname = def_stateFname,
		    std::string outputFname = def_outputFname,
		    int nthrows = def_nthrows,
		    std::string systSet = def_systSet,
		    std::string fixVar = def_fixVar,
		    std::string sampleString = def_sampleString,
		    std::string throwString = def_throwString,
		    std::string penaltyString = def_penaltyString,
		    int hie = def_hie,
		    std::string fakeDataShift = def_fakeDataShift) {

  gROOT->SetBatch(1);

  if (gRNGSeed == 0) { // if we have a time based seed its still useful for
                       // things to be reproducible, so use the time to seed a
                       // new RNG seed and spin the wheels for 100k throws;
    gRandom->SetSeed(0);
    for (size_t i = 0; i < 1E5; ++i) {
      gRNGSeed = gRandom->Integer(std::numeric_limits<unsigned>::max());
    }
  }

  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");

  std::cerr << "[RNG]: gRNGSeed = " << gRNGSeed << std::endl;

  gRandom->SetSeed(gRNGSeed);

  CheckPointHelper chk;
  if (chk.IsCounting()) {
    nthrows = std::numeric_limits<int>::max();
  }

  // Decide what is to be thrown
  bool stats_throw, fakeoa_throw, fakenuis_throw, fakenuisoa_throw, start_throw, central_throw;
  ParseThrowInstructions(throwString, stats_throw, fakeoa_throw, fakenuis_throw,
                         start_throw, central_throw, fakenuisoa_throw);

  auto fixVarVect = ParseFixPar(fixVar);

  // Get the systematics to use
  std::vector<const ISyst *> systlist = GetListOfSysts(systSet);

  // Deal with fake data shifts now, need to copy them into each throw later
  SystShifts fakeDataSyst = GetFakeDataSystShift(fakeDataShift);

  // Remove any systs which have been set as fake data from the systlist
  std::vector<std::string> bias_syst_names;
  // Loop over all systs set to a non-nominal value and remove                                                                                                               
  for (auto syst : fakeDataSyst.ActiveSysts()){
    std::cout << "Removing " << syst->ShortName() <<std::endl;
    bias_syst_names.push_back(syst->ShortName());
  }
  RemoveSysts(systlist, bias_syst_names);

  // The tree for the ND only fit
  FitTreeBlob nd_tree("nd_fit_info", "nd_params");

  // The global tree for all throw types
  FitTreeBlob global_tree("global_fit_info", "global_params");
  double globalmin;
  double this_dcp;

  std::stringstream CLI_ss("");
  CLI_ss << stateFname << " " << outputFname << " " << nthrows << " " << systSet
         << " " << sampleString << " " << throwString << " " << penaltyString
         << " " << hie;

  std::string *CLIArgs = nullptr;
  global_tree.meta_tree->Branch("CLI", &CLIArgs);
  (*CLIArgs) = CLI_ss.str();
  std::cerr << "[CLI]: " << *CLIArgs << std::endl;
  unsigned LoopTime_s;
  global_tree.throw_tree->Branch("LoopTime_s", &LoopTime_s);

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
  oscSeedsGlobal[&kFitDeltaInPiUnits] = {-0.66, 0, 0.66};
  oscSeedsGlobal[&kFitSinSqTheta23] = {0.45, 0.55};

  // Fixed par throw
  FitTreeBlob fix_tree("fix_fit_info", "fix_params");
  double fix_chisqmin;
  double fix_dchi2;
  fix_tree.throw_tree->Branch("chisqmin", &fix_chisqmin);
  fix_tree.throw_tree->Branch("globalmin", &globalmin);
  fix_tree.throw_tree->Branch("hie", &hie);
  fix_tree.throw_tree->Branch("dcp", &this_dcp);
  fix_tree.throw_tree->Branch("dchi2", &fix_dchi2);
  fix_tree.meta_tree->Branch("CLI", &CLIArgs);

  // Exclude whatever parameters need to be excluded...
  std::vector<const IFitVar *> oscVarsFIX = GetOscVars("alloscvars", hie);
  for (auto par : fixVarVect){
    RemovePars(oscVarsFIX, {par.first});
  }

  // Only add throws if the throw par not included
  std::map<const IFitVar *, std::vector<double>> oscSeedsFIX = {};

  bool hasdcp = false;
  bool hasthe = false;
  for (auto pair : fixVarVect){
    if (pair.first == "deltapi") hasdcp = true;
    if (pair.first == "ssth23") hasthe = true;
  }
  if (!hasdcp) oscSeedsFIX[&kFitDeltaInPiUnits] = {-0.66, 0, 0.66};
  if (!hasthe) oscSeedsFIX[&kFitSinSqTheta23] = {0.45, 0.55};

  global_tree.SetDirectory(fout);
  fix_tree.SetDirectory(fout);
  nd_tree.SetDirectory(fout);

  global_tree.fJobRNGSeed = gRNGSeed;
  fix_tree.fJobRNGSeed = gRNGSeed;
  nd_tree.fJobRNGSeed = gRNGSeed;

  auto lap = std::chrono::system_clock::now();
  for (int i = 0; i < nthrows; ++i) {

    unsigned loop_seed = gRandom->Integer(std::numeric_limits<unsigned>::max());
    global_tree.fLoopRNGSeed = loop_seed;
    fix_tree.fLoopRNGSeed = loop_seed;
    nd_tree.fLoopRNGSeed = loop_seed;
    gRandom->SetSeed(loop_seed);

    auto start_loop = std::chrono::system_clock::now();
    if (!i) {
      LoopTime_s = 0;
    } else {
      LoopTime_s =
          std::chrono::duration_cast<std::chrono::seconds>(start_loop - lap)
              .count();
      lap = start_loop;
    }

    std::cerr << "[THW]: Starting throw " << i << " " << BuildLogInfoString();
    // Set up throws for the starting value
    SystShifts fakeThrowSyst;

    // Always throw OA parameters now!
    osc::IOscCalculatorAdjustable *fakeThrowOsc =
        ThrownWideOscCalc(hie, oscVarsGlobal);

    // Set the oscillation pars to be fixed to the true value
    for (auto par : fixVarVect){
      SetOscillationParameter(fakeThrowOsc, par.first, par.second, hie);
    }
    this_dcp = fakeThrowOsc->GetdCP() / TMath::Pi();
    
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

    // Add in the fake data shift, no matter what else was selected
    for (auto syst : fakeDataSyst.ActiveSysts()){
      double shift = fakeDataSyst.GetShift(syst);
      fitThrowSyst.SetShift(syst, shift);
    }

    // Somebody stop him, the absolute madman!
    // Keep the same stats/syst/OA throw for all fits
    std::vector<seeded_spectra> mad_spectra_yo = {};

    Fitter::Precision fit_type = Fitter::kNormal;
    if (getenv("CAFANA_FIT_PRECISION")) {
      switch (atoi(getenv("CAFANA_FIT_PRECISION"))) {
      case 0:
        fit_type = Fitter::kFast;
        break;
      case 1:
        fit_type = Fitter::kNormal;
        break;
      case 2:
        fit_type = Fitter::kCareful;
        break;
      }
    }
    if (getenv("CAFANA_FIT_FORCE_HESSE") &&
        bool(atoi(getenv("CAFANA_FIT_FORCE_HESSE")))) {
      fit_type = fit_type | Fitter::kIncludeHesse;
    }

    // -------------------------------------
    // --------- Do the global fit ---------
    // -------------------------------------

    // This actually doesn't matter unless we apply a theta23 constraint, which
    // I think we shouldn't anyway...
    IExperiment *gpenalty = GetPenalty(hie, 1, penaltyString);

    // If the fit used the ND, run a fit with just the ND first, this the output 
    // parameter values are then used as the starting point for all subsequent seeds
    if (sampleString.find("nd") != std::string::npos){
      SystShifts nd_fit_systs;
      // Hackity hack with the sample name here...
      double nd_min = RunFitPoint(stateFname, sampleString+":ndprefit", fakeThrowOsc, fakeThrowSyst,
				  stats_throw, {}, systlist, fitThrowOsc,
				  SystShifts(fitThrowSyst), {}, nullptr,
				  fit_type, nullptr, &nd_tree, &mad_spectra_yo, nd_fit_systs);
      // The best fit should be used as the input for the next fits!
      fitThrowSyst = nd_fit_systs;
      nd_tree.Fill();
      
      std::cerr << "[THW]: ND throw " << i
		<< " fit found minimum chi2 = " << nd_min << " "
		<< BuildLogInfoString();
    }

    globalmin =
        RunFitPoint(stateFname, sampleString, fakeThrowOsc, fakeThrowSyst,
                    stats_throw, oscVarsGlobal, systlist, fitThrowOsc,
                    SystShifts(fitThrowSyst), oscSeedsGlobal, gpenalty,
                    fit_type, nullptr, &global_tree, &mad_spectra_yo);

    delete gpenalty;
    delete fitThrowOsc;
    global_tree.Fill();

    std::cerr << "[THW]: Global throw " << i
              << " fit found minimum chi2 = " << globalmin << " "
              << BuildLogInfoString();

    if (chk.ShouldCheckpoint()) {
      chk.WaitForSemaphore();
      std::cerr << "[OUT]: Writing output file:" << outputFname << std::endl;
      TDirectory *odir = gDirectory;
      fout->Write();
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

    // -------------------------------------
    // --------- Now do fix fits -----------
    // -------------------------------------

    // Set the fixed parameters to the correct value
    osc::IOscCalculatorAdjustable *testOscFIX = NuFitOscCalc(hie, 1);
    for (auto par : fixVarVect){
      SetOscillationParameter(testOscFIX, par.first, par.second, hie);
    }

    IExperiment *fix_penalty = GetPenalty(hie, 1, penaltyString);

    double fix_chisqmin =
      RunFitPoint(stateFname, sampleString, fakeThrowOsc, fakeThrowSyst,
		  stats_throw, oscVarsFIX, systlist, testOscFIX,
		  SystShifts(fitThrowSyst), oscSeedsFIX, fix_penalty,
		  fit_type, nullptr, &fix_tree, &mad_spectra_yo);
    
    delete fix_penalty;
    delete testOscFIX;

    std::cerr << "[THW]: FIX throw " << i
	      << " fit found minimum chi2 = " << fix_chisqmin << " "
	      << BuildLogInfoString();
    
    fix_dchi2 = fix_chisqmin - globalmin;
    fix_tree.Fill();
    
    if (chk.ShouldCheckpoint()) {
      chk.WaitForSemaphore();
      std::cerr << "[OUT]: Writing output file:" << outputFname << std::endl;
      TDirectory *odir = gDirectory;
      fout->Write();
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
  fout->Write();
  fout->Close();
  std::cout << "[INFO]: Done " << BuildLogInfoString();
}
    
#ifndef __CINT__
#ifndef NO_MAT_MAIN
void Usage(char const *argv[]) {
  std::cout << "[USAGE]: " << argv[0]
            << " [/path/to/state_stub] [output_filename.root] [NThrows] "
               "[SystSet: e.g. noxsec:nodet] "
               "[FixPar: e.g. deltapi:0] "
               "[Sample: e.g. ndfd] [Throw: e.g. "
               "stat:fake:start] [Penalties: e.g. nopen] [Hierarchy: e.g. 1] "
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
  std::string fixVar = (argc > 5) ? argv[5] : def_fixVar;
  std::string sampleString = (argc > 6) ? argv[6] : def_sampleString;
  std::string throwString = (argc > 7) ? argv[7] : def_throwString;
  std::string penaltyString = (argc > 8) ? argv[8] : def_penaltyString;
  int hie = (argc > 9) ? atoi(argv[9]) : def_hie;
  std::string fakeDataShift = (argc > 10) ? argv[10] : def_fakeDataShift;

  make_FC_throws(stateFname, outputFname, nthrows, systSet, fixVar, sampleString,
		 throwString, penaltyString, hie, fakeDataShift);
}
#endif
#endif
