#include "CAFAna/Extrap/IExtrap.h"

#include "CAFAna/Core/LoadFromFile.h"

#include "TDirectory.h"
#include "TObjString.h"

#include <iostream>

// To implement LoadFrom()
#include "CAFAna/Extrap/TrivialExtrap.h"

namespace ana
{
  //----------------------------------------------------------------------
  // Definition to satisfy declaration in Core/LoadFromFile.h
  template<> std::unique_ptr<IExtrap> LoadFrom<IExtrap>(TDirectory* dir, const std::string& name)
  {
    TObjString* ptag = (TObjString*)dir->Get((name+"/type").c_str());
    assert(ptag);

    const TString tag = ptag->GetString();

    if(tag == "TrivialExtrap") return TrivialExtrap::LoadFrom(dir, name);

    std::cerr << "Unknown Extrap type '" << tag << "'" << std::endl;
    abort();
  }

  //----------------------------------------------------------------------
  void IExtrap::SaveTo(TDirectory* dir, const std::string& name) const
  {
    assert(0 && "Not implemented");
  }
}
