#pragma once

#include "CAFAna/Core/IFitVar.h"

#include "CAFAna/Vars/FitVars.h"

namespace ana
{
  /// \brief Fit the product of two variables (made from angles so their ranges
  /// are 0-1). See also \ref FitVarsProductMarg.
  ///
  /// When we set the product, how do we decide to split it between the two
  /// variables? How do we marginalize over that freedom of choice? We
  /// introduce two new variables defined as:
  ///
  /// \f$ \alpha=xy \f$ and \f$
  /// \beta={2\over\pi}\arctan\left({1-x\over1-y}\right) \f$
  ///
  /// This class is \f$ \alpha \f$, FitVarsProductMarg is \f$ \beta \f$. They
  /// have the property of being orthogonal, and ranging over 0 to 1 as \f$ x
  /// \f$ and \f$ y \f$ do.
  ///
  /// One should fit over this parameter and introduce the corresponding
  /// FitVarsProductMarg as a marginalization variable.
  class FitVarsProduct: public IConstrainedFitVar
  {
  public:
    FitVarsProduct(const IConstrainedFitVar& x,
                   const IConstrainedFitVar& y);

    virtual double GetValue(const osc::IOscCalcAdjustable* osc) const;
    virtual void SetValue(osc::IOscCalcAdjustable* osc, double val) const;

    virtual double LowLimit() const {return 0;}
    virtual double HighLimit() const {return 1;}
  protected:
    const IConstrainedFitVar& fVarX;
    const IConstrainedFitVar& fVarY;
  };

  /// See the documentation for \ref FitVarsProductMarg
  class FitVarsProductMarg: public IConstrainedFitVar
  {
  public:
    FitVarsProductMarg(const IConstrainedFitVar& x,
                       const IConstrainedFitVar& y);

    virtual double GetValue(const osc::IOscCalcAdjustable* osc) const;
    virtual void SetValue(osc::IOscCalcAdjustable* osc, double val) const;

    virtual double LowLimit() const {return 0;}
    virtual double HighLimit() const {return 1;}
  protected:
    const IConstrainedFitVar& fVarX;
    const IConstrainedFitVar& fVarY;
  };

  /// Return a FitVarsProduct for \f$ \sin^2\theta_{23}\sin^22\theta_{13} \f$
  // (Don't make a const FitVarsProduct object because it depends on two other
  //  FitVars, and you'll probably run into the 'static initialization order' problem;
  //  static vars like those inside these functions are only initialized when the fn is
  //  first called, which is guaranteed to always be after the globals)
  FitVarsProduct& FitVarSinSqTheta23SinSq2Theta13();

  /// Return a FitVarsProductMarg to marginalize over this if using kFitSinSqTheta23SinSq2Theta13
  FitVarsProductMarg& FitVarSinSqTheta23SinSq2Theta13Marg();

  /// \brief \f$ 2\sin^2\theta_{23}\sin^22\theta_{13} \f$
  ///
  /// Continue to use kFitSinSqTheta23SinSq2Theta13Marg as the marg var
  class Fit2SinSqTheta23SinSq2Theta13: public IConstrainedFitVar
  {
  public:
    Fit2SinSqTheta23SinSq2Theta13()
      : IConstrainedFitVar("2ssth23ss2th13", "2sin^{2}#theta_{23}sin^{2}2#theta_{13}"),
        fVar(kFitSinSqTheta23, kFitSinSq2Theta13)
    {
    }

    double GetValue(const osc::IOscCalcAdjustable* osc) const override
    {
      return 2*fVar.GetValue(osc);
    }
    void SetValue(osc::IOscCalcAdjustable* osc, double val) const override
    {
      fVar.SetValue(osc, val/2);
    }

    double LowLimit() const override {return 0;}
    double HighLimit() const override {return 2;}
  protected:
    const FitVarsProduct fVar;
  };

  /// Return a Fit2SinSqTheta23SinSq2Theta13 for \f$ 2\sin^2\theta_{23}\sin^22\theta_{13} \f$
  Fit2SinSqTheta23SinSq2Theta13& FitVar2SinSqTheta23SinSq2Theta13();
} // namespace