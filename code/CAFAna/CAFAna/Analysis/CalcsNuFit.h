#pragma once

#include "CAFAna/Experiment/IExperiment.h"

#include "TMath.h"

namespace ana{class IOscCalculatorAdjustable;}

namespace ana
{
  // http://www.nu-fit.org/?q=node/139
  const double kNuFitDmsq21CV = 7.50e-5;
  const double kNuFitTh12CV = 33.56 * TMath::Pi()/180;

  // Have to adjust for nu-fit's weird convention in NH
  const double kNuFitDmsq32CVNH = +2.524e-3 - kNuFitDmsq21CV;
  const double kNuFitTh23CVNH = 41.6 * TMath::Pi()/180;
  const double kNuFitTh13CVNH = 8.46 * TMath::Pi()/180;
  const double kNuFitdCPCVNH = 261 * TMath::Pi()/180;

  const double kNuFitDmsq32CVIH = -2.514e-3;
  const double kNuFitTh23CVIH = 50.0 * TMath::Pi()/180;
  const double kNuFitTh13CVIH = 8.49 * TMath::Pi()/180;
  const double kNuFitdCPCVIH = 277 * TMath::Pi()/180;

  // Based on 1/3 of the 3sigma error
  const double kNuFitDmsq21Err = ((8.90-7.50)/3)*1e-5;
  const double kNuFitTh12Err = ((35.99-33.56)/3) * TMath::Pi()/180;

  const double kNuFitDmsq32ErrNH = ((2.643-2.524)/3)*1e-3;
  const double kNuFitTh23ErrNH = ((52.8-41.6)/3) * TMath::Pi()/180;
  const double kNuFitTh13ErrNH = ((8.90-8.46)/3) * TMath::Pi()/180;

  const double kNuFitDmsq32ErrIH = ((2.635-2.514)/3)*1e-3;
  const double kNuFitTh23ErrIH = ((53.1-50.0)/3) * TMath::Pi()/180;
  const double kNuFitTh13ErrIH = ((8.93-8.49)/3) * TMath::Pi()/180;


  // hie = +/-1
  osc::IOscCalculatorAdjustable* NuFitOscCalc(int hie);

  osc::IOscCalculatorAdjustable* NuFitOscCalcPlusOneSigma(int hie);

  class NuFitPenalizer: public IExperiment
  {
  public:
    double ChiSq(osc::IOscCalculatorAdjustable* calc,
                 const SystShifts& syst = SystShifts::Nominal()) const override;
  };
};
