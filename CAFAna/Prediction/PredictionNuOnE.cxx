#include "CAFAna/Prediction/PredictionNuOnE.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "StandardRecord/StandardRecord.h"

#include "TDirectory.h"
#include "TObjString.h"

namespace ana
{
  // Simple counting experiment
  const HistAxis kNuOnEaxis("Dummy nu-on-e axis", Binning::Simple(1, 0, 2), kUnweighted);

  const Var kThetaReco = SIMPLEVAR(theta_reco);
  const Var kElepReco = SIMPLEVAR(Elep_reco);
  const Var kEsqTheta = kElepReco * kThetaReco * kThetaReco;

  const Var kEhadVeto = SIMPLEVAR(Ehad_veto);

  // Private communication from Chris M
  const Cut kNuOnECut = kEsqTheta < .002 && kElepReco > .5 && kIsTrueFV && kEhadVeto < 20;

  // Account for reduced efficiency of selecting photon shower as backgrounds
  // into an analysis looking for electron showers.
  const Var kNCScaleFactor = Constant(.1);

  // --------------------------------------------------------------------------
  PredictionNuOnE::PredictionNuOnE(SpectrumLoaderBase& sigLoader,
                                   SpectrumLoaderBase& ccBkgLoader,
                                   SpectrumLoaderBase& ncBkgLoader,
                                   const SystShifts& shift,
                                   const Var& wei)
    : fSig  (  sigLoader, kNuOnEaxis, kNuOnECut, shift, wei),
      fCCBkg(ccBkgLoader, kNuOnEaxis, kNuOnECut, shift, wei),
      fNCBkg(ncBkgLoader, kNuOnEaxis, kNuOnECut, shift, wei * kNCScaleFactor)
  {
  }

  // --------------------------------------------------------------------------
  Spectrum PredictionNuOnE::Predict(osc::IOscCalc*) const
  {
    return fSig+fCCBkg+fNCBkg;
  }

  //----------------------------------------------------------------------
  Spectrum PredictionNuOnE::PredictComponent(osc::IOscCalc* calc,
                                             Flavors::Flavors_t flav,
                                             Current::Current_t curr,
                                             Sign::Sign_t sign) const
  {
    // Get binning
    Spectrum ret = fSig;
    ret.Clear();
    
    if(curr & Current::kCC){
      if(flav & Flavors::kNuMuToNuMu) ret += fSig; // mostly...
      if(flav & Flavors::kNuEToNuE) ret += fCCBkg;
    }

    if(curr & Current::kNC){
      assert(flav == Flavors::kAll);
      ret += fNCBkg;
    }

    return ret;
  }

  // --------------------------------------------------------------------------
  void PredictionNuOnE::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = dir;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("PredictionNuOnE").Write("type");

    fSig.SaveTo(dir, "sig");
    fCCBkg.SaveTo(dir, "ccbkg");
    fNCBkg.SaveTo(dir, "ncbkg");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<PredictionNuOnE> PredictionNuOnE::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    TObjString* ptag = (TObjString*)dir->Get("type");
    assert(ptag);
    assert(ptag->GetString() == "PredictionNuOnE");

    PredictionNuOnE* ret = new PredictionNuOnE
      (*Spectrum::LoadFrom(dir, "sig"),
       *Spectrum::LoadFrom(dir, "ccbkg"),
       *Spectrum::LoadFrom(dir, "ncbkg"));

    delete dir;

    return std::unique_ptr<PredictionNuOnE>(ret);
  }

  //----------------------------------------------------------------------
  std::unique_ptr<IPrediction> PredictionNuOnEGenerator::
  Generate(Loaders& loaders, const SystShifts& shiftMC) const
  {
    NuOnELoaders* noel = dynamic_cast<NuOnELoaders*>(&loaders);
    if(!noel){
      std::cout << "PredictionNuOnEGenerator must be used in conjunction with NuOnELoaders" << std::endl;
      abort();
    }

    return std::unique_ptr<IPrediction>
      (new PredictionNuOnE(noel->Signal(), noel->CCBkg(), noel->NCBkg(),
                           shiftMC, fWei));
  }

}
