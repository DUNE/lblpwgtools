#pragma once

#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/ISyst.h"

#include "CAFAna/Experiment/IExperiment.h"

class TH2;

namespace ana
{
  enum EVALORCategory{
    k_int_nu_ccqe_1 = 0,
    k_int_nu_ccqe_2 = 1,
    k_int_nu_ccqe_3 = 2,
    k_int_nubar_ccqe_1 = 3,
    k_int_nubar_ccqe_2 = 4,
    k_int_nubar_ccqe_3 = 5,
    k_int_nu_MEC_dummy = 6,
    k_int_nubar_MEC_dummy = 7,
    k_int_nu_cc1piz_1 = 8,
    k_int_nu_cc1piz_2 = 9,
    k_int_nu_cc1piz_3 = 10,
    k_int_nu_cc1pic_1 = 11,
    k_int_nu_cc1pic_2 = 12,
    k_int_nu_cc1pic_3 = 13,
    k_int_nubar_cc1piz_1 = 14,
    k_int_nubar_cc1piz_2 = 15,
    k_int_nubar_cc1piz_3 = 16,
    k_int_nubar_cc1pic_1 = 17,
    k_int_nubar_cc1pic_2 = 18,
    k_int_nubar_cc1pic_3 = 19,
    k_int_nu_2pi = 20,
    k_int_nubar_2pi = 21,
    k_int_nu_dis_1 = 22,
    k_int_nu_dis_2 = 23,
    k_int_nu_dis_3 = 24,
    k_int_nubar_dis_1 = 25,
    k_int_nubar_dis_2 = 26,
    k_int_nubar_dis_3 = 27,
    k_int_nu_coh = 28,
    k_int_nubar_coh = 29,
    k_int_nu_nc = 30,
    k_int_nubar_nc = 31,
    k_int_nuenumu_dummy = 32
  };

  std::string VALORCategoryName(EVALORCategory c);

  EVALORCategory GetVALORCategory(const caf::StandardRecord* sr);

  const Var kVALORCategory({}, GetVALORCategory);

  class DUNEXSecSyst: public ISyst
  {
  public:
    DUNEXSecSyst(EVALORCategory cat);

    virtual std::set<std::string> Requires() const override {return {};}

    virtual std::string ShortName() const override {return VALORCategoryName(fCat);}

    virtual std::string LatexName() const override {return VALORCategoryName(fCat);}

    virtual void Shift(double sigma,
                       Restorer& restore,
                       caf::StandardRecord* sr,
                       double& weight) const override
    {
      if(GetVALORCategory(sr) == fCat) weight *= 1+sigma*fScale;
    }

  protected:
    EVALORCategory fCat;
    double fScale;
  };

  class DUNEXSecCorrelation: public IExperiment
  {
  public:
    DUNEXSecCorrelation();

    virtual double ChiSq(osc::IOscCalculatorAdjustable* osc,
                         const SystShifts& syst = SystShifts::Nominal()) const override;

  protected:
    TH2* fMatrix;
  };
}
