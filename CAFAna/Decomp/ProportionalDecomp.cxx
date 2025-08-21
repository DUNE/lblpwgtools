#include "CAFAna/Decomp/ProportionalDecomp.h"

#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Core/LoadFromRegistry.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/Ratio.h"
#include "TDirectory.h"
#include "TObjString.h"
#include <cassert>

namespace ana
{
  REGISTER_LOADFROM("ProportionalDecomp", IDecomp, ProportionalDecomp);

  //----------------------------------------------------------------------
  ProportionalDecomp::ProportionalDecomp(IInteractionSource& ndmcSrc,
                                         IInteractionSource& nddataSrc,
                                         const HistAxis& axis,
                                         const Cut& cut,
                                         const SystShifts& shiftMC,
                                         const SystShifts& shiftData,
                                         const Weight& wei)
    // TODO: implement a constructor for Spectrum that can take SystShifts and Weight.
    //       Needs to be done in cafanacore.
    : fData    (nddataSrc[cut], axis),
      // NC
      fNCTot   (ndmcSrc[cut && kIsNC                 ], axis),
      fNC      (ndmcSrc[cut && kIsNC && !kIsAntiNu   ], axis),
      fNCAnti  (ndmcSrc[cut && kIsNC && kIsAntiNu    ], axis),
      // end NC                                      
      fNue     (ndmcSrc[cut && kIsBeamNue&&!kIsAntiNu], axis),
      fAntiNue (ndmcSrc[cut && kIsBeamNue&& kIsAntiNu], axis),
      fNumu    (ndmcSrc[cut && kIsNumuCC &&!kIsAntiNu], axis),
      fAntiNumu(ndmcSrc[cut && kIsNumuCC && kIsAntiNu], axis),
      fTotal   (ndmcSrc[cut && kHasNeutrino          ], axis)
  {}

  //----------------------------------------------------------------------
  ProportionalDecomp::ProportionalDecomp(Loaders& loaders,
                                         const HistAxis& axis,
                                         const Cut& cut,
                                         ana::RecoType recoIxnType,
                                         const SystShifts& shiftMC,
                                         const SystShifts& shiftData,
                                         const Weight& wei)
    : ProportionalDecomp(loaders.GetSource(DataMC::kMC,   caf::Det_t::kNEARDET, SwappingConfig::kNonSwap).Interactions(recoIxnType),
                         loaders.GetSource(DataMC::kData, caf::Det_t::kNEARDET, SwappingConfig::kNonSwap).Interactions(recoIxnType),
                         axis,
                         cut,
                         shiftMC,
                         shiftData,
                         wei)
  {}

  //----------------------------------------------------------------------
  Spectrum ProportionalDecomp::NumuComponent() const
  {
    return (fNumu/fTotal)*fData;
  }

  //----------------------------------------------------------------------
  Spectrum ProportionalDecomp::AntiNumuComponent() const
  {
    return (fAntiNumu/fTotal)*fData;
  }

  //----------------------------------------------------------------------
  // nc
  Spectrum ProportionalDecomp::NCTotalComponent() const
  {
    return (fNCTot/fTotal)*fData;
  }

  Spectrum ProportionalDecomp::NCComponent() const
  {
    return (fNC/fTotal)*fData;
  }
  
  Spectrum ProportionalDecomp::NCAntiComponent() const
  {
    return (fNCAnti/fTotal)*fData;
  }
  //end nc
  //----------------------------------------------------------------------
  Spectrum ProportionalDecomp::NueComponent() const
  {
    return (fNue/fTotal)*fData;
  }

  //----------------------------------------------------------------------
  Spectrum ProportionalDecomp::AntiNueComponent() const
  {
    return (fAntiNue/fTotal)*fData;
  }

  //----------------------------------------------------------------------
  void ProportionalDecomp::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("ProportionalDecomp").Write("type");

    fData.SaveTo(dir, "data_comp");
    //nc
    fNCTot.SaveTo(dir, "nc_tot_comp");
    fNC.SaveTo(dir, "nc_comp");
    fNCAnti.SaveTo(dir, "nc_anti_comp");
    //end nc
    fNue.SaveTo(dir, "nue_comp");
    fAntiNue.SaveTo(dir, "antinue_comp");
    fNumu.SaveTo(dir, "numu_comp");
    fAntiNumu.SaveTo(dir, "antinumu_comp");
    fTotal.SaveTo(dir, "total_comp");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<ProportionalDecomp>
  ProportionalDecomp::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    std::unique_ptr<ProportionalDecomp> ret(new ProportionalDecomp);

    // nc 
    ret->fNCTot    = *Spectrum::LoadFrom(dir, "nc_tot_comp");
    ret->fNC       = *Spectrum::LoadFrom(dir, "nc_comp");
    ret->fNCAnti   = *Spectrum::LoadFrom(dir, "nc_anti_comp");
    //end nc
    ret->fData     = *Spectrum::LoadFrom(dir, "data_comp");
    ret->fNue      = *Spectrum::LoadFrom(dir, "nue_comp");
    ret->fAntiNue  = *Spectrum::LoadFrom(dir, "antinue_comp");
    ret->fNumu     = *Spectrum::LoadFrom(dir, "numu_comp");
    ret->fAntiNumu = *Spectrum::LoadFrom(dir, "antinumu_comp");
    ret->fTotal    = *Spectrum::LoadFrom(dir, "total_comp");

    delete dir;

    return ret;
  }

}
