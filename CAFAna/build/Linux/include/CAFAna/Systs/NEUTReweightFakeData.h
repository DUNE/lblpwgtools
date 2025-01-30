#pragma once

#include "ReweightFakeDataBase.h"
#include "GeneratorReweight_NEUT.h"

class NEUTReweightFakeDataGenerator : public ReweightFakeDataGeneratorBase {
 public :

 NEUTReweightFakeDataGenerator()
   : ReweightFakeDataGeneratorBase("NEUTReweightFakeDataGenerator") {
    
    rw = new GeneratorReweight_NEUT();
    BDT_norm[0] = 1.038 * 0.976; // nue
    BDT_norm[1] = 1.035 * 0.979; // numu
    BDT_norm[2] = 1.064 * 0.945; // nuebar
    BDT_norm[3] = 1.061 * 0.946; // numubar

    weight_cap = 1000;
  };

};
