#pragma once

#include "CAFAna/Core/IFitVar.h"
#include "TMath.h"

namespace ana
{
  //----------------------------------------------------------------------


//#####################################################################
//                       Diagonal parameters                          #
//#####################################################################

//#*******************************************************************#
//#                                                                   #
//#               		          Eps_ee           	                    #
//#                                                                   #
//#*******************************************************************#

 /// \f$ \epsilon_{ee} \f$
  class FitEpsEE: virtual public IConstrainedFitVar
  {
    public:
      FitEpsEE() : IConstrainedFitVar("eps_ee", "#varepsilon_{ee}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;
      virtual double LowLimit() const override {return -5.0;}
      virtual double HighLimit() const override {return 5.0;}
  };

  /// \f$ \epsilon_{ee} \f$
  extern const FitEpsEE kFitEpsEE;



//#*******************************************************************#
//#                                                                   #
//#               		      Eps_mumu           	                      #
//#                                                                   #
//#*******************************************************************#

 /// \f$ \epsilon_{\mu\mu} \f$
  class FitEpsMuMu: virtual public IConstrainedFitVar
  {
    public:
      FitEpsMuMu() : IConstrainedFitVar("eps_mumu", "#varepsilon_{#mu#mu}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;
      virtual double LowLimit() const override {return -5.0;}
      virtual double HighLimit() const override {return 5.0;}
  };

  /// \f$ \epsilon_{\mu\mu} \f$
  extern const FitEpsMuMu kFitEpsMuMu;
  
//#*******************************************************************#
//#                                                                   #
//#                     		Eps_tautau           	                    #
//#                                                                   #
//#*******************************************************************#

 /// \f$ \epsilon_{\tau\tau} \f$
  class FitEpsTauTau: virtual public IConstrainedFitVar
  {
    public:
      FitEpsTauTau() : IConstrainedFitVar("eps_tautau", "#varepsilon_{#tau#tau}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;
      virtual double LowLimit() const override {return -5.0;}
      virtual double HighLimit() const override {return 5.0;}
  };

  /// \f$ \epsilon_{\tau\tau} \f$
  extern const FitEpsTauTau kFitEpsTauTau;   


//#####################################################################
//                     Off-diagonal parameters                        #
//#####################################################################


//#*******************************************************************#
//#                                                                   #
//#               		    |Eps_mutau|               	                #
//#                                                                   #
//#*******************************************************************#

 /// \f$ \epsilon_{\mu\tau} \f$
  class FitEpsMuTau: virtual public IConstrainedFitVar
  {
    public:
      FitEpsMuTau() : IConstrainedFitVar("|eps_mutau|", "#left|#varepsilon_{#mu#tau}#right|") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;
      virtual double LowLimit() const override {return 0.0;}
      virtual double HighLimit() const override {return 2.5;}
  };

  /// \f$ \epsilon_{\mu\tau} \f$
  extern const FitEpsMuTau kFitEpsMuTau;
  
  
  
//#*******************************************************************#
//#                                                                   #
//#               	      	delta_mutau              	                #
//#                                                                   #
//#*******************************************************************# 
  
  /// \f$ \delta_{\mu\tau}/\pi \f$
  class FitDeltaMuTauInPiUnits: virtual public IFitVar
  {
    public:
      FitDeltaMuTauInPiUnits() : IFitVar("delta_mutau", "#delta_{#mu#tau} / #pi") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;
  };

  /// \f$ \delta_{\mu\tau}/\pi \f$
  extern const FitDeltaMuTauInPiUnits kFitDeltaMuTauInPiUnits;
  

  
//#*******************************************************************#
//#                                                                   #
//#               	        	|Eps_emu|              	                #
//#                                                                   #
//#*******************************************************************# 
   
  /// \f$ \epsilon_{\e\mu} \f$
  class FitEpsEMu: virtual public IConstrainedFitVar
  {
    public:
      FitEpsEMu() : IConstrainedFitVar("|eps_emu|", "#left|#varepsilon_{e#mu}#right|") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;
      virtual double LowLimit() const override {return 0.0;} 
      virtual double HighLimit() const override {return 2.5;}
  };
  /// \f$ \epsilon_{\e\mu} \f$
  extern const FitEpsEMu kFitEpsEMu;
  
  
//#*******************************************************************#
//#                                                                   #
//#               	      	delta_emu               	                #
//#                                                                   #
//#*******************************************************************#
  
  /// \f$ \delta_{\e\mu}/\pi \f$
  class FitDeltaEMuInPiUnits: virtual public IFitVar
  {
    public:
      FitDeltaEMuInPiUnits() : IFitVar("delta_emu", "#delta_{e#mu} / #pi") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;
  };

  /// \f$ \delta_{\e\mu}/\pi \f$
  extern const FitDeltaEMuInPiUnits kFitDeltaEMuInPiUnits;


//#*******************************************************************#
//#                                                                   #
//#               		      |Eps_etau|               	                #
//#                                                                   #
//#*******************************************************************# 
   
  /// \f$ \epsilon_{\e\tau} \f$
  class FitEpsETau: virtual public IConstrainedFitVar
  {
    public:
      FitEpsETau() : IConstrainedFitVar("|eps_etau|", "#left|#varepsilon_{e#tau}#right|") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;
      virtual double LowLimit() const override {return 0.0;} 
      virtual double HighLimit() const override {return 2.5;}
  };
  /// \f$ \epsilon_{\e\tau} \f$
  extern const FitEpsETau kFitEpsETau;
  
  
//#*******************************************************************#
//#                                                                   #
//#               		    delta_etau                	                #
//#                                                                   #
//#*******************************************************************#
  
  /// \f$ \delta_{\e\tau}/\pi \f$
  class FitDeltaETauInPiUnits: virtual public IFitVar
  {
    public:
      FitDeltaETauInPiUnits() : IFitVar("delta_etau", "#delta_{e#tau} / #pi") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;
  };

  /// \f$ \delta_{\e\tau}/\pi \f$
  extern const FitDeltaETauInPiUnits kFitDeltaETauInPiUnits;
  
//#####################################################################
//                      NSI as real parameters                        #
//#####################################################################

//#*******************************************************************#
//#                                                                   #
//#                 		 Eps_mutau Real                               #
//#                                                                   #
//#*******************************************************************#

/// \f$ \epsilon_{\mu\tau} Real\f$
  class FitEpsMuTauReal: virtual public IConstrainedFitVar
  {
    public:
      FitEpsMuTauReal() : IConstrainedFitVar("eps_mutau (real)", "#varepsilon_{#mu#tau}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;
      virtual double LowLimit() const override {return -2.5;}
      virtual double HighLimit() const override {return 2.5;}
  };

  /// \f$ \epsilon_{\mu\tau} Real\f$
  extern const FitEpsMuTauReal kFitEpsMuTauReal;



//#*******************************************************************#
//#                                                                   #
//#                 		  Eps_emu Real              	                #
//#                                                                   #
//#*******************************************************************#

/// \f$ \epsilon_{\e\mu} Real\f$
  class FitEpsEMuReal: virtual public IConstrainedFitVar
  {
    public:
      FitEpsEMuReal() : IConstrainedFitVar("eps_emu (real)", "#varepsilon_{e#mu}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;
      virtual double LowLimit() const override {return -2.5;} 
      virtual double HighLimit() const override {return 2.5;}
  };
  /// \f$ \epsilon_{\e\mu} Real\f$
  extern const FitEpsEMuReal kFitEpsEMuReal;



//#*******************************************************************#
//#                                                                   #
//#                 		 Eps_etau Real                                #
//#                                                                   #
//#*******************************************************************#

/// \f$ \epsilon_{\e\tau} Real\f$
  class FitEpsETauReal: virtual public IConstrainedFitVar
  {
    public:
      FitEpsETauReal() : IConstrainedFitVar("eps_etau (real)", "#varepsilon_{e#tau}") {};

      double GetValue(const osc::IOscCalcAdjustable* osc) const override;
      void SetValue(osc::IOscCalcAdjustable* osc, double val) const override;
      virtual double LowLimit() const override {return -2.5;} 
      virtual double HighLimit() const override {return 2.5;}
  };
  /// \f$ \epsilon_{\e\tau} Real\f$
  extern const FitEpsETauReal kFitEpsETauReal;
  
} // namespace