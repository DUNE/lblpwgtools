#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include <string>
#include <vector>
#include <algorithm>

#include "TF1.h"

namespace ana {

class ProtonNeutronEnergyShiftDial : public ISyst {
  public:

    virtual ~ProtonNeutronEnergyShiftDial(){};
    
    void Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr,
	       double &weight) const override;
    
  protected:
    ProtonNeutronEnergyShiftDial(std::string name, bool applyPenalty = false)
      : ISyst(name, name, applyPenalty),
      fName(name) {}

    friend std::vector<const ISyst *> GetProtonNeutronEnergyShiftSysts(std::vector<std::string>, bool);

    std::string fName;
};

 std::vector<std::string> GetProtonNeutronEnergyShiftDialNames();
 
 bool IsProtonNeutronEnergyShiftSyst(std::string name);
 
 std::vector<const ISyst *>
   GetProtonNeutronEnergyShiftSysts(std::vector<std::string> names = {}, bool applyPenalty = false);
 
} // namespace ana
