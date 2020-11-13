#pragma once

#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Experiment/IExperiment.h"

#include "TMath.h"

#include "TRandom3.h"

namespace ana
{

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
  osc::IOscCalcAdjustable* NuFitOscCalc(int hie, int oct=1, int asimov_set=0);
  
  osc::IOscCalcAdjustable* NuFitOscCalc(int hie, int oct, std::string asimov_str);

  osc::IOscCalcAdjustable* NuFitOscCalcPlusOneSigma(int hie);

  // Add in a throw for toys
  osc::IOscCalcAdjustable* ThrownNuFitOscCalc(int hie, std::vector<const IFitVar*> oscVars, int asimov_set=0);

  // Add a different type of throw which depends less on NuFit
  osc::IOscCalcAdjustable* ThrownWideOscCalc(int hie, std::vector<const IFitVar*> oscVars, bool flatth13=false);

  bool HasVar(std::vector<const IFitVar*> oscVars, std::string name);


  class NuFitPenalizer: public IExperiment
  {
  public:
    double ChiSq(osc::IOscCalcAdjustable* calc,
                 const SystShifts& syst = SystShifts::Nominal()) const override;
  };

  class Penalizer_GlbLike: public IExperiment
  {
  public:
    Penalizer_GlbLike(int hietrue, int octtrue, 
		      bool useTh13=true, bool useTh23=false, bool useDmsq=false,int asimov_set=0);
    Penalizer_GlbLike(osc::IOscCalcAdjustable* cvcalc, int hietrue, 
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

    double ChiSq(osc::IOscCalcAdjustable* calc,
                 const SystShifts& syst = SystShifts::Nominal()) const override;

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
