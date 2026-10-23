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

#include <random> 

using namespace ana;

namespace FitUtils {

 // Seeding mechanism. Samples a random mix of seeds, but caps the max number so the fit doesn't blow up in time
 SeedList GetSeedList(std::map<const IFitVar*, std::vector<double>> seeds_to_use, size_t n_seeds, int random_seed = 54321)
 {
  
  std::default_random_engine generator(random_seed);
  std::vector<Seed> v;
  std::vector<int> quick_hash;
  
  
  const int MaxInterations = n_seeds * 5;
  int iter = 0;
  while (v.size() < n_seeds && iter < MaxInterations) {
    
    std::map<const IFitVar*, std::vector<double>> candidate_seed_map;
    int hash = 0;
    int offset = 0;
    for (auto it : seeds_to_use) {
      size_t n_possible_seeds = it.second.size();
      std::uniform_int_distribution<int> distribution(0,n_possible_seeds-1);
      int seed_index = distribution(generator);
      hash += seed_index * offset;
      offset += n_seeds;
      double seed_value = it.second[seed_index];
      std::vector<double> foo;
      foo.push_back(seed_value);
      candidate_seed_map[it.first] = foo; 
    }
    // Only add a seed if it's not already in the list
    if (std::find(quick_hash.begin(), quick_hash.end(), hash) == quick_hash.end()) {
      SeedList* temp = new SeedList(candidate_seed_map);
      Seed bar = temp->GetSeeds()[0];
      v.push_back(bar);
      quick_hash.push_back(hash);
    }
    iter += 1;
  }
  SeedList out(v);
  
  return out;
 } // end GetSeedList


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

 // Seeds to use for the fits
 std::map<const IFitVar*, std::vector<double>> SeedsToUse;

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
 RequiredCalc = "nsi"; //3Flavor

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

inline void SetModelType(const std::string model, const std::string Variable) {

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

      if (model == "nsi"){
        std::cout << "Selected the NSI model with a PMNS parameter space. This will add NSI parameters to the fit" << std::endl;

        // emu sector
        /*
        if (Variable.find("emu") != std::string::npos) {
         if (VarX != &kFitEpsEMu && VarY != &kFitEpsEMu) VarsToFit.push_back(&kFitEpsEMu);
         if (VarX != &kFitDeltaEMuInPiUnits && VarY != &kFitDeltaEMuInPiUnits)
          VarsToFit.push_back(&kFitDeltaEMuInPiUnits);   
        }
        */

        // actually testing the full NSI model here!
        if (VarX != &kFitEpsEMu && VarY != &kFitEpsEMu) VarsToFit.push_back(&kFitEpsEMu);
        if (VarX != &kFitDeltaEMuInPiUnits && VarY != &kFitDeltaEMuInPiUnits)
         VarsToFit.push_back(&kFitDeltaEMuInPiUnits);

        if (VarX != &kFitEpsETau && VarY != &kFitEpsETau) VarsToFit.push_back(&kFitEpsETau);
        if (VarX != &kFitDeltaETauInPiUnits && VarY != &kFitDeltaETauInPiUnits)
         VarsToFit.push_back(&kFitDeltaETauInPiUnits);

        if (VarX != &kFitEpsMuTau && VarY != &kFitEpsMuTau) VarsToFit.push_back(&kFitEpsMuTau);
        if (VarX != &kFitDeltaMuTauInPiUnits && VarY != &kFitDeltaMuTauInPiUnits)
         VarsToFit.push_back(&kFitDeltaMuTauInPiUnits); 
      
      }
          

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

//#*******************************************************************#
//#                                                                   #
//#                               Seeds                               #
//#                                                                   #
//#*******************************************************************#


 //#*******************************#
 //#            3Flavor            #
 //#*******************************#
  const std::vector<double> dmsq32_seedsNO   = { 2.35,  2.55};
  const std::vector<double> dmsq32_seedsIO   = {-2.35, -2.55};
  const std::vector<double> dmsq32_seeds     = {-2.44,  2.41};
  const std::vector<double> th23_seeds       = {0.45, 0.50, 0.55};
  const std::vector<double> th23_seedsUO     = {0.55, 0.60};
  const std::vector<double> th23_seedsLO     = {0.40, 0.45};
  const std::vector<double> th13_seeds       = {0.0822, 0.0868}; // +/- 1-sigma from PDG 2025 ss2th13 = 0.0845+/-0.0023
  const std::vector<double> deltaCP_seeds    = {0, 0.5, 1., 1.5};


  // ssth23 vars
  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitSinSqTheta23) != VarsToFit.end()) 
    SeedsToUse[&kFitSinSqTheta23] = th23_seeds;
  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitSinSqTheta23UpperOctant) != VarsToFit.end()) 
    SeedsToUse[&kFitSinSqTheta23UpperOctant] = th23_seedsUO;
  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitSinSqTheta23LowerOctant) != VarsToFit.end()) 
    SeedsToUse[&kFitSinSqTheta23LowerOctant] = th23_seedsLO;
  
  // reactor angle
  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitSinSq2Theta13) != VarsToFit.end())  
    SeedsToUse[&kFitSinSq2Theta13] = th13_seeds;

  // dmsq32 vars
  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitDmSq32Scaled) != VarsToFit.end()) 
    SeedsToUse[&kFitDmSq32Scaled] = dmsq32_seeds;
  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitDmSq32NHScaled) != VarsToFit.end()) 
    SeedsToUse[&kFitDmSq32NHScaled] = dmsq32_seedsNO;
  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitDmSq32IHScaled) != VarsToFit.end()) 
    SeedsToUse[&kFitDmSq32IHScaled] = dmsq32_seedsIO;

  // deltaCP
  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitDeltaInPiUnits) != VarsToFit.end())
    SeedsToUse[&kFitDeltaInPiUnits] = deltaCP_seeds;

 //#*******************************#
 //#             NSI               #
 //#*******************************#

 const std::vector<double> nsi_seeds      = {0.02, 0.15, 0.2};
 const std::vector<double> deltaNSI_seeds = {0, 0.5, 1., 1.5};

 // Off-diagonal NSI
  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitEpsETau) != VarsToFit.end()) 
    SeedsToUse[&kFitEpsETau] = nsi_seeds;
  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitEpsMuTau) != VarsToFit.end()) 
    SeedsToUse[&kFitEpsMuTau] = nsi_seeds;
  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitEpsEMu) != VarsToFit.end()) 
    SeedsToUse[&kFitEpsEMu] = nsi_seeds;

  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitDeltaETauInPiUnits) != VarsToFit.end()) 
    SeedsToUse[&kFitDeltaETauInPiUnits] = deltaNSI_seeds;
  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitDeltaMuTauInPiUnits) != VarsToFit.end()) 
    SeedsToUse[&kFitDeltaMuTauInPiUnits] = deltaNSI_seeds;
  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitDeltaEMuInPiUnits) != VarsToFit.end()) 
    SeedsToUse[&kFitDeltaEMuInPiUnits] = deltaNSI_seeds;



 //#*******************************#
 //#            sterile            #
 //#*******************************#

  const std::vector<double> ssth13_seeds_sterile    = {0.0210, 0.0222};
  const std::vector<double> dmsq32_seedsNO_sterile  = { 2.35e-3,  2.55e-3};
  const std::vector<double> dmsq32_seedsIO_sterile  = {-2.35e-3, -2.55e-3};
  const std::vector<double> dmsq32_seeds_sterile    = {-2.44e-3,  2.41e-3};
  const std::vector<double> th23_seeds_sterile      = {0.45, 0.50, 0.55};
  const std::vector<double> delta13_seeds_sterile   = {0, 0.5, 1., 1.5};

  // sterile vars are named differently, so...

  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitSinSqTheta13Sterile) != VarsToFit.end())
    SeedsToUse[&kFitSinSqTheta13Sterile] = ssth13_seeds_sterile;

  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitDelta13InPiUnitsSterile) != VarsToFit.end())
    SeedsToUse[&kFitDelta13InPiUnitsSterile] = delta13_seeds_sterile;

  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitDmSq32Sterile) != VarsToFit.end())
    SeedsToUse[&kFitDmSq32Sterile] = dmsq32_seeds_sterile;
  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitDmSq32NHSterile) != VarsToFit.end())
    SeedsToUse[&kFitDmSq32NHSterile] = dmsq32_seedsNO_sterile;
  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitDmSq32IHSterile) != VarsToFit.end())
    SeedsToUse[&kFitDmSq32IHSterile] = dmsq32_seedsIO_sterile;

  if (std::find(VarsToFit.begin(), VarsToFit.end(), &kFitSinSqTheta23Sterile) != VarsToFit.end())
    SeedsToUse[&kFitSinSqTheta23Sterile] = th23_seeds_sterile;

  // Now map out those seeds to go
  size_t n_seeds = 1;
  for (auto var : SeedsToUse) {
    n_seeds *= var.second.size();
  }
  std::cout<<"Total number of possible seeds: "<<n_seeds<<std::endl;
  
  const int MAX_N_SEEDS = 500;
  SeedList seedlist;
  if (n_seeds > MAX_N_SEEDS * 5) seedlist = GetSeedList(SeedsToUse, MAX_N_SEEDS);
  else seedlist = SeedList(SeedsToUse);
  std::cout<<"Total number of seeds in seedlist: "<<seedlist.GetSeeds().size()<<std::endl;  

} // end SetModelType

} // end namespace FitUtils    
