#include "CAFAna/Prediction/PredictAtmos.h"

#include "CAFAna/Core/MathUtil.h"
#include "CAFAna/Core/SpectrumLoader.h"

#include "OscLib/IOscCalc.h"

#include "TFile.h"
#include "TH2.h"
#include "THnSparse.h"

namespace ana
{
  const AtmosSyst kAtmosSyst;

  const Binning kRecoLEBins = Binning::Simple(25, 0, 5000);
  const Binning kRecoCosZenithBins = Binning::Simple(6, -1, .2);

  const HistAxis kRecoLEAxis("Reco L / E (km / GeV)", kRecoLEBins);
  const HistAxis kRecoCosZenithAxis("Reco cos(#theta_{zenith})", kRecoCosZenithBins);

  const HistAxis k2DRecoAxis(kRecoLEAxis, kRecoCosZenithAxis);

  const Var2DMapper mapper(kRecoLEBins, kRecoCosZenithBins);

  //---------------------------------------------------------------------------
  PredictAtmos::PredictAtmos()
    : fTauFromMu(kNullLoader, kRecoLEAxis, kNoCut),
      fAntiTauFromMu(kNullLoader, kRecoLEAxis, kNoCut),
      fNC(kNullLoader, kRecoLEAxis, kNoCut)
    /*
      fTauFromMu(kNullLoader, kRecoCosZenithAxis, kNoCut),
      fAntiTauFromMu(kNullLoader, kRecoCosZenithAxis, kNoCut),
      fNC(kNullLoader, kRecoCosZenithAxis, kNoCut)
    */
      /*
      fTauFromMu(kNullLoader, k2DRecoAxis, kNoCut),
      fAntiTauFromMu(kNullLoader, k2DRecoAxis, kNoCut),
      fNC(kNullLoader, k2DRecoAxis, kNoCut)
      */
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

      //      fTauFromMu.Fill(mapper.Map(recoLE, recocos), 1000*E/Lnu, .37 * h->GetBinContent(coord[0], coord[1]) * prob);
      //      fAntiTauFromMu.Fill(mapper.Map(recoLE, recocos), 1000*E/Lnu, .37 *ah->GetBinContent(coord[0], coord[1]) * prob);

      fTauFromMu.Fill(recoLE, 1000*E/Lnu, .37 * h->GetBinContent(coord[0], coord[1]) * prob);
      fAntiTauFromMu.Fill(recoLE, 1000*E/Lnu, .37 *ah->GetBinContent(coord[0], coord[1]) * prob);
    }


    h = (TH2*)fin.Get("hNC_UnOsc");
    h->Add((TH2*)fin.Get("hANC_UnOsc"));
    smear = (THnSparseD*)fin.Get("hMigrationMatrixNC");

    for(int i = 0; i < smear->GetNbins(); ++i){
      int coord[4];
      const double prob = smear->GetBinContent(i, coord);

      const double recoLE = smear->GetAxis(2)->GetBinCenter(coord[2]);
      const double recocos = smear->GetAxis(3)->GetBinCenter(coord[3]);

      //      fNC.Fill(mapper.Map(recoLE, recocos), .005 * h->GetBinContent(coord[0], coord[1])*prob);

      fNC.Fill(recoLE, .005 * h->GetBinContent(coord[0], coord[1])*prob);
    }

    //    fTauFromMu.OverrideLivetime(400); // kt*yr
    fTauFromMu.fPOT = 400;
    fAntiTauFromMu.fPOT = 400;
    //    fTauFromMu.fLivetime = 400;

    fNC.OverridePOT(400);
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
