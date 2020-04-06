#include "CAFAna/Analysis/AnalysisBinnings.h"
#include "CAFAna/Analysis/AnalysisVersion.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Vars/Vars.h"

#include <vector>

using namespace ana;

AxisBlob GetAxisBlob(std::string const &blob_name) {

  //Should all be statically initialized on first method call to ensure that the vars have been initialized beforehand
  // Axes
  static HistAxis const axRecoEnuFDnumu("Reco energy (GeV)", binsFDEreco,
                                             kRecoE_numu);
  static HistAxis const axRecoEnuFDnue("Reco energy (GeV)", binsFDEreco,
                                            kRecoE_nue);
  static HistAxis const axErecYrecND("Reco energy (GeV)", binsNDEreco,
                                          kRecoEnergyND, "y_{rec}", binsY,
                                          kRecoYND);
  static HistAxis const axRecoEnuFDnumu_v3("Reco energy (GeV)",
                                                binsFDEreco, kRecoE_numu);
  static HistAxis const axRecoEnuFDnue_v3("Reco energy (GeV)",
                                               binsFDEreco_v3, kRecoE_nue);
  static HistAxis const axErecYrecND_v3("Reco energy (GeV)",
                                             binsNDEreco_v3, kRecoEnergyND,
                                             "y_{rec}", binsY_v3, kRecoYND);
  static HistAxis const axErecND("Reco energy (GeV)", binsNDEreco,
                                      kRecoEnergyND);

  static HistAxis const axErecFD_FromDep("Reco energy (GeV)", binsFDEreco,
                                              kRecoE_FromDep);
  static HistAxis const axErecYrecND_FromDep(
      "Reco energy (GeV)", binsNDEreco, kRecoE_FromDep, "y_{rec}", binsY,
      kRecoY_FromDep);
  static HistAxis const axErecYrecNDOA_FromDep(
      "Reco energy (GeV)", binsNDEreco_OA, kRecoE_FromDep, "y_{rec}", binsY,
      kRecoY_FromDep);
  static HistAxis const axErecND_FromDep("#it{E}_{#nu}^{Rec.} (GeV)",
                                              binsNDEreco, kRecoE_FromDep);

  static HistAxis const axTrueE_unibin("#it{E}_{#nu} (GeV)", binsETrue,
                                            kTrueEnergy);

  static HistAxis const axTrueE_unibin_coarse(
      "#it{E}_{#nu} (GeV)", binsETrue_Coarse, kTrueEnergy);

  static HistAxis const axErec_FromDep_unibin("#it{E}_{#nu}^{Rec.} (GeV)",
                                                   binsETrue, kRecoE_FromDep);
  static HistAxis const axErecND_unibin("#it{E}_{#nu}^{Rec.} (GeV)",
                                             binsETrue, kRecoEnergyND);
  static HistAxis const axRecoEnuFDnumu_unibin("Reco energy (GeV)",
                                                    binsETrue, kRecoE_numu);
  static HistAxis const axRecoEnuFDnue_unibin("Reco energy (GeV)",
                                                   binsETrue, kRecoE_nue);

  static HistAxis const axErecND_coarsebin(
      "Reco energy (GeV)", binsEreco_Coarse, kRecoEnergyND);
  static HistAxis const axRecoEnuFDnumu_coarsebin(
      "Reco energy (GeV)", binsEreco_Coarse, kRecoE_numu);

  static HistAxis const axRecoEnuFDnue_coarsebin(
      "Reco energy (GeV)", binsEreco_Coarse, kRecoE_nue);

  static HistAxis const axErecND_verycoarsebin(
      "Reco energy (GeV)", binsEreco_VeryCoarse, kRecoEnergyND);
  static HistAxis const axRecoEnuFDnumu_verycoarsebin(
      "Reco energy (GeV)", binsEreco_VeryCoarse, kRecoE_numu);

  static HistAxis const axRecoEnuFDnue_verycoarsebin(
      "Reco energy (GeV)", binsEreco_VeryCoarse, kRecoE_nue);

  static HistAxis const axErecND_onebin("Reco energy (GeV)", onebin,
                                             kRecoEnergyND);
  static HistAxis const axRecoEnuFDnumu_onebin("Reco energy (GeV)", onebin,
                                                    kRecoE_numu);

  static HistAxis const axRecoEnuFDnue_onebin("Reco energy (GeV)", onebin,
                                                   kRecoE_nue);

  static AxisBlob const default_axes_v4{&axErecYrecND, &axRecoEnuFDnumu,
                                        &axRecoEnuFDnue};
  static AxisBlob const default_axes_v3{&axErecYrecND_v3, &axRecoEnuFDnumu_v3,
                                        &axRecoEnuFDnue_v3};
  static AxisBlob const fake_data_axes{&axErecYrecND_FromDep, &axErecFD_FromDep,
                                       &axErecFD_FromDep};

  static AxisBlob const Ax1DND_unibin{&axErecND_unibin, &axRecoEnuFDnumu_unibin,
                                      &axRecoEnuFDnue_unibin};
  static AxisBlob const Ax1DND_FromDep_unibin{
      &axErec_FromDep_unibin, &axErec_FromDep_unibin, &axErec_FromDep_unibin};

  AxisBlob AxToUse =
      (GetAnaVersion() == kV4) ? default_axes_v4 : default_axes_v3;
  if (blob_name == "1DND") {
    AxToUse.NDAx = &axErecND;
  } else if (blob_name == "FromDep") {
    AxToUse.NDAx = &axErecYrecND_FromDep;
    AxToUse.FDAx_numu = &axErecFD_FromDep;
    AxToUse.FDAx_nue = &axErecFD_FromDep;
  } else if (blob_name == "1DNDFromDep") {
    AxToUse.NDAx = &axErecND_FromDep;
    AxToUse.FDAx_numu = &axErecFD_FromDep;
    AxToUse.FDAx_nue = &axErecFD_FromDep;
  } else if (blob_name == "CoarseBin1D") {
    AxToUse.NDAx = &axErecND_coarsebin;
    AxToUse.FDAx_numu = &axRecoEnuFDnumu_coarsebin;
    AxToUse.FDAx_nue = &axRecoEnuFDnue_coarsebin;
  } else if (blob_name == "VeryCoarseBin1D") {
    AxToUse.NDAx = &axErecND_verycoarsebin;
    AxToUse.FDAx_numu = &axRecoEnuFDnumu_verycoarsebin;
    AxToUse.FDAx_nue = &axRecoEnuFDnue_verycoarsebin;
  } else if (blob_name == "OneBin") {
    AxToUse.NDAx = &axErecND_onebin;
    AxToUse.FDAx_numu = &axRecoEnuFDnumu_onebin;
    AxToUse.FDAx_nue = &axRecoEnuFDnue_onebin;
  } else if (blob_name == "ETrue") {
    AxToUse.NDAx = &axTrueE_unibin;
    AxToUse.FDAx_numu = &axTrueE_unibin;
    AxToUse.FDAx_nue = &axTrueE_unibin;
  } else if (blob_name == "ETrueCoarse") {
    AxToUse.NDAx = &axTrueE_unibin_coarse;
    AxToUse.FDAx_numu = &axTrueE_unibin_coarse;
    AxToUse.FDAx_nue = &axTrueE_unibin_coarse;
  } else if (blob_name == "FakeData") {
    AxToUse = fake_data_axes;
  } else if (blob_name == "v4") {
    AxToUse = default_axes_v4;
  } else if (blob_name == "v3") {
    AxToUse = default_axes_v3;
  }
  for (auto &v : AxToUse.NDAx->GetVars()) {
    assert(v.IsValid());
  }
  for (auto &v : AxToUse.FDAx_numu->GetVars()) {
    assert(v.IsValid());
  }
  for (auto &v : AxToUse.FDAx_nue->GetVars()) {
    assert(v.IsValid());
  }
  return AxToUse;
}
