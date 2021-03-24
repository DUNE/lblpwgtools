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

  osc::IOscCalculatorAdjustable *calc = NuFitOscCalc(1);

  Loaders TheLoaders;

  SpectrumLoader NDLoader(off_axis_file, kBeam);
  TheLoaders.AddLoader(&NDLoader, caf::kNEARDET, Loaders::kMC);

  PRISMAxisBlob axes = GetPRISMAxes(varname, "testopt", "default");

  Cut NDSignalCut = GetNDSignalCut_basis(false); //eran	
  Cut NDSignalCut_sel = GetNDSignalCut_basis(true);
  Cut NDSignalCut_sel_signal = GetNDSignalCut_basis(true) && kIsNumuCC;
  Cut NDSignalCut_sel_WSB = GetNDSignalCut_basis(true) && kIsAntiNu && !kIsNC;
  Cut NDSignalCut_sel_NC = GetNDSignalCut_basis(true) && kIsNC;

  Cut NDSignalCut_SpecRun = NDSignalCut && kSel280kARun;
  Cut NDSignalCut_SpecRun_sel = NDSignalCut_sel && kSel280kARun;
  Cut NDSignalCut_SpecRun_sel_signal = NDSignalCut_sel_signal && kSel280kARun;
  Cut NDSignalCut_SpecRun_sel_WSB = NDSignalCut_sel_WSB && kSel280kARun;
  Cut NDSignalCut_SpecRun_sel_NC = NDSignalCut_sel_NC && kSel280kARun;

  NDSignalCut = NDSignalCut && kCut280kARun;
  NDSignalCut_sel = NDSignalCut_sel && kCut280kARun;
  NDSignalCut_sel_signal = NDSignalCut_sel_signal && kCut280kARun;
  NDSignalCut_sel_WSB = NDSignalCut_sel_WSB && kCut280kARun;
  NDSignalCut_sel_NC = NDSignalCut_sel_NC && kCut280kARun;

  Var NDWeight = GetNDWeight();
  Var NDWeight_XSec = GetNDWeight("CVXSec");
  Var NDWeight_Eff = GetNDWeight("NDEff");
  Var NDWeight_XSecEff = GetNDWeight("CVXSec NDEff");

  Spectrum NDSpect_all(NDLoader, axes.XProjection, axes.OffAxisPosition,
                       kCut280kARun);
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

  Spectrum NDSpect_SpecRun_all(NDLoader, axes.XProjection, axes.OffAxisPosition,
                               kSel280kARun);
  Spectrum NDSpect_SpecRun_signal(NDLoader, axes.XProjection,
                                  axes.OffAxisPosition, NDSignalCut_SpecRun,
                                  kNoShift);
  Spectrum NDSpect_SpecRun_signal_AnaWeighted(
      NDLoader, axes.XProjection, axes.OffAxisPosition, NDSignalCut_SpecRun,
      kNoShift, NDWeight);

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

  TH1 *NearDet_SpecRun_all = NDSpect_SpecRun_all.ToTHX(POT120);
  NearDet_SpecRun_all->SetTitle(";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_SpecRun_all, "NearDet_SpecRun_all");

  TH1 *NearDet_SpecRun_signal = NDSpect_SpecRun_signal.ToTHX(POT120);
  NearDet_SpecRun_signal->SetTitle(
      ";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_SpecRun_signal, "NearDet_SpecRun_signal");

  TH1 *NearDet_SpecRun_signal_AnaWeighted =
      NDSpect_SpecRun_signal_AnaWeighted.ToTHX(POT120);
  NearDet_SpecRun_signal_AnaWeighted->SetTitle(
      ";EProxy (GeV);OffAxis Position (m);ND EvRate");
  f.WriteTObject(NearDet_SpecRun_signal_AnaWeighted,
                 "NearDet_SpecRun_signal_AnaWeighted");

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
