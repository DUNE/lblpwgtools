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
#include "CAFAna/Analysis/Calcs.h"
#include "OscLib/IOscCalc.h"

using namespace ana;

namespace FitUtils {


 // Osc parameters to be display and fit
 const ana::IFitVar* VarX = nullptr;
 const ana::IFitVar* VarY = nullptr;

 // Fit limits
 double xmin, xmax, ymin, ymax;

 // Osc parameters to be fittted/profiled over
 std::vector<const ana::IFitVar*> VarsToFit;

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
   ymin = 2.2; ymax = 2.8;
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
   ymin = 0; ymax = 2;
   UnderstoodVariable = true;

  }

 //#*******************************#
 //#       epsetau_deltaetau       #
 //#*******************************#

  if (Variable == "surf_epsetau_deltaetau") {

   VarX = &kFitDeltaETauInPiUnits;
   xmin = 0; xmax = 2;
   VarY = &kFitEpsETau;
   ymin = 0; ymax = 2;
   UnderstoodVariable = true;

  }

 //#*******************************#
 //#     epsmutau_deltamutau       #
 //#*******************************#

  if (Variable == "surf_epsmutau_deltamutau") {

   VarX = &kFitDeltaMuTauInPiUnits;
   xmin = 0; xmax = 2;
   VarY = &kFitEpsMuTau;
   ymin = 0; ymax = 2;
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

 // all BSM models need the PMNS parameters anyway...
 if (model == "3Flavor" || model == "nsi"){

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

      std::cout << "Using the 3Flavor model with " << VarsToFit.size() << " nuisance param(s): ";  // RunJoint line 559
      for (auto v : VarsToFit) std::cout << v->ShortName() << "  ";
      std::cout << std::endl;

      UnderstoodModel = true;
  }

  if (model == "nsi") {
      std::cout << "Selected the NSI model, so pulling epsilons and deltas...<ToDo>" << std::endl;
      // hacking here the VarsToFit to just do nothing rn for quick tests
      VarsToFit = {};
      UnderstoodModel = true;
  }

  // for cases where we skip profiling entirely (pass empty vector).
  if (model == "simple"){
      std::cout << "Model: simple — no profiling" << std::endl;
      UnderstoodModel = true;
  }

  if (!UnderstoodModel){
      std::cout << "ERROR: didn't understand model: " << model << std::endl;
      exit(1);
  }

} // end SetModelType    

} // end namespace FitUtils    
