#pragma once

#include "CAFAna/Core/ISyst.h"
#include "XSecSystList.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include <cassert>
#include <cmath>
#include <iostream>

namespace ana {
#ifdef BUILD_XSEC_SYSTS

  class XSecSyst : public ISyst {
  public:
    virtual ~XSecSyst(){};

    void FakeDataDialShift(double sigma,
                           caf::SRProxy *sr, double &weight) const;

    void Shift(double sigma, caf::SRProxy *sr,
               double &weight) const override;

  protected:
    XSecSyst(int syst_id, bool applyPenalty = true);

    friend std::vector<const ISyst *> GetXSecSysts(std::vector<std::string>, bool);

    int fID;
  };

  std::vector<const ISyst *>
  GetXSecSysts(std::vector<std::string> names = {}, bool applyPenalty = true);

#endif
} // namespace ana
