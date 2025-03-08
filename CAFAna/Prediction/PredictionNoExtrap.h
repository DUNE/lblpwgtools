#pragma once

#include "CAFAna/Prediction/PredictionExtrap.h"

#include "CAFAna/Prediction/PredictionGenerator.h"

#include "CAFAna/Core/IRecordSource.h"

namespace ana {
class Loaders;

/// Prediction that just uses FD MC, with no extrapolation
class PredictionNoExtrap : public PredictionExtrap {
public:
  PredictionNoExtrap(PredictionExtrap *pred);
  PredictionNoExtrap(IExtrap *extrap);

  PredictionNoExtrap(IInteractionSource& loaderNonswap,
                     IInteractionSource& loaderNue,
                     IInteractionSource& loaderNuTau,
                     const HistAxis& axis);

  //  PredictionNoExtrap(Loaders &loaders, const HistAxis &axis);

  virtual ~PredictionNoExtrap();

  static std::unique_ptr<PredictionNoExtrap> LoadFrom(TDirectory *dir, const std::string& name);
  virtual void SaveTo(TDirectory *dir, const std::string& name) const override;
};


  //---------------------------------------------------------------------------

  /// Generates FD-only predictions (no extrapolation)
  class NoExtrapGenerator: public IPredictionGenerator
  {
    public:
    NoExtrapGenerator(const HistAxis axis, const Cut cut, const Weight wei = kUnweighted );

    std::unique_ptr<IPrediction> Generate(Loaders& loaders, const ana::RecoType & ixnRecoType, const SystShifts& shiftMC = kNoShift) const override;

protected:
  HistAxis fAxis;
  Cut fCut;
  Weight fWei;
};
  */
    private:
    const HistAxis fAxis;
    const Cut fCut;
    const Weight fWei;
  };

} // namespace ana
