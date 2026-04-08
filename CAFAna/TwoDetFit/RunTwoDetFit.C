////////////////////////////////////////////////////////////////////////
//  Script to run a DUNE Two-Detector Fit. Initially 3F, soon w/ BSM  //
//                                                                    //
//  Luiz Prais - praislo@ucmail.uc.edu                                //
////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Fit/FrequentistSurface.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Vars/FitVars.h"
#include "CAFAna/Analysis/Calcs.h"

using namespace ana;

void RunTwoDetFit( std::string variable  = "prof_deltaCP"
			  )
{

 // create a root file to store contours
 TFile* RootFile;
 RootFile = new TFile( "./test-fit_NO.root", "recreate" );


 // Load predictions
 std::unique_ptr<ana::IPrediction> pred = ana::LoadFromFile<ana::PredictionNoExtrap>("my_no_extrap_pred.root",
		                                                                             "pred");

 // osc calculator. Will need a switch to BSM versions
 auto calc = DefaultOscCalc();

 // fake data file
 std::unique_ptr<TFile> fdFile(TFile::Open("MakeFakeData.root"));
 std::unique_ptr<ana::Spectrum> fakedata = ana::LoadFrom<ana::Spectrum>(fdFile.get(),
		                                                               ("fake-data"));

 // construct experiment
 ana::SingleSampleExperiment expt(pred.get(), *fakedata);		
 
 // Fit!
 ana::FrequentistSurface surface(&expt, calc,
		                         &ana::kFitDeltaInPiUnits, 50, 0.0, 2.0,
		                         &ana::kFitSinSq2Theta23, 50, 0.5, 1.0);

 RootFile->cd();  
 surface.SaveTo(RootFile, std::string("surface"));
 RootFile->Close();

 std::cout << "Best fit point: (" << surface.GetBestFitX() << ", " << surface.GetBestFitY() << ")\n";
 std::cout << "Truth point: (" << ana::kFitDeltaInPiUnits.GetValue(calc) << ", " << ana::kFitSinSq2Theta23.GetValue(calc) << ")\n";

 std::cout << "\nDone with the fit!";

}  