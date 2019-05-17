#include "CAFAna/Core/XSecSystList.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <utility>

namespace ana {

enum FitSyst {
  kFitSyst,
  kRemovedOverconstraint,
  kRemovedOddResponse,
  kUsedAsFakeData
};

enum ExtrapolatedSyst { kExtrapolated, kContinuous };

static std::pair<double, double> const kStandardRange{-3, 3};
static std::pair<double, double> const kFakeDataRange{0, 1};

struct XSecDialDescriptor {
  std::string Name;
  FitSyst IsFitSyst;
  ExtrapolatedSyst IsExtrapolateOffToOnSyst;
  std::pair<double, double> FitLimits;
};

static std::vector<XSecDialDescriptor> const XSecSystDials = {
    {"MaCCQE", kFitSyst, kContinuous, kStandardRange},
    {"VecFFCCQEshape", kFitSyst, kExtrapolated, kStandardRange},
    {"MaNCEL", kFitSyst, kContinuous, kStandardRange},
    {"EtaNCEL", kRemovedOverconstraint, kContinuous, kStandardRange},
    {"MaCCRES", kFitSyst, kContinuous, kStandardRange},
    {"MvCCRES", kFitSyst, kContinuous, kStandardRange},
    {"MaNCRES", kFitSyst, kContinuous, kStandardRange},
    {"MvNCRES", kFitSyst, kContinuous, kStandardRange},
    {"RDecBR1gamma", kRemovedOverconstraint, kContinuous, {-2, 3}},
    {"RDecBR1eta", kRemovedOverconstraint, kContinuous, {-2, 3}},
    {"Theta_Delta2Npi", kFitSyst, kExtrapolated, kStandardRange},
    {"AhtBY", kFitSyst, kContinuous, kStandardRange},
    {"BhtBY", kFitSyst, kContinuous, kStandardRange},
    {"CV1uBY", kFitSyst, kContinuous, kStandardRange},
    {"CV2uBY", kFitSyst, kContinuous, kStandardRange},
    {"FormZone", kRemovedOddResponse, kContinuous, {-2, 2}},
    {"MFP_pi", kRemovedOddResponse, kContinuous, {-2, 2}},
    {"FrCEx_pi", kFitSyst, kContinuous, {-2, 2}},
    {"FrElas_pi", kFitSyst, kContinuous, {-2, 2}},
    {"FrInel_pi", kFitSyst, kContinuous, {-2, 2}},
    {"FrAbs_pi", kFitSyst, kContinuous, {-2, 2}},
    {"FrPiProd_pi", kFitSyst, kContinuous, {-2, 2}},
    {"MFP_N", kRemovedOddResponse, kContinuous, {-2, 2}},
    {"FrCEx_N", kFitSyst, kContinuous, {-2, 2}},
    {"FrElas_N", kFitSyst, kContinuous, {-2, 2}},
    {"FrInel_N", kFitSyst, kContinuous, {-2, 2}},
    {"FrAbs_N", kFitSyst, kContinuous, {-2, 2}},
    {"FrPiProd_N", kFitSyst, kContinuous, {-2, 2}},
    {"CCQEPauliSupViaKF", kFitSyst, kExtrapolated, kStandardRange},
    {"Mnv2p2hGaussEnhancement", kUsedAsFakeData, kContinuous, kFakeDataRange},
    {"MKSPP_ReWeight", kFitSyst, kContinuous, kFakeDataRange},
    {"E2p2h_A_nu", kFitSyst, kExtrapolated, kStandardRange},
    {"E2p2h_B_nu", kFitSyst, kExtrapolated, kStandardRange},
    {"E2p2h_A_nubar", kFitSyst, kExtrapolated, kStandardRange},
    {"E2p2h_B_nubar", kFitSyst, kExtrapolated, kStandardRange},
    {"NR_nu_n_CC_2Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nu_n_CC_3Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nu_p_CC_2Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nu_p_CC_3Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nu_np_CC_1Pi", kFitSyst, kContinuous, kStandardRange},
    {"NR_nu_n_NC_1Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nu_n_NC_2Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nu_n_NC_3Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nu_p_NC_1Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nu_p_NC_2Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nu_p_NC_3Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nubar_n_CC_1Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nubar_n_CC_2Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nubar_n_CC_3Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nubar_p_CC_1Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nubar_p_CC_2Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nubar_p_CC_3Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nubar_n_NC_1Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nubar_n_NC_2Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nubar_n_NC_3Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nubar_p_NC_1Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nubar_p_NC_2Pi", kFitSyst, kContinuous, {-2, 3}},
    {"NR_nubar_p_NC_3Pi", kFitSyst, kContinuous, {-2, 3}},
    {"BeRPA_A", kFitSyst, kContinuous, kStandardRange},
    {"BeRPA_B", kFitSyst, kContinuous, kStandardRange},
    {"BeRPA_D", kFitSyst, kContinuous, kStandardRange},
    {"BeRPA_E", kRemovedOverconstraint, kContinuous, kStandardRange},
    {"C12ToAr40_2p2hScaling_nu", kFitSyst, kExtrapolated, kStandardRange},
    {"C12ToAr40_2p2hScaling_nubar", kFitSyst, kExtrapolated, kStandardRange},
    {"nuenuebar_xsec_ratio", kFitSyst, kExtrapolated, kStandardRange},
    {"nuenumu_xsec_ratio", kFitSyst, kExtrapolated, kStandardRange},
    {"SPPLowQ2Suppression", kUsedAsFakeData, kContinuous, kFakeDataRange},
    {"FSILikeEAvailSmearing", kUsedAsFakeData, kContinuous, kFakeDataRange}};

/// All dial names that should go into a state file
std::vector<std::string> const &GetAllXSecSystNames() {
  static std::vector<std::string> XSecSystNames;
  static bool first = true;
  if (first) {
    first = false;
    for (auto const &s : XSecSystDials) {
      XSecSystNames.push_back(s.Name);
    }
  }

  return XSecSystNames;
}
/// All dials used as freedoms in standard fits
std::vector<std::string> const &GetFitSystNames() {
  static std::vector<std::string> XSecSystNames;
  static bool first = true;
  if (first) {
    first = false;
    for (auto const &s : XSecSystDials) {
      if (s.IsFitSyst == kFitSyst) {
        XSecSystNames.push_back(s.Name);
      }
    }
  }

  return XSecSystNames;
}
/// Dials which should not be used as freedoms (CV weights if they exist removed
/// in SpectrumLoader)
std::vector<std::string> const &GetDoNotIncludeSystNames() {
  static std::vector<std::string> XSecSystNames;
  static bool first = true;
  if (first) {
    first = false;
    for (auto const &s : XSecSystDials) {
      if (s.IsFitSyst != kFitSyst) {
        XSecSystNames.push_back(s.Name);
      }
    }
  }

  return XSecSystNames;
}
/// Dials which have an extrapolated response outside of 0->1
std::vector<std::string> const &GetExtrapolateOffToOnSystNames() {
  static std::vector<std::string> XSecSystNames;
  static bool first = true;
  if (first) {
    first = false;
    for (auto const &s : XSecSystDials) {
      if (s.IsExtrapolateOffToOnSyst == kExtrapolated) {
        XSecSystNames.push_back(s.Name);
      }
    }
  }

  return XSecSystNames;
}
/// Dials used to generate fake data sets when set to 1, should not be fit.
std::vector<std::string> const &GetFakeDataGenerationSystNames() {
  static std::vector<std::string> XSecSystNames;
  static bool first = true;
  if (first) {
    first = false;
    for (auto const &s : XSecSystDials) {
      if (s.IsFitSyst == kUsedAsFakeData) {
        XSecSystNames.push_back(s.Name);
      }
    }
  }

  return XSecSystNames;
}

int GetXSecSystIndex(std::string const &name) {

  static std::map<std::string, int> cache;
  if (!cache.count(name)) {
    int found = -1;
    for (size_t it = 0; it < XSecSystDials.size(); ++it) {
      if (XSecSystDials[it].Name == name) {
        found = it;
        break;
      }
    }
    assert(found > -1);
    cache[name] = found;
  }
  return cache[name];
}
std::string GetXSecSystName(int index) {
  assert((index >= 0) && (index < int(XSecSystDials.size())));
  return XSecSystDials[index].Name;
}

/// Check if a dial name is in a list
bool SystNameIsInList(std::string const &name,
                      std::vector<std::string> const &listOfNames) {
  return (std::find(listOfNames.begin(), listOfNames.end(), name) !=
          listOfNames.end());
}
/// Convenience method for checking if a dial is
bool IsExtrapolateOffToOnSyst(std::string const &name) {
  static std::map<std::string, bool> cache;
  if (!cache.count(name)) {
    cache[name] = SystNameIsInList(name, GetExtrapolateOffToOnSystNames());
  }
  return cache[name];
}
bool IsExtrapolateOffToOnSyst(int index) {
  static std::map<int, bool> cache;
  if (!cache.count(index)) {
    cache[index] = IsExtrapolateOffToOnSyst(GetXSecSystName(index));
  }
  return cache[index];
}

bool IsDoNotIncludeSyst(std::string const &name) {
  static std::map<std::string, bool> cache;
  if (!cache.count(name)) {
    cache[name] = SystNameIsInList(name, GetDoNotIncludeSystNames());
  }
  return cache[name];
}
bool IsDoNotIncludeSyst(int index) {
  static std::map<int, bool> cache;
  if (!cache.count(index)) {
    cache[index] = IsDoNotIncludeSyst(GetXSecSystName(index));
  }
  return cache[index];
}

std::pair<double, double> GetXSecSystLimits(int index) {
  assert((index >= 0) && (index < int(XSecSystDials.size())));
  return XSecSystDials[index].FitLimits;
}
std::pair<double, double> GetXSecSystLimits(const std::string &name) {
  return GetXSecSystLimits(GetXSecSystIndex(name));
}
double GetXSecSystMax(int index) { return GetXSecSystLimits(index).second; }
double GetXSecSystMin(int index) { return GetXSecSystLimits(index).first; }

} // namespace ana
