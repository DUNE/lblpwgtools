#include "CAFAna/Systs/ArCScaling2DFakeData.h"
#include "TFile.h"
#include "CAFAna/Core/Utilities.h"
#include <cassert>

namespace ana {

  std::vector<std::string> GetArCScaling2DDialNames(){
    static std::vector<std::string> ret = {"ArCScaling2D_GENIEv3_G18_10b_00_000",
					   "ArCScaling2D_GENIEv3_G18_10a_02_11a",
					   "ArCScaling2D_NEUT",
                                           "ArCScaling2D_NuWro",
					   "ArCScaling2D_GiBUU",
                                           "ArCScaling2D_SuSAv2",
					   "ArCScaling2D_CRPA"};
    return ret;
  }

  bool IsArCScaling2DSyst(std::string name){
    return (std::find(GetArCScaling2DDialNames().begin(), GetArCScaling2DDialNames().end(), name) !=
	    GetArCScaling2DDialNames().end());
  }


  void ArCScaling2DDial::Shift(double sigma, Restorer &restore,
			    caf::StandardRecord *sr,
			    double &weight) const {
    
    // Play it safe
    if (fabs(sigma) < 1E-5) return;

    // Only care about CC
    if (!sr->dune.isCC) return;

    // FD only
    if (!sr->dune.isFD) return;

    double enutrue = sr->dune.Ev;
    if (enutrue > 10) enutrue = 9.9;

    double eavail = sr->dune.eP + sr->dune.ePip + sr->dune.ePim + sr->dune.ePi0 + sr->dune.eOther;
    if (eavail > 10) eavail = 9.9;

    // If this is a neutrino, get the weight
    if (sr->dune.nuPDG > 0){
      int xBin = nu_wgts->GetXaxis()->FindBin(enutrue);
      int yBin = nu_wgts->GetYaxis()->FindBin(eavail);
      weight *= nu_wgts->GetBinContent(xBin, yBin);
    }

    // Otherwise, if it's antineutrino, get the weight
    if (sr->dune.nuPDG < 0){
      int xBin = nubar_wgts->GetXaxis()->FindBin(enutrue);
      int yBin = nubar_wgts->GetYaxis()->FindBin(eavail);
      weight *= nubar_wgts->GetBinContent(xBin, yBin);
    }
  }

  
  ArCScaling2DDial::ArCScaling2DDial(std::string name, bool applyPenalty)
    : ISyst(name, name, applyPenalty, 0, 1),
      fName(name) {

    // Get the weight hists from the file
    TFile inFile((FindCAFAnaDir()+"/Systs/ArCScaling2DFakeData.root").c_str(), "READ");
    assert(!inFile.IsZombie());
    nu_wgts    = (TH2D*)inFile.Get((fName+"_14").c_str());
    nubar_wgts = (TH2D*)inFile.Get((fName+"_-14").c_str());
    
    if (!nu_wgts or !nubar_wgts){
      std::cout << fName << " unrecognised by ArCScaling2DDial" << std::endl;
      inFile.ls();
      exit(1);
    }
    nu_wgts->SetDirectory(0);
    nubar_wgts->SetDirectory(0);
  }

  std::vector<const ISyst *> GetArCScaling2DSysts(std::vector<std::string> names,
					       bool applyPenalty) {
    static std::vector<const ISyst *> ret;
    
    if (names.empty()) {
      names = GetArCScaling2DDialNames();
    }

    if (ret.empty()) {
      for (auto &it : names) {
	ret.push_back(new ArCScaling2DDial(it, applyPenalty));
      }
    }
    return ret;
  }


}
