#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "BDTReweighter.h"
#include "MissingProtonFakeData_BDTRW_FHC.h"
#include "MissingProtonFakeData_BDTRW_RHC.h"

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#ifdef BUILD_FDS_SYSTS

class MissingProtonFakeDataGenerator : public ana::ISyst {
public:
  virtual ~MissingProtonFakeDataGenerator(){};

  std::vector<BDTReweighter *> bdt_reweighter;

  bool fDoWeight;

  void Shift(double sigma,  caf::SRProxy *sr,
             double &weight) const override {

    if (!sr->isCC) {
      return;
    }

    if (sigma != 1) {
      return;
    }

    if (sr->isFD) {
      sr->eRec_FromDep -= EpFrac * sr->eDepP;
      sr->EVisReco_numu -= EpFrac * sr->eDepP;
      sr->EVisReco_nue -= EpFrac * sr->eDepP;
      sr->VisTrue_NDFD -= EpFrac * sr->eP;
      sr->HadEVisReco_FD -= EpFrac * sr->eDepP;
      sr->HadE -= EpFrac * sr->eP;
    } else {
      sr->eRec_FromDep -= EpFrac * sr->eRecoP;
      sr->EVisReco_ND -= EpFrac * sr->eRecoP;
      sr->VisTrue_NDFD -= EpFrac * sr->eP;
      sr->HadEVisReco_FD -= EpFrac *sr->eRecoP;
      sr->HadE -= EpFrac * sr->eP;
    }

    double eother = 0;
    if (std::isnormal(sr->eOther)) {
      eother = sr->eOther;
    }
    sr->eRecProxy = sr->LepE + (1.0 - EpFrac) * sr->eP +
                         sr->ePip + sr->ePim + sr->ePi0 +
                         0.135 * sr->nipi0 + eother;

    if (!fDoWeight) {
      return;
    }

    union BDTReweighter::BDTReweighterFeature features[8];

    features[5].fvalue = sr->Ev;                // Etrue
    features[6].fvalue = sr->eP;                // True proton kinetic energy
    features[7].fvalue = 1 - sr->LepE / sr->Ev; // ytrue

    for (int i = 0; i < 5; i++)
      features[i].fvalue = 0;

    bool foundMode = true;
    switch (sr->GENIE_ScatteringMode) {
    case 1:
      features[0].fvalue = 1.;
      break;
    case 3:
      features[1].fvalue = 1.;
      break;
    case 4:
      features[2].fvalue = 1.;
      break;
    case 5:
      features[3].fvalue = 1.;
      break;
    case 10:
      features[4].fvalue = 1.;
      break;
    default:
      foundMode = false;
    }

    if (foundMode) {
      double wght_val =
          bdt_reweighter[sr->nuPDG > 0 ? 0 : 1]->GetWeight(features, 1);
      weight *= wght_val;
    }
  }

public:
  MissingProtonFakeDataGenerator(bool DoWeight = true, double epfrac = 0.2)
      : ana::ISyst(DoWeight ? "MissingProtonFakeDataGenerator"
                            : "MissingProtonEnergyGenerator",
                   DoWeight ? "MissingProtonFakeDataGenerator"
                            : "MissingProtonEnergyGenerator",
                   false, 0, 1),
        fDoWeight(DoWeight), EpFrac(epfrac) {

    bdt_reweighter.push_back(new MissingProtonFakeData_BDTRW_FHC());
    bdt_reweighter.push_back(new MissingProtonFakeData_BDTRW_RHC());
  }
  double EpFrac;
};

#endif   // BUILD_FDS_SYSTS