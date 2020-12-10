#pragma once

#include "CAFAnaCore/CAFAna/Core/Var.h"

namespace caf{
  template<class T> class Proxy;
  class StandardRecord;
  typedef Proxy<StandardRecord> SRProxy;
}

namespace ana
{
  /// \brief Representation of a variable to be retrieved from a \ref
  /// caf::StandardRecord object
  ///
  /// A Var consists of a function, taking a StandardRecord and returning the
  /// value of the variable (which may be some complicated function).
  typedef _Var<caf::SRProxy> Var;

  /// \brief For Vars where literally all you need is a single CAF variable
  ///
  /// eg Var myVar = SIMPLEVAR(my.var.str);
  /// NB lack of quotes quotes around my.var.str
#define SIMPLEVAR(CAFNAME) Var([](const caf::SRProxy* sr){return sr->CAFNAME;})

  /// The simplest possible Var, always 1. Used as a default weight.
  const Var kUnweighted = Unweighted<caf::SRProxy>();

  inline Var Constant(double v){return Var([v](const caf::SRProxy*){return v;});}
}
