#pragma once

namespace util
{
  /// More efficient square function than pow(x,2)
  template<class T> inline T sqr(T x){return x*x;}

  /// More efficient cube function than pow(x,3)
  template<class T> inline T cube(T x){return x*x*x;}
}
