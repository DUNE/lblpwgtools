#include "TH1D.h"

#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Core/StanUtils.h"
#include "CAFAna/Experiment/BinnedLkhdExperiment.h"
#include "CAFAna/Prediction/IPrediction.h"

#include "OscLib/func/IOscCalculator.h"

#include "Utilities/func/Stan.h"

namespace ana
{
  BinnedLkhdExperiment::BinnedLkhdExperiment(const IPrediction *MC, const Spectrum& data)
    : fMC(MC), fData(data)
  {}

  stan::math::var BinnedLkhdExperiment::LogLikelihood(osc::_IOscCalculatorAdjustable<stan::math::var> *osc,
                                                      const SystShifts &syst) const
  {
    auto  pred = syst.IsNominal()
                 ? fMC->Predict(osc).ToBins(fData.POT())
                 : fMC->PredictSyst(osc, syst).ToBins(fData.POT());
    TH1D* hdata = fData.ToTH1(fData.POT());

    using ana::LogLikelihood;    // note: this LogLikelihood() is in StanUtils.h
    auto ll = LogLikelihood(pred, hdata) / -2.;  // LogLikelihood(), confusingly, returns chi2=-2*LL

    HistCache::Delete(hdata);

    return ll;
  }

}
