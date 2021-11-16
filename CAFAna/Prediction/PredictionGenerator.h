#pragma once

#include <memory>

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Weight.h"
//#include "CAFAna/Vars/GenieWeights.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Core/SpectrumLoader.h"

namespace ana
{

  class Loaders;
  class IPrediction;

  /** \brief Given loaders and an MC shift, Generate() generates an IPrediction

      All other arguments needed to construct the prediction are passed to the
      IPredictionGenerator constructor, and are identical for all generated
      predictions. For standalone use or use with PredictionInterp. **/
  class IPredictionGenerator
  {
    public:
    virtual std::unique_ptr<IPrediction> Generate(
    						  Loaders& loaders, const SystShifts& shiftMC = kNoShift ) const = 0;

  };

  //---------------------------------------------------------------------------

  /// Generates FD-only predictions (no extrapolation)
  class NoExtrapGenerator: public IPredictionGenerator
  {
    public:
    NoExtrapGenerator(
      const HistAxis axis,
      const Cut cut,
      const Weight wei = kUnweighted );

    std::unique_ptr<IPrediction> Generate(
    					  Loaders& loaders,
					  const SystShifts& shiftMC = kNoShift ) const override;

    private:
    const HistAxis fAxis;
    const Cut fCut;
    const Weight fWei;
  };

  //---------------------------------------------------------------------------

  /// Generates Near Detector predictions (which are oscillate-able)
  class NDPredictionGenerator : public IPredictionGenerator {
  public:

    NDPredictionGenerator(const HistAxis axis,
			  const Cut cutND,
			  const SystShifts shiftND = kNoShift,
			  const Var wei = kUnweighted);

    std::unique_ptr<IPrediction> Generate(Loaders& loaders,
					  const SystShifts& shiftMC = kNoShift) const override;

  private:

    const HistAxis fAxis;
    const Cut fCutND;
    const SystShifts fShiftND;
    const Var fWei;

  };

  //---------------------------------------------------------------------------

  /// Generates Far Detector BSM predictions
  class FDPredictionGenerator: public IPredictionGenerator {
  public:

    FDPredictionGenerator(const HistAxis axis,
			  const Cut cutFD,
			  const SystShifts shiftFD = kNoShift,
			  const Var wei = kUnweighted);

    std::unique_ptr<IPrediction> Generate(Loaders& loaders,
					  const SystShifts& shiftMC = kNoShift) const override;

  private:

    const HistAxis fAxis;
    const Cut fCutFD;
    const SystShifts fShiftFD;
    const Var fWei;

  };

}
