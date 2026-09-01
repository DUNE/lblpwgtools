////////////////////////////////////////////////////////////////////////
//  Fit utilities: model, variables, and alikes                       //
//                                                                    //
//  Luiz Prais - praislo@ucmail.uc.edu                                //
////////////////////////////////////////////////////////////////////////

// Todo: see which can remove

#include "CAFAna/Analysis/Style.h"
#include "CAFAna/Core/rootlogon.C"
#include "CAFAna/Fit/FrequentistSurface.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Fit/FrequentistSurface.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Vars/FitVars.h"
#include "CAFAna/Vars/FitVarsNSI.h"
#include "CAFAna/Vars/FitVarsSterile.h"
#include "CAFAna/Analysis/Calcs.h"
#include "OscLib/IOscCalc.h"

using namespace ana;

namespace FitUtils {


 // Osc parameters to be display and fit
 const ana::IFitVar* VarX = nullptr;
 const ana::IFitVar* VarY = nullptr;

 // Parameter space binning
 int Xbins = 30, Ybins = 30;

 // Fit limits
 double xmin, xmax, ymin, ymax;

 // Needed for sterile spaces
 bool IsLogX = false;
 bool IsLogY = false;

 // Osc parameters to be fittted/profiled over
 std::vector<const ana::IFitVar*> VarsToFit;

 std::string RequiredCalc = "3Flavor";

 // Some globals to switch the mass ordering or the octant of th23
 int  DmSq32Space;
 int  SinSqTh23Space;
 const ana::IFitVar* FitDmSq32;
 const ana::IFitVar* FitSinSqTh23;


 //#*******************************************************************#
 //#                                                                   #
 //#                    Surface to Construct                           #
 //#                                                                   #
 //#*******************************************************************#

inline void SetVariableType(const std::string Variable, const TString  FitOptions ) {

 bool UnderstoodVariable = false;
 bool MakeSurface        = false;

 // Default assumption; overridden below for NSI/sterile variables.
 RequiredCalc = "3Flavor";

 //#*******************************#
 //#  Select mass ordering space   #
 //#*******************************#

 bool onlyNO   = FitOptions.Contains("onlyNO");
 bool onlyIO   = FitOptions.Contains("onlyIO");
 bool bothNOIO = FitOptions.Contains("NOIO");

 bool UnderstoodMassOrdering = false;
 if (onlyNO){

  std::cout << "\nSelected NO"
            <<"\nThis will fit for DmSq32ScaledNH" << std::endl;
  DmSq32Space = 1;
  FitDmSq32 = &kFitDmSq32NHScaled;
  UnderstoodMassOrdering = true;

 }
  
 if (onlyIO){

  std::cout << "\nSelected IO"
            << "\nThis will fit for DmSq32ScaledIH" << std::endl;
  DmSq32Space = -1;
  FitDmSq32 = &kFitDmSq32IHScaled;
  ymax = -2.25, ymin = -2.85; 
  UnderstoodMassOrdering = true;

 }

 // there's the case where we want both NO and IO together
 if (bothNOIO){

 std::cout << "\nSelected here to account both NO and IO in the same FitVar"
           <<"\nThis will fit for DmSq32Scaled" << std::endl;
 DmSq32Space = 0;
 FitDmSq32 = &kFitDmSq32Scaled; 
 ymax = 2.2, ymin = -2.65; 
 UnderstoodMassOrdering = true;
 
 }

 if (!UnderstoodMassOrdering) {

  std::cout<<"\nCouldn't select between NO or IO "<<std::endl;
  exit(1);

 }

 //#*******************************#
 //#     Select octant space       #
 //#*******************************#

 bool onlyUO   = FitOptions.Contains("onlyUO");
 bool onlyLO   = FitOptions.Contains("onlyLO");
 bool bothUOLO = FitOptions.Contains("UOLO");

 // this is not needed for e.g. 2D contours, since we have both octants displayed
 if((Variable == "surf_ssth23_deltaCP" || 
     Variable == "surf_ssth23_dmsq32"  ||
     Variable == "prof_ssth23")        &&
     (onlyUO || onlyLO)){

  std::cout << "\nDetected octant splitting in FitOptions = " << FitOptions
            << "\nThis is not applicable to " << Variable 
            << "\nReverting this choice and proceeding. No loss of generality."
            << std::endl;

  bothUOLO = true;
  onlyUO   = false;
  onlyLO   = false;

 }

 bool UnderstoodOctant = false;

 if (onlyUO){

  std::cout << "\nSelected UO"
            << "\nThis will fit for SinSqTheta23UpperOctant"
            << std::endl;
  SinSqTh23Space = 1;
  FitSinSqTh23    = &kFitSinSqTheta23UpperOctant;
  UnderstoodOctant = true;

 }
  
 if (onlyLO){

  std::cout << "\nSelected LO"
            << "\nThis will fit for SinSqTheta23LowerOctant"
            << std::endl;
  SinSqTh23Space = -1;
  FitSinSqTh23    = &kFitSinSqTheta23LowerOctant;
  UnderstoodOctant = true;

 }

 // there's the case where we want both UO and LO together (e.g. 2D plots):
 if (bothUOLO){

  std::cout << "\nSelected here to account both UO and LO in the same FitVar"
            << "\nThis will fit for SinSqTheta23"
            << std::endl;
  SinSqTh23Space = 0;
  FitSinSqTh23    = &kFitSinSqTheta23; 
  UnderstoodOctant = true;

 }


 if (!UnderstoodOctant) {

  std::cout<<"\nCouldn't select between UO or LO "<<std::endl;
  exit(1);

 }
 
 ////////////////////////////////////////////////////////////////////
 //              1D Profiles - Std Osc 3 Flavor                    //
 ////////////////////////////////////////////////////////////////////

 //#*******************************#
 //#         prof_deltaCP          #
 //#*******************************#  

  if (Variable == "prof_deltaCP") {

   VarX = &kFitDeltaInPiUnits;
   xmin = 0.0; xmax = 2.0;
   VarY = &kFitVarForProf1D;
   ymin = -1; ymax = 1;
   Ybins = 1;
   UnderstoodVariable = true;

  }

 //#*******************************#
 //#         prof_ssth23           #
 //#*******************************#  

  if (Variable == "prof_ssth23") {

   VarX = &kFitSinSqTheta23;
   xmin = 0.25; xmax = 0.75;
   VarY = &kFitVarForProf1D;
   ymin = -1; ymax = 1;
   Ybins = 1;
   UnderstoodVariable = true;

  }

 //#*******************************#
 //#         prof_dmsq32           #
 //#*******************************#  

  if (Variable == "prof_dmsq32") {

   VarX = FitDmSq32;
   xmin = 2.4; xmax = 2.5;
   VarY = &kFitVarForProf1D;
   ymin = -1; ymax = 1;
   Ybins = 1;
   UnderstoodVariable = true;

  }

 ////////////////////////////////////////////////////////////////////
 //               2D Surfaces - Std Osc 3 Flavor                   //
 ////////////////////////////////////////////////////////////////////

 //#*******************************#
 //#         ssth23_deltaCP        #
 //#*******************************#

 if (Variable == "surf_ssth23_deltaCP") {

  VarX = &kFitDeltaInPiUnits;
  xmin = 0.0; xmax = 2.0;
  VarY = &kFitSinSqTheta23;
  ymin = 0.25; ymax = 0.75;
  MakeSurface = true;
  UnderstoodVariable = true;

  }

  //#*******************************#
  //#         ssth23_dmsq32         #
  //#*******************************#

  if (Variable == "surf_ssth23_dmsq32") {
    
   VarX = &kFitSinSqTheta23;
   xmin = 0.25; xmax = 0.75;
   VarY = FitDmSq32;
   ymin = 2.4; ymax = 2.5;
   MakeSurface = true;
   UnderstoodVariable = true;

  }


 ////////////////////////////////////////////////////////////////////
 //                     2D Surfaces - NSI                          //
 ////////////////////////////////////////////////////////////////////

 //#*******************************#
 //#         epsemu_deltaemu       #
 //#*******************************#

  if (Variable == "surf_epsemu_deltaemu") {

   VarX = &kFitDeltaEMuInPiUnits;
   xmin = 0; xmax = 2;
   VarY = &kFitEpsEMu;
   ymin = 0; ymax = 0.5;
   RequiredCalc = "nsi";
   UnderstoodVariable = true;

  }

 //#*******************************#
 //#       epsetau_deltaetau       #
 //#*******************************#

  if (Variable == "surf_epsetau_deltaetau") {

   VarX = &kFitDeltaETauInPiUnits;
   xmin = 0; xmax = 2;
   VarY = &kFitEpsETau;
   ymin = 0; ymax = 0.5;
   RequiredCalc = "nsi";
   UnderstoodVariable = true;

  }

 //#*******************************#
 //#     epsmutau_deltamutau       #
 //#*******************************#

  if (Variable == "surf_epsmutau_deltamutau") {

   VarX = &kFitDeltaMuTauInPiUnits;
   xmin = 0; xmax = 2;
   VarY = &kFitEpsMuTau;
   ymin = 0; ymax = 0.5;
   RequiredCalc = "nsi";
   UnderstoodVariable = true;

  }

 ////////////////////////////////////////////////////////////////////
 //                     1D Profiles - Sterile                      //
 ////////////////////////////////////////////////////////////////////

 //#*******************************#
 //#         prof_ssth24           #
 //#*******************************#  

  if (Variable == "prof_ssth24") {

   VarX = &kFitSinSqTheta24Sterile;
   xmin = 1e-4; xmax = 1.0;
   VarY = &kFitVarForProf1D;
   ymin = -1; ymax = 1;
   Ybins = 1;
   RequiredCalc = "sterile";
   UnderstoodVariable = true;

  }
  
 ////////////////////////////////////////////////////////////////////
 //                     2D Surfaces - Sterile                      //
 ////////////////////////////////////////////////////////////////////

 //#*******************************#
 //#         ssth14_dmsq41         #
 //#*******************************#

  if (Variable == "surf_ssth14_dmsq41") {

   VarX = &kFitSinSqTheta14Sterile;
   xmin = 1e-4; xmax = 1.0;
   VarY = &kFitDmSq41Sterile;
   ymin = 1e-4; ymax = 1e2;
   IsLogX = true;
   IsLogY = true;
   RequiredCalc = "sterile";
   UnderstoodVariable = true;

  }

 //#*******************************#
 //#         ssth24_dmsq41         #
 //#*******************************#

  if (Variable == "surf_ssth24_dmsq41") {

   VarX = &kFitSinSqTheta24Sterile;
   xmin = 1e-4; xmax = 1.0;
   VarY = &kFitDmSq41Sterile;
   ymin = 1e-4; ymax = 1e2;
   IsLogX = true;
   IsLogY = true;
   RequiredCalc = "sterile";
   UnderstoodVariable = true;

  }

 if (!UnderstoodVariable) {

  std::cout << "\nCouldn't understand Variable: " << Variable << std::endl;
  exit(1);

 }

}    

//#*******************************************************************#
//#                                                                   #
//#              Select Model and Parameters to Fit                   #
//#                                                                   #
//#*******************************************************************#

inline void SetModelType(const std::string model) {

 bool UnderstoodModel = false;

 VarsToFit = {};

 // for cases where we skip profiling entirely.
 if (model == "simple") {

      std::cout << "Model: simple — no profiling" << std::endl;
      UnderstoodModel = true;

 }
  else if (model == "3Flavor" || model == "nsi") {

      // all BSM models need the PMNS parameters anyway...
      if (VarX != &ana::kFitSinSqTheta13 && VarY != &ana::kFitSinSqTheta13)
          VarsToFit.push_back(&ana::kFitSinSqTheta13);

      if (VarX != &ana::kFitDeltaInPiUnits && VarY != &ana::kFitDeltaInPiUnits)
          VarsToFit.push_back(&ana::kFitDeltaInPiUnits);

      if (DmSq32Space ==  0 && VarX != &ana::kFitDmSq32Scaled   && VarY != &ana::kFitDmSq32Scaled)
          VarsToFit.push_back(&ana::kFitDmSq32Scaled);
      if (DmSq32Space ==  1 && VarX != &ana::kFitDmSq32NHScaled && VarY != &ana::kFitDmSq32NHScaled)
          VarsToFit.push_back(&ana::kFitDmSq32NHScaled);
      if (DmSq32Space == -1 && VarX != &ana::kFitDmSq32IHScaled && VarY != &ana::kFitDmSq32IHScaled)
          VarsToFit.push_back(&ana::kFitDmSq32IHScaled);

      if (SinSqTh23Space ==  0 && VarX != &ana::kFitSinSqTheta23            && VarY != &ana::kFitSinSqTheta23)
          VarsToFit.push_back(&ana::kFitSinSqTheta23);
      if (SinSqTh23Space ==  1 && VarX != &ana::kFitSinSqTheta23UpperOctant && VarY != &ana::kFitSinSqTheta23UpperOctant)
          VarsToFit.push_back(&ana::kFitSinSqTheta23UpperOctant);
      if (SinSqTh23Space == -1 && VarX != &ana::kFitSinSqTheta23LowerOctant && VarY != &ana::kFitSinSqTheta23LowerOctant)
          VarsToFit.push_back(&ana::kFitSinSqTheta23LowerOctant);

      if (model == "nsi")
          std::cout << "Selected the NSI model, so pulling epsilons and deltas...<ToDo, only PMNS params for now>" << std::endl;

      std::cout << "Using the " << model << " model with " << VarsToFit.size() << " nuisance param(s): ";
      for (auto v : VarsToFit) std::cout << v->ShortName() << "  ";
      std::cout << std::endl;

      UnderstoodModel = true;

  }
   else if (model == "sterile") {

      // steriles have their own PMNS-like vars
      std::cout << "Selected the sterile model, so pulling the other vars to fit..." << std::endl;
      if (VarX != &ana::kFitSinSqTheta13Sterile && VarY != &ana::kFitSinSqTheta13Sterile)
          VarsToFit.push_back(&ana::kFitSinSqTheta13Sterile);

      //if (VarX != &ana::kFitDelta14InPiUnitsSterile && VarY != &ana::kFitDelta14InPiUnitsSterile)
      //    VarsToFit.push_back(&ana::kFitDelta14InPiUnitsSterile);
      if (VarX != &ana::kFitDelta13InPiUnitsSterile && VarY != &ana::kFitDelta13InPiUnitsSterile)
          VarsToFit.push_back(&ana::kFitDelta13InPiUnitsSterile);

      if (DmSq32Space ==  0 && VarX != &ana::kFitDmSq32Sterile   && VarY != &ana::kFitDmSq32Sterile)
          VarsToFit.push_back(&ana::kFitDmSq32Sterile);
      if (DmSq32Space ==  1 && VarX != &ana::kFitDmSq32NHSterile && VarY != &ana::kFitDmSq32NHSterile)
          VarsToFit.push_back(&ana::kFitDmSq32NHSterile);
      if (DmSq32Space == -1 && VarX != &ana::kFitDmSq32IHSterile && VarY != &ana::kFitDmSq32IHSterile)
          VarsToFit.push_back(&ana::kFitDmSq32IHSterile);

      if (SinSqTh23Space ==  0 && VarX != &ana::kFitSinSqTheta23Sterile  && VarY != &ana::kFitSinSqTheta23Sterile)
          VarsToFit.push_back(&ana::kFitSinSqTheta23Sterile);

      std::cout << "Using the sterile model with " << VarsToFit.size() << " nuisance param(s): ";
      for (auto v : VarsToFit) std::cout << v->ShortName() << "  ";
      std::cout << std::endl;

      UnderstoodModel = true;

 }

  if (!UnderstoodModel){
      std::cout << "ERROR: didn't understand model: " << model << std::endl;
      exit(1);
  }

} // end SetModelType

} // end namespace FitUtils    
