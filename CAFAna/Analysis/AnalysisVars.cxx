#include "CAFAna/Analysis/AnalysisVars.h"

#include "StandardRecord/SRProxy.h"

using namespace ana;

// List of vars
// -->FD
Var const kRecoE_nue = SIMPLEVAR(Ev_reco_nue);
Var const kRecoE_numu = SIMPLEVAR(Ev_reco_numu);
Var const kRecoE_FromDep = SIMPLEVAR(eRec_FromDep);
Var const kFDNumuPid = SIMPLEVAR(cvnnumu);
Var const kFDNuePid = SIMPLEVAR(cvnnue);
Var const kMVANUMU = SIMPLEVAR(mvanumu);

Var const kProxyERec = SIMPLEVAR(eRecProxy);

// -->ND
Var const kRecoEnergyND = SIMPLEVAR(Ev_reco);
Var const kRecoYND = (SIMPLEVAR(Ev_reco) - SIMPLEVAR(Elep_reco)) /
                     SIMPLEVAR(Ev_reco);
Var const kRecoY_FromDep =
    (SIMPLEVAR(eRec_FromDep) - SIMPLEVAR(LepE)) /
    SIMPLEVAR(eRec_FromDep);

Var const kTrueEnergy = SIMPLEVAR(Ev);

Var const kLepEReco = SIMPLEVAR(RecoLepE_NDFD); // RecoLepE_NDFD

Var const kHadEReco = SIMPLEVAR(RecoHadE_NDFD);

Var const kPEReco = SIMPLEVAR(eRecoP);
Var const kPipmEReco = SIMPLEVAR(eRecoPip) + SIMPLEVAR(eRecoPim);
Var const kPi0EReco = SIMPLEVAR(eRecoPi0);

//Var const kEVisReco = SIMPLEVAR(RecoLepE_NDFD) + SIMPLEVAR(RecoHadE_NDFD);
Var const kEVisReco = SIMPLEVAR(VisReco_NDFD);

Var const kEVisTrue = SIMPLEVAR(HadE) + SIMPLEVAR(LepE);

// CV weighting
Weight const kCVXSecWeights = SIMPLEWEIGHT(total_xsSyst_cv_wgt); // kUnweighted
