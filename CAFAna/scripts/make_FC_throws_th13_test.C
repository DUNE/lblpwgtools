#include "CAFAna/Analysis/common_fit_definitions.h"
#include "OscLib/func/OscCalculatorPMNSOpt.h"
#include "CAFAna/Analysis/CheckPointHelper.h"

#include "TFormula.h"

using namespace ana;

// This throws all osc parameters, but fixes any to the "asimov" value given
osc::IOscCalculatorAdjustable* ThrownWideOscCalcWithAsimov(int hie, std::vector<const IFitVar*> oscVars, std::string asimov_set){

  // start by throwing everything
  osc::IOscCalculatorAdjustable *ret =
    ThrownWideOscCalc(hie, oscVars);

  // Need to set th13 to the value the penalty pulls to...
  if (hie>0) ret->SetTh13(kNuFitTh13CVNH);
  else ret->SetTh13(kNuFitTh13CVIH);

  // Nope out if the usual default value is given
  if (asimov_set.find("asimov0") != std::string::npos) return ret;

  // Bad things happen now
  std::vector<std::string> output;
  std::stringstream ss(asimov_set);
  std::string token;
  while (std::getline(ss, token, ',')) output.push_back(token);

  for (auto & pair : output){
    std::stringstream sss(pair);
    std::getline(sss, token, ':');
    std::string name = token;
    std::getline(sss, token, ':');
    TFormula thisForm("", token.c_str());
    double value = thisForm.Eval(0);

    if (name == "th13")
      ret->SetTh13(value);
    if (name == "deltapi")
      ret->SetdCP(value*TMath::Pi());
    if (name == "th23")
      ret->SetTh23(value);
    if (name == "dmsq32")
      ret->SetDmsq32(value/1e3);
    if (name == "ssth23")
      ret->SetTh23(asin(sqrt(value)));
  }
  return ret;
}

// Copy a calculator for playing with
osc::IOscCalculatorAdjustable* SmartCalc(osc::IOscCalculatorAdjustable *incalc, int oct, double dcp_shift=0){

  osc::IOscCalculatorAdjustable* ret = new osc::OscCalculatorPMNSOpt;
  ret->SetL(incalc->GetL());
  ret->SetRho(incalc->GetRho());
  ret->SetDmsq21(incalc->GetDmsq21());
  ret->SetTh12(incalc->GetTh12());
  ret->SetTh23(incalc->GetTh23());
  ret->SetDmsq32(incalc->GetDmsq32());
  ret->SetTh13(incalc->GetTh13());

  // If the true value is in the wrong octant, try the reflected value...                                                                                                                                    
  double dth23 = incalc->GetTh23() - TMath::Pi()/4;
  if (oct < 0 && dth23 > 0) {
    ret ->SetTh23(TMath::Pi()/4 - abs(dth23));
  }
  if (oct > 0 && dth23 < 0) {
    ret ->SetTh23(TMath::Pi()/4 + abs(dth23));
  }

  // Now try adding values to dcp and forcing to be between +/-1
  ret ->SetdCP( fmod(incalc->GetdCP()+dcp_shift+1,2) -1);

  return ret;
}

// Function to decide which oscillation parameters to throw
char const *def_stateFname = "common_state_mcc11v3.root";
char const *def_outputFname = "sens_ndfd_nosyst.root";
int const def_nthrows = 100;
char const *def_systSet = "nosyst";
char const *def_sampleString = "ndfd";
char const *def_throwString = "stat:fake:start";
int const def_hie = 1;
char const *def_asimov_set = "0";
char const *def_fakeDataShift = "nominal";
char const *def_oscParString = "alloscpars";

void make_FC_throws_th13_test(std::string stateFname = def_stateFname,
				std::string outputFname = def_outputFname,
				int nthrows = def_nthrows,
				std::string systSet = def_systSet,
				std::string sampleString = def_sampleString,
				std::string throwString = def_throwString,
				int hie = def_hie,
				std::string asimov_set = def_asimov_set,
			        std::string fakeDataShift = def_fakeDataShift,
			        std::string oscParString = def_oscParString) {

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

  // The nopen tree for all throw types
  FitTreeBlob nopen_tree("nopen_fit_info", "nopen_params");
  double nopen_chisqmin;
  int nopen_oct;
  nopen_tree.throw_tree->Branch("nopen_oct", &nopen_oct);

  std::stringstream CLI_ss("");
  CLI_ss << stateFname << " " << outputFname << " " << nthrows << " " << systSet
         << " " << sampleString << " " << throwString << " " << hie;

  std::string *CLIArgs = nullptr;
  nopen_tree.meta_tree->Branch("CLI", &CLIArgs);
  (*CLIArgs) = CLI_ss.str();
  std::cerr << "[CLI]: " << *CLIArgs << std::endl;
  unsigned LoopTime_s;
  nopen_tree.throw_tree->Branch("LoopTime_s", &LoopTime_s);

  // Fit in the correct hierarchy and octant for nopen throw
  std::vector<const IFitVar *> oscVars = GetOscVars(oscParString, hie);

  std::cerr << "[FIT]: Osc Parameters: \n\t";
  for (auto s : oscVars) {
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

  // Manually try different octants, so don't seed ssth23
  std::map<const IFitVar *, std::vector<double>> oscSeeds = {};

  // Fixed par throw
  FitTreeBlob th13_tree("th13_fit_info", "th13_params");
  double th13_chisqmin;
  int th13_oct;
  double dchisq;
  bool allvalid;

  th13_tree.throw_tree->Branch("th13_chisqmin", &th13_chisqmin);
  th13_tree.throw_tree->Branch("nopen_chisqmin", &nopen_chisqmin);
  th13_tree.throw_tree->Branch("nopen_oct", &nopen_oct);
  th13_tree.throw_tree->Branch("th13_oct", &th13_oct);
  th13_tree.throw_tree->Branch("hie", &hie);
  th13_tree.throw_tree->Branch("dchisq", &dchisq);
  th13_tree.throw_tree->Branch("allValid", &allvalid);
  th13_tree.meta_tree->Branch("CLI", &CLIArgs);

  nopen_tree.SetDirectory(fout);
  th13_tree.SetDirectory(fout);
  nd_tree.SetDirectory(fout);

  nopen_tree.fJobRNGSeed = gRNGSeed;
  th13_tree.fJobRNGSeed = gRNGSeed;
  nd_tree.fJobRNGSeed = gRNGSeed;

  // Add a forest to understand the weird fit results
  FitTreeBlob th13_LO_tree("th13_LO_fit_info", "th13_LO_params");
  FitTreeBlob th13_BOTH_tree("th13_BOTH_fit_info", "th13_BOTH_params");
  FitTreeBlob th13_UO_tree("th13_UO_fit_info", "th13_UO_params");
  FitTreeBlob nopen_LO_tree("nopen_LO_fit_info", "nopen_LO_params");
  FitTreeBlob nopen_BOTH_tree("nopen_BOTH_fit_info", "nopen_BOTH_params");
  FitTreeBlob nopen_UO_tree("nopen_UO_fit_info", "nopen_UO_params");

  th13_LO_tree.SetDirectory(fout);
  th13_BOTH_tree.SetDirectory(fout);
  th13_UO_tree.SetDirectory(fout);
  nopen_LO_tree.SetDirectory(fout);
  nopen_BOTH_tree.SetDirectory(fout);
  nopen_UO_tree.SetDirectory(fout);

  auto lap = std::chrono::system_clock::now();
  for (int i = 0; i < nthrows; ++i) {

    // Default to fits being valid
    allvalid = true;

    unsigned loop_seed = gRandom->Integer(std::numeric_limits<unsigned>::max());
    nopen_tree.fLoopRNGSeed = loop_seed;
    th13_tree.fLoopRNGSeed = loop_seed;
    nd_tree.fLoopRNGSeed = loop_seed;
    
    th13_LO_tree.fLoopRNGSeed = loop_seed;
    th13_BOTH_tree.fLoopRNGSeed = loop_seed;
    th13_UO_tree.fLoopRNGSeed = loop_seed;
    nopen_LO_tree.fLoopRNGSeed = loop_seed;
    nopen_BOTH_tree.fLoopRNGSeed = loop_seed;
    nopen_UO_tree.fLoopRNGSeed = loop_seed;

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
    // Need to add asimov options here...
    osc::IOscCalculatorAdjustable *fakeThrowOsc =
      ThrownWideOscCalcWithAsimov(hie, oscVars, asimov_set);

    // Now deal with systematics
    if (fakenuis_throw) {
      for (auto s : systlist)
        fakeThrowSyst.SetShift(
            s, GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
    } else
      fakeThrowSyst = kNoShift;

    // Prefit throws etc
    SystShifts fitThrowSyst;
    if (start_throw) {
      for (auto s : systlist)
        fitThrowSyst.SetShift(
            s, GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
    } else {
      fitThrowSyst = kNoShift;
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

    std::cout << "Fit throw syst values:" << std::endl;
    for (auto s : fitThrowSyst.ActiveSysts()) {
      std::cout << s->ShortName() << " " << fitThrowSyst.GetShift(s) << "; ";
    }
    std::cout << std::endl;

    // -------------------------------------
    // -------- Do the ND-only fit --------
    // -------------------------------------

    // If the fit used the ND, run a fit with just the ND first, this the output 
    // parameter values are then used as the starting point for all subsequent seeds
    SystShifts nd_fit_systs;
    if (sampleString.find("nd") != std::string::npos){
      // Hackity hack with the sample name here...
      double nd_min = RunFitPoint(stateFname, sampleString+":ndprefit", fakeThrowOsc, fakeThrowSyst,
				  stats_throw, {}, systlist, NuFitOscCalc(hie, 1),
				  SystShifts(fitThrowSyst), {}, nullptr,
				  fit_type, nullptr, &nd_tree, &mad_spectra_yo, nd_fit_systs);

      nd_tree.Fill();
      
      std::cout << "[THW]: ND throw " << i
		<< " fit found minimum chi2 = " << nd_min << " "
		<< BuildLogInfoString();
    } else {
      // If no ND is included, use the prefit throw as the input to all subsequent fits
      nd_fit_systs = SystShifts(fitThrowSyst);
    }

    // Skip this throw and try again if the ND fit fails (why bother)
    if (!nd_tree.fIsValid) continue;

    // -------------------------------------
    // ------ Start with the th13 fit ------
    // -------------------------------------    
    IExperiment *th13_penalty = GetPenalty(hie, 1, "th13");

    // This is temporary to keep track of what is saved
    FitTreeBlob min_th13_blob("min_th13_info", "min_th13_params");
    double this_th13_chisqmin = 1E6;
    double this_BOTH_th13_chisqmin = 1E6;
    double this_UO_th13_chisqmin = 1E6;
    double this_LO_th13_chisqmin = 1E6;

    th13_oct = 0;

    // Loop over ssth23
    for (int oct = -1; oct <= 1; oct++){

      // Get parameters that are limited to specific regions of parameter space
      std::vector<const IFitVar *> tempOscVars = GetOscVars("alloscvars", hie, oct);

      // Explicitly loop over dcp so I can control the starting value
      for (double dcp = -1; dcp < 1; dcp += 0.5){

	std::cout << "Running a th13 fit with oct = " << oct << " and dcp = " << dcp << std::endl;

	osc::IOscCalculatorAdjustable *tempFitThrowOsc = SmartCalc(fakeThrowOsc, oct, dcp);
	
	// Temporary place to save the best fit info
	FitTreeBlob temp_blob("temp_blob", "temp_blob");
	
	// Manually set the seed position for ssth23
	double temp_chisq = RunFitPoint(stateFname, sampleString, fakeThrowOsc, fakeThrowSyst,
					stats_throw, tempOscVars, systlist, tempFitThrowOsc,
					SystShifts(nd_fit_systs), oscSeeds, th13_penalty,
					fit_type, nullptr, &temp_blob, &mad_spectra_yo);

	// We want to know if any fits fail
	if (!temp_blob.fIsValid) allvalid = false;

	// Save all fit info in relevant trees
	if (oct == -1) {
	  if (temp_chisq < this_LO_th13_chisqmin)
	    th13_LO_tree .CopyVals(temp_blob);
	}
	if (oct == 0) {
	  if (temp_chisq < this_BOTH_th13_chisqmin)
	    th13_BOTH_tree.CopyVals(temp_blob);
	}
	if (oct == 1) {
	  if (temp_chisq < this_UO_th13_chisqmin)
	    th13_UO_tree .CopyVals(temp_blob);
	}
	
	// Save if this is a better minimum
	if (temp_chisq < this_th13_chisqmin){
	  this_th13_chisqmin = temp_chisq;
	  th13_oct = oct;
	  min_th13_blob.CopyVals(temp_blob);
	}
	
	delete tempFitThrowOsc;
      }
    }
    delete th13_penalty;

    std::cerr << "[THW]: th13 throw " << i
              << " fit found minimum chi2 = " << th13_chisqmin << " "
              << BuildLogInfoString();

    // -------------------------------------
    // --------- Now the nopen fit ---------
    // -------------------------------------
    IExperiment *gpenalty = GetPenalty(hie, 1, "nopen");

    // This is temporary to keep track of what is saved
    FitTreeBlob min_nopen_blob("min_nopen_info", "min_nopen_params");
    double this_nopen_chisqmin = 1E6;
    double this_BOTH_nopen_chisqmin = 1E6;
    double this_UO_nopen_chisqmin = 1E6;
    double this_LO_nopen_chisqmin = 1E6;
    nopen_oct = 0;

    // Loop over ssth23
    for (int oct = -1; oct <= 1; oct++){

      // Get parameters that are limited to specific regions of parameter space
      std::vector<const IFitVar *> tempOscVars = GetOscVars("alloscvars", hie, oct);

      // Explicitly loop over dcp so I can control the starting value
      for (double dcp = -1; dcp < 1; dcp += 0.5){

	std::cout << "Running a nopen fit with oct = " << oct << " and dcp = " << dcp<< std::endl;

	osc::IOscCalculatorAdjustable *tempFitThrowOsc = SmartCalc(fakeThrowOsc, oct, dcp);
	
	// Temporary place to save the best fit info
	FitTreeBlob temp_blob("temp_blob", "temp_blob");
	
	// Manually set the seed position for ssth23
	double temp_chisq = RunFitPoint(stateFname, sampleString, fakeThrowOsc, fakeThrowSyst,
					stats_throw, tempOscVars, systlist, tempFitThrowOsc,
					SystShifts(nd_fit_systs), oscSeeds, gpenalty,
					fit_type, nullptr, &temp_blob, &mad_spectra_yo);
	
	// We want to know if any fits fail
	if (!temp_blob.fIsValid) allvalid = false;

	// Save all fit info in relevant trees
	if (oct == -1) {
          if (temp_chisq < this_LO_nopen_chisqmin)
            nopen_LO_tree .CopyVals(temp_blob);
        }
        if (oct == 0) {
          if (temp_chisq < this_BOTH_nopen_chisqmin)
            nopen_BOTH_tree.CopyVals(temp_blob);
        }
        if (oct == 1) {
          if (temp_chisq < this_UO_nopen_chisqmin)
            nopen_UO_tree .CopyVals(temp_blob);
        }
	
	// Save if this is a better minimum
	if (temp_chisq < this_nopen_chisqmin){
	  this_nopen_chisqmin = temp_chisq;
	  nopen_oct = oct;
	  min_nopen_blob.CopyVals(temp_blob);
	}
	delete tempFitThrowOsc;
      }
    }
    delete gpenalty;

    std::cerr << "[THW]: Nopen throw " << i
              << " fit found minimum chi2 = " << nopen_chisqmin << " "
              << BuildLogInfoString();

    // Save info for both fits
    nopen_chisqmin = this_nopen_chisqmin;
    nopen_tree.CopyVals(min_nopen_blob);
    nopen_tree.Fill();

    th13_chisqmin = this_th13_chisqmin;
    th13_tree.CopyVals(min_th13_blob);
    dchisq = th13_chisqmin - nopen_chisqmin;
    th13_tree.Fill();

    // Fill everything
    nopen_LO_tree .Fill();
    nopen_BOTH_tree .Fill();
    nopen_UO_tree .Fill();
    th13_LO_tree .Fill();
    th13_BOTH_tree .Fill();
    th13_UO_tree .Fill();

    // Checkpointing
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
               "[Sample: e.g. ndfd] [Throw: e.g. "
               "stat:fake:start] [Hierarchy: e.g. 1] "
               "[asimov_set] [fake data values]"
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
  int hie = (argc > 7) ? atoi(argv[7]) : def_hie;
  std::string asimov_set = (argc > 8) ? argv[8] : def_asimov_set;
  std::string fakeDataShift = (argc > 9) ? argv[9] : def_fakeDataShift;

  make_FC_throws_th13_test(stateFname, outputFname, nthrows, systSet, sampleString,
			   throwString, hie, asimov_set, fakeDataShift);
}
#endif
#endif
