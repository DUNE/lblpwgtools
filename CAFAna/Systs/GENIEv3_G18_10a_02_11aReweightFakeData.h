#pragma once

#include "ReweightFakeDataBase.h"
#include "GeneratorReweight_GENIEv3_G18_10a_02_11a.h"

class GENIEv3_G18_10a_02_11aReweightFakeDataGenerator : public ReweightFakeDataGeneratorBase {
 public :

 GENIEv3_G18_10a_02_11aReweightFakeDataGenerator()
   : ReweightFakeDataGeneratorBase("GENIEv3_G18_10a_02_11aReweightFakeDataGenerator") {
    
    rw = new GeneratorReweight_GENIEv3_G18_10a_02_11a();
    BDT_norm[0] = 1.008 * 0.979; // nue
    BDT_norm[1] = 1.009 * 0.984; // numu
    BDT_norm[2] = 1.016 * 0.985; // nuebar
    BDT_norm[3] = 1.020 * 0.988; // numubar

    weight_cap = 1000;
  };

};
