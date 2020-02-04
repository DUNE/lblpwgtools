#pragma once

#include "CAFAna/Prediction/PredictionScaleComp.h"

#include "CAFAna/Systs/DUNEXSecSysts.h"

namespace ana
{
  class DUNEXSecDiagSyst: public ISyst
  {
  public:
    virtual void Shift(double, ana::Restorer&, caf::StandardRecord*, double&) const
    {
      assert(0 && "unimplemented");
    }

  protected:
    friend const DUNEXSecDiagSyst* GetDUNEXSecDiagSyst(int i);
    DUNEXSecDiagSyst(int i);

    int fIdx;
  };

  const DUNEXSecDiagSyst* GetDUNEXSecDiagSyst(int i);
  SystVector<DUNEXSecDiagSyst> GetDUNEXSecDiagSysts();

  class PredictionXSecDiag: public IPrediction
  {
  public:
    /// Wrap any existing prediction object
    PredictionXSecDiag(IPrediction* p);

    /// PredictionScaleComp for the ND
    PredictionXSecDiag(SpectrumLoaderBase& loader,
                       const HistAxis&   axis,
                       Cut               cut,
                       const SystShifts& shift = kNoShift,
                       const Var&        wei = kUnweighted);

    /// PredictionScaleComp for the FD
    PredictionXSecDiag(SpectrumLoaderBase& loaderNonswap,
                       SpectrumLoaderBase& loaderNue,
                       SpectrumLoaderBase& loaderNuTau,
                       const HistAxis&   axis,
                       Cut               cut,
                       const SystShifts& shift = kNoShift,
                       const Var&        wei = kUnweighted);

    virtual Spectrum Predict(osc::IOscCalculator* calc) const override
    {
      return PredictSyst(calc, SystShifts::Nominal());
    }

    virtual Spectrum PredictSyst(osc::IOscCalculator* calc,
                                 const SystShifts&    shift) const override
    {
      return PredictComponentSyst(calc, shift,
                                  Flavors::kAll, Current::kBoth, Sign::kBoth);
    }

    Spectrum PredictComponent(osc::IOscCalculator* calc,
                              Flavors::Flavors_t flav,
                              Current::Current_t curr,
                              Sign::Sign_t sign) const override
    {
      return PredictComponentSyst(calc, SystShifts::Nominal(), flav, curr, sign);
    }

    virtual Spectrum PredictComponentSyst(osc::IOscCalculator* calc,
                                          const SystShifts& syst,
                                          Flavors::Flavors_t flav,
                                          Current::Current_t curr,
                                          Sign::Sign_t sign) const override;

    /// For translating results of a fit
    SystShifts Undiagonalize(const SystShifts& s) const;

    void SaveTo(TDirectory* dir) const override {fPred->SaveTo(dir);}
    static std::unique_ptr<PredictionXSecDiag> LoadFrom(TDirectory* dir);

  protected:
    void InitCoeffs();

    IPrediction* fPred;

    std::vector<const ISyst*> fSysts;
    std::vector<const DUNEXSecDiagSyst*> fDiagSysts;

    // [diagonalized index][underlying index]
    static std::vector<std::vector<double>> fgCoeffs;
  };
}
