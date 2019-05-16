#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/XSecSystList.h"
#include "StandardRecord/StandardRecord.h"
#include <cassert>
#include <cmath>
#include <iostream>

namespace ana {
class XSecSyst : public ISyst {
public:
  virtual ~XSecSyst(){};

  void Shift(double sigma, Restorer &restore, caf::StandardRecord *sr,
             double &weight) const override {
    // No xs weights in this event, skip reweighting it
    if (sr->dune.xsSyst_wgt.empty())
      return;
    if (sr->dune.xsSyst_wgt[0].empty())
      return;

    assert(std::abs(sigma) <= 3 &&
           "XSecs responses only valid up to +/-3 sigma!");

    double osigma = sigma;
    if (fNegateNegativeSigmaShifts && (osigma < 0)) {
      sigma = fabs(sigma);
    }

    // How far apart are the points
    const double spacing = 1;

    // Get the top and bottom values in the array
    int low_index = std::floor(sigma / spacing) + 3;
    int high_index = std::ceil(sigma / spacing) + 3;

    double diff = (sigma - double(low_index)) / spacing;

    double low_weight = sr->dune.xsSyst_wgt[fID][low_index];
    double high_weight = sr->dune.xsSyst_wgt[fID][high_index];

    double fact = 1;
    if (fNegateNegativeSigmaShifts && (osigma < 0)) {

      double dx = sigma;
      double dy = ((low_weight + (high_weight - low_weight) * diff) - 1);

      fact = 1 + (dy / dx) * osigma;

    } else {
      fact = low_weight + (high_weight - low_weight) * diff;
    }

    fact = std::min(fact, 10.);
    weight *= fact;
  }

protected:
  XSecSyst(int genie_id, bool applyPenalty = true,
           bool NegateNegativeSigmaShifts = false)
      : ISyst(GetXSecSystName(genie_id), GetXSecSystName(genie_id),
              applyPenalty, GetXSecSystMin(genie_id), GetXSecSystMax(genie_id)),
        fID(genie_id), fNegateNegativeSigmaShifts(NegateNegativeSigmaShifts) {}
  friend std::vector<const ISyst *> GetXSecSysts(std::vector<std::string>, bool,
                                                 bool);

  int fID;
  bool fNegateNegativeSigmaShifts;
};

std::vector<const ISyst *>
GetXSecSysts(std::vector<std::string> names = {}, bool applyPenalty = true,
             bool NegateNegativeSigmaShifts = false) {
  static std::vector<const ISyst *> ret;

  if (names.empty()) {
    names = GetAllXSecSystNames();
  }

  if (ret.empty()) {
    for (auto &it : names) {
      ret.push_back(new XSecSyst(GetXSecSystIndex(it), applyPenalty,
                                 NegateNegativeSigmaShifts &&
                                     IsExtrapolateOffToOnSyst(it)));
    }
  }
  return ret;
}

} // namespace ana
