// ETW May 2018
// Standard script for DUNE spectra
// Updated 5/18/2020 to pick up latest CAFAna implemenation including weight CVs

#include "CAFAna/Fit/Fit.h"
#include "CAFAna/Analysis/CalcsNuFit.h"
#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Cuts/AnaCuts.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Prediction/PredictionInterp.h"
#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Systs/Systs.h"
#include "CAFAna/Analysis/AnalysisVars.h"

using namespace ana;

#include "Utilities/rootlogon.C"

#include "OscLib/func/IOscCalculator.h"

#include "StandardRecord/StandardRecord.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH2.h"

// 125 MeV bins from 0.0 to 8GeV
std::vector<double> prebins = {0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1, 
			       1.125, 1.25, 1.375, 1.5, 1.625, 1.75, 1.875, 2, 
			       2.125, 2.25, 2.375, 2.5, 2.625, 2.75, 2.875, 3, 
			       3.125, 3.25, 3.375, 3.5, 3.625, 3.75, 3.875, 4, 
			       4.125, 4.25, 4.375, 4.5, 4.625, 4.75, 4.875, 5, 
			       5.125, 5.25, 5.375, 5.5, 5.625, 5.75, 5.875, 6, 
			       6.125, 6.25, 6.375, 6.5, 6.625, 6.75, 6.875, 7, 
			       7.125, 7.25, 7.375, 7.5, 7.625, 7.75, 7.875, 8, 
			       8.125, 8.25, 8.375, 8.5, 8.625, 8.75, 8.875, 9, 
			       9.125, 9.25, 9.375, 9.5, 9.625, 9.75, 9.875, 10, 
			       11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
			       30, 40, 50, 60, 70, 80, 90, 100, 110};
std::vector<double> postbins = {0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1, 
				1.125, 1.25, 1.375, 1.5, 1.625, 1.75, 1.875, 2, 
				2.125, 2.25, 2.375, 2.5, 2.625, 2.75, 2.875, 3, 
				3.125, 3.25, 3.375, 3.5, 3.625, 3.75, 3.875, 4, 
				4.125, 4.25, 4.375, 4.5, 4.625, 4.75, 4.875, 5, 
				5.125, 5.25, 5.375, 5.5, 5.625,	5.75, 5.875, 6, 
				6.125, 6.25, 6.375, 6.5, 6.625, 6.75, 6.875, 7, 
				7.125, 7.25, 7.375, 7.5, 7.625, 7.75, 7.875, 8, 
				9, 10, 12, 14, 16, 18, 20,
				30, 40, 50, 60, 70, 80, 90, 100, 110};

ana::Binning const bins_pre = ana::Binning::Custom(prebins);
ana::Binning const bins_post = ana::Binning::Custom(postbins);
const HistAxis axis_nue_post("Reconstructed energy (GeV)", bins_post, kRecoE_nue);
const HistAxis axis_numu_post("Reconstructed energy (GeV)", bins_post,kRecoE_numu);
const HistAxis axis_smear_numu("Numu reco", bins_post, kRecoE_numu, "Neutrino E", bins_pre, kTrueEnergy);
const HistAxis axis_smear_nue("Nue reco", bins_post, kRecoE_nue, "Neutrino E", bins_pre, kTrueEnergy);

// POT/yr * 3.5yrs * mass correction
const double potFD = 3.5 * POT120 * 40/1.13;

const char* stateFname = "spec_state_forglobes.root";
const char* outputFname = "spec_hist_forglobes.root";

void spec_for_globes(bool reload = false)
{
  rootlogon(); // style
  
  if(reload || TFile(stateFname).IsZombie()){

    SpectrumLoader loaderFDFHCNonswap("/pnfs/dune/persistent/users/LBL_TDR/CAFs/v4/FD_FHC_nonswap.root");
    SpectrumLoader loaderFDFHCNue("/pnfs/dune/persistent/users/LBL_TDR/CAFs/v4/FD_FHC_nueswap.root");
    SpectrumLoader loaderFDFHCNuTau("/pnfs/dune/persistent/users/LBL_TDR/CAFs/v4/FD_FHC_tauswap.root");

    SpectrumLoader loaderFDRHCNonswap("/pnfs/dune/persistent/users/LBL_TDR/CAFs/v4/FD_RHC_nonswap.root");
    SpectrumLoader loaderFDRHCNue("/pnfs/dune/persistent/users/LBL_TDR/CAFs/v4/FD_RHC_nueswap.root");
    SpectrumLoader loaderFDRHCNuTau("/pnfs/dune/persistent/users/LBL_TDR/CAFs/v4/FD_RHC_tauswap.root");

    Loaders dummyLoaders; // PredictionGenerator insists on this
    osc::IOscCalculatorAdjustable* calc = NuFitOscCalc(1);

    NoExtrapPredictionGenerator genFDNumu(axis_numu_post, kPassFD_CVN_NUMU && kIsTrueFV, kCVXSecWeights);
    NoExtrapPredictionGenerator genFDNue(axis_nue_post, kPassFD_CVN_NUE && kIsTrueFV, kCVXSecWeights);
    NoExtrapPredictionGenerator genFDNumu_fid(axis_numu_post, kIsTrueFV, kCVXSecWeights);
    NoExtrapPredictionGenerator genFDNue_fid(axis_nue_post, kIsTrueFV, kCVXSecWeights);
    NoExtrapPredictionGenerator smearFDNumu(axis_smear_numu, kIsTrueFV, kCVXSecWeights);
    NoExtrapPredictionGenerator smearFDNue(axis_smear_nue, kIsTrueFV, kCVXSecWeights);

    Loaders FD_FHC_loaders;
    Loaders FD_RHC_loaders;
    FD_FHC_loaders .AddLoader(&loaderFDFHCNonswap,  caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNonSwap);
    FD_FHC_loaders .AddLoader(&loaderFDFHCNue,      caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNueSwap);
    FD_FHC_loaders .AddLoader(&loaderFDFHCNuTau,    caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNuTauSwap);
    FD_RHC_loaders .AddLoader(&loaderFDRHCNonswap,  caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNonSwap);
    FD_RHC_loaders .AddLoader(&loaderFDRHCNue,      caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNueSwap);
    FD_RHC_loaders .AddLoader(&loaderFDRHCNuTau,    caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNuTauSwap);

    //No systematics for this
    std::vector<const ISyst*> systlist;

    PredictionInterp predInt_FDNumuFHC(systlist, calc, genFDNumu, FD_FHC_loaders);
    PredictionInterp predInt_FDNumuRHC(systlist, calc, genFDNumu, FD_RHC_loaders);
    PredictionInterp predInt_FDNueFHC(systlist, calc, genFDNue, FD_FHC_loaders);
    PredictionInterp predInt_FDNueRHC(systlist, calc, genFDNue, FD_RHC_loaders);
    PredictionInterp predInt_FDNumuFHC_fid(systlist, calc, genFDNumu_fid, FD_FHC_loaders);
    PredictionInterp predInt_FDNumuRHC_fid(systlist, calc, genFDNumu_fid, FD_RHC_loaders);
    PredictionInterp predInt_FDNueFHC_fid(systlist, calc, genFDNue_fid, FD_FHC_loaders);
    PredictionInterp predInt_FDNueRHC_fid(systlist, calc, genFDNue_fid, FD_RHC_loaders);
    PredictionInterp predInt_FDNumuFHC_smr(systlist, calc, smearFDNumu, FD_FHC_loaders);
    PredictionInterp predInt_FDNumuRHC_smr(systlist, calc, smearFDNumu, FD_RHC_loaders);
    PredictionInterp predInt_FDNueFHC_smr(systlist, calc, smearFDNue, FD_FHC_loaders);
    PredictionInterp predInt_FDNueRHC_smr(systlist, calc, smearFDNue, FD_RHC_loaders);

    FD_FHC_loaders.Go();
    FD_RHC_loaders.Go();

    std::cout << stateFname << std::endl;
    TFile fout(stateFname, "RECREATE");
    predInt_FDNumuFHC.SaveTo(&fout, "pred_fd_numu_fhc");
    predInt_FDNumuRHC.SaveTo(&fout, "pred_fd_numu_rhc");
    predInt_FDNueFHC.SaveTo(&fout, "pred_fd_nue_fhc");
    predInt_FDNueRHC.SaveTo(&fout, "pred_fd_nue_rhc");
    predInt_FDNumuFHC_fid.SaveTo(&fout, "pred_fd_numu_fhc_fid");
    predInt_FDNumuRHC_fid.SaveTo(&fout, "pred_fd_numu_rhc_fid");
    predInt_FDNueFHC_fid.SaveTo(&fout, "pred_fd_nue_fhc_fid");
    predInt_FDNueRHC_fid.SaveTo(&fout, "pred_fd_nue_rhc_fid");
    predInt_FDNumuFHC_smr.SaveTo(&fout, "pred_fd_numu_fhc_smr");
    predInt_FDNumuRHC_smr.SaveTo(&fout, "pred_fd_numu_rhc_smr");
    predInt_FDNueFHC_smr.SaveTo(&fout, "pred_fd_nue_fhc_smr");
    predInt_FDNueRHC_smr.SaveTo(&fout, "pred_fd_nue_rhc_smr");


    std::cout << "All done making state..." << std::endl;

    }
  else{    
    std::cout << "Loading state from " << stateFname << std::endl; 
  }

  TFile fin(stateFname);
  PredictionInterp& predInt_FDNumuFHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_numu_fhc")).release();
  PredictionInterp& predInt_FDNueFHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_nue_fhc")).release();
  PredictionInterp& predInt_FDNumuRHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_numu_rhc")).release();
  PredictionInterp& predInt_FDNueRHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_nue_rhc")).release();
  PredictionInterp& predInt_FDNumuFHC_fid = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_numu_fhc_fid")).release();
  PredictionInterp& predInt_FDNueFHC_fid  = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_nue_fhc_fid")).release();
  PredictionInterp& predInt_FDNumuRHC_fid = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_numu_rhc_fid")).release();
  PredictionInterp& predInt_FDNueRHC_fid = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_nue_rhc_fid")).release();
  PredictionInterp& predInt_FDNumuFHC_smr = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_numu_fhc_smr")).release();
  PredictionInterp& predInt_FDNueFHC_smr  = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_nue_fhc_smr")).release();
  PredictionInterp& predInt_FDNumuRHC_smr = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_numu_rhc_smr")).release();
  PredictionInterp& predInt_FDNueRHC_smr = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_nue_rhc_smr")).release();

  fin.Close();
  std::cout << "Done loading state" << std::endl;

  TFile* fout = new TFile(outputFname, "RECREATE");

  osc::IOscCalculatorAdjustable* inputOsc = NuFitOscCalc(1);
  inputOsc->SetdCP(0);

  //FHC Dis
  TH1* dis_FHC_numu_sig = predInt_FDNumuFHC.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* dis_FHC_numubar_sig = predInt_FDNumuFHC.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* dis_FHC_NC_bkg = predInt_FDNumuFHC.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kNu).ToTH1(potFD);
  TH1* dis_FHC_aNC_bkg = predInt_FDNumuFHC.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kAntiNu).ToTH1(potFD);
  TH1* dis_FHC_nutau_bkg = predInt_FDNumuFHC.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* dis_FHC_nutaubar_bkg = predInt_FDNumuFHC.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kAntiNu).ToTH1(potFD);

  TH1* fid_dis_FHC_numu_sig = predInt_FDNumuFHC_fid.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* fid_dis_FHC_numubar_sig = predInt_FDNumuFHC_fid.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* fid_dis_FHC_NC_bkg = predInt_FDNumuFHC_fid.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kNu).ToTH1(potFD);
  TH1* fid_dis_FHC_aNC_bkg = predInt_FDNumuFHC_fid.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kAntiNu).ToTH1(potFD);
  TH1* fid_dis_FHC_nutau_bkg = predInt_FDNumuFHC_fid.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* fid_dis_FHC_nutaubar_bkg = predInt_FDNumuFHC_fid.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kAntiNu).ToTH1(potFD);

  TH2* smr_dis_FHC_numu_sig = predInt_FDNumuFHC_smr.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH2(potFD);
  TH2* smr_dis_FHC_numubar_sig = predInt_FDNumuFHC_smr.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH2(potFD);
  TH2* smr_dis_FHC_NC_bkg = predInt_FDNumuFHC_smr.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kNu).ToTH2(potFD);
  TH2* smr_dis_FHC_aNC_bkg = predInt_FDNumuFHC_smr.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kAntiNu).ToTH2(potFD);
  TH2* smr_dis_FHC_nutau_bkg = predInt_FDNumuFHC_smr.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kNu).ToTH2(potFD);
  TH2* smr_dis_FHC_nutaubar_bkg = predInt_FDNumuFHC_smr.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kAntiNu).ToTH2(potFD);

  dis_FHC_numu_sig->Write("dis_FHC_numu_sig");
  dis_FHC_numubar_sig->Write("dis_FHC_numubar_sig");
  dis_FHC_NC_bkg->Write("dis_FHC_NC_bkg");
  dis_FHC_aNC_bkg->Write("dis_FHC_aNC_bkg");
  dis_FHC_nutau_bkg->Write("dis_FHC_nutau_bkg");
  dis_FHC_nutaubar_bkg->Write("dis_FHC_nutaubar_bkg");

  fid_dis_FHC_numu_sig->Write("fid_dis_FHC_numu_sig");
  fid_dis_FHC_numubar_sig->Write("fid_dis_FHC_numubar_sig");
  fid_dis_FHC_NC_bkg->Write("fid_dis_FHC_NC_bkg");
  fid_dis_FHC_aNC_bkg->Write("fid_dis_FHC_aNC_bkg");
  fid_dis_FHC_nutau_bkg->Write("fid_dis_FHC_nutau_bkg");
  fid_dis_FHC_nutaubar_bkg->Write("fid_dis_FHC_nutaubar_bkg");

  smr_dis_FHC_numu_sig->Write("smr_dis_FHC_numu_sig");
  smr_dis_FHC_numubar_sig->Write("smr_dis_FHC_numubar_sig");
  smr_dis_FHC_NC_bkg->Write("smr_dis_FHC_NC_bkg");
  smr_dis_FHC_aNC_bkg->Write("smr_dis_FHC_aNC_bkg");
  smr_dis_FHC_nutau_bkg->Write("smr_dis_FHC_nutau_bkg");
  smr_dis_FHC_nutaubar_bkg->Write("smr_dis_FHC_nutaubar_bkg");

  std::cout << "Done DIS FHC" << std::endl;


  //RHC Dis
  TH1* dis_RHC_numu_sig = predInt_FDNumuRHC.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* dis_RHC_numubar_sig = predInt_FDNumuRHC.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* dis_RHC_NC_bkg = predInt_FDNumuRHC.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kNu).ToTH1(potFD);
  TH1* dis_RHC_aNC_bkg = predInt_FDNumuRHC.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kAntiNu).ToTH1(potFD);
  TH1* dis_RHC_nutau_bkg = predInt_FDNumuRHC.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* dis_RHC_nutaubar_bkg = predInt_FDNumuRHC.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kAntiNu).ToTH1(potFD);

  TH1* fid_dis_RHC_numu_sig = predInt_FDNumuRHC_fid.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* fid_dis_RHC_numubar_sig = predInt_FDNumuRHC_fid.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* fid_dis_RHC_NC_bkg = predInt_FDNumuRHC_fid.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kNu).ToTH1(potFD);
  TH1* fid_dis_RHC_aNC_bkg = predInt_FDNumuRHC_fid.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kAntiNu).ToTH1(potFD);
  TH1* fid_dis_RHC_nutau_bkg = predInt_FDNumuRHC_fid.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* fid_dis_RHC_nutaubar_bkg = predInt_FDNumuRHC_fid.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kAntiNu).ToTH1(potFD);

  TH2* smr_dis_RHC_numu_sig = predInt_FDNumuRHC_smr.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH2(potFD);
  TH2* smr_dis_RHC_numubar_sig = predInt_FDNumuRHC_smr.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH2(potFD);
  TH2* smr_dis_RHC_NC_bkg = predInt_FDNumuRHC_smr.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kNu).ToTH2(potFD);
  TH2* smr_dis_RHC_aNC_bkg = predInt_FDNumuRHC_smr.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kAntiNu).ToTH2(potFD);
  TH2* smr_dis_RHC_nutau_bkg = predInt_FDNumuRHC_smr.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kNu).ToTH2(potFD);
  TH2* smr_dis_RHC_nutaubar_bkg = predInt_FDNumuRHC_smr.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kAntiNu).ToTH2(potFD);

  dis_RHC_numu_sig->Write("dis_RHC_numu_sig");
  dis_RHC_numubar_sig->Write("dis_RHC_numubar_sig");
  dis_RHC_NC_bkg->Write("dis_RHC_NC_bkg");
  dis_RHC_aNC_bkg->Write("dis_RHC_aNC_bkg");
  dis_RHC_nutau_bkg->Write("dis_RHC_nutau_bkg");
  dis_RHC_nutaubar_bkg->Write("dis_RHC_nutaubar_bkg");

  fid_dis_RHC_numu_sig->Write("fid_dis_RHC_numu_sig");
  fid_dis_RHC_numubar_sig->Write("fid_dis_RHC_numubar_sig");
  fid_dis_RHC_NC_bkg->Write("fid_dis_RHC_NC_bkg");
  fid_dis_RHC_aNC_bkg->Write("fid_dis_RHC_aNC_bkg");
  fid_dis_RHC_nutau_bkg->Write("fid_dis_RHC_nutau_bkg");
  fid_dis_RHC_nutaubar_bkg->Write("fid_dis_RHC_nutaubar_bkg");

  smr_dis_RHC_numu_sig->Write("smr_dis_RHC_numu_sig");
  smr_dis_RHC_numubar_sig->Write("smr_dis_RHC_numubar_sig");
  smr_dis_RHC_NC_bkg->Write("smr_dis_RHC_NC_bkg");
  smr_dis_RHC_aNC_bkg->Write("smr_dis_RHC_aNC_bkg");
  smr_dis_RHC_nutau_bkg->Write("smr_dis_RHC_nutau_bkg");
  smr_dis_RHC_nutaubar_bkg->Write("smr_dis_RHC_nutaubar_bkg");

  std::cout << "Done DIS RHC" << std::endl;

  //FHC App
  TH1* app_FHC_nue_sig = predInt_FDNueFHC.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* app_FHC_nuebar_sig = predInt_FDNueFHC.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* app_FHC_NC_bkg = predInt_FDNueFHC.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kNu).ToTH1(potFD);
  TH1* app_FHC_aNC_bkg = predInt_FDNueFHC.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kAntiNu).ToTH1(potFD);
  TH1* app_FHC_nue_bkg = predInt_FDNueFHC.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* app_FHC_nuebar_bkg = predInt_FDNueFHC.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* app_FHC_nutau_bkg = predInt_FDNueFHC.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* app_FHC_nutaubar_bkg = predInt_FDNueFHC.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* app_FHC_numu_bkg = predInt_FDNueFHC.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* app_FHC_numubar_bkg = predInt_FDNueFHC.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH1(potFD);

  TH1* fid_app_FHC_nue_sig = predInt_FDNueFHC_fid.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* fid_app_FHC_nuebar_sig = predInt_FDNueFHC_fid.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* fid_app_FHC_NC_bkg = predInt_FDNueFHC_fid.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kNu).ToTH1(potFD);
  TH1* fid_app_FHC_aNC_bkg = predInt_FDNueFHC_fid.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kAntiNu).ToTH1(potFD);
  TH1* fid_app_FHC_nue_bkg = predInt_FDNueFHC_fid.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* fid_app_FHC_nuebar_bkg = predInt_FDNueFHC_fid.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* fid_app_FHC_nutau_bkg = predInt_FDNueFHC_fid.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* fid_app_FHC_nutaubar_bkg = predInt_FDNueFHC_fid.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* fid_app_FHC_numu_bkg = predInt_FDNueFHC_fid.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* fid_app_FHC_numubar_bkg = predInt_FDNueFHC_fid.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH1(potFD);

  TH2* smr_app_FHC_nue_sig = predInt_FDNueFHC_smr.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kNu).ToTH2(potFD);
  TH2* smr_app_FHC_nuebar_sig = predInt_FDNueFHC_smr.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kAntiNu).ToTH2(potFD);
  TH2* smr_app_FHC_NC_bkg = predInt_FDNueFHC_smr.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kNu).ToTH2(potFD);
  TH2* smr_app_FHC_aNC_bkg = predInt_FDNueFHC_smr.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kAntiNu).ToTH2(potFD);
  TH2* smr_app_FHC_nue_bkg = predInt_FDNueFHC_smr.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kNu).ToTH2(potFD);
  TH2* smr_app_FHC_nuebar_bkg = predInt_FDNueFHC_smr.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kAntiNu).ToTH2(potFD);
  TH2* smr_app_FHC_nutau_bkg = predInt_FDNueFHC_smr.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kNu).ToTH2(potFD);
  TH2* smr_app_FHC_nutaubar_bkg = predInt_FDNueFHC_smr.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kAntiNu).ToTH2(potFD);
  TH2* smr_app_FHC_numu_bkg = predInt_FDNueFHC_smr.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH2(potFD);
  TH2* smr_app_FHC_numubar_bkg = predInt_FDNueFHC_smr.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH2(potFD);

  app_FHC_nue_sig->Write("app_FHC_nue_sig");
  app_FHC_nuebar_sig->Write("app_FHC_nuebar_sig");
  app_FHC_NC_bkg->Write("app_FHC_NC_bkg");
  app_FHC_aNC_bkg->Write("app_FHC_aNC_bkg");
  app_FHC_nue_bkg->Write("app_FHC_nue_bkg");
  app_FHC_nuebar_bkg->Write("app_FHC_nuebar_bkg");
  app_FHC_nutau_bkg->Write("app_FHC_nutau_bkg");
  app_FHC_nutaubar_bkg->Write("app_FHC_nutaubar_bkg");
  app_FHC_numu_bkg->Write("app_FHC_numu_bkg");
  app_FHC_numubar_bkg->Write("app_FHC_numubar_bkg");

  fid_app_FHC_nue_sig->Write("fid_app_FHC_nue_sig");
  fid_app_FHC_nuebar_sig->Write("fid_app_FHC_nuebar_sig");
  fid_app_FHC_NC_bkg->Write("fid_app_FHC_NC_bkg");
  fid_app_FHC_aNC_bkg->Write("fid_app_FHC_aNC_bkg");
  fid_app_FHC_nue_bkg->Write("fid_app_FHC_nue_bkg");
  fid_app_FHC_nuebar_bkg->Write("fid_app_FHC_nuebar_bkg");
  fid_app_FHC_nutau_bkg->Write("fid_app_FHC_nutau_bkg");
  fid_app_FHC_nutaubar_bkg->Write("fid_app_FHC_nutaubar_bkg");
  fid_app_FHC_numu_bkg->Write("fid_app_FHC_numu_bkg");
  fid_app_FHC_numubar_bkg->Write("fid_app_FHC_numubar_bkg");

  smr_app_FHC_nue_sig->Write("smr_app_FHC_nue_sig");
  smr_app_FHC_nuebar_sig->Write("smr_app_FHC_nuebar_sig");
  smr_app_FHC_NC_bkg->Write("smr_app_FHC_NC_bkg");
  smr_app_FHC_aNC_bkg->Write("smr_app_FHC_aNC_bkg");
  smr_app_FHC_nue_bkg->Write("smr_app_FHC_nue_bkg");
  smr_app_FHC_nuebar_bkg->Write("smr_app_FHC_nuebar_bkg");
  smr_app_FHC_nutau_bkg->Write("smr_app_FHC_nutau_bkg");
  smr_app_FHC_nutaubar_bkg->Write("smr_app_FHC_nutaubar_bkg");
  smr_app_FHC_numu_bkg->Write("smr_app_FHC_numu_bkg");
  smr_app_FHC_numubar_bkg->Write("smr_app_FHC_numubar_bkg");

  std::cout << "Done APP FHC" << std::endl;

  //RHC App
  TH1* app_RHC_nue_sig = predInt_FDNueRHC.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* app_RHC_nuebar_sig = predInt_FDNueRHC.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* app_RHC_NC_bkg = predInt_FDNueRHC.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kNu).ToTH1(potFD);
  TH1* app_RHC_aNC_bkg = predInt_FDNueRHC.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kAntiNu).ToTH1(potFD);
  TH1* app_RHC_nue_bkg = predInt_FDNueRHC.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* app_RHC_nuebar_bkg = predInt_FDNueRHC.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* app_RHC_nutau_bkg = predInt_FDNueRHC.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* app_RHC_nutaubar_bkg = predInt_FDNueRHC.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* app_RHC_numu_bkg = predInt_FDNueRHC.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* app_RHC_numubar_bkg = predInt_FDNueRHC.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH1(potFD);

  TH1* fid_app_RHC_nue_sig = predInt_FDNueRHC_fid.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* fid_app_RHC_nuebar_sig = predInt_FDNueRHC_fid.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* fid_app_RHC_NC_bkg = predInt_FDNueRHC_fid.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kNu).ToTH1(potFD);
  TH1* fid_app_RHC_aNC_bkg = predInt_FDNueRHC_fid.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kAntiNu).ToTH1(potFD);
  TH1* fid_app_RHC_nue_bkg = predInt_FDNueRHC_fid.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* fid_app_RHC_nuebar_bkg = predInt_FDNueRHC_fid.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* fid_app_RHC_nutau_bkg = predInt_FDNueRHC_fid.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* fid_app_RHC_nutaubar_bkg = predInt_FDNueRHC_fid.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kAntiNu).ToTH1(potFD);
  TH1* fid_app_RHC_numu_bkg = predInt_FDNueRHC_fid.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH1(potFD);
  TH1* fid_app_RHC_numubar_bkg = predInt_FDNueRHC_fid.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH1(potFD);

  TH2* smr_app_RHC_nue_sig = predInt_FDNueRHC_smr.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kNu).ToTH2(potFD);
  TH2* smr_app_RHC_nuebar_sig = predInt_FDNueRHC_smr.PredictComponent(inputOsc, Flavors::kNuMuToNuE, Current::kCC, Sign::kAntiNu).ToTH2(potFD);
  TH2* smr_app_RHC_NC_bkg = predInt_FDNueRHC_smr.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kNu).ToTH2(potFD);
  TH2* smr_app_RHC_aNC_bkg = predInt_FDNueRHC_smr.PredictComponent(inputOsc, Flavors::kAll, Current::kNC, Sign::kAntiNu).ToTH2(potFD);
  TH2* smr_app_RHC_nue_bkg = predInt_FDNueRHC_smr.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kNu).ToTH2(potFD);
  TH2* smr_app_RHC_nuebar_bkg = predInt_FDNueRHC_smr.PredictComponent(inputOsc, Flavors::kNuEToNuE, Current::kCC, Sign::kAntiNu).ToTH2(potFD);
  TH2* smr_app_RHC_nutau_bkg = predInt_FDNueRHC_smr.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kNu).ToTH2(potFD);
  TH2* smr_app_RHC_nutaubar_bkg = predInt_FDNueRHC_smr.PredictComponent(inputOsc, Flavors::kAllNuTau, Current::kCC, Sign::kAntiNu).ToTH2(potFD);
  TH2* smr_app_RHC_numu_bkg = predInt_FDNueRHC_smr.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kNu).ToTH2(potFD);
  TH2* smr_app_RHC_numubar_bkg = predInt_FDNueRHC_smr.PredictComponent(inputOsc, Flavors::kAllNuMu, Current::kCC, Sign::kAntiNu).ToTH2(potFD);

  app_RHC_nue_sig->Write("app_RHC_nue_sig");
  app_RHC_nuebar_sig->Write("app_RHC_nuebar_sig");
  app_RHC_NC_bkg->Write("app_RHC_NC_bkg");
  app_RHC_aNC_bkg->Write("app_RHC_aNC_bkg");
  app_RHC_nue_bkg->Write("app_RHC_nue_bkg");
  app_RHC_nuebar_bkg->Write("app_RHC_nuebar_bkg");
  app_RHC_nutau_bkg->Write("app_RHC_nutau_bkg");
  app_RHC_nutaubar_bkg->Write("app_RHC_nutaubar_bkg");
  app_RHC_numu_bkg->Write("app_RHC_numu_bkg");
  app_RHC_numubar_bkg->Write("app_RHC_numubar_bkg");

  fid_app_RHC_nue_sig->Write("fid_app_RHC_nue_sig");
  fid_app_RHC_nuebar_sig->Write("fid_app_RHC_nuebar_sig");
  fid_app_RHC_NC_bkg->Write("fid_app_RHC_NC_bkg");
  fid_app_RHC_aNC_bkg->Write("fid_app_RHC_aNC_bkg");
  fid_app_RHC_nue_bkg->Write("fid_app_RHC_nue_bkg");
  fid_app_RHC_nuebar_bkg->Write("fid_app_RHC_nuebar_bkg");
  fid_app_RHC_nutau_bkg->Write("fid_app_RHC_nutau_bkg");
  fid_app_RHC_nutaubar_bkg->Write("fid_app_RHC_nutaubar_bkg");
  fid_app_RHC_numu_bkg->Write("fid_app_RHC_numu_bkg");
  fid_app_RHC_numubar_bkg->Write("fid_app_RHC_numubar_bkg");

  smr_app_RHC_nue_sig->Write("smr_app_RHC_nue_sig");
  smr_app_RHC_nuebar_sig->Write("smr_app_RHC_nuebar_sig");
  smr_app_RHC_NC_bkg->Write("smr_app_RHC_NC_bkg");
  smr_app_RHC_aNC_bkg->Write("smr_app_RHC_aNC_bkg");
  smr_app_RHC_nue_bkg->Write("smr_app_RHC_nue_bkg");
  smr_app_RHC_nuebar_bkg->Write("smr_app_RHC_nuebar_bkg");
  smr_app_RHC_nutau_bkg->Write("smr_app_RHC_nutau_bkg");
  smr_app_RHC_nutaubar_bkg->Write("smr_app_RHC_nutaubar_bkg");
  smr_app_RHC_numu_bkg->Write("smr_app_RHC_numu_bkg");
  smr_app_RHC_numubar_bkg->Write("smr_app_RHC_numubar_bkg");

  std::cout << "Done APP RHC" << std::endl;
  fout->Close();
}

