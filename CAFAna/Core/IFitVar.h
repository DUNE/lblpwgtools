#pragma once

#include <string>

#include "CAFAna/Core/Registry.h"

namespace osc{class IOscCalculatorAdjustable;}

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
      virtual double GetValue(const osc::IOscCalculatorAdjustable* osc) const = 0;
      virtual void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const = 0;
      virtual double Penalty(double, osc::IOscCalculatorAdjustable*) const {return 0;}

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
    virtual double Penalty(double val, osc::IOscCalculatorAdjustable*) const;
    virtual double LowLimit() const = 0;
    virtual double HighLimit() const = 0;
  protected:
    double Clamp(double val) const;
  };
} // namespace
