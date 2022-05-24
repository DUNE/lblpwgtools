#include "CAFAna/Extrap/NDExtrap.h"
#include "CAFAna/Extrap/FDExtrap.h"
#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Prediction/PredictionGenerator.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Prediction/NDPredictionSterile.h"
#include "CAFAna/Prediction/FDPredictionSterile.h"

namespace ana
{

  //--------------------------------------------------------------------------

  NoExtrapGenerator::NoExtrapGenerator(
    const HistAxis axis,
    const Cut cut,
    const Weight wei
  ) : fAxis(axis), fCut(cut), fWei(wei) {}

  std::unique_ptr<IPrediction> NoExtrapGenerator::Generate(
							   Loaders& loaders,
							   const SystShifts& shiftMC
							   ) const {
    return std::unique_ptr<IPrediction>( new PredictionNoExtrap(
								loaders, fAxis, fCut, shiftMC, fWei ) );
  }

  //---------------------------------------------------------------------------

  NDPredictionGenerator::NDPredictionGenerator(const HistAxis axis,
					       const Cut cutND,
					       const SystShifts shiftND,
					       const Weight wei)
    : fAxis(axis), fCutND(cutND), fShiftND(shiftND), fWei(wei) {
  }

  std::unique_ptr<IPrediction> NDPredictionGenerator::Generate(Loaders& loaders,
							       const SystShifts& shiftMC) const {
    NDExtrap* extrap = new NDExtrap(NDExtrap::NDExtrap_c(loaders, fAxis, fCutND, shiftMC, fWei));
    return std::make_unique<NDPredictionSterile>(extrap);
  }


  //---------------------------------------------------------------------------

  FDPredictionGenerator::FDPredictionGenerator(const HistAxis axis,
					       const Cut cutFD,
					       const SystShifts shiftFD,
					       const Weight wei)
    :fAxis(axis), fCutFD(cutFD), fShiftFD(shiftFD), fWei(wei) {
  }

  std::unique_ptr<IPrediction> FDPredictionGenerator::Generate(Loaders& loaders,
							       const SystShifts& shiftMC) const {
    FDExtrap* extrap = new FDExtrap(FDExtrap::FDExtrap_c(loaders, fAxis, fCutFD, shiftMC, fWei));
    return std::make_unique<FDPredictionSterile>(extrap);
  }

}
