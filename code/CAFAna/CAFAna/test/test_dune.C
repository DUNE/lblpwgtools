#ifdef __CINT__
void test_dune(){}
#else

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Analysis/Calcs.h"
#include "CAFAna/Analysis/Plots.h"
#include "CAFAna/Analysis/Surface.h"
#include "CAFAna/Vars/FitVars.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Experiment/MultiExperiment.h"
#include "CAFAna/Cuts/TruthCuts.h"
using namespace ana;

#include "StandardRecord/StandardRecord.h"
#include "OscLib/func/IOscCalculator.h"

#include "Utilities/rootlogon.C"

#include "TCanvas.h"
#include "TH1.h"
#include "TPad.h"

//#include "Rtypes.h"
//R__LOAD_LIBRARY(libCAFAna)
//R__LOAD_LIBRARY(/dune/app/users/bckhouse/lbl_workshop/releases/development/lib/Linux2.6-GCC-maxopt/libCAFAna.so)


//R__ADD_LIBRARY_PATH(/dune/app/users/bckhouse/lbl_workshop/releases/development/lib/Linux2.6-GCC-maxopt/)
//R__LOAD_LIBRARY(libCAFAna)

void test_dune()
{
  rootlogon(); // style

  // POT/yr * 3.5yrs * mass correction
  const double pot = 3.5 * 1.47e21 * 40/1.13;

  SpectrumLoader loader("/pnfs/dune/persistent/TaskForce_AnaTree/far/train/v2.1/numutest.root");
  SpectrumLoader loaderNue("/pnfs/dune/persistent/TaskForce_AnaTree/far/train/v2.1/nuetest.root");

  auto* loaderNumuBeam  = loader.LoaderForRunPOT(20000001);
  auto* loaderNumuNue   = loader.LoaderForRunPOT(20000002);
  auto* loaderNumuNuTau = loader.LoaderForRunPOT(20000003);
  auto* loaderNumuNC    = loader.LoaderForRunPOT(0);

  auto* loaderNueBeam  = loaderNue.LoaderForRunPOT(20000001);
  auto* loaderNueNue   = loaderNue.LoaderForRunPOT(20000002);
  auto* loaderNueNuTau = loaderNue.LoaderForRunPOT(20000003);
  auto* loaderNueNC    = loaderNue.LoaderForRunPOT(0);

  const Var Enu_reco = SIMPLEVAR(dune.Ev_reco);
  const Cut kSelNumu = SIMPLEVAR(dune.mvaresult) > 0;
  const Cut kSelNue = SIMPLEVAR(dune.mvaresult) > 0;

  PredictionNoExtrap predNumuPID(*loaderNumuBeam, *loaderNumuNue, *loaderNumuNuTau, *loaderNumuNC, "PID", Binning::Simple(100, -1, +1), SIMPLEVAR(dune.mvaresult), kNoCut);

  PredictionNoExtrap predNuePID(*loaderNueBeam, *loaderNueNue, *loaderNueNuTau, *loaderNueNC, "PID", Binning::Simple(100, -1, +1), SIMPLEVAR(dune.mvaresult), kNoCut);

  PredictionNoExtrap pred(*loaderNumuBeam, *loaderNumuNue, *loaderNumuNuTau, *loaderNueNC, "Reco E (GeV)", Binning::Simple(80, 0, 10), Enu_reco, kSelNumu);

  PredictionNoExtrap predNue(*loaderNueBeam, *loaderNueNue, *loaderNueNuTau, *loaderNueNC, "Reco E (GeV)", Binning::Simple(24, 0, 6), Enu_reco, kSelNue);

  loader.Go();
  loaderNue.Go();


  osc::IOscCalculatorAdjustable* calc = DefaultOscCalc();
  calc->SetL(1300);
  calc->SetdCP(TMath::Pi()*1.5);


  Spectrum mock = pred.Predict(calc).FakeData(pot);
  SingleSampleExperiment expt(&pred, mock);

  Spectrum mockNue = predNue.Predict(calc).FakeData(pot);
  SingleSampleExperiment exptNue(&predNue, mockNue);

  Spectrum mockNuePID = predNuePID.Predict(calc).FakeData(pot);
  Spectrum mockNumuPID = predNumuPID.Predict(calc).FakeData(pot);

  Surface surf(&expt, calc, &kFitSinSqTheta23, 20, .4, .6, &kFitDmSq32Scaled, 20, 2.35, 2.5);

  MultiExperiment me({&expt, &exptNue});

  Surface surfNue(&me, calc, &kFitDeltaInPiUnits, 20, 0, 2, &kFitSinSqTheta23, 20, .4, .6);


  new TCanvas;
  DataMCComparisonComponents(mock, &pred, calc);
  gPad->Print("components.eps");

  new TCanvas;
  DataMCComparisonComponents(mockNue, &predNue, calc);
  gPad->Print("components_nue.eps");

  new TCanvas;
  TH1* h2 = DataMCComparisonComponents(mockNumuPID, &predNumuPID, calc);
  h2->GetYaxis()->SetRangeUser(1, 1e4);
  gPad->SetLogy();
  gPad->Print("components_pid.eps");

  new TCanvas;
  TH1* h = DataMCComparisonComponents(mockNuePID, &predNuePID, calc);
  h->GetYaxis()->SetRangeUser(0, 600);
  gPad->Print("components_nue_pid.eps");

  new TCanvas;
  surf.DrawContour(Gaussian68Percent2D(surf), 7, kRed);
  surf.DrawContour(Gaussian90Percent2D(surf), kSolid, kRed);
  surf.DrawBestFit(kRed);
  gPad->Print("cont.eps");

  new TCanvas;
  surfNue.DrawContour(Gaussian68Percent2D(surfNue), 7, kRed);
  surfNue.DrawContour(Gaussian90Percent2D(surfNue), kSolid, kRed);
  surfNue.DrawBestFit(kRed);
  gPad->Print("cont_nue.eps");
}

#endif
