#include "CAFAna/Core/GenieWeightList.h"

#include <algorithm>
#include <iostream>

namespace ana
{
  //----------------------------------------------------------------------
  std::vector<std::string> GetGenieWeightNames()
  {
    // I wonder if we can share this somehow with the code that generates these?
    // return {"MaCCQE",
    //     "RvpCC1pi", "RvpCC2pi",
    //     "RvpNC1pi", "RvnNC2pi",
    //     "RvnNC1pi",
    //     "RvbarpCC1pi", "RvbarpCC2pi",
    //     "RvbarnCC1pi", "RvbarnCC2pi",
    //     "RvbarpNC1pi", "RvbarpNC2pi",
    //     "RvbarnNC1pi", "RvbarnNC2pi",
    //     "BR1gamma", "BR1eta", "Theta_Delta2Npi"};

    return {"EtaNCEL",
        "MaCCQE", "MaCCRES", "MaCOHpi", "MaNCEL", "MaNCRES",
        "MvCCRES", "MvNCRES", 
        "NonRESBGvbarnCC1pi", "NonRESBGvbarnCC2pi",
        "NonRESBGvbarnNC1pi", "NonRESBGvbarnNC2pi",
        "NonRESBGvbarpCC1pi", "NonRESBGvbarpCC2pi",
        "NonRESBGvbarpNC1pi", "NonRESBGvbarpNC2pi",
        "NonRESBGvnCC1pi", "NonRESBGvnCC2pi",
        "NonRESBGvnNC1pi", "NonRESBGvnNC2pi",
        "NonRESBGvpCC1pi", "NonRESBGvpCC2pi",
        "NonRESBGvpNC1pi", "NonRESBGvpNC2pi"
        };
  }

  //----------------------------------------------------------------------
  int GetGenieIndex(const std::string& name, bool quiet)
  {
    const std::vector<std::string> names = GetGenieWeightNames();

    auto it = std::find(names.begin(), names.end(), name);

    if(it == names.end()){
      if(!quiet){
        std::cerr << "Warning: couldn't find " << name
                  << " in list of genie systs" << std::endl;
      }
      return -1;
    }

    return it-names.begin();
  }
}
