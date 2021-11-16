/////////////////////////////////////////////////////////////////////////////////
// NDPredictionSterile.cxx
// Mike Wallbank (University of Cincinnati) <wallbank@fnal.gov>, January 2021
//
// Provide oscillated ND predictions.  Owns and interprets an NDExtrap object.
// Adapted from NOvA sterile oscillation framework.
/////////////////////////////////////////////////////////////////////////////////

// framework
#include "CAFAna/Prediction/NDPredictionSterile.h"
#include "CAFAna/Extrap/NDExtrap.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "OscLib/IOscCalc.h"
#include "OscLib/OscCalcSterile.h"

// root
#include "TDirectory.h"
#include "TObjString.h"

namespace ana {

  //----------------------------------------------------------------------
  NDPredictionSterile::NDPredictionSterile(NDExtrap* extrap) : fNDExtrap(extrap) {
  }

  //----------------------------------------------------------------------
  NDPredictionSterile::~NDPredictionSterile() {
    if (fNDExtrap)
      delete fNDExtrap;
  }

  //----------------------------------------------------------------------
  Spectrum NDPredictionSterile::Predict(osc::IOscCalc* calc) const {

    osc::NoOscillations* noosc = dynamic_cast<osc::NoOscillations*>(calc);

    if (noosc) {

      Spectrum ret = fNDExtrap->NueCCSurvComponent().Oscillated(noosc, +12, +12);
      ret += fNDExtrap->AntiNueCCSurvComponent()    .Oscillated(noosc, -12, -12);

      Spectrum temp = ((NDOscillatableSpectrum)fNDExtrap->NumuCCAppComponent()).Oscillated(noosc, +12, +14);
      if (temp.POT() > 0) ret += temp;

      temp = ((NDOscillatableSpectrum)fNDExtrap->AntiNumuCCAppComponent()).Oscillated(noosc, -12, -14);
      if (temp.POT() > 0) ret += temp;

      ret += fNDExtrap->NumuCCSurvComponent()       .Oscillated(noosc, +14, +14);
      ret += fNDExtrap->AntiNumuCCSurvComponent()   .Oscillated(noosc, -14, -14);

      temp = ((NDOscillatableSpectrum)fNDExtrap->NueCCAppComponent()).Oscillated(noosc, +14, +12);
      if (temp.POT() > 0) ret += temp;

      temp = ((NDOscillatableSpectrum)fNDExtrap->AntiNueCCAppComponent()).Oscillated(noosc, -14, -12);
      if (temp.POT() > 0) ret += temp;

      ret += fNDExtrap->NueNCComponent()     .Oscillated(noosc, +12, 0);
      ret += fNDExtrap->NumuNCComponent()    .Oscillated(noosc, +14, 0);
      ret += fNDExtrap->AntiNueNCComponent() .Oscillated(noosc, -12, 0);
      ret += fNDExtrap->AntiNumuNCComponent().Oscillated(noosc, -14, 0);
      return ret;
    }

    osc::OscCalcSterile* scalc = osc::DowncastToSterile(calc);

    Spectrum ret = fNDExtrap->NueCCSurvComponent().Oscillated(scalc, +12, +12);
    ret += fNDExtrap->AntiNueCCSurvComponent()    .Oscillated(scalc, -12, -12);

    Spectrum temp = ((NDOscillatableSpectrum)fNDExtrap->NumuCCAppComponent()).Oscillated(scalc, +12, +14);
    if (temp.POT() > 0) ret += temp;

    temp = ((NDOscillatableSpectrum)fNDExtrap->AntiNumuCCAppComponent()).Oscillated(scalc, -12, -14);
    if (temp.POT() > 0) ret += temp;

    ret += fNDExtrap->NumuCCSurvComponent()       .Oscillated(scalc, +14, +14);
    ret += fNDExtrap->AntiNumuCCSurvComponent()   .Oscillated(scalc, -14, -14);

    temp = ((NDOscillatableSpectrum)fNDExtrap->NueCCAppComponent()).Oscillated(scalc, +14, +12);
    if (temp.POT() > 0) ret += temp;

    temp = ((NDOscillatableSpectrum)fNDExtrap->AntiNueCCAppComponent()).Oscillated(scalc, -14, -12);
    if (temp.POT() > 0) ret += temp;

    ret += fNDExtrap->NueNCComponent()     .Oscillated(scalc, +12, 0);
    ret += fNDExtrap->NumuNCComponent()    .Oscillated(scalc, +14, 0);
    ret += fNDExtrap->AntiNueNCComponent() .Oscillated(scalc, -12, 0);
    ret += fNDExtrap->AntiNumuNCComponent().Oscillated(scalc, -14, 0);

    return ret;    
  }

  //----------------------------------------------------------------------
  Spectrum NDPredictionSterile::PredictComponent(osc::IOscCalc* calc,
						 Flavors::Flavors_t flav,
						 Current::Current_t curr,
						 Sign::Sign_t sign) const {

    Spectrum ret = fNDExtrap->NCComponent();
    ret.Clear();

    osc::NoOscillations* noosc = dynamic_cast<osc::NoOscillations*>(calc);
    if (noosc) {
      if (curr & Current::kCC) {
	if (flav & Flavors::kNuEToNuE    && sign & Sign::kNu)     ret += fNDExtrap->NueCCSurvComponent()     .Oscillated(noosc, +12, +12);
	if (flav & Flavors::kNuEToNuE    && sign & Sign::kAntiNu) ret += fNDExtrap->AntiNueCCSurvComponent() .Oscillated(noosc, -12, -12);
	if (flav & Flavors::kNuEToNuMu   && sign & Sign::kNu) {
	  Spectrum temp = ((NDOscillatableSpectrum)fNDExtrap->NumuCCAppComponent()).Oscillated(noosc, +12, +14);
	  if (temp.POT() > 0) ret += temp;
	}
	if (flav & Flavors::kNuEToNuMu   && sign & Sign::kAntiNu) {
	  Spectrum temp = ((NDOscillatableSpectrum)fNDExtrap->AntiNumuCCAppComponent()).Oscillated(noosc, -12, -14);
	  if (temp.POT() > 0) ret += temp;
	}
	if (flav & Flavors::kNuMuToNuE   && sign & Sign::kNu) {
	  Spectrum temp = ((NDOscillatableSpectrum)fNDExtrap->NueCCAppComponent()).Oscillated(noosc, +14, +12);
	  if (temp.POT() > 0) ret += temp;
	}
	if (flav & Flavors::kNuMuToNuE   && sign & Sign::kAntiNu) {
	  Spectrum temp = ((NDOscillatableSpectrum)fNDExtrap->AntiNueCCAppComponent()).Oscillated(noosc, -14, -12);
	  if (temp.POT() > 0) ret += temp;
	}
	if (flav & Flavors::kNuMuToNuMu  && sign & Sign::kNu)     ret += fNDExtrap->NumuCCSurvComponent()    .Oscillated(noosc, +14, +14);
	if (flav & Flavors::kNuMuToNuMu  && sign & Sign::kAntiNu) ret += fNDExtrap->AntiNumuCCSurvComponent().Oscillated(noosc, -14, -14);
      }
      if (curr & Current::kNC) {
	ret += fNDExtrap->NueNCComponent()     .Oscillated(noosc, +12, 0);
	ret += fNDExtrap->AntiNueNCComponent() .Oscillated(noosc, -12, 0);
	ret += fNDExtrap->NumuNCComponent()    .Oscillated(noosc, +14, 0);
	ret += fNDExtrap->AntiNumuNCComponent().Oscillated(noosc, -14, 0); 
      }
      return ret;
    }

    osc::OscCalcSterile* scalc = osc::DowncastToSterile(calc);
    if (scalc) {
      if (curr & Current::kCC){
	if (flav & Flavors::kNuEToNuE   && sign & Sign::kNu)     ret += fNDExtrap->NueCCSurvComponent()     .Oscillated(scalc, +12, +12);
	if (flav & Flavors::kNuEToNuE   && sign & Sign::kAntiNu) ret += fNDExtrap->AntiNueCCSurvComponent() .Oscillated(scalc, -12, -12);
	if (flav & Flavors::kNuEToNuMu  && sign & Sign::kNu) {
	  Spectrum temp = ((NDOscillatableSpectrum)fNDExtrap->NumuCCAppComponent()).Oscillated(scalc, +12, +14);
	  if (temp.POT() > 0) ret += temp;
	}
	if (flav & Flavors::kNuEToNuMu  && sign & Sign::kAntiNu) {
	  Spectrum temp = ((NDOscillatableSpectrum)fNDExtrap->AntiNumuCCAppComponent()).Oscillated(scalc, -12, -14);
	  if (temp.POT() > 0) ret += temp;
	}
	if (flav & Flavors::kNuMuToNuE  && sign & Sign::kNu) {
	  Spectrum temp = ((NDOscillatableSpectrum)fNDExtrap->NueCCAppComponent()).Oscillated(scalc, +14, +12);
	  if (temp.POT() > 0) ret += temp;
	}
	if (flav & Flavors::kNuMuToNuE  && sign & Sign::kAntiNu) {
	  Spectrum temp = ((NDOscillatableSpectrum)fNDExtrap->AntiNueCCAppComponent()).Oscillated(scalc, -14, -12);
	  if (temp.POT() > 0) ret += temp;
	}
	if (flav & Flavors::kNuMuToNuMu && sign & Sign::kNu) ret += fNDExtrap->NumuCCSurvComponent()     .Oscillated(scalc, +14, +14);
	if (flav & Flavors::kNuMuToNuMu && sign & Sign::kAntiNu) ret += fNDExtrap->AntiNumuCCSurvComponent()   .Oscillated(scalc, -14, -14);
      }
      if (curr & Current::kNC) { 
	if (flav & Flavors::kNuEToNuE   && sign & Sign::kNu)     ret += fNDExtrap->NueNCComponent()     .Oscillated(scalc, +12, 0);
	if (flav & Flavors::kNuEToNuE   && sign & Sign::kAntiNu) ret += fNDExtrap->AntiNueNCComponent() .Oscillated(scalc, -12, 0);
	if (flav & Flavors::kNuMuToNuMu && sign & Sign::kNu)     ret += fNDExtrap->NumuNCComponent()    .Oscillated(scalc, +14, 0);
	if (flav & Flavors::kNuMuToNuMu && sign & Sign::kAntiNu) ret += fNDExtrap->AntiNumuNCComponent().Oscillated(scalc, -14, 0);
      }
    }

    else
      std::cerr << "NDPredExtrap: Input calculator was not of type OscCalcSterile." << std::endl;      

    return ret;
  }

  //----------------------------------------------------------------------
  Spectrum NDPredictionSterile::FakeData(osc::IOscCalc* calc, double POT) {
    return Predict(calc).FakeData(POT);
  }

  //----------------------------------------------------------------------
  void NDPredictionSterile::SaveTo(TDirectory* dir, const std::string& name) const {

    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();
    TObjString("NDPredictionSterile").Write("type");
    fNDExtrap->SaveTo(dir, "ndextrap");
    dir->Write();

    delete dir;
    tmp->cd();

  }

  //----------------------------------------------------------------------
  std::unique_ptr<NDPredictionSterile> NDPredictionSterile::LoadFrom(TDirectory* dir, const std::string& name) {

    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    NDExtrap* extrap = ana::LoadFrom<NDExtrap>(dir, "ndextrap").release();

    delete dir;
    return std::unique_ptr<NDPredictionSterile>(new NDPredictionSterile(extrap));

  }

}  // end of namespace
