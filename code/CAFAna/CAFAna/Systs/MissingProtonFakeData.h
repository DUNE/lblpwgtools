#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include "TH2.h"

#include <memory>
#include <sstream>
#include <string>
#include <vector>

class MissingProtonFakeDataGenerator : public ana::ISyst {
public:
  virtual ~MissingProtonFakeDataGenerator(){};

  std::vector<std::unique_ptr<TH2>> nu_Histos;
  std::vector<std::unique_ptr<TH2>> nubar_Histos;

  bool fDoWeight;

  TH2 const *GetWeightingHisto(int gmode, bool is_nu) const {
    if ((gmode < 1) || (gmode > 14)) {
      return nullptr;
    }

    return (is_nu ? nu_Histos : nubar_Histos)[gmode].get();
  }

  void Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr,
             double &weight) const override {

    if (!sr->dune.isCC) {
      return;
    }

    if (sigma != 1) {
      return;
    }

    restore.Add(sr->dune.eRec_FromDep);

    if (sr->dune.isFD) {
      sr->dune.eRec_FromDep -= EpFrac * sr->dune.eDepP;
    } else {
      sr->dune.eRec_FromDep -= EpFrac * sr->dune.eRecoP;
    }

    if (!fDoWeight) {
      return;
    }

    TH2 const *wght =
        GetWeightingHisto(sr->dune.GENIE_ScatteringMode, (sr->dune.nuPDG > 0));
    if (!wght) {
      return;
    }

    int binx = wght->GetXaxis()->FindFixBin(sr->dune.Ev);
    if ((binx == 0) || (binx == wght->GetXaxis()->GetNbins() + 1)) {
      return;
    }
    int biny = wght->GetYaxis()->FindFixBin(sr->dune.eP);
    if ((biny == 0) || (biny == wght->GetYaxis()->GetNbins() + 1)) {
      return;
    }

    double wght_val = wght->GetBinContent(binx, biny);

    weight *= wght_val;
  }

public:
  MissingProtonFakeDataGenerator(bool DoWeight = true, double epfrac = 0.2)
      : ana::ISyst(DoWeight ? "MissingProtonFakeDataGenerator"
                            : "MissingProtonEnergyGenerator",
                   DoWeight ? "MissingProtonFakeDataGenerator"
                            : "MissingProtonEnergyGenerator"),
        EpFrac(epfrac), fDoWeight(DoWeight) {

    std::vector<std::string> fnames = {
        "ProtonEdepm20pc_binnedWeights_nu.root",
        "ProtonEdepm20pc_binnedWeights_nubar.root"};
    for (size_t bm = 0; bm < 2; ++bm) {
      bool is_nu = (bm == 0);
      TFile inp((ana::FindCAFAnaDir() + "/Systs/" + fnames[bm]).c_str(),
                "READ");
      assert(!inp.IsZombie());
      for (size_t i = 0; i < 15; ++i) {
        std::stringstream ss("");
        ss << "EnuTp_" << i;
        (is_nu ? nu_Histos : nubar_Histos)
            .emplace_back(
                dynamic_cast<TH2 *>(inp.Get(ss.str().c_str())->Clone()));
        (is_nu ? nu_Histos : nubar_Histos).back()->SetDirectory(nullptr);
      }
    }
  }

  double EpFrac;
};

std::vector<const ana::ISyst *> GetMissingProtonEnergyFakeDataSyst() {
  static MissingProtonFakeDataGenerator mpfd;
  return {&mpfd};
}
