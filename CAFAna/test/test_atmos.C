#include "CAFAna/Prediction/PredictAtmos.h"

#include "CAFAna/Experiment/SingleSampleExperiment.h"

#include "CAFAna/Vars/FitVars.h"

#include "CAFAna/Fit/FrequentistSurface.h"

#include "CAFAna/Analysis/Calcs.h"

#include "TCanvas.h"
#include "TH2.h"
#include "TMath.h"
#include "TPad.h"

using namespace ana;

#include "OscLib/IOscCalc.h"

void test_atmos()
{
  osc::IOscCalcAdjustable* calc = DefaultOscCalc();
  calc->SetRho(0);
  //  calc->SetRho(4); // mantle-ish

  // From Adam
  const double th12  = asin(sqrt(0.310));
  const double th13  = asin(sqrt(0.02240));
  const double th23  = asin(sqrt(0.582));
  const double dcp   = 217*TMath::Pi()/180.;
  const double dm221 = 7.39e-5;
  const double dm231 = 2.525e-3;

  calc->SetTh12(th12);
  calc->SetTh13(th13);
  calc->SetTh23(th23);
  calc->SetdCP(dcp);
  calc->SetDmsq21(dm221);
  calc->SetDmsq32(dm231-dm221);

  PredictAtmos patmos;

  patmos.fTauFromMuRW.WeightingVariable().ToTH2(350)->Draw("colz");
  new TCanvas;
  patmos.fTauFromMuRW.WeightingVariable().ToTH2(350)->ProjectionX()->Draw("hist");
  new TCanvas;
  patmos.fTauFromMuRW.WeightingVariable().ToTH2(350)->ProjectionY()->Draw("hist");
  //  return;

  Spectrum fake = patmos.Predict(calc).FakeData(350);//, kLivetime);
  fake.ToTH1(350/*, kLivetime*/)->Draw("hist");
  TH1* nc = patmos.fNC.ToTH1(350, kBlue);
  nc->Draw("hist same");
  TH1* cc = fake.ToTH1(350, kRed);
  cc->Add(nc, -1);
  cc->Draw("hist same");
  gPad->Update();
  gPad->Print("atmos_spect.png");

  //  patmos.fNC.ToTH2(350)->Draw("colz text");
  //  gPad->Print("atmos_spect_nc_2d.png");

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
