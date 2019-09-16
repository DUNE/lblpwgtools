#include "CAFAna/Systs/XSecSysts.h"

#include "CAFAna/Analysis/TDRLoaders.h"
#include "CAFAna/Analysis/CalcsNuFit.h"

#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Prediction/PredictionNoOsc.h"
#include "CAFAna/Prediction/PredictionInterp.h"

#include "CAFAna/Core/Ratio.h"

#include "CAFAna/Cuts/AnaCuts.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/Systs/DUNEFluxSysts.h"
#include "CAFAna/Systs/EnergySysts.h"
#include "CAFAna/Systs/NDRecoSysts.h"

#include "OscLib/func/IOscCalculator.h"

using namespace ana;

#include "TCanvas.h"

const Binning binsFDEreco = Binning::Simple(80, 0, 10);
const Var kRecoE_numu = SIMPLEVAR(dune.Ev_reco_numu);
const Var kRecoE_ND = SIMPLEVAR(dune.Ev_reco);
const HistAxis axRecoEnuFDnumu("Reco energy (GeV)", binsFDEreco, kRecoE_numu);
const HistAxis axRecoEnuNDnumu("Reco energy (GeV)", binsFDEreco, kRecoE_ND);

const Var kGENIEWeights = SIMPLEVAR(dune.total_cv_wgt);

std::vector<const ISyst*> GetListOfSysts(bool nd)
{
  std::vector<const ISyst*> systlist;

  std::vector<const ISyst*> fluxlist = GetDUNEFluxSysts(10, true);
  systlist.insert(systlist.end(), fluxlist.begin(), fluxlist.end());

  if(nd){
    std::vector<const ISyst*> detlist_nd  = {&kEnergyScaleMuSystND, &kChargedHadUncorrNDSyst, &kNUncorrNDSyst,
                                             &kPi0UncorrNDSyst, &kRecoNCSyst, &kLeptonAccSyst, &kHadronAccSyst};
    systlist.insert(systlist.end(), detlist_nd.begin(), detlist_nd.end());
  }
  else{
    std::vector<const ISyst*> detlist_dis  = {&keScaleMuLArSyst, &kChargedHadCorrSyst,
                                              &kChargedHadUncorrFDSyst, &kNUncorrFDSyst,
                                              &kEnergyScalePi0Syst, &kPi0UncorrFDSyst};
    systlist.insert(systlist.end(), detlist_dis.begin(), detlist_dis.end());
  }

  std::vector<const ISyst*> xseclist = GetXSecSysts({}, true);
  systlist.insert(systlist.end(), xseclist.begin(), xseclist.end());

  return systlist;
};

void test_systs(bool nd = false, bool reload = false)
{
  osc::IOscCalculatorAdjustable* calc = NuFitOscCalc(1);

  const std::vector<const ISyst*> systs = GetListOfSysts(nd);
  for(const ISyst* s: systs) std::cout << s->ShortName() << " " << s->LatexName() << std::endl;

  if(reload){
    TDRLoaders loaders(Loaders::kFHC);

    IPredictionGenerator* gen = 0;
    if(nd){
      gen = new NoOscPredictionGenerator(axRecoEnuNDnumu, kPassND_FHC_NUMU, kGENIEWeights);
    }
    else{
      gen = new NoExtrapPredictionGenerator(axRecoEnuFDnumu, kPassFD_CVN_NUMU && kIsTrueFV, kGENIEWeights);
    }

    PredictionInterp pred(systs, calc, *gen, loaders);

    loaders.Go();

    SaveToFile(pred, TString::Format("test_%s_systs.root", nd ? "nd" : "fd").Data(), "pred");
  }

  IPrediction* pred = LoadFromFile<IPrediction>(TString::Format("test_%s_systs.root", nd ? "nd" : "fd").Data(), "pred").release();

  Spectrum nom = pred->Predict(calc);

  new TCanvas;
  gPad->Print("test_systs.pdf[");

  for(const ISyst* s: systs){
    for(int sigma = 1; sigma <= 3; ++sigma){
      Spectrum up = pred->PredictSyst(calc, SystShifts(s, +sigma));
      Spectrum dn = pred->PredictSyst(calc, SystShifts(s, -sigma));

      Ratio rup(up, nom);
      Ratio rdn(dn, nom);

      TH1* hup = rup.ToTH1(kRed+sigma-1);
      hup->Draw(sigma == 1 ? "hist" : "hist same");
      hup->SetTitle(s->LatexName().c_str());
      hup->GetYaxis()->SetRangeUser(.8, 1.2);
      rdn.ToTH1(kBlue+sigma-1)->Draw("hist same");
    }

    gPad->Print("test_systs.pdf");
  }

  gPad->Print("test_systs.pdf]");
}
