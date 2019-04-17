#ifdef CAF_USE_PRISM

#include "CAFAna/Prediction/PredictionPRISM.h"

#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/OscCurve.h"
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
    : fPredictionAxis(recoAxis), fOffAxis(offAxis), fFDTrueEnergyBins() {

  bool SignalIsNumode = (static_cast<int>(fFDFluxSpecies) < 4);
  bool HaveEff = (SelectedNumuCC && AllNumuCC);

  // Use to weight by cheating Efficiency
  Var kEffWeight({}, [&](const caf::StandardRecord *sr) -> double {
    int bx = AllNumuCC->GetXaxis()->FindFixBin(sr->dune.Ev);
    int by = AllNumuCC->GetYaxis()->FindFixBin(sr->dune.vtx_x);
    int bz = AllNumuCC->GetZaxis()->FindFixBin(sr->dune.det_x);
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
      FD_loader, fPredictionAxis, cut && !kIsNC && kIsNumuCC && !kIsAntiNu,
      shift, wei);
  fFarDetSpectrumNumubar = std::make_unique<OscillatableSpectrum>(
      FD_loader, fPredictionAxis, cut && !kIsNC && kIsNumuCC && kIsAntiNu,
      shift, wei);
  fHaveFDPred = true;
}

//----------------------------------------------------------------------
void PredictionPRISM::AddNDMCBkgLoader(SpectrumLoaderBase &ND_loader,
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
  std::map<PredictionPRISM::PRISMComponent, Spectrum> Comps =
      PredictPRISMComponents(calc);

  assert(Comps.size());

  Spectrum pred = Comps[kNDDataCorr];

  for (auto const &cmp : {kFDFluxCorr, kFDNCBkg, kFDWSBkg}) {
    if (Comps.count(cmp)) {
      pred += Cmp.second;
    }
  }

  return pred;
}

std::map<PredictionPRISM::PRISMComponent, Spectrum>
PredictionPRISM::PredictPRISMComponents(osc::IOscCalculator *calc) const {

  DontAddDirectory guard;

  // Using maps for non-default constructible classes is awful...
  std::map<PredictionPRISM::PRISMComponent, ReweightableSpectrum> NDComps;
  std::map<PredictionPRISM::PRISMComponent, Spectrum> Comps;

  NDComps.emplace(kNDData, *fOffAxisSpectrum);
  NDComps.emplace(kNDDataCorr, *fOffAxisSpectrum);

  NDComps.emplace(kNDDataSig, *fOffAxisSpectrum);
  NDComps.emplace(kNDWSBkg, *fOffAxisSpectrum);
  NDComps.emplace(kNDNCBkg, *fOffAxisSpectrum);
  NDComps.emplace(kNDNueBkg, *fOffAxisSpectrum);

  NDComps.at(kNDDataSig).Clear();
  NDComps.at(kNDWSBkg).Clear();
  NDComps.at(kNDNCBkg).Clear();
  NDComps.at(kNDNueBkg).Clear();

  double xslice_width_cm =
      NDComps.at(kNDData).GetReweightTAxis()->GetBinWidth(1) * 1E2;
  double POT = 1.0 / FD_ND_FVRatio(xslice_width_cm);

  bool SignalIsNumode = (static_cast<int>(fFDFluxSpecies) < 4);

  // If we have the ND background subtract it, if not, assume we can just cheat
  // and take the ND NumuCC spectrum
  if (fHaveNDBkgPred) {
    NDComps.at(kNDNCBkg) -= *fNDBkg.fOffAxisSpectrumNC;
    NDComps.at(kNDNueBkg) -= *fNDBkg.fOffAxisSpectrumNue;
    NDComps.at(kNDNueBkg) -= *fNDBkg.fOffAxisSpectrumNuebar;
    NDComps.at(kNDWSBkg) -=
        *(SignalIsNumode ? fOffAxisSpectrumNumubar : fOffAxisSpectrumNumu);

    NDComps.at(kNDDataCorr) -= *fNDBkg.fOffAxisSpectrumNC;
    NDComps.at(kNDDataCorr) -= *fNDBkg.fOffAxisSpectrumNue;
    NDComps.at(kNDDataCorr) -= *fNDBkg.fOffAxisSpectrumNuebar;
    NDComps.at(kNDDataCorr) -=
        *(SignalIsNumode ? fOffAxisSpectrumNumubar : fOffAxisSpectrumNumu);
  } else { // Cheat and use the truth signal
    NDComps.at(kNDDataCorr).Clear();
    NDComps.at(kNDDataCorr) +=
        *(SignalIsNumode ? fOffAxisSpectrumNumu : fOffAxisSpectrumNumubar);
  }

  NDComps.at(kNDDataSig) +=
      *(SignalIsNumode ? fOffAxisSpectrumNumu : fOffAxisSpectrumNumubar);

  for (auto &NDC : NDComps) {
    NDC.second.OverridePOT(POT);
  }

  if (fFluxMatcher) {
    double max_off_axis_pos =
        NDComps.at(kNDData).GetReweightTAxis()->GetBinCenter(
            NDComps.at(kNDData).GetReweightTAxis()->GetNbins());

    TH1 const *LinearCombination = fFluxMatcher->GetMatchCoefficients(
        calc, max_off_axis_pos, fNDFluxSpecies, fFDFluxSpecies);

    Comps.emplace(kNDDataSig,
                  NDComps.at(kNDDataSig).WeightedByErrors(LinearCombination));

    Comps.emplace(kNDDataCorr,
                  NDComps.at(kNDDataCorr).WeightedByErrors(LinearCombination));

    // If we have the FD background predictions add them back in
    if (fHaveFDPred) {
      // Scale this to match the ND POT before adding it back in
      Comps.emplace(kFDNCBkg, *fFarDetSpectrumNC);

      int rightSignNumuPDG = SignalIsNumode ? 14 : -14;
      Comps.emplace(
          kFDWSBkg,
          (SignalIsNumode ? fFarDetSpectrumNumubar : fFarDetSpectrumNumu)
              ->Oscillated(calc, rightSignNumuPDG, rightSignNumuPDG));

      std::unique_ptr<TH1> flux_miss_match(static_cast<TH1 *>(
          fFluxMatcher->GetLastResidual()->Clone("flux_miss_match")));
      flux_miss_match->SetDirectory(nullptr);

      if (!fFDTrueEnergyBins.size()) {
        TAxis const *offAxis = fFarDetSpectrumNumu->GetReweightTAxis();
        for (int i = 0; i < offAxis->GetNbins(); ++i) {
          fFDTrueEnergyBins.push_back(offAxis->GetBinLowEdge(i + 1));
        }
        fFDTrueEnergyBins.push_back(offAxis->GetBinUpEdge(offAxis->GetNbins()));
      }

      assert(fFDTrueEnergyBins.size());

      std::unique_ptr<TH1> FluxMissWeighter(new TH1D(
          "fmw", "", fFDTrueEnergyBins.size() - 1, fFDTrueEnergyBins.data()));

      double miss_Emax = flux_miss_match->GetXaxis()->GetBinUpEdge(
          flux_miss_match->GetXaxis()->GetNbins());

      for (size_t bin_it = 0; bin_it < (fFDTrueEnergyBins.size() - 1);
           ++bin_it) {
        bool done = false;

        size_t nint_steps = 100;
        double bin_low_edge = fFDTrueEnergyBins[bin_it];
        double bin_up_edge = fFDTrueEnergyBins[bin_it + 1];
        double step = (bin_up_edge - bin_low_edge) / double(nint_steps);
        double sum = 0;
        size_t s_it = 0;
        for (; s_it < nint_steps; ++s_it) {
          double E = bin_low_edge + double(s_it) * step;
          if (E > miss_Emax) {
            done = true;
            break;
          }
          sum += flux_miss_match->Interpolate(E);
        }
        FluxMissWeighter->SetBinContent(bin_it, sum / double(s_it));
        if (done) {
          break;
        }
      }
      FluxMissWeighter->SetDirectory(nullptr);

      const OscCurve curve(calc, rightSignNumuPDG, rightSignNumuPDG);
      TH1D *Ps = curve.ToTH1();
      FluxMissWeighter->Multiply(Ps);

      Comps.emplace(
          kFDFluxCorr,
          (SignalIsNumode ? fFarDetSpectrumNumu : fFarDetSpectrumNumubar)
              ->WeightedByErrors(FluxMissWeighter.get()));
      HistCache::Delete(Ps);

      for (auto &cmp : Comps) { // Set these to /POT for combination
        cmp.second.ScaleToPOT(1);
      }
    }
  }

  for (auto const &NDC : NDComps) { // If you haven't been added, just add the
                                    // unweighted spectrum.
    if (!Comps.count(NDC.first)) {
      Comps.emplace(NDC.first, NDC.second.ToSpectrum());
    }
  }
  return Comps;
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
  fOffAxisSpectrumNumubar->SaveTo(dir->mkdir("spect_numubar"));

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
PredictionPRISM::LoadFrom(TDirectory *dir,
                          PRISMExtrapolator const *flux_matcher,
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
