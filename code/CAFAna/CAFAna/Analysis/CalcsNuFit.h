#pragma once

#include "CAFAna/Experiment/IExperiment.h"
#include "CAFAna/Core/IFitVar.h"

#include "TMath.h"

#include "TRandom3.h"

namespace ana{class IOscCalculatorAdjustable;}

namespace ana
{
  // http://www.nu-fit.org/?q=node/177
  // NuFit November 2018
  const double kNuFitDmsq21CV = 7.39e-5;
  const double kNuFitTh12CV = 33.82 * TMath::Pi()/180;

  // Have to adjust for nu-fit's weird convention in NH
  const double kNuFitDmsq32CVNH = +2.525e-3 - kNuFitDmsq21CV;
  const double kNuFitTh23CVNH = 49.6 * TMath::Pi()/180;
  const double kNuFitTh13CVNH = 8.61 * TMath::Pi()/180;
  const double kNuFitdCPCVNH = 215 * TMath::Pi()/180;

  const double kNuFitDmsq32CVIH = -2.512e-3;
  const double kNuFitTh23CVIH = 49.8 * TMath::Pi()/180;
  const double kNuFitTh13CVIH = 8.65 * TMath::Pi()/180;
  const double kNuFitdCPCVIH = 284 * TMath::Pi()/180;

  //Additional values of oscillation parameters
  //Th23 variations
  const double kNuFitTh23HiNH = 51.2 * TMath::Pi()/180;
  const double kNuFitTh23LoNH = 47.3 * TMath::Pi()/180;
  const double kNuFitTh23HiIH = 51.4 * TMath::Pi()/180;
  const double kNuFitTh23LoIH = 47.8 * TMath::Pi()/180;
  const double kNuFitTh23MM = 45 * TMath::Pi()/180;
  const double kNuFitTh23MaxNH = 52.4 * TMath::Pi()/180;
  const double kNuFitTh23MaxIH = 52.5 * TMath::Pi()/180;
  const double kNuFitTh23MinNH = 40.3 * TMath::Pi()/180;
  const double kNuFitTh23MinIH = 40.6 * TMath::Pi()/180;
  //Th13 variations
  const double kNuFitTh13MaxNH = 8.99 * TMath::Pi()/180;
  const double kNuFitTh13MinNH = 8.22 * TMath::Pi()/180;
  const double kNuFitTh13MaxIH = 9.03 * TMath::Pi()/180;
  const double kNuFitTh13MinIH = 8.27 * TMath::Pi()/180;
  //Dmsq32 variations
  const double kNuFitDmsq32MaxNH = 2.625e-3 - kNuFitDmsq21CV;
  const double kNuFitDmsq32MinNH = 2.427e-3 - kNuFitDmsq21CV;
  const double kNuFitDmsq32MaxIH = -2.412e-3;
  const double kNuFitDmsq32MinIH = -2.611e-3;

  // Based on 1/6 of the +/- 3sigma error
  const double kNuFitDmsq21Err = ((8.01-6.79)/6)*1e-5;
  const double kNuFitTh12Err = ((36.27-31.61)/6) * TMath::Pi()/180;

  const double kNuFitDmsq32ErrNH = ((2.625-2.427)/6)*1e-3;
  const double kNuFitTh23ErrNH = ((52.4-40.3)/6) * TMath::Pi()/180;
  const double kNuFitTh13ErrNH = ((8.99-8.22)/6) * TMath::Pi()/180;

  const double kNuFitDmsq32ErrIH = ((2.611-2.412)/6)*1e-3;
  const double kNuFitTh23ErrIH = ((52.5-40.6)/6) * TMath::Pi()/180;
  const double kNuFitTh13ErrIH = ((9.03-8.27)/6) * TMath::Pi()/180;

  //https://arxiv.org/pdf/1707.02322.pdf
  const double kBaseline = 1284.9;     // km
  const double kEarthDensity = 2.848;  // g/cm^3

  std::vector<std::pair<std::string, double> > ParseAsimovSet(std::string noApologies);

  // hie = +/-1
  // Asimov set values:
  // 0 NuFit 4.0 Central Values
  // 1 Th23 set to 90% CL lower value
  // 2 Th23 set to 90% CL upper value
  // 3 Th23 set to maximal mixing
  // 4 Th23 set to 3sig lower value
  // 5 Th23 set to 3sig upper value
  // 6 Th13 set to 3sig lower value
  // 7 Th13 set to 3sig upper value
  // 8 Dmsq32 set to 3sig lower value
  // 9 Dmsq32 set to 3sig upper value
  // 10 dCP = 0
  // 11 dCP = -pi/2
  osc::IOscCalculatorAdjustable* NuFitOscCalc(int hie, int oct=1, int asimov_set=0);
  
  osc::IOscCalculatorAdjustable* NuFitOscCalc(int hie, int oct, std::string asimov_str);

  osc::IOscCalculatorAdjustable* NuFitOscCalcPlusOneSigma(int hie);

  // Add in a throw for toys
  osc::IOscCalculatorAdjustable* ThrownNuFitOscCalc(int hie, std::vector<const IFitVar*> oscVars, int asimov_set=0);

  // Add a different type of throw which depends less on NuFit
  osc::IOscCalculatorAdjustable* ThrownWideOscCalc(int hie, std::vector<const IFitVar*> oscVars, bool flatth13=false);

  bool HasVar(std::vector<const IFitVar*> oscVars, std::string name);


  class NuFitPenalizer: public IExperiment
  {
  public:
    double ChiSq(osc::IOscCalculatorAdjustable* calc,
                 const SystShifts& syst = SystShifts::Nominal()) const override;

    void Derivative(osc::IOscCalculator*,
                    const SystShifts&,
                    std::unordered_map<const ISyst*, double>&) const override
    {
      // Nothing to do, since we only depend on osc params and the derivatives
      // with the systs are all zero. But need to implement, because the
      // default implementation indicates that we are unable to calculate the
      // necessary derivatives.
    }
  };

  class Penalizer_GlbLike: public IExperiment
  {
  public:
    Penalizer_GlbLike(int hietrue, int octtrue, 
		      bool useTh13=true, bool useTh23=false, bool useDmsq=false,int asimov_set=0);
    Penalizer_GlbLike(osc::IOscCalculatorAdjustable* cvcalc, int hietrue, 
		      bool useTh13=true, bool useTh23=false, bool useDmsq=false);
    Penalizer_GlbLike(int hietrue, int octtrue,
		      bool useTh13=true, bool useTh23=false, bool useDmsq=false,
		      std::string asimov_str="", bool modConstraint=false);
    
    void SetAsimovPoint(int asimov_set);
    void SetAsimovPoint(std::string asimov_str, bool modConstraint=false);

    double Dmsq21CV() const {return fDmsq21;}
    double Th12CV() const {return fTh12;}
    double Dmsq32CV() const {return fDmsq32;}
    double Th23CV() const {return fTh23;}
    double Th13CV() const {return fTh13;}
    double RhoCV() const {return fRho;}

    double Dmsq21Err() const {return fDmsq21Err;}
    double Th12Err() const {return fTh12Err;}
    double Dmsq32Err() const {return fDmsq32Err;}
    double Th23Err() const {return fTh23Err;}
    double Th13Err() const {return fTh13Err;}
    double RhoErr() const {return fRhoErr;}

    double ChiSq(osc::IOscCalculatorAdjustable* calc,
                 const SystShifts& syst = SystShifts::Nominal()) const override;

    void Derivative(osc::IOscCalculator*,
                    const SystShifts&,
                    std::unordered_map<const ISyst*, double>&) const override
    {
      // See justification in NuFitPenalizer::Derivative()
    }

  protected:
    double fDmsq21;
    double fTh12;
    double fDmsq32;
    double fTh23;
    double fTh13;
    double fRho;

    double fDmsq21Err;
    double fTh12Err;
    double fDmsq32Err;
    double fTh23Err;
    double fTh13Err;
    double fRhoErr;
    int fHieTrue;
    int fOctTrue;

  private:
    // Changed to options which turn on or off constraints for parameter fits
    bool fTh13Pen;
    bool fTh23Pen;
    bool fDmsqPen;
  };

}
