#include "CAFAna/Systs/FluxWiggleSqueezeFakeData.h"
 
namespace ana {

  std::vector<std::string> GetFluxWiggleSqueezeDialNames(){
    static std::vector<std::string> ret = {"FluxWiggleSqueeze_FHC",
					   "FluxWiggleSqueeze_RHC",
					   "FluxWiggleSqueeze_BOTH"};
    return ret;
  }

  bool IsFluxWiggleSqueezeSyst(std::string name){
    return (std::find(GetFluxWiggleSqueezeDialNames().begin(), GetFluxWiggleSqueezeDialNames().end(), name) !=
	    GetFluxWiggleSqueezeDialNames().end());
  }


  void FluxWiggleSqueezeDial::Shift(double sigma, Restorer &restore,
			    caf::StandardRecord *sr,
			    double &weight) const {
    // Play it safe
    if (fabs(sigma) < 1E-5) return;

    // This is a bit hacky... simply because ND files with this fix haven't been made yet
    if (!sr->dune.isFD) return;

    // Make sure weights are only applied when appropriate
    if (fName == GetFluxWiggleSqueezeDialNames()[0] && !sr->dune.isFHC){
      return;
    }
    if (fName == GetFluxWiggleSqueezeDialNames()[1] && sr->dune.isFHC){
      return;
    }

    // don't apply a weight for < 1.25 or  > 10
    if (sr->dune.Ev < 1.25 || sr->dune.Ev > 10) return;

    // Otherwise apply a weight based on the MINERvA flux wiggle function
    weight *= (weight_func->Eval(sr->dune.Ev)-1)*sigma + 1;

  }

  
  FluxWiggleSqueezeDial::FluxWiggleSqueezeDial(std::string name, bool applyPenalty)
    : ISyst(name, name, applyPenalty),
      fName(name) {
    
    // This weight function approximates the observed MINERvA flux wiggle
    weight_func = new TF1((name+"_weight_func").c_str(), "pol8", 1.25, 10);
    weight_func ->SetNpx(100);
    weight_func ->SetParameters(2.3576, -4.14698, 4.63385, -2.45296, 0.697099, \
				-0.112797, 0.0104146, -0.000511022, 1.03403e-05);
  }

  std::vector<const ISyst *> GetFluxWiggleSqueezeSysts(std::vector<std::string> names,
					       bool applyPenalty) {
    static std::vector<const ISyst *> ret;
    
    if (names.empty()) {
      names = GetFluxWiggleSqueezeDialNames();
    }

    if (ret.empty()) {
      for (auto &it : names) {
	ret.push_back(new FluxWiggleSqueezeDial(it, applyPenalty));
      }
    }
    return ret;
  }


}
