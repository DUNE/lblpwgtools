/// \file FwdDeclare.h -- consolidate forward declarations of template types
///                       and associated typdefs into one place so that they
///                       can be easily updated if needed.

#pragma once

namespace osc
{
  template<class T> class _IOscCalc;
  template<class T> class _IOscCalcAdjustable;

  typedef _IOscCalc<double> IOscCalc;
  typedef _IOscCalcAdjustable<double> IOscCalcAdjustable;
}

namespace caf
{
  class StandardRecord;
  template<class T> class Proxy;
  typedef Proxy<StandardRecord> SRProxy;
}

namespace ana
{
  template <class T> class GenericVar;
  typedef GenericVar<caf::SRProxy> Var;

  template <class T> class GenericCut;
  typedef GenericCut<caf::SRProxy> Cut;

  class SystShifts;
}

class TArrayD;
template <class T> class THnSparseT;
typedef THnSparseT<TArrayD> THnSparseD;
