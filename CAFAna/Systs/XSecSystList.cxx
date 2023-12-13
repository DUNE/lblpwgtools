#include "CAFAna/Systs/XSecSystList.h"

#include "CAFAna/Systs/XSecSysts.h"

#include <iostream>
#include <unordered_map>
#include <utility>

namespace ana {
//const std::vector<const ISyst*> kAllXsecSysts = GetXSecSysts();
//
/////\\\///\\\///\\\/// Get a specific list
//std::vector<std::string> GetXSecSystNames(std::string const &inputCategg) {
//
//  if (inputCategg == "All") {
//    return GetFitSystNames();
//  }
//
//  std::vector<std::string> XSecSystNames;
//  XSecCategg thisCategg = kNahMate;
//  if (inputCategg == "QELike")
//    thisCategg = kQELike;
//  if (inputCategg == "RES")
//    thisCategg = kRES;
//  if (inputCategg == "DIS")
//    thisCategg = kDIS;
//  if (inputCategg == "FSI")
//    thisCategg = kFSI;
//  if (inputCategg == "Ratios")
//    thisCategg = kRatios;
//  if (inputCategg == "NC")
//    thisCategg = kNC;
//  if (inputCategg == "Other")
//    thisCategg = kOther;
//
//  static auto XSecDials = GetAllXSecDials();
//  for (auto const &s : XSecDials) {
//    if (s.IsFitSyst != kFitSyst) {
//      continue;
//    }
//    if (s.Categg == thisCategg) {
//      XSecSystNames.push_back(s.Name);
//    } // Just for you Luke ;p
//  }
//
//  return XSecSystNames;
//}
//
///// All dials used as freedoms in standard fits
//std::vector<std::string> const &GetFitSystNames() {
//  static std::vector<std::string> XSecSystNames;
//  static bool first = true;
//  if (first) {
//    first = false;
//    for (auto const &s : GetAllXSecDials()) {
//      if (s.IsFitSyst == kFitSyst) {
//        XSecSystNames.push_back(s.Name);
//      }
//    }
//  }
//
//  return XSecSystNames;
//}
//
///// Dials which have an extrapolated response outside of 0->1
//std::vector<std::string> const &GetExtrapolateOffToOnSystNames() {
//  static std::vector<std::string> XSecSystNames;
//  static bool first = true;
//  if (first) {
//    first = false;
//    for (auto const &s : GetAllXSecDials()) {
//      if (s.IsExtrapolateOffToOnSyst == kExtrapolated) {
//        XSecSystNames.push_back(s.Name);
//      }
//    }
//  }
//
//  return XSecSystNames;
//}
///// Dials used to generate fake data sets when set to 1, should not be fit.
//std::vector<std::string> const &GetFakeDataGenerationSystNames() {
//  static std::vector<std::string> XSecSystNames;
//  static bool first = true;
//  if (first) {
//    first = false;
//    for (auto const &s : GetAllXSecDials()) {
//      if (s.IsFitSyst == kUsedAsFakeData) {
//        XSecSystNames.push_back(s.Name);
//      }
//    }
//  }
//
//  return XSecSystNames;
//}
//
//int GetXSecSystIndex(std::string const &name) {
//  static auto XSecDials = GetAllXSecDials();
//
//  static std::map<std::string, int> cache;
//  if (!cache.count(name)) {
//    int found = -1;
//    for (size_t it = 0; it < XSecDials.size(); ++it) {
//      if (XSecDials[it].Name == name) {
//        found = it;
//        break;
//      }
//    }
//    assert(found > -1);
//    cache[name] = found;
//  }
//  return cache[name];
//}
//
///// Convenience method for checking if a dial is
//bool IsExtrapolateOffToOnSyst(std::string const &name) {
//  static std::unordered_map<std::string, bool> cache;
//  if (!cache.count(name)) {
//    cache[name] = SystNameIsInList(name, GetExtrapolateOffToOnSystNames());
//  }
//  return cache[name];
//}
//bool IsExtrapolateOffToOnSyst(int index) {
//  static std::unordered_map<int, bool> cache;
//  if (!cache.count(index)) {
//    cache[index] = IsExtrapolateOffToOnSyst(GetXSecSystName(index));
//  }
//  return cache[index];
//}
//
//bool IsFakeDataGenerationSyst(std::string const &name) {
//  static std::unordered_map<std::string, bool> cache;
//  if (!cache.count(name)) {
//    cache[name] = SystNameIsInList(name, GetFakeDataGenerationSystNames());
//  }
//  return cache[name];
//}
//bool IsFakeDataGenerationSyst(int index) {
//  static std::unordered_map<int, bool> cache;
//  if (!cache.count(index)) {
//    cache[index] = IsFakeDataGenerationSyst(GetXSecSystName(index));
//  }
//  return cache[index];
//}
//
//std::pair<double, double> GetXSecSystLimits(int index) {
//  static auto XSecDials = GetAllXSecDials();
//  assert((index >= 0) && (index < int(XSecDials.size())));
//  return XSecDials[index].FitLimits;
//}
//std::pair<double, double> GetXSecSystLimits(const std::string &name) {
//  return GetXSecSystLimits(GetXSecSystIndex(name));
//}
//double GetXSecSystMax(int index) { return GetXSecSystLimits(index).second; }
//double GetXSecSystMin(int index) { return GetXSecSystLimits(index).first; }

} // namespace ana
