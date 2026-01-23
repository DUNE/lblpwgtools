#include "CAFAna/Core/LoadFromRegistry.h"
#include "CAFAna/Core/Ratio.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "lbl2025/NumuDecomp.h"

#include <cassert>
#include <iostream>

#include "TDirectory.h"
#include "TObjString.h"

namespace ana
{
  REGISTER_LOADFROM("NumuDecomp", IDecomp, NumuDecomp);

  //----------------------------------------------------------------------
  // TODO: Add shifts and weights.
  NumuDecomp::NumuDecomp(IInteractionSource& nearDetMCsrc,
                         IInteractionSource& nearDetDatasrc,
                         const HistAxis& axis,
                         const Cut& cut,
                         const SystShifts& shiftMC,
                         const SystShifts& shiftData,
                         const Weight& weight)
    /*
    : fData    (nearDetDatasrc[cut], axis),
      fNue     (nearDetMCsrc[cut &&  kIsBeamNue && !kIsAntiNu], axis),
      fAntiNue (nearDetMCsrc[cut &&  kIsBeamNue &&  kIsAntiNu], axis),
      fNumu    (nearDetMCsrc[cut &&  kIsNumuCC  && !kIsAntiNu], axis),
      fAntiNumu(nearDetMCsrc[cut &&  kIsNumuCC  &&  kIsAntiNu], axis),
      fNCTot   (nearDetMCsrc[cut && kIsNC],                     axis),
      fNotNumu (nearDetMCsrc[cut && !kIsNumuCC  &&  kHasNeutrino], axis)
      */
    /*
    : fData    (nearDetDatasrc[kHasNeutrino], axis),
      fNue     (nearDetMCsrc[kHasNeutrino], axis),
      fAntiNue (nearDetMCsrc[kHasNeutrino], axis),
      fNumu    (nearDetMCsrc[kHasNeutrino], axis),
      fAntiNumu(nearDetMCsrc[kHasNeutrino], axis),
      fNCTot   (nearDetMCsrc[kHasNeutrino], axis),
      fNotNumu (nearDetMCsrc[kHasNeutrino], axis)
      */
    : fData    (nearDetDatasrc[kNoCut], axis),
      fNue     (nearDetMCsrc[kNoCut], axis),
      fAntiNue (nearDetMCsrc[kNoCut], axis),
      fNumu    (nearDetMCsrc[kNoCut], axis),
      fAntiNumu(nearDetMCsrc[kNoCut], axis),
      fNCTot   (nearDetMCsrc[kNoCut], axis),
      fNotNumu (nearDetMCsrc[kNoCut], axis)
  {
    std::cout << "Getting past the setup of the spectra" << std::endl;
  }

  //----------------------------------------------------------------------
  NumuDecomp::NumuDecomp(Loaders& loaders,
                         const HistAxis& axis,
                         const Cut& cut,
                         const ana::RecoType& ixnRecoType,
                         const SystShifts& shiftMC,
                         const SystShifts& shiftData,
                         const Weight& weight)
    : NumuDecomp(loaders.GetSource(DataMC::kMC, caf::Det_t::kNEARDET, SwappingConfig::kNonSwap).Interactions(ixnRecoType),
                 // TODO: FUDGE!!!
                 loaders.GetSource(DataMC::kMC, caf::Det_t::kNEARDET, SwappingConfig::kNonSwap).Interactions(ixnRecoType),
                 //loaders.GetSource(DataMC::kData, caf::Det_t::kNEARDET, SwappingConfig::kNonSwap).Interactions(ixnRecoType),
                 axis, cut, shiftMC, shiftData, weight)
  {
  }

  //----------------------------------------------------------------------
  Spectrum NumuDecomp::NumuComponent() const
  {
    // Subtract backgrounds from data using MC, then split into nu/antinu
    // components using MC ratio
    return (fNumu/(fNumu+fAntiNumu))*(fData-fNotNumu);
  }

  //----------------------------------------------------------------------
  Spectrum NumuDecomp::AntiNumuComponent() const
  {
    return (fAntiNumu/(fNumu+fAntiNumu))*(fData-fNotNumu);
  }

  //----------------------------------------------------------------------
  void NumuDecomp::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("NumuDecomp").Write("type");

    fNCTot.SaveTo(dir, "nc_tot_comp");
    fData.SaveTo(dir, "data_comp");
    fNue.SaveTo(dir, "nue_comp");
    fAntiNue.SaveTo(dir, "antinue_comp");
    fNumu.SaveTo(dir, "numu_comp");
    fAntiNumu.SaveTo(dir, "antinumu_comp");
    fNotNumu.SaveTo(dir, "notnumu_comp");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<NumuDecomp> NumuDecomp::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str());
    assert(dir);

    std::unique_ptr<NumuDecomp> ret(new NumuDecomp);

    ret->fNCTot    = *Spectrum::LoadFrom(dir, "nc_tot_comp");
    ret->fData     = *Spectrum::LoadFrom(dir, "data_comp");
    ret->fNue      = *Spectrum::LoadFrom(dir, "nue_comp");
    ret->fAntiNue  = *Spectrum::LoadFrom(dir, "antinue_comp");
    ret->fNumu     = *Spectrum::LoadFrom(dir, "numu_comp");
    ret->fAntiNumu = *Spectrum::LoadFrom(dir, "antinumu_comp");
    ret->fNotNumu  = *Spectrum::LoadFrom(dir, "notnumu_comp");

    delete dir;

    return ret;
  }
}
