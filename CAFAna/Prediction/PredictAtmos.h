#pragma once

//#include "CAFAna/Core/ReweightableSpectrum.h"
#define protected public
#include "CAFAna/Core/OscillatableSpectrum.h"

#include "CAFAna/Prediction/IPrediction.h"

#include "CAFAna/Core/ISyst.h"

namespace ana
{
  class AtmosSyst: public ISyst
  {
  public:
    AtmosSyst() : ISyst("atmos", "Atmos Syst", true, -1000, +1000){}
    void Shift(double sigma, Restorer&, caf::SRProxy* sr, double& weight) const {}
  };

  extern const AtmosSyst kAtmosSyst;

  class PredictAtmos : public IPrediction
  {
  public:
    PredictAtmos();

    Spectrum Predict(osc::IOscCalc* calc) const {return PredictSyst(calc, kNoShift);}
    Spectrum PredictSyst(osc::IOscCalc* calc, const SystShifts& shifts) const;

    Spectrum PredictComponent(osc::IOscCalc* calc, Flavors::Flavors_t flav, Current::Current_t curr, Sign::Sign_t sign) const {abort();}

    Spectrum Predict(osc::IOscCalcStan* calc) const {abort();}
    Spectrum PredictSyst(osc::IOscCalcStan* calc, const SystShifts& shifts) const {abort();}
    Spectrum PredictComponent(osc::IOscCalcStan* calc, Flavors::Flavors_t flav, Current::Current_t curr, Sign::Sign_t sign) const {abort();}

  protected:
//    ReweightableSpectrum fTauFromMu;
    OscillatableSpectrum fTauFromMu, fAntiTauFromMu;
    Spectrum fNC;
  };
}
