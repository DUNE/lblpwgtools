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

    dir = dir->mkdir(name.c_str());
    dir->cd();

    TObjString("ModularExtrap").Write("type");

    fNueSurv->SaveTo(dir, "nue_surv");
    fNueSurvAnti->SaveTo(dir, "nue_surv_anti");

    fNumuSurv->SaveTo(dir, "numu_surv");
    fNumuSurvAnti->SaveTo(dir, "nue_surv_anti");

    fNueApp->SaveTo(dir, "nue_app");
    fNueAppAnti->SaveTo(dir, "nue_app_anti");

    fNumuApp->SaveTo(dir, "numu_app");
    fNumuAppAnti->SaveTo(dir, "numu_app_anti");

    fTauFromMu->SaveTo(dir, "nutau_from_numu");
    fTauFromMuAnti->SaveTo(dir, "nutau_from_numu_anti");

    fTauFromE->SaveTo(dir, "nutau_from_nue");
    fTauFromEAnti->SaveTo(dir, "nutau_from_nue_anti");

    fNCTot->SaveTo(dir, "nc_tot");
    fNC->SaveTo(dir, "nc");
    fNCAnti->SaveTo(dir, "nc_anti");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //---------------------------------------------------------------------------
  std::unique_ptr<ModularExtrap> ModularExtrap::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str());
    assert(dir);

    std::unique_ptr<ModularExtrap> ret(new ModularExtrap);

    ret->fNueSurv = ana::LoadFrom<ModularExtrapComponent>(dir, "nue_surv");
    ret->fNueSurvAnti = ana::LoadFrom<ModularExtrapComponent>(dir, "nue_surv_anti");

    ret->fNumuSurv = ana::LoadFrom<ModularExtrapComponent>(dir, "numu_surv");
    ret->fNumuSurvAnti = ana::LoadFrom<ModularExtrapComponent>(dir, "nue_surv_anti");

    ret->fNueApp = ana::LoadFrom<ModularExtrapComponent>(dir, "nue_app");
    ret->fNueAppAnti = ana::LoadFrom<ModularExtrapComponent>(dir, "nue_app_anti");

    ret->fNumuApp = ana::LoadFrom<ModularExtrapComponent>(dir, "numu_app");
    ret->fNumuAppAnti = ana::LoadFrom<ModularExtrapComponent>(dir, "numu_app_anti");

    ret->fTauFromMu = ana::LoadFrom<ModularExtrapComponent>(dir, "nutau_from_numu");
    ret->fTauFromMuAnti = ana::LoadFrom<ModularExtrapComponent>(dir, "nutau_from_numu_anti");

    ret->fTauFromE = ana::LoadFrom<ModularExtrapComponent>(dir, "nutau_from_nue");
    ret->fTauFromEAnti = ana::LoadFrom<ModularExtrapComponent>(dir, "nutau_from_nue_anti");

    ret->fNCTot = ana::LoadFrom<ModularExtrapComponent>(dir, "nc_tot");
    ret->fNC = ana::LoadFrom<ModularExtrapComponent>(dir, "nc");
    ret->fNCAnti = ana::LoadFrom<ModularExtrapComponent>(dir, "nc_anti");

    delete dir;

    return ret;
  }

  //---------------------------------------------------------------------------
  std::vector<ModularExtrapComponent*> ModularExtrap::GetModularExtrapComponents() const {
    return {fNueSurv.get(),
            fNueSurvAnti.get(),
            
            fNumuSurv.get(),
            fNumuSurvAnti.get(),
            
            fNueApp.get(),
            fNueAppAnti.get(),
            
            fNumuApp.get(),
            fNumuAppAnti.get(),
            
            fTauFromMu.get(),
            fTauFromMuAnti.get(),
            
            fTauFromE.get(),
            fTauFromEAnti.get(),
            
            fNCTot.get(),
            fNC.get(),
            fNCAnti.get()};
  }
 
  //---------------------------------------------------------------------------
  ModularExtrap::ModularExtrap(IInteractionSource &farDetMCnonswapSrc,
                               IInteractionSource &farDetMCfluxswapSrc,
                               IInteractionSource &farDetMCtauswapSrc,
                               const HistAxis &axis,
                               const Cut &fdcut) :
    // e -> e ----
    fNueSurv(new NoReweight(farDetMCnonswapSrc, axis, fdcut, kNoShift, kUnweighted, kIsBeamNue&&!kIsAntiNu)),
    fNueSurvAnti(new NoReweight(farDetMCnonswapSrc, axis, fdcut, kNoShift, kUnweighted, kIsBeamNue&&kIsAntiNu)),

    // mu -> mu  ----
    fNumuSurv(new NoReweight(farDetMCnonswapSrc, axis, fdcut, kNoShift, kUnweighted, kIsNumuCC&&!kIsAntiNu)),
    fNumuSurvAnti(new NoReweight(farDetMCnonswapSrc, axis, fdcut, kNoShift, kUnweighted, kIsNumuCC&&kIsAntiNu)),

    // mu -> e ----
    fNueApp(new NoReweight(farDetMCfluxswapSrc, axis, fdcut, kNoShift, kUnweighted, kIsNueApp&&!kIsAntiNu)),
    fNueAppAnti(new NoReweight(farDetMCfluxswapSrc, axis, fdcut, kNoShift, kUnweighted, kIsNueApp&&kIsAntiNu)),

    // e -> mu ----
    fNumuApp(new NoReweight(farDetMCfluxswapSrc, axis, fdcut, kNoShift, kUnweighted, kIsNumuApp&&!kIsAntiNu)),
    fNumuAppAnti(new NoReweight(farDetMCfluxswapSrc, axis, fdcut, kNoShift, kUnweighted, kIsNumuApp&&kIsAntiNu)),

    // mu -> tau ----
    fTauFromMu(new NoReweight(farDetMCtauswapSrc, axis, fdcut, kNoShift, kUnweighted, kIsTauFromMu&&!kIsAntiNu)),
    fTauFromMuAnti(new NoReweight(farDetMCtauswapSrc, axis, fdcut, kNoShift, kUnweighted, kIsTauFromMu&&kIsAntiNu)),

    // e -> tau ----
    fTauFromE(new NoReweight(farDetMCtauswapSrc, axis, fdcut, kNoShift, kUnweighted, kIsTauFromE&&!kIsAntiNu)),
    fTauFromEAnti(new NoReweight(farDetMCtauswapSrc, axis, fdcut, kNoShift, kUnweighted, kIsTauFromE&&kIsAntiNu)),

    // NC -> NC ----
    fNCTot(new NoReweight(farDetMCnonswapSrc, axis, fdcut, kNoShift, kUnweighted, kIsNC, farDetMCfluxswapSrc, farDetMCtauswapSrc)),
    fNC(new NoReweight(farDetMCnonswapSrc, axis, fdcut, kNoShift, kUnweighted, kIsNC&&!kIsAntiNu, farDetMCfluxswapSrc, farDetMCtauswapSrc)),
    fNCAnti(new NoReweight(farDetMCnonswapSrc, axis, fdcut, kNoShift, kUnweighted, kIsNC&&kIsAntiNu, farDetMCfluxswapSrc, farDetMCtauswapSrc))
  {}
}
