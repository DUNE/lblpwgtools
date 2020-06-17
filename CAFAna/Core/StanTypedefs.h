///  StanTypedefs.h:
///    Typedefs of various types templated over stan::math::var,
///    centralized here for convenience.
///    They don't get put in the header files associated with the types themselves
///    because there's an annoying chain of declarations needed for the typedef'ing,
///    and it's much easier to maintain if it's in a single place.
#pragma once

namespace stan
{
  namespace math
  {
    class var;
  }
}

namespace osc
{
  template <typename T> class _IOscCalculatorAdjustable;
  typedef _IOscCalculatorAdjustable<stan::math::var> IOscCalculatorAdjustableStan;

  template <typename T> class _IOscCalculator;
  typedef _IOscCalculator<stan::math::var> IOscCalculatorStan;

  template <typename T> class _OscCalculatorPMNS;
  typedef _OscCalculatorPMNS<stan::math::var> OscCalculatorPMNSStan;

  template <typename T> class _OscCalculatorPMNSOpt;
  typedef _OscCalculatorPMNSOpt<stan::math::var> OscCalculatorPMNSOptStan;

  template <typename T> class _OscCalculatorDMP;
  typedef _OscCalculatorDMP<stan::math::var> OscCalculatorDMPStan;

}

namespace ana
{
  // note: typedefs over forward-declared types are fragile.
  // if either the underlying type changes, its forward declaration
  // needs to be updated here.

  // ---------------------
  // vars
  template <typename T> class _IFitVar;
  typedef _IFitVar<stan::math::var> IFitVarStan;

  template <typename T> class _IConstrainedFitVar;
  typedef _IConstrainedFitVar<stan::math::var> IConstrainedFitVarStan;

  // ---------------------
  // experiments
  template <typename T> class _ILkhdExperiment;
  typedef _ILkhdExperiment<stan::math::var> ILkhdExperiment;

}
