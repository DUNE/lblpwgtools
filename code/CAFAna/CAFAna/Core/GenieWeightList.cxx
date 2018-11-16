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

    // return {"EtaNCEL",
    //     "MaCCQE", "MaCCRES", "MaCOHpi", "MaNCEL", "MaNCRES",
    //     "MvCCRES", "MvNCRES", 
    //     "NonRESBGvbarnCC1pi", "NonRESBGvbarnCC2pi",
    //     "NonRESBGvbarnNC1pi", "NonRESBGvbarnNC2pi",
    //     "NonRESBGvbarpCC1pi", "NonRESBGvbarpCC2pi",
    //     "NonRESBGvbarpNC1pi", "NonRESBGvbarpNC2pi",
    //     "NonRESBGvnCC1pi", "NonRESBGvnCC2pi",
    //     "NonRESBGvnNC1pi", "NonRESBGvnNC2pi",
    //     "NonRESBGvpCC1pi", "NonRESBGvpCC2pi",
    //     "NonRESBGvpNC1pi", "NonRESBGvpNC2pi",
    // 	"RDecBR1gamma", "RDecBR1eta",
    // 	"Theta_Delta2Npi", "R0COHpi"
    //     };

    return {"MaCCQE", "MaNCEL", "EtaNCEL", "MaCCRES", 
	"MvCCRES", "RDecBR1gamma", "RDecBR1eta", 
	"Theta_Delta2Npi", "CCQEPauliSupViaKF", "E2p2h_A_nu", 
	"E2p2h_B_nu", "E2p2h_A_nubar", "E2p2h_B_nubar", "NR_nu_n_CC_2Pi", 
	"NR_nu_n_CC_3Pi", "NR_nu_p_CC_2Pi", "NR_nu_p_CC_3Pi", "NR_nu_np_CC_1Pi", 
	"NR_nu_n_NC_1Pi", "NR_nu_n_NC_2Pi", "NR_nu_n_NC_3Pi", "NR_nu_p_NC_1Pi", 
	"NR_nu_p_NC_2Pi", "NR_nu_p_NC_3Pi", "NR_nubar_n_CC_1Pi", "NR_nubar_n_CC_2Pi", 
	"NR_nubar_n_CC_3Pi", "NR_nubar_p_CC_1Pi", "NR_nubar_p_CC_2Pi", "NR_nubar_p_CC_3Pi", 
	"NR_nubar_n_NC_1Pi", "NR_nubar_n_NC_2Pi", "NR_nubar_n_NC_3Pi", "NR_nubar_p_NC_1Pi", 
	"NR_nubar_p_NC_2Pi", "NR_nubar_p_NC_3Pi", "BeRPA_A", "BeRPA_B", "BeRPA_D", "BeRPA_E", 
	"nuenuebar_xsec_ratio", "SPPLowQ2Suppression"
	// "Mnv2p2hGaussEnhancement_CV", "Mnv2p2hGaussEnhancement_NN", "Mnv2p2hGaussEnhancement_np", "Mnv2p2hGaussEnhancement_QE", "nuenumu_xsec_ratio"
        // "C12ToAr40_2p2hScaling_nu", "C12ToAr40_2p2hScaling_nubar",
	// "NormNCRES", "MaNCRESshape", "MvNCRESshape"
	// "MKSPP_ReWeight", "EbFSLepMomShift"
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

  std::string GetGenieWeightName(int index){
    const std::vector<std::string> names = GetGenieWeightNames();
    return names[index];
  }

}
