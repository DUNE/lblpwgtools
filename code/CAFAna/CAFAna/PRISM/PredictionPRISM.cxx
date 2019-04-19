#ifdef CAF_USE_PRISM

#include "CAFAna/PRISM/PredictionPRISM.h"
#include "CAFAna/PRISM/PRISMUtils.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/OscCurve.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "OscLib/func/IOscCalculator.h"

#include "TAxis.h"
#include "TDirectory.h"
#include "TObjString.h"
#include "TH2.h"

namespace ana {

PredictionPRISM::PredictionPRISM(const HistAxis &recoAxis,
                                 const HistAxis &offAxis)
    : fOffAxisData(nullptr), fHaveData(false), fOffAxisPredGen(nullptr),
      fOffAxisPrediction(nullptr), fHaveNDPred(false), fFDPredGen(nullptr),
      fFarDetPrediction(nullptr), fHaveFDPred(false), fFluxMatcher(nullptr),
      fNDFluxSpecies(PRISMExtrapolator::FluxPredSpecies::kNumu_numode),
      fFDFluxSpecies(PRISMExtrapolator::FluxPredSpecies::kNumu_numode),
      fPredictionAxis(recoAxis), fOffAxis(offAxis), fFDTrueEnergyBins()

{
  std::vector<std::string> Labels = fPredictionAxis.GetLabels();
  std::vector<Binning> Bins = fPredictionAxis.GetBinnings();
  std::vector<Var> Vars = fPredictionAxis.GetVars();
  Labels.push_back(fOffAxis.GetLabels().front());
  Bins.push_back(fOffAxis.GetBinnings().front());
  Vars.push_back(fOffAxis.GetVars().front());
  fOffPredictionAxis = std::make_unique<HistAxis>(Labels, Bins, Vars);
}

PredictionPRISM::PredictionPRISM(SpectrumLoaderBase &ND_loader,
                                 const HistAxis &recoAxis,
                                 const HistAxis &offAxis, const Cut &cut,
                                 const Var &wei)
    : PredictionPRISM(recoAxis, offAxis)

{
  fOffAxisData = std::make_unique<ReweightableSpectrum>(
      ND_loader, fPredictionAxis, fOffAxis, cut, kNoShift, wei);
  fHaveData = true;
}

void PredictionPRISM::AddNDMCLoader(Loaders &loaders, const Cut &cut,
                                    const Var &wei,
                                    std::vector<ISyst const *> systlist) {

  osc::NoOscillations kNoOsc;

  fOffAxisPredGen =
      std::make_unique<NoOscPredictionGenerator>(*fOffPredictionAxis, cut, wei);
  fOffAxisPrediction = std::make_unique<PredictionInterp>(
      systlist, &kNoOsc, *fOffAxisPredGen, loaders);

  fHaveNDPred = true;
}

void PredictionPRISM::AddFDMCLoader(Loaders &loaders, const Cut &cut,
                                    const Var &wei,
                                    std::vector<ISyst const *> systlist) {

  osc::NoOscillations kNoOsc;

  fFDPredGen = std::make_unique<ModifiedNoExtrapPredictionGenerator>(
      fPredictionAxis, cut, wei);
  fFarDetPrediction = std::make_unique<PredictionInterp>(systlist, &kNoOsc,
                                                         *fFDPredGen, loaders);

  fFarDetPrediction->SetDontUseCache();

  fHaveFDPred = true;
}

//----------------------------------------------------------------------
Spectrum PredictionPRISM::Predict(osc::IOscCalculator *calc) const {
  return PredictSyst(calc, kNoShift);
}

//----------------------------------------------------------------------
Spectrum PredictionPRISM::PredictSyst(osc::IOscCalculator *calc,
                                      SystShifts shift) const {
  std::map<PredictionPRISM::PRISMComponent, Spectrum> Comps =
      PredictPRISMComponents(calc, shift, false);

  assert(Comps.size());

  Spectrum pred = Comps.at(kNDDataCorr);

  for (auto const &cmp : {kFDFluxCorr, kFDNCBkg, kFDWSBkg}) {
    if (Comps.count(cmp)) {
      pred += Comps.at(cmp);
    }
  }

  return pred;
}

std::map<PredictionPRISM::PRISMComponent, Spectrum>
PredictionPRISM::PredictPRISMComponents(osc::IOscCalculator *calc,
                                        SystShifts shift, bool AllComps) const {

  assert(fHaveData || fHaveNDPred);

  DontAddDirectory guard;

  // Using maps for non-default constructible classes is awful...
  std::map<PredictionPRISM::PRISMComponent, ReweightableSpectrum> NDComps;
  std::map<PredictionPRISM::PRISMComponent, Spectrum> Comps;

  double xslice_width_cm =
      NDComps.at(kNDData).GetReweightTAxis()->GetBinWidth(1) * 1E2;
  double POT = 1.0 / FD_ND_FVRatio(xslice_width_cm);

  bool SignalIsNumode = (static_cast<int>(fFDFluxSpecies) < 4);

  Sign::Sign_t SigSign = SignalIsNumode ? Sign::kNu : Sign::kAntiNu;
  Sign::Sign_t WrongSign = (!SignalIsNumode) ? Sign::kNu : Sign::kAntiNu;

  if (fHaveData) {
    NDComps.emplace(kNDData, *fOffAxisData);
  }

  if (fHaveNDPred) {

    std::unique_ptr<TH2> NDSig_h(
        fOffAxisPrediction
            ->PredictComponentSyst(calc, shift, Flavors::kAllNuMu, Current::kCC,
                                   SigSign)
            .ToTH2(1));
    ReweightableSpectrum NDSig(fOffAxis.GetVars()[0], NDSig_h.get(),
                               fPredictionAxis.GetLabels(),
                               fPredictionAxis.GetBinnings(), 1, 1);

    // NDComps.emplace(kNDData, *fOffAxisData);
    // NDComps.emplace(kNDDataCorr2D, *fOffAxisData);

    NDComps.emplace(kNDSig, NDSig);

    if (!fHaveData) {
      NDComps.emplace(kNDDataCorr2D, NDSig);
    }

    // TODO: Need to re-implement with the ND PredictionInterp
    // NDComps.emplace(kNDWSBkg, *fOffAxisData);
    // NDComps.emplace(kNDNCBkg, *fOffAxisData);
    // NDComps.emplace(kNDNueBkg, *fOffAxisData);
    //
    // NDComps.at(kNDSig).Clear();
    // NDComps.at(kNDWSBkg).Clear();
    // NDComps.at(kNDNCBkg).Clear();
    // NDComps.at(kNDNueBkg).Clear();

    // If we have the ND background subtract it, if not, assume we can just
    // cheat and take the ND NumuCC spectrum

    // NDComps.at(kNDNCBkg) -= *fNDBkg.fOffAxisSpectrumNC;
    // NDComps.at(kNDNueBkg) -= *fNDBkg.fOffAxisSpectrumNue;
    // NDComps.at(kNDNueBkg) -= *fNDBkg.fOffAxisSpectrumNuebar;
    // NDComps.at(kNDWSBkg) -=
    //     *(SignalIsNumode ? fOffAxisSpectrumNumubar : fOffAxisSpectrumNumu);
    //
    // NDComps.at(kNDDataCorr2D) -= *fNDBkg.fOffAxisSpectrumNC;
    // NDComps.at(kNDDataCorr2D) -= *fNDBkg.fOffAxisSpectrumNue;
    // NDComps.at(kNDDataCorr2D) -= *fNDBkg.fOffAxisSpectrumNuebar;
    // NDComps.at(kNDDataCorr2D) -=
    //     *(SignalIsNumode ? fOffAxisSpectrumNumubar : fOffAxisSpectrumNumu);
    // NDComps.at(kNDSig) +=
    //     *(SignalIsNumode ? fOffAxisSpectrumNumu : fOffAxisSpectrumNumubar);
  }

  for (auto &NDC : NDComps) {
    NDC.second.OverridePOT(POT);
  }

  if (fFluxMatcher) {
    double max_off_axis_pos =
        NDComps.at(kNDDataCorr2D)
            .GetReweightTAxis()
            ->GetBinCenter(
                NDComps.at(kNDDataCorr2D).GetReweightTAxis()->GetNbins());

    TH1 const *LinearCombination = fFluxMatcher->GetMatchCoefficients(
        calc, max_off_axis_pos, fNDFluxSpecies, fFDFluxSpecies);

    Comps.emplace(kNDSig,
                  NDComps.at(kNDSig).WeightedByErrors(LinearCombination));

    Comps.emplace(
        kNDDataCorr,
        NDComps.at(kNDDataCorr2D).WeightedByErrors(LinearCombination));

    // If we have the FD background predictions add them back in
    if (fHaveFDPred) {
      // Scale this to match the ND POT before adding it back in
      // Comps.emplace(kFDNCBkg, *fFarDetSpectrumNC);

      Comps.emplace(kFDWSBkg, fFarDetPrediction->PredictComponentSyst(
                                  calc, shift, Flavors::kAllNuMu, Current::kCC,
                                  WrongSign));

      std::unique_ptr<TH1> flux_miss_match(static_cast<TH1 *>(
          fFluxMatcher->GetLastResidual()->Clone("flux_miss_match")));
      flux_miss_match->SetDirectory(nullptr);

      if (!fFDTrueEnergyBins.size()) {

        const OscCurve curve(calc, 14, 14);
        std::unique_ptr<TH1> os_h(curve.ToTH1());

        for (int i = 0; i < os_h->GetXaxis()->GetNbins(); ++i) {
          fFDTrueEnergyBins.push_back(os_h->GetXaxis()->GetBinLowEdge(i + 1));
        }
        fFDTrueEnergyBins.push_back(
            os_h->GetXaxis()->GetBinUpEdge(os_h->GetXaxis()->GetNbins()));
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

      fFarDetPrediction->GetPredNomAs<PredictionModifiedNoExtrap>()
          ->SetExtraWeighting(FluxMissWeighter.get());

      Comps.emplace(kFDFluxCorr,
                    fFarDetPrediction->PredictComponentSyst(
                        calc, shift, Flavors::kAllNuMu, Current::kCC, SigSign));

      fFarDetPrediction->GetPredNomAs<PredictionModifiedNoExtrap>()
          ->UnsetExtraWeighting();

      for (auto &cmp : Comps) { // Set these to /POT for combination
        cmp.second.ScaleToPOT(1);
      }
    }
  } else {
    Comps.emplace(kNDDataCorr, NDComps.at(kNDDataCorr2D).UnWeighted());
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
  throw;
}

//----------------------------------------------------------------------
void PredictionPRISM::SaveTo(TDirectory *dir) const {
  TDirectory *tmp = gDirectory;

  dir->cd();

  TObjString("PredictionPRISM").Write("type");

  if (fHaveData) {
    fOffAxisData->SaveTo(dir->mkdir("OffAxisData"));
  }
  if (fHaveNDPred) {
    fOffAxisPrediction->SaveTo(dir->mkdir("OffAxisPrediction"));
  }
  if (fHaveFDPred) {
    fFarDetPrediction->SaveTo(dir->mkdir("FarDetPrediction"));
  }

  for (unsigned int i = 0; i < fOffAxis.GetBinnings().size(); ++i) {
    TObjString(fOffAxis.GetLabels()[i].c_str())
        .Write(TString::Format("offaxis_label%d", i).Data());
    fOffAxis.GetBinnings()[i].SaveTo(
        dir->mkdir(TString::Format("offaxis_bins%d", i)));
  }

  for (unsigned int i = 0; i < fPredictionAxis.GetBinnings().size(); ++i) {
    TObjString(fPredictionAxis.GetLabels()[i].c_str())
        .Write(TString::Format("pred_label%d", i).Data());
    fPredictionAxis.GetBinnings()[i].SaveTo(
        dir->mkdir(TString::Format("pred_bins%d", i)));
  }

  tmp->cd();
}

//----------------------------------------------------------------------
std::unique_ptr<PredictionPRISM> PredictionPRISM::LoadFrom(TDirectory *dir) {

  std::vector<std::string> offaxis_labels;
  std::vector<Binning> offaxis_bins;
  std::vector<Var> offaxis_dummy_vars;

  for (int i = 0;; ++i) {
    TDirectory *subdir =
        dir->GetDirectory(TString::Format("offaxis_bins%d", i));
    if (!subdir) {
      break;
    }
    offaxis_bins.push_back(*Binning::LoadFrom(subdir));
    TObjString *label =
        (TObjString *)dir->Get(TString::Format("offaxis_label%d", i));
    offaxis_labels.push_back(label ? label->GetString().Data() : "");
    offaxis_dummy_vars.push_back(kUnweighted);
  }

  std::vector<std::string> pred_labels;
  std::vector<Binning> pred_bins;
  std::vector<Var> pred_dummy_vars;

  for (int i = 0;; ++i) {
    TDirectory *subdir = dir->GetDirectory(TString::Format("pred_bins%d", i));
    if (!subdir) {
      break;
    }
    pred_bins.push_back(*Binning::LoadFrom(subdir));
    TObjString *label =
        (TObjString *)dir->Get(TString::Format("pred_label%d", i));
    pred_labels.push_back(label ? label->GetString().Data() : "");
    pred_dummy_vars.push_back(kUnweighted);
  }

  HistAxis const offAxis(offaxis_labels, offaxis_bins, offaxis_dummy_vars);
  HistAxis const predictionAxis(pred_labels, pred_bins, pred_dummy_vars);

  std::unique_ptr<PredictionPRISM> pred =
      std::make_unique<PredictionPRISM>(predictionAxis, offAxis);

  if (dir->GetDirectory("OffAxisData")) {
    pred->fOffAxisData =
        ReweightableSpectrum::LoadFrom(dir->GetDirectory("OffAxisData"));
    pred->fHaveData = true;
  }

  if (dir->GetDirectory("OffAxisPrediction")) {
    pred->fOffAxisPrediction =
        PredictionInterp::LoadFrom(dir->GetDirectory("OffAxisPrediction"));
    pred->fHaveNDPred = true;
  }

  if (dir->GetDirectory("FarDetPrediction")) {
    pred->fFarDetPrediction =
        PredictionInterp::LoadFrom(dir->GetDirectory("FarDetPrediction"));
    pred->fHaveFDPred = true;
  }

  assert(pred->fHaveData || pred->fHaveNDPred);

  return pred;
}
} // namespace ana

#endif
