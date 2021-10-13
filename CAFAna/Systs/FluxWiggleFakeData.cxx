#include "CAFAna/Systs/FluxWiggleFakeData.h"
 
namespace ana {

  std::vector<std::string> GetFluxWiggleDialNames(){
    static std::vector<std::string> ret = {"FluxWiggle_FHC",
					   "FluxWiggle_RHC",
					   "FluxWiggle_BOTH"};
    return ret;
  }

  bool IsFluxWiggleSyst(std::string name){
    return (std::find(GetFluxWiggleDialNames().begin(), GetFluxWiggleDialNames().end(), name) !=
	    GetFluxWiggleDialNames().end());
  }


  void FluxWiggleDial::Shift(double sigma, Restorer &restore,
			    caf::StandardRecord *sr,
			    double &weight) const {
    // Play it safe
    if (fabs(sigma) < 1E-5) return;

    // This is a bit hacky... simply because ND files with this fix haven't been made yet
    if (!sr->dune.isFD) return;

    // Make sure weights are only applied when appropriate
    if (fName == GetFluxWiggleDialNames()[0] && !sr->dune.isFHC){
      return;
    }
    if (fName == GetFluxWiggleDialNames()[1] && sr->dune.isFHC){
      return;
    }

    // don't apply a weight for < 1.25 or  > 25
    if (sr->dune.Ev < 1.25 || sr->dune.Ev > 25) return;

    // Otherwise apply a weight based on the MINERvA flux wiggle function
    weight *= (weight_func->Eval(sr->dune.Ev)-1)*sigma + 1;

  }

  
  FluxWiggleDial::FluxWiggleDial(std::string name, bool applyPenalty)
    : ISyst(name, name, applyPenalty),
      fName(name) {
    
    // This weight function approximates the observed MINERvA flux wiggle
    weight_func = new TF1((name+"_weight_func").c_str(), "pol9", 1.25, 25);
    weight_func ->SetNpx(250);
    weight_func ->SetParameters(1.03203, -0.176195, 0.201445, -0.0697988, 
				0.0111878, -0.000981451, 5.02375e-05, 
				-1.50021e-06, 2.42605e-08, -1.64368e-10);
  }

  std::vector<const ISyst *> GetFluxWiggleSysts(std::vector<std::string> names,
					       bool applyPenalty) {
    static std::vector<const ISyst *> ret;
    
    if (names.empty()) {
      names = GetFluxWiggleDialNames();
    }

    if (ret.empty()) {
      for (auto &it : names) {
	ret.push_back(new FluxWiggleDial(it, applyPenalty));
      }
    }
    return ret;
  }


}
