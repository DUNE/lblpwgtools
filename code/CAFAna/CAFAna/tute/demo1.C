// Make oscillated predictions
// cafe demo1.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Cuts/TruthCuts.h"
#include "StandardRecord/StandardRecord.h"
#include "TCanvas.h"
#include "TH1.h"

// New includes for this macro
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Analysis/Calcs.h"
#include "OscLib/func/OscCalculatorPMNSOpt.h"

using namespace ana;

void demo1()
{
  // See demo0.C for explanation of these repeated parts
  const std::string fname = "/pnfs/dune/persistent/TaskForce_AnaTree/far/train/v3.2/nu.mcc10.1_def.root";
  SpectrumLoader loader(fname);
  auto* loaderBeam  = loader.LoaderForRunPOT(20000001);
  auto* loaderNue   = loader.LoaderForRunPOT(20000002);
  auto* loaderNuTau = loader.LoaderForRunPOT(20000003);
  auto* loaderNC    = loader.LoaderForRunPOT(0);
  const Var kRecoEnergy = SIMPLEVAR(dune.Ev_reco_numu);
  const Var kMVANumu = SIMPLEVAR(dune.mvanumu);
  const Binning binsEnergy = Binning::Simple(40, 0, 10);
  const HistAxis axEnergy("Reco energy (GeV)", binsEnergy, kRecoEnergy);
  const double pot = 3.5 * 1.47e21 * 40/1.13;

  // A cut is structured like a Var, but returning bool
  const Cut kPassesMVA({},
                       [](const caf::StandardRecord* sr)
                       {
                         return sr->dune.mvanumu > 0;
                       });

  // In many cases it's easier to form them from existing Vars like this
  //  const Cut kPassesMVA = kMVANumu > 0;

  // A Prediction is an objects holding a variety of "OscillatableSpectrum"
  // objects, one for each original and final flavour combination.
  PredictionNoExtrap pred(*loaderBeam, *loaderNue, *loaderNuTau, *loaderNC,
                          axEnergy, kPassesMVA);

  // This call will fill all of the constituent parts of the prediction
  loader.Go();

  // We can extract a total prediction unoscillated
  const Spectrum sUnosc = pred.PredictUnoscillated();
  // Or oscillated, in this case using reasonable parameters from
  // Analysis/Calcs.h
  osc::IOscCalculator* calc = DefaultOscCalc();
  const Spectrum sOsc = pred.Predict(calc);

  // And we can break things down by flavour
  const Spectrum sUnoscNC = pred.PredictComponent(calc,
                                                  Flavors::kAll,
                                                  Current::kNC,
                                                  Sign::kBoth);

  // Plot what we have so far
  sUnosc.ToTH1(pot)->Draw("hist");
  sUnoscNC.ToTH1(pot, kBlue)->Draw("hist same");
  sOsc.ToTH1(pot, kRed)->Draw("hist same");

  // "Fake" data is synonymous with the Asimov data sample
  new TCanvas;
  sOsc.ToTH1(pot, kRed)->Draw("hist");
  sUnoscNC.ToTH1(pot, kBlue)->Draw("hist same");
  sOsc.FakeData(pot).ToTH1(pot)->Draw("ep same");

  // While "mock" data has statistical fluctuations in
  new TCanvas;
  sOsc.ToTH1(pot, kRed)->Draw("hist");
  sUnoscNC.ToTH1(pot, kBlue)->Draw("hist same");
  sOsc.MockData(pot).ToTH1(pot)->Draw("ep same");
}
