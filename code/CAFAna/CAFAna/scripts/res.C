// Standard script for DUNE parameter resolution
// Now uses state file created by spec.C

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
#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Analysis/Resolution.h"

#include "TF1.h"
#include "Math/WrappedTF1.h"
#include "Math/BrentRootFinder.h"

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

// 3.5yrs * POT/yr * mass correction
const double potFD = 3.5 * POT120 * 40/1.13;

const char* stateFname = "spec_state.root";

//Set systematics style by hand for now
bool nosyst = false;
bool normsyst = true;
bool fullsyst = false;

std::vector<const ISyst*> systlist;
std::vector<const ISyst*> normlist_sig = {&kNueFHCSyst, &kNumuFHCSyst, &kNueRHCSyst, &kNumuRHCSyst};
std::vector<const ISyst*> normlist_bg = {&kNueBeamFHCSyst, &kNueBeamRHCSyst, &kNCAppSyst, &kNCDisSyst, &kNutauSyst};

void res(string paramname)
{
  rootlogon(); // style

  if (paramname == "dcp") {
    std::cout << "Fitting deltaCP resolution" << std::endl;
  }
  else {
    std::cout << "Parameter " << paramname << " not supported" << std::endl;
    return;
  }
  
  if(TFile(stateFname).IsZombie()){
    std::cout << "No state available, please run spec.C true" << std::endl;
    return;
  }
  else{    
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
  PredictionInterp& predInt_FDNumuFHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_numu_fhc")).release();
  PredictionInterp& predInt_FDNueFHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_nue_fhc")).release();
  PredictionInterp& predInt_FDNumuRHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_numu_rhc")).release();
  PredictionInterp& predInt_FDNueRHC = *ana::LoadFrom<PredictionInterp>(fin.GetDirectory("pred_fd_nue_rhc")).release();

  fin.Close();

  std::cout << "Done loading state" << std::endl;

  const char* outputFname = Form("%s_res.root",paramname.c_str());
  TFile* fout = new TFile(outputFname, "RECREATE");

  TGraph* gRes_NH = new TGraph();
  TGraph* gRes_IH = new TGraph();
  for(int hie = 1; hie <= +1; hie += 2){

    const std::string hieStr = (hie > 0) ? "nh" : "ih";
    bool oscvar = true;
    double dcpstep = 2*TMath::Pi()/36;
    double thisdcp;
    for(double idcp = 0; idcp < 37; ++idcp) {
	
      thisdcp = -TMath::Pi() + idcp*dcpstep;
      //std::cout << idcp << " " << thisdcp << std::endl;
	
      osc::IOscCalculatorAdjustable* trueOsc = NuFitOscCalc(hie);
      trueOsc->SetdCP(thisdcp);

      const Spectrum data_nue_fhc_syst = predInt_FDNueFHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment app_expt_fhc_syst(&predInt_FDNueFHC, data_nue_fhc_syst);
      app_expt_fhc_syst.SetMaskHist(0.5,8.0);

      const Spectrum data_nue_rhc_syst = predInt_FDNueRHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment app_expt_rhc_syst(&predInt_FDNueRHC, data_nue_rhc_syst);
      app_expt_rhc_syst.SetMaskHist(0.5,8.0);

      const Spectrum data_numu_fhc_syst = predInt_FDNumuFHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment dis_expt_fhc_syst(&predInt_FDNumuFHC, data_numu_fhc_syst);
      dis_expt_fhc_syst.SetMaskHist(0.5,8.0);

      const Spectrum data_numu_rhc_syst = predInt_FDNumuRHC.Predict(trueOsc).FakeData(potFD);
      SingleSampleExperiment dis_expt_rhc_syst(&predInt_FDNumuRHC, data_numu_rhc_syst);
      dis_expt_rhc_syst.SetMaskHist(0.5,8.0);

      std::vector<const IFitVar*> oscVars =
	{&kFitDmSq32Scaled, &kFitSinSqTheta23,
	 &kFitTheta13, &kFitRho};

      double pmin = thisdcp-TMath::Pi()/2;
      double pmax = thisdcp+TMath::Pi()/2;
      osc::IOscCalculatorAdjustable* testOsc = trueOsc->Copy();
      osc::IOscCalculatorAdjustable* cvcalc = trueOsc->Copy();
      Penalizer_GlbLike penalty(cvcalc,hie);
      MultiExperiment full_expt_syst({&app_expt_fhc_syst, &app_expt_rhc_syst, &dis_expt_fhc_syst, &dis_expt_rhc_syst, &penalty});
      Fitter fit_syst(&full_expt_syst, oscVars, systlist);
      Resolution *res = new Resolution(fit_syst, testOsc);
      TF1 *f = new TF1("f",res,&Resolution::FitResult,-1.5*TMath::Pi(),1.5*TMath::Pi(),0,"Resolution","FitResult"); 
      ROOT::Math::WrappedTF1 wf1(*f);
      ROOT::Math::BrentRootFinder brf_up;
      ROOT::Math::BrentRootFinder brf_down;
      brf_up.SetFunction( wf1, thisdcp, pmax);
      brf_up.SetNpx(15);
      brf_up.Solve(100,1e-5,1e-7);
      double root_up = brf_up.Root();

      brf_down.SetFunction( wf1, pmin, thisdcp);
      brf_down.SetNpx(15);
      brf_down.Solve(100,1e-5,1e-7);
      double root_down = brf_down.Root();

      std::cout << thisdcp << " " << TMath::Abs(root_down-thisdcp)*180/TMath::Pi() << " " << TMath::Abs(root_up-thisdcp)*180/TMath::Pi() << std::endl;
      double result = (TMath::Abs(root_down-thisdcp)+TMath::Abs(root_up-thisdcp))*180/TMath::Pi()/2;
      if (hie > 0) {
	gRes_NH->SetPoint(gRes_NH->GetN(), thisdcp/TMath::Pi(),result);
      }
      else {
	gRes_IH->SetPoint(gRes_IH->GetN(), thisdcp/TMath::Pi(),result);
      }
    }
  }
  gRes_NH->Write("dcp_res_nh");
  gRes_IH->Write("dcp_res_ih");
  fout->Close();
}
