#include "CAFAna/Vars/Vars.h"
#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Core/MathUtil.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include <cassert>

#include "TVector3.h"

namespace ana
{
  const Var kRecoEnu = SIMPLEVAR(Enu.lep_calo);
  const TruthVar kTrueEnu = SIMPLETRUTHVAR(E);
}
