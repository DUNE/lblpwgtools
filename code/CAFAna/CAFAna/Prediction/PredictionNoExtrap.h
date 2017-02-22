#include "CAFAna/Prediction/PredictionExtrap.h"

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
    PredictionNoExtrap(DUNERunPOTSpectrumLoader& loaderBeam,
                       DUNERunPOTSpectrumLoader& loaderNue,
                       DUNERunPOTSpectrumLoader& loaderNuTau,
                       DUNERunPOTSpectrumLoader& loaderNC,
                       const std::string& label,
                       const Binning& bins,
                       const Var& var,
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

    PredictionNoExtrap(SpectrumLoaderBase& loader,
                       SpectrumLoaderBase& loaderSwap,
                       SpectrumLoaderBase& loaderTau,
                       const std::string& label,
                       const Binning& bins,
                       const Var& var,
                       const Cut& cut,
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    PredictionNoExtrap(SpectrumLoaderBase& loader,
                       SpectrumLoaderBase& loaderSwap,
                       const std::string& label,
                       const Binning& bins,
                       const Var& var,
                       const Cut& cut,
                       const SystShifts& shift = kNoShift,
                       const Var& wei = kUnweighted);

    virtual ~PredictionNoExtrap();

    static std::unique_ptr<PredictionNoExtrap> LoadFrom(TDirectory* dir);
    virtual void SaveTo(TDirectory* dir) const;

  };
}
