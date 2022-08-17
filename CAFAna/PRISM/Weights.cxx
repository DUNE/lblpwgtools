#include "CAFAna/PRISM/Weights.h"

#include "CAFAna/Analysis/AnalysisVars.h"

#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"
#include "StandardRecord/SRProxy.h"

#include "TFile.h"
#include "TH1.h"

#include <iostream>

namespace PRISM {

const ana::Weight kSpecHCRunWeight([](const caf::StandardRecord *sr) {
  return sr->SpecialRunWeight;
});

// Use to weight by Exposure
const ana::Weight kRunPlanWeight([](const caf::StandardRecord *sr) -> double {
  return sr->perPOTWeight * sr->perFileWeight;
});

const ana::Weight kMassCorrection([](const caf::StandardRecord *sr) -> double {
  return sr->NDMassCorrWeight;
});

// Manual combination of the weights above
// Reduce the number of ana::Weight object calls
const ana::Weight
kNDPRISMCVWeights([](const caf::StandardRecord *sr) -> double {
  return sr->perPOTWeight * sr->perFileWeight * sr->NDMassCorrWeight *
         sr->SpecialRunWeight;
});

ana::Weight GetNDCVWeight() { return kNDPRISMCVWeights * kCVXSecWeights; }

ana::Weight GetFDCVWeight() { return kCVXSecWeights; }

static TH1 *numode_280kA, *nubarmode_280kA;

double Get280kAWeight_numu(double enu, bool isNu) {

  // lazy load the inputs
  static bool first = true;
  if (first) {

    TDirectory *gdc = gDirectory;
    TFile wf(
        (ana::FindCAFAnaDir() + "/Systs/OnAxis4m280kAWeights.root").c_str());
    wf.GetObject("ND_nu_HC280-HC293_ratio/LBNF_numu_flux", numode_280kA);
    if (!numode_280kA) {
      std::cout << "[ERROR]: Failed to read: "
                   "\"ND_nu_HC280-HC293_ratio/LBNF_numu_flux\" from "
                << (ana::FindCAFAnaDir() + "/OnAxis4m280kAWeights.root")
                << std::endl;
      throw;
    }
    numode_280kA->SetDirectory(nullptr);

    wf.GetObject("ND_nubar_HC280-HC293_ratio/LBNF_numubar_flux",
                 nubarmode_280kA);
    if (!nubarmode_280kA) {
      std::cout << "[ERROR]: Failed to read: "
                   "\"ND_nubar_HC280-HC293_ratio/LBNF_numubar_flux\" from "
                << (ana::FindCAFAnaDir() + "/OnAxis4m280kAWeights.root")
                << std::endl;
      throw;
    }
    nubarmode_280kA->SetDirectory(nullptr);
    if (gdc) {
      gdc->cd();
    }
    first = false;
  }

  TH1 *whist = (isNu ? numode_280kA : nubarmode_280kA);
  return whist->GetBinContent(whist->GetXaxis()->FindFixBin(enu));
}

const ana::Weight k280kAWeighter([](const caf::StandardRecord *sr) -> double {
  if (sr->isFD) { 
    return 1;
  }
  // Only want to weight 'signal' numu species.
  if ((sr->isFHC && (sr->nuPDG != 14)) || (!sr->isFHC && (sr->nuPDG != -14))) {
    return 1;
  }
  return Get280kAWeight_numu(sr->Ev, sr->isFHC);
});

} // namespace PRISM
