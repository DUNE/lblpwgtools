#include "CAFAna/Systs/TruthEnergyNDSysts.h"

namespace ana {

  const TruthEnergyScaleND kTruthEnergyScaleND;
  const TruthEnergySqrtND kTruthEnergySqrtND;
  const TruthEnergyInvSqrtND kTruthEnergyInvSqrtND;
  const EMTruthUncorrND kEMTruthUncorrND;
  const EMTruthUncorrSqrtND kEMTruthUncorrSqrtND;
  const EMTruthUncorrInvSqrtND kEMTruthUncorrInvSqrtND;
  const ChargedHadTruthUncorrND kChargedHadTruthUncorrND;
  const ChargedHadTruthUncorrSqrtND kChargedHadTruthUncorrSqrtND;
  const ChargedHadTruthUncorrInvSqrtND kChargedHadTruthUncorrInvSqrtND;
  const ETruthScaleMuLArND kETruthScaleMuLArND;
  const ETruthScaleMuLArSqrtND kETruthScaleMuLArSqrtND;
  const ETruthScaleMuLArInvSqrtND kETruthScaleMuLArInvSqrtND;

  TruthEnergyNDSystVector GetTrueENDSysts() {

    TruthEnergyNDSystVector vec;
    vec.push_back(&kTruthEnergyScaleND);
    vec.push_back(&kTruthEnergySqrtND);
    vec.push_back(&kTruthEnergyInvSqrtND);
    vec.push_back(&kEMTruthUncorrND);
    vec.push_back(&kEMTruthUncorrSqrtND);
    vec.push_back(&kEMTruthUncorrInvSqrtND);
    vec.push_back(&kChargedHadTruthUncorrND);
    vec.push_back(&kChargedHadTruthUncorrSqrtND);
    vec.push_back(&kChargedHadTruthUncorrInvSqrtND);
    vec.push_back(&kETruthScaleMuLArND);
    vec.push_back(&kETruthScaleMuLArSqrtND);
    vec.push_back(&kETruthScaleMuLArInvSqrtND);

    return vec;
  }

}
