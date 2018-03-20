// Make a simple contour
// cafe demo3.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Analysis/Calcs.h"
#include "OscLib/func/OscCalculatorPMNSOpt.h"
#include "StandardRecord/StandardRecord.h"
#include "TCanvas.h"
#include "TH1.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Vars/FitVars.h"

// New includes
#include "CAFAna/Analysis/Surface.h"
#include "CAFAna/Experiment/MultiExperiment.h"

using namespace ana;

void demo3()
{
  // Repeat most of demo2.C
  const std::string fname = "/pnfs/dune/persistent/TaskForce_AnaTree/far/train/v3.2/nu.mcc10.1_def.root";
  SpectrumLoader loader(fname);
  auto* loaderBeam  = loader.LoaderForRunPOT(20000001);
  auto* loaderNue   = loader.LoaderForRunPOT(20000002);
  auto* loaderNuTau = loader.LoaderForRunPOT(20000003);
  auto* loaderNC    = loader.LoaderForRunPOT(0);
  const Var kRecoEnergy = SIMPLEVAR(dune.Ev_reco_numu);
  const Binning binsEnergy = Binning::Simple(40, 0, 10);
  const HistAxis axEnergy("Reco energy (GeV)", binsEnergy, kRecoEnergy);
  const double pot = 3.5 * 1.47e21 * 40/1.13;
  const Cut kPassesMVA = SIMPLEVAR(dune.mvanumu) > 0;
  PredictionNoExtrap pred(*loaderBeam, *loaderNue, *loaderNuTau, *loaderNC, axEnergy, kPassesMVA);
  loader.Go();
  osc::IOscCalculatorAdjustable* calc = DefaultOscCalc();
  const Spectrum data = pred.Predict(calc).MockData(pot);
  SingleSampleExperiment expt(&pred, data);

  // A Surface evaluates the experiment's chisq across a grid
  Surface surf(&expt, calc,
               &kFitSinSqTheta23, 30, 0.425, 0.575,
               &kFitDmSq32Scaled, 30, 2.35, 2.55);

  //surf.Draw();
  surf.DrawBestFit(kBlue);

  // In a full Feldman-Cousins analysis you need to provide a critical value
  // surface to be able to draw a contour. But we provide these helper
  // functions to use the gaussian up-values.
  TH2* crit1sig = Gaussian68Percent2D(surf);
  TH2* crit2sig = Gaussian2Sigma2D(surf);

  surf.DrawContour(crit1sig, 7, kBlue);
  surf.DrawContour(crit2sig, kSolid, kBlue);


  // Let's try to add in the effect of 3.5yrs of RHC data too
  const std::string fnameRHC = "/pnfs/dune/persistent/TaskForce_AnaTree/far/train/v3.2/anu.mcc10.1_def.root";
  SpectrumLoader loaderRHC(fnameRHC);
  // Annoyingly the magic numbers are different for RHC
  auto* loaderBeamRHC  = loaderRHC.LoaderForRunPOT(20000004);
  auto* loaderNueRHC   = loaderRHC.LoaderForRunPOT(20000005);
  auto* loaderNuTauRHC = loaderRHC.LoaderForRunPOT(20000006);
  auto* loaderNCRHC    = loaderRHC.LoaderForRunPOT(0);
  PredictionNoExtrap predRHC(*loaderBeamRHC, *loaderNueRHC, *loaderNuTauRHC, *loaderNCRHC, axEnergy, kPassesMVA);
  loaderRHC.Go();
  calc = DefaultOscCalc(); // Remember to reset, since fits modified it
  const Spectrum dataRHC = predRHC.Predict(calc).MockData(pot);
  SingleSampleExperiment exptRHC(&predRHC, dataRHC);

  // A MultiExperiment gets its chisq just by adding together its component
  // parts. Use to implement joint fits
  MultiExperiment exptMulti({&expt, &exptRHC});

  Surface surfMulti(&exptMulti, calc,
                    &kFitSinSqTheta23, 30, 0.425, 0.575,
                    &kFitDmSq32Scaled, 30, 2.35, 2.55);

  surfMulti.DrawBestFit(kRed);
  surfMulti.DrawContour(crit1sig, 7, kRed);
  surfMulti.DrawContour(crit2sig, kSolid, kRed);
}
