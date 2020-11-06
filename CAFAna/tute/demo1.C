// Make oscillated predictions
// cafe demo1.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Cuts/TruthCuts.h"
#include "StandardRecord/Proxy/SRProxy.h"
#include "TCanvas.h"
#include "TH1.h"

// New includes for this macro
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Analysis/Calcs.h"
#include "OscLib/OscCalcPMNSOpt.h"

using namespace ana;

void demo1()
{
  // See demo0.C for explanation of these repeated parts
  const std::string fnameNonSwap = "/dune/data/users/marshalc/CAFs/mcc11_v3/FD_FHC_nonswap.root";
  const std::string fnameNueSwap = "/dune/data/users/marshalc/CAFs/mcc11_v3/FD_FHC_nueswap.root";
  const std::string fnameTauSwap = "/dune/data/users/marshalc/CAFs/mcc11_v3/FD_FHC_tauswap.root";
  SpectrumLoader loaderNonSwap(fnameNonSwap);
  SpectrumLoader loaderNueSwap(fnameNueSwap);
  SpectrumLoader loaderTauSwap(fnameTauSwap);
  const Var kRecoEnergy = SIMPLEVAR(Ev_reco_numu);
  const Var kCVNNumu = SIMPLEVAR(cvnnumu);
  const Binning binsEnergy = Binning::Simple(40, 0, 10);
  const HistAxis axEnergy("Reco energy (GeV)", binsEnergy, kRecoEnergy);
  const double pot = 3.5 * 1.47e21 * 40/1.13;

  // A cut is structured like a Var, but returning bool
  const Cut kPassesCVN([](const caf::SRProxy* sr)
                       {
                         return sr->cvnnumu > 0.5;
                       });

  // In many cases it's easier to form them from existing Vars like this
  //  const Cut kPassesCVN = kCVNNumu > 0;

  // A Prediction is an objects holding a variety of "OscillatableSpectrum"
  // objects, one for each original and final flavour combination.
  PredictionNoExtrap pred(loaderNonSwap, loaderNueSwap, loaderTauSwap,
                          axEnergy, kPassesCVN);

  // These calls will fill all of the constituent parts of the prediction
  loaderNonSwap.Go();
  loaderNueSwap.Go();
  loaderTauSwap.Go();

  // We can extract a total prediction unoscillated
  const Spectrum sUnosc = pred.PredictUnoscillated();
  // Or oscillated, in this case using reasonable parameters from
  // Analysis/Calcs.h
  osc::IOscCalc* calc = DefaultOscCalc();
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
