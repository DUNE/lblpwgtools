#include "CAFAna/Decomp/IDecomp.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/LoadFromRegistry.h"

#include <cassert>

#include "TDirectory.h"
#include "TObjString.h"

#include <iostream>

namespace ana
{
  //----------------------------------------------------------------------
  Spectrum IDecomp::Data_Component() const
  { 
    std::cout<<"IDecomp::Data_Componet() is not implemented for this class, check your Decomp"<<std::endl;
    abort();
  }

  Spectrum IDecomp::MC_NCTotalComponent()  const
  {
    std::cout << "IDecomp: not implemented" << std::endl; abort();
  }
  Spectrum IDecomp::MC_NCComponent()       const
  {
    std::cout << "IDecomp: not implemented" << std::endl; abort();
  }
  Spectrum IDecomp::MC_NCAntiComponent()   const
  {
    std::cout << "IDecomp: not implemented" << std::endl; abort();
  }
  Spectrum IDecomp::MC_NumuComponent()     const
  {
    std::cout << "IDecomp: not implemented" << std::endl; abort();
  }
  Spectrum IDecomp::MC_AntiNumuComponent() const
  {
    std::cout << "IDecomp: not implemented" << std::endl; abort();
  }
  Spectrum IDecomp::MC_NueComponent()      const
  {
    std::cout << "IDecomp: not implemented" << std::endl; abort();
  }
  Spectrum IDecomp::MC_AntiNueComponent()  const
  {
    std::cout << "IDecomp: not implemented" << std::endl; abort();
  }

  //----------------------------------------------------------------------
  // Definition to satisfy the declaration in Core/LoadFromFile.h
  template<> std::unique_ptr<IDecomp> LoadFrom<IDecomp>(TDirectory* dir, const std::string& name)
  {
    TObjString* ptag = (TObjString*)dir->Get((name+"/type").c_str());
    assert(ptag);

    const TString tag = ptag->GetString();

    const auto func = LoadFromRegistry<IDecomp>::Get(tag.Data());
    if(func) return func(dir, name);

    std::cerr << "Unknown Decomp type '" << tag << "'" << std::endl;
    abort();
  }
}
