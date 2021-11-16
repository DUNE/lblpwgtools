/////////////////////////////////////////////////////////////////////////////////
// FDExtrap.h
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, January 2021
//
// Contains beam component information at the FD for BSM oscillations.
// Adapted from NOvA sterile oscillation framework.
/////////////////////////////////////////////////////////////////////////////////

#pragma once

// framework
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/OscillatableSpectrum.h"
#include "CAFAna/Core/SpectrumLoaderBase.h"
#include "CAFAna/Extrap/ModularExtrap.h"
#include "CAFAna/Extrap/ModularExtrapComponent.h"

// root
class TDirectory;

// stl
#include <memory>

namespace ana {

  class FDExtrap {
  public:

    static FDExtrap FDExtrap_c(Loaders& loaders,
			       const HistAxis& axis,
			       const Cut& selection,
			       const SystShifts& shiftMC = kNoShift,
			       const Var& weight = kUnweighted);

    static FDExtrap FDExtrap_c(SpectrumLoaderBase& nonswap,
			       SpectrumLoaderBase& swap, SpectrumLoaderBase& tau,
			       const HistAxis& axis,
			       const Cut& selection,
			       const SystShifts& shiftMC = kNoShift,
			       const Var& weight = kUnweighted);

    virtual ~FDExtrap() {};

    FDExtrap(const FDExtrap&) = delete;
    FDExtrap& operator=(const FDExtrap&) = delete;
    FDExtrap(FDExtrap&&) = default;
    FDExtrap& operator=(FDExtrap&&) = default;

    OscillatableSpectrum NueCCSurvComponent();
    OscillatableSpectrum AntiNueCCSurvComponent();
    OscillatableSpectrum NumuCCAppComponent();
    OscillatableSpectrum TauCCAppComponent();
    OscillatableSpectrum ETauCCAppComponent();
    OscillatableSpectrum AntiNumuCCAppComponent();
    OscillatableSpectrum AntiTauCCAppComponent();
    OscillatableSpectrum EAntiTauCCAppComponent();
    OscillatableSpectrum NumuCCSurvComponent();
    OscillatableSpectrum AntiNumuCCSurvComponent();
    OscillatableSpectrum NueCCAppComponent();
    OscillatableSpectrum AntiNueCCAppComponent();

    OscillatableSpectrum NueNCComponent();
    OscillatableSpectrum NumuNCComponent();
    OscillatableSpectrum AntiNumuNCComponent();  
    OscillatableSpectrum AntiNueNCComponent();
    OscillatableSpectrum TauNCComponent();
    OscillatableSpectrum AntiTauNCComponent();

    Spectrum NCComponent();

    virtual void SaveTo(TDirectory* dir, const std::string& name) const;
    static std::unique_ptr<FDExtrap> LoadFrom(TDirectory* dir, const std::string& name);

  protected:

    FDExtrap() {};
    FDExtrap(SpectrumLoaderBase& nonswap,
	     SpectrumLoaderBase& swap,SpectrumLoaderBase& tau,
	     const HistAxis& axis,
	     const Cut& selection,
	     const SystShifts& shiftMC = kNoShift,
	     const Var& weight = kUnweighted);

    std::unique_ptr<OscillatableSpectrum> LoadSpectrum(TDirectory* dir, std::string name);

    std::unique_ptr<OscillatableSpectrum> fEEComp;
    std::unique_ptr<OscillatableSpectrum> fAEAEComp;
    std::unique_ptr<OscillatableSpectrum> fEMComp;
    std::unique_ptr<OscillatableSpectrum> fAEAMComp;
    std::unique_ptr<OscillatableSpectrum> fETComp;
    std::unique_ptr<OscillatableSpectrum> fAEATComp;
    std::unique_ptr<OscillatableSpectrum> fMEComp;
    std::unique_ptr<OscillatableSpectrum> fAMAEComp;
    std::unique_ptr<OscillatableSpectrum> fMMComp;
    std::unique_ptr<OscillatableSpectrum> fAMAMComp;
    std::unique_ptr<OscillatableSpectrum> fMTComp;
    std::unique_ptr<OscillatableSpectrum> fAMATComp;

    std::unique_ptr<OscillatableSpectrum> fENCComp;
    std::unique_ptr<OscillatableSpectrum> fMNCComp;
    std::unique_ptr<OscillatableSpectrum> fTNCComp;
    std::unique_ptr<OscillatableSpectrum> fAENCComp;
    std::unique_ptr<OscillatableSpectrum> fAMNCComp;
    std::unique_ptr<OscillatableSpectrum> fATNCComp;
    std::unique_ptr<Spectrum> fNCCompSpec;

    SpectrumLoaderBase* fSpecLoad;
    SpectrumLoaderBase* fSpecLoadt;

  }; /// end of FDExtrap

} /// end of namespace
