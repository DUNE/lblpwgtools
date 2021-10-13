#include "CAFAna/Systs/HighEFluxMod.h"
 
namespace ana {

  std::vector<std::string> GetHighEFluxModDialNames(){
    static std::vector<std::string> ret = {"HighEFluxMod_FHC_numu",
					   "HighEFluxMod_FHC_numubar",
					   "HighEFluxMod_RHC_numu",
                                           "HighEFluxMod_RHC_numubar"};
    return ret;
  }

  bool IsHighEFluxModSyst(std::string name){
    return (std::find(GetHighEFluxModDialNames().begin(), GetHighEFluxModDialNames().end(), name) !=
	    GetHighEFluxModDialNames().end());
  }


  void HighEFluxModDial::Shift(double sigma, Restorer &restore,
			    caf::StandardRecord *sr,
			    double &weight) const {
    // Play it safe
    if (fabs(sigma) < 1E-5) return;

    // This is a bit hacky... simply because ND files with this fix haven't been made yet
    if (!sr->dune.isFD) return;

    // Make sure weights are only applied when appropriate
    if ((fName == "HighEFluxMod_FHC_numu" || fName == "HighEFluxMod_FHC_numubar") && !sr->dune.isFHC){
      return;
    }
    if ((fName == "HighEFluxMod_RHC_numu" || fName == "HighEFluxMod_RHC_numubar") && sr->dune.isFHC){
      return;
    }

    // Apply a check to the flavour
    if ((fName == "HighEFluxMod_FHC_numu" || fName == "HighEFluxMod_RHC_numu") && sr->dune.nuPDGunosc != 14)
      return;
    if ((fName == "HighEFluxMod_FHC_numubar" || fName == "HighEFluxMod_RHC_numubar") && sr->dune.nuPDGunosc != -14)
      return;	

    // Need to multiply the weight by something random...
    // only apply weights for events above 1.25 GeV
    if (sr->dune.Ev < 1.25) return;
    
    // Linear above 1.25 (0.9 at 10 GeV)
    weight *= (sr->dune.Ev * -0.1*sigma/8.75) + 1 + 0.125/8.75;
    
  }

  
  HighEFluxModDial::HighEFluxModDial(std::string name, bool applyPenalty)
    : ISyst(name, name, applyPenalty),
      fName(name) {}

  std::vector<const ISyst *> GetHighEFluxModSysts(std::vector<std::string> names,
					       bool applyPenalty) {
    static std::vector<const ISyst *> ret;
    
    if (names.empty()) {
      names = GetHighEFluxModDialNames();
    }

    if (ret.empty()) {
      for (auto &it : names) {
	ret.push_back(new HighEFluxModDial(it, applyPenalty));
      }
    }
    return ret;
  }


}
