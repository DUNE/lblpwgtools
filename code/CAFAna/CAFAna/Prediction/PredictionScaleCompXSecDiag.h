#pragma once

#include "CAFAna/Prediction/PredictionScaleComp.h"

#include "CAFAna/Systs/DUNEXSecSysts.h"

namespace ana
{
  class DUNEXSecDiagSyst: public ISyst
  {
  public:
    virtual std::set<std::string> Requires() const override {return {};}

    virtual std::string ShortName() const override;
    virtual std::string LatexName() const override;

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

  class PredictionScaleCompXSecDiag: public PredictionScaleComp
  {
  public:
    /// This is for the ND
    PredictionScaleCompXSecDiag(SpectrumLoaderBase& loader,
                                const HistAxis&     axis,
                                Cut                 cut,
                                const SystShifts&   shift = kNoShift,
                                const Var&          wei = kUnweighted);

    /// This is for the FD via PredictionNoExtrap
    PredictionScaleCompXSecDiag(DUNERunPOTSpectrumLoader& loaderBeam,
                                DUNERunPOTSpectrumLoader& loaderNue,
                                DUNERunPOTSpectrumLoader& loaderNuTau,
                                DUNERunPOTSpectrumLoader& loaderNC,
                                const HistAxis&     axis,
                                Cut                 cut,
                                const SystShifts&   shift = kNoShift,
                                const Var&          wei = kUnweighted);

    virtual Spectrum PredictSyst(osc::IOscCalculator* calc,
                                 const SystShifts&    shift) const override;

    /// For translating results of a fit
    SystShifts Undiagonalize(const SystShifts& s) const;

    static std::unique_ptr<PredictionScaleCompXSecDiag> LoadFrom(TDirectory* dir);

  protected:
    PredictionScaleCompXSecDiag(const PredictionScaleComp* p);

    void InitCoeffs();

    std::vector<const DUNEXSecDiagSyst*> fDiagSysts;

    // [diagonalized index][underlying index]
    static std::vector<std::vector<double>> fgCoeffs;
  };
}
