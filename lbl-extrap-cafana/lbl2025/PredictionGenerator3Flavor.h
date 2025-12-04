#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Prediction/PredictionExtrap.h"
#include "CAFAna/Prediction/PredictionGenerator.h"

#include "lbl2025/NumuDecomp.h"
#include "lbl2025/NumuExtrap.h"


namespace lbl2025
{
  // TODO: Add ND data shifts and weights.
  class NumuExtrapGenerator : public ana::IPredictionGenerator {
    public:
      NumuExtrapGenerator(const ana::HistAxis axis,
                          const ana::Cut cutFD,
                          const ana::Cut cutND);

      std::unique_ptr<ana::IPrediction> Generate(ana::Loaders& loaders, const ana::RecoType& ixnRecoType, const ana::SystShifts& shiftMC = ana::kNoShift) const override;

    private:
      const ana::HistAxis fAxis;
      const ana::Cut fCutFD;
      const ana::Cut fCutND;
  };
}
