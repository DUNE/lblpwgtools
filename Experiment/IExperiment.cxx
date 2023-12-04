#include "CAFAna/Experiment/IExperiment.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/LoadFromRegistry.h"
#include "CAFAna/Core/Utilities.h"

#include "TFile.h"
#include "TObjString.h"

#include <cassert>
#include <iostream>

namespace ana
{
  //----------------------------------------------------------------------
  // Definition to satisfy declaration in Core/LoadFromFile.h
  template<> std::unique_ptr<IExperiment> LoadFrom<IExperiment>(TDirectory* dir, const std::string& name)
  {
    TObjString* ptag = (TObjString*)dir->Get((name+"/type").c_str());
    assert(ptag);

    const TString tag = ptag->GetString();
    delete ptag;

    const auto func = LoadFromRegistry<IExperiment>::Get(tag.Data());
    if(func) return func(dir, name);

    std::cerr << "Unknown Experiment type '" << tag << "'" << std::endl;
    abort();
  }

  //----------------------------------------------------------------------
  void IExperiment::SaveTo(TDirectory* dir, const std::string& name) const
  {
    assert(0 && "Not implemented");
  }
}
