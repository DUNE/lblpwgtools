#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include <string>
#include <vector>
#include <algorithm>

namespace ana {

class HighEFluxModDial : public ISyst {
  public:

    virtual ~HighEFluxModDial(){};
    
    void Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr,
	       double &weight) const override;
    
  protected:
    HighEFluxModDial(std::string name, bool applyPenalty = false);

    friend std::vector<const ISyst *> GetHighEFluxModSysts(std::vector<std::string>, bool);

    std::string fName;
    
  };

  std::vector<std::string> GetHighEFluxModDialNames();

  bool IsHighEFluxModSyst(std::string name);

  std::vector<const ISyst *>
    GetHighEFluxModSysts(std::vector<std::string> names = {}, bool applyPenalty = false);
  
} // namespace ana
