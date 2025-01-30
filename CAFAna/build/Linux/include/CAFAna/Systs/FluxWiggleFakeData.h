#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include <string>
#include <vector>
#include <algorithm>

#include "TF1.h"

namespace ana {

class FluxWiggleDial : public ISyst {
  public:

    virtual ~FluxWiggleDial(){if (weight_func) delete weight_func;};
    
    void Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr,
	       double &weight) const override;
    
  protected:
    FluxWiggleDial(std::string name, bool applyPenalty = false);

    friend std::vector<const ISyst *> GetFluxWiggleSysts(std::vector<std::string>, bool);

    std::string fName;
 
    TF1 *weight_func;
};

 std::vector<std::string> GetFluxWiggleDialNames();
 
 bool IsFluxWiggleSyst(std::string name);
 
 std::vector<const ISyst *>
   GetFluxWiggleSysts(std::vector<std::string> names = {}, bool applyPenalty = false);
 
} // namespace ana
