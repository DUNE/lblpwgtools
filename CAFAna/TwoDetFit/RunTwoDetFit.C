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
#include "CAFAna/Experiment/MultiExperiment.h"
#include "CAFAna/Fit/FrequentistSurface.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Vars/FitVars.h"
#include "CAFAna/Analysis/Calcs.h"
#include "CAFAna/Analysis/CalcsNuFit.h"
#include "OscLib/IOscCalc.h"
#include "CalcsTwoDetFit.h"
#include "CAFAna/Experiment/ReactorExperiment.h"

#include "FitUtils.h"

using namespace ana;
using namespace FitUtils;
using namespace std;

// All of this must go on externals!

const ReactorExperiment* Th13ConstraintPDG2025()
{
    // https://pdg.lbl.gov/2025/web/viewer.html?file=../tables/rpp2025-sum-leptons.pdf
    // This annoying, but to use this func one must convert
    // ssth13=0.0216+/-0.0006 -> ss2th13=0.0845+/-0.0023
    return new ReactorExperiment(0.0845, 0.0023);
}  

  struct predictions {
        string name = "";
        const IPrediction * pred = 0;
        std::pair <Spectrum*, double> cos = {0,0};
        double pot = 0;
        double livetime = 0;
  };

SingleSampleExperiment* PackExperiment(predictions Preds, Spectrum* DataToUse, osc::IOscCalcAdjustable* calc) {
   
    bool PoissonError  = true;
    double POT = Preds.pot;
    std::string name = Preds.name;
    std::cout<<"\nSingle sample experiment " << name << " made with " << " POT " << POT << " tot MC " << Preds.pred->Predict(calc).Integral(POT) << " analyze data " << DataToUse->Integral(DataToUse->POT()) << std::endl;
    return new SingleSampleExperiment(Preds.pred, *DataToUse); 
}

Spectrum * GetFakeData(const IPrediction * pred,
			     osc::IOscCalc * calc,
			     const double pot,
			     const Spectrum * cosmics = 0,
			     const double livetime = 0 
  )
{
  //Total prediction at oscillation params in calc, 
  //scaled to pot; need livetime to get scale cosmics correctly
  auto ret = new Spectrum(pred->Predict(calc).FakeData(pot, livetime));
  if(cosmics) *ret += *cosmics;
  return ret;
}

void RunTwoDetFit( std::string Variable    = "surf_ssth23_deltaCP",
                   TString     FitOptions  = "onlyNO_UOLO",
                   std::string Model       = "simple"       // "3Flavor", "nsi", "simple"
                 )
{

  // Lets tmp define the POT exposure here, eventually placing it elsewhere
  // 7 years staged for now..
  const double kFD3p5yrFHCPOT = 1.36283e+23;  // from TDR-era studies: 3.5 * (1.1e20) * 40 / 1.13
  const double kFD3p5yrRHCPOT = 1.36283e+23;
  
  TString TagName = Variable + "_" + FitOptions + "_" + Model + ".root";

  // create a root file to store contours
  TFile* RootFile;
  RootFile = new TFile(TagName, "recreate" );

  auto calc = NuFitOscCalc(1);
  ResetOscCalcToAsimovPoint("pdg2025", calc);

  // overwriting for NSI, but need a better mechanism to switch calcs
  if (Model == "nsi") {
    std::cout << "\nSelected NSI model, switching to NSI calculator" << std::endl;
    calc = DefaultOscCalcNSI();
  }

  PrintOscCalc(calc);

  Spectrum*               FakeData;
  std::vector <Spectrum*> DataToUse;

  predictions fPred_NumuFHC;
  predictions fPred_NumuRHC;

  // this must (and will!) become a loop

  std::vector<predictions> preds;
  predictions tmp_fhc;
  tmp_fhc.pot  = kFD3p5yrFHCPOT;
  tmp_fhc.name = "Numu FHC";
  tmp_fhc.pred = ana::LoadFromFile<ana::PredictionNoExtrap>("pred-numu-fhc.root", "pred").release();
  preds.push_back(tmp_fhc);

  predictions tmp_rhc;
  tmp_rhc.pot  = kFD3p5yrRHCPOT;
  tmp_rhc.name = "Numu RHC";
  tmp_rhc.pred = ana::LoadFromFile<ana::PredictionNoExtrap>("pred-numu-rhc.root", "pred").release();
  preds.push_back(tmp_rhc);

  fPred_NumuFHC = preds[0];
  fPred_NumuRHC = preds[1];
  
  for (int i = 0; i < int(preds.size()); ++i){

         double POT = preds[i].pot;
         DataToUse.push_back(FakeData = GetFakeData(preds[i].pred, calc, POT));
         std::cout << "--> " << preds[i].name << " POT " << POT << " tot MC " << preds[i].pred->Predict(calc).Integral(POT) << " analyze data " << DataToUse[i]->Integral(DataToUse[i]->POT()) << std::endl;
  }

  // todo: creating here a loop for multi-sample loading
  // what we REALLY REALLY need here is a class just like in NOvA like:
  //Ana2024_3Flavor preds(exp_options, data_options, calc);
  //MultiExperiment* exptThis = preds.Experiment2024(exp_options, calc);

  auto MultiExpt = new MultiExperiment();
  MultiExpt->Add(PackExperiment(fPred_NumuFHC, DataToUse[0], calc));
  MultiExpt->Add(PackExperiment(fPred_NumuRHC, DataToUse[1], calc));

  // put a constraint on Th13. Must move this elsewhere
  std::cout << "\nPlacing a PDG 2025 constraint on Th13.." << std::endl;
  MultiExpt->Add(Th13ConstraintPDG2025());
  
  int Xbins = 30, Ybins = 30;


  //#*******************************************************************#
  //#                                                                   #
  //#                          SURFACE                                  #
  //#                                                                   #
  //#*******************************************************************#
  FitUtils::SetVariableType(Variable, FitOptions);
  FitUtils::SetModelType(Model);

  ana::FrequentistSurface surface(MultiExpt, calc,
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
