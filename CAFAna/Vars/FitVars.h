#pragma once

#include "CAFAna/Core/IFitVar.h"

#include <limits>

#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/StanTypedefs.h"
namespace ana
{
  /// \f$ \theta_{13} \f$
  class FitTheta13: virtual public IFitVar
  {
  public:
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    std::string ShortName() const override {return "th13";}
    std::string LatexName() const override {return "#theta_{13}";}
  };

  /// \f$ \theta_{13} \f$
  const FitTheta13 kFitTheta13 = FitTheta13();

  //----------------------------------------------------------------------

  /// \f$ \sin^22\theta_{13} \f$
  class FitSinSq2Theta13: virtual public IConstrainedFitVar
  {
  public:
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    std::string ShortName() const override {return "ss2th13";}
    std::string LatexName() const override {return "sin^{2}2#theta_{13}";}

    double LowLimit() const override {return 0;}
    double HighLimit() const override {return 1;}
  };

  /// \f$ \sin^22\theta_{13} \f$
  const FitSinSq2Theta13 kFitSinSq2Theta13 = FitSinSq2Theta13();

  //----------------------------------------------------------------------

  /// \f$ \delta_{CP}/\pi \f$
  class FitDeltaInPiUnits: virtual public IFitVar
  {
  public:
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    std::string ShortName() const override {return "delta(pi)";}
    std::string LatexName() const override {return "#delta / #pi";}
  };

  /// \f$ \delta_{CP}/\pi \f$
  const FitDeltaInPiUnits kFitDeltaInPiUnits = FitDeltaInPiUnits();

  //----------------------------------------------------------------------
  /// \f$ \theta_{13} \f$
  class FitTheta23: public IFitVar
  {
  public:
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    std::string ShortName() const override {return "th23";}
    std::string LatexName() const override {return "#theta_{23}";}
  };

  /// \f$ \theta_{13} \f$
  const FitTheta23 kFitTheta23 = FitTheta23();
  //----------------------------------------------------------------------

  /// \f$ \sin^2\theta_{23} \f$
  class FitSinSqTheta23: virtual public StanFitSupport<IConstrainedFitVar>
  {
  public:
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    stan::math::var GetValue(const osc::IOscCalculatorAdjustableStan* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    void SetValue(osc::IOscCalculatorAdjustableStan* osc, stan::math::var val) const override;
    std::string ShortName() const override {return "ssth23";}
    std::string LatexName() const override {return "sin^{2}#theta_{23}";}

    double LowLimit() const override {return 0;}
    double HighLimit() const override {return 1;}
  };

  /// \f$ \sin^2\theta_{23} \f$
  const FitSinSqTheta23 kFitSinSqTheta23 = FitSinSqTheta23();

  //----------------------------------------------------------------------
  /// \f$ \sin^2\theta_{23} \f$ constrained to lower octant
  class FitSinSqTheta23LowerOctant: public FitSinSqTheta23
  {
  public:
    std::string ShortName() const override {return "ssth23_lower";}

    double LowLimit() const override {return 0;}
    double HighLimit() const override {return 0.5;}
  };

  /// \f$ \sin^2\theta_{23} \f$ constrained to lower octant
  const FitSinSqTheta23LowerOctant kFitSinSqTheta23LowerOctant = FitSinSqTheta23LowerOctant();

  //----------------------------------------------------------------------
  /// \f$ \sin^2\theta_{23} \f$ constrained to upper octant
  class FitSinSqTheta23UpperOctant: public FitSinSqTheta23
  {
  public:
    std::string ShortName() const override {return "ssth23_upper";}

    double LowLimit() const override {return 0.5;}
    double HighLimit() const override {return 1;}
  };

  /// \f$ \sin^2\theta_{23} \f$ constrained to lower octant
  const FitSinSqTheta23UpperOctant kFitSinSqTheta23UpperOctant = FitSinSqTheta23UpperOctant();

  //----------------------------------------------------------------------
  class FitSinSqTheta23Symmetry: virtual public IFitVar
  {
  public:
    FitSinSqTheta23Symmetry(int sign) : fSign(sign) {}

    std::string ShortName() const override override {return "ssth23";}
    std::string LatexName() const override override {return "sin^{2}#theta_{23}";}

    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override override;

    void SetValue(osc::IOscCalculatorAdjustable* osc,
                  double val) const override;

    double Penalty(double val,
                   osc::IOscCalculatorAdjustable* calc) const override;
  protected:
    double SymmPt(double dmsq) const;

    int fSign;
  };

  // These are for use in ensuring we explore all regions of th23 space
  const FitSinSqTheta23Symmetry kFitSinSqTheta23BelowSymmetry = FitSinSqTheta23Symmetry(-1);
  const FitSinSqTheta23Symmetry kFitSinSqTheta23AboveSymmetry = FitSinSqTheta23Symmetry(+1);

  //----------------------------------------------------------------------

  /// \f$ \sin^22\theta_{23} \f$
  class FitSinSq2Theta23: virtual public IConstrainedFitVar
  {
  public:
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    std::string ShortName() const override {return "ss2th23";}
    std::string LatexName() const override {return "sin^{2}2#theta_{23}";}


    double LowLimit() const override {return 0;}
    double HighLimit() const override {return 1;}
  };

  /// \f$ \sin^22\theta_{23} \f$
  const FitSinSq2Theta23 kFitSinSq2Theta23 = FitSinSq2Theta23();

  //----------------------------------------------------------------------

  /// \f$ \Delta m^2_{32} \f$
  class FitDmSq32: virtual public IConstrainedFitVar
  {
  public:
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    std::string ShortName() const override {return "dmsq32";}
    std::string LatexName() const override {return "#Deltam^{2}_{32}";}

    // "1eV^2 splitting should be enough for anyone"
    // OscCalculatorPMNS freaks out at large splittings
    double LowLimit() const override {return -1;}
    double HighLimit() const override {return +1;}
  };

  /// \f$ \Delta m^2_{32} \f$
  const FitDmSq32 kFitDmSq32 = FitDmSq32();

  //-------------------------------------------------------------------------

  /// \f$ \Delta m^2_{32}\times10^3{\rm eV}^2 \f$
  class FitDmSq32Scaled: virtual public StanFitSupport<IConstrainedFitVar>
  {
  public:
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    std::string ShortName() const override {return "dmsq32scaled";}
    std::string LatexName() const override {return "#Deltam^{2}_{32} (10^{-3} eV^{2})";}

    // "1eV^2 splitting should be enough for anyone"
    // OscCalculatorPMNS freaks out at large splittings
    double LowLimit() const override {return -1000;}
    double HighLimit() const override {return +1000;}
  };

  /// \f$ \Delta m^2_{32}\times10^3{\rm eV}^2 \f$
  const FitDmSq32Scaled kFitDmSq32Scaled = FitDmSq32Scaled();

  //----------------------------------------------------------------------

  /// \f$ \Delta m^2_{32} constrained to normal hierarchy \f$
  class FitDmSq32NHScaled: public FitDmSq32Scaled
  {
  public:
    std::string ShortName() const override {return "dmsq32NHscaled";}

    // "1eV^2 splitting should be enough for anyone"
    double LowLimit() const override {return 0;}
  };

  /// \f$ \Delta m^2_{32} constrained to normal hierarchy \f$
  const FitDmSq32NHScaled kFitDmSq32NHScaled = FitDmSq32NHScaled();

  //----------------------------------------------------------------------

  /// \f$ \Delta m^2_{32} constrained to inverted hierarchy \f$
  class FitDmSq32IHScaled: public FitDmSq32Scaled
  {
  public:
    std::string ShortName() const override {return "dmsq32IHscaled";}

    double HighLimit() const override {return 0;}
  };

  /// \f$ \Delta m^2_{32} constrained to inverted hierarchy \f$
  const FitDmSq32IHScaled kFitDmSq32IHScaled = FitDmSq32IHScaled();

  //----------------------------------------------------------------------

  /// \f$ \tan^2\theta_{12} \f$
  class FitTanSqTheta12: public IConstrainedFitVar
  {
  public:
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    std::string ShortName() const override {return "tsth12";}
    std::string LatexName() const override {return "tan^{2}#theta_{12}";}

    double LowLimit() const override {return 0;}
    double HighLimit() const override {return std::numeric_limits<double>::max();}
  };

  /// \f$ \tan^2\theta_{12} \f$
  const FitTanSqTheta12 kFitTanSqTheta12 = FitTanSqTheta12();

  //----------------------------------------------------------------------

  /// \f$ \sin^22\theta_{12} \f$
  class FitSinSq2Theta12: public IConstrainedFitVar
  {
  public:
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    std::string ShortName() const override {return "ss2th12";}
    std::string LatexName() const override {return "sin^{2}2#theta_{12}";}

    double LowLimit() const override {return 0;}
    double HighLimit() const override {return 1;}
  };

  /// \f$ \sin^22\theta_{12} \f$
  const FitSinSq2Theta12 kFitSinSq2Theta12 = FitSinSq2Theta12();

  //----------------------------------------------------------------------

  /// \f$ \Delta m^2_{21} \f$
  class FitDmSq21: public IConstrainedFitVar
  {
  public:
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    std::string ShortName() const override {return "dmsq21";}
    std::string LatexName() const override {return "#Deltam^{2}_{21}";}

    // "1eV^2 splitting should be enough for anyone"
    // OscCalculatorPMNS freaks out at large splittings
    double LowLimit() const override {return -1;}
    double HighLimit() const override {return +1;}
  };

  /// \f$ \Delta m^2_{21} \f$
  const FitDmSq21 kFitDmSq21 = FitDmSq21();

  //----------------------------------------------------------------------
  /// \f$ \Delta m^2_{21} \f$
  class FitDmSq21Scaled: public IConstrainedFitVar
  {
  public:
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    std::string ShortName() const override {return "dmsq21scaled";}
    std::string LatexName() const override {return "#Deltam^{2}_{21}";}

    // "1eV^2 splitting should be enough for anyone"
    // OscCalculatorPMNS freaks out at large splittings
    double LowLimit() const override {return -1e5;}
    double HighLimit() const override {return +1e5;}
  };

  /// \f$ \Delta m^2_{21} \f$
  const FitDmSq21Scaled kFitDmSq21Scaled = FitDmSq21Scaled();
  //----------------------------------------------------------------------

  /// \f$ \rho \f$
  class FitRho: public IConstrainedFitVar
  {
  public:
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    std::string ShortName() const override {return "rho";}
    std::string LatexName() const override {return "#rho";}

    //Density should be greater than zero (set a ridiculously high high limit)
    double LowLimit() const override {return 0;}
    double HighLimit() const override {return 10.0;}

  };

  /// \f$ \rho \f$
  const FitRho kFitRho = FitRho();

  //----------------------------------------------------------------------



} // namespace
