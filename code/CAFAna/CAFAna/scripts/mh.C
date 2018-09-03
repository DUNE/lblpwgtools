// ETW May 2018
// Standard script for DUNE MH sensitivity
// Input files use TensorFlow CVN training from May 2018 

#ifdef __CINT__
void mh(bool reload = false){}
#else

#include "CAFAna/Analysis/Fit.h"
#include "CAFAna/Analysis/CalcsNuFit.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Experiment/MultiExperiment.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Prediction/PredictionInterp.h"
#include "CAFAna/Systs/Systs.h"
#include "CAFAna/Vars/FitVars.h"

using namespace ana;

#include "Utilities/rootlogon.C"

#include "OscLib/func/IOscCalculator.h"

#include "StandardRecord/StandardRecord.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH2.h"

const Var kRecoE_nue = SIMPLEVAR(dune.Ev_reco_nue);
const Var kRecoE_numu = SIMPLEVAR(dune.Ev_reco_numu);
const Var kPIDFD_NUMU = SIMPLEVAR(dune.cvnnumu);
const Var kPIDFD_NUE = SIMPLEVAR(dune.cvnnue);

const Var kvtxx_truth = SIMPLEVAR(dune.nuvtxx_truth);
const Var kvtxy_truth = SIMPLEVAR(dune.nuvtxy_truth);
const Var kvtxz_truth = SIMPLEVAR(dune.nuvtxz_truth);

const Cut kPassCVN_NUE = kPIDFD_NUE>0.7 && kPIDFD_NUMU<0.5;
const Cut kPassCVN_NUMU = kPIDFD_NUMU>0.5 && kPIDFD_NUE<0.7;
const Cut kPassFid_MC = kvtxx_truth<310 && kvtxx_truth>-310 && kvtxy_truth<550 && kvtxy_truth>-550 && kvtxz_truth>50 && kvtxz_truth<1244;

// 125 MeV bins from 0.5 to 8GeV (May want to revisit)
const HistAxis axis_nue("Reconstructed energy (GeV)",
                    Binning::Simple(60, 0.5, 8.0),
                    kRecoE_nue);

const HistAxis axis_numu("Reconstructed energy (GeV)",
                    Binning::Simple(60, 0.5, 8.0),
                    kRecoE_numu);

// POT/yr * 3.5yrs * mass correction
const double potFD = 3.5 * 1.47e21 * 40/1.13;

const char* stateFname = "mh_state.root";
const char* outputFname = "mh_sens.root";

//Set systematics style by hand for now
bool nosyst = false;
bool normsyst = true;
bool fullsyst = false;

//Signal Normalization Systs
const NueFHCSyst appnorm_sig_fhc;
const NumuFHCSyst disnorm_sig_fhc;
const NueRHCSyst appnorm_sig_rhc;
const NumuRHCSyst disnorm_sig_rhc;

//BG Normalization Systs
const NueBeamFHCSyst appnorm_beamnue_fhc;
const NueBeamRHCSyst appnorm_beamnue_rhc;
const NCAppSyst appnorm_nc;
const NCDisSyst disnorm_nc; 
const NutauSyst norm_tau;


std::vector<const ISyst*> systlist;
std::vector<const ISyst*> normlist_sig = {&appnorm_sig_fhc, &disnorm_sig_fhc, &appnorm_sig_rhc, &disnorm_sig_rhc};
std::vector<const ISyst*> normlist_bg = {&appnorm_beamnue_fhc, &appnorm_beamnue_rhc, &appnorm_nc, &disnorm_nc, &norm_tau};

void mh(bool reload = false)
{
  rootlogon(); // style
  
  if(reload || TFile(stateFname).IsZombie()){
    SpectrumLoader loaderFDFHC("/dune/data/users/kqi/oscillation_sample/merged_nu.root");
    //Example FD spectrum w/ new variables
    //SpectrumLoader loaderFDFHC("/pnfs/dune/persistent/users/marshalc/CAF/FarDetector/example_FD_CAF.root");

    auto* loaderFDFHCBeam  = loaderFDFHC.LoaderForRunPOT(20000001);
    auto* loaderFDFHCNue   = loaderFDFHC.LoaderForRunPOT(20000002);
    auto* loaderFDFHCNuTau = loaderFDFHC.LoaderForRunPOT(20000003);
    auto* loaderFDFHCNC    = loaderFDFHC.LoaderForRunPOT(0);

    SpectrumLoader loaderFDRHC("/dune/data/users/kqi/oscillation_sample/merged_anu.root");

    auto* loaderFDRHCBeam  = loaderFDRHC.LoaderForRunPOT(20000004);
    auto* loaderFDRHCNue   = loaderFDRHC.LoaderForRunPOT(20000005);
    auto* loaderFDRHCNuTau = loaderFDRHC.LoaderForRunPOT(20000006);
    auto* loaderFDRHCNC    = loaderFDRHC.LoaderForRunPOT(0);

    Loaders dummyLoaders; // PredictionGenerator insists on this

    //Selection applied
    PredictionNoExtrap predFDNumuFHC(*loaderFDFHCBeam, 
                                     *loaderFDFHCNue,
                                     *loaderFDFHCNuTau,
                                     *loaderFDFHCNC,
                                     axis_numu,
                                     kPassCVN_NUMU && kPassFid_MC);

    PredictionNoExtrap predFDNueFHC(*loaderFDFHCBeam, 
                                    *loaderFDFHCNue,
                                    *loaderFDFHCNuTau,
                                    *loaderFDFHCNC,
                                    axis_nue,
                                    kPassCVN_NUE && kPassFid_MC);

    PredictionNoExtrap predFDNumuRHC(*loaderFDRHCBeam, 
                                     *loaderFDRHCNue,
                                     *loaderFDRHCNuTau,
                                     *loaderFDRHCNC,
                                     axis_numu,
                                     kPassCVN_NUMU && kPassFid_MC);

    PredictionNoExtrap predFDNueRHC(*loaderFDRHCBeam, 
                                    *loaderFDRHCNue,
                                    *loaderFDRHCNuTau,
                                    *loaderFDRHCNC,
                                    axis_nue,
                                    kPassCVN_NUE && kPassFid_MC);

    DUNENoExtrapPredictionGenerator genFDNumuFHC(*loaderFDFHCBeam, 
						 *loaderFDFHCNue,
						 *loaderFDFHCNuTau,
						 *loaderFDFHCNC,
						 axis_numu,
						 kPassCVN_NUMU && kPassFid_MC);

    DUNENoExtrapPredictionGenerator genFDNumuRHC(*loaderFDRHCBeam, 
						 *loaderFDRHCNue,
						 *loaderFDRHCNuTau,
						 *loaderFDRHCNC,
						 axis_numu,
						 kPassCVN_NUMU && kPassFid_MC);
	  
    DUNENoExtrapPredictionGenerator genFDNueFHC(*loaderFDFHCBeam, 
						*loaderFDFHCNue,
						*loaderFDFHCNuTau,
						*loaderFDFHCNC,
						axis_nue,
						kPassCVN_NUE && kPassFid_MC);
    
    DUNENoExtrapPredictionGenerator genFDNueRHC(*loaderFDRHCBeam, 
						*loaderFDRHCNue,
						*loaderFDRHCNuTau,
						*loaderFDRHCNC,
						axis_nue,
						kPassCVN_NUE && kPassFid_MC);

    osc::IOscCalculatorAdjustable* calc = NuFitOscCalc(1);      

    //Note that systlist must be filled both here and after the state load
    if (normsyst) {
      systlist.insert(systlist.end(), normlist_sig.begin(), normlist_sig.end()); 
      systlist.insert(systlist.end(), normlist_bg.begin(), normlist_bg.end()); 
    }

    PredictionInterp predInterpFDNumuFHC(systlist,
					 calc, 
					 genFDNumuFHC, 
					 dummyLoaders);
    
    PredictionInterp predInterpFDNumuRHC(systlist,
					 calc, 
					 genFDNumuRHC, 
					 dummyLoaders);
    
    PredictionInterp predInterpFDNueFHC(systlist,
					calc, 
					genFDNueFHC, 
					dummyLoaders);
      
    PredictionInterp predInterpFDNueRHC(systlist,
					calc, 
					genFDNueRHC, 
					dummyLoaders);
    loaderFDFHC.Go();
    loaderFDRHC.Go();

    TFile fout(stateFname, "RECREATE");
    predFDNumuFHC.SaveTo(fout.mkdir("fd_numu_fhc"));
    predFDNueFHC.SaveTo(fout.mkdir("fd_nue_fhc"));
    predFDNumuRHC.SaveTo(fout.mkdir("fd_numu_rhc"));
    predFDNueRHC.SaveTo(fout.mkdir("fd_nue_rhc"));

    predInterpFDNumuFHC.SaveTo(fout.mkdir("fd_interp_numu_fhc"));
    predInterpFDNueFHC.SaveTo(fout.mkdir("fd_interp_nue_fhc"));
    predInterpFDNumuRHC.SaveTo(fout.mkdir("fd_interp_numu_rhc"));
    predInterpFDNueRHC.SaveTo(fout.mkdir("fd_interp_nue_rhc"));
    }
  else{    
    //Have to remake systematics list here:
    if (normsyst) {
      systlist.insert(systlist.end(), normlist_sig.begin(), normlist_sig.end()); 
      systlist.insert(systlist.end(), normlist_bg.begin(), normlist_bg.end()); 
    }

    // List all of the systematics we'll be using
    std::cout << "Systematics in this fit: " << std::endl;
    for(const ISyst* s: systlist) std::cout << s->ShortName() << "\t\t" << s->LatexName() << std::endl;
    if (systlist.size()==0) {std::cout << "None" << std::endl;}

    std::cout << "Loading state from " << stateFname << std::endl; 
  }

  TFile fin(stateFname);
  PredictionNoExtrap& predFDNumuFHC = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_numu_fhc")).release();
  PredictionNoExtrap& predFDNueFHC = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_nue_fhc")).release();
  PredictionNoExtrap& predFDNumuRHC = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_numu_rhc")).release();
  PredictionNoExtrap& predFDNueRHC = *ana::LoadFrom<PredictionNoExtrap>(fin.GetDirectory("fd_nue_rhc")).release();
  PredictionInterp& predInterpFDNumuFHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("fd_interp_numu_fhc")).release();
  PredictionInterp& predInterpFDNueFHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("fd_interp_nue_fhc")).release();
  PredictionInterp& predInterpFDNumuRHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("fd_interp_numu_rhc")).release();
  PredictionInterp& predInterpFDNueRHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("fd_interp_nue_rhc")).release();

  fin.Close();
  std::cout << "Done loading state" << std::endl;

  TFile* fout = new TFile(outputFname, "RECREATE");

  for(int hie = -1; hie <= +1; hie += 2){

    bool oscvar = true;
    double dcpstep = 2*TMath::Pi()/36;
    TGraph* gMH_NH = new TGraph();
    TGraph* gMH_IH = new TGraph();
    double thisdcp;

    for(double idcp = 0; idcp < 37; ++idcp) {
	
      thisdcp = -TMath::Pi() + idcp*dcpstep;
	
      osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie);
      trueOsc->SetdCP(thisdcp);

      //No systematics
      const Spectrum data_nue_fhc = predFDNueFHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment app_expt_fhc(&predFDNueFHC, data_nue_fhc);

      const Spectrum data_nue_rhc = predFDNueRHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment app_expt_rhc(&predFDNueRHC, data_nue_rhc);
      
      const Spectrum data_numu_fhc = predFDNumuFHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment dis_expt_fhc(&predFDNumuFHC, data_numu_fhc);

      const Spectrum data_numu_rhc = predFDNumuRHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment dis_expt_rhc(&predFDNumuRHC, data_numu_rhc);

      //With systematics

      const Spectrum data_nue_fhc_syst = predInterpFDNueFHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment app_expt_fhc_syst(&predInterpFDNueFHC, data_nue_fhc_syst);

      const Spectrum data_nue_rhc_syst = predInterpFDNueRHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment app_expt_rhc_syst(&predInterpFDNueRHC, data_nue_rhc_syst);

      const Spectrum data_numu_fhc_syst = predInterpFDNumuFHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment dis_expt_fhc_syst(&predInterpFDNumuFHC, data_numu_fhc_syst);

      const Spectrum data_numu_rhc_syst = predInterpFDNumuRHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment dis_expt_rhc_syst(&predInterpFDNumuRHC, data_numu_rhc_syst);

      std::vector<const IFitVar*> oscVars =
	{&kFitDmSq32Scaled, &kFitSinSqTheta23,
	 &kFitTheta13, &kFitDeltaInPiUnits, &kFitRho};

      double chisqmin = 99999;
      double thischisq;

      osc::IOscCalculatorAdjustable* testOsc = NuFitOscCalc(hie);	
      testOsc->SetdCP(thisdcp);
      testOsc->SetDmsq32(-1*testOsc->GetDmsq32());
      for(int ioct = -1; ioct <= 1; ioct += 2) {
	if (ioct < 0) {
	  testOsc->SetTh23(TMath::PiOver2() - testOsc->GetTh23());
	}

	osc::IOscCalculatorAdjustable* cvcalc = testOsc->Copy();	  
	Penalizer_GlbLike penalty(cvcalc,hie);

	MultiExperiment apponly_expt({&app_expt_fhc, &app_expt_rhc, &penalty});
	MultiExperiment full_expt({&app_expt_fhc, &app_expt_rhc, &dis_expt_fhc, &dis_expt_rhc, &penalty});
	MultiExperiment full_expt_syst({&app_expt_fhc_syst, &app_expt_rhc_syst, &dis_expt_fhc_syst, &dis_expt_rhc_syst, &penalty});

	Fitter fit(&full_expt, oscVars);
	Fitter fit_syst(&full_expt_syst, oscVars, systlist);

	if (nosyst) {
	  thischisq = fit.Fit(testOsc);
	}
	else {
	  thischisq = fit_syst.Fit(testOsc);
	}

	chisqmin = TMath::Min(thischisq,chisqmin);
      }

      chisqmin = TMath::Max(chisqmin,1e-6);
      std::cout << thisdcp << " " << TMath::Sqrt(chisqmin) << std::endl;
      if (hie > 0) {
	gMH_NH->SetPoint(gMH_NH->GetN(),thisdcp/TMath::Pi(),TMath::Sqrt(chisqmin));
      }
      else {
	gMH_IH->SetPoint(gMH_IH->GetN(),thisdcp/TMath::Pi(),TMath::Sqrt(chisqmin));
      }
    }

    if (hie > 0) {
      gMH_NH->Draw("ALP");
      gMH_NH->Write("sens_mh_nh");
    }
    else {
      gMH_IH->Draw("ALP");
      gMH_IH->Write("sens_mh_ih");
    }
  }
  fout->Close();
}

#endif
