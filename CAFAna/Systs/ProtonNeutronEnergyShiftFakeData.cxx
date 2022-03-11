#include "CAFAna/Systs/ProtonNeutronEnergyShiftFakeData.h"
 
namespace ana {

  std::vector<std::string> GetProtonNeutronEnergyShiftDialNames(){
    static std::vector<std::string> ret = {"ProtonNeutronEnergyShift_FHC",
					   "ProtonNeutronEnergyShift_RHC",
					   "ProtonNeutronEnergyShift_BOTH"};
    return ret;
  }

  bool IsProtonNeutronEnergyShiftSyst(std::string name){
    return (std::find(GetProtonNeutronEnergyShiftDialNames().begin(), GetProtonNeutronEnergyShiftDialNames().end(), name) !=
	    GetProtonNeutronEnergyShiftDialNames().end());
  }


  void ProtonNeutronEnergyShiftDial::Shift(double sigma, Restorer &restore,
			    caf::StandardRecord *sr,
			    double &weight) const {

    // Quantities to save
    restore.Add(sr->dune.Ev_reco_numu,
		sr->dune.Ev_reco_nue,
		sr->dune.RecoHadEnNumu,
		sr->dune.RecoHadEnNue,
		sr->dune.eRecoN,
		sr->dune.eRecoP);

    // Play it safe
    if (fabs(sigma) < 1E-5) return;

    // FD only
    if (!sr->dune.isFD) return;

    // Make sure weights are only applied when appropriate
    if (fName == GetProtonNeutronEnergyShiftDialNames()[0] && !sr->dune.isFHC){
      return;
    }
    if (fName == GetProtonNeutronEnergyShiftDialNames()[1] && sr->dune.isFHC){
      return;
    }

    // double scale = fabs(.25 * sigma);
    double scale = -0.25*sigma;

    // This mocks up a shift in the fraction of energy that goes into protons and neutrons
    // +ve shifts from protons to neutrons
    // -ve shifts from neutrons to protons

    // Keep things sane
    // if (sr->dune.eRecoN < 0) sr->dune.eRecoN = 0.;
    // if (sr->dune.eRecoP < 0) sr->dune.eRecoP = 0.;
    
    // double n_reco = sr->dune.eRecoN;
    // double n_true = sr->dune.eN;
    // double p_reco = sr->dune.eRecoP;
    // double p_true = sr->dune.eP;

    // double n_frac = 0.2;
    // if (n_true > 0) n_frac = n_reco/n_true;
    // if (n_frac > 1) n_frac = 0.2;

    // double p_frac = 0.9;
    // if (p_true > 0) p_frac = p_reco/p_true;
    // if (p_frac > 1) p_frac = 0.9;


    // Subtract energy from neutrons, add it to protons
      // if (sigma < 0){
      // sr->dune.Ev_reco_numu -= sr->dune.eRecoN * scale;
      // sr->dune.Ev_reco_nue  -= sr->dune.eRecoN * scale;
      // sr->dune.RecoHadEnNumu -= sr->dune.eRecoN * scale;
      // sr->dune.RecoHadEnNue  -= sr->dune.eRecoN * scale;

      // sr->dune.Ev_reco_numu += sr->dune.eN * scale * p_frac;
      // sr->dune.Ev_reco_nue  += sr->dune.eN * scale * p_frac;
      // sr->dune.RecoHadEnNumu += sr->dune.eN * scale * p_frac;
      // sr->dune.RecoHadEnNue  += sr->dune.eN * scale * p_frac;
      //    } 

    // Subtract energy from protons, add it to neutrons
    // if (sigma > 0) {
    sr->dune.Ev_reco_numu -= sr->dune.eRecoP * scale;
    sr->dune.Ev_reco_nue  -= sr->dune.eRecoP * scale;
    sr->dune.RecoHadEnNumu -= sr->dune.eRecoP * scale;
    sr->dune.RecoHadEnNue  -= sr->dune.eRecoP * scale;
    
      // sr->dune.Ev_reco_numu += sr->dune.eP * scale * n_frac;
      // sr->dune.Ev_reco_nue  += sr->dune.eP * scale * n_frac;
      // sr->dune.RecoHadEnNumu += sr->dune.eP * scale * n_frac;
      // sr->dune.RecoHadEnNue  += sr->dune.eP * scale * n_frac;
    // }
  }

  
  std::vector<const ISyst *> GetProtonNeutronEnergyShiftSysts(std::vector<std::string> names,
							      bool applyPenalty) {
    static std::vector<const ISyst *> ret;
    
    if (names.empty()) {
      names = GetProtonNeutronEnergyShiftDialNames();
    }

    if (ret.empty()) {
      for (auto &it : names) {
	ret.push_back(new ProtonNeutronEnergyShiftDial(it, applyPenalty));
      }
    }
    return ret;
  }


}
