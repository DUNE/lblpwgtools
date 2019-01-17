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

  std::vector<std::unique_ptr<TH2>> FHC_Histos;
  std::vector<std::unique_ptr<TH2>> RHC_Histos;

  TH2 const *GetWeightingHisto(int mode, bool isFHC) const {
    size_t indx = std::numeric_limits<size_t>::max();

    switch (mode) {
    case 0: { // QE
      indx = 0;
      break;
    }
    case 1: { // RES
      indx = 2;
      break;
    }
    case 2: { // DIS
      indx = 3;
      break;
    }
    case 10: { // MEC
      indx = 1;
      break;
    }
    default: { return nullptr; }
    }

    return (isFHC ? FHC_Histos : RHC_Histos)[indx].get();
  }

  void Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr,
             double &weight) const override {

    if (sigma != 1) {
      return;
    }

    restore.Add(sr->dune.eRec_FromDep);

    if (sr->dune.isFD) {
      sr->dune.eRec_FromDep = ((1 - EpFrac) * sr->dune.eDepP) + sr->dune.eDepN +
                              sr->dune.eDepPip + sr->dune.eDepPim +
                              sr->dune.eDepPi0 + sr->dune.eDepOther +
                              sr->dune.LepE;
    } else {
      sr->dune.eRec_FromDep = ((1 - EpFrac) * sr->dune.eRecoP) +
                              sr->dune.eRecoN + sr->dune.eRecoPip +
                              sr->dune.eRecoPim + sr->dune.eRecoPi0 +
                              sr->dune.eRecoOther + sr->dune.LepE;
    }

    TH2 const *wght = GetWeightingHisto(sr->dune.mode, sr->dune.isFHC);
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

    double wght_val = 1;//wght->GetBinContent(binx, biny);

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
  MissingProtonFakeDataGenerator(double epfrac = 0.2)
      : ISyst("MissingProtonFakeDataGenerator",
              "MissingProtonFakeDataGenerator"),
        EpFrac(epfrac) {

    std::vector<std::string> fnames = {
        "ProtonEdepm20pc_binnedWeights_FHC.root",
        "ProtonEdepm20pc_binnedWeights_RHC.root"};
    for (size_t bm = 0; bm < 2; ++bm) {
      TFile inp((FindCAFAnaDir() + "/Systs/" + fnames[bm]).c_str(), "READ");
      assert(!inp.IsZombie());
      for (size_t i = 0; i < 4; ++i) {
        std::stringstream ss("");
        ss << "EnuTp_" << (i + 1);
        (bm ? FHC_Histos : RHC_Histos)
            .emplace_back(
                dynamic_cast<TH2 *>(inp.Get(ss.str().c_str())->Clone()));
        (bm ? FHC_Histos : RHC_Histos).back()->SetDirectory(nullptr);
      }
    }
  }

  double EpFrac;
};
