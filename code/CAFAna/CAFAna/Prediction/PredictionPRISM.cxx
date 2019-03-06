#include "CAFAna/Prediction/PredictionPRISM.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Extrap/IExtrap.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Extrap/TrivialExtrap.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "TDirectory.h"
#include "TObjString.h"
#include "TAxis.h"

namespace ana {

PredictionPRISM::PredictionPRISM(
    SpectrumLoaderBase &loader, const HistAxis &recoAxis,
    const HistAxis &offAxis, const Cut &cut, const SystShifts &shift,
    const Var &wei, PRISMFluxMatcher *flux_matcher,
    PRISMFluxMatcher::FluxPredSpecies NDFluxSpecies,
    PRISMFluxMatcher::FluxPredSpecies FDFluxSpecies)
    : fOffAxisSpectrum(loader, recoAxis, offAxis, cut, shift, wei),
      fOffAxisSpectrumNC(loader, recoAxis, offAxis, cut && kIsNC, shift, wei),
      fOffAxisSpectrumNumu(loader, recoAxis, offAxis,
                           cut && !kIsNC && kIsNumuCC && !kIsAntiNu, shift,
                           wei),
      fOffAxisSpectrumNumubar(loader, recoAxis, offAxis,
                              cut && !kIsNC && kIsNumuCC && kIsAntiNu, shift,
                              wei),
      fOffAxisSpectrumNue(loader, recoAxis, offAxis,
                          cut && !kIsNC && kIsBeamNue && !kIsAntiNu, shift,
                          wei),
      fOffAxisSpectrumNuebar(loader, recoAxis, offAxis,
                             cut && !kIsNC && kIsBeamNue && kIsAntiNu, shift,
                             wei),
      fFluxMatcher(flux_matcher), fNDFluxSpecies(NDFluxSpecies),
      fFDFluxSpecies(FDFluxSpecies) {
  if (fFluxMatcher) {
    assert(fFluxMatcher->CheckOffAxisBinningConsistency(
        offAxis.GetBinnings().front()));
  }
}

//----------------------------------------------------------------------
Spectrum PredictionPRISM::Predict(osc::IOscCalculator *calc) const {
  if (fFluxMatcher) {

    double max_off_axis_pos = fOffAxisSpectrum.GetReweightTAxis()->GetBinCenter(
        fOffAxisSpectrum.GetReweightTAxis()->GetNbins());

    return fOffAxisSpectrum.WeightedBy(fFluxMatcher->GetFluxMatchCoefficients(
        calc, max_off_axis_pos, fNDFluxSpecies, fFDFluxSpecies));
  } else {
    return fOffAxisSpectrum.UnWeighted();
  }
}

//----------------------------------------------------------------------
Spectrum PredictionPRISM::PredictComponent(osc::IOscCalculator *calc,
                                           Flavors::Flavors_t flav,
                                           Current::Current_t curr,
                                           Sign::Sign_t sign) const {

  assert(fFluxMatcher);

  double max_off_axis_pos = fOffAxisSpectrum.GetReweightTAxis()->GetBinCenter(
      fOffAxisSpectrum.GetReweightTAxis()->GetNbins());

  if (flav == Flavors::kAll && curr == Current::kBoth && sign == Sign::kBoth)
    return Predict(0); // Faster

  if (curr & Current::kNC) {
    // We don't have NC broken down by sign or flavour
    assert(flav & Flavors::kAll && sign & Sign::kBoth);
    return fOffAxisSpectrumNC.WeightedBy(fFluxMatcher->GetFluxMatchCoefficients(
        calc, max_off_axis_pos, fNDFluxSpecies, fFDFluxSpecies));
  }

  assert(curr == Current::kCC);

  ReweightableSpectrum ret = fOffAxisSpectrum;
  ret.Clear();

  // Safe to assume by this point that it's 100% CC
  if (flav & Flavors::kNuMuToNuMu && sign & Sign::kNu)
    ret += fOffAxisSpectrumNumu;
  if (flav & Flavors::kNuMuToNuMu && sign & Sign::kAntiNu)
    ret += fOffAxisSpectrumNumubar;
  if (flav & Flavors::kNuEToNuE && sign & Sign::kNu)
    ret += fOffAxisSpectrumNue;
  if (flav & Flavors::kNuEToNuE && sign & Sign::kAntiNu)
    ret += fOffAxisSpectrumNuebar;

  return ret.WeightedBy(fFluxMatcher->GetFluxMatchCoefficients(
      calc, max_off_axis_pos, fNDFluxSpecies, fFDFluxSpecies));
}

//----------------------------------------------------------------------
void PredictionPRISM::SaveTo(TDirectory *dir) const {
  TDirectory *tmp = gDirectory;

  dir->cd();

  TObjString("PredictionPRISM").Write("type");

  fOffAxisSpectrum.SaveTo(dir->mkdir("spect"));
  fOffAxisSpectrumNC.SaveTo(dir->mkdir("spect_nc"));
  fOffAxisSpectrumNumu.SaveTo(dir->mkdir("spect_numu"));
  fOffAxisSpectrumNumubar.SaveTo(dir->mkdir("spect_numubar"));
  fOffAxisSpectrumNue.SaveTo(dir->mkdir("spect_nue"));
  fOffAxisSpectrumNuebar.SaveTo(dir->mkdir("spect_nuebar"));

  tmp->cd();
}

//----------------------------------------------------------------------
std::unique_ptr<PredictionPRISM>
PredictionPRISM::LoadFrom(TDirectory *dir, PRISMFluxMatcher *flux_matcher,
                          PRISMFluxMatcher::FluxPredSpecies NDFluxSpecies,
                          PRISMFluxMatcher::FluxPredSpecies FDFluxSpecies) {
  return std::unique_ptr<PredictionPRISM>(new PredictionPRISM(
      *ana::LoadFrom<ReweightableSpectrum>(dir->GetDirectory("spect")),
      *ana::LoadFrom<ReweightableSpectrum>(dir->GetDirectory("spect_nc")),
      *ana::LoadFrom<ReweightableSpectrum>(dir->GetDirectory("spect_numu")),
      *ana::LoadFrom<ReweightableSpectrum>(dir->GetDirectory("spect_numubar")),
      *ana::LoadFrom<ReweightableSpectrum>(dir->GetDirectory("spect_nue")),
      *ana::LoadFrom<ReweightableSpectrum>(dir->GetDirectory("spect_nuebar")),
      flux_matcher, NDFluxSpecies, FDFluxSpecies));
}
} // namespace ana
