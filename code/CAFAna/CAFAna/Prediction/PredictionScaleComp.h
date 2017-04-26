#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Prediction/IPrediction.h"

#include "StandardRecord/StandardRecord.h"

#include "OscLib/func/IOscCalculator.h"

namespace ana
{
  class DUNERunPOTSpectrumLoader;

  /// \brief Prediction broken down into arbitrary components whose scales can
  /// be varied independently.
  class PredictionScaleComp : public IPrediction
  {
  public:
    /// \param cut Cut applied to all histograms
    /// \param truthcuts Prediction will be broken down into N components
    ///                  following these cuts.
    PredictionScaleComp(SpectrumLoaderBase& loader,
                        const HistAxis&     axis,
                        Cut                 cut,
                        std::vector<Cut>    truthcuts,
                        const SystShifts&   shift = kNoShift,
                        const Var&          wei = kUnweighted);
    /// Constructor to take two HistAxis's to weight 2D spectra
    PredictionScaleComp(SpectrumLoaderBase& loader,
                        const HistAxis&     axis1,
                        const HistAxis&     axis2,
                        Cut                 cut,
                        std::vector<Cut>    truthcuts,
                        const SystShifts&   shift = kNoShift,
                        const Var&          wei = kUnweighted);

    /// This is for the FD via PredictionNoExtrap
    PredictionScaleComp(DUNERunPOTSpectrumLoader& loaderBeam,
                        DUNERunPOTSpectrumLoader& loaderNue,
                        DUNERunPOTSpectrumLoader& loaderNuTau,
                        DUNERunPOTSpectrumLoader& loaderNC,
                        const HistAxis&     axis,
                        Cut                 cut,
                        std::vector<Cut>    truthcuts,
                        const SystShifts&   shift = kNoShift,
                        const Var&          wei = kUnweighted);

    virtual ~PredictionScaleComp();

    Spectrum Predict(osc::IOscCalculator* osc) const override;
    Spectrum PredictSyst(osc::IOscCalculator* osc,
                         const SystShifts&    syst) const override;
    Spectrum PredictComponent(osc::IOscCalculator* calc,
                              Flavors::Flavors_t flav,
                              Current::Current_t curr,
                              Sign::Sign_t sign) const override
    {
      assert(0 && "Please don't use PredictionScaleComp::PredictComponent");
    }

    /// Use these systematics in PredictSyst to vary the corresponding
    /// components
    std::vector<const ISyst*> GetSysts() const { return fSysts; }

    //    std::vector<Spectrum*> GetSpectra() const { return fSpectra; }

    static std::unique_ptr<PredictionScaleComp> LoadFrom(TDirectory* dir);
    virtual void SaveTo(TDirectory* dir) const;

  private:
    PredictionScaleComp(const IPrediction* complement,
                        std::vector<const IPrediction*> preds);

    std::vector<const ISyst*> fSysts;
    std::vector<const IPrediction*> fPreds;

    const IPrediction* fComplement;
  };
}
