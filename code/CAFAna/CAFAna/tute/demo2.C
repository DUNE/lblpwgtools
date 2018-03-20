// Exercise the fitter
// cafe demo2.C

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

// New includes required
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Analysis/Fit.h"
#include "CAFAna/Vars/FitVars.h"

using namespace ana;

void demo2()
{
  // Repeat all of demo1.C to get us our Prediction object
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

  // We make the oscillation calculator "adjustable" so the fitter can
  // manipulate it.
  osc::IOscCalculatorAdjustable* calc = DefaultOscCalc();

  // To make a fit we need to have a "data" spectrum to compare to our MC
  // Prediction object
  const Spectrum data = pred.Predict(calc).MockData(pot);

  // An Experiment object is something that can turn oscillation parameters
  // into a chisq, in this case by comparing a Prediction and a data Spectrum
  SingleSampleExperiment expt(&pred, data);

  std::cout << "At nominal parameters chisq = " << expt.ChiSq(calc) << std::endl;
  calc->SetTh23(asin(sqrt(0.45)));
  std::cout << "At non-maximal params chisq = " << expt.ChiSq(calc) << std::endl;

  // A fitter finds the minimum chisq using MINUIT by varying the list of
  // parameters given. These are FitVars from Vars/FitVars.h. They can contain
  // snippets of code to convert from the underlying angles etc to whatever
  // function you want to fit.
  Fitter fit(&expt, {&kFitDmSq32Scaled, &kFitSinSqTheta23});
  const double best_chisq = fit.Fit(calc);

  // The osc calculator is updated in-place with the best oscillation
  // parameters
  std::cout << "Best chisq is " << best_chisq << " with "
            << "dmsq = " << calc->GetDmsq32()
            << " and sinsq = " << kFitSinSqTheta23.GetValue(calc)
            << std::endl;
}
