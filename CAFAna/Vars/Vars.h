// Eventually maybe we split this into truth-only TruthVars.{cxx,h}, etc.
#pragma once

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Weight.h"

#include "CAFAna/Vars/Vars.h"

namespace ana
{

  // simple examples
  extern const Var kRecoEnu;
  extern const TruthVar kTrueEnu;

  // useful truth-only variables
  inline const TruthVar kTrueVtxX_NT = SIMPLETRUTHVAR(vtx.x);
  inline const TruthVar kTrueVtxY_NT = SIMPLETRUTHVAR(vtx.y);
  inline const TruthVar kTrueVtxZ_NT = SIMPLETRUTHVAR(vtx.z);
  inline const Var kTrueVtxX = TruthVarToVar(kTrueVtxX_NT);
  inline const Var kTrueVtxY = TruthVarToVar(kTrueVtxY_NT);
  inline const Var kTrueVtxZ = TruthVarToVar(kTrueVtxZ_NT);

} // namespace
