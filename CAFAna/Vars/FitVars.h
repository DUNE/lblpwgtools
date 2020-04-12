#pragma once

#include <limits>

#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/StanTypedefs.h"
#include "Utilities/func/StanVar.h"

namespace ana
{
  /// \f$ \theta_{13} \f$
  class FitTheta13: virtual public IFitVar
  {
  public:
    FitTheta13() : IFitVar("th13", "#theta_{13}") {};
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
  };

  /// \f$ \theta_{13} \f$
  extern const FitTheta13 kFitTheta13;

  //----------------------------------------------------------------------

  /// \f$ \sin^22\theta_{13} \f$
  class FitSinSq2Theta13: virtual public IConstrainedFitVar
  {
  public:
    FitSinSq2Theta13() : IConstrainedFitVar("ss2th13", "sin^{2}2#theta_{13}") {};
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;

  double LowLimit() const override { return 0; }
  double HighLimit() const override { return 1; }
};

  /// \f$ \sin^22\theta_{13} \f$
  extern const FitSinSq2Theta13 kFitSinSq2Theta13;

  //----------------------------------------------------------------------

  /// \f$ \delta_{CP}/\pi \f$
  class FitDeltaInPiUnits: virtual public StanFitSupport<IFitVar>
  {
    public:
      FitDeltaInPiUnits() : StanFitSupport<IFitVar>("delta(pi)", "#delta / #pi") {};

      stan::math::var GetValue(const osc::IOscCalculatorAdjustableStan* osc) const override;
      double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
      void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
      void SetValue(osc::IOscCalculatorAdjustableStan* osc, stan::math::var val) const override;
 };

  /// \f$ \delta_{CP}/\pi \f$
  extern const FitDeltaInPiUnits kFitDeltaInPiUnits;

  //----------------------------------------------------------------------
  /// \f$ \theta_{13} \f$
  class FitTheta23: public IFitVar
  {
  public:
    FitTheta23() : IFitVar("th23", "#theta_{23}") {}

    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
  };

  /// \f$ \theta_{13} \f$
  extern const FitTheta23 kFitTheta23;
  //----------------------------------------------------------------------

  /// \f$ \sin^2\theta_{23} \f$
  class FitSinSqTheta23: virtual public StanFitSupport<IConstrainedFitVar>
  {
  public:
    FitSinSqTheta23()
      : StanFitSupport<IConstrainedFitVar>("ssth23", "sin^{2}#theta_{23}")
    {}

    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    stan::math::var GetValue(const osc::IOscCalculatorAdjustableStan* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    void SetValue(osc::IOscCalculatorAdjustableStan* osc, stan::math::var val) const override;

    double LowLimit() const override { return 0; }
    double HighLimit() const override { return 1; }
};

  /// \f$ \sin^2\theta_{23} \f$
  extern const FitSinSqTheta23 kFitSinSqTheta23;

  //----------------------------------------------------------------------
  /// \f$ \sin^2\theta_{23} \f$ constrained to lower octant
  class FitSinSqTheta23LowerOctant: public FitSinSqTheta23
  {
    public:
      FitSinSqTheta23LowerOctant()
          : StanFitSupport<IConstrainedFitVar>("ssth23_lower", "sin^{2}#theta_{23}")
      {}

      double LowLimit() const override { return 0; }
      double HighLimit() const override { return 0.5; }
  };

  /// \f$ \sin^2\theta_{23} \f$ constrained to lower octant
  extern const FitSinSqTheta23LowerOctant kFitSinSqTheta23LowerOctant;

  //----------------------------------------------------------------------
  /// \f$ \sin^2\theta_{23} \f$ constrained to upper octant
  class FitSinSqTheta23UpperOctant: public FitSinSqTheta23
  {
    public:
      FitSinSqTheta23UpperOctant()
            : StanFitSupport<IConstrainedFitVar>("ssth23_upper", "sin^{2}#theta_{23}")
        {}

      double LowLimit() const override {return 0.5;}
      double HighLimit() const override {return 1;}
  };

  /// \f$ \sin^2\theta_{23} \f$ constrained to lower octant
  extern const FitSinSqTheta23UpperOctant kFitSinSqTheta23UpperOctant;

  //----------------------------------------------------------------------
  class FitSinSqTheta23Symmetry: virtual public IFitVar
  {
  public:
    FitSinSqTheta23Symmetry(int sign)
      : IFitVar(std::string("ssth23_symm_") + (sign > 0 ? "pos" : "neg"),
                "sin^{2}#theta_{23}"),
        fSign(sign)
    {}

    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;

    void SetValue(osc::IOscCalculatorAdjustable* osc,
                  double val) const override;

    double Penalty(double val,
                   osc::IOscCalculatorAdjustable* calc) const override;
  protected:
    double SymmPt(double dmsq) const;

    int fSign;
  };

  // These are for use in ensuring we explore all regions of th23 space
  extern const FitSinSqTheta23Symmetry kFitSinSqTheta23BelowSymmetry;
  extern const FitSinSqTheta23Symmetry kFitSinSqTheta23AboveSymmetry;

  //----------------------------------------------------------------------

  /// \f$ \sin^22\theta_{23} \f$
  class FitSinSq2Theta23: virtual public IConstrainedFitVar
  {
  public:
      FitSinSq2Theta23() : IConstrainedFitVar("ss2th23", "sin^{2}2#theta_{23}") {};
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;


    double LowLimit() const override { return 0; }
    double HighLimit() const override { return 1; }
  };

  /// \f$ \sin^22\theta_{23} \f$
  extern const FitSinSq2Theta23 kFitSinSq2Theta23;

  //----------------------------------------------------------------------

  /// \f$ \Delta m^2_{32} \f$
  class FitDmSq32: virtual public IConstrainedFitVar
  {
  public:
    FitDmSq32() : IConstrainedFitVar("dmsq32", "#Deltam^{2}_{32}") {};
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;

    // "1eV^2 splitting should be enough for anyone"
    // OscCalculatorPMNS freaks out at large splittings
    double LowLimit() const override { return -1; }
    double HighLimit() const override { return +1; }
  };

  /// \f$ \Delta m^2_{32} \f$
  extern const FitDmSq32 kFitDmSq32;

  //-------------------------------------------------------------------------

  /// \f$ \Delta m^2_{32}\times10^3{\rm eV}^2 \f$
  class FitDmSq32Scaled: virtual public StanFitSupport<IConstrainedFitVar>
  {
  public:
      FitDmSq32Scaled()
        : StanFitSupport<IConstrainedFitVar>("dmsq32scaled",
                                             "#Deltam^{2}_{32} (10^{-3} eV^{2})")
      {}
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    stan::math::var GetValue(const osc::IOscCalculatorAdjustableStan* osc) const override;

    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    void SetValue(osc::IOscCalculatorAdjustableStan* osc, stan::math::var val) const override;


    // "1eV^2 splitting should be enough for anyone"
    // OscCalculatorPMNS freaks out at large splittings
    double LowLimit() const override { return -1000; }
    double HighLimit() const override { return +1000; }
  };

  /// \f$ \Delta m^2_{32}\times10^3{\rm eV}^2 \f$
  extern const FitDmSq32Scaled kFitDmSq32Scaled;

  //----------------------------------------------------------------------

  /// \f$ \Delta m^2_{32} constrained to normal hierarchy \f$
  class FitDmSq32NHScaled: public FitDmSq32Scaled
  {
    public:
      FitDmSq32NHScaled()
          : StanFitSupport<IConstrainedFitVar>("dmsq32NHscaled",
                                               "#Deltam^{2}_{32} (10^{-3} eV^{2})")
      {}

      // "1eV^2 splitting should be enough for anyone"
      double LowLimit() const override { return 0; }
  };

  /// \f$ \Delta m^2_{32} constrained to normal hierarchy \f$
  extern const FitDmSq32NHScaled kFitDmSq32NHScaled;

  //----------------------------------------------------------------------

  /// \f$ \Delta m^2_{32} constrained to inverted hierarchy \f$
  class FitDmSq32IHScaled: public FitDmSq32Scaled
  {
    public:
      FitDmSq32IHScaled()
          : StanFitSupport<IConstrainedFitVar>("dmsq32IHscaled",
                                               "#Deltam^{2}_{32} (10^{-3} eV^{2})")
      {}

      double HighLimit() const override { return 0; }
  };

  /// \f$ \Delta m^2_{32} constrained to inverted hierarchy \f$
  extern const FitDmSq32IHScaled kFitDmSq32IHScaled;

  //----------------------------------------------------------------------

  /// \f$ \tan^2\theta_{12} \f$
  class FitTanSqTheta12: public IConstrainedFitVar
  {
  public:
    FitTanSqTheta12() : IConstrainedFitVar("tsth12", "tan^{2}#theta_{12}") {};
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;
    double LowLimit() const override { return 0; }
    double HighLimit() const override { return std::numeric_limits<double>::max(); }
  };

  /// \f$ \tan^2\theta_{12} \f$
  extern const FitTanSqTheta12 kFitTanSqTheta12;

  //----------------------------------------------------------------------

  /// \f$ \sin^22\theta_{12} \f$
  class FitSinSq2Theta12: public IConstrainedFitVar
  {
  public:
    FitSinSq2Theta12() : IConstrainedFitVar("ss2th12", "sin^{2}2#theta_{12}") {};
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;

    double LowLimit() const override { return 0; }
    double HighLimit() const override { return 1; }
  };

  /// \f$ \sin^22\theta_{12} \f$
  extern const FitSinSq2Theta12 kFitSinSq2Theta12;

  //----------------------------------------------------------------------

  /// \f$ \Delta m^2_{21} \f$
  class FitDmSq21: public IConstrainedFitVar
  {
  public:
    FitDmSq21() : IConstrainedFitVar("dmsq21", "#Deltam^{2}_{21}") {};

    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;

    // "1eV^2 splitting should be enough for anyone"
    // OscCalculatorPMNS freaks out at large splittings
    double LowLimit() const override { return -1; }
    double HighLimit() const override { return +1; }
  };

  /// \f$ \Delta m^2_{21} \f$
  extern const FitDmSq21 kFitDmSq21;

  //----------------------------------------------------------------------
  /// \f$ \Delta m^2_{21} \f$
  class FitDmSq21Scaled: public IConstrainedFitVar
  {
  public:
   FitDmSq21Scaled() : IConstrainedFitVar("dmsq21scaled", "#Deltam^{2}_{21}") {};
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;

    // "1eV^2 splitting should be enough for anyone"
    // OscCalculatorPMNS freaks out at large splittings
    double LowLimit() const override { return -1e5; }
    double HighLimit() const override { return +1e5; }
  };

  /// \f$ \Delta m^2_{21} \f$
  extern const FitDmSq21Scaled kFitDmSq21Scaled;
  //----------------------------------------------------------------------

  /// \f$ \rho \f$
  class FitRho: public IConstrainedFitVar
  {
  public:
    FitRho() : IConstrainedFitVar("rho", "#rho") {}
    double GetValue(const osc::IOscCalculatorAdjustable* osc) const override;
    void SetValue(osc::IOscCalculatorAdjustable* osc, double val) const override;

    //Density should be greater than zero (set a ridiculously high high limit)
    double LowLimit() const override {return 0;}
    double HighLimit() const override {return 10.0;}

  };

  /// \f$ \rho \f$
  extern const FitRho kFitRho;

  //----------------------------------------------------------------------

} // namespace ana
