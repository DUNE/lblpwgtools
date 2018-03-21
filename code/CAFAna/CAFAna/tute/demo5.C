// Make oscillated predictions with systematics
// cafe demo5.C

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
    sr->dune.Ev_reco_numu *= (1+.1*sigma);
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
    // Note I've switched this around to apply to high energy events, to more
    // clearly seperate the effects from the energy scale syst.
    if(sr->dune.Ev_reco_numu > 7) weight *= 1+0.2*sigma;
  }
};
const ToyNormSyst nSyst;

void demo5()
{
  const std::string fname = "/pnfs/dune/persistent/TaskForce_AnaTree/far/train/v3.2/nu.mcc10.1_def.root";
  SpectrumLoader loader(fname);
  auto* loaderBeam  = loader.LoaderForRunPOT(20000001);
  auto* loaderNue   = loader.LoaderForRunPOT(20000002);
  auto* loaderNuTau = loader.LoaderForRunPOT(20000003);
  auto* loaderNC    = loader.LoaderForRunPOT(0);
  const Var kRecoEnergy = SIMPLEVAR(dune.Ev_reco_numu);
  const Binning binsEnergy = Binning::Simple(40, 0, 10);
  const HistAxis axEnergy("Reco energy (GeV)", binsEnergy, kRecoEnergy);
  const Cut kPassesMVA = SIMPLEVAR(dune.mvanumu) > 0;
  const double pot = 3.5 * 1.47e21 * 40/1.13;
  osc::IOscCalculator* calc = DefaultOscCalc();

  PredictionNoExtrap predNom(*loaderBeam, *loaderNue, *loaderNuTau, *loaderNC,
                             axEnergy, kPassesMVA);

  // Can set multiple systematics at once like this
  SystShifts bothUp;
  bothUp.SetShift(&eSyst, +1);
  bothUp.SetShift(&nSyst, +3);

  SystShifts bothDn;
  bothDn.SetShift(&eSyst, -1);
  bothDn.SetShift(&nSyst, -3);

  // Each of the constituent OscillatableSpectrum objects within these
  // predictions will be systematically altered as specified in the last
  // argument.
  PredictionNoExtrap predUp(*loaderBeam, *loaderNue, *loaderNuTau, *loaderNC,
                            axEnergy, kPassesMVA,
                            bothUp);
  PredictionNoExtrap predDn(*loaderBeam, *loaderNue, *loaderNuTau, *loaderNC,
                            axEnergy, kPassesMVA,
                            bothDn);

  // Fill all the nominal and shifted spectra within all three predictions
  loader.Go();

  const Spectrum sOscNom = predNom.Predict(calc);
  const Spectrum sOscUp = predUp.Predict(calc);
  const Spectrum sOscDn = predDn.Predict(calc);

  sOscDn.ToTH1(pot, kBlue)->Draw("hist");
  sOscNom.ToTH1(pot)->Draw("hist same");
  sOscUp.ToTH1(pot, kRed)->Draw("hist same");
}
