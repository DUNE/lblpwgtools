#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/GenieWeightList.h"
#include "StandardRecord/StandardRecord.h"
#include <cmath>
#include <cassert>

namespace ana
{
  class GenieSyst: public ISyst
  {
  public:
    GenieSyst(int genie_id) :
      ISyst(GetGenieWeightName(id), GetGenieWeightName(id)),
      id(genie_id) {}

    virtual ~GenieSyst(){};

    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr,
	       double& weight) const override{

      assert(std::abs(sigma) <= 3 && "GENIE XSECs only valid up to +/-3 sigma!");

      // How far apart are the points
      const double spacing = 1;

      // Get the top and bottom values in the array
      int low_index  = std::floor(sigma/spacing) + 3;
      int high_index = std::ceil(sigma/spacing) + 3;

      double diff = (sigma-double(low_index))/spacing;

      double low_weight  = sr->dune.genie_wgt[id][low_index];
      double high_weight = sr->dune.genie_wgt[id][high_index];

      weight *= low_weight + (high_weight - low_weight)*diff;

    }

  protected:

    int id;
  };

  std::vector<const ISyst*> GetGenieSysts(){
    static std::vector<const ISyst*> ret;

    if(ret.empty()){
      for (uint i = 0; i < GetGenieWeightNames().size(); ++i){
        ret.push_back(new GenieSyst(i));
      }
    }
    return ret;
  }

}
