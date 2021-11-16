/////////////////////////////////////////////////////////////////////////////////
// NDPredictionSterile.h
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, January 2021
//
// Provide oscillated ND predictions.  Owns and interprets an NDExtrap object.
// Adapted from NOvA sterile oscillation framework.
/////////////////////////////////////////////////////////////////////////////////

#pragma once

// framework
#include "CAFAna/Prediction/IPrediction.h"

namespace ana {

  class NDExtrap;

  /// Take the output of an extrapolation and oscillate it as required
  class NDPredictionSterile: public IPrediction {
  public:
    NDPredictionSterile(NDExtrap* extrap);
    virtual ~NDPredictionSterile();

    // un-hide inherited method stubs so we don't get warnings from the compiler
    using IPrediction::Predict;
    using IPrediction::PredictComponent;
    using IPrediction::PredictSyst;

    virtual Spectrum Predict(osc::IOscCalc* calc) const override;

    virtual Spectrum PredictComponent(osc::IOscCalc* calc,
				      Flavors::Flavors_t flav,
				      Current::Current_t curr,
				      Sign::Sign_t sign) const override;

    virtual Spectrum FakeData(osc::IOscCalc* calc, double POT);

    virtual void SaveTo(TDirectory* dir, const std::string& name) const override;

    static std::unique_ptr<NDPredictionSterile> LoadFrom(TDirectory* dir, const std::string& name);

    NDPredictionSterile() = delete;

  protected:
    NDExtrap* fNDExtrap;

  };

}
