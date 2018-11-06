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
#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "CAFAna/Analysis/Plots.h"
#include "CAFAna/Vars/FitVars.h"

#include "CAFAna/Analysis/Fit.h"
#include "CAFAna/Analysis/CalcsNuFit.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/Progress.h"

#include "CAFAna/Analysis/Surface.h"
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

const Var kvtxx_truth = SIMPLEVAR(dune.nuvtxx_truth);
const Var kvtxy_truth = SIMPLEVAR(dune.nuvtxy_truth);
const Var kvtxz_truth = SIMPLEVAR(dune.nuvtxz_truth);

// -->ND
const Var kRecoEnergyND = SIMPLEVAR(dune.Ev_reco);
const Var kRecoYND      = (SIMPLEVAR(dune.Ev_reco) - SIMPLEVAR(dune.Elep_reco))/SIMPLEVAR(dune.Ev_reco);

// FD cut
const Cut kFDSelNue     = kFDNuePid > 0.7;
const Cut kFDSelNumu    = kFDNumuPid > 0.7;
// This isn't currently working, so... get rid of it for now...
// Chris M says: The cut for the ND will be x in (-300, +300), y in (-100, +100), z in (+50, +350)
// I guess we keep the old one for the FD
// const Cut kFDPassFV     = kFDNuePid == kFDNuePid; //kvtxx_truth<310 && kvtxx_truth>-310 && kvtxy_truth<550 && kvtxy_truth>-550 && kvtxz_truth>50 && kvtxz_truth<1244;
const Cut kFDPassFV     = kMVANUMU > -1; // From Elizabeth to mock up FV cut kPID_MVA_NUMU>-1

  
// --> ND cuts, from Chris: For the numu sample: reco_numu ==1, reco_q == -1 for FHC and +1 for RHC.  Also muon_exit == 0, which means that the muon is well-reconstructed.  And Ehad_veto < 30, which means the hadronic system is (probably) well-reconstructed
const Cut kRecoNegMu    = SIMPLEVAR(dune.reco_q) == -1; // Note that for these to be true, reco_numu == 1
const Cut kRecoPosMu    = SIMPLEVAR(dune.reco_q) == +1; // reco_q == 0 if reco_numu != 1
const Cut kMuonCont     = SIMPLEVAR(dune.muon_exit) == 0;
const Cut kEhad_veto    = SIMPLEVAR(dune.Ehad_veto) < 30;

// Binnings
const Binning binsFDEreco = Binning::Simple(80, 0, 10);
const Binning binsNDEreco = Binning::Simple(40, 0, 10);
const Binning binsY       = Binning::Simple(40, 0, 1);

// Axes
const HistAxis axRecoEnuFDnumu("Reco energy (GeV)", binsFDEreco, kRecoE_numu);
const HistAxis axRecoEnuFDnue("Reco energy (GeV)", binsFDEreco, kRecoE_nue);
const HistAxis axErecYrecND("Reco energy (GeV)", binsNDEreco, kRecoEnergyND, "y_{rec}", binsY, kRecoYND);

// POT for 3.5 years
const double pot_fd = 3.5 * 1.47e21 * 40/1.13;
const double pot_nd = 3.5 * 1.47e21;

// FD spectra
SpectrumLoader FD_loaderFHCNumu("/dune/data/users/marshalc/CAFs/mcc10_test/FarDetector/FHC_nonswap.root");
SpectrumLoader FD_loaderFHCNue("/dune/data/users/marshalc/CAFs/mcc10_test/FarDetector/FHC_nueswap.root");
SpectrumLoaderBase& FD_loaderFHCNutau = kNullLoader;

SpectrumLoader FD_loaderRHCNumu("/dune/data/users/marshalc/CAFs/mcc10_test/FarDetector/RHC_nonswap.root");
SpectrumLoader FD_loaderRHCNue("/dune/data/users/marshalc/CAFs/mcc10_test/FarDetector/RHC_nueswap.root");
SpectrumLoaderBase& FD_loaderRHCNutau = kNullLoader;

// FD predictions
PredictionNoExtrap FD_predFHCNumu(FD_loaderFHCNumu, FD_loaderFHCNue, FD_loaderFHCNutau, axRecoEnuFDnumu, kFDSelNumu && kFDPassFV);
PredictionNoExtrap FD_predFHCNue (FD_loaderFHCNumu, FD_loaderFHCNue, FD_loaderFHCNutau, axRecoEnuFDnue , kFDSelNue  && kFDPassFV);
PredictionNoExtrap FD_predRHCNumu(FD_loaderRHCNumu, FD_loaderRHCNue, FD_loaderRHCNutau, axRecoEnuFDnumu, kFDSelNumu && kFDPassFV); // should the cuts be changed?
PredictionNoExtrap FD_predRHCNue (FD_loaderRHCNumu, FD_loaderRHCNue, FD_loaderRHCNutau, axRecoEnuFDnue , kFDSelNue  && kFDPassFV); // should the cuts be changed?

// ND predictions
SpectrumLoader ND_loaderFHC("/dune/data/users/marshalc/CAFs/mcc10_test/NearDetector/LAr/FHC_CAF.root");
SpectrumLoader ND_loaderRHC("/dune/data/users/marshalc/CAFs/mcc10_test/NearDetector/LAr/RHC_CAF.root");

PredictionNoOsc ND_predFHC(ND_loaderFHC, axErecYrecND, kRecoNegMu && kMuonCont && kEhad_veto);
PredictionNoOsc ND_predRHC(ND_loaderRHC, axErecYrecND, kRecoPosMu && kMuonCont && kEhad_veto);

// For the ND prediction generator
Loaders dummyLoaders;

// To get the oscillation probabilities
osc::IOscCalculatorAdjustable* calc = DefaultOscCalc();

// Deal with systematics (could be a separate file)
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

// CW: need to make these systematics work for ND too. I guess add a flag, whether to apply them or not to ND
std::vector<const ISyst*> systlist;
std::vector<const ISyst*> normlist_sig = {&appnorm_sig_fhc, &disnorm_sig_fhc, &appnorm_sig_rhc, &disnorm_sig_rhc};
std::vector<const ISyst*> normlist_bg = {&appnorm_beamnue_fhc, &appnorm_beamnue_rhc, &appnorm_nc, &disnorm_nc, &norm_tau};
std::vector<const ISyst*> fluxlist = GetDUNEFluxSysts(10);
//std::vector<const ISyst*> xseclist = {new GenieSyst(1)};
std::vector<const ISyst*> xseclist = GetGenieSysts();

std::vector<const ISyst*> GetListOfSysts(bool fluxsyst, bool xsecsyst, bool normsyst){

  std::vector<const ISyst*> systlist;
  if (fluxsyst){
    std::cout << "Adding flux systematics" << std::endl;
    systlist.insert(systlist.end(), fluxlist.begin(), fluxlist.end());
  }
  if (normsyst) {
    std::cout << "Adding norm. systematics" << std::endl;
    systlist.insert(systlist.end(), normlist_sig.begin(), normlist_sig.end()); 
  }
  if (xsecsyst) {
    std::cout << "Adding XSEC systematics" << std::endl;
    systlist.insert(systlist.end(), xseclist.begin(), xseclist.end());
  }

  // List all of the systematics we'll be using
  std::cout << "Systematics in this fit: " << std::endl;
  for(const ISyst* s: systlist) std::cout << s->ShortName() << "\t\t" << s->LatexName() << std::endl;
  if (systlist.size()==0) {std::cout << "None" << std::endl;}

  return systlist;
};


// Wow, this is ugly
std::vector<unique_ptr<PredictionInterp> > GetPredictionInterps(std::string fileName, std::vector<const ISyst*> systlist, bool reload=false){

  if(reload || TFile(fileName.c_str()).IsZombie()){
    
    // Need to start some loaders
    Loaders FD_FHC_loaders;
    Loaders FD_RHC_loaders;
    
    // Now fill the loaders
    FD_FHC_loaders .AddLoader(&FD_loaderFHCNumu,  caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNonSwap);
    FD_FHC_loaders .AddLoader(&FD_loaderFHCNue,   caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNueSwap);
    FD_FHC_loaders .AddLoader(&FD_loaderFHCNutau, caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNuTauSwap);

    FD_RHC_loaders .AddLoader(&FD_loaderRHCNumu,  caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNonSwap);
    FD_RHC_loaders .AddLoader(&FD_loaderRHCNue,   caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNueSwap);
    FD_RHC_loaders .AddLoader(&FD_loaderRHCNutau, caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNuTauSwap);

    NoExtrapPredictionGenerator genFDNumuFHC(axRecoEnuFDnumu, kFDSelNumu && kFDPassFV);

    NoExtrapPredictionGenerator genFDNumuRHC(axRecoEnuFDnumu, kFDSelNumu && kFDPassFV);
    
    NoExtrapPredictionGenerator genFDNueFHC(axRecoEnuFDnue, kFDSelNue && kFDPassFV);
    
    NoExtrapPredictionGenerator genFDNueRHC(axRecoEnuFDnue, kFDSelNue && kFDPassFV);
  
    // CW: Still need loaders at this stage for ND...
    NoOscPredictionGenerator genNDNumuFHC(ND_loaderFHC, axErecYrecND, kRecoNegMu && kMuonCont && kEhad_veto);
    
    NoOscPredictionGenerator genNDNumuRHC(ND_loaderRHC, axErecYrecND, kRecoPosMu && kMuonCont && kEhad_veto);
    
    osc::IOscCalculatorAdjustable* this_calc = NuFitOscCalc(1);      
    
    PredictionInterp predInterpFDNumuFHC(systlist,
					 this_calc, 
					 genFDNumuFHC, 
					 FD_FHC_loaders);
    
    PredictionInterp predInterpFDNumuRHC(systlist,
					 this_calc, 
					 genFDNumuRHC, 
					 FD_RHC_loaders);
    
    PredictionInterp predInterpFDNueFHC(systlist,
					this_calc, 
					genFDNueFHC, 
					FD_FHC_loaders);
      
    PredictionInterp predInterpFDNueRHC(systlist,
					this_calc, 
					genFDNueRHC, 
					FD_RHC_loaders);

    PredictionInterp predInterpNDNumuFHC(systlist,
					 this_calc,
					 genNDNumuFHC,
					 dummyLoaders);

    PredictionInterp predInterpNDNumuRHC(systlist,
					 this_calc,
					 genNDNumuRHC,
					 dummyLoaders);

    // Start all of the loaders
    FD_FHC_loaders.Go();
    FD_RHC_loaders.Go();
    ND_loaderFHC  .Go();
    ND_loaderRHC  .Go();
    
    TFile fout(fileName.c_str(), "RECREATE");
    predInterpFDNumuFHC.SaveTo(fout.mkdir("fd_interp_numu_fhc"));
    predInterpFDNueFHC .SaveTo(fout.mkdir("fd_interp_nue_fhc"));
    predInterpFDNumuRHC.SaveTo(fout.mkdir("fd_interp_numu_rhc"));
    predInterpFDNueRHC .SaveTo(fout.mkdir("fd_interp_nue_rhc"));
    predInterpNDNumuFHC.SaveTo(fout.mkdir("nd_interp_numu_fhc"));
    predInterpNDNumuRHC.SaveTo(fout.mkdir("nd_interp_numu_rhc"));
    fout .Close();
  }
  // Argh so ugly
  std::vector<unique_ptr<PredictionInterp> > return_list;
  TFile fin(fileName.c_str());
  return_list.push_back(std::unique_ptr<ana::PredictionInterp> (PredictionInterp::LoadFrom(fin.GetDirectory("fd_interp_numu_fhc")).release()));
  return_list.push_back(std::unique_ptr<ana::PredictionInterp> (PredictionInterp::LoadFrom(fin.GetDirectory("fd_interp_nue_fhc")).release()));
  return_list.push_back(std::unique_ptr<ana::PredictionInterp> (PredictionInterp::LoadFrom(fin.GetDirectory("fd_interp_numu_rhc")).release()));
  return_list.push_back(std::unique_ptr<ana::PredictionInterp> (PredictionInterp::LoadFrom(fin.GetDirectory("fd_interp_nue_rhc")).release()));
  return_list.push_back(std::unique_ptr<ana::PredictionInterp> (PredictionInterp::LoadFrom(fin.GetDirectory("nd_interp_numu_fhc")).release()));
  return_list.push_back(std::unique_ptr<ana::PredictionInterp> (PredictionInterp::LoadFrom(fin.GetDirectory("nd_interp_numu_rhc")).release()));
  fin.Close();
  return return_list;
};
