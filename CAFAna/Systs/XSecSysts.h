#pragma once

#include "CAFAna/Core/ISyst.h"
#include "XSecSystList.h"

#include "StandardRecord/SRProxy.h"

#include <cassert>
#include <cmath>
#include <iostream>

namespace ana {

class XSecSyst : public ISyst {
public:
  virtual ~XSecSyst(){};

  void FakeDataDialShift(double sigma, Restorer &restore,
                         caf::SRProxy *sr, double &weight) const;

  void Shift(double sigma, Restorer &restore, caf::SRProxy *sr,
             double &weight) const override;

protected:
  XSecSyst(int syst_id, bool applyPenalty = true);

  friend std::vector<const ISyst *> GetXSecSysts(std::vector<std::string>, bool);

  int fID;
};

std::vector<const ISyst *>
GetXSecSysts(std::vector<std::string> names = {}, bool applyPenalty = true);

} // namespace ana
