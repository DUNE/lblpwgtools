#include "CAFAna/Prediction/PredictionNoOsc.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Extrap/IExtrap.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Extrap/TrivialExtrap.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/PRISM/PRISMUtils.h"

#include "TDirectory.h"
#include "TObjString.h"

namespace ana
{
  //----------------------------------------------------------------------
  PredictionNoOsc::PredictionNoOsc(SpectrumLoaderBase& loader,
                                   const std::string& label,
                                   const Binning& bins,
                                   const Var& var,
                                   const Cut& cut,
                                   const SystShifts& shift,
                                   const Weight& wei)
    : PredictionNoOsc(loader, HistAxis(label, bins, var), cut, shift, wei)
  {
  }

  //----------------------------------------------------------------------
  PredictionNoOsc::PredictionNoOsc(SpectrumLoaderBase& loader,
                                   const HistAxis& axis,
                                   const Cut& cut,
                                   const SystShifts& shift,
                                   const Weight& wei)
    : fSpectrum(       loader, axis, cut                                      ,  shift, wei),
      fSpectrumNCTot(  loader, axis, cut &&  kIsNC                            ,  shift, wei),
      fSpectrumNC(     loader, axis, cut &&  kIsNC &&               !kIsAntiNu,  shift, wei),
      fSpectrumNCAnti( loader, axis, cut &&  kIsNC &&                kIsAntiNu,  shift, wei),
      fSpectrumNumu(   loader, axis, cut && !kIsNC && kIsNumuCC  && !kIsAntiNu,  shift, wei),
      fSpectrumNumubar(loader, axis, cut && !kIsNC && kIsNumuCC  &&  kIsAntiNu,  shift, wei),
      fSpectrumNue(    loader, axis, cut && !kIsNC && kIsBeamNue && !kIsAntiNu,  shift, wei),
      fSpectrumNuebar( loader, axis, cut && !kIsNC && kIsBeamNue &&  kIsAntiNu,  shift, wei)
  {
  }

  //----------------------------------------------------------------------
  Spectrum PredictionNoOsc::PredictComponent(osc::IOscCalc* /*calc*/,
                                             Flavors::Flavors_t flav,
                                             Current::Current_t curr,
                                             Sign::Sign_t sign) const
  {
    if(flav == Flavors::kAll &&
       curr == Current::kBoth &&
       sign == Sign::kBoth)
      return fSpectrum; // Faster

    // For when fSplitBySign is true in PredictionInterp
    if(curr & Current::kNC && sign == Sign::kNu) return fSpectrumNC;
    if(curr & Current::kNC && sign == Sign::kAntiNu) return fSpectrumNCAnti;

    if(curr & Current::kNC && sign & Sign::kBoth){
      // We don't have NC broken down by sign or flavour
      assert(flav & Flavors::kAll && sign & Sign::kBoth);
      return fSpectrumNCTot;
    }

    using namespace Flavors;
    using namespace Current;
    using namespace Sign;

    assert(curr == Current::kCC);

    Spectrum ret = fSpectrum;
    ret.Clear();

    // Safe to assume by this point that it's 100% CC
    if(flav & kNuMuToNuMu && sign & kNu)     ret += fSpectrumNumu;
    if(flav & kNuMuToNuMu && sign & kAntiNu) ret += fSpectrumNumubar;
    if(flav & kNuEToNuE   && sign & kNu)     ret += fSpectrumNue;
    if(flav & kNuEToNuE   && sign & kAntiNu) ret += fSpectrumNuebar;

    return ret;
  }

  //----------------------------------------------------------------------
  void PredictionNoOsc::SetNoOscErrors(double pot) {
    fSpectrum = SetSpectrumErrors(fSpectrum, pot);
    fSpectrumNCTot = SetSpectrumErrors(fSpectrumNC, pot);
    fSpectrumNC = SetSpectrumErrors(fSpectrumNC, pot);
    fSpectrumNCAnti = SetSpectrumErrors(fSpectrumNC, pot);  
    fSpectrumNumu = SetSpectrumErrors(fSpectrumNumu, pot);
    fSpectrumNumubar = SetSpectrumErrors(fSpectrumNumubar, pot);
    fSpectrumNue = SetSpectrumErrors(fSpectrumNue, pot);
    fSpectrumNuebar = SetSpectrumErrors(fSpectrumNuebar, pot);
  }

  //----------------------------------------------------------------------
  void PredictionNoOsc::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("PredictionNoOsc").Write("type");
    
    fSpectrum.SaveTo(dir, "spect");
    fSpectrumNCTot.SaveTo(dir, "spect_nctot");
    fSpectrumNC.SaveTo(dir, "spect_nc");
    fSpectrumNCAnti.SaveTo(dir, "spect_ncbar");
    fSpectrumNumu.SaveTo(dir, "spect_numu");
    fSpectrumNumubar.SaveTo(dir, "spect_numubar");
    fSpectrumNue.SaveTo(dir, "spect_nue");
    fSpectrumNuebar.SaveTo(dir, "spect_nuebar");

    dir->Write();
    delete dir;
 
    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<PredictionNoOsc> PredictionNoOsc::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    PredictionNoOsc* ret = new PredictionNoOsc(
      *ana::LoadFrom<Spectrum>(dir, "spect"),
      *ana::LoadFrom<Spectrum>(dir, "spect_nctot"),
      *ana::LoadFrom<Spectrum>(dir, "spect_nc"),
      *ana::LoadFrom<Spectrum>(dir, "spect_ncbar"),
      *ana::LoadFrom<Spectrum>(dir, "spect_numu"),
      *ana::LoadFrom<Spectrum>(dir, "spect_numubar"),
      *ana::LoadFrom<Spectrum>(dir, "spect_nue"),
      *ana::LoadFrom<Spectrum>(dir, "spect_nuebar"));

    delete dir;


    // Can't use make_unique because constructor is protected
    return std::unique_ptr<PredictionNoOsc>(ret);
  }

} // namespace ana
