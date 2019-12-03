#pragma once

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/ISyst.h"

#include "CAFAna/Experiment/IExperiment.h"
#include "CAFAna/Prediction/PredictionScaleComp.h"

class TH2;

#include "TMatrixD.h"

namespace ana
{
  enum EVALORCategory{
    nu_ccqe_1 = 0,
    nu_ccqe_2 = 1,
    nu_ccqe_3 = 2,
    nubar_ccqe_1 = 3,
    nubar_ccqe_2 = 4,
    nubar_ccqe_3 = 5,
    nu_MEC_dummy = 6,
    nubar_MEC_dummy = 7,
    nu_cc1piz_1 = 8,
    nu_cc1piz_2 = 9,
    nu_cc1piz_3 = 10,
    nu_cc1pic_1 = 11,
    nu_cc1pic_2 = 12,
    nu_cc1pic_3 = 13,
    nubar_cc1piz_1 = 14,
    nubar_cc1piz_2 = 15,
    nubar_cc1piz_3 = 16,
    nubar_cc1pic_1 = 17,
    nubar_cc1pic_2 = 18,
    nubar_cc1pic_3 = 19,
    nu_2pi = 20,
    nubar_2pi = 21,
    nu_dis_1 = 22,
    nu_dis_2 = 23,
    nu_dis_3 = 24,
    nubar_dis_1 = 25,
    nubar_dis_2 = 26,
    nubar_dis_3 = 27,
    nu_coh = 28,
    nubar_coh = 29,
    nu_nc = 30,
    nubar_nc = 31,
    nuenumu_dummy = 32 // TODO TODO
  };

  std::string VALORCategoryName(EVALORCategory c);
  std::string VALORCategoryLatexName(EVALORCategory c);

  EVALORCategory GetVALORCategory(const caf::StandardRecord* sr);

  const Var kVALORCategory({}, GetVALORCategory);

  /// Warning, these aren't orthogonal. Need to include DUNEXSecCorrelation in
  /// the fit (which gives MINUIT a seizure) for correctness. You may well want
  /// DUNEXSecSystPCA instead.
  class DUNEXSecSyst: public SystComponentScale
  {
  public:
    static std::unique_ptr<DUNEXSecSyst> LoadFrom(TDirectory* dir);
    virtual void SaveTo(TDirectory* dir) const override;
  protected:
    friend const DUNEXSecSyst* GetDUNEXSecSyst(EVALORCategory);
    DUNEXSecSyst(EVALORCategory cat);
  };

  const DUNEXSecSyst* GetDUNEXSecSyst(EVALORCategory cat);

  // Because vector<T*> won't automatically convert to vector<U*> even when U
  // inherits from V.
  template<class T> struct SystVector: public std::vector<const T*>
  {
    // U = any parent class
    template<class U> operator std::vector<const U*>()
    {
      return std::vector<const U*>(std::vector<const T*>::begin(),
                                   std::vector<const T*>::end());
    }
  };

  SystVector<DUNEXSecSyst> GetDUNEXSecSysts();


  class DUNEXSecCorrelation: public IExperiment
  {
  public:
    DUNEXSecCorrelation();

    virtual double ChiSq(osc::IOscCalculatorAdjustable* osc,
                         const SystShifts& syst = SystShifts::Nominal()) const override;

  protected:
    TMatrixD fMatrix;

    std::vector<double> fScales;

    std::vector<const ISyst*> fSysts;
  };
}
