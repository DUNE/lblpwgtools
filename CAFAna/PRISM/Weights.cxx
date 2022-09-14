#include "CAFAna/PRISM/Weights.h"

#include "CAFAna/Analysis/AnalysisVars.h"

#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"
#include "StandardRecord/SRProxy.h"

#include "TFile.h"
#include "TH1.h"

#include <iostream>

namespace PRISM {

std::map<int, TH1D *> fFileExposures;

const ana::Weight kSpecHCRunWeight([](const caf::StandardRecord *sr) {
  return sr->SpecialRunWeight;
});

// Use to weight by Exposure
const ana::Weight kRunPlanWeight([](const caf::StandardRecord *sr) -> double {
  return sr->perPOTWeight; 
});

const ana::Weight kMassCorrection([](const caf::StandardRecord *sr) -> double {
  return sr->NDMassCorrWeight;
});

// Use to get perFile weight
const ana::Weight kPerFileWeighter([](const caf::StandardRecord *sr) -> double {
  if (sr->isFD) {
    return 1;
  }
  return GetPerFileWeight(sr->SpecialHCRunId, sr->det_x + sr->vtx_x);  
});                                                                             

// Manual combination of the weights above
// Reduce the number of ana::Weight object calls
const ana::Weight
kNDPRISMCVWeights([](const caf::StandardRecord *sr) -> double {
  return sr->perPOTWeight * sr->NDMassCorrWeight * sr->SpecialRunWeight;
});

ana::Weight GetNDCVWeight() { return kNDPRISMCVWeights * kCVXSecWeights * kPerFileWeighter; }

ana::Weight GetFDCVWeight() { return kCVXSecWeights; }


void SetFileExposuresMap() {
  TFile fin((ana::FindCAFAnaDir() +
            "/PRISM/FileExposures/TotalOffAxisFileExposure.root").c_str());
  assert(fin.IsOpen());

  std::vector<int> SpecRunIds_all = {-293, -280, 280, 293};
  for (int SpecRunID_local : SpecRunIds_all) {
    std::stringstream ss("");
    ss << ((SpecRunID_local < 0) ? "m" : "") << SpecRunID_local;
    fin.GetObject(("FileExposure_" + ss.str()).c_str(), fFileExposures[SpecRunID_local]);
    if (!fFileExposures[SpecRunID_local]) abort();
    fFileExposures.find(SpecRunID_local)->second->SetDirectory(nullptr);
  }

  fin.Close();
}

// Get per file weight
double GetPerFileWeight(int SpecRun_ID, double abspos_x) {
  double nfiles(1);
  if (fFileExposures[SpecRun_ID]) {
    nfiles = fFileExposures[SpecRun_ID]->GetBinContent(
        fFileExposures[SpecRun_ID]->FindFixBin(abspos_x));
  }
  return 1 / nfiles;
}


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
