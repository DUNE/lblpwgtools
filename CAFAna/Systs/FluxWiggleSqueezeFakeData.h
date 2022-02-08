#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include <string>
#include <vector>
#include <algorithm>

#include "TF1.h"

namespace ana {

class FluxWiggleSqueezeDial : public ISyst {
  public:

    virtual ~FluxWiggleSqueezeDial(){if (weight_func) delete weight_func;};
    
    void Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr,
	       double &weight) const override;
    
  protected:
    FluxWiggleSqueezeDial(std::string name, bool applyPenalty = false);

    friend std::vector<const ISyst *> GetFluxWiggleSqueezeSysts(std::vector<std::string>, bool);

    std::string fName;
 
    TF1 *weight_func;
};

 std::vector<std::string> GetFluxWiggleSqueezeDialNames();
 
 bool IsFluxWiggleSqueezeSyst(std::string name);
 
 std::vector<const ISyst *>
   GetFluxWiggleSqueezeSysts(std::vector<std::string> names = {}, bool applyPenalty = false);
 
} // namespace ana
