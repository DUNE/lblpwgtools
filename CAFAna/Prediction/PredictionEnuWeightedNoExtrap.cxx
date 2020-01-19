#include "CAFAna/Prediction/PredictionEnuWeightedNoExtrap.h"

#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/OscCurve.h"

#include "CAFAna/Core/Loaders.h"

#include "CAFAna/Extrap/IExtrap.h"
#include "CAFAna/Extrap/TrivialExtrap.h"

#include "TDirectory.h"
#include "TObjString.h"

#include "TH1.h"
#include "TH2.h"

namespace ana {

//----------------------------------------------------------------------
PredictionEnuWeightedNoExtrap::PredictionEnuWeightedNoExtrap(PredictionExtrap *pred)
    : PredictionNoExtrap(pred->GetExtrap()), fEnuWeight(nullptr) {}

//----------------------------------------------------------------------
PredictionEnuWeightedNoExtrap::PredictionEnuWeightedNoExtrap(Loaders &loaders,
                                                       const HistAxis &axis,
                                                       const Cut &cut,
                                                       const SystShifts &shift,
                                                       const Var &wei)
    : PredictionNoExtrap(new TrivialExtrap(loaders, axis, cut, shift, wei)),
      fEnuWeight(nullptr) {}

//----------------------------------------------------------------------
Spectrum PredictionEnuWeightedNoExtrap::PredictComponent(osc::IOscCalculator *calc,
                                                      Flavors::Flavors_t flav,
                                                      Current::Current_t curr,
                                                      Sign::Sign_t sign) const {
  Spectrum ret = fExtrap->NCComponent(); // Get binning
  ret.Clear();

  if (curr & Current::kCC) {
    if ((flav & Flavors::kNuEToNuE) && (sign & Sign::kNu) &&
        (fExtrap->NueSurvComponent().IsAlive())) {

      const OscCurve curve(calc, +12, +12);
      TH1D *osc = curve.ToTH1();
      if (fEnuWeight) {
        osc->Multiply(fEnuWeight);
      }

      ret += fExtrap->NueSurvComponent().WeightedByErrors(osc);
      HistCache::Delete(osc);
    }
    if ((flav & Flavors::kNuEToNuE) && (sign & Sign::kAntiNu) &&
        (fExtrap->AntiNueSurvComponent().IsAlive())) {
      const OscCurve curve(calc, -12, -12);
      TH1D *osc = curve.ToTH1();
      if (fEnuWeight) {
        osc->Multiply(fEnuWeight);
      }
      ret += fExtrap->AntiNueSurvComponent().WeightedByErrors(osc);
      HistCache::Delete(osc);
    }
    if ((flav & Flavors::kNuEToNuMu) && (sign & Sign::kNu) &&
        (fExtrap->NumuAppComponent().IsAlive())) {
      const OscCurve curve(calc, +12, +14);
      TH1D *osc = curve.ToTH1();
      if (fEnuWeight) {
        osc->Multiply(fEnuWeight);
      }
      ret += fExtrap->NumuAppComponent().WeightedByErrors(osc);
      HistCache::Delete(osc);
    }
    if ((flav & Flavors::kNuEToNuMu) && (sign & Sign::kAntiNu) &&
        (fExtrap->AntiNumuAppComponent().IsAlive())) {
      const OscCurve curve(calc, -12, -14);
      TH1D *osc = curve.ToTH1();
      if (fEnuWeight) {
        osc->Multiply(fEnuWeight);
      }
      ret += fExtrap->AntiNumuAppComponent().WeightedByErrors(osc);
      HistCache::Delete(osc);
    }

    if ((flav & Flavors::kNuEToNuTau) && (sign & Sign::kNu) &&
        (fExtrap->TauFromEComponent().IsAlive())) {
      const OscCurve curve(calc, +12, +16);
      TH1D *osc = curve.ToTH1();
      if (fEnuWeight) {
        osc->Multiply(fEnuWeight);
      }
      ret += fExtrap->TauFromEComponent().WeightedByErrors(osc);
      HistCache::Delete(osc);
    }
    if ((flav & Flavors::kNuEToNuTau) && (sign & Sign::kAntiNu) &&
        (fExtrap->AntiTauFromEComponent().IsAlive())) {
      const OscCurve curve(calc, -12, -16);
      TH1D *osc = curve.ToTH1();
      if (fEnuWeight) {
        osc->Multiply(fEnuWeight);
      }
      ret += fExtrap->AntiTauFromEComponent().WeightedByErrors(osc);
      HistCache::Delete(osc);
    }

    if ((flav & Flavors::kNuMuToNuE) && (sign & Sign::kNu) &&
        (fExtrap->NueAppComponent().IsAlive())) {
      const OscCurve curve(calc, +14, +12);
      TH1D *osc = curve.ToTH1();
      if (fEnuWeight) {
        osc->Multiply(fEnuWeight);
      }
      ret += fExtrap->NueAppComponent().WeightedByErrors(osc);
      HistCache::Delete(osc);
    }
    if ((flav & Flavors::kNuMuToNuE) && (sign & Sign::kAntiNu) &&
        (fExtrap->AntiNueAppComponent().IsAlive())) {
      const OscCurve curve(calc, -14, -12);
      TH1D *osc = curve.ToTH1();
      if (fEnuWeight) {
        osc->Multiply(fEnuWeight);
      }
      ret += fExtrap->AntiNueAppComponent().WeightedByErrors(osc);
      HistCache::Delete(osc);
    }

    if ((flav & Flavors::kNuMuToNuMu) && (sign & Sign::kNu) &&
        (fExtrap->NumuSurvComponent().IsAlive())) {
      const OscCurve curve(calc, +14, +14);
      TH1D *osc = curve.ToTH1();
      if (fEnuWeight) {
        osc->Multiply(fEnuWeight);
      }
      ret += fExtrap->NumuSurvComponent().WeightedByErrors(osc);
      HistCache::Delete(osc);
    }
    if ((flav & Flavors::kNuMuToNuMu) && (sign & Sign::kAntiNu) &&
        (fExtrap->AntiNumuSurvComponent().IsAlive())) {
      const OscCurve curve(calc, -14, -14);
      TH1D *osc = curve.ToTH1();
      if (fEnuWeight) {
        osc->Multiply(fEnuWeight);
      }
      ret += fExtrap->AntiNumuSurvComponent().WeightedByErrors(osc);
      HistCache::Delete(osc);
    }
    if ((flav & Flavors::kNuMuToNuTau) && (sign & Sign::kNu) &&
        (fExtrap->TauFromMuComponent().IsAlive())) {
      const OscCurve curve(calc, +14, +16);
      TH1D *osc = curve.ToTH1();
      if (fEnuWeight) {
        osc->Multiply(fEnuWeight);
      }
      ret += fExtrap->TauFromMuComponent().WeightedByErrors(osc);
      HistCache::Delete(osc);
    }
    if ((flav & Flavors::kNuMuToNuTau) && (sign & Sign::kAntiNu) &&
        (fExtrap->AntiTauFromMuComponent().IsAlive())) {
      const OscCurve curve(calc, -14, -16);
      TH1D *osc = curve.ToTH1();
      if (fEnuWeight) {
        osc->Multiply(fEnuWeight);
      }
      ret += fExtrap->AntiTauFromMuComponent().WeightedByErrors(osc);
      HistCache::Delete(osc);
    }
  }
  if (curr & Current::kNC && (fExtrap->NCComponent().IsAlive())) {
    assert(flav == Flavors::kAll); // Don't know how to calculate anything else
    assert(sign == Sign::kBoth); // Why would you want to split NCs out by sign?

    ret += fExtrap->NCComponent();
  }

  return ret;
}

//----------------------------------------------------------------------
void PredictionEnuWeightedNoExtrap::SaveTo(TDirectory *dir) const {
  TDirectory *tmp = gDirectory;

  dir->cd();

  TObjString("PredictionEnuWeightedNoExtrap").Write("type");

  fExtrap->SaveTo(dir->mkdir("extrap"));

  tmp->cd();
}

//----------------------------------------------------------------------
std::unique_ptr<PredictionEnuWeightedNoExtrap>
PredictionEnuWeightedNoExtrap::LoadFrom(TDirectory *dir) {
  assert(dir->GetDirectory("extrap"));
  IExtrap *extrap =
      ana::LoadFrom<IExtrap>(dir->GetDirectory("extrap")).release();
  PredictionExtrap *pred = new PredictionExtrap(extrap);
  return std::unique_ptr<PredictionEnuWeightedNoExtrap>(
      new PredictionEnuWeightedNoExtrap(pred));
}
} // namespace ana
