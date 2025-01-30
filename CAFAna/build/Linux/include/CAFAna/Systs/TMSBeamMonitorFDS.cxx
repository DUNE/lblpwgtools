#include "CAFAna/Systs/TMSBeamMonitorFDS.h"
#include "TFile.h"
#include "CAFAna/Core/Utilities.h"
#include <cassert>

namespace ana {

  std::vector<std::string> GetTMSBeamMonitorFDSDialNames(){
    static std::vector<std::string> ret = {"TMS_targetDensity",
					   "TMS_BeamOffsetX",
					   "TMS_BeamTheta",
					   "TMS_BeamThetaPhi",
					   "TMS_HC",
					   "TMS_WL",
					   "TMS_DPR",
					   "TMS_Horn1_XShift",
					   "TMS_Horn1_YShift",
					   "TMS_Horn2_XShift",
					   "TMS_Horn2_YShift",
					   "TMS_HC_m1",
					   "TMS_TargetDensity_m1",
					   "TMS_BeamSigma_m1",
					   "TMS_BeamOffsetX_m1",
					   "TMS_HC_p3",
					   "TMS_Horn3_XShift",
					   "TMS_Horn3_YShift",
					   "TMS_Horn1only_XShift",
					   "TMS_Horn1only_YShift",
					   "TMS_Horn1And2_1mm_ShiftShift",
					   "TMS_Horn1And2_1mm_TiltShift",
					   "TMS_Horn1And2_2.5mm_TiltShift"};
    return ret;
  }

  bool IsTMSBeamMonitorFDSSyst(std::string name){
    return (std::find(GetTMSBeamMonitorFDSDialNames().begin(), GetTMSBeamMonitorFDSDialNames().end(), name) !=
	    GetTMSBeamMonitorFDSDialNames().end());
  }


  void TMSBeamMonitorFDSDial::Shift(double sigma, Restorer &restore,
			    caf::StandardRecord *sr,
			    double &weight) const {
    
    // Play it safe
    if (fabs(sigma) < 1E-5) return;

    // FD only
    if (!sr->dune.isFD) return;

    // Only apply to initially numu(bar) events
    if (abs(sr->dune.nuPDGunosc) != 14) return;

    // Add a cap for events with Enu > the limit
    double enutrue = sr->dune.Ev;
    if (enutrue > 20) enutrue = 19.9;

    // Get the weight
    int xBin = wgts->GetXaxis()->FindBin(enutrue);
    weight *= wgts->GetBinContent(xBin);

  }

  
  TMSBeamMonitorFDSDial::TMSBeamMonitorFDSDial(std::string name, bool applyPenalty)
    : ISyst(name, name, applyPenalty, 0, 1),
      fName(name) {

    // Get the weight hists from the file
    TFile inFile((FindCAFAnaDir()+"/Systs/tms_beamMonitoring_weights.root").c_str(), "READ");
    assert(!inFile.IsZombie());
    wgts    = (TH1D*)inFile.Get(TString::Format("%s_ratio", fName.c_str()).ReplaceAll("TMS", "FD_h"));
    
    if (!wgts){
      std::cout << fName << " unrecognized by TMSBeamMonitorFDSDial" << std::endl;
      inFile.ls();
      exit(1);
    }
    wgts->SetDirectory(0);
  }

  std::vector<const ISyst *> GetTMSBeamMonitorFDSSysts(std::vector<std::string> names,
					       bool applyPenalty) {
    static std::vector<const ISyst *> ret;
    
    if (names.empty()) {
      names = GetTMSBeamMonitorFDSDialNames();
    }

    if (ret.empty()) {
      for (auto &it : names) {
	ret.push_back(new TMSBeamMonitorFDSDial(it, applyPenalty));
      }
    }
    return ret;
  }


}
