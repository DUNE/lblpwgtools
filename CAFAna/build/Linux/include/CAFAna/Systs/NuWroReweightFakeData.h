#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include "BDTReweighter.h"
#include "GeneratorReweight_nuebar_FD_RHC.h"
#include "GeneratorReweight_nue_FD_FHC.h"
#include "GeneratorReweight_nue_FD_RHC.h"
#include "GeneratorReweight_numubar_FD_RHC.h"
#include "GeneratorReweight_numubar_ND_RHC.h"
#include "GeneratorReweight_numu_FD_FHC.h"
#include "GeneratorReweight_numu_FD_RHC.h"
#include "GeneratorReweight_numu_ND_FHC.h"
#include "GeneratorReweight_numu_ND_RHC.h"

#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <map>

class NuWroReweightFakeDataGenerator : public ana::ISyst {
public:

  virtual ~NuWroReweightFakeDataGenerator(){};

  //  std::map<std::string, BDTReweighter*> bdt_reweighters;
  std::vector<BDTReweighter*> bdt_reweighters;
  void Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr,
             double &weight) const override {

    if (!sr->dune.isCC) {
      return;
    }

    // To be on the safe side. BDT does not know about taus.
    if (abs(sr->dune.nuPDG) == 16) return;

    // Reweighting only applied up to 8 GeV
    if (sr->dune.Ev > 8.) return;

    if (sigma != 1) {
      return;
    }

    union BDTReweighter::BDTReweighterFeature features[22];

    features[0].fvalue = sr->dune.Ev;
    features[1].fvalue = sr->dune.LepE;
    features[2].fvalue = sr->dune.LepNuAngle;
    features[3].fvalue = sr->dune.Q2;
    features[4].fvalue = sr->dune.W;
    features[5].fvalue = sr->dune.X;
    features[6].fvalue = sr->dune.Y;

    features[7].fvalue = sr->dune.nP;
    features[8].fvalue = sr->dune.nN;
    features[9].fvalue = sr->dune.nipip;
    features[10].fvalue = sr->dune.nipim;
    features[11].fvalue = sr->dune.nipi0;
    features[12].fvalue = sr->dune.niem;

    features[13].fvalue = sr->dune.eP;
    features[14].fvalue = sr->dune.eN;
    features[15].fvalue = sr->dune.ePip;
    features[16].fvalue = sr->dune.ePim;
    features[17].fvalue = sr->dune.ePi0;

    // These last 4 are legacy of an initial attempt and I don't think the BDT is using them but let's fill them properly to be on the safe side.
    features[18].fvalue = (sr->dune.isFD == 0 ? 1 : 0); // FD files have isFD = 999
    features[19].fvalue = sr->dune.isFHC;
    features[20].fvalue = (sr->dune.nuPDG > 0 ? 1 : 0); // Is a neutrino, as opposed to antineutrino
    features[21].fvalue = (abs(sr->dune.nuPDG) == 14 ? 1 : 0); // Is (anti)numu, as opposed to (anti)nue

    // BDT_norm_weight normalizes weights predicted by the BDT to Genie absolute normalization
    double BDT_norm_weight = 1.;
    // absolute_NuWro_over_Genie_weight gives the absolute normalization difference between Genie and NuWro, i.e., the inclusive xsec ratio integrated over the respective fluxes.
    double absolute_NuWro_over_Genie_weight = 1.;
    // Calibration parameters for BDT output (Platt scaling)
    double plattA = -1.;
    double plattB = 0.;
    // Pointer to BDT to use
    BDTReweighter * thisReweighter = 0;

    if ((sr->dune.isFD == 0) && (sr->dune.isFHC) && (abs(sr->dune.nuPDG) == 14) && (sr->dune.nuPDG > 0)){
      BDT_norm_weight = 1.035;
      absolute_NuWro_over_Genie_weight = 1.013;
      plattA = -1.24585102;
      plattB = -0.01783353;
      thisReweighter = bdt_reweighters[6]; // 6 numu_ND_FHC
    } else if ((sr->dune.isFD == 0) && (!(sr->dune.isFHC)) && (abs(sr->dune.nuPDG) == 14) && (sr->dune.nuPDG > 0)){
      BDT_norm_weight = 1.046;
      absolute_NuWro_over_Genie_weight = 0.917;
      plattA = -1.22806734;
      plattB = -0.01548346;
      thisReweighter = bdt_reweighters[7]; // 7 numu_ND_RHC
    } else if ((sr->dune.isFD == 0) && (!(sr->dune.isFHC)) && (abs(sr->dune.nuPDG) == 14) && (!(sr->dune.nuPDG > 0))){
      BDT_norm_weight = 1.019;
      absolute_NuWro_over_Genie_weight = 0.917;
      plattA = -1.3029002;
      plattB = -0.0314188;
      thisReweighter = bdt_reweighters[8]; // 8 numubar_ND_RHC
    } else if ((!(sr->dune.isFD == 0)) && (sr->dune.isFHC) && (abs(sr->dune.nuPDG) == 14) && (sr->dune.nuPDG > 0)){
      BDT_norm_weight = 1.042;
      absolute_NuWro_over_Genie_weight = 1.011;
      plattA = -1.21073292;
      plattB = -0.02231727;
      thisReweighter = bdt_reweighters[3]; // 3 numu_FD_FHC
    } else if ((!(sr->dune.isFD == 0)) && (!(sr->dune.isFHC)) && (abs(sr->dune.nuPDG) == 14) && (sr->dune.nuPDG > 0)){
      BDT_norm_weight = 1.040;
      absolute_NuWro_over_Genie_weight = 0.981;
      plattA = -1.21420542;
      plattB = -0.02423824;
      thisReweighter = bdt_reweighters[4]; // 4 numu_FD_RHC
    } else if ((!(sr->dune.isFD == 0)) && (!(sr->dune.isFHC)) && (abs(sr->dune.nuPDG) == 14) && (!(sr->dune.nuPDG > 0))){
      BDT_norm_weight = 1.036;
      absolute_NuWro_over_Genie_weight = 0.904;
      plattA = -1.24604201;
      plattB = -0.03690337;
      thisReweighter = bdt_reweighters[5];  // 5 numubar_FD_RHC
    } else if ((!(sr->dune.isFD == 0)) && (sr->dune.isFHC) && (!(abs(sr->dune.nuPDG) == 14)) && (sr->dune.nuPDG > 0)){
      BDT_norm_weight = 1.041;
      absolute_NuWro_over_Genie_weight = 1.038;
      plattA = -1.22058287;
      plattB = -0.01992826;
      thisReweighter = bdt_reweighters[0]; // 0 nue_FD_FHC
    } else if ((!(sr->dune.isFD == 0)) && (!(sr->dune.isFHC)) && (!(abs(sr->dune.nuPDG) == 14)) && (sr->dune.nuPDG > 0)){
      BDT_norm_weight = 1.056;
      absolute_NuWro_over_Genie_weight = 0.988;
      plattA = -1.17760748;
      plattB = -0.02129378;
      thisReweighter = bdt_reweighters[1]; // 1 nue_FD_RHC
    } else if ((!(sr->dune.isFD == 0)) && (!(sr->dune.isFHC)) && (!(abs(sr->dune.nuPDG) == 14)) && (!(sr->dune.nuPDG > 0))){
      BDT_norm_weight = 1.029;
      absolute_NuWro_over_Genie_weight = 0.912;
      plattA = -1.27055928;
      plattB = -0.0294727;
      thisReweighter = bdt_reweighters[2]; // 2 nuebar_FD_RHC
    }

    if (thisReweighter) {
      weight *= absolute_NuWro_over_Genie_weight*BDT_norm_weight*exp(plattA*thisReweighter->predict(features,1) + plattB);
    }

  }

public:
  NuWroReweightFakeDataGenerator()
    : ana::ISyst("NuWroReweightFakeDataGenerator",
		 "NuWroReweightFakeDataGenerator", false, 0, 1){

    bdt_reweighters.emplace_back(new GeneratorReweight_nue_FD_FHC()); // 0 nue_FD_FHC
    bdt_reweighters.emplace_back(new GeneratorReweight_nue_FD_RHC()); // 1 nue_FD_RHC
    bdt_reweighters.emplace_back(new GeneratorReweight_nuebar_FD_RHC()); // 2 nuebar_FD_RHC
    bdt_reweighters.emplace_back(new GeneratorReweight_numu_FD_FHC()); // 3 numu_FD_FHC
    bdt_reweighters.emplace_back(new GeneratorReweight_numu_FD_RHC()); // 4 numu_FD_RHC
    bdt_reweighters.emplace_back(new GeneratorReweight_numubar_FD_RHC()); // 5 numubar_FD_RHC
    bdt_reweighters.emplace_back(new GeneratorReweight_numu_ND_FHC()); // 6 numu_ND_FHC
    bdt_reweighters.emplace_back(new GeneratorReweight_numu_ND_RHC()); // 7 numu_ND_RHC
    bdt_reweighters.emplace_back(new GeneratorReweight_numubar_ND_RHC()); // 8 numubar_ND_RHC
  }
};
