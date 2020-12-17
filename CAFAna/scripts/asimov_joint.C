#include "CAFAna/Analysis/common_fit_definitions.h"

using namespace ana;

// Function to set the binning based on the parameter short name
void GetParameterBinning(std::string parName, int &nBins, double &min,
                         double &max, double trueVal) {

  // Defaults
  nBins = 100;
  min = 0;
  max = 1;

  if (parName == "th13") {
    min = 0.10;
    max = 0.22;

    // If trueVal is set and outside the nominal range, do something
    if (abs(trueVal) < 100 && (trueVal > max || trueVal < min)){
      double temp = std::round(trueVal*100)/100.;
      min = temp - 0.03;
      max = temp + 0.03;
    }

  } else if (parName == "deltapi") {
    min = -1;
    max = 1;

  } else if (parName == "dmsq32scaled" or parName == "dmsq32") {
    min = 2.38;
    max = 2.55;
    // If trueVal is set and outside the nominal range, do something
    // if (abs(trueVal) < 100 && (trueVal > max || trueVal < min)){
    //   double temp = std::round(trueVal*100)/100.;
    //   min = temp - 0.1;
    //   max = temp + 0.1;
    // }

  } else if (parName == "ssth23") {
    min = 0.38;
    max = 0.62;

    // Can't do much here because we need to see the octant flip

  } else if (parName == "ss2th12") {
    min = 0.8;
    max = 0.9;
  } else if (parName == "dmsq21") {
    min = 6e-5;
    max = 9e-5;
  } else if (parName == "rho") {
    min = 2.5;
    max = 3.2;
  }
  return;
}

// This function unpacks the
TH1 *GetAsimovHist(std::vector<std::string> plotVarVect, osc::IOscCalculatorAdjustable* trueOscPoint = NULL) {
  TH1 *returnHist = NULL;

  int nBinsX = 0, nBinsY = 0;
  double minX = 0, maxX = 0, minY = 0, maxY = 0;

  // Get the true value of these parameters to help determine the histogram range
  

  // Now get the binnings etc for the histograms
  if (plotVarVect.size() > 0)
    GetParameterBinning(plotVarVect[0], nBinsX, minX, maxX, FindOscVal(trueOscPoint, plotVarVect[0]));
  if (plotVarVect.size() > 1)
    GetParameterBinning(plotVarVect[1], nBinsY, minY, maxY, FindOscVal(trueOscPoint, plotVarVect[1]));

  if (plotVarVect.size() == 1) {
    returnHist = new TH1D(plotVarVect[0].c_str(),
                          (plotVarVect[0] + ";" + plotVarVect[0]).c_str(),
                          nBinsX, minX, maxX);
  } else if (plotVarVect.size() > 1) {
    returnHist = new TH2D((plotVarVect[0] + "_" + plotVarVect[1]).c_str(),
                          (plotVarVect[0] + "_" + plotVarVect[1] + ";" +
                           plotVarVect[0] + ";" + plotVarVect[1])
                              .c_str(),
                          nBinsX, minX, maxX, nBinsY, minY, maxY);
  } else {
    std::cerr << "Something went wrong when setting up the histogram!"
              << std::endl;
    exit(1);
  }

  return returnHist;
}

TGraph* GetFitPoint(std::vector<std::string> plotVarVect, osc::IOscCalculatorAdjustable *oscCalc){
  
  // Get the values of the x and y co-ordinate. If this is 1-D, y=0
  double xVal = 0;
  double yVal = 0;
  
  if (plotVarVect.size() > 0) xVal = FindOscVal(oscCalc, plotVarVect[0]);
  if (plotVarVect.size() > 1) yVal = FindOscVal(oscCalc, plotVarVect[1]);

  // Now make a TGraph to return
  TGraph *ret = new TGraph(1, &xVal, &yVal);
  return ret;
}

char const *def_stateFname = "common_state_mcc11v3_broken.root";
char const *def_outputFname = "asimov_test.root";
char const *def_plotVars = "th13:deltapi";
char const *def_systSet = "nosyst";
char const *def_sampleString = "ndfd";
char const *def_penaltyString = "";
int const def_hie = 1;
char const *def_asimov_set = "0";
char const *def_fakeDataShift = "";
int const def_fitBias = 1;

// Acceptable parameter names: th13, ss2th13, delta(pi), th23, ssth23, ss2th23,
// dmsq32, dmsq32scaled, tsth12, ss2th12, dmsq21, rho
void asimov_joint(std::string stateFname = def_stateFname,
                  std::string outputFname = def_outputFname,
                  std::string plotVars = def_plotVars,
                  std::string systSet = def_systSet,
                  std::string sampleString = def_sampleString,
                  std::string penaltyString = def_penaltyString,
                  int hie = def_hie, std::string asimov_set = def_asimov_set,
                  std::string fakeDataShift = def_fakeDataShift,
		  int fitBias = def_fitBias) {

  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  std::vector<std::string> plotVarVect = SplitString(plotVars, ':');

  // This function allows any dial to be set to non-nominal
  SystShifts trueSyst = GetFakeDataSystShift(fakeDataShift);

  // Get the systematics to use
  std::vector<const ISyst *> systlist = GetListOfSysts(systSet);

  // Should the fake data dials be removed from the systlist?
  if (!fitBias){
    
    std::vector<std::string> bias_syst_names;
    // Loop over all systs set to a non-nominal value and remove
    for (auto syst : trueSyst.ActiveSysts()){
      std::cout << "Removing " << syst->ShortName() <<std::endl; 
      bias_syst_names.push_back(syst->ShortName());
      RemoveSysts(systlist, bias_syst_names);
    }
  }

  // Oscillation parameters to start with
  std::vector<const IFitVar *> oscVarsAll = GetOscVars("alloscvars", hie);

  // Remove the parameters to be scanned
  std::vector<const IFitVar *> oscVars = oscVarsAll;

  // This is very hacky... third elements in the list are used to define the
  // strip to loop at this time
  int yVal = -999;
  int xVal = -999;

  bool isGlobal = true;
  bool isSinglePoint = false;

  if (plotVarVect.size() > 0)
    RemovePars(oscVars, {plotVarVect[0]});
  if (plotVarVect.size() > 1)
    RemovePars(oscVars, {plotVarVect[1]});
  if (plotVarVect.size() > 2)
    xVal = stoi(plotVarVect[2]);
  if (plotVarVect.size() > 3) {
    isSinglePoint = true;
    yVal = stoi(plotVarVect[3]);
  }

  // Should we save the best fit information as a TGraph?
  bool saveBestFit = false;
  if (xVal > 0) saveBestFit = false;
  if (isSinglePoint) saveBestFit = true;

  // One man's continuing struggle with hacky fixes stemming from the
  // oscillation probability being buggy as hell
  if (plotVars.find("dmsq32") != std::string::npos)
    RemovePars(oscVars, {"dmsq32NHscaled", "dmsq32IHscaled"});

  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();

  FitTreeBlob asimov_tree("asimov_tree", "meta_tree");
  asimov_tree.SetDirectory(fout);

  FitTreeBlob global_tree("global_tree", "global_meta_tree");
  global_tree.SetDirectory(fout);

  // This remains the same throughout... there is one true parameter set for
  // this Asimov set
  osc::IOscCalculatorAdjustable *trueOsc = NuFitOscCalc(hie, 1, asimov_set);

  // Save the true value
  if (saveBestFit){
    TGraph *true_point = GetFitPoint(plotVarVect, trueOsc);
    true_point->Write("true_point");
    delete true_point;
  }

  // Save the true information
  SaveTrueOAParams(fout, trueOsc);

  // Start by performing a minimization across the whole space, this defines the
  // minimum chi2!
  osc::IOscCalculatorAdjustable *testOsc = NuFitOscCalc(hie, 1, asimov_set);

  IExperiment *penalty_nom = GetPenalty(hie, 1, penaltyString, asimov_set);
  SystShifts testSyst = kNoShift;

  // For the nominal, try all octant/dCP combos (shouldn't get it wrong)
  std::map<const IFitVar *, std::vector<double>> oscSeedsAll = {};
  oscSeedsAll[&kFitSinSqTheta23] = {.4, .6};
  oscSeedsAll[&kFitDeltaInPiUnits] = {-0.66, 0, 0.66};

  // For the asimov, seed whatever isn't fixed
  std::map<const IFitVar *, std::vector<double>> oscSeeds = {};
  if (std::find(plotVarVect.begin(), plotVarVect.end(), "deltapi") ==
      plotVarVect.end())
    oscSeeds[&kFitDeltaInPiUnits] = {-0.66, 0, 0.66};
  if (std::find(plotVarVect.begin(), plotVarVect.end(), "ssth23") ==
      plotVarVect.end())
    oscSeeds[&kFitSinSqTheta23] = {.4, .6};

  // Only do this if you're told to?
  double globalmin = 0;

  if (isGlobal) {
    globalmin = RunFitPoint(stateFname, sampleString, trueOsc, trueSyst, false,
                            oscVarsAll, systlist, testOsc, testSyst,
                            oscSeedsAll, penalty_nom, Fitter::kNormal, nullptr, &global_tree);
    global_tree.Fill();
    // Save this info
    if (saveBestFit){
      fout->cd();
      TGraph *best_fit = GetFitPoint(plotVarVect, testOsc);
      best_fit->Write("best_fit");
      delete best_fit;
    }
  }
  delete penalty_nom;
  delete testOsc;
  std::cout << "Found a minimum global chi2 of: " << globalmin << std::endl;

  // Need to set up the histogram to fill
  TH1 *sens_hist = GetAsimovHist(plotVarVect, trueOsc);
  if (!sens_hist) {
    std::cout << "ERROR: sens_hist not correctly produced!" << std::endl;
    abort();
  }

  int xMin = 0;
  int xMax = sens_hist->GetNbinsX();

  if (xVal >= 0) {
    xMin = xVal;
    xMax = xVal + 1;
  }

  int yMin = 0;
  int yMax = sens_hist->GetNbinsY();

  if (yVal >= 0) {
    yMin = yVal;
    yMax = yVal + 1;
  }

  // Loop over whatever bins you're supposed to
  for (int xBin = xMin; xBin < xMax; ++xBin) {
    double xCenter = sens_hist->GetXaxis()->GetBinCenter(xBin + 1);

    for (int yBin = yMin; yBin < yMax; ++yBin) {
      double yCenter = sens_hist->GetYaxis()->GetBinCenter(yBin + 1);

      osc::IOscCalculatorAdjustable *testOsc = NuFitOscCalc(hie, 1, asimov_set);
      if (plotVarVect.size() > 0)
        SetOscillationParameter(testOsc, plotVarVect[0], xCenter, hie);
      if (plotVarVect.size() > 1)
        SetOscillationParameter(testOsc, plotVarVect[1], yCenter, hie);

      IExperiment *penalty = GetPenalty(hie, 1, penaltyString, asimov_set);

      double chisqmin =
          RunFitPoint(stateFname, sampleString, trueOsc, trueSyst, false,
                      oscVars, systlist, testOsc, testSyst, oscSeeds, penalty,
                      Fitter::kNormal, nullptr, &asimov_tree);

      // Save relevant values into the tree and histogram
      double chisqdiff = chisqmin - globalmin;
      asimov_tree.throw_tree->Branch("chisqmin", &chisqmin);
      asimov_tree.throw_tree->Branch("chisqdiff", &chisqdiff);
      asimov_tree.throw_tree->Branch("globalmin", &globalmin);
      asimov_tree.throw_tree->Branch("xVal", &xCenter);
      asimov_tree.throw_tree->Branch("xName", &plotVarVect[0]);
      asimov_tree.throw_tree->Branch("isGlobal", &isGlobal);
      if (plotVarVect.size() > 1)
        asimov_tree.throw_tree->Branch("yVal", &yCenter);
      if (plotVarVect.size() > 1)
        asimov_tree.throw_tree->Branch("yName", &plotVarVect[1]);
      asimov_tree.Fill();
      sens_hist->SetBinContent(xBin + 1, yBin + 1, chisqdiff);

      delete penalty;
      delete testOsc;

      if (isSinglePoint) { // exit early if we are only running 1 point
        break;
      }
    }
    if (isGlobal &&
        isSinglePoint) { // exit early if we are only running 1 point
      break;
    }
  }

  // Save the histogram, and do something sensible with the name
  fout->cd();
  asimov_tree.Write();
  global_tree.Write();
  sens_hist->Write();
  fout->Write();
  fout->Close();
}

#ifndef __CINT__
int main(int argc, char const *argv[]) {

  gROOT->SetMustClean(false);

  std::string stateFname = (argc > 1) ? argv[1] : def_stateFname;
  std::string outputFname = (argc > 2) ? argv[2] : def_outputFname;
  std::string plotVars = (argc > 3) ? argv[3] : def_plotVars;
  std::string systSet = (argc > 4) ? argv[4] : def_systSet;
  std::string sampleString = (argc > 5) ? argv[5] : def_sampleString;
  std::string penaltyString = (argc > 6) ? argv[6] : def_penaltyString;
  int hie = (argc > 7) ? atoi(argv[7]) : def_hie;
  std::string asimov_set = (argc > 8) ? argv[8] : def_asimov_set;
  std::string fakeDataShift = (argc > 9) ? argv[9] : def_fakeDataShift;
  int fitBias = (argc > 10) ? atoi(argv[10]) : def_fitBias;

  asimov_joint(stateFname, outputFname, plotVars, systSet, sampleString,
               penaltyString, hie, asimov_set, fakeDataShift, fitBias);
}
#endif
