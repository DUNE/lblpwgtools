#include "CAFAna/Atmos/AtmosOscillogram.h"
#include "CAFAna/Atmos/AtmosOscillogramLE.h"

#include "TCanvas.h"
#include "TH2.h"
#include "TMath.h"
#include "TPad.h"

#include "TFile.h"
#include "THnSparse.h"

using namespace ana;

#include "OscLib/IOscCalc.h"
#include "OscLib/OscCalcAnalytic.h"

void test_oscillogram()
{
  osc::IOscCalcAdjustable* calc = new osc::OscCalcAnalytic;

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

  AtmosOscillogram oscgram(calc, 14, 16);
  oscgram.ToTH2()->Draw("colz");
  gPad->Print("oscillogram.pdf");

  new TCanvas;
  AtmosOscillogramLE ogLE(calc, 14, 16);
  ogLE.ToTH2()->Draw("colz");
  gPad->Print("oscillogramLE.pdf");


  // Where is the data in this space?
  TH2* pop = new TH2F("", ";True Energy (GeV); True cos(#theta_{zenith})", 400, .1, 100, 50, -1, +1);
  TH2* popLE = new TH2F("", ";True L/E (km/GeV); True L (km)", 50, 0, 5000, 25, 0, 13000);

  const std::string kFilename = "/pnfs/dune/persistent/users/bckhouse/atmos/MigrationMatrixHighRes.root";

  TFile fin(kFilename.c_str());
  TH2* h = (TH2*)fin.Get("hNuMu2NuTau_UnOsc");
    
  THnSparseD* smear = (THnSparseD*)fin.Get("hMigrationMatrixNuTau");

  for(int i = 0; i < smear->GetNbins(); ++i){
    struct {int E, cosQ, recoLE, recoCosQ;} bin;
    const double prob = smear->GetBinContent(i, &bin.E);

    const double E    = h->GetXaxis()->GetBinCenter(bin.E);
    const double cosQ = h->GetYaxis()->GetBinCenter(bin.cosQ);
    const double recoLE  = smear->GetAxis(2)->GetBinCenter(bin.recoLE);
    const double recocos = smear->GetAxis(3)->GetBinCenter(bin.recoCosQ);

    const double R = 6371; // km
    const double dR = 15; // production height (guess)
    const double L  = (R+dR)*(sqrt((1+dR/R)*(1+dR/R)+cosQ*cosQ-1)-cosQ);

    pop->Fill(E, cosQ, h->GetBinContent(bin.E, bin.cosQ));
    popLE->Fill(L/E, L, h->GetBinContent(bin.E, bin.cosQ));
  } // end for i

  new TCanvas;
  pop->Draw("colz");
  gPad->Print("pop.pdf");
  gPad->SetLogz();
  gPad->Print("pop_logz.pdf");

  new TCanvas;
  popLE->Draw("colz");
  gPad->Print("popLE.pdf");
  gPad->SetLogz();
  gPad->Print("popLE_logz.pdf");
}
