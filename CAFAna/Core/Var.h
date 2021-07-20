#pragma once

#include "CAFAnaCore/CAFAna/Core/Var.h"

#include "StandardRecord/Proxy/FwdDeclare.h"

namespace ana
{
  /// \brief Representation of a variable to be retrieved from a \ref
  /// caf::StandardRecord object
  ///
  /// A Var consists of a function, taking a StandardRecord and returning the
  /// value of the variable (which may be some complicated function).
  using Var = _Var<caf::SRProxy>;

  /// \brief For Vars where literally all you need is a single CAF variable
  ///
  /// eg Var myVar = SIMPLEVAR(my.var.str);
  /// NB lack of quotes quotes around my.var.str
#define SIMPLEVAR(CAFNAME) Var([](const caf::SRProxy* sr){return sr->CAFNAME;})

  inline Var Constant(double v){return Var([v](const caf::SRProxy*){return v;});}
}
