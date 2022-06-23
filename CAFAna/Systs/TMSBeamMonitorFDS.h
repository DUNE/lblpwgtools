#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include <string>
#include <vector>
#include <algorithm>

#include "TH2.h"

namespace ana {

class TMSBeamMonitorFDSDial : public ISyst {
  public:

    virtual ~TMSBeamMonitorFDSDial(){
      if (wgts) delete wgts;
    };
    
    void Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr,
	       double &weight) const override;
    
   protected:
    TMSBeamMonitorFDSDial(std::string name, bool applyPenalty = false);

    friend std::vector<const ISyst *> GetTMSBeamMonitorFDSSysts(std::vector<std::string>, bool);

    std::string fName;

    TH1D *wgts;
};

 std::vector<std::string> GetTMSBeamMonitorFDSDialNames();
 
 bool IsTMSBeamMonitorFDSSyst(std::string name);
 
 std::vector<const ISyst *>
   GetTMSBeamMonitorFDSSysts(std::vector<std::string> names = {}, bool applyPenalty = false);
 
} // namespace ana
