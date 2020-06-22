#include "CAFAna/Experiment/IExperiment.h"
#include "CAFAna/Experiment/ILkhdExperiment.h"

namespace ana
{
  // someday...
//  template<> std::unique_ptr<ILkhdExperiment> LoadFrom<ILkhdExperiment>(TDirectory* dir)
//  {
//    TObjString* ptag = (TObjString*)dir->Get("type");
//    assert(ptag);
//
//    const TString tag = ptag->GetString();
//
//    if(tag == "SomeExperiment") return CountingExperiment::LoadFrom(dir);
//
//    std::cerr << "Unknown Experiment type '" << tag << "'" << std::endl;
//    abort();
//  }

  double ChiSqToLklhdExptWrapper::LogLikelihood(osc::IOscCalculatorAdjustable *osc,
                                                const SystShifts &shifts) const
  {
    return fChiSqExpt.ChiSq(osc, shifts)/-2.;
  }
}