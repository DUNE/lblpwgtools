#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/Prediction/PredictionNoOsc.h"

#include "fhiclcpp/ParameterSet.h"
#include "fhiclcpp/make_ParameterSet.h"

using namespace ana;
using namespace PRISM;

void OffAxisEventRates(fhicl::ParameterSet const &pred) {

  std::string const &off_axis_file = pred.get<std::string>("input_file");
  std::vector<std::string> const &output_file =
      pred.get<std::vector<std::string>>("output_file");
  std::string const &output_dir = pred.get<std::string>("output_dir", "");
  std::string const &varname =
      pred.get<std::string>("projection_name", "EProxy");
  auto SpecialRunWeightHist = pred.get<std::pair<std::string, std::string>>(
      "special_run_weight", {"", ""});

  TH1 *whist = nullptr;
  if (SpecialRunWeightHist.first.size()) {
    TDirectory *gdc = gDirectory;
    TFile wf(SpecialRunWeightHist.first.c_str());
    wf.GetObject(SpecialRunWeightHist.second.c_str(), whist);
    if (!whist) {
      std::cout << "[ERROR]: Failed to read " << SpecialRunWeightHist.second
                << " from " << SpecialRunWeightHist.first << std::endl;
      throw;
    }
    whist->SetDirectory(nullptr);
    if (gdc) {
      gdc->cd();
    }
  }

  osc::IOscCalculatorAdjustable *calc = NuFitOscCalc(1);

  Loaders TheLoaders;

  SpectrumLoader NDLoader(off_axis_file, kBeam);
  TheLoaders.AddLoader(&NDLoader, caf::kNEARDET, Loaders::kMC);

  PRISMAxisBlob axes =
      GetPRISMAxes(varname, "testopt", bool(whist) ? "OneNegXBin" : "default");

  Cut NDSignalCut = GetNDSignalCut(false);
  Cut NDSignalCut_sel = GetNDSignalCut(true);
  Cut NDSignalCut_sel_signal = GetNDSignalCut(true) && kIsNumuCC;
  Cut NDSignalCut_sel_WSB = GetNDSignalCut(true) && kIsAntiNu && !kIsNC;
  Cut NDSignalCut_sel_NC = GetNDSignalCut(true) && kIsNC;
  Var NDWeight = GetNDWeight();
  Var NDWeight_XSec = GetNDWeight("CVXSec");
  Var NDWeight_Eff = GetNDWeight("NDEff");
  Var NDWeight_XSecEff = GetNDWeight("CVXSec NDEff");

  if (whist) { // If we are faking a special run
    auto specrunweight =
        ana::Var({}, [&](const caf::StandardRecord *sr) -> double {
          if (sr->dune.det_x || (sr->dune.vtx_x > 0)) {
            return 1;
          }
          return whist->GetBinContent(
              whist->GetXaxis()->FindFixBin(sr->dune.Ev));
        });
    NDWeight = NDWeight * specrunweight;
    NDWeight_XSec = NDWeight_XSec * specrunweight;
    NDWeight_Eff = NDWeight_Eff * specrunweight;
    NDWeight_XSecEff = NDWeight_XSecEff * specrunweight;
  }

  Spectrum NDSpect_all(NDLoader, axes.XProjection, axes.OffAxisPosition,
                       kNoCut);
  Spectrum NDSpect_signal(NDLoader, axes.XProjection, axes.OffAxisPosition,
                          NDSignalCut, kNoShift);
  Spectrum NDSpect_signal_AnaWeighted(NDLoader, axes.XProjection,
                                      axes.OffAxisPosition, NDSignalCut,
                                      kNoShift, NDWeight);
  Spectrum NDSpect_signal_CVXSecWeighted(NDLoader, axes.XProjection,
                                         axes.OffAxisPosition, NDSignalCut,
                                         kNoShift, NDWeight_XSec);
  Spectrum NDSpect_signal_EffWeighted(NDLoader, axes.XProjection,
                                      axes.OffAxisPosition, NDSignalCut,
                                      kNoShift, NDWeight_Eff);
  Spectrum NDSpect_signal_AllWeighted(NDLoader, axes.XProjection,
                                      axes.OffAxisPosition, NDSignalCut,
                                      kNoShift, NDWeight_XSecEff);

  Spectrum NDSpect_select(NDLoader, axes.XProjection, axes.OffAxisPosition,
                          NDSignalCut_sel, kNoShift);
  Spectrum NDSpect_select_AnaWeighted(NDLoader, axes.XProjection,
                                      axes.OffAxisPosition, NDSignalCut_sel,
                                      kNoShift, NDWeight);
  Spectrum NDSpect_select_AnaWeighted_Sig(
      NDLoader, axes.XProjection, axes.OffAxisPosition, NDSignalCut_sel_signal,
      kNoShift, NDWeight);
  Spectrum NDSpect_select_AnaWeighted_WSB(
      NDLoader, axes.XProjection, axes.OffAxisPosition, NDSignalCut_sel_WSB,
      kNoShift, NDWeight);
  Spectrum NDSpect_select_AnaWeighted_NC(
      NDLoader, axes.XProjection, axes.OffAxisPosition, NDSignalCut_sel_NC,
      kNoShift, NDWeight);
  Spectrum NDSpect_select_CVXSecWeighted(NDLoader, axes.XProjection,
                                         axes.OffAxisPosition, NDSignalCut_sel,
                                         kNoShift, NDWeight_XSec);
  Spectrum NDSpect_select_CVXSecWeighted_Sig(
      NDLoader, axes.XProjection, axes.OffAxisPosition, NDSignalCut_sel_signal,
      kNoShift, NDWeight_XSec);
  Spectrum NDSpect_select_CVXSecWeighted_WSB(
      NDLoader, axes.XProjection, axes.OffAxisPosition, NDSignalCut_sel_WSB,
      kNoShift, NDWeight_XSec);
  Spectrum NDSpect_select_CVXSecWeighted_NC(
      NDLoader, axes.XProjection, axes.OffAxisPosition, NDSignalCut_sel_NC,
      kNoShift, NDWeight_XSec);

  TheLoaders.Go();

  TFile f(output_file.front().c_str(),
          output_file.size() > 1 ? output_file[1].c_str() : "UPDATE");

  TH1 *NearDet_all = NDSpect_all.ToTHX(POT120);
  NearDet_all->SetTitle(";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_all, "NearDet_all");

  TH1 *NearDet_signal = NDSpect_signal.ToTHX(POT120);
  NearDet_signal->SetTitle(";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_signal, "NearDet_signal");

  TH1 *NearDet_signal_AnaWeighted = NDSpect_signal_AnaWeighted.ToTHX(POT120);
  NearDet_signal_AnaWeighted->SetTitle(
      ";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_signal_AnaWeighted, "NearDet_signal_AnaWeighted");

  TH1 *NearDet_signal_CVXSecWeighted =
      NDSpect_signal_CVXSecWeighted.ToTHX(POT120);
  NearDet_signal_CVXSecWeighted->SetTitle(
      ";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_signal_CVXSecWeighted,
                 "NearDet_signal_CVXSecWeighted");

  TH1 *NearDet_signal_EffWeighted = NDSpect_signal_EffWeighted.ToTHX(POT120);
  NearDet_signal_EffWeighted->SetTitle(
      ";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_signal_EffWeighted, "NearDet_signal_EffWeighted");

  TH1 *NearDet_signal_AllWeighted = NDSpect_signal_AllWeighted.ToTHX(POT120);
  NearDet_signal_AllWeighted->SetTitle(
      ";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_signal_AllWeighted, "NearDet_signal_AllWeighted");

  TH1 *NearDet_select = NDSpect_select.ToTHX(POT120);
  NearDet_select->SetTitle(";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_select, "NearDet_select");

  TH1 *NearDet_select_AnaWeighted = NDSpect_select_AnaWeighted.ToTHX(POT120);
  NearDet_select_AnaWeighted->SetTitle(
      ";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_select_AnaWeighted, "NearDet_select_AnaWeighted");

  TH1 *NearDet_select_AnaWeighted_Sig =
      NDSpect_select_AnaWeighted_Sig.ToTHX(POT120);
  NearDet_select_AnaWeighted_Sig->SetTitle(
      ";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_select_AnaWeighted_Sig,
                 "NearDet_select_AnaWeighted_Sig");

  TH1 *NearDet_select_AnaWeighted_WSB =
      NDSpect_select_AnaWeighted_WSB.ToTHX(POT120);
  NearDet_select_AnaWeighted_WSB->SetTitle(
      ";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_select_AnaWeighted_WSB,
                 "NearDet_select_AnaWeighted_WSB");

  TH1 *NearDet_select_AnaWeighted_NC =
      NDSpect_select_AnaWeighted_NC.ToTHX(POT120);
  NearDet_select_AnaWeighted_NC->SetTitle(
      ";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_select_AnaWeighted_NC,
                 "NearDet_select_AnaWeighted_NC");

  TH1 *NearDet_select_CVXSecWeighted =
      NDSpect_select_CVXSecWeighted.ToTHX(POT120);
  NearDet_select_CVXSecWeighted->SetTitle(
      ";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_select_CVXSecWeighted,
                 "NearDet_select_CVXSecWeighted");

  TH1 *NearDet_select_CVXSecWeighted_Sig =
      NDSpect_select_CVXSecWeighted_Sig.ToTHX(POT120);
  NearDet_select_CVXSecWeighted_Sig->SetTitle(
      ";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_select_CVXSecWeighted_Sig,
                 "NearDet_select_CVXSecWeighted_Sig");

  TH1 *NearDet_select_CVXSecWeighted_WSB =
      NDSpect_select_CVXSecWeighted_WSB.ToTHX(POT120);
  NearDet_select_CVXSecWeighted_WSB->SetTitle(
      ";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_select_CVXSecWeighted_WSB,
                 "NearDet_select_CVXSecWeighted_WSB");

  TH1 *NearDet_select_CVXSecWeighted_NC =
      NDSpect_select_CVXSecWeighted_NC.ToTHX(POT120);
  NearDet_select_CVXSecWeighted_NC->SetTitle(
      ";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_select_CVXSecWeighted_NC,
                 "NearDet_select_CVXSecWeighted_NC");

  f.Write();
}

#ifndef __CINT__
int main(int argc, char const *argv[]) {
  // Make sure systs are applied to ND distributions which are per 1 POT.
  setenv("CAFANA_PRED_MINMCSTATS", "0", 1);
  gROOT->SetMustClean(false);

  if (argc != 2) {
    std::cout << "[ERROR]: Expected to be passed the location of a single "
                 "configuration FHiCL file."
              << std::endl;
    return 1;
  }

  fhicl::ParameterSet const &ps = fhicl::make_ParameterSet(argv[1]);

  for (fhicl::ParameterSet const &offaxisevrt :
       ps.get<std::vector<fhicl::ParameterSet>>("PRISMEventRates")) {
    OffAxisEventRates(offaxisevrt);
  }
}
#endif
