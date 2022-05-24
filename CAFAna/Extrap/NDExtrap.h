/////////////////////////////////////////////////////////////////////////////////
// NDExtrap.h
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, January 2021
//
// Contains beam component information at the ND.
// Adapted from NOvA sterile oscillation framework.
/////////////////////////////////////////////////////////////////////////////////

#pragma once

// framework
#include "CAFAna/Core/OscillatableSpectrum.h"
#include "CAFAna/Core/NDOscillatableSpectrum.h"
#include "CAFAna/Core/SpectrumLoaderBase.h"
#include "CAFAna/Core/Ratio.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Decomp/IDecomp.h"
#include "CAFAna/Extrap/ModularExtrap.h"
#include "CAFAna/Extrap/ModularExtrapComponent.h"

// stl
#include <memory>

class TDirectory;

namespace ana {

  class NDExtrap {
  public:

    static NDExtrap NDExtrap_c(Loaders& loaders,
			       const HistAxis& axis,
			       const Cut& selection,
			       const SystShifts& shiftMC = kNoShift,
			       const Weight& weight = kUnweighted);

    static NDExtrap NDExtrap_c(SpectrumLoaderBase& nonswap,
			       SpectrumLoaderBase& swap,
			       const HistAxis& axis,
			       const Cut& selection,
			       const SystShifts& shiftMC = kNoShift,
			       const Weight& weight = kUnweighted);

    virtual ~NDExtrap(){};

    NDExtrap(const NDExtrap&) = delete;
    NDExtrap& operator=(const NDExtrap&) = delete;
    NDExtrap(NDExtrap&&) = default;
    NDExtrap& operator=(NDExtrap&&) = default;

    NDOscillatableSpectrum NueCCSurvComponent();
    NDOscillatableSpectrum AntiNueCCSurvComponent();
    NDOscillatableSpectrum NumuCCAppComponent();
    NDOscillatableSpectrum AntiNumuCCAppComponent();
    NDOscillatableSpectrum NumuCCSurvComponent();
    NDOscillatableSpectrum AntiNumuCCSurvComponent();
    NDOscillatableSpectrum NueCCAppComponent();
    NDOscillatableSpectrum AntiNueCCAppComponent();

    NDOscillatableSpectrum NueNCComponent();
    NDOscillatableSpectrum NumuNCComponent();
    NDOscillatableSpectrum AntiNumuNCComponent();  
    NDOscillatableSpectrum AntiNueNCComponent();

    Spectrum NCComponent();

    // double GetWeightedPOT();

    virtual void SaveTo(TDirectory* dir, const std::string& name) const;
    static std::unique_ptr<NDExtrap> LoadFrom(TDirectory* dir, const std::string& name);

    double fPOT;

  protected:

    NDExtrap() {};

    NDExtrap(SpectrumLoaderBase& nonswap,
	     SpectrumLoaderBase& swap,
	     const HistAxis& axis,
	     const Cut& selection,
	     const SystShifts& shiftMC = kNoShift,
	     const Weight& weight = kUnweighted);

    std::unique_ptr<NDOscillatableSpectrum> fEEComp;
    std::unique_ptr<NDOscillatableSpectrum> fAEAEComp;
    std::unique_ptr<NDOscillatableSpectrum> fEMComp;
    std::unique_ptr<NDOscillatableSpectrum> fAEAMComp;
    std::unique_ptr<NDOscillatableSpectrum> fMMComp;
    std::unique_ptr<NDOscillatableSpectrum> fAMAMComp;
    std::unique_ptr<NDOscillatableSpectrum> fMEComp;
    std::unique_ptr<NDOscillatableSpectrum> fAMAEComp;

    std::unique_ptr<NDOscillatableSpectrum> fENCComp;
    std::unique_ptr<NDOscillatableSpectrum> fMNCComp;
    std::unique_ptr<NDOscillatableSpectrum> fAENCComp;
    std::unique_ptr<NDOscillatableSpectrum> fAMNCComp;
    std::unique_ptr<Spectrum> fNCCompSpec;

    SpectrumLoaderBase* fSpecLoad;

  }; /// end of NDExtrap

} /// end of namespace
