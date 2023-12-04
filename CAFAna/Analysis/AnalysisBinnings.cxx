#include "CAFAna/Analysis/AnalysisBinnings.h"
#include "CAFAna/Analysis/AnalysisVersion.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Vars/Vars.h"

#include <vector>

using namespace ana;

AxisBlob GetAxisBlob(std::string const &blob_name) {

  //Should all be statically initialized on first method call to ensure that the vars have been initialized beforehand
  // Axes
  static HistAxis const axRecoEnuFDnumu("Reco energy (GeV)", kFDRecoBinning,
                                             kRecoE_numu);
  static HistAxis const axRecoEnuFDnue("Reco energy (GeV)", kFDRecoBinning,
                                            kRecoE_nue);
  static HistAxis const axErecYrecND("Reco energy (GeV)", kNDRecoBinning,
                                          kRecoEnergyND, "y_{rec}", kYBinning,
                                          kRecoYND);
  static HistAxis const axRecoEnuFDnumu_v3("Reco energy (GeV)",
                                                kFDRecoBinning, kRecoE_numu);
  static HistAxis const axRecoEnuFDnue_v3("Reco energy (GeV)",
                                               kFDRecoV3Binning, kRecoE_nue);
  static HistAxis const axErecYrecND_v3("Reco energy (GeV)",
                                             kNDRecoV3Binning, kRecoEnergyND,
                                             "y_{rec}", kYV3Binning, kRecoYND);
  static HistAxis const axErecND("Reco energy (GeV)", kNDRecoBinning,
                                      kRecoEnergyND);

  static HistAxis const axErecFD_FromDep("Reco energy (GeV)", kFDRecoBinning,
                                              kRecoE_FromDep);
  static HistAxis const axErecYrecND_FromDep(
      "Reco energy (GeV)", kNDRecoBinning, kRecoE_FromDep, "y_{rec}", kYBinning,
      kRecoY_FromDep);
  static HistAxis const axErecYrecNDOA_FromDep(
      "Reco energy (GeV)", kNDRecoOABinning, kRecoE_FromDep, "y_{rec}", kYBinning,
      kRecoY_FromDep);
  static HistAxis const axErecND_FromDep("#it{E}_{#nu}^{Rec.} (GeV)",
                                              kNDRecoBinning, kRecoE_FromDep);

  static HistAxis const axTrueE_unibin("#it{E}_{#nu} (GeV)", kTrueBinning,
                                            kTrueEnergy);

  static HistAxis const axTrueE_unibin_coarse(
      "#it{E}_{#nu} (GeV)", kTrueCoarseBinning, kTrueEnergy);

  static HistAxis const axErec_FromDep_unibin("#it{E}_{#nu}^{Rec.} (GeV)",
                                                   kTrueBinning, kRecoE_FromDep);
  static HistAxis const axErecND_unibin("#it{E}_{#nu}^{Rec.} (GeV)",
                                             kTrueBinning, kRecoEnergyND);
  static HistAxis const axRecoEnuFDnumu_unibin("Reco energy (GeV)",
                                                    kTrueBinning, kRecoE_numu);
  static HistAxis const axRecoEnuFDnue_unibin("Reco energy (GeV)",
                                                   kTrueBinning, kRecoE_nue);

  static HistAxis const axErecND_coarsebin(
      "Reco energy (GeV)", kRecoCoarseBinning, kRecoEnergyND);
  static HistAxis const axRecoEnuFDnumu_coarsebin(
      "Reco energy (GeV)", kRecoCoarseBinning, kRecoE_numu);

  static HistAxis const axRecoEnuFDnue_coarsebin(
      "Reco energy (GeV)", kRecoCoarseBinning, kRecoE_nue);

  static HistAxis const axErecND_verycoarsebin(
      "Reco energy (GeV)", kRecoVeryCoarseBinning, kRecoEnergyND);
  static HistAxis const axRecoEnuFDnumu_verycoarsebin(
      "Reco energy (GeV)", kRecoVeryCoarseBinning, kRecoE_numu);

  static HistAxis const axRecoEnuFDnue_verycoarsebin(
      "Reco energy (GeV)", kRecoVeryCoarseBinning, kRecoE_nue);

  static HistAxis const axErecND_onebin("Reco energy (GeV)", kOneBinBinning,
                                             kRecoEnergyND);
  static HistAxis const axRecoEnuFDnumu_onebin("Reco energy (GeV)", kOneBinBinning,
                                                    kRecoE_numu);

  static HistAxis const axRecoEnuFDnue_onebin("Reco energy (GeV)", kOneBinBinning,
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
  } else if (blob_name == "CoarseNue") {
    AxToUse.FDAx_nue = &axRecoEnuFDnue_coarsebin;
  } else if (blob_name == "VeryCoarseNue") {
    AxToUse.FDAx_nue = &axRecoEnuFDnue_verycoarsebin;
  } else if (blob_name == "OneBinNue") {
    AxToUse.FDAx_nue = &axRecoEnuFDnue_onebin;
  }
  return AxToUse;
}
