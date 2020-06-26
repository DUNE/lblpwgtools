#include "CAFAna/Prediction/IPrediction.h"

#include "CAFAna/Core/LoadFromFile.h"

#include "OscLib/func/IOscCalculator.h"

#include <cassert>
#include <iostream>

#include "TDirectory.h"
#include "TObjString.h"

// To implement LoadFrom()
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Prediction/PredictionInterp.h"
#include "CAFAna/Prediction/PredictionNoOsc.h"
#include "CAFAna/Prediction/PredictionScaleComp.h"
#include "CAFAna/Prediction/PredictionNuOnE.h"

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

    if(tag == "PredictionNoExtrap") return PredictionNoExtrap::LoadFrom(dir, name);

    // Backwards compatibility
    if(tag == "PredictionInterp" ||
       tag == "PredictionInterp2") return PredictionInterp::LoadFrom(dir, name);

    if(tag == "PredictionNoOsc") return PredictionNoOsc::LoadFrom(dir, name);

    if(tag == "PredictionScaleComp") return PredictionScaleComp::LoadFrom(dir, name);

    if(tag == "PredictionNuOnE") return PredictionNuOnE::LoadFrom(dir, name);

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
  Spectrum IPrediction::Predict(osc::IOscCalculatorStan *calc) const
  {
    assert(0 && "This Prediction hasn't implemented a Stan-aware Predict()!");
    return Predict(calc);  // prevent compiler warning
  }
  //----------------------------------------------------------------------
  Spectrum IPrediction::PredictSyst(osc::IOscCalculator* calc,
                                    const SystShifts& syst) const
  {
    assert(syst.IsNominal() && "This Prediction doesn't support PredictSyst(). Did you just mean Predict()?");

    // Default implementation: no treatment of systematics
    return Predict(calc);
  }

  //----------------------------------------------------------------------
  Spectrum IPrediction::PredictSyst(osc::IOscCalculatorStan* calc,
                                    const SystShifts& syst) const
  {
    assert(syst.IsNominal() && "This Prediction doesn't support PredictSyst(). Did you just mean Predict()?");

    // Default implementation: no treatment of systematics
    return Predict(calc);
  }

  //----------------------------------------------------------------------
  // placeholder method that should be overridden by Stan-aware concrete Prediction classes
  Spectrum IPrediction::PredictComponent(osc::IOscCalculatorStan *calc,
                                         Flavors::Flavors_t flav,
                                         Current::Current_t curr,
                                         Sign::Sign_t sign) const
  {
    assert(0 && "This Prediction hasn't implemented a Stan-aware PredictComponent()!");
    return PredictComponent(calc, flav, curr, sign);  // prevent compiler warning
  }

  //----------------------------------------------------------------------
  Spectrum IPrediction::PredictComponentSyst(osc::IOscCalculator* calc,
                                             const SystShifts& syst,
                                             Flavors::Flavors_t flav,
                                             Current::Current_t curr,
                                             Sign::Sign_t sign) const
  {
    assert(syst.IsNominal() && "This Prediction doesn't support PredictSyst(). Did you just mean Predict()?");

    // Default implementation: no treatment of systematics
    return PredictComponent(calc, flav, curr, sign);
  }

  //----------------------------------------------------------------------
  Spectrum IPrediction::PredictComponentSyst(osc::IOscCalculatorStan* calc,
                                             const SystShifts& syst,
                                             Flavors::Flavors_t flav,
                                             Current::Current_t curr,
                                             Sign::Sign_t sign) const
  {
    assert(syst.IsNominal() && "This Prediction doesn't support PredictSyst(). Did you just mean Predict()?");

    // Default implementation: no treatment of systematics
    return PredictComponent(calc, flav, curr, sign);
  }

  //----------------------------------------------------------------------
  void IPrediction::SaveTo(TDirectory*, const std::string&) const
  {
    assert(0 && "Not implemented");
  }
}
