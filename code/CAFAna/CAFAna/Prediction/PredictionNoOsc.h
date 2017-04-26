#include "CAFAna/Prediction/IPrediction.h"

namespace ana
{
  class Loaders;

  class DUNERunPOTSpectrumLoader;

  /// Prediction that wraps a simple Spectrum
  class PredictionNoOsc: public IPrediction
  {
  public:
    PredictionNoOsc(SpectrumLoaderBase& loader,
                    const HistAxis& axis,
                    const Cut& cut,
                    const SystShifts& shift = kNoShift,
                    const Var& wei = kUnweighted);

    PredictionNoOsc(SpectrumLoaderBase& loader,
                    const std::string& label,
                    const Binning& bins,
                    const Var& var,
                    const Cut& cut,
                    const SystShifts& shift = kNoShift,
                    const Var& wei = kUnweighted);

    static std::unique_ptr<PredictionNoOsc> LoadFrom(TDirectory* dir);
    virtual void SaveTo(TDirectory* dir) const;

    virtual Spectrum Predict(osc::IOscCalculator* calc) const override
    {
      return fSpectrum;
    }

    virtual Spectrum PredictComponent(osc::IOscCalculator* calc,
                                      Flavors::Flavors_t flav,
                                      Current::Current_t curr,
                                      Sign::Sign_t sign) const override
    {
      assert(0 && "PredictionNoOsc::PredictComponent() not implemented");
    }

  protected:
    PredictionNoOsc(const Spectrum& s) : fSpectrum(s) {}

    Spectrum fSpectrum;
  };
}
