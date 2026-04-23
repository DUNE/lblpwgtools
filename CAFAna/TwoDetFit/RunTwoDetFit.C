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

using namespace ana;

void RunTwoDetFit( std::string variable    = "surf_ssth23_deltaCP",
                   TString     exp_options = "onlyNO_UOLO",  // RunJoint line 119
                   std::string model       = "simple"       // "3Flavor" or "simple" (no profiling)
                 )
{
  
  TString TagName = variable + "_" + exp_options + "_" + model + ".root";

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

  // construct experiment
  ana::SingleSampleExperiment expt(pred.get(), *fakedata);

  //#*******************************************************************#
  //#                    A few general settings                         #
  //#*******************************************************************#  
  double sinmax   = 0.65, sinmin  = 0.35; // set fitting limits for ssth23 
  double deltamax = 2.8, deltamin = 2.2; //  set fitting limits for dmsq32

  //------------------------------------------------------------------//
  //               Define surface axes from variable string           //
  //------------------------------------------------------------------//
  // Reproduces RunJointFit2024.C lines 390-523

  const ana::IFitVar* varX = nullptr;
  const ana::IFitVar* varY = nullptr;
  double xmin, xmax, ymin, ymax;
  int Xsteps = 30, Ysteps = 30;

  bool understood_variable = false;
  bool MakeSurface         = false;

  //#*******************************#
  //#  Select mass ordering space   #
  //#*******************************#

  int  fitdmsq32;
  const ana::IFitVar* dmsq32; // define a general dmsq32 var, split later

  bool onlyNO   = exp_options.Contains("onlyNO");
  bool onlyIO   = exp_options.Contains("onlyIO");
  bool bothNOIO = exp_options.Contains("NOIO");

  bool understood_mass_ordering = false;
  if (onlyNO){
      std::cout<<"\nSelected NO"
               <<"\nThis will fit for DmSq32ScaledNH" << std::endl;
      fitdmsq32 = 1;
      dmsq32 = &kFitDmSq32NHScaled;
      understood_mass_ordering = true;
  }
  
  if (onlyIO){
      std::cout<<"\nSelected IO"
               <<"\nThis will fit for DmSq32ScaledIH" << std::endl;
      fitdmsq32 = -1;
      dmsq32 = &kFitDmSq32IHScaled;
      deltamax = -2.25, deltamin = -2.85; 
      understood_mass_ordering = true;

    //   if(RHCOnly){
    //    deltamax = -2.00, deltamin = -3.00; 
    //   }
    //   // nue only needs more range
    //   if(nueOnly){
    //    deltamax = -1.2, deltamin = -3.00; 
    //   }  
  }

  // there's the case where we want both NO and IO together
  if (bothNOIO){
      std::cout<<"\nSelected here to account both NO and IO in the same FitVar"
               <<"\nThis will fit for DmSq32Scaled" << std::endl;
      fitdmsq32 = 0;
      dmsq32 = &kFitDmSq32Scaled; 
      deltamax = 2.2, deltamin = -2.65; 
      understood_mass_ordering = true;
  }

  if (!understood_mass_ordering) {
  std::cout<<"\nCouldn't select between NO or IO "<<std::endl;
  exit(1);
  }

  //#*******************************#
  //#     Select octant space       #
  //#*******************************#

  int  fitssth23;
  const ana::IFitVar* ssth23; // define a general ssth23 var, split later

  bool onlyUO   = exp_options.Contains("onlyUO");
  bool onlyLO   = exp_options.Contains("onlyLO");
  bool bothUOLO = exp_options.Contains("UOLO");

  // this is not needed for e.g. 2D contours, since we have both octants displayed
  if((variable == "surf_ssth23_deltaCP" || 
      variable == "surf_ssth23_dmsq32"  ||
      variable == "prof_ssth23")        &&
      (onlyUO || onlyLO)){

     std::cout << "\nDetected octant splitting in exp_options = " << exp_options
               << "\nThis is not applicable to " << variable 
               << "\nReverting this choice and proceeding. No loss of generality."
               << std::endl;

     bothUOLO = true;
     onlyUO   = false;
     onlyLO   = false;

     }

  bool understood_octant = false;

  if (onlyUO){
      std::cout << "\nSelected UO"
                << "\nThis will fit for SinSqTheta23UpperOctant"
                << std::endl;
      fitssth23 = 1;
      ssth23    = &kFitSinSqTheta23UpperOctant;
      understood_octant = true;
  }
  
  if (onlyLO){
      std::cout << "\nSelected LO"
                << "\nThis will fit for SinSqTheta23LowerOctant"
                << std::endl;
      fitssth23 = -1;
      ssth23    = &kFitSinSqTheta23LowerOctant;
      understood_octant = true;
  }

  // there's the case where we want both UO and LO together (e.g. 2D plots):
  if (bothUOLO){
      std::cout << "\nSelected here to account both UO and LO in the same FitVar"
                << "\nThis will fit for SinSqTheta23"
                << std::endl;
      fitssth23 = 0;
      ssth23    = &kFitSinSqTheta23; 
      understood_octant = true;
  }


  if (!understood_octant) {
  std::cout<<"\nCouldn't select between UO or LO "<<std::endl;
  exit(1);
  }


   ////////////////////////////////////////////////////////////////////
  //                       2D Surfaces                              //
  ////////////////////////////////////////////////////////////////////

  //#*******************************#
  //#         ssth23_deltaCP        #
  //#*******************************#

  if (variable == "surf_ssth23_deltaCP") {
    varX = &kFitDeltaInPiUnits;
    xmin = 0.0; xmax = 2.0;
    varY = &kFitSinSqTheta23;
    ymin = sinmin; ymax = sinmax;
    MakeSurface = true;
    understood_variable = true;
  }

  //#*******************************#
  //#         ssth23_dmsq32         #
  //#*******************************#

  
  if (variable == "surf_ssth23_dmsq32") {
    
    varX = &kFitSinSqTheta23;
    xmin = sinmin; xmax = sinmax;
    varY = dmsq32;
    ymin = deltamin; ymax = deltamax;
    MakeSurface = true;
    understood_variable = true;

  }


// when an above is selected, proceed
  if (!understood_variable) {
    std::cout<<"Didn't understand variable: "<<variable<<std::endl;
    exit(1);
  }


  //------------------------------------------------------------------//
  //               Define parameters to marginalize over              //
  //------------------------------------------------------------------//
  // Reproduces the 3Flavor logic from RunJointFit2024.C lines 541-562.
  // If model == "simple", skip profiling entirely (pass empty vector).

  std::vector<const ana::IFitVar*> vars_to_fit;

  bool understood_model = false;

  if (model == "3Flavor"){

      // Reactor angle theta13 — RunJoint line 544
      if (varX != &ana::kFitSinSq2Theta13 && varY != &ana::kFitSinSq2Theta13)
          vars_to_fit.push_back(&ana::kFitSinSq2Theta13);

      // CP phase delta — RunJoint line 545
      if (varX != &ana::kFitDeltaInPiUnits && varY != &ana::kFitDeltaInPiUnits)
          vars_to_fit.push_back(&ana::kFitDeltaInPiUnits);

      // Atmospheric mass splitting — RunJoint lines 548-550
      if (fitdmsq32 ==  0 && varX != &ana::kFitDmSq32Scaled   && varY != &ana::kFitDmSq32Scaled)
          vars_to_fit.push_back(&ana::kFitDmSq32Scaled);
      if (fitdmsq32 ==  1 && varX != &ana::kFitDmSq32NHScaled && varY != &ana::kFitDmSq32NHScaled)
          vars_to_fit.push_back(&ana::kFitDmSq32NHScaled);
      if (fitdmsq32 == -1 && varX != &ana::kFitDmSq32IHScaled && varY != &ana::kFitDmSq32IHScaled)
          vars_to_fit.push_back(&ana::kFitDmSq32IHScaled);

      // Atmospheric angle theta23 — RunJoint lines 553-555
      if (fitssth23 ==  0 && varX != &ana::kFitSinSqTheta23            && varY != &ana::kFitSinSqTheta23)
          vars_to_fit.push_back(&ana::kFitSinSqTheta23);
      if (fitssth23 ==  1 && varX != &ana::kFitSinSqTheta23UpperOctant && varY != &ana::kFitSinSqTheta23UpperOctant)
          vars_to_fit.push_back(&ana::kFitSinSqTheta23UpperOctant);
      if (fitssth23 == -1 && varX != &ana::kFitSinSqTheta23LowerOctant && varY != &ana::kFitSinSqTheta23LowerOctant)
          vars_to_fit.push_back(&ana::kFitSinSqTheta23LowerOctant);

      std::cout << "Using the 3Flavor model with " << vars_to_fit.size() << " nuisance param(s): ";  // RunJoint line 559
      for (auto v : vars_to_fit) std::cout << v->ShortName() << "  ";
      std::cout << std::endl;

      understood_model = true;
  }

  if (model == "simple"){
      std::cout << "Model: simple — no profiling" << std::endl;
      understood_model = true;
  }

  if (!understood_model){
      std::cout << "ERROR: didn't understand model: " << model << std::endl;
      return;
  }

  //#*******************************************************************#
  //#                                                                   #
  //#                          SURFACE                                  #
  //#                                                                   #
  //#*******************************************************************#
  ana::FrequentistSurface surface(&expt, calc,
                                  varX, Xsteps, xmin, xmax,
                                  varY, Ysteps, ymin, ymax,
                                  vars_to_fit);

  RootFile->cd();
  surface.SaveTo(RootFile, std::string("surface"));
  RootFile->Close();

  std::cout << "Best fit point: (" << surface.GetBestFitX() << ", " << surface.GetBestFitY() << ")\n";
  std::cout << "Truth point: ("    << varX->GetValue(calc)  << ", " << varY->GetValue(calc)  << ")\n";

  std::cout << "\nDone with the fit!" << std::endl;

}
