#include "CAFAna/Systs/TruthEnergyFDSysts.h"

namespace ana {

  const TruthEnergyScaleFD kTruthEnergyScaleFD;
  const TruthEnergySqrtFD kTruthEnergySqrtFD;
  const TruthEnergyInvSqrtFD kTruthEnergyInvSqrtFD;
  const EMTruthUncorrFD kEMTruthUncorrFD;
  const EMTruthUncorrSqrtFD kEMTruthUncorrSqrtFD;
  const EMTruthUncorrInvSqrtFD kEMTruthUncorrInvSqrtFD;
  const ChargedHadTruthUncorrFD kChargedHadTruthUncorrFD;
  const ChargedHadTruthUncorrSqrtFD kChargedHadTruthUncorrSqrtFD;
  const ChargedHadTruthUncorrInvSqrtFD kChargedHadTruthUncorrInvSqrtFD;
  const ETruthScaleMuLArFD kETruthScaleMuLArFD;
  const ETruthScaleMuLArSqrtFD kETruthScaleMuLArSqrtFD;
  const ETruthScaleMuLArInvSqrtFD kETruthScaleMuLArInvSqrtFD;

  TruthEnergyFDSystVector GetTrueEFDSysts() {

    TruthEnergyFDSystVector vec;
    vec.push_back(&kTruthEnergyScaleFD);
    vec.push_back(&kTruthEnergySqrtFD);
    vec.push_back(&kTruthEnergyInvSqrtFD);
    vec.push_back(&kEMTruthUncorrFD);
    vec.push_back(&kEMTruthUncorrSqrtFD);
    vec.push_back(&kEMTruthUncorrInvSqrtFD);
    vec.push_back(&kChargedHadTruthUncorrFD);
    vec.push_back(&kChargedHadTruthUncorrSqrtFD);
    vec.push_back(&kChargedHadTruthUncorrInvSqrtFD);
    vec.push_back(&kETruthScaleMuLArFD);
    vec.push_back(&kETruthScaleMuLArSqrtFD);
    vec.push_back(&kETruthScaleMuLArInvSqrtFD);

    return vec;
  }

}
