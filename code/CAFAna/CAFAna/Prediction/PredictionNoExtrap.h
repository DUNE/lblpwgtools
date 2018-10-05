#include "CAFAna/Prediction/PredictionExtrap.h"

#include "CAFAna/Prediction/PredictionGenerator.h"

namespace ana
{
  class Loaders;

  class DUNERunPOTSpectrumLoader;

  /// Prediction that just uses FD MC, with no extrapolation
  class PredictionNoExtrap: public PredictionExtrap
  {
  public:
    PredictionNoExtrap(PredictionExtrap* pred);

    // This is the DUNE constructor
    PredictionNoExtrap(SpectrumLoaderBase& loaderNumu,
                       SpectrumLoaderBase& loaderNue,
                       SpectrumLoaderBase& loaderNuTau,
                       const std::string& label,
                       const Binning& bins,
                       const Var& var,
                       const Cut& cut,                       
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    PredictionNoExtrap(SpectrumLoaderBase& loaderNumu,
                       SpectrumLoaderBase& loaderNue,
                       SpectrumLoaderBase& loaderNuTau,
		       const HistAxis& axis,
		       const Cut& cut,
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    PredictionNoExtrap(Loaders& loaders,
                       const std::string& label,
                       const Binning& bins,
                       const Var& var,
                       const Cut& cut,
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    PredictionNoExtrap(Loaders& loaders,
                       const HistAxis& axis,
                       const Cut& cut,
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    virtual ~PredictionNoExtrap();

    static std::unique_ptr<PredictionNoExtrap> LoadFrom(TDirectory* dir);
    virtual void SaveTo(TDirectory* dir) const override;

  };

  class NoExtrapPredictionGenerator: public IPredictionGenerator
  {
  public:
    NoExtrapPredictionGenerator(HistAxis axis, Cut cut)
      : fAxis(axis), fCut(cut)
    {
    }

    virtual std::unique_ptr<IPrediction>
    Generate(Loaders& loaders, const SystShifts& shiftMC = kNoShift) const override
    {
      return std::unique_ptr<IPrediction>(
                                          new PredictionNoExtrap(loaders, fAxis, fCut, shiftMC));
    }

  protected:
    HistAxis fAxis;
    Cut fCut;
  };
}
