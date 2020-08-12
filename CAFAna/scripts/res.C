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

#include "OscLib/IOscCalc.h"

#include "StandardRecord/StandardRecord.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH2.h"

const Var kRecoE_nue = SIMPLEVAR(Ev_reco_nue);
const Var kRecoE_numu = SIMPLEVAR(Ev_reco_numu);

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

  int parcode; // 0=dcp, 1=th12, 2=th23, 3=dm2
  double dcpstep = 1;
  double dcplo = -999;
  int nstep = 1;

  if (paramname == "dcp") {
    std::cout << "Fitting deltaCP resolution" << std::endl;
    dcpstep = 2*TMath::Pi()/36;
    dcplo = -TMath::Pi();
    nstep = 37;
    parcode = 0;
  }
  else if (paramname == "th13") {
    std::cout << "Fitting theta13 resolution" << std::endl;
    dcpstep = 2*TMath::Pi()/36;
    dcplo = -TMath::Pi();
    nstep = 37;
    parcode = 1;
  }
  else if (paramname == "th23") {
    std::cout << "Fitting theta23 resolution" << std::endl;
    dcpstep = 2*TMath::Pi()/36;
    dcplo = -TMath::Pi();
    nstep = 37;
    parcode = 2;
  }
  else if (paramname == "Dmsq") {
    std::cout << "Fitting Dmsq32 resolution" << std::endl;
    dcpstep = 2*TMath::Pi()/36;
    dcplo = -TMath::Pi();
    nstep = 37;
    parcode = 3;
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
  PredictionInterp& predInt_FDNumuFHC = *ana::LoadFrom<PredictionInterp>(&fin, "pred_fd_numu_fhc").release();
  PredictionInterp& predInt_FDNueFHC = *ana::LoadFrom<PredictionInterp>(&fin, "pred_fd_nue_fhc").release();
  PredictionInterp& predInt_FDNumuRHC = *ana::LoadFrom<PredictionInterp>(&fin, "pred_fd_numu_rhc").release();
  PredictionInterp& predInt_FDNueRHC = *ana::LoadFrom<PredictionInterp>(&fin, "pred_fd_nue_rhc").release();

  fin.Close();

  std::cout << "Done loading state" << std::endl;

  const char* outputFname = Form("%s_res.root",paramname.c_str());
  TFile* fout = new TFile(outputFname, "RECREATE");

  TGraph* gRes_NH = new TGraph();
  TGraph* gRes_IH = new TGraph();
  for(int hie = 1; hie <= +1; hie += 2){

    const std::string hieStr = (hie > 0) ? "nh" : "ih";
    bool oscvar = true;
    double thisdcp;
    for(int idcp = 0; idcp < nstep; ++idcp) {
	
      osc::IOscCalcAdjustable* trueOsc = NuFitOscCalc(hie);

      thisdcp = dcplo + idcp*dcpstep;
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

      double pmin, pmax, funclo, funchi, thispar;
      std::vector<const IFitVar*> oscVars;      
      bool arg1 = true, arg2=true, arg3= true;
      double prec1 = 1e-5, prec2 = 1e-7;
      if (parcode == 0) {
	oscVars = {&kFitDmSq32Scaled, &kFitSinSqTheta23, &kFitTheta13, &kFitRho};
	pmin = thisdcp-TMath::Pi()/2;
	pmax = thisdcp+TMath::Pi()/2;
	funclo = -1.5*TMath::Pi();
	funchi = 1.5*TMath::Pi();
	thispar = thisdcp;
      }
      else if (parcode == 1) {
	oscVars = {&kFitDmSq32Scaled, &kFitSinSqTheta23, &kFitDeltaInPiUnits, &kFitRho};
	thispar = trueOsc->GetTh13() ;
	pmin =  thispar - 1*TMath::Pi()/180;
	pmax = thispar + 1*TMath::Pi()/180;
	funclo = pmin - 0.5*TMath::Pi()/180;
	funchi = pmax + 0.5*TMath::Pi()/180;
	arg2 = false; //turn off external constrain on th13
      }
      else if (parcode == 2) {
	oscVars = {&kFitDmSq32Scaled, &kFitTheta13, &kFitDeltaInPiUnits, &kFitRho};
	thispar = trueOsc->GetTh23() ;
	pmin =  thispar - 1*TMath::Pi()/180;
	pmax = thispar + 1*TMath::Pi()/180;
	funclo = pmin - 0.5*TMath::Pi()/180;
	funchi = pmax + 0.5*TMath::Pi()/180;
	arg3 = false; //turn off external constrain on th23
      }
      else if (parcode == 3) {
	oscVars = {&kFitSinSqTheta23, &kFitTheta13, &kFitDeltaInPiUnits, &kFitRho};
	thispar = trueOsc->GetDmsq32() ;
	pmin =  thispar - 2e-5;
	pmax = thispar + 2e-5;
	funclo = pmin - 1e-5;
	funchi = pmax + 1e-5;
	arg4 = false; //turn off external constrain on Dmsq
	prec1 = 1e-7;
	prec2 = 1e-9;
      }

      osc::IOscCalcAdjustable* testOsc = trueOsc->Copy();
      osc::IOscCalcAdjustable* cvcalc = trueOsc->Copy();
      Penalizer_GlbLike penalty(cvcalc,hie,arg1,arg2,arg3);
      MultiExperiment full_expt_syst({&app_expt_fhc_syst, &app_expt_rhc_syst, &dis_expt_fhc_syst, &dis_expt_rhc_syst, &penalty});

      Fitter fit_syst(&full_expt_syst, oscVars, systlist);
      Resolution *res = new Resolution(fit_syst, testOsc, parcode);
      TF1 *f = new TF1("f",res,&Resolution::FitResult,funclo,funchi,0,"Resolution","FitResult"); 

      ROOT::Math::WrappedTF1 wf1(*f);
      ROOT::Math::BrentRootFinder brf_up;
      ROOT::Math::BrentRootFinder brf_down;
      brf_up.SetFunction( wf1, thispar, pmax);
      brf_up.SetNpx(15);
      brf_up.Solve(100,prec1,prec2);
      double root_up = brf_up.Root();

      brf_down.SetFunction( wf1, pmin, thispar);
      brf_down.SetNpx(15);
      brf_down.Solve(100,prec1,prec2);
      double root_down = brf_down.Root();

      double result;
      if (parcode==0) {
	std::cout << thisdcp << " " << TMath::Abs(root_down-thisdcp)*180/TMath::Pi() << " " << TMath::Abs(root_up-thisdcp)*180/TMath::Pi() << std::endl;
	result = (TMath::Abs(root_down-thisdcp)+TMath::Abs(root_up-thisdcp))*180/TMath::Pi()/2;
      }
      else if (parcode==1) {
	double diffavg = (TMath::Abs(root_down-thispar)+TMath::Abs(root_up-thispar))/2;
	result = TMath::Sin(2*(thispar+diffavg))*TMath::Sin(2*(thispar+diffavg)) - TMath::Sin(2*thispar)*TMath::Sin(2*thispar);
	std::cout << thisdcp << " " << thispar << " " << TMath::Abs(root_down-thispar) << " " << TMath::Abs(root_up-thispar) << " " << result << std::endl;
      }
      else if (parcode==2) {
	double diffavg = (TMath::Abs(root_down-thispar)+TMath::Abs(root_up-thispar))/2;
	result = TMath::Sin(thispar+diffavg)*TMath::Sin(thispar+diffavg) - TMath::Sin(thispar)*TMath::Sin(thispar);
	std::cout << thisdcp << " " << thispar << " " << TMath::Abs(root_down-thispar) << " " << TMath::Abs(root_up-thispar) << " " << result << std::endl;
      }
      else if (parcode==3) {
	double diffavg = (TMath::Abs(root_down-thispar)+TMath::Abs(root_up-thispar))/2;
	result = diffavg*1e5;
	std::cout << thisdcp << " " << thispar << " " << TMath::Abs(root_down-thispar) << " " << TMath::Abs(root_up-thispar) << " " << result << std::endl;
      }

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
