#include "CAFAna/Systs/CrazyFluxFakeData.h"
 
#include "StandardRecord/Proxy/SRProxy.h"

namespace ana {

  std::vector<std::string> GetCrazyFluxDialNames(){
    static std::vector<std::string> ret = {"CrazyFlux_FHC",
					   "CrazyFlux_RHC",
					   "CrazyFlux_BOTH"};
    return ret;
  }

  bool IsCrazyFluxSyst(std::string name){
    return (std::find(GetCrazyFluxDialNames().begin(), GetCrazyFluxDialNames().end(), name) !=
	    GetCrazyFluxDialNames().end());
  }


  void CrazyFluxDial::Shift(double sigma, Restorer &restore,
			    caf::SRProxy *sr,
			    double &weight) const {
    // Play it safe
    if (fabs(sigma) < 1E-5) return;

    // This is a bit hacky... simply because ND files with this fix haven't been made yet
    if (!sr->isFD) return;

    // Make sure weights are only applied when appropriate
    if (fName == GetCrazyFluxDialNames()[0] && !sr->isFHC){
      return;
    }
    if (fName == GetCrazyFluxDialNames()[1] && sr->isFHC){
      return;
    }
    
    // If you get this far, apply a weight
    // All values in the array except [3], the nominal, should be the same
    weight *= sr->wgt_CrazyFlux[0];
  }

  
  CrazyFluxDial::CrazyFluxDial(std::string name, bool applyPenalty)
    : ISyst(name, name, applyPenalty),
      fName(name) {}

  std::vector<const ISyst *> GetCrazyFluxSysts(std::vector<std::string> names,
					       bool applyPenalty) {
    static std::vector<const ISyst *> ret;
    
    if (names.empty()) {
      names = GetCrazyFluxDialNames();
    }

    if (ret.empty()) {
      for (auto &it : names) {
	ret.push_back(new CrazyFluxDial(it, applyPenalty));
      }
    }
    return ret;
  }


}
