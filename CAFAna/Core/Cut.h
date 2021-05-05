#pragma once

#include "CAFAnaCore/CAFAna/Core/Cut.h"

#include "StandardRecord/Proxy/FwdDeclare.h"

namespace ana
{
  using Cut = _Cut<caf::SRProxy>;

  /// The simplest possible cut: pass everything, used as a default
  const Cut kNoCut(NoCut<caf::SRProxy>{});
}
