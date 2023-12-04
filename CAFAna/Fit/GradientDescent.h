#pragma once

#include "Math/Minimizer.h"

#include <vector>

namespace ana
{
  /// A minimalistic gradient descent fitter to complement MINUIT's more
  /// elaborate offerings
  class GradientDescent: public ROOT::Math::Minimizer
  {
  public:
    GradientDescent(const ROOT::Math::IMultiGradFunction& func);

    virtual ~GradientDescent();

    void SetFunction(const ROOT::Math::IMultiGradFunction& func) override {fFunc = &func;}
    void SetFunction(const ROOT::Math::IMultiGenFunction& func) override {abort();}

    virtual bool SetVariable(unsigned int ivar, const std::string & name, double val, double step) override;

    virtual bool Minimize() override;

    virtual double MinValue() const override {return fChi;}
    virtual const double* X() const override {return fVals.data();}
    virtual const double* Errors() const override {return fErrs.data();}
    virtual unsigned int NDim() const override {return fVals.size();}

  protected:
    // Math utilities
    double Magnitude(const std::vector<double>& xs) const;
    void MakeUnit(std::vector<double>& xs) const;

    const ROOT::Math::IMultiGradFunction* fFunc;

    std::vector<double> fVals;
    std::vector<double> fErrs;
    double fChi;
  };
}
