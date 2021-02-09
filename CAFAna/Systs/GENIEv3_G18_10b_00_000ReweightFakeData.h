#pragma once

#include "ReweightFakeDataBase.h"
#include "GeneratorReweight_GENIEv3_G18_10b_00_000.h"

class GENIEv3_G18_10b_00_000ReweightFakeDataGenerator : public ReweightFakeDataGeneratorBase {
 public :

 GENIEv3_G18_10b_00_000ReweightFakeDataGenerator()
   : ReweightFakeDataGeneratorBase("GENIEv3_G18_10b_00_000ReweightFakeDataGenerator") {
    
    rw = new GeneratorReweight_GENIEv3_G18_10b_00_000();
    BDT_norm[0] = 1.044 * 1.006; // nue
    BDT_norm[1] = 1.040 * 1.011; // numu
    BDT_norm[2] = 1.054 * 1.017; // nuebar
    BDT_norm[3] = 1.052 * 1.020; // numubar

    weight_cap = 1000;
  };

};
