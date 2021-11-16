/////////////////////////////////////////////////////////////////////////////////
// FDPredictionSterile.h
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, January 2021
//
// Provide oscillated FD predictions in a BSM oscillation framework.
// Owns and interprets an FDExtrap object.
// Adapted from NOvA sterile oscillation framework.
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CAFAna/Prediction/IPrediction.h"

namespace ana {

  class FDExtrap;

  /// Take the output of an extrapolation and oscillate it as required
  class FDPredictionSterile: public IPrediction {
  public:

    FDPredictionSterile(FDExtrap* extrap);
    virtual ~FDPredictionSterile();

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

    static std::unique_ptr<FDPredictionSterile> LoadFrom(TDirectory* dir, const std::string& name);

    FDPredictionSterile() = delete;

  protected:

    FDExtrap* fFDExtrap;

  };

}
