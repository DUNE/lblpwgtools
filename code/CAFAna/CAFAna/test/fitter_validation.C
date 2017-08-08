#ifdef __CINT__
void fitter_validation(bool reload = false){}
#else

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Systs/DUNEFluxSysts.h"
#include "CAFAna/Systs/Systs.h"

using namespace ana;

#include "Utilities/rootlogon.C"

#include "OscLib/func/OscCalculatorPMNSOpt.h"

#include "StandardRecord/StandardRecord.h"

#include "TFile.h"
#include "TH1.h"

const Var kRecoE = SIMPLEVAR(dune.Ev_reco);
const Var kPIDFD = SIMPLEVAR(dune.mvaresult);

// 250 MeV bins from 0 to 8GeV
const HistAxis axis("Reconstructed energy (GeV)",
                    Binning::Simple(32, 0, 8),
                    kRecoE);

// POT/yr * 5yrs * mass correction
const double potFD = 5 * 1.47e21 * 40/1.13;

const char* stateFname = "fitter_validation_state.root";
const char* outputFname = "fitter_validation_cafana.root";

// http://www.nu-fit.org/?q=node/139
osc::IOscCalculatorAdjustable* NuFitOscCalc(int hie)
{
  assert(hie == +1 || hie == -1);

  osc::IOscCalculatorAdjustable* ret = new osc::OscCalculatorPMNSOpt;
  ret->SetL(1300);
  ret->SetRho(2.95674); // g/cm^3. Dan Cherdack's doc "used in GLOBES"

  ret->SetDmsq21(7.50e-5);
  ret->SetTh12(33.56*TMath::Pi()/180);

  if(hie > 0){
    ret->SetDmsq32(+2.524e-3);
    ret->SetTh23(41.6*TMath::Pi()/180);
    ret->SetTh13(8.46*TMath::Pi()/180);
    ret->SetdCP(261*TMath::Pi()/180);
  }
  else{
    ret->SetDmsq32(-2.514e-3);
    ret->SetTh23(50.0*TMath::Pi()/180);
    ret->SetTh13(8.49*TMath::Pi()/180);
    ret->SetdCP(277*TMath::Pi()/180);
  }

  return ret;
}

void table(FILE* f, IPrediction* p, osc::IOscCalculator* calc)
{
  TH1* hnue = p->PredictComponent(calc, Flavors::kNuMuToNuE, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* hnuebar = p->PredictComponent(calc, Flavors::kNuMuToNuE, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* hnumu = p->PredictComponent(calc, Flavors::kNuMuToNuMu, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* hnumubar = p->PredictComponent(calc, Flavors::kNuMuToNuMu, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* hnutau = p->PredictComponent(calc, Flavors::kNuMuToNuTau, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* hnutaubar = p->PredictComponent(calc, Flavors::kNuMuToNuTau, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* hbeamnue = p->PredictComponent(calc, Flavors::kNuEToNuE, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* hbeamnuebar = p->PredictComponent(calc, Flavors::kNuEToNuE, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* hnc = p->PredictComponent(calc, Flavors::kAll, Current::kNC, Sign::kBoth).ToTH1(potFD);

  fprintf(f, "E_reco\t\tnue\tnuebar\tnumu\tnumubar\tnutau\ttaubar\tbeamnue\tbeambar\tNC\n");

  for(int i = 1; i <= hnc->GetNbinsX(); ++i){
    fprintf(f, "%g < E < %g\t",
           hnc->GetXaxis()->GetBinLowEdge(i),
           hnc->GetXaxis()->GetBinUpEdge(i));

    fprintf(f, "%.2lf\t%.2lf\t%.2lf\t%.2lf\t%.2lf\t%.2lf\t%.2lf\t%.2lf\t%.2lf\n",
            hnue->GetBinContent(i),
            hnuebar->GetBinContent(i),
            hnumu->GetBinContent(i),
            hnumubar->GetBinContent(i),
            hnutau->GetBinContent(i),
            hnutaubar->GetBinContent(i),
            hbeamnue->GetBinContent(i),
            hbeamnuebar->GetBinContent(i),
            hnc->GetBinContent(i));
  }
}

void fitter_validation(bool reload = false)
{
  rootlogon(); // style

  if(reload || TFile(stateFname).IsZombie()){
    SpectrumLoader loaderFDNumuFHC("/pnfs/dune/persistent/TaskForce_AnaTree/far/train/v2.1/numutest.root");
    SpectrumLoader loaderFDNueFHC("/pnfs/dune/persistent/TaskForce_AnaTree/far/train/v2.1/nuetest.root");

    auto* loaderFDNumuFHCBeam  = loaderFDNumuFHC.LoaderForRunPOT(20000001);
    auto* loaderFDNumuFHCNue   = loaderFDNumuFHC.LoaderForRunPOT(20000002);
    auto* loaderFDNumuFHCNuTau = loaderFDNumuFHC.LoaderForRunPOT(20000003);
    auto* loaderFDNumuFHCNC    = loaderFDNumuFHC.LoaderForRunPOT(0);

    auto* loaderFDNueFHCBeam  = loaderFDNueFHC.LoaderForRunPOT(20000001);
    auto* loaderFDNueFHCNue   = loaderFDNueFHC.LoaderForRunPOT(20000002);
    auto* loaderFDNueFHCNuTau = loaderFDNueFHC.LoaderForRunPOT(20000003);
    auto* loaderFDNueFHCNC    = loaderFDNueFHC.LoaderForRunPOT(0);

    SpectrumLoader loaderFDNumuRHC("/pnfs/dune/persistent/TaskForce_AnaTree/far/train/v2.1/anumutest.root");
    SpectrumLoader loaderFDNueRHC("/pnfs/dune/persistent/TaskForce_AnaTree/far/train/v2.1/anuetest.root");

    auto* loaderFDNumuRHCBeam  = loaderFDNumuRHC.LoaderForRunPOT(20000004);
    auto* loaderFDNumuRHCNue   = loaderFDNumuRHC.LoaderForRunPOT(20000005);
    auto* loaderFDNumuRHCNuTau = loaderFDNumuRHC.LoaderForRunPOT(20000006);
    auto* loaderFDNumuRHCNC    = loaderFDNumuRHC.LoaderForRunPOT(0);
    
    auto* loaderFDNueRHCBeam  = loaderFDNueRHC.LoaderForRunPOT(20000004);
    auto* loaderFDNueRHCNue   = loaderFDNueRHC.LoaderForRunPOT(20000005);
    auto* loaderFDNueRHCNuTau = loaderFDNueRHC.LoaderForRunPOT(20000006);
    auto* loaderFDNueRHCNC    = loaderFDNueRHC.LoaderForRunPOT(0);

    PredictionNoExtrap predFDNumuFHC(*loaderFDNumuFHCBeam, 
                                     *loaderFDNumuFHCNue,
                                     *loaderFDNumuFHCNuTau,
                                     *loaderFDNumuFHCNC,
                                     axis,
                                     kPIDFD > 0.8);

    PredictionNoExtrap predFDNueFHC(*loaderFDNueFHCBeam, 
                                    *loaderFDNueFHCNue,
                                    *loaderFDNueFHCNuTau,
                                    *loaderFDNueFHCNC,
                                    axis,
                                    kPIDFD > 0.8);

    PredictionNoExtrap predFDNumuRHC(*loaderFDNumuRHCBeam, 
                                     *loaderFDNumuRHCNue,
                                     *loaderFDNumuRHCNuTau,
                                     *loaderFDNumuRHCNC,
                                     axis,
                                     kPIDFD > 0.8);

    PredictionNoExtrap predFDNueRHC(*loaderFDNueRHCBeam, 
                                    *loaderFDNueRHCNue,
                                    *loaderFDNueRHCNuTau,
                                    *loaderFDNueRHCNC,
                                    axis,
                                    kPIDFD > 0.8);


    // Numbers are after sections in Dan's document
    const SystShifts shifts2a(&kNCSyst, +1);

    PredictionNoExtrap predFDNumuFHC2a(*loaderFDNumuFHCBeam, 
                                       *loaderFDNumuFHCNue,
                                       *loaderFDNumuFHCNuTau,
                                       *loaderFDNumuFHCNC,
                                       axis,
                                       kPIDFD > 0.8,
                                       shifts2a);

    PredictionNoExtrap predFDNueFHC2a(*loaderFDNueFHCBeam, 
                                      *loaderFDNueFHCNue,
                                      *loaderFDNueFHCNuTau,
                                      *loaderFDNueFHCNC,
                                      axis,
                                      kPIDFD > 0.8,
                                      shifts2a);

    PredictionNoExtrap predFDNumuRHC2a(*loaderFDNumuRHCBeam, 
                                       *loaderFDNumuRHCNue,
                                       *loaderFDNumuRHCNuTau,
                                       *loaderFDNumuRHCNC,
                                       axis,
                                       kPIDFD > 0.8,
                                       shifts2a);

    PredictionNoExtrap predFDNueRHC2a(*loaderFDNueRHCBeam, 
                                      *loaderFDNueRHCNue,
                                      *loaderFDNueRHCNuTau,
                                      *loaderFDNueRHCNC,
                                      axis,
                                      kPIDFD > 0.8,
                                      shifts2a);

    // TODO - this isn't exactly Dan's prescription
    DUNEFluxSystVector fsv = GetDUNEFluxSysts(256);
    SystShifts shifts2b;
    gRandom->SetSeed(42);
    for(const ISyst* s: fsv) shifts2b.SetShift(s, gRandom->Gaus(0, 1));

    PredictionNoExtrap predFDNumuFHC2b(*loaderFDNumuFHCBeam, 
                                       *loaderFDNumuFHCNue,
                                       *loaderFDNumuFHCNuTau,
                                       *loaderFDNumuFHCNC,
                                       axis,
                                       kPIDFD > 0.8,
                                       shifts2b);

    PredictionNoExtrap predFDNueFHC2b(*loaderFDNueFHCBeam, 
                                      *loaderFDNueFHCNue,
                                      *loaderFDNueFHCNuTau,
                                      *loaderFDNueFHCNC,
                                      axis,
                                      kPIDFD > 0.8,
                                      shifts2b);

    PredictionNoExtrap predFDNumuRHC2b(*loaderFDNumuRHCBeam, 
                                       *loaderFDNumuRHCNue,
                                       *loaderFDNumuRHCNuTau,
                                       *loaderFDNumuRHCNC,
                                       axis,
                                       kPIDFD > 0.8,
                                       shifts2b);

    PredictionNoExtrap predFDNueRHC2b(*loaderFDNueRHCBeam, 
                                      *loaderFDNueRHCNue,
                                      *loaderFDNueRHCNuTau,
                                      *loaderFDNueRHCNC,
                                      axis,
                                      kPIDFD > 0.8,
                                      shifts2b);


    loaderFDNumuFHC.Go();
    loaderFDNueFHC.Go();
    loaderFDNumuRHC.Go();
    loaderFDNueRHC.Go();

    TFile fout(stateFname, "RECREATE");
    predFDNumuFHC.SaveTo(fout.mkdir("fd_numu_fhc"));
    predFDNueFHC.SaveTo(fout.mkdir("fd_nue_fhc"));
    predFDNumuRHC.SaveTo(fout.mkdir("fd_numu_rhc"));
    predFDNueRHC.SaveTo(fout.mkdir("fd_nue_rhc"));

    predFDNumuFHC2a.SaveTo(fout.mkdir("fd_numu_fhc_2a"));
    predFDNueFHC2a.SaveTo(fout.mkdir("fd_nue_fhc_2a"));
    predFDNumuRHC2a.SaveTo(fout.mkdir("fd_numu_rhc_2a"));
    predFDNueRHC2a.SaveTo(fout.mkdir("fd_nue_rhc_2a"));

    predFDNumuFHC2b.SaveTo(fout.mkdir("fd_numu_fhc_2b"));
    predFDNueFHC2b.SaveTo(fout.mkdir("fd_nue_fhc_2b"));
    predFDNumuRHC2b.SaveTo(fout.mkdir("fd_numu_rhc_2b"));
    predFDNueRHC2b.SaveTo(fout.mkdir("fd_nue_rhc_2b"));

    std::cout << "Saved state to " << stateFname << std::endl;
  }
  else{
    std::cout << "Loading state from " << stateFname << std::endl;
  }

  TFile fin(stateFname);
  PredictionNoExtrap& predFDNumuFHC = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_numu_fhc")).release();
  PredictionNoExtrap& predFDNueFHC = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_nue_fhc")).release();
  PredictionNoExtrap& predFDNumuRHC = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_numu_rhc")).release();
  PredictionNoExtrap& predFDNueRHC = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_nue_rhc")).release();

  IPrediction* predFDNumuFHC2a = ana::LoadFrom<IPrediction>(fin.GetDirectory("fd_numu_fhc_2a")).release();
  IPrediction* predFDNueFHC2a = ana::LoadFrom<IPrediction>(fin.GetDirectory("fd_nue_fhc_2a")).release();
  IPrediction* predFDNumuRHC2a = ana::LoadFrom<IPrediction>(fin.GetDirectory("fd_numu_rhc_2a")).release();
  IPrediction* predFDNueRHC2a = ana::LoadFrom<IPrediction>(fin.GetDirectory("fd_nue_rhc_2a")).release();

  IPrediction* predFDNumuFHC2b = ana::LoadFrom<IPrediction>(fin.GetDirectory("fd_numu_fhc_2b")).release();
  IPrediction* predFDNueFHC2b = ana::LoadFrom<IPrediction>(fin.GetDirectory("fd_nue_fhc_2b")).release();
  IPrediction* predFDNumuRHC2b = ana::LoadFrom<IPrediction>(fin.GetDirectory("fd_numu_rhc_2b")).release();
  IPrediction* predFDNueRHC2b = ana::LoadFrom<IPrediction>(fin.GetDirectory("fd_nue_rhc_2b")).release();
  fin.Close();
  std::cout << "Done loading state" << std::endl;

  TFile* fout = new TFile(outputFname, "RECREATE");

  for(int hie = -1; hie <= +1; hie += 2){
    osc::IOscCalculatorAdjustable* inputOsc = NuFitOscCalc(hie);
    const std::string hieStr = (hie > 0) ? "nh" : "ih";
    for(int deltaIdx = 0; deltaIdx < 4; ++deltaIdx){
      inputOsc->SetdCP(deltaIdx/2.*TMath::Pi());
      const std::string dcpStr = TString::Format("%gpi", deltaIdx/2.).Data();

      FILE* f = fopen(("numu_fhc_"+hieStr+"_"+dcpStr+".txt").c_str(), "w");
      table(f, &predFDNumuFHC, inputOsc);
      fclose(f);

      f = fopen(("nue_fhc_"+hieStr+"_"+dcpStr+".txt").c_str(), "w");
      table(f, &predFDNueFHC, inputOsc);
      fclose(f);

      f = fopen(("numu_rhc_"+hieStr+"_"+dcpStr+".txt").c_str(), "w");
      table(f, &predFDNumuRHC, inputOsc);
      fclose(f);

      f = fopen(("nue_rhc_"+hieStr+"_"+dcpStr+".txt").c_str(), "w");
      table(f, &predFDNueRHC, inputOsc);
      fclose(f);
      
      TH1* hnumufhc = predFDNumuFHC.Predict(inputOsc).ToTH1(potFD);
      TH1* hnuefhc = predFDNueFHC.Predict(inputOsc).ToTH1(potFD);
      TH1* hnumurhc = predFDNumuFHC.Predict(inputOsc).ToTH1(potFD);
      TH1* hnuerhc = predFDNueFHC.Predict(inputOsc).ToTH1(potFD);

      hnumufhc->Write(("numu_fhc_"+hieStr+"_"+dcpStr).c_str());
      hnuefhc->Write(("nue_fhc_"+hieStr+"_"+dcpStr).c_str());
      hnumurhc->Write(("numu_rhc_"+hieStr+"_"+dcpStr).c_str());
      hnuerhc->Write(("nue_rhc_"+hieStr+"_"+dcpStr).c_str());
    } // end for delta
  } // end for hie

  osc::IOscCalculatorAdjustable* inputOsc = NuFitOscCalc(+1);
  inputOsc->SetdCP(0);

  TH1* hnumufhc2a = predFDNumuFHC2a->Predict(inputOsc).ToTH1(potFD);
  TH1* hnuefhc2a = predFDNueFHC2a->Predict(inputOsc).ToTH1(potFD);
  TH1* hnumurhc2a = predFDNumuFHC2a->Predict(inputOsc).ToTH1(potFD);
  TH1* hnuerhc2a = predFDNueFHC2a->Predict(inputOsc).ToTH1(potFD);

  hnumufhc2a->Write("numu_fhc_2a");
  hnuefhc2a->Write("nue_fhc_2a");
  hnumurhc2a->Write("numu_rhc_2a");
  hnuerhc2a->Write("nue_rhc_2a");

  TH1* hnumufhc2b = predFDNumuFHC2b->Predict(inputOsc).ToTH1(potFD);
  TH1* hnuefhc2b = predFDNueFHC2b->Predict(inputOsc).ToTH1(potFD);
  TH1* hnumurhc2b = predFDNumuFHC2b->Predict(inputOsc).ToTH1(potFD);
  TH1* hnuerhc2b = predFDNueFHC2b->Predict(inputOsc).ToTH1(potFD);

  hnumufhc2b->Write("numu_fhc_2b");
  hnuefhc2b->Write("nue_fhc_2b");
  hnumurhc2b->Write("numu_rhc_2b");
  hnuerhc2b->Write("nue_rhc_2b");

  std::cout << "Wrote " << outputFname << std::endl;
}

#endif
