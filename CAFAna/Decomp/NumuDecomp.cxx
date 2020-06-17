#include "CAFAna/Decomp/NumuDecomp.h"

#include "CAFAna/Cuts/TruthCuts.h"
#include "CAFAna/Core/Ratio.h"

#include <cassert>

#include "TDirectory.h"
#include "TObjString.h"

namespace ana
{
  //----------------------------------------------------------------------
  NumuDecomp::NumuDecomp(SpectrumLoaderBase& loaderMC,
                         SpectrumLoaderBase& loaderData,
                         const std::string& label,
                         const Binning& bins,
                         const Var& var,
                         const Cut& cut,
                         const SystShifts& shiftMC,
                         const SystShifts& shiftData,
                         const Var& wei)
    : NumuDecomp(loaderMC, loaderData, HistAxis(label, bins, var),
                 cut, shiftMC, shiftData, wei)
  {
  }

  //----------------------------------------------------------------------
  NumuDecomp::NumuDecomp(SpectrumLoaderBase& loaderMC,
                         SpectrumLoaderBase& loaderData,
                         const HistAxis& axis,
                         const Cut& cut,
                         const SystShifts& shiftMC,
                         const SystShifts& shiftData,
                         const Var& wei)
    : fData    (loaderData, axis, cut,                         shiftData, wei),
      fNC      (loaderMC,   axis, cut && kIsNC,                  shiftMC, wei),
      fNue     (loaderMC,   axis, cut && kIsBeamNue&&!kIsAntiNu, shiftMC, wei),
      fAntiNue (loaderMC,   axis, cut && kIsBeamNue&& kIsAntiNu, shiftMC, wei),
      fNumu    (loaderMC,   axis, cut && kIsNumuCC &&!kIsAntiNu, shiftMC, wei),
      fAntiNumu(loaderMC,   axis, cut && kIsNumuCC && kIsAntiNu, shiftMC, wei),
      fNotNumu (loaderMC,   axis, cut && !kIsNumuCC,             shiftMC, wei)
  {
  }

  //----------------------------------------------------------------------
  NumuDecomp::NumuDecomp(Loaders& loaders,
                         const HistAxis& axis,
                         const Cut& cut,
                         const SystShifts& shiftMC,
                         const SystShifts& shiftData,
                         const Var& wei)
    : NumuDecomp(loaders.GetLoader(caf::kNEARDET, Loaders::kMC),
                 loaders.GetLoader(caf::kNEARDET, Loaders::kData),
                 axis, cut, shiftMC, shiftData, wei)
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

    fNC.SaveTo(dir, "nc_comp");
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
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    std::unique_ptr<NumuDecomp> ret(new NumuDecomp);

    ret->fNC       = *Spectrum::LoadFrom(dir, "nc_comp");
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
