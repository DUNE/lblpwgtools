#include "CAFAna/Prediction/PredictionNuOnE.h"

#include "StandardRecord/StandardRecord.h"

#include "TDirectory.h"
#include "TObjString.h"

namespace ana
{
  // Simple counting experiment
  const HistAxis kNuOnEaxis("Dummy nu-on-e axis", Binning::Simple(1, 0, 2), kUnweighted);

  const Var kThetaReco = SIMPLEVAR(dune.theta_reco);
  const Var kElepReco = SIMPLEVAR(dune.Elep_reco);
  const Cut kNuOnECut = kElepReco * kThetaReco * kThetaReco < .002 && kElepReco > .5;

  // --------------------------------------------------------------------------
  PredictionNuOnE::PredictionNuOnE(SpectrumLoaderBase& sigLoader,
                                   SpectrumLoaderBase& ccBkgLoader,
                                   SpectrumLoaderBase& ncBkgLoader,
                                   const SystShifts& shift,
                                   const Var& wei)
    : fSig  (  sigLoader, kNuOnEaxis, kNuOnECut, shift, wei),
      fCCBkg(ccBkgLoader, kNuOnEaxis, kNuOnECut, shift, wei),
      fNCBkg(ncBkgLoader, kNuOnEaxis, kNuOnECut, shift, wei)
  {
  }

  // --------------------------------------------------------------------------
  Spectrum PredictionNuOnE::Predict(osc::IOscCalculator*) const
  {
    return fSig+fCCBkg+fNCBkg;
  }

  // --------------------------------------------------------------------------
  Spectrum PredictionNuOnE::PredictComponent(osc::IOscCalculator* calc,
                                             Flavors::Flavors_t flav,
                                             Current::Current_t curr,
                                             Sign::Sign_t sign) const
  {
    assert(0 && "PredictionNuOnE::PredictComponent() not implemented");
  }

  // --------------------------------------------------------------------------
  void PredictionNuOnE::SaveTo(TDirectory* dir) const
  {
    TDirectory* tmp = dir;

    dir->cd();
    TObjString("PredictionNuOnE").Write("type");

    fSig.SaveTo(dir->mkdir("sig"));
    fCCBkg.SaveTo(dir->mkdir("ccbkg"));
    fNCBkg.SaveTo(dir->mkdir("ncbkg"));

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<PredictionNuOnE> PredictionNuOnE::LoadFrom(TDirectory* dir)
  {
    TObjString* ptag = (TObjString*)dir->Get("type");
    assert(ptag);
    assert(ptag->GetString() == "PredictionNuOnE");

    assert(dir->GetDirectory("sig"));
    assert(dir->GetDirectory("ccbkg"));
    assert(dir->GetDirectory("ncbkg"));

    PredictionNuOnE* ret = new PredictionNuOnE
      (*Spectrum::LoadFrom(dir->GetDirectory("sig")),
       *Spectrum::LoadFrom(dir->GetDirectory("ccbkg")),
       *Spectrum::LoadFrom(dir->GetDirectory("ncbkg")));

    return std::unique_ptr<PredictionNuOnE>(ret);
  }
}
