#include "CAFAna/Experiment/IChiSqExperiment.h"

#include "CAFAna/Core/Utilities.h"

#include "TFile.h"
#include "TObjString.h"

#include <cassert>
#include <iostream>

// To implement LoadFrom()
#include "CAFAna/Experiment/CountingExperiment.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Experiment/SolarConstraints.h"
#include "CAFAna/Experiment/MultiExperiment.h"
#include "CAFAna/Experiment/ReactorExperiment.h"

namespace ana
{
  //----------------------------------------------------------------------
  // Definition to satisfy declaration in Core/LoadFromFile.h
  template<> std::unique_ptr<IChiSqExperiment> LoadFrom<IChiSqExperiment>(TDirectory* dir, const std::string& name)
  {
    TObjString* ptag = (TObjString*)dir->Get((name+"/type").c_str());
    assert(ptag);

    const TString tag = ptag->GetString();
    delete ptag;

    if(tag == "CountingExperiment") return CountingExperiment::LoadFrom(dir, name);
    if(tag == "ReactorExperiment") return ReactorExperiment::LoadFrom(dir, name);
    if(tag == "SingleSampleExperiment") return SingleSampleExperiment::LoadFrom(dir, name);
    if(tag == "SolarConstraints") return SolarConstraints::LoadFrom(dir, name);
    if(tag == "MultiExperiment") return MultiExperiment::LoadFrom(dir, name);

    std::cerr << "Unknown Experiment type '" << tag << "'" << std::endl;
    abort();
  }

  //----------------------------------------------------------------------
  void IChiSqExperiment::SaveTo(TDirectory* dir, const std::string& name) const
  {
    assert(0 && "Not implemented");
  }
}
