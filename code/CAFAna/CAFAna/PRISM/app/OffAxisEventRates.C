#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/Prediction/PredictionNoOsc.h"

using namespace ana;
using namespace PRISM;

void OffAxisEventRates(std::string const &off_axis_file,
                       std::string const &output_file) {

  osc::IOscCalculatorAdjustable *calc = NuFitOscCalc(1);

  Loaders TheLoaders;

  SpectrumLoader NDLoader(off_axis_file, kBeam);
  TheLoaders.AddLoader(&NDLoader, caf::kNEARDET, Loaders::kMC);

  PRISMAxisBlob axes = GetPRISMAxes("ETrue");

  Cut NDSignalCut = GetNDSignalCut();
  Var NDWeight = GetNDWeight();
  Var NDWeight_XSec = GetNDWeight("CVXSec");
  Var NDWeight_Eff = GetNDWeight("Eff");
  Var NDWeight_XSecEff = GetNDWeight("CVXSec Eff");

  Spectrum NDSpect_all(NDLoader, axes.XProjection, axes.OffAxisPosition,
                       kNoCut);
  Spectrum NDSpect_Selected(NDLoader, axes.XProjection, axes.OffAxisPosition,
                            NDSignalCut, kNoShift);
  Spectrum NDSpect_AnaWeighted(NDLoader, axes.XProjection, axes.OffAxisPosition,
                               NDSignalCut, kNoShift, NDWeight);
  Spectrum NDSpect_CVXSecWeighted(NDLoader, axes.XProjection,
                                  axes.OffAxisPosition, NDSignalCut, kNoShift,
                                  NDWeight_XSec);
  Spectrum NDSpect_EffWeighted(NDLoader, axes.XProjection, axes.OffAxisPosition,
                               NDSignalCut, kNoShift, NDWeight_Eff);
  Spectrum NDSpect_AllWeighted(NDLoader, axes.XProjection, axes.OffAxisPosition,
                               NDSignalCut, kNoShift, NDWeight_XSecEff);

  TheLoaders.Go();

  TFile f(output_file.c_str(), "RECREATE");

  TH1 *NearDet_all = NDSpect_all.ToTHX(POT120);
  NearDet_all->SetTitle("OffAxis;ND EvRate");
  f.WriteTObject(NearDet_all, "NearDet_all");

  TH1 *NearDet_Selected = NDSpect_Selected.ToTHX(POT120);
  NearDet_Selected->SetTitle("OffAxis;ND EvRate");
  f.WriteTObject(NearDet_Selected, "NearDet_Selected");

  TH1 *NearDet_AnaWeighted = NDSpect_AnaWeighted.ToTHX(POT120);
  NearDet_AnaWeighted->SetTitle("OffAxis;ND EvRate");
  f.WriteTObject(NearDet_AnaWeighted, "NearDet_AnaWeighted");

  TH1 *NearDet_CVXSecWeighted = NDSpect_CVXSecWeighted.ToTHX(POT120);
  NearDet_CVXSecWeighted->SetTitle("OffAxis;ND EvRate");
  f.WriteTObject(NearDet_CVXSecWeighted, "NearDet_CVXSecWeighted");

  TH1 *NearDet_EffWeighted = NDSpect_EffWeighted.ToTHX(POT120);
  NearDet_EffWeighted->SetTitle("OffAxis;ND EvRate");
  f.WriteTObject(NearDet_EffWeighted, "NearDet_EffWeighted");

  TH1 *NearDet_AllWeighted = NDSpect_AllWeighted.ToTHX(POT120);
  NearDet_AllWeighted->SetTitle("OffAxis;ND EvRate");
  f.WriteTObject(NearDet_AllWeighted, "NearDet_AllWeighted");

  f.Write();
}

#ifndef __CINT__
int main(int argc, char const *argv[]) { OffAxisEventRates(argv[1], argv[2]); }
#endif
