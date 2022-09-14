#pragma once

#include "CAFAna/Analysis/AnalysisVersion.h"

#include <algorithm>
#include <cassert>
#include <map>
#include <string>
#include <vector>

namespace ana {
class ISyst;

enum FitSyst {
  kFitSyst,
  kRemovedOverconstraint,
  kRemovedOddResponse,
  kRemovedOther,
  kUsedAsFakeData
};

enum XSecCategg { kQELike, kRES, kDIS, kFSI, kRatios, kNC, kOther, kNahMate };

enum ExtrapolatedSyst { kExtrapolated, kContinuous };

static std::pair<double, double> const kStandardRange{-3, 3};
static std::pair<double, double> const kFakeDataRange{0, 1};

struct XSecDialDescriptor {
  std::string Name;
  FitSyst IsFitSyst;
  ExtrapolatedSyst IsExtrapolateOffToOnSyst;
  std::pair<double, double> FitLimits;
  XSecCategg Categg;
};

/// All dial names that should go into a state file
inline std::vector<XSecDialDescriptor> const &GetAllXSecDials() {
  static std::vector<XSecDialDescriptor> XSecSystDials;
  static bool first = true;
  if (first) {

    first = false;
    XSecSystDials = {
        {"MaCCQE", kFitSyst, kContinuous, kStandardRange, kQELike},
        {"VecFFCCQEshape", kFitSyst, kExtrapolated, kStandardRange, kQELike},
        {"MaNCEL", kUsedAsFakeData, kContinuous, kStandardRange, kNC},
        {"EtaNCEL", kRemovedOverconstraint, kContinuous, kStandardRange, kNC},
        {"MaCCRES", kFitSyst, kContinuous, kStandardRange, kRES},
        {"MvCCRES", kFitSyst, kContinuous, kStandardRange, kRES},
        {"MaNCRES", kFitSyst, kContinuous, kStandardRange, kNC},
        {"MvNCRES", kFitSyst, kContinuous, kStandardRange, kNC},
        {"RDecBR1gamma", kRemovedOverconstraint, kContinuous, {-2, 3}, kRES},
        {"RDecBR1eta", kRemovedOverconstraint, kContinuous, {-2, 3}, kRES},
        {"Theta_Delta2Npi", kFitSyst, kExtrapolated, kStandardRange, kDIS},
        {"AhtBY", kFitSyst, kContinuous, kStandardRange, kDIS},
        {"BhtBY", kFitSyst, kContinuous, kStandardRange, kDIS},
        {"CV1uBY", kFitSyst, kContinuous, kStandardRange, kDIS},
        {"CV2uBY", kFitSyst, kContinuous, kStandardRange, kDIS},
        {"FormZone", kUsedAsFakeData, kContinuous, {-2, 2}, kFSI},
        {"MFP_pi", kUsedAsFakeData, kContinuous, {-2, 2}, kFSI},
        {"FrCEx_pi", kFitSyst, kContinuous, {-2, 2}, kFSI},
        {"FrElas_pi", kFitSyst, kContinuous, {-2, 2}, kFSI},
        {"FrInel_pi", kFitSyst, kContinuous, {-2, 2}, kFSI},
        {"FrAbs_pi", kFitSyst, kContinuous, {-2, 2}, kFSI},
        {"FrPiProd_pi", kFitSyst, kContinuous, {-2, 2}, kFSI},
        {"MFP_N", kUsedAsFakeData, kContinuous, {-2, 2}, kFSI},
        {"FrCEx_N", kFitSyst, kContinuous, {-2, 2}, kFSI},
        {"FrElas_N", kFitSyst, kContinuous, {-2, 2}, kFSI},
        {"FrInel_N", kFitSyst, kContinuous, {-2, 2}, kFSI},
        {"FrAbs_N", kFitSyst, kContinuous, {-2, 2}, kFSI},
        {"FrPiProd_N", kFitSyst, kContinuous, {-2, 2}, kFSI},
        {"CCQEPauliSupViaKF", kFitSyst, kExtrapolated, kStandardRange, kQELike},
        // Must be here to fill branch in SpectrumLoader but shouldn't be used
        // as the dials below separate out the responses into multiple fake data
        // sets.
        {"Mnv2p2hGaussEnhancement",
         kRemovedOther,
         kContinuous,
         {0, 0},
         kQELike},
        {"Mnv2p2hGaussEnhancement_NN", kUsedAsFakeData, kContinuous,
         kFakeDataRange, kQELike},
        {"Mnv2p2hGaussEnhancement_2p2h", kUsedAsFakeData, kContinuous,
         kFakeDataRange, kQELike},
        {"Mnv2p2hGaussEnhancement_1p1h", kUsedAsFakeData, kContinuous,
         kFakeDataRange, kQELike},
        {"MKSPP_ReWeight", kUsedAsFakeData, kContinuous, kFakeDataRange, kRES},
        {"E2p2h_A_nu", kFitSyst, kExtrapolated, kStandardRange, kQELike},
        {"E2p2h_B_nu", kFitSyst, kExtrapolated, kStandardRange, kQELike},
        {"E2p2h_A_nubar", kFitSyst, kExtrapolated, kStandardRange, kQELike},
        {"E2p2h_B_nubar", kFitSyst, kExtrapolated, kStandardRange, kQELike},
        {"NR_nu_n_CC_2Pi", kFitSyst, kContinuous, {-2, 3}, kDIS},
        {"NR_nu_n_CC_3Pi", kFitSyst, kContinuous, {-2, 3}, kDIS},
        {"NR_nu_p_CC_2Pi", kFitSyst, kContinuous, {-2, 3}, kDIS},
        {"NR_nu_p_CC_3Pi", kFitSyst, kContinuous, {-2, 3}, kDIS},
        {"NR_nu_np_CC_1Pi", kFitSyst, kContinuous, kStandardRange, kDIS},
        {"NR_nu_n_NC_1Pi", kFitSyst, kContinuous, {-2, 3}, kNC},
        {"NR_nu_n_NC_2Pi", kFitSyst, kContinuous, {-2, 3}, kNC},
        {"NR_nu_n_NC_3Pi", kFitSyst, kContinuous, {-2, 3}, kNC},
        {"NR_nu_p_NC_1Pi", kFitSyst, kContinuous, {-2, 3}, kNC},
        {"NR_nu_p_NC_2Pi", kFitSyst, kContinuous, {-2, 3}, kNC},
        {"NR_nu_p_NC_3Pi", kFitSyst, kContinuous, {-2, 3}, kNC},
        {"NR_nubar_n_CC_1Pi", kFitSyst, kContinuous, {-2, 3}, kDIS},
        {"NR_nubar_n_CC_2Pi", kFitSyst, kContinuous, {-2, 3}, kDIS},
        {"NR_nubar_n_CC_3Pi", kFitSyst, kContinuous, {-2, 3}, kDIS},
        {"NR_nubar_p_CC_1Pi", kFitSyst, kContinuous, {-2, 3}, kDIS},
        {"NR_nubar_p_CC_2Pi", kFitSyst, kContinuous, {-2, 3}, kDIS},
        {"NR_nubar_p_CC_3Pi", kFitSyst, kContinuous, {-2, 3}, kDIS},
        {"NR_nubar_n_NC_1Pi", kFitSyst, kContinuous, {-2, 3}, kNC},
        {"NR_nubar_n_NC_2Pi", kFitSyst, kContinuous, {-2, 3}, kNC},
        {"NR_nubar_n_NC_3Pi", kFitSyst, kContinuous, {-2, 3}, kNC},
        {"NR_nubar_p_NC_1Pi", kFitSyst, kContinuous, {-2, 3}, kNC},
        {"NR_nubar_p_NC_2Pi", kFitSyst, kContinuous, {-2, 3}, kNC},
        {"NR_nubar_p_NC_3Pi", kFitSyst, kContinuous, {-2, 3}, kNC},
        {"BeRPA_A", kFitSyst, kContinuous, kStandardRange, kQELike},
        {"BeRPA_B", kFitSyst, kContinuous, kStandardRange, kQELike},
        {"BeRPA_D", kFitSyst, kContinuous, kStandardRange, kQELike},
        {"BeRPA_E", kUsedAsFakeData, kContinuous, kStandardRange, kQELike},
        {"C12ToAr40_2p2hScaling_nu", kFitSyst, kExtrapolated, kStandardRange,
         kQELike},
        {"C12ToAr40_2p2hScaling_nubar", kFitSyst, kExtrapolated, kStandardRange,
         kQELike},
        {"nuenuebar_xsec_ratio", kFitSyst, kExtrapolated, kStandardRange,
         kRatios},
        {"nuenumu_xsec_ratio", kFitSyst, kExtrapolated, kStandardRange,
         kRatios},
        {"SPPLowQ2Suppression", kUsedAsFakeData, kContinuous, kFakeDataRange,
         kRES},
        {"FSILikeEAvailSmearing", kUsedAsFakeData, kContinuous, kFakeDataRange,
         kFSI},
        {"MissingProtonFakeData", kUsedAsFakeData, kContinuous, kFakeDataRange,
         kOther},
        {"NuWroReweightFakeData", kUsedAsFakeData, kContinuous, kFakeDataRange,
         kOther}};

    static auto AnaV = GetAnaVersion();
    if (AnaV == kV3) {
      std::string dial_name;
      auto sel = [&](XSecDialDescriptor const &d) -> bool {
        return d.Name == dial_name;
      };

      dial_name = "Mnv2p2hGaussEnhancement";
      auto mnv_dial =
          std::find_if(XSecSystDials.begin(), XSecSystDials.end(), sel);
      mnv_dial->IsFitSyst = kFitSyst;
      mnv_dial->IsExtrapolateOffToOnSyst = kExtrapolated;
      mnv_dial->FitLimits = kStandardRange;

      dial_name = "Mnv2p2hGaussEnhancement_NN";
      auto mnv_dial_NN =
          std::find_if(XSecSystDials.begin(), XSecSystDials.end(), sel);
      mnv_dial_NN->IsFitSyst = kRemovedOther;
      mnv_dial_NN->FitLimits = {0, 0};

      dial_name = "Mnv2p2hGaussEnhancement_2p2h";
      auto mnv_dial_2p2h =
          std::find_if(XSecSystDials.begin(), XSecSystDials.end(), sel);
      mnv_dial_2p2h->IsFitSyst = kRemovedOther;
      mnv_dial_2p2h->FitLimits = {0, 0};

      dial_name = "Mnv2p2hGaussEnhancement_1p1h";
      auto mnv_dial_1p1h =
          std::find_if(XSecSystDials.begin(), XSecSystDials.end(), sel);
      mnv_dial_1p1h->IsFitSyst = kRemovedOther;
      mnv_dial_1p1h->FitLimits = {0, 0};

      dial_name = "MKSPP_ReWeight";
      auto mk_dial =
          std::find_if(XSecSystDials.begin(), XSecSystDials.end(), sel);
      mk_dial->IsFitSyst = kFitSyst;
      mk_dial->IsExtrapolateOffToOnSyst = kExtrapolated;
      mk_dial->FitLimits = kStandardRange;

      dial_name = "SPPLowQ2Suppression";
      auto spplowq2 =
          std::find_if(XSecSystDials.begin(), XSecSystDials.end(), sel);
      spplowq2->IsFitSyst = kFitSyst;
      spplowq2->IsExtrapolateOffToOnSyst = kExtrapolated;
      spplowq2->FitLimits = kStandardRange;

      dial_name = "FSILikeEAvailSmearing";
      auto fslilikesmear =
          std::find_if(XSecSystDials.begin(), XSecSystDials.end(), sel);
      fslilikesmear->IsFitSyst = kFitSyst;
      fslilikesmear->IsExtrapolateOffToOnSyst = kExtrapolated;
      fslilikesmear->FitLimits = kStandardRange;
    }
  }

  return XSecSystDials;
}

/// All dial names that should go into a state file
inline std::vector<std::string> const &GetAllXSecSystNames() {
  static std::vector<std::string> XSecSystNames;
  static bool first = true;
  if (first) {
    first = false;
    for (auto const &s : GetAllXSecDials()) {
      XSecSystNames.push_back(s.Name);
    }
  }

  return XSecSystNames;
}
/// Return a specific category of XSEC systs
std::vector<std::string>
GetXSecSystNames(std::string const &inputCategg = "All");
/// All dials used as freedoms in standard fits
std::vector<std::string> const &GetFitSystNames();
/// Dials which should not be used as freedoms (CV weights if they exist removed
/// in SpectrumLoader)
inline std::vector<std::string> const &GetDoNotIncludeSystNames() {
  static std::vector<std::string> XSecSystNames;
  static bool first = true;
  if (first) {
    first = false;
    for (auto const &s : GetAllXSecDials()) {
      if (s.IsFitSyst != kFitSyst) {
        XSecSystNames.push_back(s.Name);
      }
    }
  }

  return XSecSystNames;
}
/// Dials which have an extrapolated response outside of 0->1
std::vector<std::string> const &GetExtrapolateOffToOnSystNames();
/// Dials used to generate fake data sets when set to 1, should not be fit.
std::vector<std::string> const &GetFakeDataGenerationSystNames();

int GetXSecSystIndex(std::string const &name);
inline std::string GetXSecSystName(int index) {
  static auto XSecDials = GetAllXSecDials();
  assert((index >= 0) && (index < int(XSecDials.size())));
  return XSecDials[index].Name;
}

/// Check if a dial name is in a list
inline bool SystNameIsInList(std::string const &name,
                             std::vector<std::string> const &listOfNames) {
  return (std::find(listOfNames.begin(), listOfNames.end(), name) !=
          listOfNames.end());
}

/// Convenience method for checking if a dial name is
bool IsExtrapolateOffToOnSyst(std::string const &);
bool IsExtrapolateOffToOnSyst(int);

bool IsDoNotIncludeSyst(std::string const &);
bool IsDoNotIncludeSyst(unsigned int);

bool IsFakeDataGenerationSyst(std::string const &);
bool IsFakeDataGenerationSyst(int);

std::pair<double, double> GetXSecSystLimits(const std::string &name);
double GetXSecSystMax(int index);
double GetXSecSystMin(int index);

// actually instantiate them so that the Registry knows about them
extern const std::vector<const ISyst *> kAllXsecSysts;

} // namespace ana
