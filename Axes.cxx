#include "CAFAna/PRISM/Axes.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"

#include "CAFAna/Analysis/AnalysisVars.h"

#include "StandardRecord/SRProxy.h"

#include <iostream>

using namespace ana;

namespace PRISM {

const Var kTrueOffAxisPos_m = SIMPLEVAR(abspos_x) * Constant(1.0E-2);

Binning GetBinning(std::string const &xbinning) {
  if (xbinning == "uniform_fine") {
    return Binning::Simple(100, 0, 10);
  }
  if (xbinning == "uniform") {
    return Binning::Simple(150, 0, 25);
  }
  if (xbinning == "uniform_smallrange") {
    return Binning::Simple(50, 0, 10);
  }
  if (xbinning == "uniform_coarse") {
    return Binning::Simple(25, 0,
                           10); // used to be 25, 10 (bad tail going out to 10)
  }
  if (xbinning == "testopt") {
    std::vector<double> BE = {
        0,
    };

    while (BE.back() < 4) {
      BE.push_back(BE.back() + 0.25);
    }

    while (BE.back() < 8) {
      BE.push_back(BE.back() + 0.5);
    }

    return Binning::Custom(BE);
  } else if (xbinning == "default") {
    std::vector<double> BE = {
        0,
    };
    while (BE.back() < 1) {
      BE.push_back(BE.back() + 0.5);
    }
    while (BE.back() < 4.0) {
      BE.push_back(BE.back() + 0.25);
    }
    while (BE.back() < 6.0) {
      BE.push_back(BE.back() + 1.0);
    }
    while (BE.back() < 10.0) {
      BE.push_back(BE.back() + 4.0);
    }
    return Binning::Custom(BE);
  } else if (xbinning == "event_rate_match") {
    std::vector<double> BE = {
        0,
    };

    while (BE.back() < 10) {
      BE.push_back(BE.back() + 0.2);
    }

    while (BE.back() < 20) {
      BE.push_back(BE.back() + 1.0);
    }

    while (BE.back() < 50) {
      BE.push_back(BE.back() + 5.0);
    }

    while (BE.back() < 100) {
      BE.push_back(BE.back() + 10.0);
    }

    while (BE.back() < 120) {
      BE.push_back(BE.back() + 20.0);
    }

    return Binning::Custom(BE);
  } else {
    std::cout << "[ERROR]: Unknown PRISM binning definition: " << xbinning
              << std::endl;
    abort();
  }
}

Binning GetOABinning(std::string const &oabinning) {
  if (oabinning == "default") {
    std::array<double, 3> OABinning = {0.5, -30, 0}; // -32
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
  } else if (varname == "EVisReco") {
    return std::make_pair("Reco E_{vis.} (GeV)", kEVisReco);
  } else if (varname == "EVisTrue") {
    return std::make_pair("True E_{vis.} (GeV)", kEVisTrue);
  } else if (varname == "RecoELep") {
    return std::make_pair("Reco E_{lep.} (GeV)", kLepEReco);
  } else if (varname == "RecoEHad") {
    return std::make_pair("Reco E_{had.} (GeV)", kHadEReco);
  } else if (varname == "RecoEP") {
    return std::make_pair("Reco E_{p} (GeV)", kPEReco);
  } else if (varname == "RecoEPipm") {
    return std::make_pair("Reco E_{#pi^{+/-}} (GeV)", kPipmEReco);
  } else if (varname == "RecoEPi0") {
    return std::make_pair("Reco E_{#pi^{0}} (GeV)", kPi0EReco);
  } else if (varname == "RecoEOther") {
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

  HistAxis axOffAxis280kAPos("Off axis position (m)", Binning::Simple(1, -2, 0),
                             kTrueOffAxisPos_m);

  // Seperate ND and FD axes for ND->FD extrapolation.
  // Possible fine binning for ND axis, just keep them the
  // same for now.
  HistAxis xaxND = RecoObservable(varname, xbinning); // xbinning default_fine
  HistAxis xaxFD = RecoObservable(varname, xbinning);

  return {xaxND, xaxFD, axOffAxisPos, axOffAxis280kAPos};
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
    bins.push_back(GetBinning(binning));
    vars.push_back(truevardefHad.second);
  } else if (obsvarname == "ELepEHadReco") {
    auto truevardefLep = GetVar("ELep");
    labels.push_back(truevardefLep.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(truevardefLep.second);
    auto truevardefHad = GetVar("EHad");
    labels.push_back(truevardefHad.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(truevardefHad.second);
  } else {
    std::cout << "[ERROR] Unknown var name: " << obsvarname << std::endl;
    abort();
  }

  if (obsvarname == "ELepEHad" || obsvarname == "ELepEHadReco") {
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
  } else if (obsvarname == "ELepEHadReco") {
    auto vardefLep = GetVar("RecoELep");
    labels.push_back(vardefLep.first);
    bins.push_back(GetBinning(binning));
    vars.push_back(vardefLep.second);
    auto vardefHad = GetVar("RecoEHad");
    labels.push_back(vardefHad.first);
    bins.push_back(GetBinning(binning));
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
  if (var == "RecoELep" || var == "EVisReco" || var == "ELepEHadReco" ||
      var == "RecoEHad") {
    return true;
  } else {
    return false;
  }
}

} // namespace PRISM