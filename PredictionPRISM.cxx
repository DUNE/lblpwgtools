#include "CAFAna/PRISM/PredictionPRISM.h"
#include "CAFAna/PRISM/PRISMUtils.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/OscCurve.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "OscLib/func/IOscCalculator.h"

#include "TAxis.h"
#include "TDirectory.h"
#include "TH2.h"
#include "TObjString.h"

namespace ana {

PredictionPRISM::PredictionPRISM(const HistAxis &recoAxis,
                                 const HistAxis &offAxis)
    : fOffAxisData(nullptr), fHaveData(false), fOffAxisPredGen(nullptr),
      fOffAxisPrediction(nullptr), fHaveNDPred(false), fNCCorrection(false),
      fWSBCorrection(false), fNueCorrection(false), fFDPredGen(nullptr),
      fFarDetPrediction(nullptr), fHaveFDPred(false), fFluxMatcher(nullptr),
      fNDFluxSpecies(PRISMExtrapolator::FluxPredSpecies::kNumu_numode),
      fFDFluxSpecies(PRISMExtrapolator::FluxPredSpecies::kNumu_numode),
      fPredictionAxis(recoAxis), fOffAxis(offAxis), fIgnoreData(false)

{
  std::vector<std::string> Labels = fPredictionAxis.GetLabels();
  std::vector<Binning> Bins = fPredictionAxis.GetBinnings();
  std::vector<Var> Vars = fPredictionAxis.GetVars();
  Labels.push_back(fOffAxis.GetLabels().front());
  Bins.push_back(fOffAxis.GetBinnings().front());
  Vars.push_back(fOffAxis.GetVars().front());
  fOffPredictionAxis = std::make_unique<HistAxis>(Labels, Bins, Vars);

  std::vector<double> offAxisBinEdges = fOffAxis.GetBinnings().front().Edges();

  fMaxOffAxis =
      (offAxisBinEdges.back() + offAxisBinEdges[offAxisBinEdges.size() - 2]) /
      2.0;

  // TODO we should make this correct every bin by its width, this currently
  // uses the second bin width only because a hack that uses the first bin to
  // mock up a special beam run
  double xslice_width_cm = (offAxisBinEdges[2] - offAxisBinEdges[1]) * 1E2;
  fDefaultOffAxisPOT = 1.0 / FD_ND_FVRatio(xslice_width_cm);

  DontAddDirectory guard;

  // Just using this to match binning
  osc::NoOscillations kNoOsc;
  const OscCurve curve(&kNoOsc, 14, 14);
  fFluxMissWeighter = std::unique_ptr<TH1>(curve.ToTH1());
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
      PredictPRISMComponents(calc, shift);

  assert(Comps.size());

  return Comps.at(kNDDataCorr);
}

void PredictionPRISM::InterpolateFluxMissMatcher() const {
  std::unique_ptr<TH1> match_residual = std::unique_ptr<TH1>(
      dynamic_cast<TH1 *>(fFluxMatcher->GetLastResidual()->Clone()));

  fFluxMissWeighter->Clear();

  double miss_Emax = match_residual->GetXaxis()->GetBinUpEdge(
      match_residual->GetXaxis()->GetNbins());

  bool done = false;
  for (int bin_it = 0; bin_it < fFluxMissWeighter->GetXaxis()->GetNbins();
       ++bin_it) {

    size_t nint_steps = 100;
    double bin_low_edge =
        fFluxMissWeighter->GetXaxis()->GetBinLowEdge(bin_it + 1);
    double bin_up_edge =
        fFluxMissWeighter->GetXaxis()->GetBinUpEdge(bin_it + 1);
    double step = (bin_up_edge - bin_low_edge) / double(nint_steps);
    double sum = 0;
    size_t s_it = 0;
    for (; s_it < nint_steps; ++s_it) {
      double E = bin_low_edge + double(s_it) * step;
      if (E > miss_Emax) {
        done = true;
        break;
      }
      sum += match_residual->Interpolate(E);
    }
    fFluxMissWeighter->SetBinContent(bin_it, sum / double(s_it));
    if (done) {
      break;
    }
  }
}

std::map<PredictionPRISM::PRISMComponent, Spectrum>
PredictionPRISM::PredictPRISMComponents(osc::IOscCalculator *calc,
                                        SystShifts shift) const {

  assert((fHaveData && (!fIgnoreData)) || fHaveNDPred);

  DontAddDirectory guard;

  // Using maps for non-default constructible classes is awful...
  std::map<PredictionPRISM::PRISMComponent, ReweightableSpectrum> NDComps;
  std::map<PredictionPRISM::PRISMComponent, Spectrum> Comps;

  bool SignalIsNumode = (static_cast<int>(fFDFluxSpecies) < 4);

  Sign::Sign_t SigSign = SignalIsNumode ? Sign::kNu : Sign::kAntiNu;
  Sign::Sign_t WrongSign = (!SignalIsNumode) ? Sign::kNu : Sign::kAntiNu;

  if (fOffAxisFakeData) {
    NDComps.emplace(kNDData, *fOffAxisFakeData);
  } else if (fHaveData && !fIgnoreData) {
    NDComps.emplace(kNDData, *fOffAxisData);
  }

  if (NDComps.count(kNDData)) {
    NDComps.emplace(kNDDataCorr2D, NDComps.at(kNDData));
  }

  if (fHaveNDPred) {

    Spectrum NDSig_spec = fOffAxisPrediction->PredictComponentSyst(
        calc, shift, Flavors::kAllNuMu, Current::kCC, SigSign);

    double NDPOT = NDSig_spec.POT();

    std::unique_ptr<TH2> NDSig_h(NDSig_spec.ToTH2(NDPOT));

    ReweightableSpectrum NDSig(fOffAxis.GetVars()[0], NDSig_h.get(),
                               fPredictionAxis.GetLabels(),
                               fPredictionAxis.GetBinnings(), 1, 1);

    NDComps.emplace(kNDSig, NDSig);

    if (fNCCorrection) {
      std::unique_ptr<TH2> NC_h(
          fOffAxisPrediction
              ->PredictComponentSyst(calc, shift, Flavors::kAll, Current::kNC,
                                     Sign::kBoth)
              .ToTH2(NDPOT));
      ReweightableSpectrum NC(fOffAxis.GetVars()[0], NC_h.get(),
                              fPredictionAxis.GetLabels(),
                              fPredictionAxis.GetBinnings(), 1, 1);

      NDComps.emplace(kNDNCBkg, NC);
      if (NDComps.count(kNDDataCorr2D)) {
        NDComps.at(kNDDataCorr2D) -= NDComps.at(kNDNCBkg);
      }
    }

    if (fNueCorrection) {
      std::unique_ptr<TH2> Nue_h(
          fOffAxisPrediction
              ->PredictComponentSyst(calc, shift, Flavors::kAllNuE,
                                     Current::kCC, Sign::kBoth)
              .ToTH2(NDPOT));
      ReweightableSpectrum Nue(fOffAxis.GetVars()[0], Nue_h.get(),
                               fPredictionAxis.GetLabels(),
                               fPredictionAxis.GetBinnings(), 1, 1);

      NDComps.emplace(kNDNueBkg, Nue);
      if (NDComps.count(kNDDataCorr2D)) {
        NDComps.at(kNDDataCorr2D) -= NDComps.at(kNDNueBkg);
      }
    }

    if (fWSBCorrection) {
      std::unique_ptr<TH2> WSB_h(
          fOffAxisPrediction
              ->PredictComponentSyst(calc, shift, Flavors::kAllNuMu,
                                     Current::kCC, WrongSign)
              .ToTH2(NDPOT));
      ReweightableSpectrum WSB(fOffAxis.GetVars()[0], WSB_h.get(),
                               fPredictionAxis.GetLabels(),
                               fPredictionAxis.GetBinnings(), 1, 1);

      NDComps.emplace(kNDWSBkg, WSB);
      if (NDComps.count(kNDDataCorr2D)) {
        NDComps.at(kNDDataCorr2D) -= NDComps.at(kNDWSBkg);
      }
    }

    // If you don't have a data prediction or you have fIgnoreData set, just use
    // the signal prediction.
    if (!NDComps.count(kNDDataCorr2D)) {
      NDComps.emplace(kNDDataCorr2D, NDSig);
    }
  }

  for (auto &NDC : NDComps) {
    NDC.second.OverridePOT(fDefaultOffAxisPOT);
  }

  if (fFluxMatcher) {
    TH1 const *LinearCombination = fFluxMatcher->GetMatchCoefficients(
        calc, fMaxOffAxis, fNDFluxSpecies, fFDFluxSpecies, shift);

    if (NDComps.count(kNDSig)) {
      Comps.emplace(kNDSig,
                    NDComps.at(kNDSig).WeightedByErrors(LinearCombination));
    }

    Comps.emplace(
        kNDDataCorr,
        NDComps.at(kNDDataCorr2D).WeightedByErrors(LinearCombination));

    // If we have the FD background predictions add them back in
    if (fHaveFDPred) {

      if (fNCCorrection) {
        Comps.emplace(kFDNCBkg, fFarDetPrediction->PredictComponentSyst(
                                    calc, shift, Flavors::kAll, Current::kNC,
                                    Sign::kBoth));
        Comps.at(kNDDataCorr) += Comps.at(kFDNCBkg);
      }

      if (fNueCorrection) {
        Comps.emplace(kFDNueBkg, fFarDetPrediction->PredictComponentSyst(
                                     calc, shift, Flavors::kAllNuE,
                                     Current::kCC, Sign::kBoth));
        Comps.at(kNDDataCorr) += Comps.at(kFDNueBkg);
      }

      if (fWSBCorrection) {
        Comps.emplace(kFDWSBkg, fFarDetPrediction->PredictComponentSyst(
                                    calc, shift, Flavors::kAllNuMu,
                                    Current::kCC, WrongSign));
        Comps.at(kNDDataCorr) += Comps.at(kFDWSBkg);
      }

      InterpolateFluxMissMatcher();

      fFarDetPrediction->GetPredNomAs<PredictionEnuWeightedNoExtrap>()
          ->SetEnuWeighting(fFluxMissWeighter.get());

      Comps.emplace(kFDFluxCorr,
                    fFarDetPrediction->PredictComponentSyst(
                        calc, shift, Flavors::kAllNuMu, Current::kCC, SigSign));

      Comps.at(kNDDataCorr) += Comps.at(kFDFluxCorr);

      fFarDetPrediction->GetPredNomAs<PredictionEnuWeightedNoExtrap>()
          ->UnSetEnuWeighting();

      for (auto &cmp : Comps) { // Set these to /POT for combination
        cmp.second.ScaleToPOT(1);
      }
    }
  }

  for (auto const &NDC :
       NDComps) { // If you haven't been added, project to a 2D spectrum
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

void PredictionPRISM::SetFakeDataShift(SystShifts s) {
  fHaveFakeData = true;

  if (!fHaveNDPred) {
    std::cout << "[ERROR]: Attempting to build fake data without an available "
                 "ND MC PredictionInterp."
              << std::endl;
    throw;
  }

  DontAddDirectory guard;
  bool SignalIsNumode = (static_cast<int>(fFDFluxSpecies) < 4);

  Sign::Sign_t SigSign = SignalIsNumode ? Sign::kNu : Sign::kAntiNu;

  osc::NoOscillations noosc;

  Spectrum NDSig_spec = fOffAxisPrediction->PredictComponentSyst(
      &noosc, s, Flavors::kAllNuMu, Current::kCC, SigSign);

  double NDPOT = NDSig_spec.POT();

  std::unique_ptr<TH2> NDSig_h(NDSig_spec.ToTH2(NDPOT));

  fOffAxisFakeData = std::make_unique<ReweightableSpectrum>(
      fOffAxis.GetVars()[0], NDSig_h.get(), fPredictionAxis.GetLabels(),
      fPredictionAxis.GetBinnings(), 1, 1);
}

void PredictionPRISM::UnsetFakeDataShift() { fHaveFakeData = false; }

} // namespace ana
