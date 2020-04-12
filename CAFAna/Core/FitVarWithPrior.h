#pragma once

#include "CAFAna/Core/IFitVar.h"
#include "Utilities/func/StanVar.h"

namespace osc
{
  template <typename T>
  class _IOscCalculatorAdjustable;
}

namespace ana
{
  /// \brief A convenience FitVar_StanSupport that wraps a pre-existing FitVar_StanSupport with a function you specify (enables re-use of functions)
  ///
  /// Warning: the function you pass as a prior *must* be normalizable (i.e., finite integral)
  /// when multiplied by the likelihood over the domain of your Var.
  /// This class can't check for you (the MCMC is going to explore that space);
  /// if your prior*likelihood isn't normalizable, you are likely to get unexpected results.
  class FitVarWithPrior : virtual public StanFitSupport<IFitVar>
  {
    public:
      /// Function that will return the *log* of the value of the prior density for a given parameter
      typedef std::function<typename stan::math::var(stan::math::var, const osc::_IOscCalculatorAdjustable <stan::math::var>*)> PriorFnType;

      /// Constructor.  Ensure that \a var doesn't go out of scope before this object does!
      FitVarWithPrior(const StanFitSupport<IFitVar> *var,
                      PriorFnType priorFn,
                      const std::string &priorName)
          : StanFitSupport<IFitVar>(dynamic_cast<const IFitVar*>(var)->ShortName() + "_" + priorName,
                                    dynamic_cast<const IFitVar*>(var)->LatexName()),
            fVar(var), fPriorFn(priorFn)
      {}

      /// Forward to wrapped Var's GetValue()
      double GetValue(const osc::IOscCalculatorAdjustable *osc) const override
      {
        return fVar->GetValue(osc);
      }
      /// Forward to wrapped Var's GetValue()
      stan::math::var GetValue(const osc::_IOscCalculatorAdjustable<stan::math::var> *osc) const override
      {
        return fVar->GetValue(osc);
      };

      /// Forward to wrapped Var's SetValue()
      void SetValue(osc::IOscCalculatorAdjustable *osc, double val) const override
      {
        fVar->SetValue(osc, val);
      };
      /// Forward to wrapped Var's SetValue()
      void SetValue(osc::_IOscCalculatorAdjustable<stan::math::var> *osc, stan::math::var val) const override
      {
        fVar->SetValue(osc, val);
      };

      /// Implement calculation of log-prior by forwarding to stored function
      stan::math::var
      LogPrior(const stan::math::var &val, const osc::_IOscCalculatorAdjustable<stan::math::var> *calc) const override
      {
        return fPriorFn(val, calc);
      }

    private:
      const StanFitSupport<IFitVar> *fVar;

      PriorFnType fPriorFn;
  };

/// Version of FitVarWithPrior for use with constrained FitVar_StanSupports
  class ConstrainedFitVarWithPrior : virtual public StanFitSupport<IConstrainedFitVar>
  {
    public:
      typedef std::function<typename stan::math::var(const stan::math::var &,
                                                     const osc::_IOscCalculatorAdjustable<stan::math::var> *)> PriorFnType;

      ConstrainedFitVarWithPrior(const StanFitSupport<IConstrainedFitVar> * var,
                                 FitVarWithPrior::PriorFnType priorFn,
                                 const std::string &priorName)
          : StanFitSupport<IConstrainedFitVar>(dynamic_cast<const IFitVar*>(var)->ShortName() + "_" + priorName,
                                               dynamic_cast<const IFitVar*>(var)->LatexName()),
            fVar(var),
            fPriorFn(priorFn),
            fPriorFnName(priorName)
      {}

      double GetValue(const osc::IOscCalculatorAdjustable *osc) const override
      {
        return fVar->GetValue(osc);
      }
      stan::math::var GetValue(const osc::IOscCalculatorAdjustableStan *osc) const override
      {
        return fVar->GetValue(osc);
      };

      void SetValue(osc::IOscCalculatorAdjustable *osc, double val) const override
      {
        fVar->SetValue(osc, val);
      };
      void SetValue(osc::IOscCalculatorAdjustableStan *osc, stan::math::var val) const override
      {
        fVar->SetValue(osc, val);
      };

      stan::math::var
      LogPrior(const stan::math::var &val, const osc::_IOscCalculatorAdjustable<stan::math::var> *calc) const override
      {
        return fPriorFn(val, calc);
      }

      double LowLimit() const override
      {
        return fVar->LowLimit();
      };

      double HighLimit() const override
      {
        return fVar->HighLimit();
      };

      const std::string &PriorName() const
      {
        return fPriorFnName;
      };

    private:
      const StanFitSupport<IConstrainedFitVar> * fVar;

      PriorFnType fPriorFn;

      std::string fPriorFnName;

  };
} // namespace ana
