/////////////////////////////////////////////////////////////////////////////////
// NDExtrap.cxx
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, January 2021
//
// Contains beam component information at the ND.
// Adapted from NOvA sterile oscillation framework.
/////////////////////////////////////////////////////////////////////////////////

// framework
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/SpectrumLoaderBase.h"
#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Extrap/NDExtrap.h"

// root
#include "TDirectory.h"
#include "TH1D.h"
#include "TObject.h"
#include "TObjString.h"
#include "TVectorD.h"

// stl
#include <cassert>
#include <iostream>
#include <utility>

namespace ana {

  //---------------------------------------------------------------------------
  NDExtrap NDExtrap::NDExtrap_c(Loaders& loaders,
				const HistAxis& axis,
				const Cut& selection,
				const SystShifts& shiftMC,
				const Weight& weight) {
    return NDExtrap::NDExtrap_c(loaders.GetLoader(caf::kNEARDET, Loaders::kMC, Loaders::kNonSwap),
				loaders.GetLoader(caf::kNEARDET,  Loaders::kMC, Loaders::kNueSwap),
				axis, selection, shiftMC, weight);
  }

  //---------------------------------------------------------------------------
  NDExtrap NDExtrap::NDExtrap_c(SpectrumLoaderBase& nonswap,
				SpectrumLoaderBase& swap,
				const HistAxis& axis,
				const Cut& selection,                    
				const SystShifts& shiftMC,
				const Weight& weight) {
    NDExtrap extrap(nonswap, swap, axis, selection, shiftMC, weight);
    return extrap;
  }

  // //---------------------------------------------------------------------
  // double NDExtrap::GetWeightedPOT() {
  //   fPOT = fSpecLoad->GetWeightedPOT();
  //   std::cout << "SetWeightedPOT: " << fPOT << ", specload->get..." << fSpecLoad->GetWeightedPOT() << std::endl;
  //   return fPOT;
  // }

  //----------------------------------------------------------------------
  Spectrum NDExtrap::NCComponent() { 
    return *fNCCompSpec;
  }

  //-----------------------------------------------------------------------
  NDOscillatableSpectrum NDExtrap::NueCCSurvComponent() {
    return *fEEComp;
  }

  //---------------------------------------------------------------------
  NDOscillatableSpectrum NDExtrap::AntiNueCCSurvComponent() {
    return *fAEAEComp;
  }

  //---------------------------------------------------------------------
  NDOscillatableSpectrum NDExtrap::NumuCCAppComponent() {
    return *fEMComp;
  }

  //---------------------------------------------------------------------
  NDOscillatableSpectrum NDExtrap::AntiNumuCCAppComponent() {
    return *fAEAMComp;
  }

  //---------------------------------------------------------------------
  NDOscillatableSpectrum NDExtrap::NumuCCSurvComponent() {
    return *fMMComp;
  }

  //---------------------------------------------------------------------
  NDOscillatableSpectrum NDExtrap::AntiNumuCCSurvComponent() {
    return *fAMAMComp;
  }

  //---------------------------------------------------------------------
  NDOscillatableSpectrum NDExtrap::NueCCAppComponent() {
    return *fMEComp;
  }

  //---------------------------------------------------------------------
  NDOscillatableSpectrum NDExtrap::AntiNueCCAppComponent() {
    return *fAMAEComp; 
  }

  //---------------------------------------------------------------------
  NDOscillatableSpectrum NDExtrap::NueNCComponent() {
    return *fENCComp;
  }

  //---------------------------------------------------------------------
  NDOscillatableSpectrum NDExtrap::NumuNCComponent() {
    return *fMNCComp;
  }

  //---------------------------------------------------------------------
  NDOscillatableSpectrum NDExtrap::AntiNueNCComponent() {
    return *fAENCComp;
  }

  //---------------------------------------------------------------------
  NDOscillatableSpectrum NDExtrap::AntiNumuNCComponent() {
    return *fAMNCComp;
  }

  //---------------------------------------------------------------------
  NDExtrap::NDExtrap(SpectrumLoaderBase& nonswap,
                     SpectrumLoaderBase& swap,
                     const HistAxis& axis,
                     const Cut& selection,                    
                     const SystShifts& shiftMC,
                     const Weight& weight)
    : fPOT(0),
      fEEComp  (new NDOscillatableSpectrum(nonswap, axis, selection && kIsBeamNue && !kIsAntiNu,          shiftMC, weight)),
      fAEAEComp(new NDOscillatableSpectrum(nonswap, axis, selection && kIsBeamNue && kIsAntiNu,           shiftMC, weight)),
      fEMComp  (new NDOscillatableSpectrum(swap,    axis, selection && kIsNumuApp && !kIsAntiNu,          shiftMC, weight)),
      fAEAMComp(new NDOscillatableSpectrum(swap,    axis, selection && kIsNumuApp && kIsAntiNu,           shiftMC, weight)),
      fMMComp  (new NDOscillatableSpectrum(nonswap, axis, selection && kIsNumuCC  && !kIsAntiNu,          shiftMC, weight)),
      fAMAMComp(new NDOscillatableSpectrum(nonswap, axis, selection && kIsNumuCC  && kIsAntiNu,           shiftMC, weight)), 
      fMEComp  (new NDOscillatableSpectrum(swap,    axis, selection && kIsSig     && !kIsAntiNu,          shiftMC, weight)), 
      fAMAEComp(new NDOscillatableSpectrum(swap,    axis, selection && kIsSig     && kIsAntiNu,           shiftMC, weight)), 
      fENCComp (new NDOscillatableSpectrum(nonswap, axis, selection && kIsNue     && !kIsAntiNu && kIsNC, shiftMC, weight)), 
      fMNCComp (new NDOscillatableSpectrum(nonswap, axis, selection && kIsNumu    && !kIsAntiNu && kIsNC, shiftMC, weight)), 
      fAENCComp(new NDOscillatableSpectrum(nonswap, axis, selection && kIsNue     && kIsAntiNu  && kIsNC, shiftMC, weight)),
      fAMNCComp(new NDOscillatableSpectrum(nonswap, axis, selection && kIsNumu    && kIsAntiNu  && kIsNC, shiftMC, weight)),
      fNCCompSpec(new Spectrum(nonswap, axis, selection && kIsNC, shiftMC, weight)),
      fSpecLoad(&nonswap) {
  }

  //---------------------------------------------------------------------
  void NDExtrap::SaveTo(TDirectory* dir, const std::string& name) const {

    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("NDExtrap").Write("type");
    // TVectorD weightedpot(1);
    // weightedpot[0]= fSpecLoad->GetWeightedPOT();
    // weightedpot.Write("weightedpot");
    fEEComp->SaveTo(dir, "EEComp");
    fAEAEComp->SaveTo(dir, "AEAEComp");
    fEMComp->SaveTo(dir, "EMComp");
    fAEAMComp->SaveTo(dir, "AEAMComp");
    fMMComp->SaveTo(dir, "MMComp");
    fAMAMComp->SaveTo(dir, "AMAMComp");
    fMEComp->SaveTo(dir, "MEComp");
    fAMAEComp->SaveTo(dir, "AMAEComp");
    fENCComp->SaveTo(dir, "ENCComp");
    fMNCComp->SaveTo(dir, "MNCComp");
    fAENCComp->SaveTo(dir, "AENCComp");
    fAMNCComp->SaveTo(dir, "AMNCComp");
    fNCCompSpec->SaveTo(dir, "NCComp");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<NDExtrap> NDExtrap::LoadFrom(TDirectory* dir, const std::string& name) {

    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    std::unique_ptr<NDExtrap> ret(new NDExtrap);
    //TVectorD *v = (TVectorD*)dir->Get("weightedpot");
    //ret->fPOT=(*v)(0);
    ret->fEEComp = ana::LoadFrom<NDOscillatableSpectrum>(dir, "EEComp");
    ret->fAEAEComp = ana::LoadFrom<NDOscillatableSpectrum>(dir, "AEAEComp");
    ret->fEMComp = ana::LoadFrom<NDOscillatableSpectrum>(dir, "EMComp");
    ret->fAEAMComp = ana::LoadFrom<NDOscillatableSpectrum>(dir, "AEAMComp");
    ret->fMMComp = ana::LoadFrom<NDOscillatableSpectrum>(dir, "MMComp");
    ret->fAMAMComp = ana::LoadFrom<NDOscillatableSpectrum>(dir, "AMAMComp");
    ret->fMEComp = ana::LoadFrom<NDOscillatableSpectrum>(dir, "MEComp");
    ret->fAMAEComp = ana::LoadFrom<NDOscillatableSpectrum>(dir, "AMAEComp");
    ret->fENCComp = ana::LoadFrom<NDOscillatableSpectrum>(dir, "ENCComp");
    ret->fMNCComp = ana::LoadFrom<NDOscillatableSpectrum>(dir, "MNCComp");
    ret->fAENCComp = ana::LoadFrom<NDOscillatableSpectrum>(dir, "AENCComp");
    ret->fAMNCComp = ana::LoadFrom<NDOscillatableSpectrum>(dir, "AMNCComp");
    ret->fNCCompSpec = ana::LoadFrom<Spectrum>(dir, "NCComp");

    delete dir;
    return ret;

  }

} // end of namespace ana
