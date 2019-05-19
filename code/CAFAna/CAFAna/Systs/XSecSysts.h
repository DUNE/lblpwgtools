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

  void FakeDataDialShift(double sigma, Restorer &restore,
                         caf::StandardRecord *sr, double &weight) const;

  void Shift(double sigma, Restorer &restore, caf::StandardRecord *sr,
             double &weight) const override;

protected:
  XSecSyst(int syst_id, bool applyPenalty = true);

  friend std::vector<const ISyst *> GetXSecSysts(std::vector<std::string>, bool);

  int fID;
};

std::vector<const ISyst *>
GetXSecSysts(std::vector<std::string> names = {}, bool applyPenalty = true);

} // namespace ana
