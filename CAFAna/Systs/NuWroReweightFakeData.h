#pragma once

#include "ReweightFakeDataBase.h"
#include "GeneratorReweight_NUWRO.h"

class NuWroReweightFakeDataGenerator : public ReweightFakeDataGeneratorBase {
 public :

 NuWroReweightFakeDataGenerator()
   : ReweightFakeDataGeneratorBase("NuWroReweightFakeDataGenerator") {

    rw = new GeneratorReweight_NUWRO();
    BDT_norm[0] = 1.043 * 1.001; // nue
    BDT_norm[1] = 1.044 * 1.0; // numu
    BDT_norm[2] = 1.049 * 1.018; // nuebar
    BDT_norm[3] = 1.046 * 1.006; // numubar
    
    weight_cap = 1000;
  };

};
