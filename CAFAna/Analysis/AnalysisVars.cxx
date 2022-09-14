#include "CAFAna/Analysis/AnalysisVars.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

using namespace ana;

// List of vars
// -->FD
Var const kRecoE_nue = SIMPLEVAR(Ev_reco_nue);
Var const kRecoE_numu = SIMPLEVAR(Ev_reco_numu);
Var const kRecoE_FromDep = SIMPLEVAR(eRec_FromDep);
Var const kFDNumuPid = SIMPLEVAR(cvnnumu);
Var const kFDNuePid = SIMPLEVAR(cvnnue);
Var const kMVANUMU = SIMPLEVAR(mvanumu);

Var const kLepEReco_numu = SIMPLEVAR(RecoLepEnNumu);
Var const kLepEReco_nue = SIMPLEVAR(RecoLepEnNue);

Var const kHadEReco_numu = SIMPLEVAR(RecoHadEnNumu);
Var const kHadEReco_nue = SIMPLEVAR(RecoHadEnNue);

Var const kEVisReco_numu = SIMPLEVAR(EVisReco_numu);
Var const kEVisReco_nue = SIMPLEVAR(EVisReco_nue);

// -->ND
Var const kRecoEnergyND = SIMPLEVAR(Ev_reco);
Var const kRecoYND = (SIMPLEVAR(Ev_reco) - SIMPLEVAR(Elep_reco)) /
                     SIMPLEVAR(Ev_reco);
Var const kRecoY_FromDep =
    (SIMPLEVAR(eRec_FromDep) - SIMPLEVAR(LepE)) /
    SIMPLEVAR(eRec_FromDep);

Var const kNDLepEReco = SIMPLEVAR(Elep_reco);
Var const kNDEVisReco = SIMPLEVAR(EVisReco_ND);

// -->Common ND & FD
Var const kTrueEnergy = SIMPLEVAR(Ev);
Var const kProxyERec = SIMPLEVAR(eRecProxy);
Var const kEVisTrue = SIMPLEVAR(VisTrue_NDFD);

Var const kPEReco = SIMPLEVAR(eRecoP);
Var const kPipmEReco = SIMPLEVAR(eRecoPip) + SIMPLEVAR(eRecoPim); 
Var const kPi0EReco = SIMPLEVAR(eRecoPi0);

// CV weighting
Weight const kCVXSecWeights = SIMPLEWEIGHT(total_xsSyst_cv_wgt); // kUnweighted
