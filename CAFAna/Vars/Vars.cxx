#include "CAFAna/Vars/Vars.h"
#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Core/MathUtil.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include <cassert>

#include "TVector3.h"

namespace ana
{
//  // -->FD
//  const Var kRecoE_nue = SIMPLEVAR(Ev_reco_nue);
//  const Var kRecoE_numu = SIMPLEVAR(Ev_reco_numu);
//  const Var kRecoE_FromDep = SIMPLEVAR(eRec_FromDep);
//  const Var kFDNumuPid = SIMPLEVAR(cvnnumu);
//  const Var kFDNuePid = SIMPLEVAR(cvnnue);
//  const Var kMVANUMU = SIMPLEVAR(mvanumu);
//
//  // -->ND
//  const Var kRecoEnergyND = SIMPLEVAR(Ev_reco);
//  const Var kRecoYND = (SIMPLEVAR(Ev_reco) - SIMPLEVAR(Elep_reco)) / SIMPLEVAR(Ev_reco);
//  const Var kRecoY_FromDep = (SIMPLEVAR(eRec_FromDep) - SIMPLEVAR(LepE)) / SIMPLEVAR(eRec_FromDep);
  const Var kTrueEnergy = SIMPLEVAR(mc.nu[0].E);//Ev);
//
//  // CV weighting
//  const Weight kCVXSecWeights = SIMPLEWEIGHT(total_xsSyst_cv_wgt); // kUnweighted

}
