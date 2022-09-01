#include "CAFAna/PRISM/Axes.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"

#include "CAFAna/Analysis/AnalysisVars.h"

#include "StandardRecord/SRProxy.h"

#include <iostream>
#include <array>

using namespace ana;

namespace PRISM {

const Var kTrueOffAxisPos_m = SIMPLEVAR(abspos_x) * Constant(1.0E-2);

Binning GetBinning(std::string const &xbinning) {
  if (xbinning == "uniform_fine") {
    return Binning::Simple(120, 0, 6);
  } else if (xbinning == "uniform") {
    return Binning::Simple(150, 0, 25);
  } else if (xbinning == "uniform_smallrange") {
    return Binning::Simple(50, 0, 10);
  } else if (xbinning == "uniform_coarse") {
    return Binning::Simple(6, 0, 6); 
  } else if (xbinning == "prism_default") {
    return kPRISMRecoBinning;
  } else if (xbinning == "prism_fine_default") {
    return kPRISMFineRecoBinning;
  } else if (xbinning == "default") {
    return kFDRecoBinning;
  } else if (xbinning == "event_rate_match") {
    return kLinearCombBinning; 
  } else if (xbinning == "had_default") {
    return kHadRecoBinning;
  } else if (xbinning == "lep_default") {
    return kLepRecoBinning;
  } else if (xbinning == "true_osc_binning") {
    return kTrueEnergyBins;
  } else {
    std::cout << "[ERROR]: Unknown PRISM binning definition: " << xbinning
              << std::endl;
    abort();
  }
}

Binning GetOABinning(std::string const &oabinning) {
  if (oabinning == "default") {
    std::array<double, 3> OABinning = {0.5, -30, +2}; // +2
    double OA_bin_width_m = OABinning[0];
    double OA_min_m = OABinning[1];
    double OA_max_m = OABinning[2];
    size_t NOABins = (OA_max_m - OA_min_m) / OA_bin_width_m;
    return Binning::Simple(NOABins, OA_min_m, OA_max_m);
  } else {
    std::cout << "[ERROR]: Unknown PRISM OA binning definition: " << oabinning
              << std::endl;
    abort();
  }
}

std::pair<std::string, Var> GetVar(std::string const &varname) {

  if (varname == "ETrue") {
    return std::make_pair("True E_{#nu} (GeV)", kTrueEnergy);
  } else if (varname == "ELep") {
    return std::make_pair("True E_{lep.} (GeV)", SIMPLEVAR(LepE));
  } else if (varname == "EHad") {
    return std::make_pair("True E_{had.} (GeV)", SIMPLEVAR(HadE));
  } else if (varname == "EP") {
    return std::make_pair("True E_{p} (GeV)", SIMPLEVAR(eP));
  } else if (varname == "EPipm") {
    return std::make_pair("True E_{#pi^{+/-}} (GeV)", SIMPLEVAR(ePipm));
  } else if (varname == "EPi0") {
    return std::make_pair("True E_{#pi^{0}} (GeV)", SIMPLEVAR(eTotalPi0));
  } else if (varname == "EOther") {
    return std::make_pair("True E_{other} (GeV)", SIMPLEVAR(eOther));
  } else if (varname == "EProxy") {
    return std::make_pair("Truth proxy E_{#nu} (GeV)", kProxyERec);
  } else if (varname == "ERec") {
    return std::make_pair("E_{Dep.} (GeV)", kRecoE_FromDep);
  } else if (varname == "EVisReco_numu") {
    return std::make_pair("Vis. E_{rec.} (GeV)", kEVisReco_numu);
  } else if (varname == "EVisReco_nue") {
    return std::make_pair("Vis. E_{rec.} (GeV)", kEVisReco_nue);
  } else if (varname == "EVisReco_ND") {
    return std::make_pair("Vis. E_{rec.} (GeV)", kNDEVisReco);
  } else if (varname == "EnuReco_numu") {
    return std::make_pair("Neutrino E_{rec.} (GeV)", kRecoE_numu);
  } else if (varname == "EnuReco_nue") {
    return std::make_pair("Neutrino E_{rec.} (GeV)", kRecoE_nue);
  } else if (varname == "EnuReco_ND") {
    return std::make_pair("Neutrino E_{rec.} (GeV)", kRecoEnergyND);
  } else if (varname == "EVisTrue") {
    return std::make_pair("True E_{vis.} (GeV)", kEVisTrue);
  } else if (varname == "RecoELep_numu") {
    return std::make_pair("Reco E_{lep.} (GeV)", kLepEReco_numu);
  } else if (varname == "RecoELep_nue") {
    return std::make_pair("Reco E_{lep.} (GeV)", kLepEReco_nue);
  } else if (varname == "RecoELep_ND") {
    return std::make_pair("Reco E_{lep.} (GeV)", kNDLepEReco);
  } else if (varname == "HadEVisReco_ND") {
    return std::make_pair("Reco E_{had.} (GeV)", SIMPLEVAR(HadEVisReco_ND));
  } else if (varname == "HadEVisReco_FD") {
    return std::make_pair("Reco E_{had.} (GeV)", SIMPLEVAR(HadEVisReco_FD));
  } else if (varname == "RecoEHad_numu") {
    return std::make_pair("Reco E_{had.} (GeV)", kHadEReco_numu);
  } else if (varname == "RecoEHad_nue") {
    return std::make_pair("Reco E_{had.} (GeV)", kHadEReco_nue);
  }  else if (varname == "RecoEP") {
    return std::make_pair("Reco E_{p} (GeV)", kPEReco);
  }  else if (varname == "RecoEPipm") {
    return std::make_pair("Reco E_{#pi^{+/-}} (GeV)", kPipmEReco);
  }  else if (varname == "RecoEPi0") {
    return std::make_pair("Reco E_{#pi^{0}} (GeV)", kPi0EReco);
  }  else if (varname == "RecoEOther") {
    return std::make_pair("Reco E_{other} (GeV)", SIMPLEVAR(eRecoOther));
  } else {
    std::cout << "[ERROR]: Unknown PRISM var definition: " << varname
              << std::endl;
    abort();
  }
}

HistAxis GetEventRateMatchAxes(std::string const &binning) {
  auto vardef = GetVar("ETrue");
  return HistAxis(vardef.first, GetBinning(binning), vardef.second);
}

PRISMAxisBlob GetPRISMAxes(std::string const &varname,
                           std::string const &xbinning,
                           std::string const &oabinning) {

  HistAxis axOffAxisPos("Off axis position (m)", GetOABinning(oabinning),
                        kTrueOffAxisPos_m);

  HistAxis axOffAxis280kAPos("Off axis position (m)", Binning::Simple(1, -2, +2), 
                             kTrueOffAxisPos_m);
  
  // Seperate ND and FD axes for ND->FD extrapolation.
  // Possible fine binning for ND axis and extended energy range.
  // Only needed for EVisReco, not ELepEHad.
  bool OneDAxis(false);
  if (varname == "EVisReco" || varname == "EProxy" || varname == "EnuReco") OneDAxis = true;
  std::string varname_ND(varname);
  std::string varname_numu(varname);
  std::string varname_nue(varname);
  if (varname == "EVisReco" || varname == "ELepEHadVisReco" || varname == "EnuReco") {
    varname_ND   += "_ND";
    varname_numu += "_numu";
    varname_nue  += "_nue";
  }
  HistAxis xaxND      = RecoObservable(varname_ND.c_str(),
                                       OneDAxis ? "prism_fine_default" : xbinning);
  HistAxis xaxFD_numu = RecoObservable(varname_numu.c_str(), xbinning);
  HistAxis xaxFD_nue  = RecoObservable(varname_nue.c_str(), xbinning);

  return {xaxND, xaxFD_numu, xaxFD_nue, axOffAxisPos, axOffAxis280kAPos};
}

// Return HistAxis for true energy version of observable
HistAxis TrueObservable(std::string const &obsvarname, 
                        std::string const &binning) {
  auto truevardef = GetVar("ETrue");
  std::vector<std::string> labels;
  std::vector<Binning> bins;
  std::vector<Var> vars;

  if (obsvarname == "EProxy") {
    truevardef = GetVar("ETrue");
  } else if (obsvarname == "ETrue") {
    truevardef = GetVar("ETrue");
  } else if (obsvarname == "RecoELep") {
    truevardef = GetVar("ELep");
  } else if (obsvarname == "ELep") {
    truevardef = GetVar("ELep");
  } else if (obsvarname == "EHad") {
    truevardef = GetVar("EHad");
  } else if (obsvarname == "RecoEHad") { 
    truevardef = GetVar("EHad");
  } else if (obsvarname == "EVisReco") {
    truevardef = GetVar("EVisTrue");
  } else if (obsvarname == "EnuReco") {
    truevardef = GetVar("ETrue"); // EVisTrue
  } else if (obsvarname == "EVisTrue") {
    truevardef = GetVar("ETrue");
  } else if (obsvarname == "RecoEP") {
    truevardef = GetVar("EP");
  } else if (obsvarname == "RecoEPipm") { 
    truevardef = GetVar("EPipm");
  } else if (obsvarname == "RecoEPi0") {
    truevardef = GetVar("EPi0");
  } else if (obsvarname == "RecoEOther") {
    truevardef = GetVar("EOther");
  } else if (obsvarname == "ELepEHad") {
    auto truevardefLep = GetVar("ELep");
    labels.push_back(truevardefLep.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(truevardefLep.second);
    auto truevardefHad = GetVar("EHad");
    labels.push_back(truevardefHad.first);
    bins.push_back(GetBinning("had_default"));
    vars.push_back(truevardefHad.second);
  } else if (obsvarname == "EVisEHad") {
    auto truevardefHad = GetVar("EHad");
    labels.push_back(truevardefHad.first);
    bins.push_back(GetBinning("had_default"));
    vars.push_back(truevardefHad.second);
    auto vardefVis = GetVar("EVisTrue");
    labels.push_back(vardefVis.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(vardefVis.second);
  } else if (obsvarname == "EVisELep") {
    auto truevardefLep = GetVar("ELep");
    labels.push_back(truevardefLep.first);
    bins.push_back(GetBinning("lep_default"));
    vars.push_back(truevardefLep.second);
    auto vardefVis = GetVar("EVisTrue");
    labels.push_back(vardefVis.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(vardefVis.second);
  } else if (obsvarname == "ELepEHadReco") {
    auto truevardefLep = GetVar("ELep");
    labels.push_back(truevardefLep.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(truevardefLep.second);
    auto truevardefHad = GetVar("EHad");
    labels.push_back(truevardefHad.first);
    bins.push_back(GetBinning("had_default"));
    vars.push_back(truevardefHad.second);
  } else if (obsvarname == "EVisEHadReco") {
    auto truevardefHad = GetVar("EHad");
    labels.push_back(truevardefHad.first);
    bins.push_back(GetBinning("had_default"));
    vars.push_back(truevardefHad.second);
    auto vardefVis = GetVar("EVisTrue");
    labels.push_back(vardefVis.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(vardefVis.second);
  } else if (obsvarname == "EVisELepReco") {
    auto truevardefLep = GetVar("ELep");
    labels.push_back(truevardefLep.first);
    bins.push_back(GetBinning("lep_default"));
    vars.push_back(truevardefLep.second);
    auto vardefVis = GetVar("EVisTrue");
    labels.push_back(vardefVis.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(vardefVis.second);
  } else {
    std::cout << "[ERROR] Unknown var name: " << obsvarname << std::endl;
    abort();
  }   
  
  if (obsvarname == "ELepEHad" || obsvarname == "ELepEHadReco") {
    return HistAxis(labels, bins, vars);
  } else if (obsvarname == "EVisEHad" || obsvarname == "EVisEHadReco") {
    return HistAxis(labels, bins, vars);
  } else if (obsvarname == "EVisELep" || obsvarname == "EVisELepReco") {
    return HistAxis(labels, bins, vars);
  } else {
    return HistAxis(truevardef.first, GetBinning(binning), truevardef.second);
  }
}

HistAxis RecoObservable(std::string const &obsvarname,
                        std::string const &binning) {              
  std::vector<std::string> labels;
  std::vector<Binning> bins;
  std::vector<Var> vars;

  if (obsvarname == "ELepEHad") { // 2D ELep EHad Prediction
    auto vardefLep = GetVar("ELep");   
    labels.push_back(vardefLep.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(vardefLep.second);   
    auto vardefHad = GetVar("EHad");
    labels.push_back(vardefHad.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(vardefHad.second);
  } else if  (obsvarname == "EVisEHad") {
    auto vardefHad = GetVar("EHad");
    labels.push_back(vardefHad.first);
    bins.push_back(GetBinning("had_default"));
    vars.push_back(vardefHad.second);
    auto vardefVis = GetVar("EVisTrue");
    labels.push_back(vardefVis.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(vardefVis.second);
  } else if  (obsvarname == "EVisELep") {
    auto vardefLep = GetVar("ELep");
    labels.push_back(vardefLep.first);
    bins.push_back(GetBinning("lep_default"));
    vars.push_back(vardefLep.second);
    auto vardefVis = GetVar("EVisTrue");
    labels.push_back(vardefVis.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(vardefVis.second);
  } else if (obsvarname == "ELepEHadVisReco_numu") {   
    auto vardefLep = GetVar("RecoELep_numu");
    labels.push_back(vardefLep.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(vardefLep.second); 
    auto vardefHad = GetVar("HadEVisReco_FD"); 
    labels.push_back(vardefHad.first);
    bins.push_back(GetBinning("had_default")); 
    vars.push_back(vardefHad.second);
  } else if (obsvarname == "ELepEHadVisReco_nue") {
    auto vardefLep = GetVar("RecoELep_nue");
    labels.push_back(vardefLep.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(vardefLep.second); 
    auto vardefHad = GetVar("HadEVisReco_FD");
    labels.push_back(vardefHad.first);
    bins.push_back(GetBinning("had_default"));
    vars.push_back(vardefHad.second);
  } else if (obsvarname == "ELepEHadVisReco_ND") {
    auto vardefLep = GetVar("RecoELep_ND");
    labels.push_back(vardefLep.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(vardefLep.second);
    auto vardefHad = GetVar("HadEVisReco_ND");
    labels.push_back(vardefHad.first);
    bins.push_back(GetBinning("had_default"));
    vars.push_back(vardefHad.second);   
  } else {
    auto vardef = GetVar(obsvarname);
    labels.push_back(vardef.first);
    bins.push_back(GetBinning(binning));  
    vars.push_back(vardef.second);
  }

  return HistAxis(labels, bins, vars);
}

bool isRecoND(std::string var) {
  if (var == "RecoELep" || var == "EVisReco" || var == "EVisEHadReco" ||
      var == "RecoEHad" || var == "EVisEHadReco") {
    return true;
  } else {
    return false;
  }
}

} // namespace PRISM
