#pragma once

#include <map>
#include <string>

namespace ana
{
  template<class T> class Registry
  {
  public:
    static void Register(const T* s);

    static void UnRegister(const T* s);

    static const T* ShortNameToPtr(const std::string& s,
                                   bool allowFail = false);

    static void Print();
  protected:
    static std::map<std::string, const T*>& Map();
  };
}
