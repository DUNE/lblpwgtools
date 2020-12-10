#pragma once

#include "CAFAnaCore/CAFAna/Core/Cut.h"

namespace caf{
  template<class T> class Proxy;
  class StandardRecord;
  typedef Proxy<StandardRecord> SRProxy;
}

namespace ana
{
  typedef _Cut<caf::SRProxy> Cut;

  /// The simplest possible cut: pass everything, used as a default
  const Cut kNoCut(NoCut<caf::SRProxy>{});
}
