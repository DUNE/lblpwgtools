/////////////////////////////////////////////////////////////////////////////////
// FDPredictionSterile.cxx
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, January 2021
//
// Provide oscillated FD predictions in a BSM oscillation framework.
// Owns and interprets an FDExtrap object.
// Adapted from NOvA sterile oscillation framework.
/////////////////////////////////////////////////////////////////////////////////

// framework
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/OscillatableSpectrum.h"
#include "CAFAna/Extrap/FDExtrap.h"
#include "CAFAna/Prediction/FDPredictionSterile.h"
#include "OscLib/OscCalcSterile.h"

// root
#include "TDirectory.h"
#include "TObjString.h"

namespace ana {

  //----------------------------------------------------------------------
  FDPredictionSterile::FDPredictionSterile(FDExtrap* extrap) : fFDExtrap(extrap) {
  }

  //----------------------------------------------------------------------
  FDPredictionSterile::~FDPredictionSterile() {
    if (fFDExtrap) delete fFDExtrap;
  }

  //----------------------------------------------------------------------
  Spectrum FDPredictionSterile::Predict(osc::IOscCalc* calc) const {

    osc::NoOscillations* noosc = dynamic_cast<osc::NoOscillations*>(calc);
    if (noosc) {
      Spectrum ret = fFDExtrap->NueCCSurvComponent()     .Oscillated(noosc, +12, +12);
      ret +=         fFDExtrap->AntiNueCCSurvComponent() .Oscillated(noosc, -12, -12);
      ret +=         fFDExtrap->NumuCCAppComponent()     .Oscillated(noosc, +12, +14);
      ret +=         fFDExtrap->AntiNumuCCAppComponent() .Oscillated(noosc, -12, -14);
      ret +=         fFDExtrap->NumuCCSurvComponent()    .Oscillated(noosc, +14, +14);
      ret +=         fFDExtrap->AntiNumuCCSurvComponent().Oscillated(noosc, -14, -14);
      ret +=         fFDExtrap->NueCCAppComponent()      .Oscillated(noosc, +14, +12);
      ret +=         fFDExtrap->AntiNueCCAppComponent()  .Oscillated(noosc, -14, -12);
      ret +=         fFDExtrap->TauCCAppComponent()      .Oscillated(noosc, +14, +16);
      ret +=         fFDExtrap->AntiTauCCAppComponent()  .Oscillated(noosc, -14, -16);
      ret +=         fFDExtrap->ETauCCAppComponent()     .Oscillated(noosc, +12, +16);
      ret +=         fFDExtrap->EAntiTauCCAppComponent() .Oscillated(noosc, -12, -16);
      ret +=         fFDExtrap->NueNCComponent()         .Oscillated(noosc, +12, 0);
      ret +=         fFDExtrap->NumuNCComponent()        .Oscillated(noosc, +14, 0);
      ret +=         fFDExtrap->TauNCComponent()         .Oscillated(noosc, +16, 0);
      ret +=         fFDExtrap->AntiNueNCComponent()     .Oscillated(noosc, -12, 0);
      ret +=         fFDExtrap->AntiNumuNCComponent()    .Oscillated(noosc, -14, 0);
      ret +=         fFDExtrap->AntiTauNCComponent()     .Oscillated(noosc, -16, 0);
      return ret;
    }

    osc::OscCalcSterile* scalc = osc::DowncastToSterile(calc);
    Spectrum ret = fFDExtrap->NueCCSurvComponent()     .Oscillated(scalc, +12, +12);
    ret +=         fFDExtrap->AntiNueCCSurvComponent() .Oscillated(scalc, -12, -12);
    ret +=         fFDExtrap->NumuCCAppComponent()     .Oscillated(scalc, +12, +14);
    ret +=         fFDExtrap->AntiNumuCCAppComponent() .Oscillated(scalc, -12, -14);
    ret +=         fFDExtrap->NumuCCSurvComponent()    .Oscillated(scalc, +14, +14);
    ret +=         fFDExtrap->AntiNumuCCSurvComponent().Oscillated(scalc, -14, -14);
    ret +=         fFDExtrap->NueCCAppComponent()      .Oscillated(scalc, +14, +12);
    ret +=         fFDExtrap->AntiNueCCAppComponent()  .Oscillated(scalc, -14, -12);
    ret +=         fFDExtrap->TauCCAppComponent()      .Oscillated(scalc, +14, +16);
    ret +=         fFDExtrap->AntiTauCCAppComponent()  .Oscillated(scalc, -14, -16);
    ret +=         fFDExtrap->ETauCCAppComponent()     .Oscillated(scalc, +12, +16);
    ret +=         fFDExtrap->EAntiTauCCAppComponent() .Oscillated(scalc, -12, -16);
    ret +=         fFDExtrap->NueNCComponent()         .Oscillated(scalc, +12, 0);
    ret +=         fFDExtrap->NumuNCComponent()        .Oscillated(scalc, +14, 0);
    ret +=         fFDExtrap->TauNCComponent()         .Oscillated(scalc, +16, 0);
    ret +=         fFDExtrap->AntiNueNCComponent()     .Oscillated(scalc, -12, 0);
    ret +=         fFDExtrap->AntiNumuNCComponent()    .Oscillated(scalc, -14, 0);
    ret +=         fFDExtrap->AntiTauNCComponent()     .Oscillated(scalc, -16, 0);
    return ret;
  }

  //----------------------------------------------------------------------
  Spectrum FDPredictionSterile::PredictComponent(osc::IOscCalc* calc,
						 Flavors::Flavors_t flav,
						 Current::Current_t curr,
						 Sign::Sign_t sign) const {

    Spectrum ret = fFDExtrap->NCComponent();
    ret.Clear();

    osc::NoOscillations* noosc = dynamic_cast<osc::NoOscillations*>(calc);
    if (noosc) {
      if (curr & Current::kCC) {
	if (flav & Flavors::kNuEToNuE    && sign & Sign::kNu)     ret += fFDExtrap->NueCCSurvComponent()     .Oscillated(noosc, +12, +12);
	if (flav & Flavors::kNuEToNuE    && sign & Sign::kAntiNu) ret += fFDExtrap->AntiNueCCSurvComponent() .Oscillated(noosc, -12, -12);

	if (flav & Flavors::kNuEToNuMu   && sign & Sign::kNu)     ret += fFDExtrap->NumuCCAppComponent()     .Oscillated(noosc, +12, +14);
	if (flav & Flavors::kNuEToNuMu   && sign & Sign::kAntiNu) ret += fFDExtrap->AntiNumuCCAppComponent() .Oscillated(noosc, -12, -14);

	if (flav & Flavors::kNuMuToNuE   && sign & Sign::kNu)     ret += fFDExtrap->NueCCAppComponent()      .Oscillated(noosc, +14, +12);
	if (flav & Flavors::kNuMuToNuE   && sign & Sign::kAntiNu) ret += fFDExtrap->AntiNueCCAppComponent()  .Oscillated(noosc, -14, -12);

	if (flav & Flavors::kNuMuToNuMu  && sign & Sign::kNu)     ret += fFDExtrap->NumuCCSurvComponent()    .Oscillated(noosc, +14, +14);
	if (flav & Flavors::kNuMuToNuMu  && sign & Sign::kAntiNu) ret += fFDExtrap->AntiNumuCCSurvComponent().Oscillated(noosc, -14, -14);

	if (flav & Flavors::kNuMuToNuTau && sign & Sign::kNu)     ret += fFDExtrap->TauCCAppComponent()      .Oscillated(noosc, +14, +16);
	if (flav & Flavors::kNuMuToNuTau && sign & Sign::kAntiNu) ret += fFDExtrap->AntiTauCCAppComponent()  .Oscillated(noosc, -14, -16);

	if (flav & Flavors::kNuEToNuTau  && sign & Sign::kNu)     ret += fFDExtrap->ETauCCAppComponent()     .Oscillated(noosc, +12, +16);
	if (flav & Flavors::kNuEToNuTau  && sign & Sign::kAntiNu) ret += fFDExtrap->EAntiTauCCAppComponent() .Oscillated(noosc, -12, -16);
      }
      if (curr & Current::kNC) {
	ret += fFDExtrap->NueNCComponent()     .Oscillated(noosc, +12, 0);
	ret += fFDExtrap->AntiNueNCComponent() .Oscillated(noosc, -12, 0);
	ret += fFDExtrap->NumuNCComponent()    .Oscillated(noosc, +14, 0);
	ret += fFDExtrap->AntiNumuNCComponent().Oscillated(noosc, -14, 0); 
	ret += fFDExtrap->TauNCComponent()     .Oscillated(noosc, +16, 0);
	ret += fFDExtrap->AntiTauNCComponent() .Oscillated(noosc, -16, 0);
      }
      return ret;
    }

    osc::OscCalcSterile* scalc = osc::DowncastToSterile(calc);
    if (scalc) {
      if (curr & Current::kCC) {
	if (flav & Flavors::kNuEToNuE    && sign & Sign::kNu)     ret += fFDExtrap->NueCCSurvComponent()     .Oscillated(scalc, +12, +12);
	if (flav & Flavors::kNuEToNuE    && sign & Sign::kAntiNu) ret += fFDExtrap->AntiNueCCSurvComponent() .Oscillated(scalc, -12, -12);

	if (flav & Flavors::kNuEToNuMu   && sign & Sign::kNu)     ret += fFDExtrap->NumuCCAppComponent()     .Oscillated(scalc, +12, +14);
	if (flav & Flavors::kNuEToNuMu   && sign & Sign::kAntiNu) ret += fFDExtrap->AntiNumuCCAppComponent() .Oscillated(scalc, -12, -14);

	if (flav & Flavors::kNuMuToNuE   && sign & Sign::kNu)     ret += fFDExtrap->NueCCAppComponent()      .Oscillated(scalc, +14, +12);
	if (flav & Flavors::kNuMuToNuE   && sign & Sign::kAntiNu) ret += fFDExtrap->AntiNueCCAppComponent()  .Oscillated(scalc, -14, -12);

	if (flav & Flavors::kNuMuToNuMu  && sign & Sign::kNu)     ret += fFDExtrap->NumuCCSurvComponent()    .Oscillated(scalc, +14, +14);
	if (flav & Flavors::kNuMuToNuMu  && sign & Sign::kAntiNu) ret += fFDExtrap->AntiNumuCCSurvComponent().Oscillated(scalc, -14, -14);

	if (flav & Flavors::kNuEToNuTau  && sign & Sign::kNu)     ret += fFDExtrap->ETauCCAppComponent()     .Oscillated(scalc, +12, +16);
	if (flav & Flavors::kNuEToNuTau  && sign & Sign::kAntiNu) ret += fFDExtrap->EAntiTauCCAppComponent() .Oscillated(scalc, -12, -16);

	if (flav & Flavors::kNuMuToNuTau && sign & Sign::kNu)     ret += fFDExtrap->TauCCAppComponent()      .Oscillated(scalc, +14, +16);
	if (flav & Flavors::kNuMuToNuTau && sign & Sign::kAntiNu) ret += fFDExtrap->AntiTauCCAppComponent()  .Oscillated(scalc, -14, -16);
      }
      if (curr & Current::kNC) { 
	if (flav & Flavors::kNuEToNuE   && sign & Sign::kNu)     ret += fFDExtrap->NueNCComponent()     .Oscillated(scalc, +12, 0);
	if (flav & Flavors::kNuEToNuE   && sign & Sign::kAntiNu) ret += fFDExtrap->AntiNueNCComponent() .Oscillated(scalc, -12, 0);
	if (flav & Flavors::kNuMuToNuMu && sign & Sign::kNu)     ret += fFDExtrap->NumuNCComponent()    .Oscillated(scalc, +14, 0);
	if (flav & Flavors::kNuMuToNuMu && sign & Sign::kAntiNu) ret += fFDExtrap->AntiNumuNCComponent().Oscillated(scalc, -14, 0);
	if (flav & Flavors::kAllNuTau   && sign & Sign::kNu)     ret += fFDExtrap->TauNCComponent()     .Oscillated(scalc, +16, 0);
	if (flav & Flavors::kAllNuTau   && sign & Sign::kAntiNu) ret += fFDExtrap->AntiTauNCComponent() .Oscillated(scalc, -16, 0);
      }
    }
    else {
      std::cerr << "NDPredExtrap: Input calculator was not of type OscCalcSterile." << std::endl;
    }

    return ret;
  }

  //----------------------------------------------------------------------
  Spectrum FDPredictionSterile::FakeData(osc::IOscCalc* calc, double POT) {
    return Predict(calc).FakeData(POT);
  }

  //----------------------------------------------------------------------
  void FDPredictionSterile::SaveTo(TDirectory* dir, const std::string& name) const {

    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("FDPredictionSterile").Write("type");
    fFDExtrap->SaveTo(dir, "fdextrap");

    dir->Write();
    delete dir;

    tmp->cd();

  }

  //----------------------------------------------------------------------
  std::unique_ptr<FDPredictionSterile> FDPredictionSterile::LoadFrom(TDirectory* dir, const std::string& name) {

    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    FDExtrap* extrap = ana::LoadFrom<FDExtrap>(dir, "fdextrap").release();

    delete dir;

    return std::unique_ptr<FDPredictionSterile>(new FDPredictionSterile(extrap));

  }

}  // end of namespace
