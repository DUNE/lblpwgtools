#include "CAFAna/Vars/Vars.h"
#include "CAFAna/Core/Utilities.h"

#include "Utilities/func/MathUtil.h"

#include "StandardRecord/StandardRecord.h"

#include <cassert>

#include "TVector3.h"

namespace ana{

	// -->FD
	const Var kRecoE_nue = SIMPLEVAR(dune.Ev_reco_nue);
	const Var kRecoE_numu = SIMPLEVAR(dune.Ev_reco_numu);
	const Var kRecoE_FromDep = SIMPLEVAR(dune.eRec_FromDep);
	const Var kFDNumuPid = SIMPLEVAR(dune.cvnnumu);
	const Var kFDNuePid = SIMPLEVAR(dune.cvnnue);
	const Var kMVANUMU = SIMPLEVAR(dune.mvanumu);

	// -->ND
	const Var kRecoEnergyND = SIMPLEVAR(dune.Ev_reco);
	const Var kRecoYND = (SIMPLEVAR(dune.Ev_reco) - SIMPLEVAR(dune.Elep_reco)) /
	                     SIMPLEVAR(dune.Ev_reco);
	const Var kRecoY_FromDep =
	    (SIMPLEVAR(dune.eRec_FromDep) - SIMPLEVAR(dune.LepE)) /
	    SIMPLEVAR(dune.eRec_FromDep);

	const Var kTrueEnergy = SIMPLEVAR(dune.Ev);

	// CV weighting
	const Var kCVXSecWeights = SIMPLEVAR(dune.total_xsSyst_cv_wgt); // kUnweighted

}