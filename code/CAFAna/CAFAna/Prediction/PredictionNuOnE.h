#pragma once

#include "CAFAna/Prediction/IPrediction.h"

namespace ana
{
  class PredictionNuOnE: public IPrediction
  {
  public:
    PredictionNuOnE(SpectrumLoaderBase& sigLoader,
                    SpectrumLoaderBase& ccBkgLoader,
                    SpectrumLoaderBase& ncBkgLoader,
                    const SystShifts& shift = kNoShift,
                    const Var& wei = kUnweighted);

    virtual Spectrum Predict(osc::IOscCalculator*) const override;

    // Not implemented
    virtual Spectrum PredictComponent(osc::IOscCalculator* calc,
                                      Flavors::Flavors_t flav,
                                      Current::Current_t curr,
                                      Sign::Sign_t sign) const override;

    virtual void SaveTo(TDirectory* dir) const override;
    static std::unique_ptr<PredictionNuOnE> LoadFrom(TDirectory* dir);

  protected:
    PredictionNuOnE(const Spectrum& sig, const Spectrum& ccbkg, const Spectrum& ncbkg) : fSig(sig), fCCBkg(ccbkg), fNCBkg(ncbkg) {}

    Spectrum fSig, fCCBkg, fNCBkg;
  };

}
