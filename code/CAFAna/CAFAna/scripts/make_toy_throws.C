#include "common_fit_definitions.C"

// Need to accept filename, ND/FD, systs and reload as arguments
void make_toy_throws(std::string stateFname = "common_state_mcc11v3.root",
                     std::string outputFname = "throws_ndfd_nosyst.root",
                     int nthrows = 100, std::string systSet = "nosyst",
                     std::string sampleString = "ndfd",
                     std::string throwString = "stat:fake:start",
                     std::string penaltyString = "nopen",
                     int hie=1,
		     std::string asimov_set="0",
		     std::string oscVarString = "th13:deltapi:th23:dmsq32") {

  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  // Decide what is to be thrown
  bool stats_throw, fakeoa_throw, fakenuis_throw, start_throw, central_throw;
  ParseThrowInstructions(throwString, stats_throw, fakeoa_throw, fakenuis_throw, start_throw, central_throw);

  // Get the systematics to use
  std::vector<const ISyst *> systlist = GetListOfSysts(systSet);

  // Oscillation parameters to use
  std::vector<const IFitVar*> oscVars = {};
  if (sampleString.find("fd") != std::string::npos) {
    oscVars = GetOscVars(oscVarString, hie);
  }

  // Deal with seeds once
  std::map<const IFitVar*, std::vector<double>> oscSeeds;
  if (sampleString.find("fd") != std::string::npos) {
    oscSeeds[&kFitSinSqTheta23] = {.4, .6}; // try both octants                                                                                                              
    oscSeeds[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};
  }

  // Setup output file
  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");

  FitTreeBlob pftree("fit_info");
  pftree.throw_tree->SetDirectory(fout);

  for (int i = 0; i < nthrows; ++i) {

    std::cout << "Starting throw " << i << std::endl;

    // Set up throws for the starting value
    SystShifts fakeThrowSyst;
    osc::IOscCalculatorAdjustable *fakeThrowOsc;
    
    // First deal with OA parameters
    if (fakeoa_throw || central_throw) fakeThrowOsc = ThrownWideOscCalc(hie, oscVars);
    else fakeThrowOsc = NuFitOscCalc(hie, 1, asimov_set);
      
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
      fitThrowOsc = NuFitOscCalc(hie, 1, asimov_set);
    }
    Fitter::Precision fitStrategy = Fitter::kNormal; //|Fitter::kIncludeHesse;

    IExperiment *penalty = GetPenalty(hie, 1, penaltyString);

    double thischisq =
        RunFitPoint(stateFname, sampleString, fakeThrowOsc, fakeThrowSyst,
                    stats_throw, oscVars, systlist, fitThrowOsc, fitThrowSyst,
                    oscSeeds, penalty, fitStrategy, nullptr, &pftree);
    pftree.throw_tree->Fill();
    std::cout << "Throw " << i << ": found minimum chi2 = " << thischisq
              << std::endl;
    // Done with this systematic throw
    delete penalty;
  }

  // Now close the file
  fout->Write();
  fout->Close();
}
