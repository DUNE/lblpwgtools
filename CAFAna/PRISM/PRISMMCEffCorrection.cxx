#include "CAFAna/PRISM/PRISMUtils.h"

//#include "CAFAna/PRISM/PredictionPRISM.h"
//#include "CAFAna/PRISM/EigenUtils.h"
#include "CAFAna/PRISM/PRISMMCEffCorrection.h"

//#include "CAFAna/Analysis/CalcsNuFit.h"
//#include "CAFAna/Analysis/common_fit_definitions.h"
//#include "CAFAna/Analysis/AnalysisVars.h"

//#include "CAFAna/Cuts/TruthCuts.h"

using namespace PRISM;

namespace ana {

//----------------------------------------------------
// Class for ND and FD MC based efficiency correction:
//----------------------------------------------------

MCEffCorrection::MCEffCorrection(PredictionInterp const * NDunsel_293kA,
                                 PredictionInterp const * NDsel_293kA,
                                 PredictionInterp const * NDunsel_280kA,
                                 PredictionInterp const * NDsel_280kA,
                                 PredictionInterp const * FDunsel,
                                 PredictionInterp const * FDsel) : 
                                 hNDunselected_293kA(nullptr), hNDselected_293kA(nullptr),
                                 hNDunselected_280kA(nullptr), hNDselected_280kA(nullptr),
                                 hFDunselected(nullptr), hFDselected(nullptr), 
                                 fDoneOnce(false) {
  fNDunselected_293kA = NDunsel_293kA;
  fNDselected_293kA = NDsel_293kA;
  fNDunselected_280kA = NDunsel_280kA;
  fNDselected_280kA = NDsel_280kA;
  fFDunselected = FDunsel;
  fFDselected = FDsel;
}

//----------------------------------------------------

MCEffCorrection::MCEffCorrection(const MCEffCorrection &EffCorr) :
                                 hNDunselected_293kA(nullptr), 
                                 hNDselected_293kA(nullptr), 
                                 hNDunselected_280kA(nullptr), 
                                 hNDselected_280kA(nullptr), 
                                 hFDunselected(nullptr), 
                                 hFDselected(nullptr),  
                                 fDoneOnce(EffCorr.fDoneOnce) {

  fNDunselected_293kA = EffCorr.fNDunselected_293kA;
  fNDselected_293kA = EffCorr.fNDselected_293kA;
  fNDunselected_280kA = EffCorr.fNDunselected_280kA;
  fNDselected_280kA = EffCorr.fNDselected_280kA;
  fFDunselected = EffCorr.fFDunselected;
  fFDselected = EffCorr.fFDselected;
}

//----------------------------------------------------

MCEffCorrection::~MCEffCorrection() {
  //HistCache::Delete(hNDunselected_293kA);
  //HistCache::Delete(hNDselected_293kA);
  //HistCache::Delete(hNDunselected_280kA);
  //HistCache::Delete(hNDselected_280kA);
  //HistCache::Delete(hFDunselected);
  //HistCache::Delete(hFDselected);
}

//----------------------------------------------------

void MCEffCorrection::CalcEfficiency(osc::IOscCalc *calc,
                                     HistAxis const &axis,
                                     ana::SystShifts shift,
                                     Flavors::Flavors_t NDflav,
                                     Flavors::Flavors_t FDflav,
                                     Current::Current_t curr,
                                     Sign::Sign_t NDsign, 
                                     Sign::Sign_t FDsign) const {

  ana::SystShifts syst = shift; 

  if (!fNDunselected_293kA || !fNDselected_293kA || !fNDunselected_280kA || !fNDselected_280kA) {
    std::cout << "[WARNING] No NDunselected and/or NDselected Pred" << std::endl;
    abort();
  }

  auto sNDunselected_293kA = fNDunselected_293kA->PredictComponentSyst(calc, syst, 
                                                                       NDflav, curr, NDsign);
  auto sNDunselected_280kA = fNDunselected_280kA->PredictComponentSyst(calc, syst,
                                                                       NDflav, curr, NDsign);

  // True axis binning might be different to reco binning in fAnalysisAxis.
  // Make sure bins in true energy efficiency correction matches
  std::vector<std::string> Labels = sNDunselected_293kA.GetLabels();
  Labels.pop_back(); // Remove off-axis position axis
  std::vector<Binning> Bins = sNDunselected_293kA.GetBinnings();
  Bins.pop_back();
  // This is annoying, true axis will have a reco label. I can live with this though.
  std::vector<Var> Vars = axis.GetVars();
  HistAxis trueaxis(Labels, Bins, Vars);

  // Analysis axis could be 2D, so put into RWSpec so we can have it projected into 1D trueaxis
  ReweightableSpectrum rwsNDunselected_293kA = ToReweightableSpectrum(sNDunselected_293kA, 
                                                                      1, trueaxis);
  ReweightableSpectrum rwsNDunselected_280kA = ToReweightableSpectrum(sNDunselected_280kA,
                                                                      1, trueaxis);
  std::unique_ptr<TH2> NDunsel_293kA = std::unique_ptr<TH2>(rwsNDunselected_293kA.ToTH2(1));
  std::unique_ptr<TH2> NDunsel_280kA = std::unique_ptr<TH2>(rwsNDunselected_280kA.ToTH2(1));  
  
  // Selected ND MC
  auto sNDselected_293kA = fNDselected_293kA->PredictComponentSyst(calc, syst, 
                                                                   NDflav, curr, NDsign);
  auto sNDselected_280kA = fNDselected_280kA->PredictComponentSyst(calc, syst, 
                                                                   NDflav, curr, NDsign);
  ReweightableSpectrum rwsNDselected_293kA = ToReweightableSpectrum(sNDselected_293kA, 
                                                                    1, trueaxis);
  ReweightableSpectrum rwsNDselected_280kA = ToReweightableSpectrum(sNDselected_280kA, 
                                                                    1, trueaxis);
  std::unique_ptr<TH2> NDsel_293kA = std::unique_ptr<TH2>(rwsNDselected_293kA.ToTH2(1));
  std::unique_ptr<TH2> NDsel_280kA = std::unique_ptr<TH2>(rwsNDselected_280kA.ToTH2(1));

  if (!fFDunselected || !fFDselected) {
    std::cout << "[WARNING] No FDunselected and or FDselected Pred" << std::endl; 
    abort();
  }
  // FD unselected
  auto sFDunselected = fFDunselected->PredictComponentSyst(calc, syst, FDflav, curr, FDsign);
  std::unique_ptr<TH1> FDunsel = std::unique_ptr<TH1>(sFDunselected.ToTH1(1));
  // FD selected
  auto sFDselected = fFDselected->PredictComponentSyst(calc, syst, FDflav, curr, FDsign);
  std::unique_ptr<TH1> FDsel = std::unique_ptr<TH1>(sFDselected.ToTH1(1));

  // Calculate ND efficiency
  // efficiency fluctuates slightly with OA position
  // Efficiency for 293kA sampple:
  for (int slice = 0; slice < NDsel_293kA->GetYaxis()->GetNbins(); slice++) { // OA slice
    std::vector<double> SliceEfficiency;
    for (int ebin = 0; ebin < NDsel_293kA->GetXaxis()->GetNbins(); ebin++) { // Energy bins
      double NDbin_eff = NDsel_293kA->GetBinContent(ebin + 1, slice + 1) /
                         NDunsel_293kA->GetBinContent(ebin + 1, slice + 1);
      if (std::isnormal(NDbin_eff)) {
        SliceEfficiency.push_back(NDbin_eff);
      } else {
        SliceEfficiency.push_back(1E-6); // arbitrarily small instead of 0
      }
    }
    NDefficiency_293kA.push_back(SliceEfficiency);
  }

  // Efficiency for 280kA sample:
  for (int slice = 0; slice < NDsel_280kA->GetYaxis()->GetNbins(); slice++) { // OA slice
    std::vector<double> SliceEfficiency;
    for (int ebin = 0; ebin < NDsel_280kA->GetXaxis()->GetNbins(); ebin++) { // Energy bins
      double NDbin_eff = NDsel_280kA->GetBinContent(ebin + 1, slice + 1) /
                         NDunsel_280kA->GetBinContent(ebin + 1, slice + 1);
      if (std::isnormal(NDbin_eff)) {
        SliceEfficiency.push_back(NDbin_eff);
      } else {
        SliceEfficiency.push_back(1E-6); // arbitrarily small instead of 0 
      }
    }
    NDefficiency_280kA.push_back(SliceEfficiency);
  }

  // Calculate FD efficiency
  for (int ebin = 0; ebin < FDsel->GetXaxis()->GetNbins(); ebin++) {
    double FDbin_eff = FDsel->GetBinContent(ebin + 1) / 
                       FDunsel->GetBinContent(ebin + 1);
    if (std::isnormal(FDbin_eff)) {
      FDefficiency.push_back(FDbin_eff);
    } else {
      FDefficiency.push_back(1E-6);
    }
  }

  // Don't want to keep saving the same histograms.
  // Asign the histograms once for writing.
  if (!fDoneOnce) {
    hNDunselected_293kA = std::move(NDunsel_293kA);
    hNDselected_293kA = std::move(NDsel_293kA);
    hNDunselected_280kA = std::move(NDunsel_280kA);
    hNDselected_280kA = std::move(NDsel_280kA);
    hFDunselected = std::move(FDunsel);
    hFDselected = std::move(FDsel); 
  }
  fDoneOnce = true;

  // Delete histograms to clear up the cache.
  //HistCache::Delete(NDunsel_293kA);
  //HistCache::Delete(NDsel_293kA);
  //HistCache::Delete(NDunsel_280kA);
  //HistCache::Delete(NDsel_280kA);
  //HistCache::Delete(FDunsel);
  //HistCache::Delete(FDsel);
}

//-----------------------------------------------------------

void MCEffCorrection::Write(TDirectory *dir) const {
  dir->WriteTObject(hNDunselected_293kA.get(), "NDUnselected_293kA");
  dir->WriteTObject(hNDselected_293kA.get(), "NDSelected_293kA");
  dir->WriteTObject(hNDunselected_280kA.get(), "NDUnselected_280kA");
  dir->WriteTObject(hNDselected_280kA.get(), "NDSelected_280kA");
  dir->WriteTObject(hFDunselected.get(), "FDUnselected");
  dir->WriteTObject(hFDselected.get(), "FDSelected"); 
}

//-----------------------------------------------------------

} // namespace ana
