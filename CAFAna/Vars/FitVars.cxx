#include "CAFAna/Vars/FitVars.h"


#include "OscLib/func/IOscCalculator.h"
#include "Utilities/func/MathUtil.h"
#include "Utilities/func/StanVar.h"

#include <cassert>
#include <cmath>

namespace ana
{
  // declared as 'extern' in FitVars.h
  const FitTheta13 kFitTheta13;
  const FitSinSq2Theta13 kFitSinSq2Theta13;
  const FitDeltaInPiUnits kFitDeltaInPiUnits;
  const FitTheta23 kFitTheta23;
  const FitSinSqTheta23 kFitSinSqTheta23;
  const FitSinSqTheta23LowerOctant kFitSinSqTheta23LowerOctant;
  const FitSinSqTheta23UpperOctant kFitSinSqTheta23UpperOctant;
  const FitSinSqTheta23Symmetry kFitSinSqTheta23BelowSymmetry(-1);
  const FitSinSqTheta23Symmetry kFitSinSqTheta23AboveSymmetry(+1);
  const FitSinSq2Theta23 kFitSinSq2Theta23;
  const FitDmSq32 kFitDmSq32;
  const FitDmSq32Scaled kFitDmSq32Scaled;
  const FitDmSq32NHScaled kFitDmSq32NHScaled;
  const FitDmSq32IHScaled kFitDmSq32IHScaled;
  const FitTanSqTheta12 kFitTanSqTheta12;
  const FitSinSq2Theta12 kFitSinSq2Theta12;
  const FitDmSq21 kFitDmSq21;
  const FitDmSq21Scaled kFitDmSq21Scaled;
  const FitRho kFitRho;

  //----------------------------------------------------------------------
  double FitTheta13::GetValue(const osc::IOscCalculatorAdjustable* osc) const
  {
    return osc->GetTh13();
  }

  //----------------------------------------------------------------------
  void FitTheta13::SetValue(osc::IOscCalculatorAdjustable* osc, double val) const
  {
    osc->SetTh13(val);
  }

  //----------------------------------------------------------------------
  double FitSinSq2Theta13::GetValue(const osc::IOscCalculatorAdjustable* osc) const
  {
    return util::sqr(sin(2*osc->GetTh13()));
  }

  //----------------------------------------------------------------------
  void FitSinSq2Theta13::SetValue(osc::IOscCalculatorAdjustable* osc, double val) const
  {
    osc->SetTh13(asin(sqrt(Clamp(val)))/2);
  }

  //----------------------------------------------------------------------
  // helper function so the code doesn't need to be written twice
  namespace
  {
    template<typename T>
    T dcP_GetVal_Helper(const osc::_IOscCalculatorAdjustable<T>* osc)
    {
      T ret = osc->GetdCP()/M_PI;

      // convert to the range 0-2
      auto a = floor(ret/2+1);
      ret -= 2*a;
      // Instead of figuring out all the rounding just nudge the last little bit
      while(ret < 0) ret += 2;
      while(ret > 2) ret -= 2;

      return ret;
    }
  }
  double FitDeltaInPiUnits::GetValue(const osc::IOscCalculatorAdjustable* osc) const
  {
    return dcP_GetVal_Helper(osc);
  }
  stan::math::var FitDeltaInPiUnits::GetValue(const osc::IOscCalculatorAdjustableStan* osc) const
  {
    return dcP_GetVal_Helper(osc);
  }

  //----------------------------------------------------------------------
  void FitDeltaInPiUnits::SetValue(osc::IOscCalculatorAdjustable* osc, double val) const
  {
    osc->SetdCP(M_PI*val);
  }
  void FitDeltaInPiUnits::SetValue(osc::IOscCalculatorAdjustableStan* osc, stan::math::var val) const
  {
    osc->SetdCP(M_PI*val);
  }

  //----------------------------------------------------------------------
  double FitTheta23::GetValue(const osc::IOscCalculatorAdjustable* osc) const
  {
    return osc->GetTh23();
  }

  //----------------------------------------------------------------------
  void FitTheta23::SetValue(osc::IOscCalculatorAdjustable* osc, double val) const
  {
    osc->SetTh23(val);
  }

  //----------------------------------------------------------------------

  //----------------------------------------------------------------------
  double FitSinSqTheta23::GetValue(const osc::IOscCalculatorAdjustable* osc) const
  {
    return util::sqr(sin(osc->GetTh23()));
  }

  //----------------------------------------------------------------------
  stan::math::var FitSinSqTheta23::GetValue(const osc::IOscCalculatorAdjustableStan *osc) const
  {
    return util::sqr(sin(osc->GetTh23()));
  }

  //----------------------------------------------------------------------
  void FitSinSqTheta23::SetValue(osc::IOscCalculatorAdjustable* osc, double val) const
  {
    osc->SetTh23(asin(sqrt(Clamp(val))));
  }


  //----------------------------------------------------------------------
  void FitSinSqTheta23::SetValue(osc::IOscCalculatorAdjustableStan *osc, stan::math::var val) const
  {
    osc->SetTh23(asin(sqrt(this->Clamp(val))));
  }

  //----------------------------------------------------------------------
  double FitSinSqTheta23Symmetry::
  GetValue(const osc::IOscCalculatorAdjustable* osc) const
  {
    return util::sqr(sin(osc->GetTh23()));
  }

  //----------------------------------------------------------------------
  void FitSinSqTheta23Symmetry::
  SetValue(osc::IOscCalculatorAdjustable* osc, double val) const
  {
    const double v0 = SymmPt(osc->GetDmsq32());
    if(fSign*val < fSign*v0) val = v0;
    osc->SetTh23(asin(sqrt(val)));
  }

  //----------------------------------------------------------------------
  double FitSinSqTheta23Symmetry::
  Penalty(double val, osc::IOscCalculatorAdjustable* calc) const
  {
    const double v0 = SymmPt(calc->GetDmsq32());
    if(fSign*val < fSign*v0) return util::sqr(val-v0);
    return 0;
  }

  //----------------------------------------------------------------------
  double FitSinSqTheta23Symmetry::SymmPt(double dmsq) const
  {
    // Determined empirically based on the NuFit 4.0 best fits
    const double kTh23SymmPtNH = 0.516413;
    const double kTh23SymmPtIH = 0.506624;

    if(dmsq > 0) return kTh23SymmPtNH; else return kTh23SymmPtIH;
  }

  //----------------------------------------------------------------------
  double FitSinSq2Theta23::GetValue(const osc::IOscCalculatorAdjustable* osc) const
  {
    return util::sqr(sin(2*osc->GetTh23()));
  }

  //----------------------------------------------------------------------
  void FitSinSq2Theta23::SetValue(osc::IOscCalculatorAdjustable* osc, double val) const
  {
    osc->SetTh23(asin(sqrt(Clamp(val)))/2);
  }

  //----------------------------------------------------------------------
  double FitDmSq32::GetValue(const osc::IOscCalculatorAdjustable* osc) const
  {
    return osc->GetDmsq32();
  }

  //----------------------------------------------------------------------
  void FitDmSq32::SetValue(osc::IOscCalculatorAdjustable* osc, double val) const
  {
    osc->SetDmsq32(Clamp(val));
  }

  //----------------------------------------------------------------------
  double FitDmSq32Scaled::GetValue(const osc::IOscCalculatorAdjustable* osc) const
  {
    return osc->GetDmsq32()*1000.0;
  }

  //----------------------------------------------------------------------
  stan::math::var FitDmSq32Scaled::GetValue(const osc::IOscCalculatorAdjustableStan *osc) const
  {
    return osc->GetDmsq32()*1000.0;;
  }

  //----------------------------------------------------------------------
  void FitDmSq32Scaled::SetValue(osc::IOscCalculatorAdjustableStan *osc, stan::math::var val) const
  {
    osc->SetDmsq32(this->Clamp(val)/1000.0);
  }


  //----------------------------------------------------------------------
  void FitDmSq32Scaled::SetValue(osc::IOscCalculatorAdjustable* osc, double val) const
  {
    osc->SetDmsq32(Clamp(val/1000.0));
  }

  //----------------------------------------------------------------------
  double FitTanSqTheta12::GetValue(const osc::IOscCalculatorAdjustable* osc) const
  {
    return util::sqr(tan(osc->GetTh12()));
  }

  //----------------------------------------------------------------------
  void FitTanSqTheta12::SetValue(osc::IOscCalculatorAdjustable* osc, double val) const
  {
    osc->SetTh12(atan(sqrt(Clamp(val))));
  }

  //----------------------------------------------------------------------
  double FitSinSq2Theta12::GetValue(const osc::IOscCalculatorAdjustable* osc) const
  {
    return util::sqr(sin(2*osc->GetTh12()));
  }

  //----------------------------------------------------------------------
  void FitSinSq2Theta12::SetValue(osc::IOscCalculatorAdjustable* osc, double val) const
  {
    osc->SetTh12(asin(sqrt(Clamp(val)))/2);
  }

  //----------------------------------------------------------------------
  double FitDmSq21::GetValue(const osc::IOscCalculatorAdjustable* osc) const
  {
    return osc->GetDmsq21();
  }

  //----------------------------------------------------------------------
  void FitDmSq21::SetValue(osc::IOscCalculatorAdjustable* osc, double val) const
  {
    osc->SetDmsq21(Clamp(val));
  }
  //----------------------------------------------------------------------
  double FitDmSq21Scaled::GetValue(const osc::IOscCalculatorAdjustable* osc) const
  {
    return osc->GetDmsq21()*1e5;
  }

  //----------------------------------------------------------------------
  void FitDmSq21Scaled::SetValue(osc::IOscCalculatorAdjustable* osc, double val) const
  {
    osc->SetDmsq21(Clamp(val/1e5));
  }

  //----------------------------------------------------------------------
  double FitRho::GetValue(const osc::IOscCalculatorAdjustable* osc) const
  {
    return osc->GetRho();
  }

  //----------------------------------------------------------------------
  void FitRho::SetValue(osc::IOscCalculatorAdjustable* osc, double val) const
  {
    osc->SetRho(Clamp(val));
  }

} // namespace
