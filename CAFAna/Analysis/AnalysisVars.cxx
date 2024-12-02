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

Var const kLepEReco_numu = SIMPLEVAR(RecoLepEnNumu); 
Var const kLepEReco_nue = SIMPLEVAR(RecoLepEnNue); 

Var const kHadEReco_numu = SIMPLEVAR(RecoHadEnNumu);
Var const kHadEReco_nue = SIMPLEVAR(RecoHadEnNue);

Var const kEVisReco_numu = SIMPLEVAR(EVisReco_numu);
Var const kEVisReco_nue = SIMPLEVAR(EVisReco_nue);
//Var const kEVisReco_numu = SIMPLEVAR(ELep_param) + SIMPLEVAR(HadEVisReco_FD);
//Var const kEVisReco_nue = SIMPLEVAR(ELep_param) + SIMPLEVAR(HadEVisReco_FD);;
//==FD from FDExtrapPred
//Var const kRecoE_FDExtrapPred_nue = SIMPLEVAR(pred_fd_nue_nu_E);
Var const kRecoE_FDExtrapPred_numu = SIMPLEVAR(pred_fd_numu_nu_E);
//Vars for NDFD Extrapolation Resolution Matrix
Var const kpairedData_numu_nu_E = SIMPLEVAR(pairedData_numu_nu_E);
Var const kEnuReco_Pred = SIMPLEVAR(pred_numu_nu_E);

Var const kLepEreco_FDExtrapPred_nue = SIMPLEVAR(pred_fd_nue_lep_E);
Var const kLepEreco_FDExtrapPred_numu = SIMPLEVAR(pred_fd_numu_lep_E);

//Var const kHadEreco_FDExtrapPred_nue = SIMPLEVAR(pred_fd_nue_nu_E) - SIMPLEVAR(pred_fd_nue_lep_E);
Var const kHadEreco_FDExtrapPred_numu = SIMPLEVAR(pred_fd_numu_nu_E) - SIMPLEVAR(pred_fd_numu_lep_E);
// -->ND
Var const kRecoEnergyND = SIMPLEVAR(Ev_reco);
Var const kRecoYND = (SIMPLEVAR(Ev_reco) - SIMPLEVAR(Elep_reco)) /
                     SIMPLEVAR(Ev_reco);
Var const kRecoY_FromDep =
    (SIMPLEVAR(eRec_FromDep) - SIMPLEVAR(LepE)) /
    SIMPLEVAR(eRec_FromDep);

Var const kNDLepEReco = SIMPLEVAR(Elep_reco);
Var const kNDEVisReco = SIMPLEVAR(EVisReco_ND);
//Var const kNDEVisReco = SIMPLEVAR(ELep_param) + SIMPLEVAR(HadEVisReco_ND);

// -->Common ND & FD
Var const kTrueEnergy = SIMPLEVAR(Ev);
Var const kProxyERec = SIMPLEVAR(eRecProxy);
Var const kEVisTrue = SIMPLEVAR(VisTrue_NDFD);

Var const kHadE_param = SIMPLEVAR(HadE_param);
Var const kLepE_param = SIMPLEVAR(ELep_param);
Var const kEVis_param = SIMPLEVAR(EVis_param);
//Var const kEVis_param = SIMPLEVAR(ELep_param) + SIMPLEVAR(HadE_param);

Var const kPEReco = SIMPLEVAR(eRecoP);
Var const kPipmEReco = SIMPLEVAR(eRecoPip) + SIMPLEVAR(eRecoPim); 
Var const kPi0EReco = SIMPLEVAR(eRecoPi0);

// CV weighting
Weight const kCVXSecWeights = SIMPLEWEIGHT(total_xsSyst_cv_wgt); // kUnweighted
