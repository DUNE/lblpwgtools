#pragma once

#include "CAFAna/Prediction/PredictionScaleComp.h"

#include "CAFAna/Systs/DUNEXSecSysts.h"

namespace ana
{
  class DUNEXSecDiagSyst: public ISyst
  {
  public:
    virtual void Shift(double, ana::Restorer&, caf::SRProxy*, double&) const
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
                       const Weight&     wei = kUnweighted);

    /// PredictionScaleComp for the FD
    PredictionXSecDiag(SpectrumLoaderBase& loaderNonswap,
                       SpectrumLoaderBase& loaderNue,
                       SpectrumLoaderBase& loaderNuTau,
                       const HistAxis&   axis,
                       Cut               cut,
                       const SystShifts& shift = kNoShift,
                       const Weight&     wei = kUnweighted);

    virtual Spectrum Predict(osc::IOscCalc* calc) const override
    {
      return PredictSyst(calc, SystShifts::Nominal());
    }

    virtual Spectrum PredictSyst(osc::IOscCalc* calc,
                                 const SystShifts&    shift) const override
    {
      return PredictComponentSyst(calc, shift,
                                  Flavors::kAll, Current::kBoth, Sign::kBoth);
    }

    Spectrum PredictComponent(osc::IOscCalc* calc,
                              Flavors::Flavors_t flav,
                              Current::Current_t curr,
                              Sign::Sign_t sign) const override
    {
      return PredictComponentSyst(calc, SystShifts::Nominal(), flav, curr, sign);
    }

    virtual Spectrum PredictComponentSyst(osc::IOscCalc* calc,
                                          const SystShifts& syst,
                                          Flavors::Flavors_t flav,
                                          Current::Current_t curr,
                                          Sign::Sign_t sign) const override;

    /// For translating results of a fit
    SystShifts Undiagonalize(const SystShifts& s) const;

    void SaveTo(TDirectory* dir, const std::string& name) const override {fPred->SaveTo(dir, name);}
    static std::unique_ptr<PredictionXSecDiag> LoadFrom(TDirectory* dir, const std::string& name);

  protected:
    void InitCoeffs();

    IPrediction* fPred;

    std::vector<const ISyst*> fSysts;
    std::vector<const DUNEXSecDiagSyst*> fDiagSysts;

    // [diagonalized index][underlying index]
    static std::vector<std::vector<double>> fgCoeffs;
  };
}
