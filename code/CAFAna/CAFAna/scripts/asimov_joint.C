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
  nBins = 200;
  min = 0;
  max = 1;

  if (parName == "th13") {
    nBins = 200;
    min = 0.12;
    max = 0.21;
    return;
  }
  if (parName == "deltapi") {
    nBins = 200;
    min = -1;
    max = 1;
  }
  if (parName == "dmsq32scaled") {
    nBins = 200;
    min = 2.25;
    max = 2.65;
  }
  if (parName == "ssth23") {
    nBins = 200;
    min = 0.38;
    max = 0.62;
  }
  if (parName == "ss2th12") {
    nBins = 200;
    min = 0.8;
    max = 0.9;
  }
  if (parName == "dmsq21") {
    nBins = 200;
    min = 6e-5;
    max = 9e-5;
  }
  if (parName == "rho") {
    nBins = 200;
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

  std::string yVal;
  // Now get the binnings etc for the histograms
  if (plotVarVect.size() > 0)
    GetParameterBinning(plotVarVect[0], nBinsX, minX, maxX);
  if (plotVarVect.size() > 1)
    GetParameterBinning(plotVarVect[1], nBinsY, minY, maxY);
  if (plotVarVect.size() > 2){
    yVal = plotVarVect[2];
    plotVarVect.pop_back();
  }
  
  if (plotVarVect.size() == 1)
    returnHist = new TH1D(plotVarVect[0].c_str(),
                          (plotVarVect[0] + ";" + plotVarVect[0]).c_str(),
                          nBinsX, minX, maxX);
  else if (plotVarVect.size() == 2)
    returnHist = new TH2D((plotVarVect[0] + "_" + plotVarVect[1]).c_str(),
                          (plotVarVect[0] + "_" + plotVarVect[1] + ";" +
                           plotVarVect[0] + ";" + plotVarVect[1])
                              .c_str(),
                          nBinsX, minX, maxX, nBinsY, minY, maxY);

  // if (not yVal.empty()) returnHist->SetName((plotVarVect[0] + "_" + plotVarVect[1]+"_"+yVal).c_str());
  
  return returnHist;
}

// Acceptable parameter names: th13, ss2th13, delta(pi), th23, ssth23, ss2th23, dmsq32, dmsq32scaled, tsth12, ss2th12, dmsq21, rho
void asimov_joint(std::string stateFname="common_state_mcc11v3_broken.root",
		  std::string outputFname="asimov_test.root",
		  std::string plotVars="th13:deltapi",
		  std::string systSet = "nosyst",
		  std::string sampleString="ndfd",
		  std::string penaltyString="",
		  int hie = 1,
		  std::string asimov_set="0",
		  bool useProtonFakeData=false){

  gROOT->SetBatch(1);
  gRandom->SetSeed(0);  

  std::vector<std::string> plotVarVect = SplitString(plotVars, ':');

  // Get the systematics to use
  std::vector<const ISyst*> systlist = GetListOfSysts(systSet);

  // Oscillation parameters to start with
  std::vector<const IFitVar *> oscVarsAll = GetOscVars("alloscvars", hie);

  // Remove the parameters to be scanned
  std::vector<const IFitVar *> oscVars = oscVarsAll;

  // This is very hacky... third elements in the list are used to define the strip to loop at this time
  int yVal = -1;
  if (plotVarVect.size() > 0)
    RemovePars(oscVars, {plotVarVect[0]});
  if (plotVarVect.size() > 1)
    RemovePars(oscVars, {plotVarVect[1]});
  if (plotVarVect.size() > 2)
    yVal = stoi(plotVarVect[2]);
  
  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();

  FitTreeBlob asimov_tree("asimov_tree");
  asimov_tree.throw_tree->SetDirectory(fout);
  
  // This remains the same throughout... there is one true parameter set for this Asimov set
  osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie, 1, asimov_set);
  
  // Save the true information
  SaveTrueOAParams(fout, trueOsc);

  // Start by performing a minimization across the whole space, this defines the minimum chi2!
  osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie, 1, asimov_set);
  
  IExperiment *penalty_nom = GetPenalty(hie, 1, penaltyString, asimov_set);
  SystShifts trueSyst = kNoShift;
  if (useProtonFakeData) {
    trueSyst.SetShift(GetMissingProtonEnergyFakeDataSyst().front(), 1);
  }
  SystShifts testSyst = kNoShift;
  
  // For the nominal, try all octant/dCP combos (shouldn't get it wrong)
  std::map<const IFitVar*, std::vector<double>> oscSeedsAll = {};
  oscSeedsAll[&kFitSinSqTheta23] = {.4, .6};
  oscSeedsAll[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};

  // For the asimov, seed whatever isn't fixed
  std::map<const IFitVar*, std::vector<double>> oscSeeds = {};
  if (std::find(plotVarVect.begin(), plotVarVect.end(), "deltapi") == plotVarVect.end())
    oscSeeds[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};
  if (std::find(plotVarVect.begin(), plotVarVect.end(), "ssth23") == plotVarVect.end())
    oscSeeds[&kFitSinSqTheta23] = {.4, .6};
  
  
  double globalmin = RunFitPoint(stateFname, sampleString,
				 trueOsc, trueSyst, false,
				 oscVarsAll, systlist,
				 testOsc, testSyst,
				 oscSeedsAll, penalty_nom,
				 Fitter::kNormal, nullptr);
  delete penalty_nom;  
  std::cout << "Found a minimum global chi2 of: " << globalmin << std::endl;
  
  // Need to set up the histogram to fill
  TH1 *sens_hist = GetAsimovHist(plotVarVect);
  if (!sens_hist) {
    std::cout << "ERROR: sens_hist not correctly produced!" << std::endl;
    abort();
  }
  
  int yMin = 0;
  int yMax = sens_hist->GetNbinsY();

  if (yVal >= 0){
    yMin = yVal;
    yMax = yVal+1;
  }
  
  // Now loop over the bins in both x and y (if 1D, one loop does nothing)
  for (int xBin = 0; xBin < sens_hist->GetNbinsX(); ++xBin){

    double xCenter = sens_hist->GetXaxis()->GetBinCenter(xBin+1);
    // If yVal is set, only scan over that value... serialise these
    for (int yBin = yMin; yBin < yMax; ++yBin){
      double yCenter = sens_hist->GetYaxis()->GetBinCenter(yBin+1);
      
      osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie, 1, asimov_set);
      if (plotVarVect.size() > 0)
	SetOscillationParameter(testOsc, plotVarVect[0], xCenter, hie);
      if (plotVarVect.size() > 1)
	SetOscillationParameter(testOsc, plotVarVect[1], yCenter, hie);
      
      IExperiment *penalty = GetPenalty(hie, 1, penaltyString, asimov_set);

      double chisqmin = RunFitPoint(stateFname, sampleString,
				    trueOsc, trueSyst, false,
				    oscVars, systlist,
				    testOsc, testSyst,
				    oscSeeds, penalty,
				    Fitter::kNormal, nullptr,
				    &asimov_tree);

      // Save relevant values into the tree and histogram
      double chisqdiff = chisqmin - globalmin;
      asimov_tree.throw_tree->Branch("chisqmin", &chisqmin);
      asimov_tree.throw_tree->Branch("chisqdiff", &chisqdiff);
      asimov_tree.throw_tree->Branch("globalmin", &globalmin);
      asimov_tree.throw_tree->Branch("xVal", &xCenter);
      asimov_tree.throw_tree->Branch("xName", &plotVarVect[0]);
      if (plotVarVect.size() > 1) asimov_tree.throw_tree->Branch("yVal", &yCenter);
      if (plotVarVect.size() > 1) asimov_tree.throw_tree->Branch("yName", &plotVarVect[1]);
      asimov_tree.throw_tree->Fill();
      sens_hist->SetBinContent(xBin+1, yBin+1, chisqdiff);
      delete penalty;
    }
  }
  
  // Save the histogram, and do something sensible with the name
  fout->cd();
  sens_hist->Write();
  fout->Write();
  fout->Close();
  delete sens_hist;
}
