#include "CAFAna/Analysis/common_fit_definitions.h"
#include "CAFAna/Analysis/AnalysisVersion.h"

#include "CAFAna/Analysis/AnalysisBinnings.h"
#include "CAFAna/Analysis/AnalysisVars.h"
#include "CAFAna/Analysis/Calcs.h"
#include "CAFAna/Analysis/CalcsNuFit.h"
#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Analysis/Plots.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Var.h"

#include "CAFAna/Cuts/AnaCuts.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/Experiment/MultiExperiment.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"

#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Prediction/PredictionNoOsc.h"

#include "CAFAna/Vars/FitVars.h"

#include "CAFAna/Systs/DUNEFluxSysts.h"
#include "CAFAna/Systs/EnergySysts.h"
#include "CAFAna/Systs/FDRecoSysts.h"
#include "CAFAna/Systs/NDRecoSysts.h"
#include "CAFAna/Systs/NuOnESysts.h"
#include "CAFAna/Systs/Systs.h"
#include "CAFAna/Systs/XSecSysts.h"
#include "CAFAna/Systs/CrazyFluxFakeData.h"

#include "OscLib/func/IOscCalculator.h"
#include "OscLib/func/OscCalculatorPMNSOpt.h"
#include "OscLib/func/OscCalculatorGeneral.h"

#include "StandardRecord/StandardRecord.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TSystem.h"
#include "TTree.h"

#ifdef USE_PREDINTERP_OMP
#include <omp.h>
#endif

#include <chrono>
#include <tuple>

using namespace ana;

unsigned gRNGSeed = 0;

// POT for 3.5 years
const double pot_fd_FVMassFactor = (40 / 1.13);
const double pot_fd = 3.5 * POT120 * pot_fd_FVMassFactor;
const double pot_nd = 3.5 * POT120;
// This is pretty annoying, but the above is for 7 years staged, which is 336 kT
// MW yr
const double nom_exposure = 336.;

size_t NFluxParametersToAddToStatefile = 30;

double GetBoundedGausThrow(double min, double max) {
  double val = -999;
  while (val > max || val < min)
    val = gRandom->Gaus();
  return val;
}

// I miss python...
std::vector<std::string> SplitString(std::string input, char delim) {
  std::vector<std::string> output;
  std::stringstream ss(input);
  std::string token;
  while (std::getline(ss, token, delim))
    output.push_back(token);
  return output;
}

// For ease of penalty terms...
IChiSqExperiment *GetPenalty(int hie, int oct, std::string penalty,
                        std::string asimov_set, bool modConstraint) {

  // First, decide which to use
  std::vector<std::string> penalties = SplitString(penalty, ':');
  bool useTh13 = false;
  bool useDmsq32 = false;
  bool useTh23 = false;

  for (auto &str : penalties) {
    if (str == "th13" || str == "allpen")
      useTh13 = true;
    if (str == "dmsq32" || str == "allpen")
      useDmsq32 = true;
    if (str == "th23" || str == "allpen")
      useTh23 = true;
  }

  Penalizer_GlbLike *ret =
      new Penalizer_GlbLike(hie, oct, useTh13, useDmsq32, useTh23, 0);
  ret->SetAsimovPoint(asimov_set, modConstraint);
  return ret;
}

std::vector<const IFitVar *> GetOscVars(std::string oscVarString, int hie,
                                        int oct) {

  std::vector<std::string> osc_vars = SplitString(oscVarString, ':');

  std::vector<const IFitVar *> rtn_vars;

  for (auto &v : osc_vars) {
    if (v == "th13" || v == "alloscvars") {
      rtn_vars.push_back(&kFitTheta13);
    }
    // Deal with bounded dmsq32
    if (v == "dmsq32" || v == "alloscvars") {
      if (hie == -1)
        rtn_vars.push_back(&kFitDmSq32IHScaled);
      else if (hie == 1)
        rtn_vars.push_back(&kFitDmSq32NHScaled);
      else
        rtn_vars.push_back(&kFitDmSq32Scaled);
    }

    // Deal with octant boundaries
    if (v == "th23" || v == "alloscvars") {
      if (oct == -1)
        rtn_vars.push_back(&kFitSinSqTheta23LowerOctant);
      else if (oct == 1)
        rtn_vars.push_back(&kFitSinSqTheta23UpperOctant);
      else
        rtn_vars.push_back(&kFitSinSqTheta23);
    }
    if (v == "deltapi" || v == "alloscvars") {
      rtn_vars.push_back(&kFitDeltaInPiUnits);
    }

    // Add back in the 21 parameters
    if (v == "dmsq21" || v == "alloscvars") {
      rtn_vars.push_back(&kFitDmSq21Scaled);
    }
    if (v == "th12" || v == "alloscvars") {
      rtn_vars.push_back(&kFitSinSq2Theta12);
    }
    // Rho rho rho your boat...
    if (v == "rho" || v == "alloscvars") {
      rtn_vars.push_back(&kFitRho);
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

void KeepSysts(std::vector<const ISyst *> &systlist,
               std::vector<const ISyst *> const &systsToInclude) {
  systlist.erase(std::remove_if(systlist.begin(), systlist.end(),
                                [&](const ISyst *s) {
                                  return (std::find(systsToInclude.begin(),
                                                    systsToInclude.end(),
                                                    s) == systsToInclude.end());
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

std::vector<const ISyst *> GetListOfSysts(bool fluxsyst_Nov17, bool xsecsyst,
                                          bool detsyst, bool useND, bool useFD,
                                          bool useNueOnE, bool useFakeDataDials,
                                          bool fluxsyst_CDR, int NFluxSysts,
                                          bool removeFDNonFitDials) {

  // This doesn't need to be an argument because I basically never change it:
  bool fluxXsecPenalties = true;

  std::vector<const ISyst *> systlist;
  if (fluxsyst_Nov17) {
    std::vector<const ISyst *> fluxlist_Nov17 =
        GetDUNEFluxSysts(NFluxSysts, fluxXsecPenalties, false);
    systlist.insert(systlist.end(), fluxlist_Nov17.begin(),
                    fluxlist_Nov17.end());
  }

  if (fluxsyst_CDR) {
    std::vector<const ISyst *> fluxlist_CDR =
        GetDUNEFluxSysts(NFluxSysts, fluxXsecPenalties, true);
    systlist.insert(systlist.end(), fluxlist_CDR.begin(), fluxlist_CDR.end());
  }

  if (detsyst) {
    std::vector<const ISyst *> elist = GetEnergySysts();
    std::vector<const ISyst *> fdlist = GetFDRecoSysts();
    std::vector<const ISyst *> ndlist = GetNDRecoSysts();
    std::vector<const ISyst *> nuelist = GetNuOnESysts();

    systlist.insert(systlist.end(), elist.begin(), elist.end());
    if (useFD) {
      systlist.insert(systlist.end(), fdlist.begin(), fdlist.end());
    }
    if (useND) {
      systlist.insert(systlist.end(), ndlist.begin(), ndlist.end());
    }
    if (useND && useNueOnE) {
      systlist.insert(systlist.end(), nuelist.begin(), nuelist.end());
    }
  }

  if (xsecsyst) {
    std::vector<const ISyst *> xseclist =
        GetXSecSysts(GetAllXSecSystNames(), fluxXsecPenalties);
    systlist.insert(systlist.end(), xseclist.begin(), xseclist.end());

    // Always remove these dials
    RemoveSysts(systlist, GetDoNotIncludeSystNames());
  }

  // If using fake data dials (for state generation) add them back in
  if (useFakeDataDials) {
    std::vector<const ISyst *> xseclist =
        GetXSecSysts(GetAllXSecSystNames(), fluxXsecPenalties);
    KeepSysts(xseclist, GetFakeDataGenerationSystNames());
    systlist.insert(systlist.end(), xseclist.begin(), xseclist.end());

    std::vector<const ISyst *> crazyfluxlist =
      GetCrazyFluxSysts();
    systlist.insert(systlist.end(), crazyfluxlist.begin(), crazyfluxlist.end());
  }

  return systlist;
}

std::vector<const ISyst *> GetListOfFakeDataSysts() {
  return GetListOfSysts(false, false, false, false, false, false,
                        true /*add fake data*/, false);
}

std::vector<const ISyst *> GetListOfSysts(std::string systString, bool useND,
                                          bool useFD, bool useNueOnE) {

  if (systString == "fakedata") {
    return GetListOfFakeDataSysts();
  }

  // Now defaults to true!
  bool detsyst = true;
  bool fluxsyst_Nov17 = (GetAnaVersion() == kV3) ? false : true;
  bool fluxsyst_CDR = (GetAnaVersion() == kV3) ? true : false;
  bool xsecsyst = true;
  bool useFakeData = false;
  int NFluxSysts =
      (GetAnaVersion() == kV3) ? 10 : NFluxParametersToAddToStatefile;

  // If you find an argument in the form list:name1:name2:name3 etc etc, keep
  // only those systematics This is pretty much a magic option to allow single
  // parameters... there must be a better way, but for now I'm just going to
  // continue to support it
  if (systString.find("list") != std::string::npos) {

    // 1) Get a default list with everything
    std::vector<const ISyst *> namedList =
        GetListOfSysts(true, true, true, useND, useFD, useNueOnE,
                       false /*no fake data*/, true /*Get CDR flux systs*/);
    // for (auto & syst : namedList) std::cout << syst->ShortName() <<
    // std::endl; 2) Interpret the list of short names
    std::vector<std::string> systs = SplitString(systString, ':');

    // 3) Don't include "list"
    systs.erase(systs.begin());

    // 4) Regret nothing
    KeepSysts(namedList, systs);

    // 5) $$$ Profit
    return namedList;
  }

  // Can't transform anymore, so... BEHAVE YOURSELF
  // std::transform(systString.begin(), systString.end(), systString.begin(),
  // ::tolower);

  // Do even more horrific things...
  std::vector<std::string> systs = SplitString(systString, ':');

  // Start off by checking for certain keywords
  for (auto syst : systs) {
    if (syst == "allsyst") {
      xsecsyst = true;
      fluxsyst_Nov17 = (GetAnaVersion() == kV3) ? false : true;
      fluxsyst_CDR = (GetAnaVersion() == kV3) ? true : false;
      detsyst = true;
    }

    if (syst == "nosyst") {
      xsecsyst = false;
      fluxsyst_Nov17 = false;
      fluxsyst_CDR = false;
      detsyst = false;
    }

    // Now we're getting a bit funky as these options now conflict.
    // But, if you do something stupid, YOU ONLY HAVE YOURSELF TO BLAME
    if (syst == "nodet") {
      detsyst = false;
    }
    if (syst == "noflux") {
      fluxsyst_CDR = false;
      fluxsyst_Nov17 = false;
    }
    if (syst == "cdrflux") {
      fluxsyst_CDR = true;
      fluxsyst_Nov17 = false;
    }
    if (syst == "nov17flux") {
      fluxsyst_CDR = false;
      fluxsyst_Nov17 = true;
    }
    if (syst == "fakedata") {
      useFakeData = true;
    } // LOOK MA, I GOT BRACES!
    if (syst == "nofakedata") {
      useFakeData = false;
    }

    if (syst.find("nflux=") == 0) {
      auto NFluxSplit = SplitString(syst, '=');
      if (NFluxSplit.size() != 2) {
        std::cout << "[ERROR]: Found NFlux option, but couldn't parse how many "
                     "to include, expected to find nflux=<0--"
                  << NFluxParametersToAddToStatefile << "> but found: \""
                  << syst << "\".";
        abort();
      }

      NFluxSysts = atoi(NFluxSplit[1].c_str());
    }
    if (syst == "noxsec") {
      xsecsyst = false;
    }
  }

  // Okay, now get the list, and start from there...
  std::vector<const ISyst *> namedList =
      GetListOfSysts(fluxsyst_Nov17, xsecsyst, detsyst, useND, useFD, useNueOnE,
                     useFakeData, fluxsyst_CDR, NFluxSysts);

  // Now do something REALLY FUNKY. Remove specific dials from the list we
  // already have Need to allow single dials, and a few specific groups...
  for (auto syst : systs) {
    // ignore anything we previously dealt with
    if ((syst == "noxsec") || (syst == "nodet") || (syst == "noflux") ||
        (syst.find("nflux=") == 0)) {
      continue;
    }
    // Now remove some specific groups
    // nofd_det, nofd_escale, nofd_muon_escale, noxsec_qe, noxsec_res,
    // noxsec_dis, noxsec_fsi, noxsec_ratios
    else if (syst == "nofd_det") {
      RemoveSysts(namedList, fd_det_list);
    } else if (syst == "nofd_escale") {
      RemoveSysts(namedList, fd_escale_list);
    } else if (syst == "nofd_muon_escale") {
      RemoveSysts(namedList, fd_muon_escale_list);
    } else if (syst == "nofd_eres") {
      RemoveSysts(namedList, fd_eres_list);
    } else if (syst == "nofd_muon_eres") {
      RemoveSysts(namedList, fd_muon_eres_list);
    } else if (syst == "nofd_other_det") {
      RemoveSysts(namedList, fd_other_det_list);
    } else if (syst == "nofd_nonlin_det") {
      RemoveSysts(namedList, fd_nonlin_det_list);
    } else if (syst == "noxsec_qe") {
      RemoveSysts(namedList, GetXSecSystNames("QELike"));
    } else if (syst == "noxsec_res") {
      RemoveSysts(namedList, GetXSecSystNames("RES"));
    } else if (syst == "noxsec_dis") {
      RemoveSysts(namedList, GetXSecSystNames("DIS"));
    } else if (syst == "noxsec_fsi") {
      RemoveSysts(namedList, GetXSecSystNames("FSI"));
    } else if (syst == "noxsec_ratios") {
      RemoveSysts(namedList, GetXSecSystNames("Ratios"));
    }
    // If not, remove as if it's a single parameter instruction
    else {
      RemoveSysts(namedList, {syst.erase(0, 2)});
    }
  }

  if (GetAnaVersion() == kV3) {
    RemoveSysts(namedList,
                {"UncorrFDTotSqrt", "UncorrFDTotInvSqrt", "UncorrFDHadSqrt",
                 "UncorrFDHadInvSqrt", "UncorrFDMuSqrt", "UncorrFDMuInvSqrt",
                 "UncorrFDNSqrt", "UncorrFDNInvSqrt", "UncorrFDEMSqrt",
                 "UncorrFDEMInvSqrt", "ChargedHadUncorrFD"});
  }

  // Now return the list
  return namedList;
}

std::vector<const ISyst *> GetListOfSysts(char const *systCString, bool useND,
                                          bool useFD, bool useNueOnE) {
  return GetListOfSysts(std::string(systCString), useND, useFD, useNueOnE);
}

std::vector<const ISyst *>
OrderListOfSysts(std::vector<const ISyst *> const &systlist) {
  std::vector<const ISyst *> superorder;
  for (auto &s : GetListOfSysts("flux:noxsec:nodet")) {
    superorder.emplace_back(s);
  }
  for (auto &s : GetListOfSysts("cdrflux:noxsec:nodet")) {
    superorder.emplace_back(s);
  }
  for (auto &s : GetListOfSysts("noflux:xsec:nodet")) {
    superorder.emplace_back(s);
  }
  for (auto &s : GetListOfSysts("noflux:noxsec:det")) {
    superorder.emplace_back(s);
  }
  for (auto &s : GetListOfSysts("fakedata")) {
    superorder.emplace_back(s);
  }
  std::vector<const ISyst *> retlist;

  for (auto s : superorder) {
    if (std::find(systlist.begin(), systlist.end(), s) == systlist.end()) {
      continue;
    }
    retlist.emplace_back(s);
  }
  return retlist;
}

/*
  The behaviour of fake data dials is a bit clunky. Instead of the multiple spline
  points used for normal dials, they have fixed values for non-zero values of the
  dial. So, for some, +/-3, +/-2, +/-1 values of the dial will all give the same
  value. Some are set so all negative (positive) values give a -3 (+3) sigma
  variation, which is why there's some messing around in the function below.
  The spline behaviour is forced (for XSec dials) in XSecSyst::FakeDataDialShift,
  which is called only when creating the input CAFs. This is almost certainly not
  optimal, but is easy to extend for other dials.
 */
SystShifts GetFakeDataGeneratorSystShift(std::string input) {

  // Default to nominal
  SystShifts thisShift = kNoShift;

  if (input.empty() || input == "nominal")
    return thisShift;

  // Allow _pos and _neg endings
  std::vector<std::string> split_input = SplitString(input, ':');
  std::vector<std::string> fake_data_names;
  std::vector<int> dial_vals;

  for (auto in_name : split_input){
    std::string name = in_name;
    int val = 1;

    if (in_name.compare(in_name.length()-4, 4, "_pos") == 0){
      name = in_name.substr(0, in_name.length()-4);
      val  = 1;
    } else if (in_name.compare(in_name.length()-4, 4, "_neg") == 0){
      name = in_name.substr(0, in_name.length()-4);
      val  = -1;
    }

    // Check nobody did anything dumb...
    assert(IsFakeDataGenerationSyst(name) || IsCrazyFluxSyst(name));
    fake_data_names.push_back(name);
    dial_vals.push_back(val);
  }

  std::vector<ISyst const *> FDSyst = GetListOfSysts();
  KeepSysts(FDSyst, fake_data_names);

  for (uint i=0; i<FDSyst.size(); i++){
    thisShift.SetShift(FDSyst[i], dial_vals[i]);
  }

  return thisShift;
}

std::string GetSampleName(SampleType sample) {
  switch (sample) {
  case kFDFHC:
    return "FD_FHC";
  case kFDRHC:
    return "FD_RHC";
  case kNDFHC:
    return "ND_FHC";
  case kNDRHC:
    return "ND_RHC";
  case kNDNue:
    return "ND_nue";
  case kNDFHC_OA /*how you like this space, Callum?*/:
    return "ND_FHC_OA";
  case /* LOVE IT M8                */ kUnknown:
  default:
    return "NONE";
  }
}

SampleType GetSampleType(std::string const &sample) {

  if (sample == "FD_FHC")
    return kFDFHC;
  if (sample == "FD_RHC")
    return kFDRHC;
  if (sample == "ND_FHC")
    return kNDFHC;
  if (sample == "ND_RHC")
    return kNDRHC;
  if (sample == "ND_nue")
    return kNDNue;
  if (sample == "ND_FHC_OA")
    return kNDFHC_OA;
  return kUnknown;
}

void MakePredictionInterp(TDirectory *saveDir, SampleType sample,
                          std::vector<const ISyst *> systlist,
                          AxisBlob const &axes,
                          std::vector<std::string> const &non_swap_file_list,
                          std::vector<std::string> const &nue_swap_file_list,
                          std::vector<std::string> const &tau_swap_file_list,
                          int max) {

  for (auto &v : axes.NDAx->GetVars()) {
    assert(v.IsValid());
  }
  for (auto &v : axes.FDAx_numu->GetVars()) {
    assert(v.IsValid());
  }
  for (auto &v : axes.FDAx_nue->GetVars()) {
    assert(v.IsValid());
  }

  bool use_cv_weights = true;
  if (getenv("CAFANA_IGNORE_CV_WEIGHT")) {
    use_cv_weights = !atoi(getenv("CAFANA_IGNORE_CV_WEIGHT"));
  }

  bool use_selection = true;
  if (getenv("CAFANA_IGNORE_SELECTION")) {
    use_selection = !atoi(getenv("CAFANA_IGNORE_SELECTION"));
  }

  // Move to the save directory
  saveDir->cd();
  osc::IOscCalculatorAdjustable *this_calc = NuFitOscCalc(1);

  bool isfhc =
      ((sample == kNDFHC) || (sample == kNDFHC_OA) || (sample == kFDFHC));

  // FD samples
  if ((sample == kFDFHC) || (sample == kFDRHC)) {

    Loaders these_loaders;
    SpectrumLoader loaderNumu(non_swap_file_list, kBeam, max);
    SpectrumLoader loaderNue(nue_swap_file_list, kBeam, max);
    SpectrumLoader loaderNutau(tau_swap_file_list, kBeam, max);

    these_loaders.AddLoader(&loaderNumu, caf::kFARDET, Loaders::kMC, ana::kBeam,
                            Loaders::kNonSwap);
    these_loaders.AddLoader(&loaderNue, caf::kFARDET, Loaders::kMC, ana::kBeam,
                            Loaders::kNueSwap);
    these_loaders.AddLoader(&loaderNutau, caf::kFARDET, Loaders::kMC,
                            ana::kBeam, Loaders::kNuTauSwap);

    NoExtrapPredictionGenerator genFDNumu(
        *axes.FDAx_numu,
        use_selection ? kPassFD_CVN_NUMU && kIsTrueFV : kIsTrueFV,
        use_cv_weights ? kCVXSecWeights : kUnweighted);
    NoExtrapPredictionGenerator genFDNue(
        *axes.FDAx_nue,
        use_selection ? kPassFD_CVN_NUE && kIsTrueFV : kIsTrueFV,
        use_cv_weights ? kCVXSecWeights : kUnweighted);
    PredictionInterp predInterpFDNumu(systlist, this_calc, genFDNumu,
                                      these_loaders);
    PredictionInterp predInterpFDNue(systlist, this_calc, genFDNue,
                                     these_loaders);
    these_loaders.Go();

    std::cout << "Saving " << GetSampleName(sample) << std::endl;
    predInterpFDNumu.SaveTo(saveDir->mkdir(
        (std::string("fd_interp_numu_") + std::string(isfhc ? "fhc" : "rhc"))
            .c_str()));
    std::cout << "Saving " << GetSampleName(sample) << std::endl;
    predInterpFDNue.SaveTo(saveDir->mkdir(
        (std::string("fd_interp_nue_") + std::string(isfhc ? "fhc" : "rhc"))
            .c_str()));

  } else if ((sample == kNDFHC) || (sample == kNDRHC) ||
             (sample == kNDFHC_OA)) {

    // Now ND
    Loaders these_loaders;
    SpectrumLoader loaderNumu(non_swap_file_list, kBeam, max);
    these_loaders.AddLoader(&loaderNumu, caf::kNEARDET, Loaders::kMC);

    NoOscPredictionGenerator genNDNumu(
        *axes.NDAx,
        use_selection
            ? (isfhc ? kPassND_FHC_NUMU : kPassND_RHC_NUMU) && kIsTrueFV
            : kIsTrueFV,
        use_cv_weights ? kCVXSecWeights : kUnweighted);

    PredictionInterp predInterpNDNumu(systlist, this_calc, genNDNumu,
                                      these_loaders);
    these_loaders.Go();

    std::cout << "Saving " << GetSampleName(sample) << std::endl;
    predInterpNDNumu.SaveTo(saveDir->mkdir(
        (std::string("nd_interp_numu_") + std::string(isfhc ? "fhc" : "rhc"))
            .c_str()));
  }
}

std::vector<std::string> const sample_dir_order = {
    "fd_interp_numu_fhc", "fd_interp_nue_fhc",  "fd_interp_numu_rhc",
    "fd_interp_nue_rhc",  "nd_interp_numu_fhc", "nd_interp_numu_rhc"};
std::vector<std::string> const sample_suffix_order = {
    "FD_FHC", "FD_FHC", "FD_RHC", "FD_RHC", "ND_FHC", "ND_RHC"};

std::vector<std::unique_ptr<ana::PredictionInterp>>
GetPredictionInterps(std::string fileName,
                     std::vector<const ISyst *> systlist) {

  // Make sure the syst registry has been populated with all the systs we could
  // want to use
  static std::vector<const ISyst *> systs = GetListOfSysts();

  // Hackity hackity hack hack
  bool fileNameIsStub = (fileName.find(".root") == std::string::npos);

  std::vector<std::unique_ptr<PredictionInterp>> return_list;

  for (size_t s_it = 0; s_it < sample_dir_order.size(); ++s_it) {
    std::string state_fname =
        fileNameIsStub ? fileName + "_" + sample_suffix_order[s_it] + ".root"
                       : fileName;
    TFile *fin =
        TFile::Open(state_fname.c_str(), "READ"); // Allows xrootd streaming
    assert(fin && !fin->IsZombie());
    std::cout << "[LOAD]: Retrieving " << sample_dir_order[s_it] << " from "
              << state_fname << ":" << sample_dir_order[s_it] << std::endl;
    return_list.emplace_back(LoadFrom<PredictionInterp>(
        fin->GetDirectory(sample_dir_order[s_it].c_str())));
    delete fin;

    std::vector<ana::ISyst const *> systs_to_remove =
        return_list.back()->GetAllSysts();
    std::vector<std::string> used_syst_names;
    for (auto s : systlist) {
      used_syst_names.push_back(s->ShortName());
    }
    RemoveSysts(systs_to_remove, used_syst_names);
    if (systs_to_remove.size()) {
      return_list.back()->DiscardSysts(systs_to_remove);
    }
  }
  return return_list;
}

TH2D *make_corr_from_covar(TH2D *covar) {

  TH2D *corr = (TH2D *)covar->Clone();
  corr->SetNameTitle("corr", "corr");

  for (int i = 0; i < covar->GetNbinsX(); ++i) {
    double istddev = sqrt(covar->GetBinContent(i + 1, i + 1));
    for (int j = 0; j < covar->GetNbinsX(); ++j) {
      double jstddev = sqrt(covar->GetBinContent(j + 1, j + 1));
      double new_corr = covar->GetBinContent(i + 1, j + 1) / istddev / jstddev;
      corr->SetBinContent(i + 1, j + 1, new_corr);
    }
  }
  return corr;
}

// Yet another string parser that does far too much. I can't be stopped!
void ParseDataSamples(std::string cmdLineInput, double &pot_nd_fhc,
                      double &pot_nd_rhc, double &pot_fd_fhc_nue,
                      double &pot_fd_rhc_nue, double &pot_fd_fhc_numu,
                      double &pot_fd_rhc_numu) {

  // Did somebody say overextend the command line arguments even further?
  // Well okay!
  std::vector<std::string> input_vect = SplitString(cmdLineInput, ':');

  // Default to 7 years staged. Value is actually in kt MW yr
  double exposure = nom_exposure;
  if (input_vect.size() > 1)
    exposure = stod(input_vect[1]);
  std::string input = input_vect[0];

  // LoWeR cAsE sO i CaN bE sIlLy WiTh InPuTs
  std::transform(input.begin(), input.end(), input.begin(), ::tolower);

  // Look for some other magic information
  for (auto str : input_vect) {
    if (str.find("full") != std::string::npos or
        str.find("15year") != std::string::npos)
      exposure = 1104;

    if (str.find("nom") != std::string::npos or
        str.find("7year") != std::string::npos)
      exposure = 336;

    if (str.find("10year") != std::string::npos)
      exposure = 624;
  }

  double exposure_ratio = exposure / nom_exposure;

  // Now sort out which samples to include
  pot_nd_fhc = pot_nd_rhc = pot_fd_fhc_nue = pot_fd_rhc_nue = pot_fd_fhc_numu =
      pot_fd_rhc_numu = 0;

  // Hacky McHackerson is here to stay!
  if (input.find("nd") != std::string::npos) {
    pot_nd_fhc = pot_nd_rhc = pot_nd * exposure_ratio;
  }
  if (input.find("fd") != std::string::npos) {
    pot_fd_fhc_nue = pot_fd_rhc_nue = pot_fd_fhc_numu = pot_fd_rhc_numu =
        pot_fd * exposure_ratio;
  }

  // Now allow specific subsets
  if (input.find("fhc") != std::string::npos) {
    pot_nd_rhc = pot_fd_rhc_nue = pot_fd_rhc_numu = 0;
  }

  if (input.find("rhc") != std::string::npos) {
    pot_nd_fhc = pot_fd_fhc_nue = pot_fd_fhc_numu = 0;
  }

  if (input.find("numu") != std::string::npos) {
    pot_fd_fhc_nue = pot_fd_rhc_nue = 0;
  }

  if (input.find("nue") != std::string::npos) {
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
    if (str == "fakenuis" || str == "all")
      fakeNuis = true;
    if (str == "start" || str == "all")
      start = true;
    if (str == "central" || str == "all")
      central = true;
  }
  return;
}

TMatrixD *MakeCovmat(PredictionInterp const &prediction,
                     std::vector<ISyst const *> const &systs,
                     osc::IOscCalculatorAdjustable *calc, size_t NToys,
                     TDirectory *outdir) {
  std::vector<std::vector<double>> ThrownSpectra;
  std::vector<double> MeanSpectra;

  SystShifts shift;

  if (outdir) {
    outdir->cd();
    std::unique_ptr<TH1> nominal_spectra(
        prediction.PredictSyst(calc, kNoShift).ToTH1(1));
    nominal_spectra->Write("nominal_throw_spectra");
    nominal_spectra->SetDirectory(nullptr);
  }

  size_t NBins = std::numeric_limits<size_t>::max();
  for (size_t t_it = 0; t_it < NToys; ++t_it) { // throws

    shift.ResetToNominal();
    // Throw new param values
    for (auto s : systs) {
      double v = GetBoundedGausThrow(s->Min(), s->Max());
      shift.SetShift(s, v);
    }

    // Make prediction TH1
    std::unique_ptr<TH1> thrown_spectra(
        prediction.PredictSyst(calc, shift).ToTH1(1));
    if (outdir) {
      thrown_spectra->Write(
          (std::string("thrown_spectra_") + std::to_string(t_it)).c_str());
    }
    thrown_spectra->SetDirectory(nullptr);

    if (NBins == std::numeric_limits<size_t>::max()) {
      NBins = thrown_spectra->GetXaxis()->GetNbins();

      ThrownSpectra = std::vector<std::vector<double>>(
          NToys, std::vector<double>(NBins, 0));
      MeanSpectra = std::vector<double>(NBins, 0);
    }

    for (size_t bi_it = 0; bi_it < NBins; ++bi_it) { // spectra bins
      double bin_throw = thrown_spectra->GetBinContent(bi_it + 1);
      MeanSpectra[bi_it] += bin_throw;
      ThrownSpectra[t_it][bi_it] = bin_throw;
    }
  }

  for (double &bc : MeanSpectra) {
    bc /= double(NToys);
  }

  // Build covmat
  TMatrixD *mat = new TMatrixD(NBins, NBins);

  for (size_t rbi_it = 0; rbi_it < NBins; ++rbi_it) {
    for (size_t cbi_it = 0; cbi_it < NBins; ++cbi_it) {
      mat->operator()(rbi_it, cbi_it) = 0;
    }
  }
  double nthrow_fact = 1.0 / double(NToys - 1);

  for (size_t t_it = 0; t_it < NToys; ++t_it) { // throws
    for (size_t rbi_it = 0; rbi_it < NBins; ++rbi_it) {
      for (size_t cbi_it = 0; cbi_it < NBins; ++cbi_it) {
        double rdiff = (ThrownSpectra[t_it][rbi_it] / MeanSpectra[rbi_it]) - 1;
        double cdiff = (ThrownSpectra[t_it][cbi_it] / MeanSpectra[cbi_it]) - 1;
        mat->operator()(rbi_it, cbi_it) += (rdiff * cdiff * nthrow_fact);
      }
    }
  }
  return mat;
}

void SaveTrueOAParams(TDirectory *outDir, osc::IOscCalculatorAdjustable *calc,
                      std::string tree_name) {

  outDir->cd();
  double L = calc->GetL();
  double rho = calc->GetRho();
  double dmsq21 = calc->GetDmsq21();
  double dmsq32 = calc->GetDmsq32();
  double th12 = calc->GetTh12();
  double th13 = calc->GetTh13();
  double th23 = calc->GetTh23();
  double ssth23 = sin(calc->GetTh23()) * sin(calc->GetTh23());
  double deltapi = calc->GetdCP() / TMath::Pi();

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

void SaveParams(TDirectory *outDir, std::vector<const ISyst *> systlist) {
  outDir->cd();
  std::vector<std::string> *fParamNames = new std::vector<std::string>();
  for (auto it : systlist)
    fParamNames->push_back(it->ShortName());
  TTree *t = new TTree("meta", "Parameter meta-data");
  t->Branch("fParamNames", &fParamNames);
  t->Fill();
  t->Write();
  delete fParamNames;
  delete t;
}

FitTreeBlob::FitTreeBlob(std::string tree_name, std::string meta_tree_name)
    : fMeta_filled(false), throw_tree(nullptr), meta_tree(nullptr), fNFills(0) {

  fFakeDataVals = new std::vector<double>();
  fParamNames = new std::vector<std::string>();
  fPreFitValues = new std::vector<double>();
  fPreFitErrors = new std::vector<double>();
  fPostFitValues = new std::vector<double>();
  fPostFitErrors = new std::vector<double>();
  fCentralValues = new std::vector<double>();
  fEnvVarNames = new std::vector<std::string>();
  fEnvVarValues = new std::vector<std::string>();
  fSpectraRNGSeeds = new std::vector<unsigned>();

  TDirectory *odir = gDirectory;

  if (tree_name.size()) {

    throw_tree = new TTree(tree_name.c_str(), "Fit information");
    throw_tree->SetAutoSave(10);
    throw_tree->Branch("chisq", &fChiSq);
    throw_tree->Branch("NSeconds", &fNSeconds);
    throw_tree->Branch("ResMemUsage", &fResMemUsage);
    throw_tree->Branch("VirtMemUsage", &fVirtMemUsage);
    throw_tree->Branch("NFCN", &fNFCN);
    throw_tree->Branch("EDM", &fEDM);
    throw_tree->Branch("IsValid", &fIsValid);
    throw_tree->Branch("fFakeDataVals", &fFakeDataVals);
    throw_tree->Branch("fPreFitValues", &fPreFitValues);
    throw_tree->Branch("fPreFitErrors", &fPreFitErrors);
    throw_tree->Branch("fPostFitValues", &fPostFitValues);
    throw_tree->Branch("fPostFitErrors", &fPostFitErrors);
    throw_tree->Branch("fCentralValues", &fCentralValues);
    throw_tree->Branch("LoopRNGSeed", &fLoopRNGSeed);
    throw_tree->Branch("JobRNGSeed", &fJobRNGSeed);
    throw_tree->Branch("SpectraRNGSeeds", &fSpectraRNGSeeds);
    throw_tree->Branch("ProcFitN", &fNFills);
    throw_tree->Branch("NOscSeeds", &fNOscSeeds);

    if (meta_tree_name.size()) {

      meta_tree = new TTree(meta_tree_name.c_str(), "Parameter meta-data");
      meta_tree->SetAutoSave(10);
      meta_tree->Branch("fParamNames", &fParamNames);
      meta_tree->Branch("fEnvVarNames", &fEnvVarNames);
      meta_tree->Branch("fEnvVarValues", &fEnvVarValues);
      meta_tree->Branch("JobRNGSeed", &fJobRNGSeed);
#ifdef USE_PREDINTERP_OMP
      meta_tree->Branch("NMaxThreads", &fNMaxThreads);
#endif
    }
  }

  if (odir) {
    odir->cd();
  }
}

FitTreeBlob *FitTreeBlob::MakeReader(TTree *t, TTree *m) {
  FitTreeBlob *ftb = new FitTreeBlob();
  t->SetBranchAddress("chisq", &ftb->fChiSq);
  t->SetBranchAddress("NSeconds", &ftb->fNSeconds);
  t->SetBranchAddress("ResMemUsage", &ftb->fResMemUsage);
  t->SetBranchAddress("VirtMemUsage", &ftb->fVirtMemUsage);
  t->SetBranchAddress("NFCN", &ftb->fNFCN);
  t->SetBranchAddress("EDM", &ftb->fEDM);
  t->SetBranchAddress("IsValid", &ftb->fIsValid);
  t->SetBranchAddress("fFakeDataVals", &ftb->fFakeDataVals);
  t->SetBranchAddress("fPreFitValues", &ftb->fPreFitValues);
  t->SetBranchAddress("fPreFitErrors", &ftb->fPreFitErrors);
  t->SetBranchAddress("fPostFitValues", &ftb->fPostFitValues);
  t->SetBranchAddress("fPostFitErrors", &ftb->fPostFitErrors);
  t->SetBranchAddress("fCentralValues", &ftb->fCentralValues);
  t->SetBranchAddress("LoopRNGSeed", &ftb->fLoopRNGSeed);
  t->SetBranchAddress("JobRNGSeed", &ftb->fJobRNGSeed);
  t->SetBranchAddress("SpectraRNGSeeds", &ftb->fSpectraRNGSeeds);
  t->SetBranchAddress("ProcFitN", &ftb->fNFills);
  t->SetBranchAddress("NOscSeeds", &ftb->fNOscSeeds);

  m->SetBranchAddress("fParamNames", &ftb->fParamNames);
  m->SetBranchAddress("fEnvVarNames", &ftb->fEnvVarNames);
  m->SetBranchAddress("fEnvVarValues", &ftb->fEnvVarValues);
  m->SetBranchAddress("JobRNGSeed", &ftb->fJobRNGSeed);
#ifdef USE_PREDINTERP_OMP
  m->SetBranchAddress("NMaxThreads", &ftb->fNMaxThreads);
#endif
  return ftb;
}
FitTreeBlob::~FitTreeBlob() {
  delete fFakeDataVals;
  delete fParamNames;
  delete fPreFitValues;
  delete fPreFitErrors;
  delete fPostFitValues;
  delete fPostFitErrors;
  delete fCentralValues;
  delete fEnvVarNames;
  delete fEnvVarValues;
  delete fSpectraRNGSeeds;
}
void FitTreeBlob::CopyVals(FitTreeBlob const &fb) {
  (*fFakeDataVals) = (*fb.fFakeDataVals);
  (*fParamNames) = (*fb.fParamNames);
  (*fEnvVarNames) = (*fb.fEnvVarNames);
  (*fEnvVarValues) = (*fb.fEnvVarValues);
  (*fPreFitValues) = (*fb.fPreFitValues);
  (*fPreFitErrors) = (*fb.fPreFitErrors);
  (*fPostFitValues) = (*fb.fPostFitValues);
  (*fPostFitErrors) = (*fb.fPostFitErrors);
  (*fCentralValues) = (*fb.fCentralValues);
  (*fSpectraRNGSeeds) = (*fb.fSpectraRNGSeeds);
  fChiSq = fb.fChiSq;
  fNFCN = fb.fNFCN;
  fEDM = fb.fEDM;
  fIsValid = fb.fIsValid;
  fNSeconds = fb.fNSeconds;
  fResMemUsage = fb.fResMemUsage;
  fVirtMemUsage = fb.fVirtMemUsage;
  fLoopRNGSeed = fb.fLoopRNGSeed;
  fJobRNGSeed = fb.fJobRNGSeed;
  fNFills = fb.fNFills;
  fNOscSeeds = fb.fNOscSeeds;
#ifdef USE_PREDINTERP_OMP
  fNMaxThreads = fb.fNMaxThreads;
#endif
}
void FitTreeBlob::Fill() {
  if (throw_tree) {
    throw_tree->Fill();
  }

  if (meta_tree && !fMeta_filled) {
    meta_tree->Fill();
    fMeta_filled = true;
  }

  fNFills++;
}
void FitTreeBlob::SetDirectory(TDirectory *d) {
  if (throw_tree) {
    throw_tree->SetDirectory(d);
  }
  if (meta_tree) {
    meta_tree->SetDirectory(d);
  }
}
void FitTreeBlob::SetDirectoryClone(TDirectory *d) {
  if (!d) {
    return;
  }
  if (throw_tree) {
    TTree *clone = throw_tree->CloneTree();
    clone->ResetBranchAddresses();
    clone->SetDirectory(d);
    throw_tree->SetDirectory(nullptr);
  }
  if (meta_tree) {
    TTree *clone = meta_tree->CloneTree();
    clone->ResetBranchAddresses();
    clone->SetDirectory(d);
    meta_tree->SetDirectory(nullptr);
  }
}
void FitTreeBlob::Write() {
  if (throw_tree) {
    throw_tree->Write();
  }
  if (meta_tree) {
    meta_tree->Write();
  }
}
void FitTreeBlob::WriteClone() {
  if (throw_tree) {
    TTree *clone = throw_tree->CloneTree();
    clone->ResetBranchAddresses();
    clone->Write();
    throw_tree->SetDirectory(nullptr);
  }
  if (meta_tree) {
    TTree *clone = meta_tree->CloneTree();
    clone->ResetBranchAddresses();
    clone->Write();
    meta_tree->SetDirectory(nullptr);
  }
}

std::string BuildLogInfoString() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  ProcInfo_t procinfo;
  gSystem->GetProcInfo(&procinfo);
  MemInfo_t meminfo;
  gSystem->GetMemInfo(&meminfo);

  std::stringstream ss("");
  ss << " (ProcResMem: " << (double(procinfo.fMemResident) / 1E6) << " GB) @ "
     << std::ctime(&now_time);

  return ss.str();
}

std::vector<seeded_spectra>
BuildSpectra(PredictionInterp *predFDNumuFHC, PredictionInterp *predFDNueFHC,
             PredictionInterp *predFDNumuRHC, PredictionInterp *predFDNueRHC,
             PredictionInterp *predNDNumuFHC, PredictionInterp *predNDNumuRHC,
             osc::IOscCalculatorAdjustable *fakeDataOsc,
             SystShifts fakeDataSyst, bool fakeDataStats, double pot_fd_fhc_nue,
             double pot_fd_fhc_numu, double pot_fd_rhc_nue,
             double pot_fd_rhc_numu, double pot_nd_fhc, double pot_nd_rhc,
             std::vector<unsigned> const &Seeds) {
  std::vector<seeded_spectra> spectra;
  size_t iseed = 0;

  // Ordering of these is important
  // kFDNueFHC
  if (predFDNueFHC) {
    spectra.emplace_back(
        Seeds[iseed], std::unique_ptr<Spectrum>(new Spectrum(
                          predFDNueFHC->PredictSyst(fakeDataOsc, fakeDataSyst)
                              .MockData(pot_fd_fhc_nue, fakeDataStats,
                                        fakeDataStats ? Seeds[iseed] : 0))));
    iseed++;
  }
  // kFDNumuFHC
  if (predFDNumuFHC) {
    spectra.emplace_back(
        Seeds[iseed], std::unique_ptr<Spectrum>(new Spectrum(
                          predFDNumuFHC->PredictSyst(fakeDataOsc, fakeDataSyst)
                              .MockData(pot_fd_fhc_numu, fakeDataStats,
                                        fakeDataStats ? Seeds[iseed] : 0))));
    iseed++;
  }
  // kFDNueRHC
  if (predFDNueRHC) {
    spectra.emplace_back(
        Seeds[iseed], std::unique_ptr<Spectrum>(new Spectrum(
                          predFDNueRHC->PredictSyst(fakeDataOsc, fakeDataSyst)
                              .MockData(pot_fd_rhc_nue, fakeDataStats,
                                        fakeDataStats ? Seeds[iseed] : 0))));
    iseed++;
  }
  // kFDNumuRHC
  if (predFDNumuRHC) {
    spectra.emplace_back(
        Seeds[iseed], std::unique_ptr<Spectrum>(new Spectrum(
                          predFDNumuRHC->PredictSyst(fakeDataOsc, fakeDataSyst)
                              .MockData(pot_fd_rhc_numu, fakeDataStats,
                                        fakeDataStats ? Seeds[iseed] : 0))));
    iseed++;
  }
  // kNDNumuFHC
  if (predNDNumuFHC) {
    spectra.emplace_back(
        Seeds[iseed], std::unique_ptr<Spectrum>(new Spectrum(
                          predNDNumuFHC->PredictSyst(fakeDataOsc, fakeDataSyst)
                              .MockData(pot_nd_fhc, fakeDataStats,
                                        fakeDataStats ? Seeds[iseed] : 0))));
    iseed++;
  }
  // kNDNumuRHC
  if (predNDNumuRHC) {
    spectra.emplace_back(
        Seeds[iseed], std::unique_ptr<Spectrum>(new Spectrum(
                          predNDNumuRHC->PredictSyst(fakeDataOsc, fakeDataSyst)
                              .MockData(pot_nd_rhc, fakeDataStats,
                                        fakeDataStats ? Seeds[iseed] : 0))));
    iseed++;
  }

  return spectra;
}

double RunFitPoint(std::string stateFileName, std::string sampleString,
                   osc::IOscCalculatorAdjustable *fakeDataOsc,
                   SystShifts fakeDataSyst, bool fakeDataStats,
                   std::vector<const IFitVar *> oscVars,
                   std::vector<const ISyst *> systlist,
                   osc::IOscCalculatorAdjustable *fitOsc, SystShifts fitSyst,
                   ana::SeedList oscSeeds, IChiSqExperiment *penaltyTerm,
                   MinuitFitter::FitOpts fitStrategy, TDirectory *outDir,
                   FitTreeBlob *PostFitTreeBlob,
                   std::vector<seeded_spectra> *spectra, SystShifts &bf) {

  assert(systlist.size() + oscVars.size());

  auto AnaV = GetAnaVersion();

  bool turbose = false;
  if (getenv("CAFANA_FIT_TURBOSE") &&
      bool(atoi(getenv("CAFANA_FIT_TURBOSE")))) {
    turbose = true;
  }

#ifdef USE_PREDINTERP_OMP
  size_t maxthreads = omp_get_max_threads();
  if (maxthreads > 4) {
    std::cerr << "[INFO]: Cannot run with OMP_NUM_THREADS > 4" << std::endl;
    abort();
  }
  if (PostFitTreeBlob) {
    PostFitTreeBlob->fNMaxThreads = maxthreads;
  }
#endif

  // Start by getting the PredictionInterps... better that this is done here
  // than elsewhere as they aren't smart enough to know what they are (so the
  // order matters) Note that all systs are used to load the PredictionInterps
  static bool first_load = true;
  static auto start_load = std::chrono::system_clock::now();

  static bool PI_load = true;
  static std::vector<ana::ISyst const *> syststoload = systlist;
  if (PI_load) {

    for (auto &env_str :
         {"CAFANA_ANALYSIS_VERSION", "CAFANA_USE_UNCORRNDCOVMAT",
          "CAFANA_USE_NDCOVMAT", "CAFANA_IGNORE_CV_WEIGHT",
          "CAFANA_IGNORE_SELECTION", "CAFANA_DISABLE_DERIVATIVES",
          "CAFANA_DONT_CLAMP_SYSTS", "CAFANA_FIT_TURBOSE",
          "CAFANA_FIT_FORCE_HESSE", "CAFANA_PRED_MINMCSTATS", "FIT_PRECISION",
          "FIT_TOLERANCE", "SLURM_JOB_ID", "SLURM_PROCID", "SLURM_NODEID",
          "SLURM_LOCALID"}) {
      if (getenv(env_str)) {
        std::cout << "[ENV]: " << env_str << " = " << getenv(env_str)
                  << std::endl;
        if (PostFitTreeBlob) {
          PostFitTreeBlob->fEnvVarNames->push_back(env_str);
          PostFitTreeBlob->fEnvVarValues->push_back(getenv(env_str));
        }
      }
    }

    std::vector<ana::ISyst const *> fdlos =
        GetListOfSysts(false, false, false, false, false, false,
                       true /*add fake data*/, false);
    syststoload.insert(syststoload.end(), fdlos.begin(), fdlos.end());
    PI_load = false;
  }

  static std::vector<std::unique_ptr<PredictionInterp>> interp_list =
      GetPredictionInterps(stateFileName, syststoload);

  static PredictionInterp &predFDNumuFHC = *interp_list[0].release();
  static PredictionInterp &predFDNueFHC = *interp_list[1].release();
  static PredictionInterp &predFDNumuRHC = *interp_list[2].release();
  static PredictionInterp &predFDNueRHC = *interp_list[3].release();
  static PredictionInterp &predNDNumuFHC = *interp_list[4].release();
  static PredictionInterp &predNDNumuRHC = *interp_list[5].release();
  if (first_load) {
    static auto end_load = std::chrono::system_clock::now();
    std::cerr << "[LOAD]: Done in "
              << std::chrono::duration_cast<std::chrono::seconds>(end_load -
                                                                  start_load)
                     .count()
              << " s " << BuildLogInfoString();
    first_load = false;
  }

  // String parsing time!
  double pot_nd_fhc, pot_nd_rhc, pot_fd_fhc_nue, pot_fd_rhc_nue,
      pot_fd_fhc_numu, pot_fd_rhc_numu;
  ParseDataSamples(sampleString, pot_nd_fhc, pot_nd_rhc, pot_fd_fhc_nue,
                   pot_fd_rhc_nue, pot_fd_fhc_numu, pot_fd_rhc_numu);

  // If a directory has been given, a whole mess of stuff will be saved there.
  if (outDir) {
    outDir->cd();
  }

  // Need to make a fake throw list for later book-keeping
  std::vector<double> fFakeDataVals;
  for (const IFitVar *v : oscVars)
    fFakeDataVals.push_back(v->GetValue(fakeDataOsc));
  for (const ISyst *s : systlist)
    fFakeDataVals.push_back(fakeDataSyst.GetShift(s));

  // One problem with this method is that the experiments are reproduced for
  // every single call... Set up the fake data histograms, and save them if
  // relevant... This came back to bite me. I need to do multiple fits with the
  // same fake data throw, so I need these to persist between calls to this
  // function Sadly, the locked down ownership etc means I can't think of a
  // better solution to this And the alternative CAFAna is probably just
  // copy-pasta-ing this function in all of the scripts that use it...
  std::vector<seeded_spectra> LastShredsOfMyDignityAndSanity;
  if (!spectra) {
    spectra = &LastShredsOfMyDignityAndSanity;
  }

  if (!spectra->size()) {

    if (turbose) {
      std::cout << "[INFO]: Loading spectra. " << BuildLogInfoString()
                << std::endl;
    }

    std::vector<unsigned> seeds;
    for (size_t i = 0; i < 6; ++i) {
      seeds.push_back(gRandom->Integer(std::numeric_limits<unsigned>::max()));
    }

    (*spectra) = BuildSpectra(&predFDNumuFHC, &predFDNueFHC, &predFDNumuRHC,
                              &predFDNueRHC, &predNDNumuFHC, &predNDNumuRHC,
                              fakeDataOsc, fakeDataSyst, fakeDataStats,
                              pot_fd_fhc_nue, pot_fd_fhc_numu, pot_fd_rhc_nue,
                              pot_fd_rhc_numu, pot_nd_fhc, pot_nd_rhc, seeds);
  }

  if (turbose) {
    std::cout << "[INFO]: Have fit spectra. " << BuildLogInfoString()
              << std::endl;
  }
  if (turbose) {
    std::cout << "[INFO]: Setting up experiments. " << BuildLogInfoString()
              << std::endl;
  }

  // If using the multi sample covariances then they must be added to the
  // MultiExperiment
  SingleSampleExperiment app_expt_fhc(&predFDNueFHC,
                                      *spectra->at(kFDNueFHC).spect);
  app_expt_fhc.SetMaskHist(0.5, (AnaV == kV4) ? 10 : 8);

  SingleSampleExperiment dis_expt_fhc(&predFDNumuFHC,
                                      *spectra->at(kFDNumuFHC).spect);
  dis_expt_fhc.SetMaskHist(0.5, (AnaV == kV4) ? 10 : 8);

  SingleSampleExperiment app_expt_rhc(&predFDNueRHC,
                                      *spectra->at(kFDNueRHC).spect);
  app_expt_rhc.SetMaskHist(0.5, (AnaV == kV4) ? 10 : 8);

  SingleSampleExperiment dis_expt_rhc(&predFDNumuRHC,
                                      *spectra->at(kFDNumuRHC).spect);
  dis_expt_rhc.SetMaskHist(0.5, (AnaV == kV4) ? 10 : 8);

  SingleSampleExperiment nd_expt_fhc(&predNDNumuFHC,
                                     *spectra->at(kNDNumuFHC).spect);
  nd_expt_fhc.SetMaskHist(0.5, 10, 0, -1);

  SingleSampleExperiment nd_expt_rhc(&predNDNumuRHC,
                                     *spectra->at(kNDNumuRHC).spect);
  nd_expt_rhc.SetMaskHist(0.5, 10, 0, -1);

  if (PostFitTreeBlob) {
    // Save the seeds used to do the stats throws
    (*PostFitTreeBlob->fSpectraRNGSeeds) = std::vector<unsigned>{
        spectra->at(kFDNueFHC).stats_seed,  spectra->at(kFDNumuFHC).stats_seed,
        spectra->at(kFDNueRHC).stats_seed,  spectra->at(kFDNumuRHC).stats_seed,
        spectra->at(kNDNumuFHC).stats_seed, spectra->at(kNDNumuRHC).stats_seed};
  }

  if (turbose) {
    std::cout << "[INFO]: Have experiments. " << BuildLogInfoString()
              << std::endl;
  }

  // Save prefit starting distributions
  if (outDir) {

    if (turbose) {
      std::cout << "[INFO]: Saving prefit info. " << BuildLogInfoString()
                << std::endl;
    }

    if (pot_fd_fhc_nue > 0) {
      TH1 *data_nue_fhc_hist =
          spectra->at(kFDNueFHC).spect->ToTHX(pot_fd_fhc_nue);
      data_nue_fhc_hist->SetName("data_fd_nue_fhc");
      data_nue_fhc_hist->Write();
      TH1 *pre_fd_nue_fhc = GetMCSystTotal(&predFDNueFHC, fitOsc, fitSyst,
                                           "prefit_fd_nue_fhc", pot_fd_fhc_nue);
      pre_fd_nue_fhc->SetTitle(
          std::to_string(app_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_fd_nue_fhc->Write();
    }
    if (pot_fd_fhc_numu > 0) {
      TH1 *data_numu_fhc_hist =
          spectra->at(kFDNumuFHC).spect->ToTHX(pot_fd_fhc_numu);
      data_numu_fhc_hist->SetName("data_fd_numu_fhc");
      data_numu_fhc_hist->Write();
      TH1 *pre_fd_numu_fhc =
          GetMCSystTotal(&predFDNumuFHC, fitOsc, fitSyst, "prefit_fd_numu_fhc",
                         pot_fd_fhc_numu);
      pre_fd_numu_fhc->SetTitle(
          std::to_string(dis_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_fd_numu_fhc->Write();
    }
    if (pot_fd_rhc_nue > 0) {
      TH1 *data_nue_rhc_hist =
          spectra->at(kFDNueRHC).spect->ToTHX(pot_fd_rhc_nue);
      data_nue_rhc_hist->SetName("data_fd_nue_rhc");
      data_nue_rhc_hist->Write();
      TH1 *pre_fd_nue_rhc = GetMCSystTotal(&predFDNueRHC, fitOsc, fitSyst,
                                           "prefit_fd_nue_rhc", pot_fd_rhc_nue);
      pre_fd_nue_rhc->SetTitle(
          std::to_string(app_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_fd_nue_rhc->Write();
    }
    if (pot_fd_rhc_numu > 0) {
      TH1 *data_numu_rhc_hist =
          spectra->at(kFDNumuRHC).spect->ToTHX(pot_fd_rhc_numu);
      data_numu_rhc_hist->SetName("data_fd_numu_rhc");
      data_numu_rhc_hist->Write();
      TH1 *pre_fd_numu_rhc =
          GetMCSystTotal(&predFDNumuRHC, fitOsc, fitSyst, "prefit_fd_numu_rhc",
                         pot_fd_rhc_numu);
      pre_fd_numu_rhc->SetTitle(
          std::to_string(dis_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_fd_numu_rhc->Write();
    }
    if (pot_nd_fhc > 0) {
      TH1 *nd_data_numu_fhc_hist =
          spectra->at(kNDNumuFHC).spect->ToTHX(pot_nd_fhc);
      TH1 *nd_data_numu_fhc_hist_1D =
          spectra->at(kNDNumuFHC).spect->ToTH1(pot_nd_fhc);
      nd_data_numu_fhc_hist->SetName("data_nd_numu_fhc");
      nd_data_numu_fhc_hist_1D->SetName("data_nd_numu_fhc_1D");
      nd_data_numu_fhc_hist->Write();
      nd_data_numu_fhc_hist_1D->Write();

      TH1 *pre_nd_numu_fhc = GetMCSystTotal(&predNDNumuFHC, fitOsc, fitSyst,
                                            "prefit_nd_numu_fhc", pot_nd_fhc);
      pre_nd_numu_fhc->SetTitle(
          std::to_string(nd_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_nd_numu_fhc->Write();
      TH1 *pre_nd_numu_fhc_1D =
          GetMCSystTotal(&predNDNumuFHC, fitOsc, fitSyst,
                         "prefit_nd_numu_fhc_1D", pot_nd_fhc, true);
      pre_nd_numu_fhc_1D->SetTitle(
          std::to_string(nd_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_nd_numu_fhc_1D->Write();
    }
    if (pot_nd_rhc) {
      TH1 *nd_data_numu_rhc_hist =
          spectra->at(kNDNumuRHC).spect->ToTHX(pot_nd_rhc);
      TH1 *nd_data_numu_rhc_hist_1D =
          spectra->at(kNDNumuRHC).spect->ToTH1(pot_nd_rhc);
      nd_data_numu_rhc_hist->SetName("data_nd_numu_rhc");
      nd_data_numu_rhc_hist_1D->SetName("data_nd_numu_rhc_1D");
      nd_data_numu_rhc_hist->Write();
      nd_data_numu_rhc_hist_1D->Write();

      TH1 *pre_nd_numu_rhc = GetMCSystTotal(&predNDNumuRHC, fitOsc, fitSyst,
                                            "prefit_nd_numu_rhc", pot_nd_rhc);
      pre_nd_numu_rhc->SetTitle(
          std::to_string(nd_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_nd_numu_rhc->Write();
      TH1 *pre_nd_numu_rhc_1D =
          GetMCSystTotal(&predNDNumuRHC, fitOsc, fitSyst,
                         "prefit_nd_numu_rhc_1D", pot_nd_rhc, true);
      pre_nd_numu_rhc_1D->SetTitle(
          std::to_string(nd_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_nd_numu_rhc_1D->Write();
    }

    if (turbose) {
      std::cout << "[INFO]: Done writing prefit " << BuildLogInfoString()
                << std::endl;
    }
  }

  // Now sort out the experiment
  MultiExperiment this_expt;
  if (pot_nd_fhc > 0)
    this_expt.Add(&nd_expt_fhc);
  if (pot_nd_rhc > 0)
    this_expt.Add(&nd_expt_rhc);
  if (pot_fd_fhc_numu > 0)
    this_expt.Add(&dis_expt_fhc);
  if (pot_fd_rhc_numu > 0)
    this_expt.Add(&dis_expt_rhc);
  if (pot_fd_fhc_nue > 0)
    this_expt.Add(&app_expt_fhc);
  if (pot_fd_rhc_nue > 0)
    this_expt.Add(&app_expt_rhc);

  if (turbose) {
    std::cout << "[INFO]: Built multi-experiment " << BuildLogInfoString()
              << std::endl;
  }

  bool UseNDCovMat = true;
  if (getenv("CAFANA_USE_NDCOVMAT")) {
    UseNDCovMat = bool(atoi(getenv("CAFANA_USE_NDCOVMAT")));
  }

  bool UseV3NDCovMat = (AnaV == kV3);
  if (getenv("CAFANA_USE_UNCORRNDCOVMAT")) {
    UseV3NDCovMat = bool(atoi(getenv("CAFANA_USE_UNCORRNDCOVMAT")));
  }

  // Add in the covariance matrices via the MultiExperiment
  // idx must be in correct order to access correct part of matrix
  // Don't use FD covmx fits
  if (UseNDCovMat && (pot_nd_rhc > 0) && (pot_nd_fhc > 0)) {

    // Get the ndCov
    const std::string detCovPath =
        "/pnfs/dune/persistent/users/LBL_TDR/CAFs/v4/";
#ifndef DONT_USE_FQ_HARDCODED_SYST_PATHS
    std::string covFileName =
        detCovPath + ((AnaV == kV3) ? "/Systs/det_sys_cov_v3binning.root"
                                    : "/det_sys_cov.root");
#else
    std::string covFileName =
        FindCAFAnaDir() + ((AnaV == kV3) ? "/Systs/det_sys_cov_v3binning.root"
                                         : "/Systs/det_sys_cov.root");
#endif

    if (turbose) {
      std::cout << "[INFO]: Adding ND covmat " << BuildLogInfoString()
                << std::endl;
    }
    if (UseV3NDCovMat) {

      std::cout << "[INFO]: Using v3-like ND covmat treadment." << std::endl;

      TDirectory *thisDir = gDirectory->CurrentDirectory();
      TFile covMatFile(covFileName.c_str());
      TMatrixD *fake_uncorr = (TMatrixD *)covMatFile.Get("nd_all_frac_cov");
      if (!fake_uncorr) {
        std::cout << "Could not obtain covariance matrix named "
                     "\"nd_all_frac_cov\"  from "
                  << covFileName << std::endl;
        abort();
      }

      TMatrixD *covmx_fhc_only = (TMatrixD *)covMatFile.Get("nd_fhc_frac_cov");

      assert(fake_uncorr->GetNrows() == 2 * covmx_fhc_only->GetNrows());

      size_t NRows = fake_uncorr->GetNrows();
      size_t NRows_FHC = covmx_fhc_only->GetNrows();
      for (size_t row_it = 0; row_it < NRows; ++row_it) {
        for (size_t col_it = 0; col_it < NRows; ++col_it) {

          // Could use TMatrix::SetSub but I don't trust TMatrix...
          if (((row_it >= NRows_FHC) && (col_it < NRows_FHC)) ||
              ((row_it < NRows_FHC) && (col_it >= NRows_FHC))) {
            (*fake_uncorr)[row_it][col_it] = 0;
          } else {
            size_t row_fhc_only_it = row_it % NRows_FHC;
            size_t col_fhc_only_it = col_it % NRows_FHC;
            (*fake_uncorr)[row_it][col_it] =
                (*covmx_fhc_only)[row_fhc_only_it][col_fhc_only_it];
          }
        }
      }

      this_expt.AddCovarianceMatrix(fake_uncorr, true, {0, 1});

      thisDir->cd();

    } else {
      this_expt.AddCovarianceMatrix(covFileName, "nd_all_frac_cov", true,
                                    {0, 1});
    }
    if (turbose) {
      std::cout << "[INFO]: Finished adding ND covmat " << BuildLogInfoString()
                << std::endl;
    }
  }

  // Add in the penalty...
  if (penaltyTerm) {
    this_expt.Add(penaltyTerm);
  }

  auto start_fit = std::chrono::system_clock::now();
  // Now set up the fit itself
  std::cerr << "[INFO]: Beginning fit. " << BuildLogInfoString();
  MinuitFitter this_fit(&this_expt, oscVars, systlist, fitStrategy);
  double thischisq =
      this_fit.Fit(fitOsc, fitSyst, oscSeeds, {}, MinuitFitter::kVerbose);
  auto end_fit = std::chrono::system_clock::now();
  std::time_t end_fit_time = std::chrono::system_clock::to_time_t(end_fit);
  std::cerr << "[FIT]: Finished fit in "
            << std::chrono::duration_cast<std::chrono::seconds>(end_fit -
                                                                start_fit)
                   .count()
            << " s after " << this_fit.GetNFCN() << " iterations "
            << BuildLogInfoString();

  bf = fitSyst;

  // If we have a directory to save to... save some stuff...
  if (outDir) {
    if (turbose) {
      std::cout << "[INFO]: Writing fit tree " << BuildLogInfoString();
    }
    std::vector<std::string> fParamNames = this_fit.GetParamNames();
    std::vector<double> fPreFitValues = this_fit.GetPreFitValues();
    std::vector<double> fPreFitErrors = this_fit.GetPreFitErrors();
    std::vector<double> fPostFitValues = this_fit.GetPostFitValues();
    std::vector<double> fPostFitErrors = this_fit.GetPostFitErrors();
    std::vector<double> fCentralValues = this_fit.GetCentralValues();
    double fNFCN = this_fit.GetNFCN();
    double fEDM = this_fit.GetEDM();
    bool fIsValid = this_fit.GetIsValid();

    TMatrixDSym *covar = (TMatrixDSym *)this_fit.GetCovariance();
    TH2D hist_covar = TH2D(*covar);
    hist_covar.SetName("covar");
    TH2D hist_corr = *make_corr_from_covar(&hist_covar);

    if (turbose) {
      std::cout << "[INFO]: Writing postfit spectra " << BuildLogInfoString()
                << std::endl;
    }

    if (pot_fd_fhc_nue) {
      TH1 *post_fd_nue_fhc = GetMCSystTotal(
          &predFDNueFHC, fitOsc, fitSyst, "postfit_fd_nue_fhc", pot_fd_fhc_nue);
      post_fd_nue_fhc->SetTitle(
          std::to_string(app_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_fd_nue_fhc->Write();
    }
    if (pot_fd_fhc_numu) {
      TH1 *post_fd_numu_fhc =
          GetMCSystTotal(&predFDNumuFHC, fitOsc, fitSyst, "postfit_fd_numu_fhc",
                         pot_fd_fhc_numu);
      post_fd_numu_fhc->SetTitle(
          std::to_string(dis_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_fd_numu_fhc->Write();
    }
    if (pot_fd_rhc_nue) {
      TH1 *post_fd_nue_rhc = GetMCSystTotal(
          &predFDNueRHC, fitOsc, fitSyst, "postfit_fd_nue_rhc", pot_fd_rhc_nue);
      post_fd_nue_rhc->SetTitle(
          std::to_string(app_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_fd_nue_rhc->Write();
    }
    if (pot_fd_rhc_numu) {
      TH1 *post_fd_numu_rhc =
          GetMCSystTotal(&predFDNumuRHC, fitOsc, fitSyst, "postfit_fd_numu_rhc",
                         pot_fd_rhc_numu);
      post_fd_numu_rhc->SetTitle(
          std::to_string(dis_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_fd_numu_rhc->Write();
    }
    if (pot_nd_fhc) {
      TH1 *post_nd_numu_fhc = GetMCSystTotal(&predNDNumuFHC, fitOsc, fitSyst,
                                             "postfit_nd_numu_fhc", pot_nd_fhc);
      post_nd_numu_fhc->SetTitle(
          std::to_string(nd_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_nd_numu_fhc->Write();
      TH1 *post_nd_numu_fhc_1D =
          GetMCSystTotal(&predNDNumuFHC, fitOsc, fitSyst,
                         "postfit_nd_numu_fhc_1D", pot_nd_fhc, true);
      post_nd_numu_fhc_1D->SetTitle(
          std::to_string(nd_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_nd_numu_fhc_1D->Write();
    }
    if (pot_nd_rhc) {
      TH1 *post_nd_numu_rhc = GetMCSystTotal(&predNDNumuRHC, fitOsc, fitSyst,
                                             "postfit_nd_numu_rhc", pot_nd_rhc);
      post_nd_numu_rhc->SetTitle(
          std::to_string(nd_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_nd_numu_rhc->Write();
      TH1 *post_nd_numu_rhc_1D =
          GetMCSystTotal(&predNDNumuRHC, fitOsc, fitSyst,
                         "postfit_nd_numu_rhc_1D", pot_nd_rhc, true);
      post_nd_numu_rhc_1D->SetTitle(
          std::to_string(nd_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_nd_numu_rhc_1D->Write();
    }

    // Save information
    hist_covar.Write();
    hist_corr.Write();
    covar->Write("covar_mat");
  }

  if (PostFitTreeBlob) {
    if (turbose) {
      std::cout << "[INFO]: Assigning fit tree blob values "
                << BuildLogInfoString() << std::endl;
    }
    (*PostFitTreeBlob->fParamNames) = this_fit.GetParamNames();
    (*PostFitTreeBlob->fPreFitValues) = this_fit.GetPreFitValues();
    (*PostFitTreeBlob->fPreFitErrors) = this_fit.GetPreFitErrors();
    (*PostFitTreeBlob->fPostFitValues) = this_fit.GetPostFitValues();
    (*PostFitTreeBlob->fPostFitErrors) = this_fit.GetPostFitErrors();
    (*PostFitTreeBlob->fCentralValues) = this_fit.GetCentralValues();
    (*PostFitTreeBlob->fFakeDataVals) = fFakeDataVals;
    PostFitTreeBlob->fNFCN = this_fit.GetNFCN();
    PostFitTreeBlob->fEDM = this_fit.GetEDM();
    PostFitTreeBlob->fIsValid = this_fit.GetIsValid();
    PostFitTreeBlob->fChiSq = thischisq;
    PostFitTreeBlob->fNSeconds =
        std::chrono::duration_cast<std::chrono::seconds>(end_fit - start_fit)
            .count();
    PostFitTreeBlob->fNOscSeeds = oscSeeds.size();

    ProcInfo_t procinfo;
    gSystem->GetProcInfo(&procinfo);
    PostFitTreeBlob->fResMemUsage = procinfo.fMemResident;
    PostFitTreeBlob->fVirtMemUsage = procinfo.fMemVirtual;
  }

  if (turbose) {
    std::cout << "[INFO]: Finished run fit point." << BuildLogInfoString()
              << std::endl;
  }

  return thischisq;
}
