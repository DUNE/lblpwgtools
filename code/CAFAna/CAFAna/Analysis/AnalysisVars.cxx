#include "CAFAna/Analysis/AnalysisVars.h"

#include "StandardRecord/StandardRecord.h"

using namespace ana;

// List of vars
// -->FD
Var const kRecoE_nue = SIMPLEVAR(dune.Ev_reco_nue);
Var const kRecoE_numu = SIMPLEVAR(dune.Ev_reco_numu);
Var const kRecoE_FromDep = SIMPLEVAR(dune.eRec_FromDep);
Var const kFDNumuPid = SIMPLEVAR(dune.cvnnumu);
Var const kFDNuePid = SIMPLEVAR(dune.cvnnue);
Var const kMVANUMU = SIMPLEVAR(dune.mvanumu);

// -->ND
Var const kRecoEnergyND = SIMPLEVAR(dune.Ev_reco);
Var const kRecoYND = (SIMPLEVAR(dune.Ev_reco) - SIMPLEVAR(dune.Elep_reco)) /
                     SIMPLEVAR(dune.Ev_reco);
Var const kRecoY_FromDep =
    (SIMPLEVAR(dune.eRec_FromDep) - SIMPLEVAR(dune.LepE)) /
    SIMPLEVAR(dune.eRec_FromDep);

Var const kTrueEnergy = SIMPLEVAR(dune.Ev);

// CV weighting
Var const kCVXSecWeights = SIMPLEVAR(dune.total_xsSyst_cv_wgt); // kUnweighted
