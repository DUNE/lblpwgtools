#pragma once

#include <string>

#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/StanTypedefs.h"

#include "Utilities/func/StanUtils.h"

namespace ana
{
  //----------------------------------------------------------------------
  /// Interface definition for fittable variables
  class IFitVar
  {
  public:
    virtual double GetValue(const osc::IOscCalculatorAdjustable* osc) const = 0;
    virtual void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const = 0;
    virtual std::string ShortName() const = 0;
    virtual std::string LatexName() const = 0;
    virtual double Penalty(double /*val*/,
                           osc::IOscCalculatorAdjustable*) const {return 0;}
  };

  //----------------------------------------------------------------------

  /// Base class for variables with constraints. Apply penalty outside range
  class IConstrainedFitVar: public IFitVar
  {
  public:
    virtual double Penalty(double val, osc::IOscCalculatorAdjustable*) const override;
    virtual double LowLimit() const = 0;
    virtual double HighLimit() const = 0;

  protected:
      template <typename T>
      T Clamp(T val) const
      {
        return std::max(util::GetValAs<T>(LowLimit()), std::min(val, util::GetValAs<T>(HighLimit())));
      }
  };

  // We want the callers of these to be inlined below so the class can work
  // templated over any type, but we need the implementation in the .cxx so we
  // don't have to include the full Stan.h here. This is ugly :(
  stan::math::var StanLog(const stan::math::var& x);
  stan::math::var StanExp(const stan::math::var& x);

  //----------------------------------------------------------------------
  /// Mixin class to indicate that Stan support should be expected in a given FitVar.
  /// Inherit from this <i>in addition to</i> either IFitVar or IConstrainedFitVar.
  /// For more on this idiom consult, e.g., http://www.drdobbs.com/cpp/mixin-based-programming-in-c/184404445?pgno=1
  template <typename VarClass>
  class StanFitSupport : public VarClass
  {
    public:
      // just pass all the interesting constructor work to the base class.  we have no data.
      using VarClass::VarClass;



      // also make sure we don't lose its implementations of GetValue() and SetValue()
      using VarClass::GetValue;
      using VarClass::SetValue;

      // suggest implementing these by making a templated private method
      // which both the <double> and <stan::math::var> versions forward to
      virtual stan::math::var GetValue(const osc::IOscCalculatorAdjustableStan * osc) const = 0;
      virtual void SetValue(osc::IOscCalculatorAdjustableStan * osc, stan::math::var val) const = 0;

      /// The prior probability density distribution on this variable (which can depend on others if needed).
      /// If there are optimizations that can make log(Prior()) more expensive than a direct implementation,
      /// consider implementing LogPrior() directly instead.
      virtual stan::math::var Prior(const stan::math::var& var, const osc::IOscCalculatorAdjustableStan* calc) const {return StanExp(LogPrior(var, calc));}

      /// Log of the prior probability density.  Default implementation just does log(Prior()),
      /// but this can be overridden if there optimizations that speed up the calculation.
      virtual stan::math::var LogPrior(const stan::math::var& var, const osc::IOscCalculatorAdjustableStan* calc) const {return StanLog(Prior(var, calc));}
  };
} // namespace
