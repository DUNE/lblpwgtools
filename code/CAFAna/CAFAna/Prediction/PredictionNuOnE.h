#pragma once

#include "CAFAna/Prediction/IPrediction.h"

#include "CAFAna/Prediction/PredictionGenerator.h"

#include "CAFAna/Core/Loaders.h"

namespace ana
{
  /// Jam the necessary loaders into approximately the expected structure
  class NuOnELoaders: public Loaders
  {
  public:
    NuOnELoaders(SpectrumLoaderBase& sig,
                 SpectrumLoaderBase& ccBkg,
                 SpectrumLoaderBase& ncBkg)
      : fSig(sig), fCCBkg(ccBkg), fNCBkg(ncBkg)
    {
    }

    SpectrumLoaderBase& Signal() {return fSig;}
    SpectrumLoaderBase& CCBkg() {return fCCBkg;}
    SpectrumLoaderBase& NCBkg() {return fNCBkg;}

    void Go() {fSig.Go(); fCCBkg.Go(); fNCBkg.Go();}
  protected:
    SpectrumLoaderBase &fSig, &fCCBkg, &fNCBkg;
  };


  class PredictionNuOnE: public IPrediction
  {
  public:
    PredictionNuOnE(SpectrumLoaderBase& sigLoader,
                    SpectrumLoaderBase& ccBkgLoader,
                    SpectrumLoaderBase& ncBkgLoader,
                    const SystShifts& shift = kNoShift,
                    const Var& wei = kUnweighted);

    PredictionNuOnE(NuOnELoaders& loaders,
                    const SystShifts& shift = kNoShift,
                    const Var& wei = kUnweighted)
      : PredictionNuOnE(loaders.Signal(), loaders.CCBkg(), loaders.NCBkg(),
                        shift, wei)
    {
    }

    virtual Spectrum Predict(osc::IOscCalculator*) const override;

    /// We call signal numu CC and CC bkg nue CC for this function's purposes
    virtual Spectrum PredictComponent(osc::IOscCalculator* calc,
                                      Flavors::Flavors_t flav,
                                      Current::Current_t curr,
                                      Sign::Sign_t sign) const override;

    virtual void SaveTo(TDirectory* dir) const override;
    static std::unique_ptr<PredictionNuOnE> LoadFrom(TDirectory* dir);

    Spectrum Signal() const {return fSig;}
    Spectrum CCBkg() const {return fCCBkg;}
    Spectrum NCBkg() const {return fNCBkg;}

  protected:
    PredictionNuOnE(const Spectrum& sig, const Spectrum& ccbkg, const Spectrum& ncbkg) : fSig(sig), fCCBkg(ccbkg), fNCBkg(ncbkg) {}

    Spectrum fSig, fCCBkg, fNCBkg;
  };


  class PredictionNuOnEGenerator: public IPredictionGenerator
  {
  public:
    PredictionNuOnEGenerator(Var wei = kUnweighted) : fWei(wei)
    {
    }

    virtual std::unique_ptr<IPrediction>
    Generate(Loaders& loaders,
             const SystShifts& shiftMC = kNoShift) const override;

  protected:
    Var fWei;
  };
}
