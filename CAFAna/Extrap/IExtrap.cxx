#include "CAFAna/Extrap/IExtrap.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/LoadFromRegistry.h"

#include "TDirectory.h"
#include "TObjString.h"

#include <iostream>

namespace ana
{
  //----------------------------------------------------------------------
  // Definition to satisfy declaration in Core/LoadFromFile.h
  template<> std::unique_ptr<IExtrap> LoadFrom<IExtrap>(TDirectory* dir, const std::string& name)
  {
    TObjString* ptag = (TObjString*)dir->Get((name+"/type").c_str());
    assert(ptag);

    const TString tag = ptag->GetString();
    delete ptag;

    const auto func = LoadFromRegistry<IExtrap>::Get(tag.Data());
    if(func) return func(dir, name);

    std::cerr << "Unknown Extrap type '" << tag << "'" << std::endl;
    abort();
  }

  //----------------------------------------------------------------------
  void IExtrap::SaveTo(TDirectory* dir, const std::string& name) const
  {
    assert(0 && "Not implemented");
  }
}
