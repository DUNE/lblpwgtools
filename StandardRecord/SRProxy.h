#pragma once

#include "StandardRecord/StandardRecord.h"

namespace caf
{
  /// cafanacore wants us to be using SRProxy. This is a do-nothing stand-on to
  /// satisfy it.
  template<class T> class Proxy: public T
  {
  public:
    Proxy(const T& t) : T(t) {}
  };

  typedef Proxy<StandardRecord> SRProxy;
}
