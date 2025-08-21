#include "CAFAna/Extrap/ModularExtrap.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/LoadFromRegistry.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "TObjString.h"
#include "TDirectory.h"

namespace ana
{
  REGISTER_LOADFROM("ModularExtrap", IExtrap, ModularExtrap);

  //---------------------------------------------------------------------------

  void ModularExtrap::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("ModularExtrap").Write("type");

    fEEextrap->SaveTo(dir, "EEextrap");
    fEMextrap->SaveTo(dir, "EMextrap");
    fMEextrap->SaveTo(dir, "MEextrap");
    fMMextrap->SaveTo(dir, "MMextrap");
    fEEAntiextrap->SaveTo(dir, "EEAntiextrap");
    fEMAntiextrap->SaveTo(dir, "EMAntiextrap");
    fMEAntiextrap->SaveTo(dir, "MEAntiextrap");
    fMMAntiextrap->SaveTo(dir, "MMAntiextrap");
    fMTextrap->SaveTo(dir, "MTextrap");
    fETextrap->SaveTo(dir, "ETextrap");
    fMTAntiextrap->SaveTo(dir, "MTAntiextrap");
    fETAntiextrap->SaveTo(dir, "ETAntiextrap");
    //nc
    fNCTotalextrap->SaveTo(dir, "NCTotalextrap");
    fNCAntiextrap->SaveTo(dir, "NCAntiextrap");
    fNCextrap->SaveTo(dir, "NCextrap");
    //end nc

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //---------------------------------------------------------------------------

  void  ModularExtrap::SavePlotsNue( TDirectory* dir, double potFD ) const
  {
    TDirectory* tmp = gDirectory;
    dir->cd();
    fMEextrap->SavePlots( dir->mkdir("MEextrap"), potFD );
    fMEAntiextrap->SavePlots( dir->mkdir("MEAntiextrap"), potFD );
    fEEextrap->SavePlots( dir->mkdir("EEextrap"), potFD );
    fMMextrap->SavePlots( dir->mkdir("MMextrap"), potFD );
    fNCTotalextrap->SavePlots( dir->mkdir("NCTotalextrap"), potFD );
    tmp->cd();
  }

  //---------------------------------------------------------------------------

  void  ModularExtrap::SavePlotsNueRHC( TDirectory* dir, double potFD ) const
  {
    TDirectory* tmp = gDirectory;
    dir->cd();
    fMEextrap->SavePlots( dir->mkdir("MEextrap"), potFD );
    fMEAntiextrap->SavePlots( dir->mkdir("MEAntiextrap"), potFD );
    fEEextrap->SavePlots( dir->mkdir("EEextrap"), potFD );
    fEEAntiextrap->SavePlots( dir->mkdir("EEAntiextrap"), potFD );
    fMMextrap->SavePlots( dir->mkdir("MMextrap"), potFD );
    fMMAntiextrap->SavePlots( dir->mkdir("MMAntiextrap"), potFD );
    fNCextrap->SavePlots( dir->mkdir("NCextrap"), potFD );
    fNCAntiextrap->SavePlots( dir->mkdir("NCAntiextrap"), potFD );
    tmp->cd();
  }

  //---------------------------------------------------------------------------

  void  ModularExtrap::SavePlotsNumu( TDirectory* dir, double potFD ) const
  {
    TDirectory* tmp = gDirectory;
    dir->cd();
    fMMextrap->SavePlots( dir->mkdir("MMextrap"), potFD );
    fMMAntiextrap->SavePlots( dir->mkdir("MMAntiextrap"), potFD );
    tmp->cd();
  }

  //---------------------------------------------------------------------------

  std::unique_ptr<ModularExtrap> ModularExtrap::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    std::unique_ptr<ModularExtrap> ret(new ModularExtrap);

    ret->fEEextrap = ana::LoadFrom<ModularExtrapComponent>(dir, "EEextrap");
    ret->fEMextrap = ana::LoadFrom<ModularExtrapComponent>(dir, "EMextrap");
    ret->fMEextrap = ana::LoadFrom<ModularExtrapComponent>(dir, "MEextrap");
    ret->fMMextrap = ana::LoadFrom<ModularExtrapComponent>(dir, "MMextrap");
    ret->fEEAntiextrap = ana::LoadFrom<ModularExtrapComponent>(dir, "EEAntiextrap");
    ret->fEMAntiextrap = ana::LoadFrom<ModularExtrapComponent>(dir, "EMAntiextrap");
    ret->fMEAntiextrap = ana::LoadFrom<ModularExtrapComponent>(dir, "MEAntiextrap");
    ret->fMMAntiextrap = ana::LoadFrom<ModularExtrapComponent>(dir, "MMAntiextrap");
    ret->fNCTotalextrap = ana::LoadFrom<ModularExtrapComponent>(dir, "NCTotalextrap");
    ret->fNCextrap = ana::LoadFrom<ModularExtrapComponent>(dir, "NCextrap");
    ret->fNCAntiextrap = ana::LoadFrom<ModularExtrapComponent>(dir, "NCAntiextrap");
    ret->fMTextrap = ana::LoadFrom<ModularExtrapComponent>(dir, "MTextrap");
    ret->fETextrap = ana::LoadFrom<ModularExtrapComponent>(dir, "ETextrap"); 
    ret->fMTAntiextrap = ana::LoadFrom<ModularExtrapComponent>(dir, "MTAntiextrap");
    ret->fETAntiextrap = ana::LoadFrom<ModularExtrapComponent>(dir, "ETAntiextrap");
 
    delete dir;

    return ret;
  }
 
  //---------------------------------------------------------------------------

  ModularExtrap::ModularExtrap(
    SpectrumLoader& farMCswap,
    SpectrumLoader& farMCnonswap,
    SpectrumLoader& farMCtauswap,
    const HistAxis& axis,
    const Cut& fdcut,
    ana::RecoType recoIxnType,
    const SystShifts& shiftMC,
    const Weight& weight
  ) :

    // e -> e ----
    fEEextrap( new NoReweight(
      farMCnonswap, axis, fdcut, recoIxnType, shiftMC, weight, kIsBeamNue&&!kIsAntiNu)),
    fEEAntiextrap( new NoReweight(
      farMCnonswap, axis, fdcut, recoIxnType, shiftMC, weight, kIsBeamNue&&kIsAntiNu)),

    // mu -> mu  ----
    fMMextrap( new NoReweight(
      farMCnonswap, axis, fdcut, recoIxnType,shiftMC, weight, kIsNumuCC&&!kIsAntiNu)),
    fMMAntiextrap( new NoReweight(
      farMCnonswap, axis, fdcut, recoIxnType, shiftMC, weight, kIsNumuCC&&kIsAntiNu)),

    // mu -> e ----
    fMEextrap( new NoReweight(
      farMCswap, axis, fdcut, recoIxnType, shiftMC, weight, kIsNueApp&&!kIsAntiNu)),
    fMEAntiextrap( new NoReweight(
      farMCswap, axis, fdcut, recoIxnType, shiftMC, weight, kIsNueApp&&kIsAntiNu)),

    // e -> mu ----
    fEMextrap( new NoReweight(
      farMCswap, axis, fdcut, recoIxnType, shiftMC, weight, kIsNumuApp&&!kIsAntiNu)),
    fEMAntiextrap( new NoReweight(
      farMCswap, axis, fdcut, recoIxnType, shiftMC, weight, kIsNumuApp&&kIsAntiNu)),

    // NC -> NC ----
    fNCTotalextrap( new NoReweight(
      farMCnonswap, axis, fdcut, recoIxnType, shiftMC, weight, kIsNC, farMCswap, farMCtauswap)),
    fNCextrap( new NoReweight(
      farMCnonswap, axis, fdcut, recoIxnType, shiftMC, weight, kIsNC&&!kIsAntiNu, farMCswap, farMCtauswap)),
    fNCAntiextrap( new NoReweight(
      farMCnonswap, axis, fdcut, recoIxnType, shiftMC, weight, kIsNC&&kIsAntiNu, farMCswap, farMCtauswap)),

    // mu -> tau ----
    fMTextrap( new NoReweight(
      farMCtauswap, axis, fdcut, recoIxnType, shiftMC, weight, kIsTauFromMu&&!kIsAntiNu)),
    fMTAntiextrap( new NoReweight(
      farMCtauswap, axis, fdcut, recoIxnType, shiftMC, weight, kIsTauFromMu&&kIsAntiNu)),

    // e -> tau ----
    fETextrap( new NoReweight(
      farMCtauswap, axis, fdcut, recoIxnType, shiftMC, weight, kIsTauFromE&&!kIsAntiNu)),
    fETAntiextrap( new NoReweight(
      farMCtauswap, axis, fdcut, recoIxnType, shiftMC, weight, kIsTauFromE&&kIsAntiNu))

  {}

  //---------------------------------------------------------------------------

  OscillatableSpectrum ModularExtrap::NueSurvComponent()
    {return fEEextrap->Return();}

  OscillatableSpectrum ModularExtrap::AntiNueSurvComponent()
    {return fEEAntiextrap->Return();}

  OscillatableSpectrum ModularExtrap::NumuSurvComponent()
    {return fMMextrap->Return();}

  OscillatableSpectrum ModularExtrap::AntiNumuSurvComponent()
    {return fMMAntiextrap->Return();}

  OscillatableSpectrum ModularExtrap::NueAppComponent()
    {return fMEextrap->Return();}

  OscillatableSpectrum ModularExtrap::AntiNueAppComponent()
    {return fMEAntiextrap->Return();}

  OscillatableSpectrum ModularExtrap::NumuAppComponent()
    {return fEMextrap->Return();}

  OscillatableSpectrum ModularExtrap::AntiNumuAppComponent()
    {return fEMAntiextrap->Return();}

  Spectrum ModularExtrap::NCComponent()
    {return fNCextrap->Return().Unoscillated();}

  Spectrum ModularExtrap::NCAntiComponent()
    {return fNCAntiextrap->Return().Unoscillated();}

  Spectrum ModularExtrap::NCTotalComponent()
    {return NCComponent() + NCAntiComponent() ;}

  OscillatableSpectrum ModularExtrap::TauFromMuComponent()
    {return fMTextrap->Return();}

  OscillatableSpectrum ModularExtrap::AntiTauFromMuComponent()
    {return fMTAntiextrap->Return();}

  OscillatableSpectrum ModularExtrap::TauFromEComponent()
    {return fETextrap->Return();}

  OscillatableSpectrum ModularExtrap::AntiTauFromEComponent()
    {return fETAntiextrap->Return();}

}
