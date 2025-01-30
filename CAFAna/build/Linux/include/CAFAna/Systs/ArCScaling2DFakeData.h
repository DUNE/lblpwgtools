#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include <string>
#include <vector>
#include <algorithm>

#include "TH2.h"

namespace ana {

class ArCScaling2DDial : public ISyst {
  public:

    virtual ~ArCScaling2DDial(){
      if (nu_wgts) delete nu_wgts;
      if (nubar_wgts) delete nubar_wgts;
    };
    
    void Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr,
	       double &weight) const override;
    
   protected:
    ArCScaling2DDial(std::string name, bool applyPenalty = false);

    friend std::vector<const ISyst *> GetArCScaling2DSysts(std::vector<std::string>, bool);

    std::string fName;

    TH2D *nu_wgts;
    TH2D *nubar_wgts;
};

 std::vector<std::string> GetArCScaling2DDialNames();
 
 bool IsArCScaling2DSyst(std::string name);
 
 std::vector<const ISyst *>
   GetArCScaling2DSysts(std::vector<std::string> names = {}, bool applyPenalty = false);
 
} // namespace ana
