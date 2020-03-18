#include "CAFAna/Core/Registry.h"

#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include <iostream>

namespace ana
{
  //----------------------------------------------------------------------
  template<class T> std::map<std::string, const T*>& Registry<T>::Map()
  {
    // https://isocpp.org/wiki/faq/ctors#static-init-order
    static auto m = new std::map<std::string, const T*>;
    return *m;
  }

  //----------------------------------------------------------------------
  template<class T> void Registry<T>::Register(const T* s)
  {
    const std::string name = s->ShortName();

    // uncomment if you need to work out what's going on under the hood
    // std::cout << "Registering " << DemangledTypeName<T>() << " named " << s->ShortName() << std::endl;

    if (Map().count(name)){
      std::cout << "Registry: Error: " << DemangledTypeName(s)
                << " '" << name << "' registered multiple times." << std::endl
                << "Check you declared it with 'extern' in the .h and "
                << "instantiated it in the .cxx and check for other  "
                << DemangledTypeName(s)
                << "s that happen to have the same name." << std::endl;
      // Store an entry there, so we can detect further duplicates, but make it
      // invalid.
      throw name;
    }

    Map()[name] = s;
  }

  //----------------------------------------------------------------------
  template<class T> void Registry<T>::UnRegister(const T* s)
  {
    auto it = Map().find(s->ShortName());

    if(it == Map().end()){
      std::cout << "Registry: Error: unregistering " << DemangledTypeName(s)
                << " '" << s->ShortName() << "' that was never registered!"
                << std::endl;
      return;
    }

    // If was multiply registered, leave that signal
    if(it->second) Map().erase(it);
  }

  //----------------------------------------------------------------------
  template<class T> const T* Registry<T>::ShortNameToPtr(const std::string& s,
                                                         bool allowFail)
  {
    auto it = Map().find(s);
    if (it == Map().end()){
      if (allowFail) return 0;
      std::cout << "Registry: Error: "
                << "Object with short name '" << s << "' not found. "
                << "Pass allowFail=true to return NULL in this case."
                << std::endl;

//      std::cout << "I know about the following " << DemangledTypeName<T>() << "s:" << std::endl;
      for (const auto &o : Map())
        std::cout << "  " << o.first << std::endl;
      abort();
    }

    const T* ret = it->second;

    if (!ret){
      std::cout << "Registry: Error: object '" << s
                << "' was registered multiple times. Refusing to return a"
                << " random instance. Go fix the syst registration."
                << std::endl;
      abort();
    }

    return ret;
  }

  //----------------------------------------------------------------------
  template<class T> void Registry<T>::Print()
  {
    std::cout << Map().size() << " Registry contains pointers:" << std::endl;
    for (auto it: Map())
    {
      std::cout << it.first << " :\t";

      if (it.second)
      {
        std::cout << DemangledTypeName(it.second)
                  << " at " << &it.second << std::endl;
      } else
      {
        std::cout << "MULTIPLY REGISTERED!" << std::endl;
      }
    }
  }

  //----------------------------------------------------------------------
  template class Registry<IFitVar>;
  template class Registry<ISyst>;
}
