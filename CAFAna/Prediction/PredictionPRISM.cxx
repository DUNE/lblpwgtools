#ifdef CAF_USE_PRISM

#include "CAFAna/Prediction/PredictionPRISM.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Extrap/IExtrap.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Extrap/TrivialExtrap.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "TAxis.h"
#include "TDirectory.h"
#include "TObjString.h"

namespace ana {

PredictionPRISM::PredictionPRISM(
    SpectrumLoaderBase &ND_loader, const HistAxis &recoAxis,
    const HistAxis &offAxis, const Cut &cut, const SystShifts &shift,
    const Var &wei, PRISMExtrapolator const *flux_matcher,
    PRISMExtrapolator::FluxPredSpecies NDFluxSpecies,
    PRISMExtrapolator::FluxPredSpecies FDFluxSpecies, TH3 const *SelectedNumuCC,
    TH3 const *AllNumuCC)
    : fPredictionAxis(recoAxis), fOffAxis(offAxis) {

  bool SignalIsNumode = (static_cast<int>(fFDFluxSpecies) < 4);
  bool HaveEff = (SelectedNumuCC && AllNumuCC);

  // Use to weight by cheating Efficiency
  Var kEffWeight([&](const caf::StandardRecord *sr) -> double {
    int bx = AllNumuCC->GetXaxis()->FindFixBin(sr->Ev);
    int by = AllNumuCC->GetYaxis()->FindFixBin(sr->vtx_x);
    int bz = AllNumuCC->GetZaxis()->FindFixBin(sr->det_x);
    int ev_bin = AllNumuCC->GetBin(bx, by, bz);
    return AllNumuCC->GetBinContent(ev_bin) /
           SelectedNumuCC->GetBinContent(ev_bin);
  });

  // When default initialization is cheap, its easier to read explicit
  // assignments
  fOffAxisSpectrum = std::make_unique<ReweightableSpectrum>(
      ND_loader, recoAxis, offAxis, cut, shift, wei);
  fOffAxisSpectrumNumu = std::make_unique<ReweightableSpectrum>(
      ND_loader, recoAxis, offAxis, cut && !kIsNC && kIsNumuCC && !kIsAntiNu,
      shift, wei * (HaveEff ? kEffWeight : ana::Constant(1)));
  fOffAxisSpectrumNumubar = std::make_unique<ReweightableSpectrum>(
      ND_loader, recoAxis, offAxis, cut && !kIsNC && kIsNumuCC && kIsAntiNu,
      shift, wei * (HaveEff ? kEffWeight : ana::Constant(1)));

  fNDBkg = NDBackgroundSpectra{nullptr, nullptr, nullptr};
  fHaveNDBkgPred = false;

  fFarDetSpectrumNC = nullptr;
  fFarDetSpectrumNumu = nullptr;
  fFarDetSpectrumNumubar = nullptr;
  fHaveFDPred = false;

  fFluxMatcher = flux_matcher;
  fNDFluxSpecies = NDFluxSpecies;
  fFDFluxSpecies = FDFluxSpecies;

  if (fFluxMatcher) {
    assert(fFluxMatcher->CheckOffAxisBinningConsistency(
        offAxis.GetBinnings().front()));
  }
}

//----------------------------------------------------------------------
void PredictionPRISM::AddFDMCLoader(SpectrumLoaderBase &FD_loader,
                                    const Cut &cut, const SystShifts &shift,
                                    const Var &wei) {
  fFarDetSpectrumNC = std::make_unique<Spectrum>(FD_loader, fPredictionAxis,
                                                 cut && kIsNC, shift, wei);
  fFarDetSpectrumNumu = std::make_unique<OscillatableSpectrum>(
      FD_loader, fPredictionAxis, cut && kIsNC && kIsNumuCC && !kIsAntiNu,
      shift, wei);
  fFarDetSpectrumNumubar = std::make_unique<OscillatableSpectrum>(
      FD_loader, fPredictionAxis, cut && !kIsNC && kIsNumuCC && kIsAntiNu,
      shift, wei);
  fHaveFDPred = true;
}

//----------------------------------------------------------------------
void PredictionPRISM::AddNDMCLoader(SpectrumLoaderBase &ND_loader,
                                    const Cut &cut, const SystShifts &shift,
                                    const Var &wei) {

  fNDBkg.fOffAxisSpectrumNC = std::make_unique<ReweightableSpectrum>(
      ND_loader, fPredictionAxis, fOffAxis, cut && kIsNC, shift, wei);
  fNDBkg.fOffAxisSpectrumNue = std::make_unique<ReweightableSpectrum>(
      ND_loader, fPredictionAxis, fOffAxis,
      cut && !kIsNC && kIsBeamNue && !kIsAntiNu, shift, wei);
  fNDBkg.fOffAxisSpectrumNuebar = std::make_unique<ReweightableSpectrum>(
      ND_loader, fPredictionAxis, fOffAxis,
      cut && !kIsNC && kIsBeamNue && kIsAntiNu, shift, wei);
  fHaveNDBkgPred = true;
}

//----------------------------------------------------------------------
Spectrum PredictionPRISM::Predict(osc::IOscCalculator *calc) const {

  ReweightableSpectrum ret = *fOffAxisSpectrum;
  ret.OverridePOT(1);

  bool SignalIsNumode = (static_cast<int>(fFDFluxSpecies) < 4);

  // If we have the ND background subtract it, if not, assume we can just cheat
  // and take the ND NumuCC spectrum
  if (fHaveNDBkgPred) {
    fNDBkg.fOffAxisSpectrumNC->OverridePOT(1);
    fNDBkg.fOffAxisSpectrumNue->OverridePOT(1);
    fNDBkg.fOffAxisSpectrumNuebar->OverridePOT(1);
    fOffAxisSpectrumNumubar->OverridePOT(1);
    fOffAxisSpectrumNumu->OverridePOT(1);

    ret -= *fNDBkg.fOffAxisSpectrumNC;
    ret -= *fNDBkg.fOffAxisSpectrumNue;
    ret -= *fNDBkg.fOffAxisSpectrumNuebar;
    ret -= *(SignalIsNumode ? fOffAxisSpectrumNumubar : fOffAxisSpectrumNumu);
  } else {
    fOffAxisSpectrumNumubar->OverridePOT(1);
    fOffAxisSpectrumNumu->OverridePOT(1);
    ret = *(SignalIsNumode ? fOffAxisSpectrumNumu : fOffAxisSpectrumNumubar);
  }

  if (fFluxMatcher) {
    double max_off_axis_pos = ret.GetReweightTAxis()->GetBinCenter(
        ret.GetReweightTAxis()->GetNbins());

    // If we have the FD background predictions add them back in
    if (fHaveFDPred) {
      // Scale this up to match the FD POT before adding it back in
      ret.ScaleToPOT(fFarDetSpectrumNC->POT());

      Spectrum rets = ret.WeightedBy(fFluxMatcher->GetMatchCoefficients(
          calc, max_off_axis_pos, fNDFluxSpecies, fFDFluxSpecies));

      rets += *fFarDetSpectrumNC;
      rets += (SignalIsNumode ? fFarDetSpectrumNumu : fFarDetSpectrumNumubar)
                  ->Oscillated(calc, 14, 14);
      return rets;
    } else {
      return ret.WeightedBy(fFluxMatcher->GetMatchCoefficients(
          calc, max_off_axis_pos, fNDFluxSpecies, fFDFluxSpecies));
    }

  } else {
    return ret.UnWeighted();
  }
}

//----------------------------------------------------------------------
Spectrum PredictionPRISM::PredictComponent(osc::IOscCalculator *calc,
                                           Flavors::Flavors_t flav,
                                           Current::Current_t curr,
                                           Sign::Sign_t sign) const {

  // Fill in later
  return fOffAxisSpectrum->UnWeighted();
}

//----------------------------------------------------------------------
void PredictionPRISM::SaveTo(TDirectory *dir) const {
  TDirectory *tmp = gDirectory;

  dir->cd();

  TObjString("PredictionPRISM").Write("type");

  fOffAxisSpectrum->SaveTo(dir->mkdir("spect"));
  fOffAxisSpectrumNumu->SaveTo(dir->mkdir("spect_numu"));
  fOffAxisSpectrumNumu->SaveTo(dir->mkdir("spect_numubar"));

  if (fHaveNDBkgPred) {
    fNDBkg.fOffAxisSpectrumNC->SaveTo(dir->mkdir("spect_nd_NC"));
    fNDBkg.fOffAxisSpectrumNue->SaveTo(dir->mkdir("spect_nd_nue"));
    fNDBkg.fOffAxisSpectrumNuebar->SaveTo(dir->mkdir("spect_nd_nuebar"));
  }

  if (fHaveFDPred) {
    fFarDetSpectrumNC->SaveTo(dir->mkdir("spect_fd_NC"));
    fFarDetSpectrumNumu->SaveTo(dir->mkdir("spect_fd_numu"));
    fFarDetSpectrumNumubar->SaveTo(dir->mkdir("spect_fd_numubar"));
  }

  tmp->cd();
}

//----------------------------------------------------------------------
std::unique_ptr<PredictionPRISM>
PredictionPRISM::LoadFrom(TDirectory *dir, PRISMExtrapolator const *flux_matcher,
                          PRISMExtrapolator::FluxPredSpecies NDFluxSpecies,
                          PRISMExtrapolator::FluxPredSpecies FDFluxSpecies) {
  std::unique_ptr<PredictionPRISM> pred(new PredictionPRISM(
      ana::LoadFrom<ReweightableSpectrum>(dir->GetDirectory("spect")),
      ana::LoadFrom<ReweightableSpectrum>(dir->GetDirectory("spect_numu")),
      ana::LoadFrom<ReweightableSpectrum>(dir->GetDirectory("spect_numubar")),
      flux_matcher, NDFluxSpecies, FDFluxSpecies));

  if (dir->GetDirectory("spect_nd_NC")) {
    pred->SetNDBkgPredicitions(
        ana::LoadFrom<ReweightableSpectrum>(dir->GetDirectory("spect_nd_NC")),
        ana::LoadFrom<ReweightableSpectrum>(dir->GetDirectory("spect_nd_nue")),
        ana::LoadFrom<ReweightableSpectrum>(
            dir->GetDirectory("spect_nd_nuebar")));
  }

  if (dir->GetDirectory("spect_fd_NC")) {
    pred->SetFDPredicitions(
        ana::LoadFrom<Spectrum>(dir->GetDirectory("spect_fd_NC")),
        ana::LoadFrom<OscillatableSpectrum>(dir->GetDirectory("spect_fd_numu")),
        ana::LoadFrom<OscillatableSpectrum>(
            dir->GetDirectory("spect_fd_numubar")));
  }

  return pred;
}
} // namespace ana

#endif
