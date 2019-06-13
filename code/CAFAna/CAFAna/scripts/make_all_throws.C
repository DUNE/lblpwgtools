// One script to bring them all and in the darkness bind them...

#include "common_fit_definitions.C"

void make_all_throws(std::string stateFname="common_state_mcc11v3.root",
		     std::string outputFname="sens_ndfd_nosyst.root",
		     int nthrows = 100, std::string systSet = "nosyst", 
		     std::string sampleString="ndfd",
		     std::string throwString = "stat:fake:start",
		     std::string penaltyString="nopen", int hie=1){
  
  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  // Decide what is to be thrown
  bool stats_throw, fakeoa_throw, fakenuis_throw, start_throw, central_throw;
  ParseThrowInstructions(throwString, stats_throw, fakeoa_throw, fakenuis_throw, start_throw, central_throw);

  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(systSet);

  // Setup an output file
  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");

  // The global tree for all throw types
  FitTreeBlob global_tree("global_fit_info");
  global_tree.throw_tree->SetDirectory(fout);
  
  // Fit in the correct hierarchy and octant for global throw
  std::vector<const IFitVar*> oscVarsGlobal = GetOscVars("alloscvars", hie);

  std::map<const IFitVar*, std::vector<double>> oscSeedsGlobal;
  oscSeedsGlobal[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};
  oscSeedsGlobal[&kFitSinSqTheta23] = {0.4, 0.6};

  // MH specific
  FitTreeBlob mh_tree("mh_fit_info");
  mh_tree.throw_tree->SetDirectory(fout);

  // Fit in the incorrect hierarchy for the exclusion
  std::vector<const IFitVar*> oscVarsMH = GetOscVars("alloscvars", -1*hie);

  // CPV specific
  FitTreeBlob cpv_tree("cpv_fit_info");
  cpv_tree.throw_tree->SetDirectory(fout);

  // CP-conserving specific (no dCP)
  std::vector<const IFitVar*> oscVarsCPV = GetOscVars("dmsq32:th23:th13:th12:dmsq21:rho", hie);

  std::map<const IFitVar*, std::vector<double>> oscSeedsCPV = {};
  oscSeedsCPV[&kFitSinSqTheta23] = {.4, .6}; // try both octants 

  // Octant specific
  FitTreeBlob oct_tree("oct_fit_info");
  oct_tree.throw_tree->SetDirectory(fout);

  std::map<const IFitVar*, std::vector<double>> oscSeedsOct;
  oscSeedsOct[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};

  // Loop over requested throws
  for (int i = 0; i < nthrows; ++i) {
    std::cout << "Starting throw " << i << ":" << std::endl;

    // Set up throws for the starting value
    SystShifts fakeThrowSyst;

    // Always throw OA parameters now!
    osc::IOscCalculatorAdjustable *fakeThrowOsc = ThrownWideOscCalc(hie, oscVarsGlobal);

    double thisdcp = fakeThrowOsc->GetdCP()/TMath::Pi();
    double this_th23 = fakeThrowOsc->GetTh23();
    double this_ssth23 = sin(this_th23)*sin(this_th23);
    int oct = (this_ssth23 > 0.5) ? 1 : -1;
    
    // This is very very dumb, but unavoidable given how the parameters work...
    std::vector<const IFitVar*> oscVarsOct = GetOscVars("alloscvars", hie, -1*oct);

    // Now deal with systematics
    if (fakenuis_throw){
      for (auto s : systlist)
	fakeThrowSyst.SetShift(s, GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
    } else fakeThrowSyst = kNoShift;

    // Prefit throws etc
    SystShifts fitThrowSyst;
    osc::IOscCalculatorAdjustable *fitThrowOsc;
    if (start_throw) {
      for (auto s : systlist)
        fitThrowSyst.SetShift(s, GetBoundedGausThrow(s->Min() * 0.8, s->Max() * 0.8));
      fitThrowOsc = ThrownWideOscCalc(hie, oscVarsGlobal);
    } else {
      fitThrowSyst = kNoShift;
      fitThrowOsc = NuFitOscCalc(hie, 1);
    }

    // Somebody stop him, the absolute madman!
    // Keep the same stats/syst/OA throw for all fits
    std::vector<std::unique_ptr<Spectrum> > mad_spectra_yo = {};
    
    auto fit_type = Fitter::kNormal;

    // -------------------------------------
    // --------- Do the global fit ---------
    // ------------------------------------- 
    std::cout << " -- Global fit" << std::endl;

    // This actually doesn't matter unless we apply a theta23 constraint, which I think we shouldn't anyway...
    IExperiment *gpenalty = GetPenalty(hie, 1, penaltyString);

    double globalmin = RunFitPoint(stateFname, sampleString,
				   fakeThrowOsc, fakeThrowSyst, stats_throw,
				   oscVarsGlobal, systlist,
				   fitThrowOsc, SystShifts(fitThrowSyst),
				   oscSeedsGlobal, gpenalty, fit_type,
				   nullptr, &global_tree, &mad_spectra_yo);     
    global_tree.throw_tree->Fill();
    delete gpenalty;
    delete fitThrowOsc;

    // -------------------------------------
    // --------- Now do CPV fits -----------
    // -------------------------------------
    std::cout << " -- CPV fit" << std::endl;
    {
    // Now fit several times to find the best fit when dCP = 0, pi
    double cpv_chisqmin = 99999;
    double cpv_thischisq;

    for (int tdcp = 0; tdcp < 2; ++tdcp) {
      double dcptest = tdcp*TMath::Pi();

      // Now testOsc is restricted to CP conservation
      osc::IOscCalculatorAdjustable* testOscCPV = NuFitOscCalc(hie, 1);
      testOscCPV->SetdCP(dcptest);

      IExperiment *cpv_penalty = GetPenalty(hie, 1, penaltyString);
      
      cpv_thischisq = RunFitPoint(stateFname, sampleString,
				  fakeThrowOsc, fakeThrowSyst, stats_throw,
				  oscVarsCPV, systlist,
				  testOscCPV, SystShifts(fitThrowSyst),
				  oscSeedsCPV, cpv_penalty, fit_type,
				  nullptr, &cpv_tree, &mad_spectra_yo);

      cpv_chisqmin = TMath::Min(cpv_thischisq,cpv_chisqmin);
      delete cpv_penalty;
      delete testOscCPV;
    }

    double cpv_dchi2 = cpv_chisqmin - globalmin;
    double cpv_significance = 0;
    if (cpv_dchi2 > 0) cpv_significance = sqrt(cpv_dchi2);
    else std::cerr << "CPV: dchi2 of " << cpv_dchi2 << "; " << cpv_chisqmin << " - " << globalmin << std::endl;

    // Add the variables of interest to the tree
    cpv_tree.throw_tree->Branch("chisqmin", &cpv_chisqmin);
    cpv_tree.throw_tree->Branch("globalmin", &globalmin);
    cpv_tree.throw_tree->Branch("hie", &hie);
    cpv_tree.throw_tree->Branch("dcp", &thisdcp);
    cpv_tree.throw_tree->Branch("dchi2", &cpv_dchi2);
    cpv_tree.throw_tree->Branch("significance", &cpv_significance);
    cpv_tree.throw_tree->Fill();
    }

    // -------------------------------------
    // --------- Now octant fits -----------
    // ------------------------------------- 
    std::cout << " -- Octant fit" << std::endl;

    osc::IOscCalculatorAdjustable* testOscOct = NuFitOscCalc(hie, -1*oct);

    // No penalty on the octant, so ignore it...
    IExperiment *oct_penalty = GetPenalty(hie, 1, penaltyString);

    double oct_chisqmin = RunFitPoint(stateFname, sampleString,
				      fakeThrowOsc, fakeThrowSyst, stats_throw,
				      oscVarsOct, systlist,
				      testOscOct, SystShifts(fitThrowSyst),
				      oscSeedsOct, oct_penalty, fit_type,
				      nullptr, &oct_tree, &mad_spectra_yo);

    double oct_dchi2 = oct_chisqmin - globalmin;
    double oct_significance = 0;
    if (oct_dchi2 > 0) oct_significance = sqrt(oct_dchi2);
    else std::cerr << "Octant: dchi2 of " << oct_dchi2 << "; " << oct_chisqmin << " - " << globalmin << std::endl;

    // Add the variables of interest to the tree
    oct_tree.throw_tree->Branch("chisqmin", &oct_chisqmin);
    oct_tree.throw_tree->Branch("globalmin", &globalmin);
    oct_tree.throw_tree->Branch("hie", &hie);
    oct_tree.throw_tree->Branch("ssth23", &this_ssth23);
    oct_tree.throw_tree->Branch("th23", &this_th23);
    oct_tree.throw_tree->Branch("dchi2", &oct_dchi2);
    oct_tree.throw_tree->Branch("significance", &oct_significance);
    oct_tree.throw_tree->Fill();
    delete oct_penalty;
    delete testOscOct;
   
    // -------------------------------------
    // --------- Now the MH fits -----------
    // -------------------------------------
    std::cout << " -- MH fit" << std::endl;

    // Force the testOsc to be in the wrong hierarchy 
    osc::IOscCalculatorAdjustable* testOscMH = NuFitOscCalc(-1*hie, 1);

    // Wrong hierarchy remember
    IExperiment *mh_penalty = GetPenalty(-1*hie, 1, penaltyString);
    
    double mh_chisqmin = RunFitPoint(stateFname, sampleString,
				     fakeThrowOsc, fakeThrowSyst, stats_throw,
				     oscVarsMH, systlist,
				     testOscMH, SystShifts(fitThrowSyst),
				     oscSeedsGlobal, mh_penalty, fit_type, // same seeds as a global fit
				     nullptr, &mh_tree, &mad_spectra_yo);
  
    double mh_dchi2 = mh_chisqmin - globalmin;
    double mh_significance = 0;
    if (mh_dchi2 > 0) mh_significance = sqrt(mh_dchi2);
    else std::cerr << "MH: dchi2 of " << mh_dchi2 << "; " << mh_chisqmin << " - " << globalmin << std::endl;

    // Add the variables of interest to the tree
    mh_tree.throw_tree->Branch("chisqmin", &mh_chisqmin);
    mh_tree.throw_tree->Branch("globalmin", &globalmin);
    mh_tree.throw_tree->Branch("hie", &hie);
    mh_tree.throw_tree->Branch("dcp", &thisdcp);
    mh_tree.throw_tree->Branch("dchi2", &mh_dchi2);
    mh_tree.throw_tree->Branch("significance", &mh_significance);
    mh_tree.throw_tree->Fill();

    delete mh_penalty;
    delete testOscMH;

    delete fakeThrowOsc;
  }

  fout->Write();  
  fout->Close();
}
