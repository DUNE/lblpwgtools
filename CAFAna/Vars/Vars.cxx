#include "CAFAna/Vars/Vars.h"
#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Core/MathUtil.h"

#include "StandardRecord/SRProxy.h"

#include <cassert>

#include "TVector3.h"

namespace ana
{
  // -->FD
  const Var kRecoE_nue = SIMPLEVAR(Ev_reco_nue);
  const Var kRecoE_numu = SIMPLEVAR(Ev_reco_numu);
  const Var kRecoE_FromDep = SIMPLEVAR(eRec_FromDep);
  const Var kFDNumuPid = SIMPLEVAR(cvnnumu);
  const Var kFDNuePid = SIMPLEVAR(cvnnue);
  const Var kMVANUMU = SIMPLEVAR(mvanumu);
  //FD from NDFDExtrapPred
//  const Var kRecoE_FDExtrapPred_nue = SIMPLEVAR(pred_fd_nue_nu_E);
  const Var kRecoE_FDExtrapPred_numu = SIMPLEVAR(pred_fd_numu_nu_E);

  // -->ND
  const Var kRecoEnergyND = SIMPLEVAR(Ev_reco);
  const Var kRecoYND = (SIMPLEVAR(Ev_reco) - SIMPLEVAR(Elep_reco)) / SIMPLEVAR(Ev_reco);
  const Var kRecoY_FromDep = (SIMPLEVAR(eRec_FromDep) - SIMPLEVAR(LepE)) / SIMPLEVAR(eRec_FromDep);
  const Var kTrueEnergy = SIMPLEVAR(Ev);

  // CV weighting
  const Weight kCVXSecWeights = SIMPLEWEIGHT(total_xsSyst_cv_wgt); // kUnweighted

}
