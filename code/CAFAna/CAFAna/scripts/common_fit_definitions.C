#include "CAFAna/Experiment/MultiExperiment.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Prediction/PredictionInterp.h"
#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Prediction/PredictionNoOsc.h"
#include "CAFAna/Analysis/Calcs.h"
#include "OscLib/func/IOscCalculator.h"
#include "OscLib/func/OscCalculatorPMNSOpt.h"
#include "StandardRecord/StandardRecord.h"
#include "TCanvas.h"
#include "CAFAna/Systs/Systs.h"
#include "CAFAna/Systs/DUNEFluxSysts.h"
#include "CAFAna/Systs/GenieSysts.h"
#include "CAFAna/Systs/EnergySysts.h"
#include "CAFAna/Systs/NDRecoSysts.h"
#include "CAFAna/Systs/FDRecoSysts.h"
#include "CAFAna/Systs/NuOnESysts.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "CAFAna/Analysis/Plots.h"
#include "CAFAna/Vars/FitVars.h"

#include "CAFAna/Analysis/Fit.h"
#include "CAFAna/Analysis/CalcsNuFit.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/Progress.h"

#include "CAFAna/Analysis/Surface.h"
#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Cuts/AnaCuts.h"
#include <tuple>
#include "Utilities/rootlogon.C"

using namespace ana;

// List of vars
// -->FD
const Var kRecoE_nue  = SIMPLEVAR(dune.Ev_reco_nue);
const Var kRecoE_numu = SIMPLEVAR(dune.Ev_reco_numu);
const Var kFDNumuPid  = SIMPLEVAR(dune.cvnnumu);
const Var kFDNuePid   = SIMPLEVAR(dune.cvnnue);
const Var kMVANUMU    = SIMPLEVAR(dune.mvanumu);

// -->ND
const Var kRecoEnergyND = SIMPLEVAR(dune.Ev_reco);
const Var kRecoYND      = (SIMPLEVAR(dune.Ev_reco) - SIMPLEVAR(dune.Elep_reco))/SIMPLEVAR(dune.Ev_reco);

// CV weighting
const Var kGENIEWeights = SIMPLEVAR(dune.total_cv_wgt); // kUnweighted

// FD cut
const Cut kFDSelNue     = kFDNuePid > 0.7;
const Cut kFDSelNumu    = kFDNumuPid > 0.7;
  
// --> ND cuts, from Chris: For the numu sample: reco_numu ==1, reco_q == -1 for FHC and +1 for RHC.  Also muon_exit == 0, which means that the muon is well-reconstructed.  And Ehad_veto < 30, which means the hadronic system is (probably) well-reconstructed
const Cut kRecoNegMu    = SIMPLEVAR(dune.reco_q) == -1; // Note that for these to be true, reco_numu == 1
const Cut kRecoPosMu    = SIMPLEVAR(dune.reco_q) == +1; // reco_q == 0 if reco_numu != 1
const Cut kMuonCont     = SIMPLEVAR(dune.muon_exit) == 0;
const Cut kEhad_veto    = SIMPLEVAR(dune.Ehad_veto) < 30;

// Binnings
const Binning binsFDEreco = Binning::Simple(80, 0, 10);
const Binning binsNDEreco = Binning::Simple(40, 0, 10);
const Binning binsY       = Binning::Simple(5, 0, 1);
						      
// Axes
const HistAxis axRecoEnuFDnumu("Reco energy (GeV)", binsFDEreco, kRecoE_numu);
const HistAxis axRecoEnuFDnue("Reco energy (GeV)", binsFDEreco, kRecoE_nue);
const HistAxis axErecYrecND("Reco energy (GeV)", binsNDEreco, kRecoEnergyND, "y_{rec}", binsY, kRecoYND);

// POT for 3.5 years
const double pot_fd = 3.5 * POT120 * 40/1.13;
const double pot_nd = 3.5 * POT120;

// Global file path...
const std::string cafFilePath="/dune/data/users/marshalc/CAFs/mcc11_v2";

// To get the oscillation probabilities
// osc::IOscCalculatorAdjustable* calc = DefaultOscCalc();


std::vector<const ISyst*> GetListOfSysts(bool fluxsyst=true, bool xsecsyst=true, bool detsyst=true,
					 bool useND=true, bool useFD=true,
					 bool useNueOnE=false){
  // This doesn't need to be an argument because I basically never change it:
  bool fluxXsecPenalties = true;

  std::vector<const ISyst*> systlist;
  if (fluxsyst){
    std::vector<const ISyst*> fluxlist = GetDUNEFluxSysts(10, fluxXsecPenalties);
    systlist.insert(systlist.end(), fluxlist.begin(), fluxlist.end());
  }

  if (detsyst) {
    std::vector<const ISyst*> elist  = GetEnergySysts();
    std::vector<const ISyst*> fdlist = GetFDRecoSysts();
    std::vector<const ISyst*> ndlist = GetNDRecoSysts();
    std::vector<const ISyst*> nuelist= GetNuOnESysts();

    systlist.insert(systlist.end(), elist.begin(), elist.end());
    if (useFD) systlist.insert(systlist.end(), fdlist.begin(), fdlist.end());
    if (useND) systlist.insert(systlist.end(), ndlist.begin(), ndlist.end());
    if (useND && useNueOnE) systlist.insert(systlist.end(), nuelist.begin(), nuelist.end());
  }

  if (xsecsyst) {
    std::vector<const ISyst*> xseclist = GetGenieSysts(GetGenieWeightNames(), fluxXsecPenalties);    
    systlist.insert(systlist.end(), xseclist.begin(), xseclist.end());
  }

  return systlist;
};


// Use a sample enum, maybe this should live elsewhere?
enum SampleType{kFDFHC, kFDRHC, kNDFHC, kNDRHC, kNDNue, kUnknown};

std::string GetSampleName(SampleType sample){
  switch(sample){
  case kFDFHC : return "FD_FHC";
  case kFDRHC : return "FD_RHC";
  case kNDFHC : return "ND_FHC";
  case kNDRHC : return "ND_RHC";    
  case kNDNue : return "ND_nue";
  case kUnknown :
  default : return "NONE";
  }
};

SampleType GetSampleType(std::string sample){

  if (sample == "FD_FHC") return kFDFHC;
  if (sample == "FD_RHC") return kFDRHC;
  if (sample == "ND_FHC") return kNDFHC;
  if (sample == "ND_RHC") return kNDRHC;    
  if (sample == "ND_nue") return kNDNue;
  return kUnknown;
}

void MakePredictionInterp(TDirectory* saveDir, SampleType sample,
			  std::vector<const ISyst*> systlist,
			  int max=0){

  // Move to the save directory
  saveDir->cd();
  osc::IOscCalculatorAdjustable* this_calc = NuFitOscCalc(1);      

  // FD samples
  if (sample == kFDFHC || sample == kFDRHC){

    Loaders these_loaders;
    SpectrumLoader loaderNumu(cafFilePath+"/"+ GetSampleName(sample)+"_nonswap.root", kBeam, max);
    SpectrumLoader loaderNue(cafFilePath+"/"+ GetSampleName(sample)+"_nueswap.root", kBeam, max);
    SpectrumLoader loaderNutau(cafFilePath+"/"+ GetSampleName(sample)+"_tauswap.root", kBeam, max);
    
    these_loaders .AddLoader(&loaderNumu, caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNonSwap);
    these_loaders .AddLoader(&loaderNue, caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNueSwap);
    these_loaders .AddLoader(&loaderNutau, caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNuTauSwap);

    NoExtrapPredictionGenerator genFDNumu(axRecoEnuFDnumu, kPassFD_CVN_NUMU && kIsTrueFV, kGENIEWeights);
    NoExtrapPredictionGenerator genFDNue(axRecoEnuFDnue, kPassFD_CVN_NUE && kIsTrueFV, kGENIEWeights);
    PredictionInterp predInterpFDNumu(systlist, this_calc, genFDNumu, these_loaders);
    PredictionInterp predInterpFDNue(systlist, this_calc, genFDNue, these_loaders);
    these_loaders.Go();

    std::cout << "Saving " << GetSampleName(sample) << std::endl;
    predInterpFDNumu.SaveTo(saveDir->mkdir((std::string("fd_interp_numu_") + std::string(sample == kFDFHC ? "fhc" : "rhc")).c_str()));
    std::cout << "Saving " << GetSampleName(sample) << std::endl;
    predInterpFDNue .SaveTo(saveDir->mkdir((std::string("fd_interp_nue_") + std::string(sample == kFDFHC ? "fhc" : "rhc")).c_str()));

  } else if (sample == kNDFHC || sample == kNDRHC){
    // Now ND    
    Loaders these_loaders;
    SpectrumLoader loaderNumu(cafFilePath+"/"+ GetSampleName(sample)+"_CAF.root", kBeam, max);
    these_loaders .AddLoader(&loaderNumu, caf::kNEARDET, Loaders::kMC);
    
    NoOscPredictionGenerator genNDNumu(axErecYrecND, (sample == kNDFHC ? kPassND_FHC_NUMU : kPassND_RHC_NUMU)
     				       && kIsTrueFV, kGENIEWeights);
  
    PredictionInterp predInterpNDNumu(systlist, this_calc, genNDNumu, these_loaders);
    these_loaders.Go();
  
    std::cout << "Saving " << GetSampleName(sample) << std::endl;
    predInterpNDNumu.SaveTo(saveDir->mkdir((std::string("nd_interp_numu_") + std::string(sample == kNDFHC ? "fhc" : "rhc")).c_str()));
  }
}


std::vector<unique_ptr<ana::PredictionInterp> > GetPredictionInterps(std::string fileName,
								     std::vector<const ISyst*> systlist,
								     int max=0, bool reload=false){

  if (reload || TFile(fileName.c_str()).IsZombie()){
    std::cout << "Now creating PredictionInterps in series... maybe you should use the other scripts to do them in parallel?" << std::endl;

    TFile fout(fileName.c_str(), "RECREATE");
    MakePredictionInterp(&fout, kFDFHC, systlist, max);
    MakePredictionInterp(&fout, kFDRHC, systlist, max);
    MakePredictionInterp(&fout, kNDFHC, systlist, max);
    MakePredictionInterp(&fout, kNDRHC, systlist, max);
    fout .Close();
  }
  std::vector<unique_ptr<PredictionInterp> > return_list;
  TFile fin(fileName.c_str());
  std::cout << "Retrieving FD FHC numu" << std::endl;
  return_list.push_back(std::unique_ptr<ana::PredictionInterp> (LoadFrom<PredictionInterp>(fin.GetDirectory("fd_interp_numu_fhc")).release()));
  std::cout << "Retrieving FD FHC nue" << std::endl;
  return_list.push_back(std::unique_ptr<ana::PredictionInterp> (LoadFrom<PredictionInterp>(fin.GetDirectory("fd_interp_nue_fhc")).release()));
  std::cout << "Retrieving FD RHC numu" << std::endl;
  return_list.push_back(std::unique_ptr<ana::PredictionInterp> (LoadFrom<PredictionInterp>(fin.GetDirectory("fd_interp_numu_rhc")).release()));
  std::cout << "Retrieving FD RHC nue" << std::endl;
  return_list.push_back(std::unique_ptr<ana::PredictionInterp> (LoadFrom<PredictionInterp>(fin.GetDirectory("fd_interp_nue_rhc")).release()));
  std::cout << "Retrieving ND FHC" << std::endl;
  return_list.push_back(std::unique_ptr<ana::PredictionInterp> (LoadFrom<PredictionInterp>(fin.GetDirectory("nd_interp_numu_fhc")).release()));
  std::cout << "Retrieving ND RHC" << std::endl;
  return_list.push_back(std::unique_ptr<ana::PredictionInterp> (LoadFrom<PredictionInterp>(fin.GetDirectory("nd_interp_numu_rhc")).release()));
  fin.Close();
  return return_list;
};

void RemoveSysts(std::vector<const ISyst *> &systlist,
                 std::vector<std::string> const &namesToRemove) {
  systlist.erase(std::remove_if(systlist.begin(), systlist.end(),
                                [&](const ISyst *s) {
                                  return (std::find(namesToRemove.begin(),
                                                    namesToRemove.end(),
                                                    s->ShortName()) !=
                                          namesToRemove.end());
                                }),
                 systlist.end());
}

TH2D* make_corr_from_covar(TH2D* covar){

  TH2D *corr = (TH2D*)covar->Clone();
  corr      ->SetNameTitle("corr", "corr");

  for (int i = 0; i < covar->GetNbinsX(); ++i){
    double istddev = sqrt(covar->GetBinContent(i+1, i+1));
    for (int j = 0; j < covar->GetNbinsX(); ++j){
      double jstddev  = sqrt(covar->GetBinContent(j+1, j+1));
      double new_corr = covar->GetBinContent(i+1, j+1)/istddev/jstddev; 
      corr ->SetBinContent(i+1, j+1, new_corr);
    }
  }
  return corr;
}

std::vector<ana::IExperiment*> iHateThisSoMuch;

MultiExperiment GetMultiExperiment(std::string stateFileName, double pot_nd_fhc, double pot_nd_rhc, double pot_fd_fhc, double pot_fd_rhc,
				   osc::IOscCalculatorAdjustable* fakeThrowOsc, SystShifts fakeThrowSyst=kNoShift,
				   bool stats_throw=false){

  // Start by getting the PredictionInterps... better that this is done here than elsewhere as they aren't smart enough to know what they are (so the order matters)
  // Note that all systs are used to load the PredictionInterps
  static std::vector<unique_ptr<PredictionInterp> > interp_list = GetPredictionInterps(stateFileName, GetListOfSysts());
  static PredictionInterp& predFDNumuFHC = *interp_list[0].release();
  static PredictionInterp& predFDNueFHC  = *interp_list[1].release();
  static PredictionInterp& predFDNumuRHC = *interp_list[2].release();
  static PredictionInterp& predFDNueRHC  = *interp_list[3].release();
  static PredictionInterp& predNDNumuFHC = *interp_list[4].release();
  static PredictionInterp& predNDNumuRHC = *interp_list[5].release();

  if (iHateThisSoMuch.size()){
    for (auto & my_remaining_goodwill : iHateThisSoMuch) delete my_remaining_goodwill;
  }
  iHateThisSoMuch.clear();
    
  const Spectrum data_nue_fhc = predFDNueFHC.PredictSyst(fakeThrowOsc, fakeThrowSyst).MockData(pot_fd_fhc, stats_throw);
  SingleSampleExperiment *app_expt_fhc = new SingleSampleExperiment(&predFDNueFHC, data_nue_fhc);
  app_expt_fhc->SetMaskHist(0.5, 8);
  iHateThisSoMuch.push_back(app_expt_fhc);
  
  const Spectrum data_nue_rhc = predFDNueRHC.PredictSyst(fakeThrowOsc, fakeThrowSyst).MockData(pot_fd_fhc, stats_throw);
  SingleSampleExperiment *app_expt_rhc = new SingleSampleExperiment(&predFDNueRHC, data_nue_rhc);
  app_expt_rhc->SetMaskHist(0.5, 8);
  iHateThisSoMuch.push_back(app_expt_rhc);
  
  const Spectrum data_numu_fhc = predFDNumuFHC.PredictSyst(fakeThrowOsc, fakeThrowSyst).MockData(pot_fd_rhc, stats_throw);
  SingleSampleExperiment *dis_expt_fhc = new SingleSampleExperiment(&predFDNumuFHC, data_numu_fhc);
  dis_expt_fhc->SetMaskHist(0.5, 8);
  iHateThisSoMuch.push_back(dis_expt_fhc);  
  
  const Spectrum data_numu_rhc = predFDNumuRHC.PredictSyst(fakeThrowOsc, fakeThrowSyst).MockData(pot_fd_rhc, stats_throw);
  SingleSampleExperiment *dis_expt_rhc = new SingleSampleExperiment(&predFDNumuRHC, data_numu_rhc);
  dis_expt_rhc->SetMaskHist(0.5, 8);
  iHateThisSoMuch.push_back(dis_expt_rhc);  
  
  const Spectrum nd_data_numu_fhc = predNDNumuFHC.PredictSyst(fakeThrowOsc, fakeThrowSyst).MockData(pot_nd_fhc, stats_throw);
  SingleSampleExperiment *nd_expt_fhc = new SingleSampleExperiment(&predNDNumuFHC, nd_data_numu_fhc);
  //nd_expt_fhc.SetMaskHist(0, -1, 0.1, 1);
  iHateThisSoMuch.push_back(nd_expt_fhc);  

  const Spectrum nd_data_numu_rhc = predNDNumuRHC.PredictSyst(fakeThrowOsc, fakeThrowSyst).MockData(pot_nd_rhc, stats_throw);
  SingleSampleExperiment *nd_expt_rhc = new SingleSampleExperiment(&predNDNumuRHC, nd_data_numu_rhc);
  //nd_expt_rhc.SetMaskHist(0, -1, 0.1, 1);
  iHateThisSoMuch.push_back(nd_expt_rhc);  
  
  MultiExperiment ret;

  if (pot_nd_fhc > 0) ret.Add(nd_expt_fhc);
  if (pot_nd_rhc > 0) ret.Add(nd_expt_rhc);
  if (pot_fd_fhc > 0) {
    ret.Add(app_expt_fhc);
    ret.Add(dis_expt_fhc);
  }
  if (pot_fd_rhc > 0) {
    ret.Add(app_expt_rhc);
    ret.Add(dis_expt_rhc);
  }  
  return ret;
};
