#include "CAFAna/Systs/EnergySysts.h"

namespace ana {
const EnergyScaleFD kEnergyScaleFD;
const UncorrFDTotSqrt kUncorrFDTotSqrt;
const UncorrFDTotInvSqrt kUncorrFDTotInvSqrt;
const UncorrFDHadSqrt kUncorrFDHadSqrt;
const UncorrFDHadInvSqrt kUncorrFDHadInvSqrt;
const UncorrFDMuSqrt kUncorrFDMuSqrt;
const UncorrFDMuInvSqrt kUncorrFDMuInvSqrt;
const UncorrFDNSqrt kUncorrFDNSqrt;
const UncorrFDNInvSqrt kUncorrFDNInvSqrt;
const UncorrFDEMSqrt kUncorrFDEMSqrt;
const UncorrFDEMInvSqrt kUncorrFDEMInvSqrt;
const EScaleMuLArFD kEScaleMuLArFD;
const ChargedHadUncorrFD kChargedHadUncorrFD;
const NUncorrFD kNUncorrFD;
const EMUncorrFD kEMUncorrFD;
const MuonResFD kMuonResFD;
const EMResFD kEMResFD;
const ChargedHadResFD kChargedHadResFD;
const NResFD kNResFD;

EnergySystVector GetEnergySysts() {

  EnergySystVector vec;
  vec.push_back(&kEnergyScaleFD);
  vec.push_back(&kUncorrFDTotSqrt);//v4
  vec.push_back(&kUncorrFDTotInvSqrt);//v4
  vec.push_back(&kUncorrFDHadSqrt);//v4
  vec.push_back(&kUncorrFDHadInvSqrt);//v4
  vec.push_back(&kUncorrFDMuSqrt);//v4
  vec.push_back(&kUncorrFDMuInvSqrt);//v4
  vec.push_back(&kUncorrFDNSqrt);//v4
  vec.push_back(&kUncorrFDNInvSqrt);//v4
  vec.push_back(&kUncorrFDEMSqrt);//v4
  vec.push_back(&kUncorrFDEMInvSqrt);//v4
  vec.push_back(&kEScaleMuLArFD);
  vec.push_back(&kChargedHadUncorrFD);//v4
  vec.push_back(&kNUncorrFD);
  vec.push_back(&kEMUncorrFD);
  vec.push_back(&kMuonResFD);
  vec.push_back(&kEMResFD);
  vec.push_back(&kChargedHadResFD);
  vec.push_back(&kNResFD);

  return vec;
}
} // namespace ana
