#pragma once

#include "CAFAna/Core/Spectrum.h"

#include "CAFAna/Experiment/IChiSqExperiment.h"

#include "CAFAna/Prediction/IPrediction.h"

namespace ana {
class SimpleChi2Experiment : public IChiSqExperiment {
public:
  SimpleChi2Experiment(
      IPrediction const *Pred, Spectrum const &Data, bool UseHistError = false,
      double POT = 0,
      std::pair<double, double> erange = {-std::numeric_limits<double>::max(),
                                          std::numeric_limits<double>::max()})
      : fPred(Pred), fUseHistError(UseHistError) {
    fPOT = POT;
    if (fPOT == 0) {
      fPOT = Data.POT();
    }
    fData = Data.ToTH1(fPOT);

    if (erange.first != -std::numeric_limits<double>::max()) {
      fBinRange.first = fData->GetXaxis()->FindFixBin(erange.first);
      if (fBinRange.first < 1) {
        fBinRange.first = 1;
      }
    } else {
      fBinRange.first = 1;
    }

    if (erange.second != std::numeric_limits<double>::max()) {
      fBinRange.second = fData->GetXaxis()->FindFixBin(erange.second);
      if (fBinRange.second > (fData->GetXaxis()->GetNbins() + 1)) {
        fBinRange.second = (fData->GetXaxis()->GetNbins() + 1);
      }
    } else {
      fBinRange.second = (fData->GetXaxis()->GetNbins() + 1);
    }
  }

  IPrediction const *fPred;
  TH1 const *fData;
  double fPOT;
  bool fUseHistError;

  std::pair<int, int> fBinRange;

  TH1D *GetPred(osc::IOscCalculatorAdjustable *osc,
                const SystShifts &syst = SystShifts::Nominal()) const {
    return fPred->PredictSyst(osc, syst).ToTH1(fPOT);
  }

  double ChiSq(osc::IOscCalculatorAdjustable *osc,
               const SystShifts &syst = SystShifts::Nominal()) const {

    TH1D *PredHist = GetPred(osc, syst);
    double chi2 = 0;
    for (int bi = fBinRange.first; bi < fBinRange.second; ++bi) {
      double pbc = PredHist->GetBinContent(bi + 1);
      double pbe =
          (fUseHistError ? pow(PredHist->GetBinError(bi + 1), 2) : 0) + pbc;

      if (!std::isnormal(pbe)) {
        continue;
      }

      double dbc = fData->GetBinContent(bi + 1);

      double contrib = pow((pbc - dbc), 2) / pbe;

      chi2 += contrib;
    }
    HistCache::Delete(PredHist);

    return chi2;
  }
};

class PRISMChi2Experiment : public IChiSqExperiment {
public:
  PRISMChi2Experiment(
      PredictionPRISM const *Pred, Spectrum const &Data,
      bool UseHistError = false, double POT = 0,
      PRISM::MatchChan match_chan = PRISM::kNumuDisappearance_Numode,
      std::pair<double, double> erange = {-std::numeric_limits<double>::max(),
                                          std::numeric_limits<double>::max()})
      : fPred(Pred), fUseHistError(UseHistError) {
    fPOT = POT;
    if (fPOT == 0) {
      fPOT = Data.POT();
    }
    fData = Data.ToTH1(fPOT);

    fMatchChannel = match_chan;

    if (erange.first != -std::numeric_limits<double>::max()) {
      fBinRange.first = fData->GetXaxis()->FindFixBin(erange.first);
      if (fBinRange.first < 1) {
        fBinRange.first = 1;
      }
    } else {
      fBinRange.first = 1;
    }

    if (erange.second != std::numeric_limits<double>::max()) {
      fBinRange.second = fData->GetXaxis()->FindFixBin(erange.second);
      if (fBinRange.second > (fData->GetXaxis()->GetNbins() + 1)) {
        fBinRange.second = (fData->GetXaxis()->GetNbins() + 1);
      }
    } else {
      fBinRange.second = (fData->GetXaxis()->GetNbins() + 1);
    }
  }

  PredictionPRISM const *fPred;
  TH1 const *fData;
  double fPOT;
  bool fUseHistError;

  PRISM::MatchChan fMatchChannel;
  std::pair<int, int> fBinRange;

  TH1D *GetPred(osc::IOscCalculatorAdjustable *osc,
                const SystShifts &syst = SystShifts::Nominal()) const {
    return fPred->PredictPRISMComponents(osc, syst, fMatchChannel)
        .at(PredictionPRISM::kPRISMPred)
        .ToTH1(fPOT);
  }

  double ChiSq(osc::IOscCalculatorAdjustable *osc,
               const SystShifts &syst = SystShifts::Nominal()) const {

    TH1D *PredHist = GetPred(osc, syst);

    double chi2 = 0;
    for (int bi = fBinRange.first; bi < fBinRange.second; ++bi) {
      double pbc = PredHist->GetBinContent(bi + 1);
      double pbe =
          (fUseHistError ? pow(PredHist->GetBinError(bi + 1), 2) : 0) + pbc;

      if (!std::isnormal(pbe)) {
        continue;
      }

      double dbc = fData->GetBinContent(bi + 1);

      double contrib = pow((pbc - dbc), 2) / pbe;

      chi2 += contrib;
    }
    HistCache::Delete(PredHist);

    return chi2;
  }
};
} // namespace ana