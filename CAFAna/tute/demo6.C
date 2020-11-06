// Introduces PredictionInterp
// cafe demo6.C

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Analysis/Calcs.h"
#include "CAFAna/Analysis/TDRLoaders.h"
#include "StandardRecord/SRProxy.h"
#include "OscLib/OscCalcPMNSOpt.h"
#include "TCanvas.h"
#include "TH1.h"

// new includes
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Prediction/PredictionInterp.h"

#include "TRandom3.h"

using namespace ana;

class ToyEnergyScaleSyst: public ISyst
{
public:
  ToyEnergyScaleSyst() : ISyst("toyEScale", "Toy Energy Scale") {}
  void Shift(double sigma,
             caf::StandardRecord* sr, double& weight) const override
  {
    sr->dune.Ev_reco_numu *= (1+.1*sigma);
  }
};
const ToyEnergyScaleSyst eSyst;

class ToyNormSyst: public ISyst
{
public:
  ToyNormSyst() : ISyst("toyNorm", "Toy Norm Syst") {}
  void Shift(double sigma,
             caf::StandardRecord* sr, double& weight) const override
  {
    if(sr->Ev_reco_numu > 7) weight *= 1+0.2*sigma;
  }
};
const ToyNormSyst nSyst;

void demo6()
{
  TDRLoaders loaders(Loaders::kFHC);
  const Var kRecoEnergy = SIMPLEVAR(Ev_reco_numu);
  const Binning binsEnergy = Binning::Simple(40, 0, 10);
  const HistAxis axEnergy("Reco energy (GeV)", binsEnergy, kRecoEnergy);
  const double pot = 3.5 * 1.47e21 * 40/1.13;
  const Cut kPassesCVN = SIMPLEVAR(cvnnumu) > .5;
  osc::IOscCalc* calc = DefaultOscCalc();

  // We're going to use a PredictionInterp that will allow us to interpolate to
  // any values of the systematic parameters. Internally that works by creating
  // various predictions at different values of the parameters, so we need to
  // add this extra layer of indirection to allow it to create those.
  NoExtrapPredictionGenerator gen(axEnergy, kPassesCVN);

  // PredictionInterp needs:
  // - The list of systematics it should be ready to interpolate over
  // - A "seed" oscillation calculator it will do its expansions around
  // - The generator from above
  // - The loaders
  PredictionInterp predInterp({&eSyst, &nSyst}, calc, gen, loaders);

  // Fill all the different variants of the predictions that PredictionInterp
  // needs to make.
  loaders.Go();

  // Make some nice plots of what the interpolation looks like in each bin
  predInterp.DebugPlots(calc);

  // We can generate predictions at whatever values of the systematic shifts we
  // want. Prove it by plotting 100 different possible "universes".
  new TCanvas;

  TH1* hnom = predInterp.Predict(calc).ToTH1(pot);
  hnom->SetLineWidth(3);
  hnom->Draw("hist");
  hnom->GetYaxis()->SetRangeUser(0, 2250);

  for(int i = 0; i < 100; ++i){
    SystShifts s;
    s.SetShift(&eSyst, gRandom->Gaus());
    s.SetShift(&nSyst, gRandom->Gaus());

    TH1* hshift = predInterp.PredictSyst(calc, s).ToTH1(pot);
    hshift->SetLineColor(kGray);
    hshift->Draw("hist same");
  }

  hnom->Draw("hist same");
}
