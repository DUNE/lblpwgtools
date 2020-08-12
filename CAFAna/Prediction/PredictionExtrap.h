#pragma once

#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Extrap/IExtrap.h"

namespace ana
{
  //  class IExtrap;

  /// Take the output of an extrapolation and oscillate it as required
  class PredictionExtrap: public IPrediction
  {
    public:
      /// Takes ownership of \a extrap
      PredictionExtrap(IExtrap* extrap);
      virtual ~PredictionExtrap();

      // un-hide inherited method stubs so we don't get warnings from the compiler
      using IPrediction::Predict;
      using IPrediction::PredictComponent;
      using IPrediction::PredictSyst;

      Spectrum Predict(osc::IOscCalc* calc) const override;
      Spectrum Predict(osc::IOscCalcStan* calc) const override;

      Spectrum PredictComponent(osc::IOscCalc* calc,
                                Flavors::Flavors_t flav,
                                Current::Current_t curr,
                                Sign::Sign_t sign) const override;
      Spectrum PredictComponent(osc::IOscCalcStan* calc,
                                Flavors::Flavors_t flav,
                                Current::Current_t curr,
                                Sign::Sign_t sign) const override;

      OscillatableSpectrum ComponentCC(int from, int to) const override;
      Spectrum ComponentNC() const override;

    virtual void SaveTo(TDirectory* dir, const std::string& name) const override;
    static std::unique_ptr<PredictionExtrap> LoadFrom(TDirectory* dir, const std::string& name);

      PredictionExtrap() = delete;

      IExtrap* GetExtrap() const {return fExtrap;}

    protected:
      /// Templated helper function called by the non-templated versions
      template<typename T>
      Spectrum _PredictComponent(osc::_IOscCalc<T>* calc,
                                 Flavors::Flavors_t flav,
                                 Current::Current_t curr,
                                 Sign::Sign_t sign) const;

      IExtrap* fExtrap;
  };
}
