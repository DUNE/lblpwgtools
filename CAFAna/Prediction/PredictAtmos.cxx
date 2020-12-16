#include "CAFAna/Prediction/PredictAtmos.h"

#include "CAFAna/Core/MathUtil.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/SpectrumLoader.h"

#include "OscLib/IOscCalc.h"

#include "TFile.h"
#include "TH2.h"
#include "THnSparse.h"
#include "TPad.h"

namespace ana
{
  const double kTauEff = .37;
  const double kNCEff = .005;

  const double kExposure = 350; // what the file is scaled to

  const double kRefBaseline = 1000;

  const AtmosSyst kAtmosSyst;

  const Binning kRecoLEBins = Binning::Simple(20, 0, 3000);
  const Binning kRecoCosZenithBins = Binning::Simple(10, -1, .2);

  const HistAxis kRecoLEAxis("Reco L / E (km / GeV)", kRecoLEBins);
  const HistAxis kRecoCosZenithAxis("Reco cos(#theta_{zenith})", kRecoCosZenithBins);

  const HistAxis k2DRecoAxis(kRecoLEAxis, kRecoCosZenithAxis);

  const Var2DMapper recoMapper(kRecoLEBins, kRecoCosZenithBins);


  const Binning kTrueEBins = Binning::Simple(400, 0.1, 100);
  // TODO it's not really clear that 50 cos_zenith bins are enough. Or,
  // shouldn't be bin evenly around the perimeter of the earth instead?
  const Binning kTrueCosZenithBins = Binning::Simple(50, -1, +1);

  const HistAxis kTrueEAxis("True E (GeV)", kTrueEBins);
  const HistAxis kTrueCosZenithAxis("True cos(#theta_{zenith})", kTrueCosZenithBins);

  const HistAxis k2DTrueAxis(kTrueEAxis, kTrueCosZenithAxis);
  const Var2DMapper trueMapper(kTrueEBins, kTrueCosZenithBins);

  //---------------------------------------------------------------------------
  PredictAtmos::PredictAtmos()
    : 
      fTauFromMu(kNullLoader, kRecoLEAxis, kNoCut),
      fAntiTauFromMu(kNullLoader, kRecoLEAxis, kNoCut),
      fTauFromE(kNullLoader, kRecoLEAxis, kNoCut),
      fAntiTauFromE(kNullLoader, kRecoLEAxis, kNoCut),
      fNC(kNullLoader, kRecoLEAxis, kNoCut),
    /*
      fTauFromMu(kNullLoader, kRecoCosZenithAxis, kNoCut),
      fAntiTauFromMu(kNullLoader, kRecoCosZenithAxis, kNoCut),
      fNC(kNullLoader, kRecoCosZenithAxis, kNoCut),
    */
    /*
      fTauFromMu(kNullLoader, k2DRecoAxis, kNoCut),
      fAntiTauFromMu(kNullLoader, k2DRecoAxis, kNoCut),
      fTauFromE(kNullLoader, k2DRecoAxis, kNoCut),
      fAntiTauFromE(kNullLoader, k2DRecoAxis, kNoCut),
      fNC(kNullLoader, k2DRecoAxis, kNoCut),
    */
      fTauFromMuRW(kNullLoader, kRecoLEAxis, k2DTrueAxis, kNoCut)
  {
    //    TFile fin("/pnfs/dune/persistent/users/bckhouse/atmos/DUNEAtmos400ktyr.root");
    TFile fin("/pnfs/dune/persistent/users/bckhouse/atmos/MigrationMatrixHighRes.root");
    TH2*  mh = (TH2*)fin.Get("hNuMu2NuTau_UnOsc");
    TH2* amh = (TH2*)fin.Get("hANuMu2ANuTau_UnOsc");
    TH2*  eh = (TH2*)fin.Get("hNuE2NuTau_UnOsc");
    TH2* aeh = (TH2*)fin.Get("hANuE2ANuTau_UnOsc");
    //    h->SetDirectory(0);
    
    THnSparseD* smear = (THnSparseD*)fin.Get("hMigrationMatrixNuTau");

    Progress prog("Loading migration matrices");

    for(int i = 0; i < smear->GetNbins(); ++i){
      struct {int E, cosQ, recoLE, recoCosQ;} bin;
      const double prob = smear->GetBinContent(i, &bin.E);

      const double E    = mh->GetXaxis()->GetBinCenter(bin.E);
      const double cosQ = mh->GetYaxis()->GetBinCenter(bin.cosQ);
      const double recoLE  = smear->GetAxis(2)->GetBinCenter(bin.recoLE);
      const double recocos = smear->GetAxis(3)->GetBinCenter(bin.recoCosQ);

      const double R = 6371; // km
      const double dR = 15; // production height (guess)
      const double Lnu  = (R+dR)*(sqrt(util::sqr(1+dR/R)+cosQ*cosQ-1)-cosQ);

      const double xval = recoLE;
      //      const double xval = recoMapper.Map(recoLE, recocos);

      const double yval = kRefBaseline*E/Lnu;

      fTauFromMu.Fill    (xval, yval, kTauEff *  mh->GetBinContent(bin.E, bin.cosQ) * prob);
      fAntiTauFromMu.Fill(xval, yval, kTauEff * amh->GetBinContent(bin.E, bin.cosQ) * prob);

      fTauFromE.Fill     (xval, yval, kTauEff *  eh->GetBinContent(bin.E, bin.cosQ) * prob);
      fAntiTauFromE.Fill (xval, yval, kTauEff * aeh->GetBinContent(bin.E, bin.cosQ) * prob);

      fTauFromMuRW.Fill(recoLE, trueMapper.Map(E, cosQ), kTauEff * mh->GetBinContent(bin.E, bin.cosQ) * prob);

      prog.SetProgress(i/(2.*smear->GetNbins()));
    }


    TH2* h = (TH2*)fin.Get("hNC_UnOsc");
    h->Add((TH2*)fin.Get("hANC_UnOsc"));
    smear = (THnSparseD*)fin.Get("hMigrationMatrixNC");

    for(int i = 0; i < smear->GetNbins(); ++i){
      struct {int E, cosQ, recoLE, recoCosQ;} bin;
      const double prob = smear->GetBinContent(i, &bin.E);

      const double recoLE  = smear->GetAxis(2)->GetBinCenter(bin.recoLE);
      const double recocos = smear->GetAxis(3)->GetBinCenter(bin.recoCosQ);

      const double xval = recoLE;
      //      const double xval = recoMapper.Map(recoLE, recocos);

      fNC.Fill(xval, kNCEff * h->GetBinContent(bin.E, bin.cosQ) * prob);

      prog.SetProgress(.5+i/(2.*smear->GetNbins()));
    }

    //    fTauFromMu.OverrideLivetime(kExposure); // kt*yr
    fTauFromMu.fPOT = kExposure;
    fAntiTauFromMu.fPOT = kExposure;
    //    fTauFromMu.fLivetime = kExposure;
    fTauFromE.fPOT = kExposure;
    fAntiTauFromE.fPOT = kExposure;

    fNC.OverridePOT(kExposure);

    fTauFromMuRW.fPOT = kExposure;
  }

  //---------------------------------------------------------------------------
  Spectrum PredictAtmos::PredictSyst(osc::IOscCalc* calc,
                                     const SystShifts& shift) const
  {
    const double scale = std::max(0., 1+.25*shift.GetShift(&kAtmosSyst));

    /*
    osc::IOscCalcAdjustable* ac = (osc::IOscCalcAdjustable*)calc;

    TH2* h = fTauFromMuRW.WeightingVariable().ToTH2(kExposure);
    for(int j = 1; j <= h->GetNbinsY(); ++j){
      const double cosQ = h->GetYaxis()->GetBinCenter(j);
      const double R = 6371; // km
      const double dR = 15; // production height (guess)
      const double Lnu  = (R+dR)*(sqrt(util::sqr(1+dR/R)+cosQ*cosQ-1)-cosQ);

      ac->SetL(Lnu);

      for(int i = 1; i <= h->GetNbinsX(); ++i){
        const double E = h->GetXaxis()->GetBinCenter(i);

        h->SetBinContent(i, j, ac->P(14, 16, E));
      }
    }

    h->Draw("colz");
    h->GetXaxis()->SetRangeUser(0, 40);
    gPad->Print("oscillogram.pdf");
    abort();
    */

    ((osc::IOscCalcAdjustable*)calc)->SetL(kRefBaseline);

    Spectrum ret = (fTauFromMu.Oscillated(calc, 14, 16) +
                    fAntiTauFromMu.Oscillated(calc, -14, -16) +
                    fTauFromE.Oscillated(calc, 12, 16) +
                    fAntiTauFromE.Oscillated(calc, -12, -16) +
                    fNC);

    ret.Scale(scale);

    return ret;
  }
}
