#include "CAFAna/Prediction/PredictAtmos.h"

#include "CAFAna/Core/MathUtil.h"
#include "CAFAna/Core/SpectrumLoader.h"

#include "OscLib/IOscCalc.h"

#include "TFile.h"
#include "TH2.h"
#include "THnSparse.h"
#include "TPad.h"

namespace ana
{
  const AtmosSyst kAtmosSyst;

  //---------------------------------------------------------------------------
  PredictAtmos::PredictAtmos()
    : fTauFromMu("Reco L/E", Binning::Simple(25, 0, 5000), kNullLoader, kUnweighted, kNoCut),
      fAntiTauFromMu("Reco L/E", Binning::Simple(25, 0, 5000), kNullLoader, kUnweighted, kNoCut),
      fNC("Reco L/E", Binning::Simple(25, 0, 5000), kNullLoader, kUnweighted, kNoCut)/*
    fTauFromMu("Reco cos", Binning::Simple(6, -1, .2), kNullLoader, kUnweighted, kNoCut),
    fAntiTauFromMu("Reco cos", Binning::Simple(6, -1, .2), kNullLoader, kUnweighted, kNoCut),
    fNC("Reco cos", Binning::Simple(6, -1, .2), kNullLoader, kUnweighted, kNoCut)*/
  {
    TFile fin("/pnfs/dune/persistent/users/bckhouse/atmos/DUNEAtmos400ktyr.root");
    TH2* h = (TH2*)fin.Get("hNuMu2NuTau_UnOsc");
    TH2* ah = (TH2*)fin.Get("hANuMu2ANuTau_UnOsc");
    //    h->SetDirectory(0);
    
    THnSparseD* smear = (THnSparseD*)fin.Get("hMigrationMatrixNuTau");

    for(int i = 0; i < smear->GetNbins(); ++i){
      int coord[4];
      const double prob = smear->GetBinContent(i, coord);

      const double E = h->GetXaxis()->GetBinCenter(coord[0]);
      const double cosQ = h->GetYaxis()->GetBinCenter(coord[1]);
      const double recoLE = smear->GetAxis(2)->GetBinCenter(coord[2]);
      const double recocos = smear->GetAxis(3)->GetBinCenter(coord[3]);

      const double R = 6371; // km
      const double dR = 15; // production height (guess)
      const double Lnu  = (R+dR)*(sqrt(util::sqr(1+dR/R)+cosQ*cosQ-1)-cosQ);

      fTauFromMu.Fill(recoLE, 1000*E/Lnu, .37 * h->GetBinContent(coord[0], coord[1]) * prob);
      fAntiTauFromMu.Fill(recoLE, 1000*E/Lnu, .37 *ah->GetBinContent(coord[0], coord[1]) * prob);
    }


    h = (TH2*)fin.Get("hNC_UnOsc");
    h->Add((TH2*)fin.Get("hANC_UnOsc"));
    smear = (THnSparseD*)fin.Get("hMigrationMatrixNC");

    for(int i = 0; i < smear->GetNbins(); ++i){
      int coord[4];
      const double prob = smear->GetBinContent(i, coord);

      //      const double E = h->GetXaxis()->GetBinCenter(coord[0]);
      //      const double cosQ = h->GetYaxis()->GetBinCenter(coord[1]);
      const double recoLE = smear->GetAxis(2)->GetBinCenter(coord[2]);
      const double recocos = smear->GetAxis(3)->GetBinCenter(coord[3]);

      //      const double R = 6371; // km
      //      const double dR = 15; // production height (guess)
      //      const double Lnu  = (R+dR)*(sqrt(util::sqr(1+dR/R)+cosQ*cosQ-1)-cosQ);

      fNC.Fill(recoLE, .005 * h->GetBinContent(coord[0], coord[1])*prob);
    }

    /*
    for(int i = 1; i <= 400; ++i){
      for(int j = 1; j <= 100; ++j){
        // NO. TODO VarMapper from nova branch
        //        fTauFromMu.Fill(h->GetXaxis()->GetBinCenter(i),
        //                        h->GetYaxis()->GetBinCenter(j),
        //                        h->GetBinContent(i, j));

        const double E = h->GetXaxis()->GetBinCenter(i);
        const double cosQ = h->GetYaxis()->GetBinCenter(j);

        const double R = 6371; // km
        const double dR = 15; // production height (guess)
        const double Lnu  = (R+dR)*(sqrt(util::sqr(1+dR/R)+cosQ*cosQ-1)-cosQ);

        //        h->SetBinContent(i, j, Lnu/E);

        fTauFromMu.Fill(Lnu/E, 1000*E/Lnu, .37 * h->GetBinContent(i, j));
      } // end for j
    } // end for i
    */

    //    fTauFromMu.OverrideLivetime(400); // kt*yr
    fTauFromMu.fPOT = 400;
    fAntiTauFromMu.fPOT = 400;
    //    fTauFromMu.fLivetime = 400;

    fNC.OverridePOT(400);

    //    h->Draw("colz");
    //    gPad->SetLogz();
    //    h->SetMaximum(20);
    //    gPad->Print("LE.png");
  }

  //---------------------------------------------------------------------------
  Spectrum PredictAtmos::PredictSyst(osc::IOscCalc* calc,
                                     const SystShifts& shift) const
  {
    const double scale = std::max(0., 1+.25*shift.GetShift(&kAtmosSyst));

    ((osc::IOscCalcAdjustable*)calc)->SetL(1000);

    Spectrum ret = fTauFromMu.Oscillated(calc, 14, 16) + fAntiTauFromMu.Oscillated(calc, -14, -16) + fNC;

    ret.Scale(scale);

    return ret;
  }
}
