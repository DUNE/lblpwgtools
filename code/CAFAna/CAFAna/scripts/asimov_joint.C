#include "common_fit_definitions.C"

// I miss python
std::string sanitize(std::string word) {
  uint i = 0;

  while (i < word.size()) {
    if (word[i] == '(' || word[i] == ')') {
      word.erase(i, 1);
      continue;
    }
    i++;
  }
  return word;
}

void RemovePars(std::vector<const IFitVar *> &osclist,
                std::vector<std::string> const &namesToRemove) {
  osclist.erase(std::remove_if(osclist.begin(), osclist.end(),
                               [&](const IFitVar *s) {
                                 return (std::find(namesToRemove.begin(),
                                                   namesToRemove.end(),
                                                   sanitize(s->ShortName())) !=
                                         namesToRemove.end());
                               }),
                osclist.end());
}

// Function to set the binning based on the parameter short name
void GetParameterBinning(std::string parName, int &nBins, double &min,
                         double &max) {

  // Defaults
  nBins = 25;
  min = 0;
  max = 1;

  if (parName == "th13") {
    nBins = 35;
    min = 0.12;
    max = 0.21;
    return;
  }
  if (parName == "deltapi") {
    nBins = 40;
    min = -1;
    max = 1;
  }
  if (parName == "dmsq32scaled") {
    nBins = 40;
    min = 2.25;
    max = 2.65;
  }
  if (parName == "ssth23") {
    nBins = 60;
    min = 0.38;
    max = 0.62;
  }
  if (parName == "ss2th12") {
    nBins = 25;
    min = 0.8;
    max = 0.9;
  }
  if (parName == "dmsq21") {
    nBins = 25;
    min = 6e-5;
    max = 9e-5;
  }
  if (parName == "rho") {
    nBins = 50;
    min = 2.5;
    max = 3.2;
  }
  return;
}

// Likely to have bugs in the translation between what I want to look at, and
// what CAFAna wants to show me...
void SetOscillationParameter(osc::IOscCalculatorAdjustable *calc,
                             std::string parName, double parVal, int hie) {

  if (parName == "th13")
    calc->SetTh13(parVal);
  else if (parName == "deltapi")
    calc->SetdCP(TMath::Pi() * parVal);
  else if (parName == "dmsq32scaled")
    calc->SetDmsq32(hie < 0 ? -1 * parVal / 1000. : parVal / 1000.);
  else if (parName == "ssth23")
    calc->SetTh23(asin(sqrt(parVal)));
  else if (parName == "ss2th12")
    calc->SetTh12(asin(sqrt(parVal)) / 2);
  else if (parName == "dmsq21")
    calc->SetDmsq21(parVal);
  else if (parName == "rho")
    calc->SetRho(parVal);
  return;
}

// This function unpacks the
TH1 *GetAsimovHist(std::vector<std::string> plotVarVect) {
  TH1 *returnHist = NULL;

  int nBinsX = 0, nBinsY = 0;
  double minX = 0, maxX = 0, minY = 0, maxY = 0;

  // Now get the binnings etc for the histograms
  if (plotVarVect.size() > 0)
    GetParameterBinning(plotVarVect[0], nBinsX, minX, maxX);
  if (plotVarVect.size() > 1)
    GetParameterBinning(plotVarVect[1], nBinsY, minY, maxY);

  if (plotVarVect.size() == 1)
    returnHist = new TH1D(plotVarVect[0].c_str(),
                          (plotVarVect[0] + ";" + plotVarVect[0]).c_str(),
                          5 * nBinsX, minX, maxX);
  else if (plotVarVect.size() == 2)
    returnHist = new TH2D((plotVarVect[0] + "_" + plotVarVect[1]).c_str(),
                          (plotVarVect[0] + "_" + plotVarVect[1] + ";" +
                           plotVarVect[0] + ";" + plotVarVect[1])
                              .c_str(),
                          nBinsX, minX, maxX, nBinsY, minY, maxY);

  return returnHist;
}

// Acceptable parameter names: th13, ss2th13, delta(pi), th23, ssth23, ss2th23, dmsq32, dmsq32scaled, tsth12, ss2th12, dmsq21, rho
void asimov_joint(std::string stateFname="common_state_mcc11v3_broken.root",
		  std::string outputFname="asimov_test.root",
		  std::string plotVars="th13:deltapi",
		  std::string systSet = "nosyst",
		  std::string sampleString="ndfd", std::string penaltyString="",int asimov_set=0,
		  bool useProtonFakeData=false){

  gROOT->SetBatch(1);

  std::vector<std::string> plotVarVect = SplitString(plotVars, ':');

  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(systSet);

  // Oscillation parameters to start with
  std::vector<const IFitVar *> oscVarsAll = {
      &kFitDmSq32Scaled,  &kFitSinSqTheta23, &kFitTheta13, &kFitDeltaInPiUnits,
      &kFitSinSq2Theta12, &kFitDmSq21,       &kFitRho};

  // Remove the parameters to be scanned
  std::vector<const IFitVar *> oscVarsFree = oscVarsAll;
  if (plotVarVect.size() > 0)
    RemovePars(oscVarsFree, {plotVarVect[0]});
  if (plotVarVect.size() > 1)
    RemovePars(oscVarsFree, {plotVarVect[1]});

  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();

  FitTreeBlob pftree("fit_info");
  pftree.throw_tree->SetDirectory(fout);

  // Produce Asimovs for both hierarchies in all cases
  for (int hie = -1; hie <= +1; hie += 2) {

    TDirectory* nomDir = (TDirectory*) fout->mkdir(hie > 0 ? "nom_dir_nh" : "nom_dir_ih");

    // This remains the same throughout... there is one true parameter set for this Asimov set
    osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie, 1, asimov_set);

    // Start by performing a minimization across the whole space, this defines the minimum chi2!
    osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie, 1, asimov_set);

    IExperiment *penalty_nom = GetPenalty(hie, 1, penaltyString, asimov_set);
    SystShifts trueSyst = kNoShift;
    if (useProtonFakeData) {
      trueSyst.SetShift(GetMissingProtonEnergyFakeDataSyst().front(), 1);
    }
    SystShifts testSyst = kNoShift;

    // For the nominal, try all octant/dCP combos (shouldn't get it wrong)
    std::map<const IFitVar*, std::vector<double>> oscSeeds = {};

    if (std::find(plotVarVect.begin(), plotVarVect.end(), "ssth23") == plotVarVect.end()){
      oscSeeds[&kFitSinSqTheta23] = {.4, .6};
    }
    if (std::find(plotVarVect.begin(), plotVarVect.end(), "deltapi") == plotVarVect.end()){
      oscSeeds[&kFitDeltaInPiUnits] = {0, 0.5, 1, 1.5};
    }

    double globalmin = RunFitPoint(stateFname, sampleString,
				   trueOsc, trueSyst, false,
				   oscVarsFree, systlist,
				   testOsc, testSyst,
				   oscSeeds, penalty_nom,
				   Fitter::kNormal, nomDir);
    delete penalty_nom;

    std::cout << "Found a minimum global chi2 of: " << globalmin << std::endl;
    fout->cd();
    nomDir->Write();

    // Need to set up the histogram to fill
    TH1 *sens_hist = GetAsimovHist(plotVarVect);
    if (!sens_hist) {
      std::cout << "ERROR: sens_hist not correctly produced!" << std::endl;
      abort();
    }
    sens_hist->SetName(
        (sens_hist->GetName() + std::string(hie > 0 ? "_nh" : "_ih")).c_str());

    // Now loop over the bins in both x and y (if 1D, one loop does nothing)
    for (int xBin = 0; xBin < sens_hist->GetNbinsX(); ++xBin){
      for (int yBin = 0; yBin < sens_hist->GetNbinsY(); ++yBin){

	double chisqmin = 99999;
	double thischisq;

	// Save information if this is 1D only (for now)
	TDirectory* minDir = NULL;
	// if (plotVarVect.size() == 1)
	//   minDir = (TDirectory*) fout->mkdir((std::string(hie > 0 ? "nh" : "ih") + "_" + plotVarVect[0]+"_"+std::to_string(xBin)).c_str());

	// If the parameters of interest don't include theta23, need to loop over octant too...
	// for theta23, this *should* be fine if I only change the ioct in the penalty term... it'll just be terrible in the wrong octant
	for(int ioct = -1; ioct <= 1; ioct += 2) {

	  // Figure out what the fixed parameters are, and put them into the true osc parameters. Also need to do the same for the test osc
	  // Fix whatever I need to!
	  // Probably need a function here to do the fixing
	  osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie, ioct, asimov_set);
	  if (plotVarVect.size() > 0)
	    SetOscillationParameter(testOsc, plotVarVect[0], sens_hist->GetXaxis()->GetBinCenter(xBin+1), hie);
	  if (plotVarVect.size() > 1)
	    SetOscillationParameter(testOsc, plotVarVect[1], sens_hist->GetYaxis()->GetBinCenter(yBin+1), hie);

	  IExperiment *penalty = GetPenalty(hie, ioct, penaltyString, asimov_set);

	  std::map<const IFitVar*, std::vector<double>> oscSeeds = {};

	  // If deltaCP not fixed, seed it!
	  if (std::find(plotVarVect.begin(), plotVarVect.end(), "deltapi") == plotVarVect.end()){
	    oscSeeds[&kFitDeltaInPiUnits] = {0, 0.5, 1, 1.5};
	  }

	  // Add something to save a directory here
	  TDirectory* tempDir = (TDirectory*) fout->mkdir("tempdir");

	  thischisq = RunFitPoint(stateFname, sampleString,
				  trueOsc, trueSyst, false,
				  oscVarsFree, systlist,
				  testOsc, testSyst,
				  oscSeeds, penalty,
				  Fitter::kNormal,
				  tempDir);

	  if (thischisq < chisqmin){
	    chisqmin = thischisq;

	    // Maybe not the best way to ensure only the best fit option is kept...
	    if (minDir){
	      minDir->Delete("*;*");
	      TKey *key;
	      TIter nextkey(tempDir->GetListOfKeys());
	      while ((key = (TKey*)nextkey())) {
		TObject *obj = key->ReadObj();
		minDir->cd();
		obj->Write();
		delete obj;
	      }
	    }
	  }
	  delete penalty;
	  fout->Delete("tempdir;*");
	}
	// Save the value into the hist
	sens_hist->SetBinContent(xBin+1, yBin+1, chisqmin - globalmin);
	fout->cd();
	if (minDir) minDir->Write();
	delete minDir;
      }
    }

    // Save the histogram, and do something sensible with the name
    fout->cd();
    sens_hist->Write();
    delete sens_hist;
  }
  fout->Close();
}
