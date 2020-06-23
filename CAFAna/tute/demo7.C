// Use full set of flux and xsec systs
// cafe demo7.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Vars/FitVars.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Analysis/Calcs.h"
#include "CAFAna/Analysis/Surface.h"
#include "CAFAna/Analysis/TDRLoaders.h"
#include "StandardRecord/StandardRecord.h"
#include "OscLib/func/OscCalculatorPMNSOpt.h"
#include "TCanvas.h"
#include "TH1.h"

// new includes
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Prediction/PredictionInterp.h"

#include "TRandom3.h"


#include "CAFAna/Systs/DUNEFluxSysts.h"
#include "CAFAna/Systs/XSecSysts.h"

using namespace ana;

void demo7()
{
  TDRLoaders loaders(Loaders::kFHC);
  const Var kRecoEnergy = SIMPLEVAR(Ev_reco_numu);
  const Binning binsEnergy = Binning::Simple(40, 0, 10);
  const HistAxis axEnergy("Reco energy (GeV)", binsEnergy, kRecoEnergy);
  const double pot = 3.5 * 1.47e21 * 40/1.13;
  const Cut kPassesCVN = SIMPLEVAR(cvnnumu) > .5;
  osc::IOscCalculatorAdjustable* calc = DefaultOscCalc();
  NoExtrapPredictionGenerator gen(axEnergy, kPassesCVN);

  // First 10 principal components of diagonalized flux covariance matrix
  std::vector<const ISyst*> fluxsysts = GetDUNEFluxSysts(10);
  // Curated GENIE cross section uncertainties list
  std::vector<const ISyst*> xsecsysts = GetXSecSysts();
  std::vector<const ISyst*> allSysts;
  allSysts.insert(allSysts.end(), xsecsysts.begin(), xsecsysts.end());
  allSysts.insert(allSysts.end(), fluxsysts.begin(), fluxsysts.end());

  // List all of the systematics we'll be using
  for(const ISyst* s: allSysts) std::cout << s->ShortName() << "\t\t" << s->LatexName() << std::endl;

  PredictionInterp predInterp(allSysts, calc, gen, loaders);

  loaders.Go();

  new TCanvas;
  TH1* hnom = predInterp.Predict(calc).ToTH1(pot);
  hnom->SetLineWidth(3);
  hnom->Draw("hist");
  hnom->GetYaxis()->SetRangeUser(0, 2500);

  for(int i = 0; i < 100; ++i){
    SystShifts shifts;
    for(const ISyst* s: allSysts) shifts.SetShift(s, gRandom->Gaus());
    predInterp.PredictSyst(calc, shifts).ToTH1(pot, kGray)->Draw("hist same");
  }

  hnom->Draw("hist same");


  new TCanvas;

  const Spectrum data = predInterp.Predict(calc).FakeData(pot);
  SingleSampleExperiment expt(&predInterp, data);

  // The regular stats-only contour
  Surface surf(&expt, calc,
               &kFitSinSqTheta23, 30, 0.425, 0.575,
               &kFitDmSq32Scaled, 30, 2.35, 2.55);

  surf.DrawBestFit(kBlue);

  surf.DrawContour(Gaussian68Percent2D(surf), 7, kBlue);
  surf.DrawContour(Gaussian2Sigma2D(surf), kSolid, kBlue);

  gPad->Update();

  // This takes substantially longer, we reduced the bin count a lot to keep it
  // reasonable.
  Surface surfSyst(&expt, calc,
                   &kFitSinSqTheta23, 10, 0.45, 0.6,
                   &kFitDmSq32Scaled, 10, 2.35, 2.5,
                   {}, // list of oscillation parameters to profile over
                   allSysts); // list of systs to profile over

  surfSyst.DrawContour(Gaussian68Percent2D(surfSyst), 7, kRed);
  surfSyst.DrawContour(Gaussian2Sigma2D(surfSyst), kSolid, kRed);
}
