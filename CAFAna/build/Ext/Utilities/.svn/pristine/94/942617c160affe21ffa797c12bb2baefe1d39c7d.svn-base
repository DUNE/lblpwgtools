#pragma once

/// \file StanUtils.h  -- tools for use with Stan that don't have an obvious other home

#ifndef DARWINBUILD
#include "Utilities/func/Stan.h"
#endif //DARWINBUILD

class TH1;

namespace util
{
  // need a couple of functions so that we can
  // always get the value in functions
  // that are templated over both double and stan::math::var.
#ifndef DARWINBUILD
  /// Either no-op or convert stan::math::var to double.
  /// Exists so GetVal<T>(var) can be used with one signature inside a function templated over both stan::math::var & double.
  /// (see, e.g., CAFAna/Fit/StanFitter.cxx.)
  template <typename T>
  inline T GetValAs(const stan::math::var &val);

  /// explicit specialization for double return value
  template <>
  inline double GetValAs(const stan::math::var &val) {return val.val();}

  /// explicit specialization for stan::math::var return value
  template <>
  inline stan::math::var GetValAs(const stan::math::var &val) { return val; }
#endif // DARWINBUILD

  /// Companion to GetValAs<>(stan::math::var) to provide ability to use GetValAs<double>(double)
  /// without implicitly converting to a stan::math::var.
  // (templated so that it respects the same convention as the one above)
  template <typename T>
  inline typename std::enable_if<std::is_convertible<T, double>::value, T>::type GetValAs(double val)  { return val; }

#ifndef DARWINBUILD
  /// Replacement for operator=() making it clear that you're 'casting' across double-stan::math::var boundary
  inline void SetVal(double& target, const stan::math::var& source) {target = source.val();}
  inline void SetVal(stan::math::var& target, const double& source) {target = source;}
  inline void SetVal(stan::math::var& target, const stan::math::var& source) {target = source;}
#endif

  inline void SetVal(double& target, const double& source) {target = source;}

}
