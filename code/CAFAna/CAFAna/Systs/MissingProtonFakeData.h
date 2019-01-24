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

  TH2 const *GetWeightingHisto(int gmode, bool is_matter) const {
    if ((gmode < 1) || (gmode > 14)) {
      return nullptr;
    }

    return (is_matter ? nu_Histos : nubar_Histos)[gmode].get();
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

    // std::cout << "Enu: " << sr->dune.Ev << ", xbin = " << binx << std::endl;
    // std::cout << "Ep: " << sr->dune.eP << ", ybin = " << biny << std::endl;
    // std::cout << "Wght: " << wght->GetBinContent(binx, biny) << std::endl;

    if (!std::isnormal(wght_val)) {
      std::cout << "Ep: " << sr->dune.eP << std::endl;
      std::cout << "Enu: " << sr->dune.Ev << std::endl;
      std::cout << "Wght: " << wght_val << std::endl;
    }

    weight *= wght_val;
  }

public:
  MissingProtonFakeDataGenerator(double epfrac = 0.2, bool DoWeight = true)
      : ISyst(DoWeight ? "MissingProtonFakeDataGenerator"
                       : "MissingProtonEnergyGenerator",
              DoWeight ? "MissingProtonFakeDataGenerator"
                       : "MissingProtonEnergyGenerator"),
        EpFrac(epfrac), fDoWeight(DoWeight) {

    std::vector<std::string> fnames = {
        "ProtonEdepm20pc_binnedWeights_nu.root",
        "ProtonEdepm20pc_binnedWeights_nubar.root"};
    for (size_t bm = 0; bm < 2; ++bm) {
      TFile inp((FindCAFAnaDir() + "/Systs/" + fnames[bm]).c_str(), "READ");
      assert(!inp.IsZombie());
      for (size_t i = 0; i < 15; ++i) {
        std::stringstream ss("");
        ss << "EnuTp_" << i;
        (bm ? nu_Histos : nubar_Histos)
            .emplace_back(
                dynamic_cast<TH2 *>(inp.Get(ss.str().c_str())->Clone()));
        (bm ? nu_Histos : nubar_Histos).back()->SetDirectory(nullptr);
      }
    }
  }

  double EpFrac;
};
