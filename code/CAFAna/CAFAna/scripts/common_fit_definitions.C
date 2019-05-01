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
#include "CAFAna/Systs/MissingProtonFakeData.h"
#include "CAFAna/Systs/NuWroReweightFakeData.h"
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
const Var kRecoE_FromDep  = SIMPLEVAR(dune.eRec_FromDep);
const Var kFDNumuPid  = SIMPLEVAR(dune.cvnnumu);
const Var kFDNuePid   = SIMPLEVAR(dune.cvnnue);
const Var kMVANUMU    = SIMPLEVAR(dune.mvanumu);

// -->ND
const Var kRecoEnergyND = SIMPLEVAR(dune.Ev_reco);
const Var kRecoYND      = (SIMPLEVAR(dune.Ev_reco) - SIMPLEVAR(dune.Elep_reco))/SIMPLEVAR(dune.Ev_reco);
const Var kRecoY_FromDep  = (SIMPLEVAR(dune.eRec_FromDep) - SIMPLEVAR(dune.LepE))/SIMPLEVAR(dune.eRec_FromDep);

const Var kTrueEnergy  = SIMPLEVAR(dune.Ev);

// CV weighting
const Var kGENIEWeights = SIMPLEVAR(dune.total_cv_wgt); // kUnweighted

// Cuts are now defined in CAFAna/Cuts/AnaCuts.h Removed from here to elide any
// confusion

// ND binning
std::vector<double> binEEdges = {0., 0.75, 1., 1.25, 1.5, 1.75, 2., 2.25, 2.5, 2.75, 3., 3.25, 3.5, 3.75,
                        				 4., 4.25, 4.5, 5., 5.5, 6., 7., 8., 10.};
std::vector<double> binYEdges = {0, 0.1, 0.2, 0.3, 0.4, 0.6, 0.8, 1.0};

// Binnings
const Binning binsFDEreco = Binning::Custom(binEEdges);//Binning::Simple(80, 0, 10);
const Binning binsNDEreco =
    Binning::Custom(binEEdges); // Binning::Simple(40, 0, 10);
const Binning binsNDEreco_OA = Binning::Simple(20, 0, 4);
const Binning binsY = Binning::Custom(binYEdges); // Binning::Simple(5, 0, 1);
const Binning binsETrue = Binning::Simple(100, 0, 10);
const Binning binsETrue_Coarse = Binning::Simple(20, 0, 10);
const Binning binsEreco_Coarse = Binning::Simple(20, 0, 10);
const Binning binsEreco_VeryCoarse = Binning::Simple(5, 0, 10);
const Binning onebin = Binning::Simple(1, 0.5, 10);

// Axes
const HistAxis axRecoEnuFDnumu("Reco energy (GeV)", binsFDEreco, kRecoE_numu);
const HistAxis axRecoEnuFDnue("Reco energy (GeV)", binsFDEreco, kRecoE_nue);
const HistAxis axErecYrecND("Reco energy (GeV)", binsNDEreco, kRecoEnergyND,
                            "y_{rec}", binsY, kRecoYND);
const HistAxis axErecND("Reco energy (GeV)", binsNDEreco, kRecoEnergyND);

const HistAxis axErecFD_FromDep("Reco energy (GeV)", binsFDEreco,
                                   kRecoE_FromDep);
const HistAxis axErecYrecND_FromDep("Reco energy (GeV)", binsNDEreco,
                                    kRecoE_FromDep, "y_{rec}", binsY,
                                    kRecoY_FromDep);
const HistAxis axErecYrecNDOA_FromDep("Reco energy (GeV)", binsNDEreco_OA,
                                      kRecoE_FromDep, "y_{rec}", binsY,
                                      kRecoY_FromDep);
const HistAxis axErecND_FromDep("#it{E}_{#nu}^{Rec.} (GeV)", binsNDEreco,
                              kRecoE_FromDep);

const HistAxis axTrueE_unibin("#it{E}_{#nu} (GeV)", binsETrue, kTrueEnergy);

const HistAxis axTrueE_unibin_coarse("#it{E}_{#nu} (GeV)", binsETrue_Coarse, kTrueEnergy);

const HistAxis axErec_FromDep_unibin("#it{E}_{#nu}^{Rec.} (GeV)", binsETrue,
                                     kRecoE_FromDep);
const HistAxis axErecND_unibin("#it{E}_{#nu}^{Rec.} (GeV)", binsETrue,
                               kRecoEnergyND);
const HistAxis axRecoEnuFDnumu_unibin("Reco energy (GeV)", binsETrue,
                                      kRecoE_numu);
const HistAxis axRecoEnuFDnue_unibin("Reco energy (GeV)", binsETrue,
                                      kRecoE_nue);

const HistAxis axErecND_coarsebin("Reco energy (GeV)", binsEreco_Coarse,
                                  kRecoEnergyND);
const HistAxis axRecoEnuFDnumu_coarsebin("Reco energy (GeV)", binsEreco_Coarse,
                                         kRecoE_numu);

const HistAxis axRecoEnuFDnue_coarsebin("Reco energy (GeV)", binsEreco_Coarse,
                                        kRecoE_nue);

const HistAxis axErecND_verycoarsebin("Reco energy (GeV)", binsEreco_VeryCoarse,
                                      kRecoEnergyND);
const HistAxis axRecoEnuFDnumu_verycoarsebin("Reco energy (GeV)",
                                             binsEreco_VeryCoarse, kRecoE_numu);

const HistAxis axRecoEnuFDnue_verycoarsebin("Reco energy (GeV)",
                                            binsEreco_VeryCoarse, kRecoE_nue);

const HistAxis axErecND_onebin("Reco energy (GeV)", onebin, kRecoEnergyND);
const HistAxis axRecoEnuFDnumu_onebin("Reco energy (GeV)", onebin, kRecoE_numu);

const HistAxis axRecoEnuFDnue_onebin("Reco energy (GeV)", onebin, kRecoE_nue);

struct AxisBlob {
  HistAxis const *NDAx;
  HistAxis const *FDAx_numu;
  HistAxis const *FDAx_nue;
};

AxisBlob const default_axes{&axErecYrecND,&axRecoEnuFDnumu,&axRecoEnuFDnue};
AxisBlob const fake_data_axes{&axErecYrecND_FromDep,&axErecFD_FromDep,&axErecFD_FromDep};


AxisBlob const Ax1DND_unibin{&axErecND_unibin,&axRecoEnuFDnumu_unibin,&axRecoEnuFDnue_unibin};
AxisBlob const Ax1DND_FromDep_unibin{&axErec_FromDep_unibin,&axErec_FromDep_unibin,&axErec_FromDep_unibin};

AxisBlob GetAxisBlob(std::string blob_name) {
  AxisBlob AxToUse = default_axes;
  if (blob_name == "1DND") {
    AxToUse.NDAx = &axErecND;
  } else if (blob_name == "FromDep") {
    AxToUse.NDAx = &axErecYrecND_FromDep;
    AxToUse.FDAx_numu = &axErecFD_FromDep;
    AxToUse.FDAx_nue = &axErecFD_FromDep;
  } else if (blob_name == "1DNDFromDep") {
    AxToUse.NDAx = &axErecND_FromDep;
    AxToUse.FDAx_numu = &axErecFD_FromDep;
    AxToUse.FDAx_nue = &axErecFD_FromDep;
  } else if (blob_name == "CoarseBin1D") {
    AxToUse.NDAx = &axErecND_coarsebin;
    AxToUse.FDAx_numu = &axRecoEnuFDnumu_coarsebin;
    AxToUse.FDAx_nue = &axRecoEnuFDnue_coarsebin;
  } else if (blob_name == "VeryCoarseBin1D") {
    AxToUse.NDAx = &axErecND_verycoarsebin;
    AxToUse.FDAx_numu = &axRecoEnuFDnumu_verycoarsebin;
    AxToUse.FDAx_nue = &axRecoEnuFDnue_verycoarsebin;
  } else if (blob_name == "OneBin") {
    AxToUse.NDAx = &axErecND_onebin;
    AxToUse.FDAx_numu = &axRecoEnuFDnumu_onebin;
    AxToUse.FDAx_nue = &axRecoEnuFDnue_onebin;
  } else if (blob_name == "ETrue") {
    AxToUse.NDAx = &axTrueE_unibin;
    AxToUse.FDAx_numu = &axTrueE_unibin;
    AxToUse.FDAx_nue = &axTrueE_unibin;
  } else if (blob_name == "ETrueCoarse") {
    AxToUse.NDAx = &axTrueE_unibin_coarse;
    AxToUse.FDAx_numu = &axTrueE_unibin_coarse;
    AxToUse.FDAx_nue = &axTrueE_unibin_coarse;
  } else if (blob_name == "FakeData") {
    AxToUse = fake_data_axes;
  }
  return AxToUse;
}

// POT for 3.5 years
const double pot_fd = 3.5 * POT120 * 40/1.13;
const double pot_nd = 3.5 * POT120;
// This is pretty annoying, but the above is for 7 years staged, which is 336 kT MW yr
const double nom_exposure = 336.;

// Global file path...
#ifndef DONT_USE_FQ_HARDCODED_SYST_PATHS
const std::string cafFilePath="/dune/data/users/marshalc/CAFs/mcc11_v3";
#else
const std::string cafFilePath="root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/dune/persistent/users/picker24/CAFv4/";
#endif

bool const UseOffAxisFluxUncertainties = false; //true;
size_t const NFluxParametersToUse = 10; //30;

double GetBoundedGausThrow(double min, double max){
  double val = -999;
  while (val > max || val < min) val = gRandom->Gaus();
  return val;
}

// I miss python...
std::vector<std::string> SplitString(std::string input, char delim = ' '){
  std::vector<std::string> output;
  std::stringstream ss(input);
  std::string token;
  while (std::getline(ss, token, delim)) output.push_back(token);
  return output;
}

// For ease of penalty terms...
IExperiment* GetPenalty(int hie, int oct, std::string penalty, std::string asimov_set="0", bool modConstraint=false){

  // First, decide which to use
  std::vector<std::string> penalties = SplitString(penalty, ':');
  bool useTh13   = false;
  bool useDmsq32 = false;
  bool useTh23   = false;

  for (auto & str : penalties){
    if (str == "th13" || str == "allpen") useTh13 = true;
    if (str == "dmsq32" || str == "allpen") useDmsq32 = true;
    if (str == "th23" || str == "allpen") useTh23 = true;
  }

  Penalizer_GlbLike *ret = new Penalizer_GlbLike(hie, oct, useTh13, useDmsq32, useTh23, 0);
  ret->SetAsimovPoint(asimov_set, modConstraint);
  return ret;
}

std::vector<const IFitVar *> GetOscVars(std::string oscVarString, int hie = 0) {

  std::vector<std::string> osc_vars = SplitString(oscVarString, ':');

  std::vector<const IFitVar *> rtn_vars;

  for (auto &v : osc_vars) {
    if (v == "th13" || v == "alloscvars") {
      rtn_vars.push_back(&kFitTheta13);
    }
    // Deal with bounded dmsq32
    if (hie == -1){
      if (v == "dmsq32" || v == "alloscvars") {
	rtn_vars.push_back(&kFitDmSq32IHScaled);
      }
    } else if (hie == 1){
      if (v == "dmsq32" || v == "alloscvars") {
	rtn_vars.push_back(&kFitDmSq32NHScaled);
      }
    } else {
      if (v == "dmsq32" || v == "alloscvars") {
	rtn_vars.push_back(&kFitDmSq32Scaled);
      }
    }
    if (v == "th23" || v == "alloscvars") {
      rtn_vars.push_back(&kFitSinSqTheta23);
    }
    if (v == "deltapi" || v == "alloscvars") {
      rtn_vars.push_back(&kFitDeltaInPiUnits);
    }
  }
  return rtn_vars;
}


// Take a list of all the systs known about, and retain the named systs...
void KeepSysts(std::vector<const ISyst *> &systlist,
	       std::vector<std::string> const &systsToInclude) {
  systlist.erase(std::remove_if(systlist.begin(), systlist.end(),
                                [&](const ISyst *s) {
                                  return (std::find(systsToInclude.begin(),
						    systsToInclude.end(),
						    s->ShortName()) ==
                                          systsToInclude.end());
                                }),
                 systlist.end());
}

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


std::vector<const ISyst*> GetListOfSysts(bool fluxsyst=true, bool xsecsyst=true, bool detsyst=true,
					 bool useND=true, bool useFD=true,
					 bool useNueOnE=false,
					 bool useMissingProtonFakeData=true, bool useNuWroReweightFakeData=true){
  // This doesn't need to be an argument because I basically never change it:
  bool fluxXsecPenalties = true;

  std::vector<const ISyst *> systlist;
  if (fluxsyst) {
    std::vector<const ISyst *> fluxlist =
        GetDUNEFluxSysts(NFluxParametersToUse, fluxXsecPenalties, UseOffAxisFluxUncertainties);

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
    std::vector<const ISyst*> xseclist = GetGenieSysts(GetGenieWeightNames(), fluxXsecPenalties, true); // the trailing true says on/off dials should extrapolate to -1 rather than mirror.
    systlist.insert(systlist.end(), xseclist.begin(), xseclist.end());
  }

  if(useMissingProtonFakeData){
    systlist.push_back(GetMissingProtonEnergyFakeDataSyst().front());
  }
  if(useNuWroReweightFakeData){
    systlist.push_back(GetNuWroReweightFakeDataSyst().front());
  }

  // For now, hard code this part... too many damned scripts to change...
  RemoveSysts(systlist, {"eScaleND","eScaleMuLArND", "eScaleMuND", "ChargedHadCorr", "ChargedHadAnticorrSyst",
	"eScaleN_ND", "EMUncorrND", "MuonResND","EMResND", "ChargedHadResND", "UncorrNDHadLinSyst", "UncorrNDPi0LinSyst",
	"UncorrNDNLinSyst", "UncorrNDHadSqrtSyst", "UncorrNDPi0SqrtSyst", "UncorrNDNSqrtSyst", "LeptonAccSyst", "HadronAccSyst"});
  // Get rid of these too...
  RemoveSysts(systlist, {"MFP_N", "MFP_pi", "FormZone"});
  // Woops, this got lost when reverting to the pre-Hack days version
  RemoveSysts(systlist, {"RDecBR1gamma", "RDecBR1eta", "EtaNCEL", "BeRPA_E", "FSILikeEAvailSmearing"});

  return systlist;
};

// Define a few groups of systematics:
//nofd_det, nofd_escale, nofd_muon_escale, noxsec_qe, noxsec_res, noxsec_dis, noxsec_fsi, noxsec_ratios

// All detector nuisance parameters
std::vector<std::string> fd_det_list = {"eScaleFD", "eScaleMuLArFD", "eScaleN_FD", "EMUncorrFD", "MuonResFD", 
					"EMResFD", "ChargedHadResFD", "FDRecoNumuSyst", "FDRecoNueSyst", "FVNumuFD", "FVNueFD"};

// FD detector subsets
std::vector<std::string> fd_escale_list = {"eScaleFD", "eScaleMuLArFD", "eScaleN_FD"};
std::vector<std::string> fd_muon_escale_list = {"eScaleMuLArFD"};
std::vector<std::string> fd_eres_list = {"MuonResFD", "EMResFD", "ChargedHadResFD"};
std::vector<std::string> fd_muon_eres_list = {"MuonResFD"};

// All QE XSEC parameters
std::vector<std::string> xsec_qe_list = {"MaCCQE", "VecFFCCQEshape","CCQEPauliSupViaKF", "Mnv2p2hGaussEnhancement","E2p2h_A_nu", 
					 "E2p2h_B_nu", "E2p2h_A_nubar", "E2p2h_B_nubar","BeRPA_A", "BeRPA_B", "BeRPA_D",
					 "C12ToAr40_2p2hScaling_nu", "C12ToAr40_2p2hScaling_nubar"};

// All RES XSEC parameters
std::vector<std::string> xsec_res_list = {"MaCCRES", "MvCCRES", "MaNCRES", "MvNCRES", "NR_nu_np_CC_1Pi", 
					  "NR_nu_n_NC_1Pi", "NR_nu_p_NC_1Pi", "NR_nubar_n_CC_1Pi", "NR_nubar_p_CC_1Pi", 
					  "NR_nubar_n_NC_1Pi", "NR_nubar_p_NC_1Pi", "SPPLowQ2Suppression"};

// All NR_* and AKGY
std::vector<std::string> xsec_dis_list = {"AhtBY", "BhtBY", "CV1uBY", "CV2uBY","NR_nu_n_CC_2Pi", "NR_nu_n_CC_3Pi", "NR_nu_p_CC_2Pi", 
					  "NR_nu_p_CC_3Pi", "NR_nu_n_NC_2Pi", "NR_nu_n_NC_3Pi", "NR_nu_p_NC_2Pi", "NR_nu_p_NC_3Pi", 
					  "NR_nubar_n_CC_2Pi", "NR_nubar_n_CC_3Pi", "NR_nubar_p_CC_2Pi","NR_nubar_p_CC_3Pi", 
					  "NR_nubar_n_NC_2Pi", "NR_nubar_n_NC_3Pi", "NR_nubar_p_NC_2Pi", "NR_nubar_p_NC_3Pi",
					  "Theta_Delta2Npi"};

// All nuclear effects/FSI
std::vector<std::string> xsec_fsi_list = {"FrCEx_pi", "FrElas_pi", "FrInel_pi", "FrAbs_pi", "FrPiProd_pi", 
					  "FrCEx_N", "FrElas_N", "FrInel_N", "FrAbs_N", "FrPiProd_N"};

std::vector<std::string> xsec_ratios_list = {"nuenuebar_xsec_ratio", "nuenumu_xsec_ratio"};

std::vector<const ISyst*> GetListOfSysts(std::string systString,
					 bool useND=true, bool useFD=true,
					 bool useNueOnE=false,
				 	 bool useMissingProtonFakeData=false,
				 	 bool useNuWroReweightFakeData=false){
  // Now defaults to true!
  bool detsyst  = true;
  bool fluxsyst = true;
  bool xsecsyst = true;

  // If you find an argument in the form list:name1:name2:name3 etc etc, keep only those systematics
  // This is pretty much a magic option to allow single parameters... there must be a better way, but for now I'm just going to continue to support it
  if (systString.find("list") != std::string::npos){

    // 1) Get a default list with everything
    std::vector<const ISyst*> namedList = GetListOfSysts(true, true, true, useND, useFD, useNueOnE);
    // for (auto & syst : namedList) std::cout << syst->ShortName() << std::endl;
    // 2) Interpret the list of short names
    std::vector<std::string> systs = SplitString(systString, ':');

    // 3) Don't include "list"
    systs.erase(systs.begin());

    // 4) Regret nothing
    KeepSysts(namedList, systs);

    // 5) $$$ Profit
    return namedList;
  }

  // Can't transform anymore, so... BEHAVE YOURSELF
  // std::transform(systString.begin(), systString.end(), systString.begin(), ::tolower);

  // Do even more horrific things...
  std::vector<std::string> systs = SplitString(systString, ':');

  // Start off by checking for certain keywords
  for (auto syst : systs){
    if (syst == "allsyst"){
      xsecsyst = true;
      fluxsyst = true;
      detsyst = true;
    }
    
    if (syst == "nosyst"){
      xsecsyst = false;
      fluxsyst = false;
      detsyst = false;
    }

    // Now we're getting a bit funky as these options now conflict.
    // But, if you do something stupid, YOU ONLY HAVE YOURSELF TO BLAME
    if (syst == "nodet") detsyst = false;
    if (syst == "noflux") fluxsyst = false;
    if (syst == "noxsec") xsecsyst = false;

    // Something else really horrible to keep this supported...
    if (syst == "prot_fakedata") useMissingProtonFakeData = true;
    if (syst == "nuwro_fakedata") useNuWroReweightFakeData = true;
  }

  // Okay, now get the list, and start from there...
  std::vector<const ISyst*> namedList = GetListOfSysts(fluxsyst, xsecsyst, detsyst,
						       useND, useFD, useNueOnE, 
						       useMissingProtonFakeData, 
						       useNuWroReweightFakeData);

  // Now do something REALLY FUNKY. Remove specific dials from the list we already have
  // Need to allow single dials, and a few specific groups...
  for (auto syst : systs){
    // ignore anything we previously dealt with
    if (syst == "noxsec" ||
	syst == "nodet" ||
	syst == "noflux")
      continue;
    // Now remove some specific groups
    // nofd_det, nofd_escale, nofd_muon_escale, noxsec_qe, noxsec_res, noxsec_dis, noxsec_fsi, noxsec_ratios
    else if (syst == "nofd_det") RemoveSysts(namedList, fd_det_list);
    else if (syst == "nofd_escale") RemoveSysts(namedList, fd_escale_list);
    else if (syst == "nofd_muon_escale") RemoveSysts(namedList, fd_muon_escale_list);
    else if (syst == "noxsec_qe") RemoveSysts(namedList, xsec_qe_list);
    else if (syst == "noxsec_res") RemoveSysts(namedList, xsec_res_list);
    else if (syst == "noxsec_dis") RemoveSysts(namedList, xsec_dis_list);
    else if (syst == "noxsec_fsi") RemoveSysts(namedList, xsec_fsi_list);
    else if (syst == "noxsec_ratios") RemoveSysts(namedList, xsec_ratios_list);
    // If not, remove as if it's a single parameter instruction
    else RemoveSysts(namedList, {syst.erase(0, 2)});
  }  
  
  // Now return the list
  return namedList;
}

std::vector<const ISyst*> GetListOfSysts(char const *systCString,
					 bool useND=true, bool useFD=true,
					 bool useNueOnE=false,
				 	 bool useMissingProtonFakeData=false,
					 bool useNuWroReweightFakeData=false){
						 return GetListOfSysts(std::string(systCString), useND, useFD, useNueOnE);
					 }

// Use a sample enum, maybe this should live elsewhere?
enum SampleType{kFDFHC, kFDRHC, kNDFHC, kNDRHC, kNDNue, kNDFHC_OA, kUnknown};

std::string GetSampleName(SampleType sample){
  switch(sample){
  case kFDFHC : return "FD_FHC";
  case kFDRHC : return "FD_RHC";
  case kNDFHC : return "ND_FHC";
  case kNDRHC : return "ND_RHC";
  case kNDNue : return "ND_nue";
  case kNDFHC_OA /*how you like this space, Callum?*/     : return "ND_FHC_OA";
  case  /* LOVE IT M8                */ kUnknown :
  default : return "NONE";
  }
};

SampleType GetSampleType(std::string sample){

  if (sample == "FD_FHC") return kFDFHC;
  if (sample == "FD_RHC") return kFDRHC;
  if (sample == "ND_FHC") return kNDFHC;
  if (sample == "ND_RHC") return kNDRHC;
  if (sample == "ND_nue") return kNDNue;
  if (sample == "ND_FHC_OA") return kNDFHC_OA;
  return kUnknown;
}


void MakePredictionInterp(TDirectory* saveDir, SampleType sample,
			  std::vector<const ISyst*> systlist,
			  int max=0, AxisBlob const &axes = default_axes){

  // Move to the save directory
  saveDir->cd();
  osc::IOscCalculatorAdjustable* this_calc = NuFitOscCalc(1);

bool isfhc = ((sample == kNDFHC) ||
(sample == kNDFHC_OA) || (sample == kFDFHC));

  // FD samples
  if ((sample == kFDFHC) || (sample == kFDRHC)){

    Loaders these_loaders;
    SpectrumLoader loaderNumu(cafFilePath+"/"+ GetSampleName(sample)+"_nonswap.root", kBeam, max);
    SpectrumLoader loaderNue(cafFilePath+"/"+ GetSampleName(sample)+"_nueswap.root", kBeam, max);
    SpectrumLoader loaderNutau(cafFilePath+"/"+ GetSampleName(sample)+"_tauswap.root", kBeam, max);

    these_loaders .AddLoader(&loaderNumu, caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNonSwap);
    these_loaders .AddLoader(&loaderNue, caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNueSwap);
    these_loaders .AddLoader(&loaderNutau, caf::kFARDET, Loaders::kMC, ana::kBeam, Loaders::kNuTauSwap);

    NoExtrapPredictionGenerator genFDNumu(*axes.FDAx_numu, kPassFD_CVN_NUMU && kIsTrueFV, kGENIEWeights);
    NoExtrapPredictionGenerator genFDNue(*axes.FDAx_nue, kPassFD_CVN_NUE && kIsTrueFV, kGENIEWeights);
    PredictionInterp predInterpFDNumu(systlist, this_calc, genFDNumu, these_loaders);
    PredictionInterp predInterpFDNue(systlist, this_calc, genFDNue, these_loaders);
    these_loaders.Go();

    std::cout << "Saving " << GetSampleName(sample) << std::endl;
    predInterpFDNumu.SaveTo(saveDir->mkdir((std::string("fd_interp_numu_") + std::string(isfhc ? "fhc" : "rhc")).c_str()));
    std::cout << "Saving " << GetSampleName(sample) << std::endl;
    predInterpFDNue .SaveTo(saveDir->mkdir((std::string("fd_interp_nue_") + std::string(isfhc ? "fhc" : "rhc")).c_str()));

  } else if ((sample == kNDFHC) || (sample == kNDRHC) || (sample == kNDFHC_OA)){

    // Now ND
    Loaders these_loaders;
    SpectrumLoader loaderNumu(cafFilePath+"/"+ GetSampleName(sample)+"_CAF.root", kBeam, max);
    these_loaders .AddLoader(&loaderNumu, caf::kNEARDET, Loaders::kMC);

    NoOscPredictionGenerator genNDNumu(*axes.NDAx, (isfhc ? kPassND_FHC_NUMU : kPassND_RHC_NUMU)
     				       && kIsTrueFV, kGENIEWeights);

    PredictionInterp predInterpNDNumu(systlist, this_calc, genNDNumu, these_loaders);
    these_loaders.Go();

    std::cout << "Saving " << GetSampleName(sample) << std::endl;
    predInterpNDNumu.SaveTo(saveDir->mkdir((std::string("nd_interp_numu_") + std::string(isfhc ? "fhc" : "rhc")).c_str()));
  }
}

bool const kAddSampleTypeToFilename = true;
bool const kFileContainsAllSamples = false;

static std::vector<std::string> const sample_dir_order = {
    "fd_interp_numu_fhc", "fd_interp_nue_fhc",  "fd_interp_numu_rhc",
    "fd_interp_nue_rhc",  "nd_interp_numu_fhc", "nd_interp_numu_rhc"};
static std::vector<std::string> const sample_suffix_order = {
    "FD_FHC", "FD_FHC", "FD_RHC", "FD_RHC", "ND_FHC", "ND_RHC"};

std::vector<std::unique_ptr<ana::PredictionInterp>>
GetPredictionInterps(std::string fileName, std::vector<const ISyst *> systlist,
                     int max = 0, bool reload = false,
                     AxisBlob const &axes = default_axes) {

  // Hackity hackity hack hack
  bool fileNameIsStub = (fileName.find(".root") == std::string::npos);

  //To allow XRootD input files.
  TFile *testF = !fileNameIsStub ? TFile::Open(fileName.c_str(),"READ") : nullptr;
  if (!fileNameIsStub && (reload || testF->IsZombie())) {
    if(fileName.find("root://") == 0){
      std::cout << "Passed xrootd path that cannot be opened, we cannot regenerate input histograms with this. Aborting." << std::endl;
      abort();
    }

    std::cout << "Now creating PredictionInterps in series... maybe you should "
                 "use the other scripts to do them in parallel?"
              << std::endl;

    TFile fout(fileName.c_str(), "RECREATE");
    MakePredictionInterp(&fout, kFDFHC, systlist, max, axes);
    MakePredictionInterp(&fout, kFDRHC, systlist, max, axes);
    MakePredictionInterp(&fout, kNDFHC, systlist, max, axes);
    MakePredictionInterp(&fout, kNDRHC, systlist, max, axes);
    fout.Close();
  }

  std::vector<std::unique_ptr<PredictionInterp>> return_list;

  for (size_t s_it = 0; s_it < sample_dir_order.size(); ++s_it) {
    std::string state_fname =
        fileNameIsStub ? fileName + "_" + sample_suffix_order[s_it] + ".root"
                       : fileName;
    TFile *fin = TFile::Open(state_fname.c_str(),"READ"); // Allows xrootd streaming
    assert(fin && !fin->IsZombie());
    std::cout << "Retrieving " << sample_dir_order[s_it] << " from "
              << state_fname << ":" << sample_dir_order[s_it] << std::endl;
    return_list.emplace_back(LoadFrom<PredictionInterp>(
        fin->GetDirectory(sample_dir_order[s_it].c_str())));
    delete fin;
  }
  return return_list;
};

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
// NOW DEPRECATED
MultiExperiment GetMultiExperiment(std::string stateFileName, double pot_nd_fhc, double pot_nd_rhc, double pot_fd_fhc, double pot_fd_rhc,
				   osc::IOscCalculatorAdjustable* fakeDataOsc, SystShifts fakeDataSyst=kNoShift,
				   bool stats_throw=false){

  std::cout << "If you're using GetMultiExperiment, you need to update it!" << std::endl;
  // Start by getting the PredictionInterps... better that this is done here than elsewhere as they aren't smart enough to know what they are (so the order matters)
  // Note that all systs are used to load the PredictionInterps
  static std::vector<std::unique_ptr<PredictionInterp> > interp_list = GetPredictionInterps(stateFileName, GetListOfSysts());
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

  const Spectrum data_nue_fhc = predFDNueFHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_fd_fhc, stats_throw);
  SingleSampleExperiment *app_expt_fhc = new SingleSampleExperiment(&predFDNueFHC, data_nue_fhc);
  app_expt_fhc->SetMaskHist(0.5, 8);
  iHateThisSoMuch.push_back(app_expt_fhc);

  const Spectrum data_nue_rhc = predFDNueRHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_fd_fhc, stats_throw);
  SingleSampleExperiment *app_expt_rhc = new SingleSampleExperiment(&predFDNueRHC, data_nue_rhc);
  app_expt_rhc->SetMaskHist(0.5, 8);
  iHateThisSoMuch.push_back(app_expt_rhc);

  const Spectrum data_numu_fhc = predFDNumuFHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_fd_rhc, stats_throw);
  SingleSampleExperiment *dis_expt_fhc = new SingleSampleExperiment(&predFDNumuFHC, data_numu_fhc);
  dis_expt_fhc->SetMaskHist(0.5, 8);
  iHateThisSoMuch.push_back(dis_expt_fhc);

  const Spectrum data_numu_rhc = predFDNumuRHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_fd_rhc, stats_throw);
  SingleSampleExperiment *dis_expt_rhc = new SingleSampleExperiment(&predFDNumuRHC, data_numu_rhc);
  dis_expt_rhc->SetMaskHist(0.5, 8);
  iHateThisSoMuch.push_back(dis_expt_rhc);

  const Spectrum nd_data_numu_fhc = predNDNumuFHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_nd_fhc, stats_throw);
  SingleSampleExperiment *nd_expt_fhc = new SingleSampleExperiment(&predNDNumuFHC, nd_data_numu_fhc);
  //nd_expt_fhc.SetMaskHist(0.5, -1, 0.1, 1);
  iHateThisSoMuch.push_back(nd_expt_fhc);

  const Spectrum nd_data_numu_rhc = predNDNumuRHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_nd_rhc, stats_throw);
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

// Yet another string parser that does far too much. I can't be stopped!
void ParseDataSamples(std::string cmdLineInput, double& pot_nd_fhc, double& pot_nd_rhc,
		      double& pot_fd_fhc_nue, double& pot_fd_rhc_nue, double& pot_fd_fhc_numu,
		      double& pot_fd_rhc_numu){

  // Did somebody say overextend the command line arguments even further?
  // Well okay!
  std::vector<std::string> input_vect = SplitString(cmdLineInput, ':');
  
  // Default to 7 years staged. Value is actually in kt MW yr
  double exposure = 336;
  if (input_vect.size() > 1) exposure = stod(input_vect[1]);
  std::string input = input_vect[0];

  // LoWeR cAsE sO i CaN bE sIlLy WiTh InPuTs
  std::transform(input.begin(), input.end(), input.begin(), ::tolower);

  // Look for some other magic information
  for (auto str : input_vect){
    if (str.find("full") != std::string::npos or str.find("15year") != std::string::npos)
      exposure = 1104;
    
    if (str.find("nom") != std::string::npos or str.find("7year") != std::string::npos)
      exposure = 336;
    
    if (str.find("10year") != std::string::npos)
      exposure = 624;
  }

  double exposure_ratio = exposure/nom_exposure;
  std::cout << "Using exposure: " << exposure << "; and ratio: " << exposure_ratio << std::endl;
  
  // Now sort out which samples to include
  pot_nd_fhc = pot_nd_rhc = pot_fd_fhc_nue = pot_fd_rhc_nue = pot_fd_fhc_numu = pot_fd_rhc_numu = 0;

  // Hacky McHackerson is here to stay!
  if (input.find("nd") != std::string::npos){
    pot_nd_fhc = pot_nd_rhc = pot_nd*exposure_ratio;
  }
  if (input.find("fd") != std::string::npos){
    pot_fd_fhc_nue = pot_fd_rhc_nue = pot_fd_fhc_numu = pot_fd_rhc_numu = pot_fd*exposure_ratio;
  }

  // Now allow specific subsets
  if (input.find("fhc") != std::string::npos){
    pot_nd_rhc = pot_fd_rhc_nue = pot_fd_rhc_numu = 0;
  }

  if (input.find("rhc") != std::string::npos){
    pot_nd_fhc = pot_fd_fhc_nue = pot_fd_fhc_numu = 0;
  }

  if (input.find("numu") != std::string::npos){
    pot_fd_fhc_nue = pot_fd_rhc_nue = 0;
  }

  if (input.find("nue") != std::string::npos){
    pot_fd_fhc_numu = pot_fd_rhc_numu = 0;
  }
  return;
}

void ParseThrowInstructions(std::string throwString, bool &stats, bool &fakeOA, 
			    bool &fakeNuis, bool &start, bool &central) {

  std::vector<std::string> instructions = SplitString(throwString, ':');

  stats = false;
  fakeOA = false;
  fakeNuis = false;
  start = false;
  central = false;

  for (auto &str : instructions) {

    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    if (str == "stat" || str == "all")
      stats = true;
    if (str == "fake" || str == "all")
      fakeOA = fakeNuis = true;
    if (str == "fakeoa" || str == "all")
      fakeOA = true;
    if (str == "fakenuis"|| str == "all")
      fakeNuis = true;
    if (str == "start" || str == "all")
      start = true;
    if (str == "central" || str == "all")
      central = true;
  }
  return;
}


void SaveTrueOAParams(TDirectory *outDir, osc::IOscCalculatorAdjustable *calc, std::string tree_name="true_OA"){
  
  outDir->cd();
  double L = calc->GetL();
  double rho = calc->GetRho();
  double dmsq21 = calc->GetDmsq21();
  double dmsq32 = calc->GetDmsq32();
  double th12 = calc->GetTh12();
  double th13 = calc->GetTh13();
  double th23 = calc->GetTh23();
  double ssth23 = sin(calc->GetTh23())*sin(calc->GetTh23());
  double deltapi = calc->GetdCP()/TMath::Pi();

  TTree *tree = new TTree(tree_name.c_str(), tree_name.c_str());
  tree->Branch("L", &L);
  tree->Branch("rho", &rho);
  tree->Branch("dmsq21", &dmsq21);
  tree->Branch("dmsq32", &dmsq32);
  tree->Branch("th12", &th12);
  tree->Branch("th13", &th13);
  tree->Branch("th23", &th23);
  tree->Branch("ssth23", &ssth23);
  tree->Branch("deltapi", &deltapi);
  tree->Fill();
  tree->Write();
  return;
}

struct FitTreeBlob {
  FitTreeBlob(std::string tree_name = "") {
    if (tree_name.size()) {
      throw_tree = new TTree(tree_name.c_str(), tree_name.c_str());
      throw_tree->Branch("chisq", &fChiSq);
      throw_tree->Branch("NFCN", &fNFCN);
      throw_tree->Branch("EDM", &fEDM);
      throw_tree->Branch("IsValid", &fIsValid);
      throw_tree->Branch("fParamNames", &fParamNames);
      throw_tree->Branch("fFakeDataVals", &fFakeDataVals);
      throw_tree->Branch("fPreFitValues", &fPreFitValues);
      throw_tree->Branch("fPreFitErrors", &fPreFitErrors);
      throw_tree->Branch("fPostFitValues", &fPostFitValues);
      throw_tree->Branch("fPostFitErrors", &fPostFitErrors);
      throw_tree->Branch("fMinosErrors", &fMinosErrors);
      throw_tree->Branch("fCentralValues", &fCentralValues);
    }
  }
  void CopyVals(FitTreeBlob const &fb) {
    fFakeDataVals = fb.fFakeDataVals;
    fParamNames = fb.fParamNames;
    fPreFitValues = fb.fPreFitValues;
    fPreFitErrors = fb.fPreFitErrors;
    fPostFitValues = fb.fPostFitValues;
    fPostFitErrors = fb.fPostFitErrors;
    fMinosErrors = fb.fMinosErrors;
    fCentralValues = fb.fCentralValues;
    fChiSq = fb.fChiSq;
    fNFCN = fb.fNFCN;
    fEDM = fb.fEDM;
    fIsValid = fb.fIsValid;
  }
  TTree *throw_tree;
  std::vector<double> fFakeDataVals;
  std::vector<std::string> fParamNames;
  std::vector<double> fPreFitValues;
  std::vector<double> fPreFitErrors;
  std::vector<double> fPostFitValues;
  std::vector<double> fPostFitErrors;
  std::vector<double> fCentralValues;
  std::vector<std::pair<double,double>> fMinosErrors;
  double fChiSq;
  double fNFCN;
  double fEDM;
  bool fIsValid;
};

double RunFitPoint(std::string stateFileName, std::string sampleString,
		   osc::IOscCalculatorAdjustable* fakeDataOsc, SystShifts fakeDataSyst, bool fakeDataStats,
		   std::vector<const IFitVar*> oscVars, std::vector<const ISyst*> systlist,
		   osc::IOscCalculatorAdjustable* fitOsc, SystShifts fitSyst,
		   std::map<const IFitVar*, std::vector<double>> oscSeeds={},
		   IExperiment *penaltyTerm=NULL, Fitter::Precision fitStrategy=Fitter::kNormal,
		   TDirectory *outDir=NULL, FitTreeBlob *PostFitTreeBlob=nullptr, 
		   std::vector<unique_ptr<Spectrum> > *spectra = nullptr, SystShifts &bf = junkShifts){

  assert(systlist.size()+oscVars.size());

  // Start by getting the PredictionInterps... better that this is done here than elsewhere as they aren't smart enough to know what they are (so the order matters)
  // Note that all systs are used to load the PredictionInterps
  static std::vector<std::unique_ptr<PredictionInterp> > interp_list = GetPredictionInterps(stateFileName, GetListOfSysts());
  static PredictionInterp& predFDNumuFHC = *interp_list[0].release();
  static PredictionInterp& predFDNueFHC  = *interp_list[1].release();
  static PredictionInterp& predFDNumuRHC = *interp_list[2].release();
  static PredictionInterp& predFDNueRHC  = *interp_list[3].release();
  static PredictionInterp& predNDNumuFHC = *interp_list[4].release();
  static PredictionInterp& predNDNumuRHC = *interp_list[5].release();

  // Get the ndCov
#ifndef DONT_USE_FQ_HARDCODED_SYST_PATHS
  std::string covFileName =
      cafFilePath+"/ND_syst_cov_withRes.root";
#else
  std::string covFileName =
      FindCAFAnaDir() + "/Systs/ND_syst_cov_withRes.root";
#endif
  std::string covName = "nd_frac_cov";

  // String parsing time!
  double pot_nd_fhc, pot_nd_rhc, pot_fd_fhc_nue, pot_fd_rhc_nue, pot_fd_fhc_numu, pot_fd_rhc_numu;
  ParseDataSamples(sampleString, pot_nd_fhc, pot_nd_rhc,
		   pot_fd_fhc_nue, pot_fd_rhc_nue, pot_fd_fhc_numu,
		   pot_fd_rhc_numu);

  // If a directory has been given, a whole mess of stuff will be saved there.
  if (outDir) outDir->cd();

  // Need to make a fake throw list for later book-keeping
  std::vector<double> fFakeDataVals;
  for(const IFitVar* v: oscVars) fFakeDataVals.push_back(v->GetValue(fakeDataOsc));
  for(const ISyst* s: systlist) fFakeDataVals.push_back(fakeDataSyst.GetShift(s));

  // One problem with this method is that the experiments are reproduced for every single call...
  // Set up the fake data histograms, and save them if relevant...
  // This came back to bite me. I need to do multiple fits with the same fake data throw, so I need these to persist between calls to this function
  // Sadly, the locked down ownership etc means I can't think of a better solution to this
  // And the alternative CAFAna is probably just copy-pasta-ing this function in all of the scripts that use it...
  std::vector<unique_ptr<Spectrum> > LastShredsOfMyDignityAndSanity;
  if(!spectra){ spectra = &LastShredsOfMyDignityAndSanity; }

  if (!spectra->size()){
    std::cout << "Remaking spectra" << std::endl;
    spectra->emplace_back(std::unique_ptr<Spectrum>(new Spectrum(predFDNueFHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_fd_fhc_nue, fakeDataStats))));
    spectra->emplace_back(std::unique_ptr<Spectrum>(new Spectrum(predFDNumuFHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_fd_fhc_numu, fakeDataStats))));
    spectra->emplace_back(std::unique_ptr<Spectrum>(new Spectrum(predFDNueRHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_fd_rhc_nue, fakeDataStats))));
    spectra->emplace_back(std::unique_ptr<Spectrum>(new Spectrum(predFDNumuRHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_fd_rhc_numu, fakeDataStats))));
    spectra->emplace_back(std::unique_ptr<Spectrum>(new Spectrum(predNDNumuFHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_nd_fhc, fakeDataStats))));
    spectra->emplace_back(std::unique_ptr<Spectrum>(new Spectrum(predNDNumuRHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_nd_rhc, fakeDataStats))));
  }

  // const Spectrum data_nue_fhc = predFDNueFHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_fd_fhc_nue, fakeDataStats);
  SingleSampleExperiment app_expt_fhc(&predFDNueFHC, *(*spectra)[0]);
  app_expt_fhc.SetMaskHist(0.5, 8);

  // const Spectrum data_numu_fhc = predFDNumuFHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_fd_fhc_numu, fakeDataStats);
  SingleSampleExperiment dis_expt_fhc(&predFDNumuFHC, *(*spectra)[1]);
  dis_expt_fhc.SetMaskHist(0.5, 8);

  // const Spectrum data_nue_rhc = predFDNueRHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_fd_rhc_nue, fakeDataStats);
  SingleSampleExperiment app_expt_rhc(&predFDNueRHC, *(*spectra)[2]);
  app_expt_rhc.SetMaskHist(0.5, 8);

  // const Spectrum data_numu_rhc = predFDNumuRHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_fd_rhc_numu, fakeDataStats);
  SingleSampleExperiment dis_expt_rhc(&predFDNumuRHC, *(*spectra)[3]);
  dis_expt_rhc.SetMaskHist(0.5, 8);

  // const Spectrum nd_data_numu_fhc = predNDNumuFHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_nd_fhc, fakeDataStats);
  SingleSampleExperiment nd_expt_fhc(&predNDNumuFHC, *(*spectra)[4], covFileName, covName);
  nd_expt_fhc.SetMaskHist(0.5, 10, 0, -1);

  // const Spectrum nd_data_numu_rhc = predNDNumuRHC.PredictSyst(fakeDataOsc, fakeDataSyst).MockData(pot_nd_rhc, fakeDataStats);
  SingleSampleExperiment nd_expt_rhc(&predNDNumuRHC, *(*spectra)[5], covFileName, covName);
  nd_expt_rhc.SetMaskHist(0.5, 10, 0, -1);

  // What is the chi2 between the data, and the thrown prefit distribution?
  // std::cout << "Prefit chi-square:" << std::endl;
  // if (pot_fd_fhc_nue > 0) std::cout << "\t FD nue FHC = " << app_expt_fhc.ChiSq(fitOsc, fitSyst) << "; POT = " << pot_fd_fhc_nue << std::endl;
  // if (pot_fd_fhc_numu > 0) std::cout << "\t FD numu FHC = " << dis_expt_fhc.ChiSq(fitOsc, fitSyst) << "; POT = " << pot_fd_fhc_numu << std::endl;
  // if (pot_fd_rhc_nue > 0) std::cout << "\t FD nue RHC = " << app_expt_rhc.ChiSq(fitOsc, fitSyst) << "; POT = " << pot_fd_rhc_nue << std::endl;
  // if (pot_fd_rhc_numu > 0) std::cout << "\t FD numu RHC = " << dis_expt_rhc.ChiSq(fitOsc, fitSyst) << "; POT = " << pot_fd_rhc_numu << std::endl;
  // if (pot_nd_fhc > 0) std::cout << "\t ND FHC = " << nd_expt_fhc.ChiSq(fitOsc, fitSyst) << "; POT = " << pot_nd_fhc << std::endl;
  // if (pot_nd_rhc > 0) std::cout << "\t ND RHC = " << nd_expt_rhc.ChiSq(fitOsc, fitSyst) << "; POT = " << pot_nd_rhc << std::endl;

  // Save prefit starting distributions
  if (outDir){
    if (pot_fd_fhc_nue > 0){
      TH1* data_nue_fhc_hist = (*spectra)[0]->ToTHX(pot_fd_fhc_nue);
      data_nue_fhc_hist ->SetName("data_fd_nue_fhc");
      data_nue_fhc_hist ->Write();
      TH1* pre_fd_nue_fhc = GetMCSystTotal(&predFDNueFHC, fitOsc, fitSyst, "prefit_fd_nue_fhc", pot_fd_fhc_nue);
      pre_fd_nue_fhc   ->SetTitle(std::to_string(app_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_fd_nue_fhc   ->Write();
    }
    if (pot_fd_fhc_numu > 0){
      TH1* data_numu_fhc_hist = (*spectra)[1]->ToTHX(pot_fd_fhc_numu);
      data_numu_fhc_hist ->SetName("data_fd_numu_fhc");
      data_numu_fhc_hist ->Write();
      TH1* pre_fd_numu_fhc = GetMCSystTotal(&predFDNumuFHC, fitOsc, fitSyst, "prefit_fd_numu_fhc", pot_fd_fhc_numu);
      pre_fd_numu_fhc   ->SetTitle(std::to_string(dis_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_fd_numu_fhc   ->Write();
    }
    if (pot_fd_rhc_nue > 0){
      TH1* data_nue_rhc_hist = (*spectra)[2]->ToTHX(pot_fd_rhc_nue);
      data_nue_rhc_hist ->SetName("data_fd_nue_rhc");
      data_nue_rhc_hist ->Write();
      TH1* pre_fd_nue_rhc = GetMCSystTotal(&predFDNueRHC, fitOsc, fitSyst, "prefit_fd_nue_rhc", pot_fd_rhc_nue);
      pre_fd_nue_rhc   ->SetTitle(std::to_string(app_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_fd_nue_rhc   ->Write();
    }
    if (pot_fd_rhc_numu > 0){
      TH1* data_numu_rhc_hist = (*spectra)[3]->ToTHX(pot_fd_rhc_numu);
      data_numu_rhc_hist ->SetName("data_fd_numu_rhc");
      data_numu_rhc_hist ->Write();
      TH1* pre_fd_numu_rhc = GetMCSystTotal(&predFDNumuRHC, fitOsc, fitSyst, "prefit_fd_numu_rhc", pot_fd_rhc_numu);
      pre_fd_numu_rhc   ->SetTitle(std::to_string(dis_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_fd_numu_rhc   ->Write();
    }
    if (pot_nd_fhc > 0){
      TH1* nd_data_numu_fhc_hist = (*spectra)[4]->ToTHX(pot_nd_fhc);
      TH1* nd_data_numu_fhc_hist_1D = (*spectra)[4]->ToTH1(pot_nd_fhc);
      nd_data_numu_fhc_hist ->SetName("data_nd_numu_fhc");
      nd_data_numu_fhc_hist_1D ->SetName("data_nd_numu_fhc_1D");
      nd_data_numu_fhc_hist ->Write();
      nd_data_numu_fhc_hist_1D ->Write();

      TH1* pre_nd_numu_fhc = GetMCSystTotal(&predNDNumuFHC, fitOsc, fitSyst, "prefit_nd_numu_fhc", pot_nd_fhc);
      pre_nd_numu_fhc   ->SetTitle(std::to_string(nd_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_nd_numu_fhc   ->Write();
      TH1* pre_nd_numu_fhc_1D = GetMCSystTotal(&predNDNumuFHC, fitOsc, fitSyst, "prefit_nd_numu_fhc_1D", pot_nd_fhc, true);
      pre_nd_numu_fhc_1D   ->SetTitle(std::to_string(nd_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_nd_numu_fhc_1D   ->Write();
    }
    if (pot_nd_rhc){
      TH1* nd_data_numu_rhc_hist = (*spectra)[5]->ToTHX(pot_nd_rhc);
      TH1* nd_data_numu_rhc_hist_1D = (*spectra)[5]->ToTH1(pot_nd_rhc);
      nd_data_numu_rhc_hist ->SetName("data_nd_numu_rhc");
      nd_data_numu_rhc_hist_1D ->SetName("data_nd_numu_rhc_1D");
      nd_data_numu_rhc_hist ->Write();
      nd_data_numu_rhc_hist_1D ->Write();

      TH1* pre_nd_numu_rhc = GetMCSystTotal(&predNDNumuRHC, fitOsc, fitSyst, "prefit_nd_numu_rhc", pot_nd_rhc);
      pre_nd_numu_rhc   ->SetTitle(std::to_string(nd_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_nd_numu_rhc   ->Write();
      TH1* pre_nd_numu_rhc_1D = GetMCSystTotal(&predNDNumuRHC, fitOsc, fitSyst, "prefit_nd_numu_rhc_1D", pot_nd_rhc, true);
      pre_nd_numu_rhc_1D   ->SetTitle(std::to_string(nd_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_nd_numu_rhc_1D   ->Write();
    }
  }

  // Now sort out the experiment
  MultiExperiment this_expt;
  if (pot_nd_fhc > 0) this_expt.Add(&nd_expt_fhc);
  if (pot_nd_rhc > 0) this_expt.Add(&nd_expt_rhc);
  if (pot_fd_fhc_nue > 0)  this_expt.Add(&app_expt_fhc);
  if (pot_fd_fhc_numu > 0) this_expt.Add(&dis_expt_fhc);
  if (pot_fd_rhc_nue > 0)  this_expt.Add(&app_expt_rhc);
  if (pot_fd_rhc_numu > 0) this_expt.Add(&dis_expt_rhc);

  // Add in the penalty...
  if (penaltyTerm) this_expt.Add(penaltyTerm);

  // Now set up the fit itself
  Fitter this_fit(&this_expt, oscVars, systlist, fitStrategy);
  double thischisq = this_fit.Fit(fitOsc, fitSyst, oscSeeds, {}, Fitter::kVerbose);

  bf = fitSyst;

  // std::cout << "Postfit chi-square:" << std::endl;
  // if (pot_fd_fhc_nue > 0) std::cout << "\t FD nue FHC = " << app_expt_fhc.ChiSq(fitOsc, fitSyst) << std::endl;
  // if (pot_fd_fhc_numu > 0) std::cout << "\t FD numu FHC = " << dis_expt_fhc.ChiSq(fitOsc, fitSyst) << std::endl;
  // if (pot_fd_rhc_nue > 0) std::cout << "\t FD nue RHC = " << app_expt_rhc.ChiSq(fitOsc, fitSyst) << std::endl;
  // if (pot_fd_rhc_numu > 0) std::cout << "\t FD numu RHC = " << dis_expt_rhc.ChiSq(fitOsc, fitSyst) << std::endl;
  // if (pot_nd_fhc > 0) std::cout << "\t ND FHC = " << nd_expt_fhc.ChiSq(fitOsc, fitSyst) << std::endl;
  // if (pot_nd_rhc > 0) std::cout << "\t ND RHC = " << nd_expt_rhc.ChiSq(fitOsc, fitSyst) << std::endl;

  // If we have a directory to save to... save some stuff...
  if (outDir){
    std::vector<std::string> fParamNames = this_fit.GetParamNames();
    std::vector<double> fPreFitValues  = this_fit.GetPreFitValues();
    std::vector<double> fPreFitErrors  = this_fit.GetPreFitErrors();
    std::vector<double> fPostFitValues = this_fit.GetPostFitValues();
    std::vector<double> fPostFitErrors = this_fit.GetPostFitErrors();
    std::vector<double> fCentralValues = this_fit.GetCentralValues();
    std::vector<std::pair<double,double>> fMinosErrors   = this_fit.GetMinosErrors();
    double fNFCN = this_fit.GetNFCN();
    double fEDM = this_fit.GetEDM();
    bool fIsValid = this_fit.GetIsValid();

    TMatrixDSym* covar = (TMatrixDSym*)this_fit.GetCovariance();
    TH2D hist_covar = TH2D(*covar);
    hist_covar .SetName("covar");
    TH2D hist_corr = *make_corr_from_covar(&hist_covar);

    if (pot_fd_fhc_nue){
      TH1* post_fd_nue_fhc = GetMCSystTotal(&predFDNueFHC, fitOsc, fitSyst, "postfit_fd_nue_fhc", pot_fd_fhc_nue);
      post_fd_nue_fhc   ->SetTitle(std::to_string(app_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_fd_nue_fhc   ->Write();
    }
    if (pot_fd_fhc_numu){
      TH1* post_fd_numu_fhc = GetMCSystTotal(&predFDNumuFHC, fitOsc, fitSyst, "postfit_fd_numu_fhc", pot_fd_fhc_numu);
      post_fd_numu_fhc   ->SetTitle(std::to_string(dis_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_fd_numu_fhc   ->Write();
    }
    if (pot_fd_rhc_nue){
      TH1* post_fd_nue_rhc = GetMCSystTotal(&predFDNueRHC, fitOsc, fitSyst, "postfit_fd_nue_rhc", pot_fd_rhc_nue);
      post_fd_nue_rhc   ->SetTitle(std::to_string(app_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_fd_nue_rhc   ->Write();
    }
    if (pot_fd_rhc_numu){
      TH1* post_fd_numu_rhc = GetMCSystTotal(&predFDNumuRHC, fitOsc, fitSyst, "postfit_fd_numu_rhc", pot_fd_rhc_numu);
      post_fd_numu_rhc   ->SetTitle(std::to_string(dis_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_fd_numu_rhc   ->Write();
    }
    if (pot_nd_fhc){
      TH1* post_nd_numu_fhc = GetMCSystTotal(&predNDNumuFHC, fitOsc, fitSyst, "postfit_nd_numu_fhc", pot_nd_fhc);
      post_nd_numu_fhc   ->SetTitle(std::to_string(nd_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_nd_numu_fhc   ->Write();
      TH1* post_nd_numu_fhc_1D = GetMCSystTotal(&predNDNumuFHC, fitOsc, fitSyst, "postfit_nd_numu_fhc_1D", pot_nd_fhc, true);
      post_nd_numu_fhc_1D   ->SetTitle(std::to_string(nd_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_nd_numu_fhc_1D   ->Write();
    }
    if (pot_nd_rhc){
      TH1* post_nd_numu_rhc = GetMCSystTotal(&predNDNumuRHC, fitOsc, fitSyst, "postfit_nd_numu_rhc", pot_nd_rhc);
      post_nd_numu_rhc   ->SetTitle(std::to_string(nd_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_nd_numu_rhc   ->Write();
      TH1* post_nd_numu_rhc_1D = GetMCSystTotal(&predNDNumuRHC, fitOsc, fitSyst, "postfit_nd_numu_rhc_1D", pot_nd_rhc, true);
      post_nd_numu_rhc_1D   ->SetTitle(std::to_string(nd_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_nd_numu_rhc_1D   ->Write();
    }

    // Save information
    TTree *t = new TTree("fit_info", "fit_info");
    t->Branch("chisq", &thischisq);
    t->Branch("NFCN", &fNFCN);
    t->Branch("EDM", &fEDM);
    t->Branch("IsValid", &fIsValid);
    t->Branch("fParamNames",&fParamNames);
    t->Branch("fFakeDataVals",&fFakeDataVals);
    t->Branch("fPreFitValues",&fPreFitValues);
    t->Branch("fPreFitErrors",&fPreFitErrors);
    t->Branch("fPostFitValues",&fPostFitValues);
    t->Branch("fPostFitErrors",&fPostFitErrors);
    t->Branch("fMinosErrors",&fMinosErrors);
    t->Branch("fCentralValues",&fCentralValues);
    t->Fill();
    t->Write();
    hist_covar.Write();
    hist_corr.Write();
    delete t;
  }

  if(PostFitTreeBlob){
    PostFitTreeBlob->fParamNames = this_fit.GetParamNames();
    PostFitTreeBlob->fPreFitValues  = this_fit.GetPreFitValues();
    PostFitTreeBlob->fPreFitErrors  = this_fit.GetPreFitErrors();
    PostFitTreeBlob->fPostFitValues = this_fit.GetPostFitValues();
    PostFitTreeBlob->fPostFitErrors = this_fit.GetPostFitErrors();
    PostFitTreeBlob->fMinosErrors   = this_fit.GetMinosErrors();
    PostFitTreeBlob->fCentralValues = this_fit.GetCentralValues();
    PostFitTreeBlob->fFakeDataVals = fFakeDataVals;
    PostFitTreeBlob->fNFCN = this_fit.GetNFCN();
    PostFitTreeBlob->fEDM = this_fit.GetEDM();
    PostFitTreeBlob->fIsValid = this_fit.GetIsValid();
    PostFitTreeBlob->fChiSq = thischisq;
  }

  return thischisq;
};
