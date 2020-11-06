// Make oscillated predictions with systematics
// cafe demo5.C

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

using namespace ana;

class ToyEnergyScaleSyst: public ISyst
{
public:
  ToyEnergyScaleSyst() : ISyst("toyEScale", "Toy Energy Scale")
  {
  }

  void Shift(double sigma,
             caf::StandardRecord* sr,
             double& weight) const override
  {
    sr->dune.Ev_reco_numu *= (1+.1*sigma);
  }
};
const ToyEnergyScaleSyst eSyst;

class ToyNormSyst: public ISyst
{
public:
  ToyNormSyst() : ISyst("toyNorm", "Toy Norm Syst")
  {
  }

  void Shift(double sigma,
             caf::StandardRecord* sr,
             double& weight) const override
  {
    // Note I've switched this around to apply to high energy events, to more
    // clearly seperate the effects from the energy scale syst.
    if(sr->Ev_reco_numu > 7) weight *= 1+0.2*sigma;
  }
};
const ToyNormSyst nSyst;

void demo5()
{
  TDRLoaders loaders(Loaders::kFHC);
  const Var kRecoEnergy = SIMPLEVAR(Ev_reco_numu);
  const Binning binsEnergy = Binning::Simple(40, 0, 10);
  const HistAxis axEnergy("Reco energy (GeV)", binsEnergy, kRecoEnergy);
  const double pot = 3.5 * 1.47e21 * 40/1.13;
  const Cut kPassesCVN = SIMPLEVAR(cvnnumu) > .5;
  osc::IOscCalc* calc = DefaultOscCalc();

  PredictionNoExtrap predNom(loaders, axEnergy, kPassesCVN);


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
  PredictionNoExtrap predUp(loaders, axEnergy, kPassesCVN, bothUp);
  PredictionNoExtrap predDn(loaders, axEnergy, kPassesCVN, bothDn);

  // Fill all the nominal and shifted spectra within all three predictions
  loaders.Go();

  const Spectrum sOscNom = predNom.Predict(calc);
  const Spectrum sOscUp = predUp.Predict(calc);
  const Spectrum sOscDn = predDn.Predict(calc);

  sOscDn.ToTH1(pot, kBlue)->Draw("hist");
  sOscNom.ToTH1(pot)->Draw("hist same");
  sOscUp.ToTH1(pot, kRed)->Draw("hist same");
}
