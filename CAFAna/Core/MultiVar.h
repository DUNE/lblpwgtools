#pragma once

#include "CAFAnaCore/CAFAna/Core/MultiVar.h"

namespace caf{
  template<class T> class Proxy;
  class StandardRecord;
  typedef Proxy<StandardRecord> SRProxy;
}

namespace ana
{
  typedef _MultiVar<caf::SRProxy> MultiVar;
}
