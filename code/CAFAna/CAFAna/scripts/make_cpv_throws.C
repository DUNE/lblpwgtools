#include "common_fit_definitions.C"

void make_cpv_throws(std::string stateFname="common_state_mcc11v3.root",
		     std::string outputFname="cpv_sens_ndfd_nosyst.root",
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

  // Oscillation parameters to use for a fixed dCP value
  std::vector<const IFitVar*> oscVars = GetOscVars("dmsq32:th23:th13", hie);

  // Oscillation parameters when finding the global minimum
  std::vector<const IFitVar*> oscVarsAll = GetOscVars("alloscvars", hie);

  // Setup an output file
  TFile* fout = new TFile(outputFname.c_str(), "RECREATE");
  FitTreeBlob global_tree("global_fit_info");
  global_tree.throw_tree->SetDirectory(fout);

  FitTreeBlob cpv_tree("cpv_fit_info");
  cpv_tree.throw_tree->SetDirectory(fout);

  // Sort out seeds once at the start
  std::map<const IFitVar*, std::vector<double>> oscSeedsAll;
  oscSeedsAll[&kFitSinSqTheta23] = {.4, .6}; // try both octants                                                                                                             
  oscSeedsAll[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5}; // Maybe not necessary...
  
  // For dCP=0/pi
  std::map<const IFitVar*, std::vector<double>> oscSeeds = {};
  oscSeeds[&kFitSinSqTheta23] = {.4, .6}; // try both octants

  // Interpret the dcpstep once
  double dcpstep = 2*TMath::Pi()/36;
  double thisdcp = -TMath::Pi() + idcp*dcpstep;

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
    
    // Need to do the initial fit for this throw... find the minimum chi2 for this throw
    // Ignoring the possibility of a theta23 penalty here
    IExperiment *gpenalty = GetPenalty(hie, 1, penaltyString);

    double globalmin = RunFitPoint(stateFname, sampleString,
				   fakeThrowOsc, fakeThrowSyst, stats_throw,
				   oscVarsAll, systlist,
				   fakeThrowOsc, fakeThrowSyst,
				   oscSeedsAll, gpenalty, Fitter::kNormal, 
				   nullptr, &global_tree, &mad_spectra_yo);     
    global_tree.throw_tree->Fill();
    delete gpenalty;

    // Now fit several times to find the best fit when dCP = 0, pi
    double chisqmin = 99999;
    double thischisq;
    
    for (int tdcp = 0; tdcp < 2; ++tdcp) {
      double dcptest = tdcp*TMath::Pi();

      // Now testOsc is restricted to CP conservation
      osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie, 1);
      testOsc->SetdCP(dcptest);
      
      IExperiment *penalty = GetPenalty(hie, 1, penaltyString);
      
      thischisq = RunFitPoint(stateFname, sampleString,
			      fakeThrowOsc, fakeThrowSyst, stats_throw,
			      oscVars, systlist,
			      testOsc, fitThrowSyst,
			      oscSeeds, penalty, Fitter::kNormal, 
			      nullptr, &cpv_tree, &mad_spectra_yo);
      
      chisqmin = TMath::Min(thischisq,chisqmin);
      delete penalty;
    }
    
    double dchi2 = chisqmin - globalmin;
    double significance = 0;
    if (dchi2 > 0) significance = sqrt(dchi2);
    else std::cout << "ERROR: dchi2 of " << dchi2 << "; " << chisqmin << " - " << globalmin << std::endl;

    // Add the variables of interest to the tree
    cpv_tree.throw_tree->Branch("chisqmin", &chisqmin);
    cpv_tree.throw_tree->Branch("globalmin", &globalmin);
    cpv_tree.throw_tree->Branch("hie", &hie);
    cpv_tree.throw_tree->Branch("idcp", &idcp);
    cpv_tree.throw_tree->Branch("dcp", &thisdcp);
    cpv_tree.throw_tree->Branch("dchi2", &dchi2);
    cpv_tree.throw_tree->Branch("significance", &significance);
    cpv_tree.throw_tree->Fill();
  }

  fout->Write();  
  fout->Close();
}
