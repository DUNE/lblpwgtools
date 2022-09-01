#include "CAFAna/Prediction/IPrediction.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/LoadFromRegistry.h"

#include "OscLib/IOscCalc.h"

#include <cassert>
#include <iostream>

#include "TDirectory.h"
#include "TObjString.h"

namespace ana
{
  //----------------------------------------------------------------------
  // Definition to satisfy declaration in Core/LoadFromFile.h
  template<> std::unique_ptr<IPrediction> LoadFrom<IPrediction>(TDirectory* dir, const std::string& name)
  {
    TObjString* ptag = (TObjString*)dir->Get((name+"/type").c_str());
    assert(ptag);
    const TString tag = ptag->GetString();
    delete ptag;

    const auto func = LoadFromRegistry<IPrediction>::Get(tag.Data());
    if(func) return func(dir, name);

    std::cerr << "Unknown Prediction type '" << tag << "'" << std::endl;
    abort();
  }

  //----------------------------------------------------------------------
  Spectrum IPrediction::PredictUnoscillated() const
  {
    // Default implementation
    osc::NoOscillations noosc;
    return Predict(&noosc);
  }

  //----------------------------------------------------------------------
  // placeholder method that should be overridden by Stan-aware concrete Prediction classes
  Spectrum IPrediction::Predict(osc::IOscCalcStan *calc) const
  {
    std::cout << "This Prediction hasn't implemented a Stan-aware Predict()!" << std::endl;
    abort();
  }
  //----------------------------------------------------------------------
  Spectrum IPrediction::PredictSyst(osc::IOscCalc* calc,
                                    const SystShifts& syst) const
  {
    if(!syst.IsNominal()){
      std::cout << "This Prediction doesn't support PredictSyst(). Did you just mean Predict()?" << std::endl;
      abort();
    }

    // Default implementation: no treatment of systematics
    return Predict(calc);
  }

  //----------------------------------------------------------------------
  Spectrum IPrediction::PredictSyst(osc::IOscCalcStan* calc,
                                    const SystShifts& syst) const
  {
    if(!syst.IsNominal() || syst.HasAnyStan()){
      std::cout << "This Prediction hasn't implemented a Stan-aware PredictComponentSyst()!" << std::endl;
      abort();
    }

    // Default implementation: no treatment of systematics
    return Predict(calc);
  }

  //----------------------------------------------------------------------
  // placeholder method that should be overridden by Stan-aware concrete Prediction classes
  Spectrum IPrediction::PredictComponent(osc::IOscCalcStan *calc,
                                         Flavors::Flavors_t flav,
                                         Current::Current_t curr,
                                         Sign::Sign_t sign) const
  {
    std::cout << "This Prediction hasn't implemented a Stan-aware PredictComponent()!" << std::endl;
    abort();
  }

  //----------------------------------------------------------------------
  Spectrum IPrediction::PredictComponentSyst(osc::IOscCalc* calc,
                                             const SystShifts& syst,
                                             Flavors::Flavors_t flav,
                                             Current::Current_t curr,
                                             Sign::Sign_t sign) const
  {
    if(!syst.IsNominal()){
      std::cout << "This Prediction doesn't support PredictSyst(). Did you just mean Predict()?" << std::endl;
      abort();
    }

    // Default implementation: no treatment of systematics
    return PredictComponent(calc, flav, curr, sign);
  }

  //----------------------------------------------------------------------
  Spectrum IPrediction::PredictComponentSyst(osc::IOscCalcStan* calc,
                                             const SystShifts& syst,
                                             Flavors::Flavors_t flav,
                                             Current::Current_t curr,
                                             Sign::Sign_t sign) const
  {
    if(!syst.IsNominal() || syst.HasAnyStan()){
      std::cout << "This Prediction hasn't implemented a Stan-aware PredictComponentSyst()" << std::endl;
    }

    // Default implementation: no treatment of systematics
    return PredictComponent(calc, flav, curr, sign);
  }

  //----------------------------------------------------------------------
  OscillatableSpectrum IPrediction::ComponentCC(int from, int to) const
  {
    std::cout << "WARNING! ComponentCC is unimplemented in IPrediction" << std::endl; abort();
  }

  Spectrum IPrediction::ComponentNCTotal() const
  {
    std::cout << "WARNING! ComponentNCTotal is unimplemented in IPrediction" << std::endl; abort();
  }

  Spectrum IPrediction::ComponentNC() const
  {
    std::cout << "WARNING! ComponentNC is unimplemented in IPrediction" << std::endl; abort();
  }

  Spectrum IPrediction::ComponentNCAnti() const
  {
    std::cout << "WARNING! ComponentNCAnti is unimplemented in IPrediction" << std::endl; abort();
  }

  //----------------------------------------------------------------------
  void IPrediction::SaveTo(TDirectory*, const std::string&) const
  {
    assert(0 && "Not implemented");
  }
}
