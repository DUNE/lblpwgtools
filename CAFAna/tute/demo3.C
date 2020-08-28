// Make a simple contour
// cafe demo3.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Analysis/Calcs.h"
#include "CAFAna/Analysis/TDRLoaders.h"
#include "OscLib/OscCalcPMNSOpt.h"
#include "StandardRecord/StandardRecord.h"
#include "TCanvas.h"
#include "TH1.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Vars/FitVars.h"

// New includes
#include "CAFAna/Fit/FrequentistSurface.h"
#include "CAFAna/Experiment/MultiExperiment.h"

using namespace ana;

void demo3()
{
  // Repeat most of demo2.C
  //
  // Except we're introducing "Loaders", which knows the latest CAF locations
  // and packages the three types of loader together into one handy class.
  TDRLoaders loaders(Loaders::kFHC);
  const Var kRecoEnergy = SIMPLEVAR(Ev_reco_numu);
  const Binning binsEnergy = Binning::Simple(40, 0, 10);
  const HistAxis axEnergy("Reco energy (GeV)", binsEnergy, kRecoEnergy);
  const double pot = 3.5 * 1.47e21 * 40/1.13;
  const Cut kPassesCVN = SIMPLEVAR(cvnnumu) > .5;
  PredictionNoExtrap pred(loaders, axEnergy, kPassesCVN);
  loaders.Go();
  osc::IOscCalcAdjustable* calc = DefaultOscCalc();
  const Spectrum data = pred.Predict(calc).MockData(pot);
  SingleSampleExperiment expt(&pred, data);

  // A Surface evaluates the experiment's chisq across a grid
  FrequentistSurface surf(&expt, calc,
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
  TDRLoaders loadersRHC(Loaders::kRHC);
  PredictionNoExtrap predRHC(loadersRHC, axEnergy, kPassesCVN);
  loadersRHC.Go();
  calc = DefaultOscCalc(); // Remember to reset, since fits modified it
  const Spectrum dataRHC = predRHC.Predict(calc).MockData(pot);
  SingleSampleExperiment exptRHC(&predRHC, dataRHC);

  // A MultiExperiment gets its chisq just by adding together its component
  // parts. Use to implement joint fits
  MultiExperiment exptMulti({&expt, &exptRHC});

  FrequentistSurface surfMulti(&exptMulti, calc,
                               &kFitSinSqTheta23, 30, 0.425, 0.575,
                               &kFitDmSq32Scaled, 30, 2.35, 2.55);

  surfMulti.DrawBestFit(kRed);
  surfMulti.DrawContour(crit1sig, 7, kRed);
  surfMulti.DrawContour(crit2sig, kSolid, kRed);
}
