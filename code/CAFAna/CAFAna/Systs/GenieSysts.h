#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/GenieWeightList.h"
#include "StandardRecord/StandardRecord.h"
#include <cmath>

namespace ana
{
  class GenieSyst: public ISyst
  {
  public:
    
    GenieSyst(int genie_id): id(genie_id) {}
    
    virtual ~GenieSyst(){};

    std::string ShortName() const override {return GetGenieWeightName(id);}
    std::string LatexName() const override {return GetGenieWeightName(id);}
    
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr,
	       double& weight) const override{

      int index = int(std::round(sigma)/0.5) + 6;
      weight *= sr->dune.genie_wgt[id][index];
    }

  protected:
    
    int id;
  };

  std::vector<const ISyst*> GetGenieSysts(){
    std::vector<const ISyst*> ret;
    
    for (uint i = 0; i < GetGenieWeightNames().size(); ++i){
      ret.push_back(new GenieSyst(i));
    }
    return ret;
  }

}
