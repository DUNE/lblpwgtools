#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include <string>
#include <vector>
#include <algorithm>

namespace ana {
#ifdef BUILD_FLUX_SYSTS

class CrazyFluxDial : public ISyst {
  public:

    virtual ~CrazyFluxDial(){};

    void Shift(double sigma, caf::SRProxy *sr,
	       double &weight) const override;

  protected:
    CrazyFluxDial(std::string name, bool applyPenalty = false);

    friend std::vector<const ISyst *> GetCrazyFluxSysts(std::vector<std::string>, bool);

    std::string fName;

  };

  std::vector<std::string> GetCrazyFluxDialNames();

  bool IsCrazyFluxSyst(std::string name);

  std::vector<const ISyst *>
    GetCrazyFluxSysts(std::vector<std::string> names = {}, bool applyPenalty = false);

#endif   // BUILD_FLUX_SYSTS
} // namespace ana
