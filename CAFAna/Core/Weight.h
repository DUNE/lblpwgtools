#pragma once

#include "CAFAnaCore/CAFAna/Core/Weight.h"

#include "duneanaobj/StandardRecord/Proxy/FwdDeclare.h"

namespace ana
{
  using Weight = _Weight<caf::SRProxy>;

#define SIMPLEWEIGHT(CAFNAME) Weight([](const caf::SRProxy* sr){return sr->CAFNAME;})

  const Weight kUnweighted = Unweighted<caf::SRProxy>();
}
