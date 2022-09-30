#include "CAFAna/Prediction/PredictionGenerator.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"

namespace ana
{

  //--------------------------------------------------------------------------
  /* TODO TODO think about generators
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
  */
}
