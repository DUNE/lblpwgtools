#pragma once

#include "CAFAna/Core/GenieWeightList.h"
#include "CAFAna/Core/ISyst.h"
#include "StandardRecord/StandardRecord.h"
#include <cassert>
#include <cmath>
#include <iostream>

namespace ana {
class GenieSyst : public ISyst {
public:
  virtual ~GenieSyst(){};

  void Shift(double sigma, Restorer &restore, caf::StandardRecord *sr,
             double &weight) const override
  {
    // No GENIE weights in this event, skip reweighting it
    if(sr->dune.genie_wgt.empty()) return;
    if(sr->dune.genie_wgt[0].empty()) return;

    assert(std::abs(sigma) <= 3 && "GENIE XSECs only valid up to +/-3 sigma!");

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

    double low_weight = sr->dune.genie_wgt[fID][low_index];
    double high_weight = sr->dune.genie_wgt[fID][high_index];

    double fact = 1;
    if (fNegateNegativeSigmaShifts && (osigma < 0)) {

      double dx = sigma;
      double dy = ((low_weight + (high_weight - low_weight) * diff) - 1);

      fact = 1 + (dy / dx) * osigma;

      // std::cout << ShortName() << " sigma = " << osigma << " weight = "
      //           << (low_weight + (high_weight - low_weight) * diff)
      //           << " replaced with weight of " << fact << std::endl;
    } else {
      fact = low_weight + (high_weight - low_weight) * diff;
    }

    // if (fact < 0) {
    //   std::cout << "[INFO]: " << ShortName() << " Bad Weight = " << fact
    //             << " @ sigma = " << osigma << std::endl;
    //   fact = 0;
    // }
    fact = std::min(fact, 10.);
    weight *= fact;
  }

protected:
  GenieSyst(int genie_id, bool applyPenalty = true,
            bool NegateNegativeSigmaShifts = false)
      : ISyst(GetGenieWeightName(genie_id), GetGenieWeightName(genie_id),
              applyPenalty, GetGenieMin(genie_id), GetGenieMax(genie_id)),
        fID(genie_id), fNegateNegativeSigmaShifts(NegateNegativeSigmaShifts) {}
  friend std::vector<const ISyst *> GetGenieSysts(std::vector<std::string>,
                                                  bool, bool);

  int fID;
  bool fNegateNegativeSigmaShifts;
};

std::vector<const ISyst *>
GetGenieSysts(std::vector<std::string> names = {}, bool applyPenalty = true,
              bool NegateNegativeSigmaShifts = false) {
  static std::vector<const ISyst *> ret;

  if (names.empty())
    names = GetGenieWeightNames();

  if (ret.empty()) {
    for (auto &it : names) {
      ret.push_back(
          new GenieSyst(GetGenieIndex(it), applyPenalty,
                        NegateNegativeSigmaShifts && GetGenieIsOnOff(it)));
    }
  }
  return ret;
}

} // namespace ana
