#pragma once

#include <string>

#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/Registry.h"
#include "CAFAna/Core/StanTypedefs.h"
#include "CAFAna/Core/StanVar.h"

#include "CAFAna/Core/StanUtils.h"

#include "OscLib/OscCalc.h"

namespace ana
{
  //----------------------------------------------------------------------
  /// Interface definition for fittable variables
  class IFitVar
  {
    public:
      IFitVar(std::string shortName, std::string latexName)
        : fShortName(std::move(shortName)),
          fLatexName(std::move(latexName))
      {
        Registry<IFitVar>::Register(this);
      }

      virtual ~IFitVar()
      {
        Registry<IFitVar>::UnRegister(this);
      }
      virtual double GetValue(const osc::IOscCalcAdjustable* osc) const = 0;
      virtual void SetValue(osc::IOscCalcAdjustable* osc, double val) const = 0;
      virtual double Penalty(double, osc::IOscCalcAdjustable*) const {return 0;}

      const std::string & ShortName() const { return fShortName; };
      const std::string & LatexName() const { return fLatexName; };

    private:
      std::string fShortName;
      std::string fLatexName;
  };

  //----------------------------------------------------------------------

  /// Base class for variables with constraints. Apply penalty outside range
  class IConstrainedFitVar: public IFitVar
  {
  public:
    using IFitVar::IFitVar;

    virtual double Penalty(double val, osc::IOscCalcAdjustable*) const;
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
      virtual stan::math::var GetValue(const osc::IOscCalcAdjustableStan * osc) const = 0;
      virtual void SetValue(osc::IOscCalcAdjustableStan * osc, stan::math::var val) const = 0;

      /// The prior probability density distribution on this variable (which can depend on others if needed).
      /// If there are optimizations that can make log(Prior()) more expensive than a direct implementation,
      /// consider implementing LogPrior() directly instead.
      virtual stan::math::var Prior(const stan::math::var& var, const osc::IOscCalcAdjustableStan* calc) const {return StanExp(LogPrior(var, calc));}

      /// Log of the prior probability density.  Default implementation just does log(Prior()),
      /// but this can be overridden if there optimizations that speed up the calculation.
      virtual stan::math::var LogPrior(const stan::math::var& var, const osc::IOscCalcAdjustableStan* calc) const {return StanLog(Prior(var, calc));}
  };
} // namespace
