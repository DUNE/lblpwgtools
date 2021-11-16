/////////////////////////////////////////////////////////////////////////////////
// FDExtrap.cxx
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, January 2021
//
// Contains beam component information at the FD for BSM oscillations.
// Adapted from NOvA sterile oscillation framework.
/////////////////////////////////////////////////////////////////////////////////

// framework
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Extrap/FDExtrap.h"

// root
#include "TDirectory.h"
#include "TObjString.h"
#include "TVectorD.h"

// stl
#include <cassert>

namespace ana {

  //---------------------------------------------------------------------------
  FDExtrap FDExtrap::FDExtrap_c(Loaders& loaders,
				const HistAxis& axis,
				const Cut& selection,
				const SystShifts& shiftMC,
				const Var& weight) {
    return FDExtrap::FDExtrap_c(loaders.GetLoader(caf::kFARDET, Loaders::kMC, Loaders::kNonSwap),
				loaders.GetLoader(caf::kFARDET, Loaders::kMC, Loaders::kNueSwap),
				loaders.GetLoader(caf::kFARDET, Loaders::kMC, Loaders::kNuTauSwap),
				axis, selection, shiftMC, weight);
  }

  //---------------------------------------------------------------------------
  FDExtrap FDExtrap::FDExtrap_c(SpectrumLoaderBase& nonswap,
				SpectrumLoaderBase& swap, SpectrumLoaderBase& tau,
				const HistAxis& axis,
				const Cut& selection,                    
				const SystShifts& shiftMC,
				const Var& weight) {
    FDExtrap extrap(nonswap, swap, tau, axis, selection, shiftMC, weight);
    return extrap;
  }

  //----------------------------------------------------------------------
  Spectrum FDExtrap::NCComponent() {
    return *fNCCompSpec;
  }

  //-----------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::NueCCSurvComponent() {
    return *fEEComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::AntiNueCCSurvComponent() {
    return *fAEAEComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::NumuCCAppComponent() {
    return *fEMComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::AntiNumuCCAppComponent() {
    return *fAEAMComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::NumuCCSurvComponent() {
    return *fMMComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::AntiNumuCCSurvComponent() {
    return *fAMAMComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::NueCCAppComponent() {
    return *fMEComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::TauCCAppComponent() {
    return *fMTComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::AntiTauCCAppComponent() {
    return *fAMATComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::ETauCCAppComponent() {
    return *fETComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::EAntiTauCCAppComponent() {
    return *fAEATComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::AntiNueCCAppComponent() {
    return *fAMAEComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::NueNCComponent() {
    return *fENCComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::NumuNCComponent() {
    return *fMNCComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::TauNCComponent() {
    return *fTNCComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::AntiNueNCComponent() {
    return *fAENCComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::AntiNumuNCComponent() {
    return *fAMNCComp;
  }

  //---------------------------------------------------------------------
  OscillatableSpectrum FDExtrap::AntiTauNCComponent() {
    return *fATNCComp;
  }

  //---------------------------------------------------------------------
  FDExtrap::FDExtrap(SpectrumLoaderBase& nonswap,
                     SpectrumLoaderBase& swap,
                     SpectrumLoaderBase& tau,
                     const HistAxis& axis,
                     const Cut& selection,
                     const SystShifts& shiftMC,
                     const Var& weight)
    : /* fPOT(0),*/
    fEEComp  (new OscillatableSpectrum(nonswap, axis, selection && kIsBeamNue   && !kIsAntiNu,          shiftMC, weight)),
    fAEAEComp(new OscillatableSpectrum(nonswap, axis, selection && kIsBeamNue   && kIsAntiNu,           shiftMC, weight)),
    fEMComp  (new OscillatableSpectrum(swap,    axis, selection && kIsNumuApp   && !kIsAntiNu,          shiftMC, weight)), 
    fAEAMComp(new OscillatableSpectrum(swap,    axis, selection && kIsNumuApp   && kIsAntiNu,           shiftMC, weight)),
    fETComp  (new OscillatableSpectrum(tau,     axis, selection && kIsTauFromE  && !kIsAntiNu,          shiftMC, weight)),
    fAEATComp(new OscillatableSpectrum(tau,     axis, selection && kIsTauFromE  && kIsAntiNu,           shiftMC, weight)),
    fMEComp  (new OscillatableSpectrum(swap,    axis, selection && kIsSig       && !kIsAntiNu,          shiftMC, weight)),
    fAMAEComp(new OscillatableSpectrum(swap,    axis, selection && kIsSig       && kIsAntiNu,           shiftMC, weight)),
    fMMComp  (new OscillatableSpectrum(nonswap, axis, selection && kIsNumuCC    && !kIsAntiNu,          shiftMC, weight)),
    fAMAMComp(new OscillatableSpectrum(nonswap, axis, selection && kIsNumuCC    && kIsAntiNu,           shiftMC, weight)), 
    fMTComp  (new OscillatableSpectrum(tau,     axis, selection && kIsTauFromMu && !kIsAntiNu,          shiftMC, weight)),
    fAMATComp(new OscillatableSpectrum(tau,     axis, selection && kIsTauFromMu && kIsAntiNu,           shiftMC, weight)),
    fENCComp (new OscillatableSpectrum(nonswap, axis, selection && kIsNue       && !kIsAntiNu && kIsNC, shiftMC, weight)), 
    fMNCComp (new OscillatableSpectrum(nonswap, axis, selection && kIsNumu      && !kIsAntiNu && kIsNC, shiftMC, weight)),
    fTNCComp (new OscillatableSpectrum(nonswap, axis, selection && kIsTau       && !kIsAntiNu && kIsNC, shiftMC, weight)),
    fAENCComp(new OscillatableSpectrum(nonswap, axis, selection && kIsNue       && kIsAntiNu  && kIsNC, shiftMC, weight)),
    fAMNCComp(new OscillatableSpectrum(nonswap, axis, selection && kIsNumu      && kIsAntiNu  && kIsNC, shiftMC, weight)),
    fATNCComp(new OscillatableSpectrum(nonswap, axis, selection && kIsTau       && kIsAntiNu  && kIsNC, shiftMC, weight)),
    fNCCompSpec(new Spectrum(nonswap, axis, selection && kIsNC, shiftMC, weight)),
    fSpecLoad(&nonswap),
    fSpecLoadt(&tau) {
  }

  //---------------------------------------------------------------------
  void FDExtrap::SaveTo(TDirectory* dir, const std::string& name) const {

    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("FDExtrap").Write("type");

    // nue -> nue
    fEEComp->SaveTo(dir, "EEComp");
    fAEAEComp->SaveTo(dir, "AEAEComp");

    // nue -> numu
    fEMComp->SaveTo(dir, "EMComp");
    fAEAMComp->SaveTo(dir, "AEAMComp");

    // nue -> nutau
    fETComp->SaveTo(dir, "ETComp");
    fAEATComp->SaveTo(dir, "AEATComp");

    // numu -> nue
    fMEComp->SaveTo(dir, "MEComp");
    fAMAEComp->SaveTo(dir, "AMAEComp");

    // numu -> numu
    fMMComp->SaveTo(dir, "MMComp");
    fAMAMComp->SaveTo(dir, "AMAMComp");

    // numu -> nutau
    fMTComp->SaveTo(dir, "MTComp");
    fAMATComp->SaveTo(dir, "AMATComp");

    // nc components
    fENCComp->SaveTo(dir, "ENCComp");
    fMNCComp->SaveTo(dir, "MNCComp");
    fTNCComp->SaveTo(dir, "TNCComp");
    fATNCComp->SaveTo(dir, "ATNCComp");
    fAENCComp->SaveTo(dir, "AENCComp");
    fAMNCComp->SaveTo(dir, "AMNCComp");
    fNCCompSpec->SaveTo(dir, "NCComp"); 

    dir->Write();
    delete dir;

    tmp->cd();

  }

  //----------------------------------------------------------------------
  std::unique_ptr<FDExtrap> FDExtrap::LoadFrom(TDirectory* dir, const std::string& name) {

    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    std::unique_ptr<FDExtrap> ret(new FDExtrap);

    // nue -> nue
    ret->fEEComp = ana::LoadFrom<OscillatableSpectrum>(dir, "EEComp");
    ret->fAEAEComp = ana::LoadFrom<OscillatableSpectrum>(dir, "AEAEComp");

    // nue -> numu
    ret->fEMComp = ana::LoadFrom<OscillatableSpectrum>(dir, "EMComp");
    ret->fAEAMComp = ana::LoadFrom<OscillatableSpectrum>(dir, "AEAMComp");

    // nue -> nutau
    ret->fETComp = ana::LoadFrom<OscillatableSpectrum>(dir, "ETComp");
    ret->fAEATComp = ana::LoadFrom<OscillatableSpectrum>(dir, "AEATComp");

    // numu -> nue
    ret->fMEComp = ana::LoadFrom<OscillatableSpectrum>(dir, "MEComp");
    ret->fAMAEComp = ana::LoadFrom<OscillatableSpectrum>(dir, "AMAEComp");

    // numu -> numu
    ret->fMMComp = ana::LoadFrom<OscillatableSpectrum>(dir, "MMComp");
    ret->fAMAMComp = ana::LoadFrom<OscillatableSpectrum>(dir, "AMAMComp");

    // numu -> nutau
    ret->fMTComp = ana::LoadFrom<OscillatableSpectrum>(dir, "MTComp");
    ret->fAMATComp = ana::LoadFrom<OscillatableSpectrum>(dir, "AMATComp");

    // nc components
    ret->fENCComp = ana::LoadFrom<OscillatableSpectrum>(dir, "ENCComp");
    ret->fMNCComp = ana::LoadFrom<OscillatableSpectrum>(dir, "MNCComp");
    ret->fTNCComp = ana::LoadFrom<OscillatableSpectrum>(dir, "TNCComp");
    ret->fATNCComp = ana::LoadFrom<OscillatableSpectrum>(dir, "ATNCComp");
    ret->fAENCComp = ana::LoadFrom<OscillatableSpectrum>(dir, "AENCComp");
    ret->fAMNCComp = ana::LoadFrom<OscillatableSpectrum>(dir, "AMNCComp");
    ret->fNCCompSpec = ana::LoadFrom<Spectrum>(dir, "NCComp");

    delete dir;
    return ret;

  }

}
