#include "CAFAna/Prediction/PredictAtmos.h"

#include "CAFAna/Experiment/SingleSampleExperiment.h"

#include "CAFAna/Vars/FitVars.h"

#include "CAFAna/Fit/FrequentistSurface.h"

#include "CAFAna/Analysis/Calcs.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TPad.h"

using namespace ana;

#include "OscLib/IOscCalc.h"

void test_atmos()
{
  osc::IOscCalcAdjustable* calc = DefaultOscCalc();
  calc->SetRho(0);
  //  calc->SetRho(4); // mantle-ish

  PredictAtmos patmos;
  Spectrum fake = patmos.Predict(calc).FakeData(400);//, kLivetime);
  fake.ToTH1(350/*, kLivetime*/)->Draw("hist");
  patmos.fNC.ToTH1(350, kBlue)->Draw("hist same");
  gPad->Update();
  gPad->Print("atmos_spect.png");
  new TCanvas;

  SingleSampleExperiment expt(&patmos, fake);
  
  /*
  FrequentistSurface s1(&expt,
                        calc,
                        &kFitSinSqTheta23, 50, 0, 1,
                        &kFitDmSq31Scaled, 50, 1, 6);
  s1.DrawContour(Gaussian3Sigma2D(s1), kSolid, kRed);
  */

  FrequentistSurface surf(&expt,
                          calc,
                          &kFitSinSqTheta23, 50, 0, 1,
                          &kFitDmSq31Scaled, 50, 1, 6,
                          {}, {&kAtmosSyst});

//  surf.Draw();
  surf.DrawContour(Gaussian68Percent2D(surf), 7, kGreen+2);
  surf.DrawContour(Gaussian3Sigma2D(surf), kSolid, kGreen+2);

  gPad->Print("atmos_cont.pdf");
  gPad->Print("atmos_cont.png");
}
