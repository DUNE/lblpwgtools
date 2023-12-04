#include "CAFAna/Decomp/IDecomp.h"

#include "CAFAna/Core/LoadFromFile.h"

#include <cassert>

#include "TDirectory.h"
#include "TObjString.h"

// So we can implement LoadFrom()
#include "CAFAna/Decomp/NumuDecomp.h"

namespace ana
{
  //----------------------------------------------------------------------
  // Definition to satisfy the declaration in Core/LoadFromFile.h
  template<> std::unique_ptr<IDecomp> LoadFrom<IDecomp>(TDirectory* dir, const std::string& name)
  {
    TObjString* ptag = (TObjString*)dir->Get((name+"/type").c_str());
    assert(ptag);

    const TString tag = ptag->GetString();

    if(tag == "NumuDecomp") return NumuDecomp::LoadFrom(dir, name);

    std::cerr << "Unknown Decomp type '" << tag << "'" << std::endl;
    abort();
  }
}
