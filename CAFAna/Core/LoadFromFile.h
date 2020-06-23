#pragma once

#include <cassert>
#include <iostream>
#include <memory>
#include <string>

#include "TFile.h"

#include "CAFAna/Core/FwdDeclare.h"

class TDirectory;

namespace ana
{
  //----------------------------------------------------------------------
  // Most classes are happy to load themselves
  template<class T> std::unique_ptr<T> LoadFrom(TDirectory* dir, const std::string& label)
  {
    return T::LoadFrom(dir, label);
  }

  //----------------------------------------------------------------------
  // But if you're trying to load a base class we need to figure out which
  // derived class is actually in the file and hand off to that. The
  // implementations of these are in the cxx files for the base classes in
  // question.
  class IDecomp;
  template<> std::unique_ptr<IDecomp> LoadFrom<IDecomp>(TDirectory* dir, const std::string& label);
  class IExtrap;
  template<> std::unique_ptr<IExtrap> LoadFrom<IExtrap>(TDirectory* dir, const std::string& label);
  class IPrediction;
  template<> std::unique_ptr<IPrediction> LoadFrom<IPrediction>(TDirectory* dir, const std::string& label);
  class IExperiment;
  template<> std::unique_ptr<IExperiment> LoadFrom<IExperiment>(TDirectory* dir, const std::string& label);
  class ModularExtrapComponent;
  template<> std::unique_ptr<ModularExtrapComponent>
    LoadFrom<ModularExtrapComponent>(TDirectory* dir, const std::string& label);
  class IBkgdEstimator;
  template<> std::unique_ptr<IBkgdEstimator> LoadFrom<IBkgdEstimator>(TDirectory* dir, const std::string& label);

  // This one is actually implemented in LoadFromFile.cxx to avoid polluting
  // OscLib with CAFAna conventions.
  template<> std::unique_ptr<osc::IOscCalculator> LoadFrom<osc::IOscCalculator>(TDirectory* dir, const std::string& label);

  //----------------------------------------------------------------------
  // For symmetry
  template<class T> void SaveTo(const T& x, TDirectory* dir, const std::string& label)
  {
    x.SaveTo(dir, label);
  }

  // Also in the cxx, to avoid having to put this logic into OscLib
  template<> void SaveTo(const osc::IOscCalculator& x, TDirectory* dir, const std::string& label);

  //----------------------------------------------------------------------
  template<class T> std::unique_ptr<T> LoadFromFile(const std::string& fname,
                                                    const std::string& label)
  {
    TFile fin(fname.c_str());
    assert(!fin.IsZombie());
    return LoadFrom<T>(&fin, label);
  }

  //----------------------------------------------------------------------
  template<class T> void SaveToFile(const T& x,
                                    const std::string& fname,
                                    const std::string& label)
  {
    TFile fout(fname.c_str(), "RECREATE");
    x.SaveTo(&fout, label);
  }
}
