#pragma once

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/ISyst.h"

namespace ana
{
  /// Uncertainty in the scale of a single component of the spectrum
  class SystComponentScale: public ISyst
  {
  public:
    /// \param cut Select the component to be scaled
    SystComponentScale(const std::string& shortName,
                       const std::string& latexName,
                       const Cut& cut, double oneSigma)
      : fShortName(shortName), fLatexName(latexName),
        fCut(cut), fOneSigma(oneSigma)
    {
    }

    virtual ~SystComponentScale();

    std::set<std::string> Requires() const override {return fCut.Requires();}

    std::string ShortName() const override {return fShortName;}
    std::string LatexName() const override {return fLatexName;}

    /// Scaling this component between 1/(1+x) and (1+x) is the 1-sigma range
    double OneSigmaScale() const {return fOneSigma;}

    const Cut& GetCut() const {return fCut;}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override;

  protected:
    std::string fShortName;
    std::string fLatexName;
    Cut fCut;
    double fOneSigma;
  };
} // namespace
