#include "common_fit_definitions.C"

void make_octant_throws(std::string stateFname="common_state_mcc11v3.root",
			std::string outputFname="octant_sens_ndfd_nosyst.root",
			int nthrows = 50, std::string systSet = "nosyst", 
			std::string sampleString="ndfd",
			std::string throwString = "start",
			std::string penaltyString="nopen", int hie=1, int issth23=0){
  
  gROOT->SetBatch(1);
  gRandom->SetSeed(0);
  
  // Decide what is to be thrown
  bool stats_throw, fakeoa_throw, fakenuis_throw, start_throw, central_throw;
  ParseThrowInstructions(throwString, stats_throw, fakeoa_throw, fakenuis_throw, start_throw, central_throw);
  
  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(systSet);
  
  // Interpret the octant step once
  // Care about ssth23 = 0.3--0.7, have 31 steps
  double minVal = 0.3;
  double maxVal = 0.7;
  int nsteps = 31;
  double ssth23_step = (maxVal - minVal)/nsteps;
  double this_ssth23 = minVal + issth23*ssth23_step;
  double this_th23   = asin(sqrt(this_ssth23));

  int oct = (this_ssth23 > 0.5) ? 1 : -1;
  std::cout << "ssth23 = " << this_ssth23 << "; found octant = " << oct << std::endl;

  // Fit in BOTH octants for the global fit
  std::vector<const IFitVar*> oscVars = GetOscVars("alloscvars", hie, 0);
  
  // Fit in the incorrect octant for the exclusion
  std::vector<const IFitVar*> oscVarsWrong = GetOscVars("alloscvars", hie, -1*oct);
  
  // Setup an output file
  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  FitTreeBlob global_tree("global_fit_info");
  global_tree.throw_tree->SetDirectory(fout);
  FitTreeBlob oct_tree("oct_fit_info");
  oct_tree.throw_tree->SetDirectory(fout);

  // Deal with seeds once
  std::map<const IFitVar*, std::vector<double>> oscSeeds;
  oscSeeds[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};
  
  std::map<const IFitVar*, std::vector<double>> oscSeedsG;
  oscSeedsG[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};
  oscSeedsG[&kFitSinSqTheta23] = {0.4, 0.6};
  
  // Loop over requested throws
  for (int i = 0; i < nthrows; ++i) {

    std::cout << "Starting throw " << i << std::endl;

    // Set up throws for the starting value
    SystShifts fakeThrowSyst;
    osc::IOscCalculatorAdjustable *fakeThrowOsc;

    // First deal with OA parameters
    if (fakeoa_throw || central_throw) fakeThrowOsc = ThrownWideOscCalc(hie, oscVars);
    else fakeThrowOsc = NuFitOscCalc(hie, oct);

    // Set theta23
    fakeThrowOsc->SetTh23(this_th23);

    // Now deal with systematics
    if (fakenuis_throw and not central_throw){
      for (auto s : systlist)
	fakeThrowSyst.SetShift(s, GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
    } else fakeThrowSyst = kNoShift;
    
    if (central_throw){
      for (auto s : systlist)
	s->SetCentral(GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
    }

    // Prefit
    SystShifts fitThrowSyst;
    osc::IOscCalculatorAdjustable *fitThrowOsc;
    if (start_throw) {
      for (auto s : systlist)
	fitThrowSyst.SetShift(s, GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
      fitThrowOsc = ThrownWideOscCalc(hie, oscVars);
    } else {
      fitThrowSyst = kNoShift;
      fitThrowOsc = NuFitOscCalc(hie, oct);
    }

    // Somebody stop him, the absolute madman!
    // Keep the same stats throw for both fits to get the delta chi2
    std::vector<std::unique_ptr<Spectrum> > mad_spectra_yo = {};

    // Need to find the best fit in the correct octant
    IExperiment *gpenalty = GetPenalty(hie, 1, penaltyString);

    double globalmin = RunFitPoint(stateFname, sampleString,
				   fakeThrowOsc, fakeThrowSyst, stats_throw,
				   oscVars, systlist,
				   fitThrowOsc, fitThrowSyst,
				   oscSeedsG, gpenalty, Fitter::kNormal, 
				   nullptr, &global_tree, &mad_spectra_yo);     
    global_tree.throw_tree->Fill();

    // Now force the testOsc to be in the wrong octant
    osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie, -1*oct);

    // No penalty on the octant, so ignore it...
    IExperiment *penalty = GetPenalty(hie, 1, penaltyString);
    
    double chisqmin = RunFitPoint(stateFname, sampleString,
				  fakeThrowOsc, fakeThrowSyst, stats_throw, // This line is actually ignored...
				  oscVarsWrong, systlist,
				  testOsc, fitThrowSyst,
				  oscSeeds, penalty, Fitter::kNormal, 
				  nullptr, &oct_tree, &mad_spectra_yo);
  
    double dchi2 = chisqmin - globalmin;
    double significance = 0;
    if (dchi2 > 0) significance = sqrt(dchi2);
    else std::cout << "ERROR: dchi2 of " << dchi2 << "; " << chisqmin << " - " << globalmin << std::endl;

    // Add the variables of interest to the tree
    oct_tree.throw_tree->Branch("chisqmin", &chisqmin);
    oct_tree.throw_tree->Branch("globalmin", &globalmin);
    oct_tree.throw_tree->Branch("hie", &hie);
    oct_tree.throw_tree->Branch("issth23", &issth23);
    oct_tree.throw_tree->Branch("ssth23", &this_ssth23);
    oct_tree.throw_tree->Branch("th23", &this_th23);
    oct_tree.throw_tree->Branch("dchi2", &dchi2);
    oct_tree.throw_tree->Branch("significance", &significance);
    oct_tree.throw_tree->Fill();

    delete penalty;
    delete gpenalty;
  }

  fout->Write();  
  fout->Close();
}
