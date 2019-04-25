#include "common_fit_definitions.C"

void make_mh_throws(std::string stateFname="common_state_mcc11v3.root",
		    std::string outputFname="mh_sens_ndfd_nosyst.root",
		    int nthrows = 100, std::string systSet = "nosyst", 
		    std::string sampleString="ndfd",
		    std::string throwString = "stat:fake:start",
		    std::string penaltyString="nopen", int hie=1, int idcp=0){

  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  // Decide what is to be thrown
  bool stats_throw, fakeoa_throw, fakenuis_throw, start_throw, central_throw;
  ParseThrowInstructions(throwString, stats_throw, fakeoa_throw, fakenuis_throw, start_throw, central_throw);

  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(systSet);

  // Fit in the correct hierachy for the global fit
  std::vector<const IFitVar*> oscVars = GetOscVars("alloscvars", hie);

  // Fit in the incorrect hierarchy for the exclusion
  std::vector<const IFitVar*> oscVarsWrong = GetOscVars("alloscvars", -1*hie);
  
  // Setup an output file
  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  FitTreeBlob global_tree("global_fit_info");
  global_tree.throw_tree->SetDirectory(fout);
  FitTreeBlob mh_tree("mh_fit_info");
  mh_tree.throw_tree->SetDirectory(fout);

  // Interpret the dcpstep once
  double dcpstep = 2*TMath::Pi()/36;
  double thisdcp = -TMath::Pi() + idcp*dcpstep;

  // Deal with seeds once
  std::map<const IFitVar*, std::vector<double>> oscSeeds;
  oscSeeds[&kFitSinSqTheta23] = {.4, .6};
  oscSeeds[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};

  // Loop over requested throws
  for (int i = 0; i < nthrows; ++i) {

    std::cout << "Starting throw " << i << std::endl;

    // Set up throws for the starting value
    SystShifts fakeThrowSyst;
    osc::IOscCalculatorAdjustable *fakeThrowOsc;

    // First deal with OA parameters
    if (fakeoa_throw || central_throw) fakeThrowOsc = ThrownWideOscCalc(hie, oscVars);
    else fakeThrowOsc = NuFitOscCalc(hie, 1);

    // Set dCP correctly for this throw...
    fakeThrowOsc->SetdCP(thisdcp);

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
      fitThrowOsc = NuFitOscCalc(hie, 1);
    }

    // Somebody stop him, the absolute madman!
    // Keep the same stats throw for both fits to get the delta chi2
    std::vector<std::unique_ptr<Spectrum> > mad_spectra_yo = {};

    // Need to find the best fit in the correct hierachy
    // Note that I'm ignoring the octant here
    // This actually doesn't matter unless we apply a theta23 constraint, which I think we shouldn't anyway...
    IExperiment *gpenalty = GetPenalty(hie, 1, penaltyString);

    double globalmin = RunFitPoint(stateFname, sampleString,
				   fakeThrowOsc, fakeThrowSyst, stats_throw,
				   oscVars, systlist,
				   fitThrowOsc, fitThrowSyst,
				   oscSeeds, gpenalty, Fitter::kNormal, 
				   nullptr, &global_tree, &mad_spectra_yo);     
    global_tree.throw_tree->Fill();

    // Now force the testOsc to be in the wrong hierarchy 
    osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(-1*hie, 1);
    testOsc->SetdCP(thisdcp);
    fitThrowOsc->SetDmsq32(-1*fitThrowOsc->GetDmsq32());
    // Wrong hierarchy remember
    IExperiment *penalty = GetPenalty(-1*hie, 1, penaltyString);
    
    double chisqmin = RunFitPoint(stateFname, sampleString,
				  fakeThrowOsc, fakeThrowSyst, stats_throw, // This line is actually ignored...
				  oscVarsWrong, systlist,
				  testOsc, fitThrowSyst,
				  oscSeeds, penalty, Fitter::kNormal, 
				  nullptr, &mh_tree, &mad_spectra_yo);
  
    double dchi2 = chisqmin - globalmin;
    double significance = 0;
    if (dchi2 > 0) significance = sqrt(dchi2);
    else std::cout << "ERROR: dchi2 of " << dchi2 << "; " << chisqmin << " - " << globalmin << std::endl;

    // Add the variables of interest to the tree
    mh_tree.throw_tree->Branch("chisqmin", &chisqmin);
    mh_tree.throw_tree->Branch("globalmin", &globalmin);
    mh_tree.throw_tree->Branch("hie", &hie);
    mh_tree.throw_tree->Branch("idcp", &idcp);
    mh_tree.throw_tree->Branch("dcp", &thisdcp);
    mh_tree.throw_tree->Branch("dchi2", &dchi2);
    mh_tree.throw_tree->Branch("significance", &significance);
    mh_tree.throw_tree->Fill();

    delete penalty;
    delete gpenalty;
  }

  fout->Write();  
  fout->Close();
}
