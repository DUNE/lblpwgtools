#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"

#include "CAFAna/PRISM/PRISMUtils.h"

#include "CAFAna/Analysis/AnalysisBinnings.h"
#include "CAFAna/Analysis/AnalysisVars.h"

#include "CAFAna/Cuts/AnaCuts.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "StandardRecord/StandardRecord.h"

#include "TGraph.h"

#include <cmath>
#include <iostream>

using namespace ana;

namespace PRISM {

const Var kTrueOffAxisPos =
    (SIMPLEVAR(dune.det_x) + (SIMPLEVAR(dune.vtx_x) * Constant(1.0E-2)));

const Cut kETrueLT10GeV({"kETrueLT10GeV"}, [](const caf::StandardRecord *sr) {
  return (sr->dune.Ev < 10);
});

Binning GetBinning(std::string const &xbinning) {
  if (xbinning == "uniform") {
    return Binning::Simple(100, 0, 10);
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
    return binsNDEreco;
  } else {
    std::cout << "[ERROR]: Unknown PRISM binning definition: " << xbinning
              << std::endl;
    abort();
  }
}

Binning GetOABinning(std::string const &oabinning) {
  if (oabinning == "default") {
    std::array<double, 3> OABinning = {0.5, -3, 33};
    double OA_bin_width_m = OABinning[0];
    double OA_min_m = OABinning[1];
    double OA_max_m = OABinning[2];
    size_t NOABins = (OA_max_m - OA_min_m) / OA_bin_width_m;
    return Binning::Simple(NOABins, OA_min_m, OA_max_m);
  } else if (oabinning == "OneNegXBin") {
    std::vector<double> BE = {-2, 0};
    while (BE.back() < 32.75) {
      BE.push_back(BE.back() + 0.5);
    }
    return Binning::Custom(BE);
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
    return std::make_pair("True E_{lep.} (GeV)", SIMPLEVAR(dune.LepE));
  } else if (varname == "EProxy") {
    return std::make_pair("Truth proxy E_{#nu} (GeV)", kProxyERec);
  } else if (varname == "ERec") {
    return std::make_pair("E_{Dep.} (GeV)", kRecoE_FromDep);
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
                        kTrueOffAxisPos);

  auto vardef = GetVar(varname);
  HistAxis xax(vardef.first, GetBinning(xbinning), vardef.second);

  return {xax, axOffAxisPos};
}

const Cut kIsOutOfTheDesert({"kIsOutOfTheDesert"},
                            [](const caf::StandardRecord *sr) {
                              return (fabs(sr->dune.vtx_x) < 200);
                            });

std::vector<double> const FDnumuFHCSelEff_enu = {
    0.546, 0.683, 0.821, 0.945, 1.07, 1.19, 1.44, 1.67,
    1.94,  2.25,  2.86,  4.17,  4.58, 5.44, 6.57, 7.65};

std::vector<double> const FDnumuFHCSelEff_eff = {
    0.624, 0.708, 0.761, 0.808, 0.839, 0.869, 0.902, 0.914,
    0.930, 0.944, 0.956, 0.958, 0.960, 0.943, 0.929, 0.912};

std::vector<double> const ND_UnGeoCorrectible_enu = {
    0.183, 0.502, 0.870, 1.25, 1.68, 2.45, 3.27, 4.19, 5.28, 6.48, 9.05};

std::vector<double> const ND_UnGeoCorrectible_eff = {0.996, 0.996, 0.985, 0.966,
                                                     0.958, 0.932, 0.898, 0.867,
                                                     0.845, 0.807, 0.742};

TGraph FDnumuFHCSelEff(FDnumuFHCSelEff_enu.size(), FDnumuFHCSelEff_enu.data(),
                       FDnumuFHCSelEff_eff.data());
const Var kFDEff({}, [](const caf::StandardRecord *sr) -> double {
  return FDnumuFHCSelEff.Eval(sr->dune.Ev);
});

TGraph ND_UnGeoCorrectibleEff(ND_UnGeoCorrectible_enu.size(),
                              ND_UnGeoCorrectible_enu.data(),
                              ND_UnGeoCorrectible_eff.data());
const Var kNDEff({}, [](const caf::StandardRecord *sr) -> double {
  return ND_UnGeoCorrectibleEff.Eval(sr->dune.Ev);
});

// Use to weight by Exposure
const Var kRunPlanWeight({}, [](const caf::StandardRecord *sr) -> double {
  return sr->dune.perPOTWeight * sr->dune.perFileWeight;
});

Cut GetNDSignalCut(bool UseOnAxisSelection, bool isNuMode) {

  return UseOnAxisSelection
             ? ((isNuMode ? kPassND_FHC_NUMU : kPassND_RHC_NUMU) && kIsTrueFV &&
                kIsOutOfTheDesert)
             : (kIsNumuCC && (isNuMode ? !kIsAntiNu : kIsAntiNu) && kIsTrueFV &&
                kIsOutOfTheDesert);
}
Cut GetFDSignalCut(bool UseOnAxisSelection, bool isNuMode, bool isNuMu) {
  return UseOnAxisSelection
             ? ((isNuMu ? kPassFD_CVN_NUMU : kPassFD_CVN_NUE) && kIsTrueFV)
             : ((isNuMode ? !kIsAntiNu : kIsAntiNu) &&
                (isNuMu ? kIsNumuCC : kIsNueApp) && kIsTrueFV);
}

Var GetAnalysisWeighters(std::string const &eweight, bool isNuMode) {
  Var weight = Constant(1);

  if (eweight.find("CVXSec") != std::string::npos) {
    weight = weight * kCVXSecWeights;
  }
  if (eweight.find("NDEff") != std::string::npos) {
    if (!isNuMode) {
      std::cout << "ERROR: Do not have efficiency curves for RHC, please add "
                   "them to PRISMAnalysisDefinitions.cxx or remove this weight"
                << std::endl;
      abort();
    }
    weight = weight * kNDEff;
  }
  if (eweight.find("FDEff") != std::string::npos) {
    if (!isNuMode) {
      std::cout << "ERROR: Do not have efficiency curves for RHC, please add "
                   "them to PRISMAnalysisDefinitions.cxx or remove this weight"
                << std::endl;
      abort();
    }
    weight = weight * kFDEff;
  }
  return weight;
}

Var GetNDWeight(std::string const &eweight, bool isNuMode) {
  return kRunPlanWeight * kMassCorrection *
         GetAnalysisWeighters(eweight, isNuMode);
}
Var GetFDWeight(std::string const &eweight, bool isNuMode) {
  return GetAnalysisWeighters(eweight, isNuMode);
}

static TH1 *numode_280kA, *nubarmode_280kA;

const Var k280kAWeighter({}, [](const caf::StandardRecord *sr) -> double {
  if (sr->dune.isFD || sr->dune.det_x || (sr->dune.vtx_x > 0)) {
    return 1;
  }
  // Only want to weight 'signal' numu species.
  if ((sr->dune.isFHC && (sr->dune.nuPDG != 14)) ||
      (!sr->dune.isFHC && (sr->dune.nuPDG != -14))) {
    return 1;
  }

  TH1 *whist = (sr->dune.isFHC ? numode_280kA : nubarmode_280kA);
  return whist->GetBinContent(whist->GetXaxis()->FindFixBin(sr->dune.Ev));
});

ana::Var GetNDSpecialRun(std::string const &SRDescriptor) {

  if (!SRDescriptor.length()) {
    return ana::Constant(1);
  }

  if (SRDescriptor == "OnAxis280kA") {
    static bool first = true;
    if (first) {

      TDirectory *gdc = gDirectory;
      TFile wf(
          (ana::FindCAFAnaDir() + "/Systs/OnAxis4m280kAWeights.root").c_str());
      wf.GetObject("ND_nu_HC280-HC293_ratio/LBNF_numu_flux", numode_280kA);
      if (!numode_280kA) {
        std::cout << "[ERROR]: Failed to read: "
                     "\"ND_nu_HC280-HC293_ratio/LBNF_numu_flux\" from "
                  << (ana::FindCAFAnaDir() + "/OnAxis4m280kAWeights.root")
                  << std::endl;
        throw;
      }
      numode_280kA->SetDirectory(nullptr);

      wf.GetObject("ND_nubar_HC280-HC293_ratio/LBNF_numubar_flux",
                   nubarmode_280kA);
      if (!nubarmode_280kA) {
        std::cout << "[ERROR]: Failed to read: "
                     "\"ND_nubar_HC280-HC293_ratio/LBNF_numubar_flux\" from "
                  << (ana::FindCAFAnaDir() + "/OnAxis4m280kAWeights.root")
                  << std::endl;
        throw;
      }
      nubarmode_280kA->SetDirectory(nullptr);
      if (gdc) {
        gdc->cd();
      }
      first = false;
    }

    return k280kAWeighter;
  } else {
    std::cout << "[ERROR]: Unknown special run type " << SRDescriptor
              << std::endl;
    abort();
  }
}

bool operator&(NuChan const &l, NuChan const &r) {
  return bool(static_cast<int>(l) & static_cast<int>(r));
}

std::ostream &operator<<(std::ostream &os, NuChan const &nc) {
  switch (nc) {
  case NuChan::kNumuApp: {
    return os << "kNumuApp";
  }
  case NuChan::kNumuBarApp: {
    return os << "kNumuBarApp";
  }
  case NuChan::kNueApp: {
    return os << "kNueApp";
  }
  case NuChan::kNueBarApp: {
    return os << "kNueBarApp";
  }
  case NuChan::kNumuIntrinsic: {
    return os << "kNumuIntrinsic";
  }
  case NuChan::kNumuBarIntrinsic: {
    return os << "kNumuBarIntrinsic";
  }
  case NuChan::kNueIntrinsic: {
    return os << "kNueIntrinsic";
  }
  case NuChan::kNueBarIntrinsic: {
    return os << "kNueBarIntrinsic";
  }
  case NuChan::kNumu: {
    return os << "kNumu";
  }
  case NuChan::kNumuBar: {
    return os << "kNumuBar";
  }
  case NuChan::kNue: {
    return os << "kNue";
  }
  case NuChan::kNueBar: {
    return os << "kNueBar";
  }
  case NuChan::kNumuNumuBar: {
    return os << "kNumuNumuBar";
  }
  case NuChan::kNueNueBar: {
    return os << "kNueNueBar";
  }
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, BeamMode const &bm) {
  switch (bm) {
  case BeamMode::kNuMode: {
    return os << "kNuMode";
  }
  case BeamMode::kNuBarMode: {
    return os << "kNuBarMode";
  }
  }
  return os;
}
std::ostream &operator<<(std::ostream &os, BeamChan const &bm) {
  return os << "{ " << bm.mode << ", " << bm.chan << " }"; 
}
bool operator==(BeamChan const &l, BeamChan const &r) {
  return (l.mode == r.mode) && (l.chan == r.chan);
}

bool operator<(BeamChan const &l, BeamChan const &r) {
  if (l.mode == r.mode) {
    return l.chan < r.chan;
  }
  return l.mode < r.mode;
}

bool operator<(MatchChan const &l, MatchChan const &r) {
  if (l.from == r.from) {
    return l.to < r.to;
  }
  return l.from < r.from;
}
std::ostream &operator<<(std::ostream &os, MatchChan const &mc) {
  return os << mc.from << " -> " << mc.to;
}

} // namespace PRISM
