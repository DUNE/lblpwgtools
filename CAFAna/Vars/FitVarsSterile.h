#pragma once

#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Vars/FitVarsProduct.h"
#include "TMath.h"

namespace ana
{
  //----------------------------------------------------------------------

  /// \f$ \Delta m^2_{32} \f$
  class FitDmSq32Sterile: public IConstrainedFitVar
  {
    public:
      FitDmSq32Sterile() : IConstrainedFitVar("dmsq32_sterile", "#Deltam^{2}_{32}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;

      double LowLimit() const override {return -1e-2;}
      double HighLimit() const override {return 1e-2;}
  };

  /// \f$ \Delta m^2_{32} \f$
  extern const FitDmSq32Sterile kFitDmSq32Sterile;

  //----------------------------------------------------------------------

  /// \f$ \Delta m^2_{32} NH \f$
  class FitDmSq32NHSterile: public IConstrainedFitVar
  {
    public:
      FitDmSq32NHSterile() : IConstrainedFitVar("dmsq32_nh_sterile", "#Deltam^{2}_{32} NH") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;

      double LowLimit() const override {return 0;}
      double HighLimit() const override {return 1e-2;}
  };

  /// \f$ \Delta m^2_{32} NH \f$
  extern const FitDmSq32NHSterile kFitDmSq32NHSterile;

  //----------------------------------------------------------------------

  /// \f$ \Delta m^2_{32} IH \f$
  class FitDmSq32IHSterile: public IConstrainedFitVar
  {
    public:
      FitDmSq32IHSterile() : IConstrainedFitVar("dmsq32_ih_sterile", "#Deltam^{2}_{32} IH") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;

      double LowLimit() const override {return -1e-2;}
      double HighLimit() const override {return 0;}
  };

  /// \f$ \Delta m^2_{32} IH \f$
  extern const FitDmSq32IHSterile kFitDmSq32IHSterile;

  //----------------------------------------------------------------------

  /// \f$ \Delta m^2_{41} \f$
  class FitDmSq41Sterile: public IConstrainedFitVar
  {
    public:
      FitDmSq41Sterile() : IConstrainedFitVar("dmsq41", "#Deltam^{2}_{41}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;

      double LowLimit() const override {return 1e-4;}
      double HighLimit() const override {return 1e2;}
  };

  /// \f$ \Delta m^2_{41} \f$
  extern const FitDmSq41Sterile kFitDmSq41Sterile;

  //----------------------------------------------------------------------

  /// \f$ \log_{10} \Delta m^2_{41} \f$
  class FitLog10DmSq41: public IConstrainedFitVar
  {
    public:
      FitLog10DmSq41() : IConstrainedFitVar("log10(dmsq41)", "log_{10} #Deltam^{2}_{41}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;

      double LowLimit() const override {return -3;}
      double HighLimit() const override {return 2;}
  };

  /// \f$ \Delta m^2_{41} \f$
  extern const FitLog10DmSq41 kFitLog10DmSq41;

  //----------------------------------------------------------------------

  /// \f$ \Delta m^2_{43} \f$
  class FitDmSq43Sterile: public IFitVar
  {
    public:
      FitDmSq43Sterile() : IFitVar("dmsq43", "#Deltam^{2}_{43}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;
  };

  /// \f$ \Delta m^2_{43} \f$
  extern const FitDmSq43Sterile kFitDmSq43Sterile;

  //----------------------------------------------------------------------

  /// \f$ \delta_{13}/\pi \f$
  class FitDelta13InPiUnitsSterile: public IFitVar
  {
    public:
      FitDelta13InPiUnitsSterile() : IFitVar("delta13(pi)", "#delta_{13} / #pi") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;
  };

  /// \f$ \delta_{CP}/\pi \f$
  extern const FitDelta13InPiUnitsSterile kFitDelta13InPiUnitsSterile;

  //----------------------------------------------------------------------

  /// \f$ \delta_{13}/\pi \f$
  class FitDelta14InPiUnitsSterile: public IFitVar
  {
    public:
      FitDelta14InPiUnitsSterile() : IFitVar("delta14(pi)", "#delta_{14} / #pi") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;
  };

  /// \f$ \delta_{14}/\pi \f$
  extern const FitDelta14InPiUnitsSterile kFitDelta14InPiUnitsSterile;

  //----------------------------------------------------------------------

  /// \f$ \delta_{24}/\pi \f$
  class FitDelta24InPiUnitsSterile: public IConstrainedFitVar
  {
    public:
      FitDelta24InPiUnitsSterile() : IConstrainedFitVar("delta24(pi)", "#delta_{24} / #pi") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;

      double LowLimit() const override {return 0;}
      double HighLimit() const override {return 2;}
  };

  /// \f$ \delta_{24}/\pi \f$
  extern const FitDelta24InPiUnitsSterile kFitDelta24InPiUnitsSterile;

  //----------------------------------------------------------------------

  /// \f$ \theta_{13} \f$
  class FitTheta13Sterile: public IConstrainedFitVar
  {
    public:
      FitTheta13Sterile() : IConstrainedFitVar("th13_sterile", "#theta_{13}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return TMath::Pi()/2;}
  };

  /// \f$ \theta_{13} \f$
  extern const FitTheta13Sterile kFitTheta13Sterile;

  //----------------------------------------------------------------------

  /// \f$ \sin^2\theta_{13} \f$
  class FitSinSqTheta13Sterile: public IConstrainedFitVar
  {
    public:
      FitSinSqTheta13Sterile() : IConstrainedFitVar("ssth13_sterile", "sin^{2}#theta_{13}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return 1;}
  };

  /// \f$ \sin^2\theta_{13} \f$
  extern const FitSinSqTheta13Sterile kFitSinSqTheta13Sterile;

  //----------------------------------------------------------------------

  /// \f$ \sin^2 2 \theta_{13} \f$
  class FitSinSq2Theta13Sterile: public IConstrainedFitVar
  {
  public:
    FitSinSq2Theta13Sterile() : IConstrainedFitVar("ss2th13_sterile", "sin^{2}2#theta_{13}") {};

    double GetValue(const osc::IOscCalcAdjustable* osc) const override;
    void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;

    double LowLimit() const override {return 0;}
    double HighLimit() const override {return 1;}
  };

  /// \f$ \sin^2 2 \theta_{13} \f$
  extern const FitSinSq2Theta13Sterile kFitSinSq2Theta13Sterile;

  //----------------------------------------------------------------------

  /// \f$ \theta_{23} \f$
  class FitTheta23Sterile: public IConstrainedFitVar
  {
    public:
      FitTheta23Sterile() : IConstrainedFitVar("th23_sterile", "#theta_{23}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return TMath::Pi()/2;}
  };

  /// \f$ \theta_{23} \f$
  extern const FitTheta23Sterile kFitTheta23Sterile;

  //----------------------------------------------------------------------

  /// \f$ \theta_{23} LO \f$
  class FitTheta23LOSterile: public IConstrainedFitVar
  {
    public:
      FitTheta23LOSterile() : IConstrainedFitVar("th23_lo_sterile", "#theta_{23} LO") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;

      double LowLimit() const override {return 0;}
      double HighLimit() const override {return TMath::Pi()/4;}
  };

  /// \f$ \theta_{23} LO \f$
  extern const FitTheta23LOSterile kFitTheta23LOSterile;

  //----------------------------------------------------------------------

  /// \f$ \theta_{23} UO \f$
  class FitTheta23UOSterile: public IConstrainedFitVar
  {
    public:
      FitTheta23UOSterile() : IConstrainedFitVar("th23_uo_sterile", "#theta_{23} UO") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return TMath::Pi()/4;}
      double HighLimit() const override {return TMath::Pi()/2;}
  };

  /// \f$ \theta_{23} UO \f$
  extern const FitTheta23UOSterile kFitTheta23UOSterile;

  //----------------------------------------------------------------------

  /// \f$ \sin^2\theta_{23} \f$
  class FitSinSqTheta23Sterile: public IConstrainedFitVar
  {
    public:
      FitSinSqTheta23Sterile() : IConstrainedFitVar("ssth23_sterile", "sin^{2}#theta_{23}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return 1;}
  };

  /// \f$ \sin^2\theta_{23} \f$
  extern const FitSinSqTheta23Sterile kFitSinSqTheta23Sterile;

  //----------------------------------------------------------------------

  /// \f$ \theta_{14} \f$
  class FitTheta14Sterile: public IConstrainedFitVar
  {
    public:
      FitTheta14Sterile() : IConstrainedFitVar("th14", "#theta_{14}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return TMath::Pi()/2;}
  };

  /// \f$ \theta_{14} \f$
  extern const FitTheta14Sterile kFitTheta14Sterile;

  //----------------------------------------------------------------------

  /// \f$ \sin^2\theta_{14} \f$
  class FitSinSqTheta14Sterile: public IConstrainedFitVar
  {
    public:
      FitSinSqTheta14Sterile() : IConstrainedFitVar("ssth14", "sin^{2}#theta_{14}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return 1;}
  };

  /// \f$ \sin^2\theta_{14} \f$
  extern const FitSinSqTheta14Sterile kFitSinSqTheta14Sterile;

  //----------------------------------------------------------------------

  /// \f$ \theta_{24} \f$
  class FitTheta24Sterile: public IConstrainedFitVar
  {
    public:
      FitTheta24Sterile() : IConstrainedFitVar("th24", "#theta_{24}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return TMath::Pi()/4;}
  };

  /// \f$ \theta_{24} \f$
  extern const FitTheta24Sterile kFitTheta24Sterile;

  //----------------------------------------------------------------------

  /// \f$ \sin^2\theta_{24} \f$
  class FitSinSqTheta24Sterile: public IConstrainedFitVar
  {
    public:
      FitSinSqTheta24Sterile() : IConstrainedFitVar("ssth24", "sin^{2}#theta_{24}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return 1;}
  };

  /// \f$ \sin^2\theta_{24} \f$
  extern const FitSinSqTheta24Sterile kFitSinSqTheta24Sterile;

  //----------------------------------------------------------------------

  /// \f$ \sin^2 2\theta_{24} \f$
  class FitSinSq2Theta24Sterile: public IConstrainedFitVar
  {
    public:
      FitSinSq2Theta24Sterile() : IConstrainedFitVar("ss2th24", "sin^{2} 2#theta_{24}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return 1;}
  };

  /// \f$ \sin^2 2\theta_{24} \f$
  extern const FitSinSq2Theta24Sterile kFitSinSq2Theta24Sterile;

  //----------------------------------------------------------------------

  /// \f$ \theta_{34} \f$
  class FitTheta34Sterile: public IConstrainedFitVar
  {
    public:
      FitTheta34Sterile() : IConstrainedFitVar("th34", "#theta_{34}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return TMath::Pi()/2;}
  };

  /// \f$ \theta_{34} \f$
  extern const FitTheta34Sterile kFitTheta34Sterile;

  //----------------------------------------------------------------------

  /// \f$ \sin^2\theta_{34} \f$
  class FitSinSqTheta34Sterile: public IConstrainedFitVar
  {
    public:
      FitSinSqTheta34Sterile() : IConstrainedFitVar("ssth34", "sin^{2}#theta_{34}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return 1;}
  };

  /// \f$ \sin^2\theta_{34} \f$
  extern const FitSinSqTheta34Sterile kFitSinSqTheta34Sterile;

  //----------------------------------------------------------------------

  /// \f$ \theta_{13} \f$
  class FitTheta13InDegreesSterile: public IConstrainedFitVar
  {
    public:
      FitTheta13InDegreesSterile() : IConstrainedFitVar("th13(degrees)", "#theta_{13}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return 90;}
  };

  /// \f$ \theta_{13} \f$
  extern const FitTheta13InDegreesSterile kFitTheta13InDegreesSterile;

  //----------------------------------------------------------------------

  /// \f$ \theta_{23} \f$
  class FitTheta23InDegreesSterile: public IConstrainedFitVar
  {
    public:
      FitTheta23InDegreesSterile() : IConstrainedFitVar("th23(degrees)", "#theta_{23}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return 90;}
  };

  /// \f$ \theta_{23} \f$
  extern const FitTheta23InDegreesSterile kFitTheta23InDegreesSterile;

  //----------------------------------------------------------------------

  /// \f$ \theta_{14} \f$
  class FitTheta14InDegreesSterile: public IConstrainedFitVar
  {
    public:
      FitTheta14InDegreesSterile() : IConstrainedFitVar("th14(degrees)", "#theta_{14}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return 90;}
  };

  /// \f$ \theta_{14} \f$
  extern const FitTheta14InDegreesSterile kFitTheta14InDegreesSterile;

  //----------------------------------------------------------------------

  /// \f$ \theta_{24} \f$
  class FitTheta24InDegreesSterile: public IConstrainedFitVar
  {
    public:
      FitTheta24InDegreesSterile() : IConstrainedFitVar("th24(degrees)", "#theta_{24}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return 45;}
  };

  /// \f$ \theta_{24} \f$
  extern const FitTheta24InDegreesSterile kFitTheta24InDegreesSterile;

  //----------------------------------------------------------------------

  /// \f$ \theta_{34} \f$
  class FitTheta34InDegreesSterile: public IConstrainedFitVar
  {
    public:
      FitTheta34InDegreesSterile() : IConstrainedFitVar("th34(degrees)", "#theta_{34}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;


      double LowLimit() const override {return 0;}
      double HighLimit() const override {return 90;}
  };

  /// \f$ \theta_{34} \f$
  extern const FitTheta34InDegreesSterile kFitTheta34InDegreesSterile;

  //----------------------------------------------------------------------

  // \f$ \sin^{2} 2 #theta_{#mu #tau} \f$
  extern const FitVarsProduct kFitSinSq2ThetaMuTauSterileAlpha;
  extern const FitVarsProductMarg kFitSinSq2ThetaMuTauSterileBeta;

} // namespace