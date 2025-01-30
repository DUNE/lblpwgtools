#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include <string>
#include <vector>
#include <algorithm>

namespace ana {

class CrazyFluxDial : public ISyst {
  public:

    virtual ~CrazyFluxDial(){};
    
    void Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr,
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
  
} // namespace ana
