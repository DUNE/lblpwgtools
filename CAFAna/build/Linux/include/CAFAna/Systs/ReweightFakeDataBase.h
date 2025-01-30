#pragma once

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/Utilities.h"

#include "StandardRecord/StandardRecord.h"

#include "BDTReweighter.h"

#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <map>

class ReweightFakeDataGeneratorBase : public ana::ISyst {
public:

  virtual ~ReweightFakeDataGeneratorBase(){};

  ReweightFakeDataGeneratorBase(const char * fakeDataName) : ana::ISyst(fakeDataName, fakeDataName, false, 0, 1) {;}
  
  BDTReweighter * rw;

  float BDT_norm[4];

  float weight_cap; 

  void Shift(double sigma, ana::Restorer &restore, caf::StandardRecord *sr,
             double &weight) const override {


    // To be on the safe side. BDT does not know about taus.
    if (abs(sr->dune.nuPDG) == 16) return;

    // Reweighting "only" applied up to 50 GeV
    if (sr->dune.Ev > 50.) return;

    if (sigma != 1) {
      return;
    }

    union BDTReweighter::BDTReweighterFeature features[23];

    features[0].fvalue = sr->dune.nuPDG > 0 ? 1 : 0; // isNu
    features[1].fvalue = abs(sr->dune.nuPDG) == 12 ? 1 : 0; // isNue
    features[2].fvalue = abs(sr->dune.nuPDG) == 14 ? 1 : 0; // isNumu
    features[3].fvalue = abs(sr->dune.nuPDG) == 16 ? 1 : 0; // isNutau
    features[4].fvalue = sr->dune.isCC ? 1 : 0; // cc

    features[5].fvalue = sr->dune.Ev;
    features[6].fvalue = sr->dune.LepE;
    features[7].fvalue = cos(sr->dune.LepNuAngle); 
    features[8].fvalue = sr->dune.Q2;
    features[9].fvalue = sr->dune.W;
    features[10].fvalue = sr->dune.X;
    features[11].fvalue = sr->dune.Y;

    features[12].fvalue = sr->dune.nP;
    features[13].fvalue = sr->dune.nN;
    features[14].fvalue = sr->dune.nipip;
    features[15].fvalue = sr->dune.nipim;
    features[16].fvalue = sr->dune.nipi0;
    features[17].fvalue = sr->dune.niem;

    features[18].fvalue = sr->dune.eP;
    features[19].fvalue = sr->dune.eN;
    features[20].fvalue = sr->dune.ePip;
    features[21].fvalue = sr->dune.ePim;
    features[22].fvalue = sr->dune.ePi0;

    int normIndex = -1;
    
    switch (sr->dune.nuPDG) {
    case 12 :
      normIndex = 0;
      break;
    case 14 :
      normIndex = 1;
      break;
    case -12 :
      normIndex = 2;
      break;
    case -14 :
      normIndex = 3;
      break;
    default :
      std::cerr << "ReweightFakeDataBase ERROR: Unknown neutrino species" << std::endl;
    }

    float bdt_weight = exp(rw->predict(features,1));
    if (bdt_weight > weight_cap) bdt_weight = weight_cap;

    weight *= BDT_norm[normIndex]*bdt_weight;

  }
};
