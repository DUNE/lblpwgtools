#include "CAFAna/Atmos/PredictAtmos.h"

#include "CAFAna/Core/MathUtil.h"
//#include "CAFAna/Core/Progress.h"
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
  void FillTauOscSpect(AtmosOscSpect& aos, const std::string& fname, const std::string& histname)
  {
    TFile fin(fname.c_str());
    TH2* h = (TH2*)fin.Get(histname.c_str());
    
    THnSparseD* smear = (THnSparseD*)fin.Get("hMigrationMatrixNuTau");

    for(int i = 0; i < smear->GetNbins(); ++i){
      struct {int E, cosQ, recoLE, recoCosQ;} bin;
      const double prob = smear->GetBinContent(i, &bin.E);

      const double E    = h->GetXaxis()->GetBinCenter(bin.E);
      const double cosQ = h->GetYaxis()->GetBinCenter(bin.cosQ);
      const double recoLE  = smear->GetAxis(2)->GetBinCenter(bin.recoLE);
      const double recocos = smear->GetAxis(3)->GetBinCenter(bin.recoCosQ);

      //      const double R = 6371; // km
      //      const double dR = 15; // production height (guess)
      //      const double Lnu  = (R+dR)*(sqrt(util::sqr(1+dR/R)+cosQ*cosQ-1)-cosQ);

      //      const double xval = recoLE;
      const double xval = recoMapper.Map(recoLE, recocos);

      //      const double yval = kRefBaseline*E/Lnu;

      const double yval = trueMapper.Map(E, cosQ);

      aos.Fill(xval, yval, kTauEff * h->GetBinContent(bin.E, bin.cosQ) * prob);
    } // end for i

    aos.fPOT = kExposure;
  }

  //---------------------------------------------------------------------------
  void FillAtmosNCSpect(Spectrum& s, const std::string& fname, const std::string& histname)
  {
    TFile fin(fname.c_str());
    TH2* h = (TH2*)fin.Get(histname.c_str());

    THnSparseD* smear = (THnSparseD*)fin.Get("hMigrationMatrixNC");

    for(int i = 0; i < smear->GetNbins(); ++i){
      struct {int E, cosQ, recoLE, recoCosQ;} bin;
      const double prob = smear->GetBinContent(i, &bin.E);

      const double recoLE  = smear->GetAxis(2)->GetBinCenter(bin.recoLE);
      const double recocos = smear->GetAxis(3)->GetBinCenter(bin.recoCosQ);

      //const double xval = recoLE;
      const double xval = recoMapper.Map(recoLE, recocos);

      s.Fill(xval, kNCEff * h->GetBinContent(bin.E, bin.cosQ) * prob);
    }

    s.OverridePOT(kExposure);
  }

  //---------------------------------------------------------------------------
  PredictAtmos::PredictAtmos(const std::string& fname)
    : fTauFromMu(k2DRecoAxis),
      fAntiTauFromMu(k2DRecoAxis),
      fTauFromE(k2DRecoAxis),
      fAntiTauFromE(k2DRecoAxis),
      fNC(kNullLoader, k2DRecoAxis, kNoCut)
  {
    FillTauOscSpect(fTauFromMu, fname, "hNuMu2NuTau_UnOsc");
    FillTauOscSpect(fAntiTauFromMu, fname, "hANuMu2ANuTau_UnOsc");
    FillTauOscSpect(fTauFromE, fname, "hNuE2NuTau_UnOsc");
    FillTauOscSpect(fAntiTauFromE, fname, "hANuE2ANuTau_UnOsc");

    // Accumulate both into the same spectrum
    FillAtmosNCSpect(fNC, fname, "hNC_UnOsc");
    FillAtmosNCSpect(fNC, fname, "hANC_UnOsc");

    //      fTauFromMuRW.Fill(recoLE, trueMapper.Map(E, cosQ), kTauEff * mh->GetBinContent(bin.E, bin.cosQ) * prob);
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

    Spectrum ret = (fTauFromMu.Oscillated(calc, 14, 16) +
                    fAntiTauFromMu.Oscillated(calc, -14, -16) +
                    fTauFromE.Oscillated(calc, 12, 16) +
                    fAntiTauFromE.Oscillated(calc, -12, -16) +
                    fNC);

    ret.Scale(scale);

    return ret;
  }
}
