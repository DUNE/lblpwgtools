#pragma once

#include "CAFAna/Prediction/IPrediction.h"

#include "CAFAna/Prediction/PredictionGenerator.h"

#include "CAFAna/Core/IRecordSource.h"

namespace ana
{
  /// Jam the necessary loaders into approximately the expected structure
  class NuOnELoaders // : public Loaders
  {
  public:
    NuOnELoaders(IRecordSource& sig,
                 IRecordSource& ccBkg,
                 IRecordSource& ncBkg)
      : fSig(sig), fCCBkg(ccBkg), fNCBkg(ncBkg)
    {
    }

    IRecordSource& Signal() {return fSig;}
    IRecordSource& CCBkg() {return fCCBkg;}
    IRecordSource& NCBkg() {return fNCBkg;}

    // TODO TODO TODO    void Go() {fSig.Go(); fCCBkg.Go(); fNCBkg.Go();}
  protected:
    IRecordSource &fSig, &fCCBkg, &fNCBkg;
  };


  class PredictionNuOnE: public IPrediction
  {
  public:
    PredictionNuOnE(IRecordSource& sigSrc,
                    IRecordSource& ccBkgSrc,
                    IRecordSource& ncBkgSrc);

    PredictionNuOnE(NuOnELoaders& loaders)
      : PredictionNuOnE(loaders.Signal(), loaders.CCBkg(), loaders.NCBkg())
    {
    }

    virtual Spectrum Predict(osc::IOscCalc*) const override;

    /// We call signal numu CC and CC bkg nue CC for this function's purposes
    virtual Spectrum PredictComponent(osc::IOscCalc* calc,
                                      Flavors::Flavors_t flav,
                                      Current::Current_t curr,
                                      Sign::Sign_t sign) const override;

    virtual void SaveTo(TDirectory* dir, const std::string& name) const override;
    static std::unique_ptr<PredictionNuOnE> LoadFrom(TDirectory* dir, const std::string& name);

    Spectrum Signal() const {return fSig;}
    Spectrum CCBkg() const {return fCCBkg;}
    Spectrum NCBkg() const {return fNCBkg;}

  protected:
    PredictionNuOnE(const Spectrum& sig, const Spectrum& ccbkg, const Spectrum& ncbkg) : fSig(sig), fCCBkg(ccbkg), fNCBkg(ncbkg) {}

    Spectrum fSig, fCCBkg, fNCBkg;
  };

  /* TODO think about generators
  class PredictionNuOnEGenerator: public IPredictionGenerator
  {
  public:
    PredictionNuOnEGenerator(Weight wei = kUnweighted) : fWei(wei)
    {
    }

    virtual std::unique_ptr<IPrediction>
    Generate(Loaders& loaders,
             const SystShifts& shiftMC = kNoShift) const override;

  protected:
    Weight fWei;
  };
  */
}
