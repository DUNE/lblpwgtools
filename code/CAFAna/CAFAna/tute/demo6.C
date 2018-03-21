// Introduces PredictionInterp
// cafe demo6.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Analysis/Calcs.h"
#include "StandardRecord/StandardRecord.h"
#include "OscLib/func/OscCalculatorPMNSOpt.h"
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
  std::string ShortName() const override {return "toyEScale";}
  std::string LatexName() const override {return "Toy Energy Scale";}
  void Shift(double sigma,
             Restorer& restore,
             caf::StandardRecord* sr,
             double& weight) const override
  {
    restore.Add(sr->dune.Ev_reco_numu);
    sr->dune.Ev_reco_numu *= (1+.05*sigma);
  }
};
const ToyEnergyScaleSyst eSyst;

class ToyNormSyst: public ISyst
{
public:
  std::string ShortName() const override {return "toyNorm";}
  std::string LatexName() const override {return "Toy Norm syst";}
  void Shift(double sigma,
             Restorer& restore,
             caf::StandardRecord* sr,
             double& weight) const override
  {
    if(sr->dune.Ev_reco_numu > 7) weight *= 1+0.1*sigma;
  }
};
const ToyNormSyst nSyst;

void demo6()
{
  const std::string fname = "/pnfs/dune/persistent/TaskForce_AnaTree/far/train/v3.2/nu.mcc10.1_def.root";
  SpectrumLoader loader(fname);
  auto* loaderBeam  = loader.LoaderForRunPOT(20000001);
  auto* loaderNue   = loader.LoaderForRunPOT(20000002);
  auto* loaderNuTau = loader.LoaderForRunPOT(20000003);
  auto* loaderNC    = loader.LoaderForRunPOT(0);
  const Var kRecoEnergy = SIMPLEVAR(dune.Ev_reco_numu);
  const Binning binsEnergy = Binning::Simple(20, 0, 10);
  const HistAxis axEnergy("Reco energy (GeV)", binsEnergy, kRecoEnergy);
  const Cut kPassesMVA = SIMPLEVAR(dune.mvanumu) > 0;
  const double pot = 3.5 * 1.47e21 * 40/1.13;
  osc::IOscCalculator* calc = DefaultOscCalc();

  // We're going to use a PredictionInterp that will allow us to interpolate to
  // any values of the systematic parameters. Internally that works by creating
  // various predictions at different values of the paramters, so we need to
  // add this extra layer of indirection to allow it to create those.
  DUNENoExtrapPredictionGenerator gen(*loaderBeam,  *loaderNue,
                                      *loaderNuTau, *loaderNC,
                                      axEnergy, kPassesMVA);

  // Apologies for this. Interface wart.
  Loaders dummyLoaders;

  // PredictionInterp needs:
  // - The list of systematics it should be ready to interpolate over
  // - A "seed" oscillation calculator it will do its expansions around
  // - The generator from above
  // - The dummy loaders (sorry)
  PredictionInterp predInterp({&eSyst, &nSyst},
                              calc,
                              gen,
                              dummyLoaders);

  // Fill all the different variants of the predictions that PredictionInterp
  // needs to make.
  loader.Go();

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
