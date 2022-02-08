#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include "BDTReweighter.h"
#include "MissingProtonFakeData_BDTRW_FHC.h"
#include "MissingProtonFakeData_BDTRW_RHC.h"

#include <memory>
#include <sstream>
#include <string>
#include <vector>

class MissingProtonFakeDataGenerator : public ana::ISyst {
public:

  virtual ~MissingProtonFakeDataGenerator(){};

  std::vector<BDTReweighter*> bdt_reweighter;

  bool fDoWeight;

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

    union BDTReweighter::BDTReweighterFeature features[8];

    features[5].fvalue = sr->dune.Ev; // Etrue
    features[6].fvalue = sr->dune.eP; // True proton kinetic energy
    features[7].fvalue = 1-sr->dune.LepE/sr->dune.Ev; // ytrue

    for (int i = 0; i < 5; i++) features[i].fvalue = 0;

    bool foundMode = true;
    switch(sr->dune.GENIE_ScatteringMode) {
    case 1 : features[0].fvalue = 1.;
      break;
    case 3:  features[1].fvalue = 1.;
      break;
    case 4:  features[2].fvalue = 1.;
      break;
    case 5:  features[3].fvalue = 1.;
      break;
    case 10:  features[4].fvalue = 1.;
      break;
    default :
      foundMode = false;
    }

    if (foundMode) {
      double wght_val = bdt_reweighter[sr->dune.nuPDG > 0 ? 0 : 1]->GetWeight(features, 1);
      weight *= wght_val;
    }
  }

public:
  MissingProtonFakeDataGenerator(bool DoWeight = true, double epfrac = 0.2)
      : ana::ISyst(DoWeight ? "MissingProtonFakeDataGenerator"
                            : "MissingProtonEnergyGenerator",
                   DoWeight ? "MissingProtonFakeDataGenerator"
                            : "MissingProtonEnergyGenerator", false, 0, 1),
        fDoWeight(DoWeight), EpFrac(epfrac) {

    bdt_reweighter.push_back(new MissingProtonFakeData_BDTRW_FHC());
    bdt_reweighter.push_back(new MissingProtonFakeData_BDTRW_RHC());

  }
  double EpFrac;
};


class MissingProtonFakeDataGeneratorNu : public ana::ISyst {
public:

  virtual ~MissingProtonFakeDataGeneratorNu(){};

  std::vector<BDTReweighter*> bdt_reweighter;

  bool fDoWeight;

  void Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr,
             double &weight) const override {

    if (!sr->dune.isCC) {
      return;
    }

    if (sr->dune.nuPDG < 0){
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

    union BDTReweighter::BDTReweighterFeature features[8];

    features[5].fvalue = sr->dune.Ev; // Etrue
    features[6].fvalue = sr->dune.eP; // True proton kinetic energy
    features[7].fvalue = 1-sr->dune.LepE/sr->dune.Ev; // ytrue

    for (int i = 0; i < 5; i++) features[i].fvalue = 0;

    bool foundMode = true;
    switch(sr->dune.GENIE_ScatteringMode) {
    case 1 : features[0].fvalue = 1.;
      break;
    case 3:  features[1].fvalue = 1.;
      break;
    case 4:  features[2].fvalue = 1.;
      break;
    case 5:  features[3].fvalue = 1.;
      break;
    case 10:  features[4].fvalue = 1.;
      break;
    default :
      foundMode = false;
    }

    if (foundMode) {
      double wght_val = bdt_reweighter[sr->dune.nuPDG > 0 ? 0 : 1]->GetWeight(features, 1);
      weight *= wght_val;
    }
  }

public:
  MissingProtonFakeDataGeneratorNu(bool DoWeight = true, double epfrac = 0.2)
      : ana::ISyst(DoWeight ? "MissingProtonFakeDataGeneratorNu"
                            : "MissingProtonEnergyGeneratorNu",
                   DoWeight ? "MissingProtonFakeDataGeneratorNu"
                            : "MissingProtonEnergyGeneratorNu", false, 0, 1),
        fDoWeight(DoWeight), EpFrac(epfrac) {

    bdt_reweighter.push_back(new MissingProtonFakeData_BDTRW_FHC());
    bdt_reweighter.push_back(new MissingProtonFakeData_BDTRW_RHC());

  }
  double EpFrac;
};

class MissingProtonFakeDataGeneratorNubar : public ana::ISyst {
public:

  virtual ~MissingProtonFakeDataGeneratorNubar(){};

  std::vector<BDTReweighter*> bdt_reweighter;

  bool fDoWeight;

  void Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr,
             double &weight) const override {

    if (!sr->dune.isCC) {
      return;
    }

    if (sr->dune.nuPDG > 0){
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

    union BDTReweighter::BDTReweighterFeature features[8];

    features[5].fvalue = sr->dune.Ev; // Etrue
    features[6].fvalue = sr->dune.eP; // True proton kinetic energy
    features[7].fvalue = 1-sr->dune.LepE/sr->dune.Ev; // ytrue

    for (int i = 0; i < 5; i++) features[i].fvalue = 0;

    bool foundMode = true;
    switch(sr->dune.GENIE_ScatteringMode) {
    case 1 : features[0].fvalue = 1.;
      break;
    case 3:  features[1].fvalue = 1.;
      break;
    case 4:  features[2].fvalue = 1.;
      break;
    case 5:  features[3].fvalue = 1.;
      break;
    case 10:  features[4].fvalue = 1.;
      break;
    default :
      foundMode = false;
    }

    if (foundMode) {
      double wght_val = bdt_reweighter[sr->dune.nuPDG > 0 ? 0 : 1]->GetWeight(features, 1);
      weight *= wght_val;
    }
  }

public:
  MissingProtonFakeDataGeneratorNubar(bool DoWeight = true, double epfrac = 0.2)
      : ana::ISyst(DoWeight ? "MissingProtonFakeDataGeneratorNubar"
                            : "MissingProtonEnergyGeneratorNubar",
                   DoWeight ? "MissingProtonFakeDataGeneratorNubar"
                            : "MissingProtonEnergyGeneratorNubar", false, 0, 1),
        fDoWeight(DoWeight), EpFrac(epfrac) {

    bdt_reweighter.push_back(new MissingProtonFakeData_BDTRW_FHC());
    bdt_reweighter.push_back(new MissingProtonFakeData_BDTRW_RHC());

  }
  double EpFrac;
};
