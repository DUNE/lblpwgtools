////////////////////////////////////////////////////////////////////////
//  Script to run a DUNE Two-Detector Fit. Initially 3F, soon w/ BSM  //
//                                                                    //
//  Luiz Prais - praislo@ucmail.uc.edu                                //
//  Silvio Motta - smotta93.fis@gmail.com                             //
////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Fit/FrequentistSurface.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Vars/FitVars.h"
#include "CAFAna/Analysis/Calcs.h"
#include "OscLib/IOscCalc.h"
#include "CalcsTwoDetFit.h"

#include "FitUtils.h"

using namespace ana;
using namespace FitUtils;

void RunTwoDetFit( std::string Variable    = "surf_ssth23_deltaCP",
                   TString     FitOptions  = "onlyNO_UOLO",
                   std::string Model       = "simple"       // "3Flavor", "nsi", "simple"
                 )
{
  
  TString TagName = Variable + "_" + FitOptions + "_" + Model + ".root";

  // create a root file to store contours
  TFile* RootFile;
  RootFile = new TFile(TagName, "recreate" );

  // Load predictions
  std::unique_ptr<ana::IPrediction> pred = ana::LoadFromFile<ana::PredictionNoExtrap>("pred.root",
                                                                                      "pred");

  // osc calculator. Will need a switch to BSM versions
  //auto calc = DefaultOscCalc();

  // fake data file
  std::unique_ptr<TFile> fdFile(TFile::Open("fakeData.root"));
  std::unique_ptr<ana::Spectrum> fakedata = ana::LoadFrom<ana::Spectrum>(fdFile.get(), "fake-data");

  // while tmp using the lbl-extrap files as placeholders, better to pull the calc from those files anyway
  std::unique_ptr<osc::IOscCalc> c = ana::LoadFrom<osc::IOscCalc>(fdFile.get(), "calc");
  auto calc = dynamic_cast<osc::IOscCalcAdjustable*>(c.get());

  // overwriting for NSI, but need a better mechanism to switch calcs
  if (Model == "nsi") {
    std::cout << "\nSelected NSI model, switching to NSI calculator" << std::endl;
    calc = DefaultOscCalcNSI();
  }

  PrintOscCalc(calc);

  // construct experiment
  ana::SingleSampleExperiment expt(pred.get(), *fakedata);
  
  int Xbins = 30, Ybins = 30;


  //#*******************************************************************#
  //#                                                                   #
  //#                          SURFACE                                  #
  //#                                                                   #
  //#*******************************************************************#
  FitUtils::SetVariableType(Variable, FitOptions);
  FitUtils::SetModelType(Model);

  ana::FrequentistSurface surface(&expt, calc,
                                  VarX, Xbins, xmin, xmax,
                                  VarY, Ybins, ymin, ymax,
                                  VarsToFit);

  RootFile->cd();
  surface.SaveTo(RootFile, std::string("surface"));
  RootFile->Close();

  std::cout << "Best fit point: (" << surface.GetBestFitX() << ", " << surface.GetBestFitY() << ")\n";
  std::cout << "Truth point: ("    << VarX->GetValue(calc)  << ", " << VarY->GetValue(calc)  << ")\n";

  std::cout << "\nDone with the fit!" << std::endl;

}
