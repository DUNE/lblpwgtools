#include "CAFAna/Systs/EnergySysts.h"

namespace ana {
// FD
// Total
const EnergyScaleFD kEnergyScaleFD;
const UncorrFDTotSqrt kUncorrFDTotSqrt;
const UncorrFDTotInvSqrt kUncorrFDTotInvSqrt;
// Muon
const EScaleMuLArFD kEScaleMuLArFD;
const UncorrFDMuSqrt kUncorrFDMuSqrt;
const UncorrFDMuInvSqrt kUncorrFDMuInvSqrt;
// EM
const EMUncorrFD kEMUncorrFD;
const UncorrFDEMSqrt kUncorrFDEMSqrt;
const UncorrFDEMInvSqrt kUncorrFDEMInvSqrt;
// Charged hadron
const ChargedHadUncorrFD kChargedHadUncorrFD;
const UncorrFDHadSqrt kUncorrFDHadSqrt;
const UncorrFDHadInvSqrt kUncorrFDHadInvSqrt;
// Neutron
const NUncorrFD kNUncorrFD;
const UncorrFDNSqrt kUncorrFDNSqrt;
const UncorrFDNInvSqrt kUncorrFDNInvSqrt;
// Resolutions
const MuonResFD kMuonResFD;
const EMResFD kEMResFD;
const ChargedHadResFD kChargedHadResFD;
const NResFD kNResFD;

// ND
// Total
const EnergyScaleND kEnergyScaleND;
const UncorrNDTotSqrt kUncorrNDTotSqrt;
const UncorrNDTotInvSqrt kUncorrNDTotInvSqrt;
// Muon
const EScaleMuLArND kEScaleMuLArND;
const UncorrNDMuLArSqrt kUncorrNDMuLArSqrt;
const UncorrNDMuLArInvSqrt kUncorrNDMuLArInvSqrt;
const EScaleMuSpectND kEScaleMuSpectND;
const UncorrNDMuSpectSqrt kUncorrNDMuSpectSqrt;
const UncorrNDMuSpectInvSqrt kUncorrNDMuSpectInvSqrt;
// EM
const EMUncorrND kEMUncorrND;
const UncorrNDEMSqrt kUncorrNDEMSqrt;
const UncorrNDEMInvSqrt kUncorrNDEMInvSqrt;
// Charged hadron
const ChargedHadUncorrND kChargedHadUncorrND;
const UncorrNDHadSqrt kUncorrNDHadSqrt;
const UncorrNDHadInvSqrt kUncorrNDHadInvSqrt;
// Neutron
const NUncorrND kNUncorrND;
const UncorrNDNSqrt kUncorrNDNSqrt;
const UncorrNDNInvSqrt kUncorrNDNInvSqrt;
// Resolutions
const MuonResND kMuonResND;
const EMResND kEMResND;
const ChargedHadResND kChargedHadResND;
const NResND kNResND;

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

EnergySystVector GetNDEnergySysts() {

  EnergySystVector vec;
  vec.push_back(&kEnergyScaleND);
  vec.push_back(&kUncorrNDTotSqrt);
  vec.push_back(&kUncorrNDTotInvSqrt);
  vec.push_back(&kUncorrNDHadSqrt);
  vec.push_back(&kUncorrNDHadInvSqrt);
  vec.push_back(&kUncorrNDMuLArSqrt);
  vec.push_back(&kUncorrNDMuLArInvSqrt);
  vec.push_back(&kUncorrNDMuSpectSqrt);
  vec.push_back(&kUncorrNDMuSpectInvSqrt);
  vec.push_back(&kUncorrNDNSqrt);
  vec.push_back(&kUncorrNDNInvSqrt);
  vec.push_back(&kUncorrNDEMSqrt);
  vec.push_back(&kUncorrNDEMInvSqrt);
  vec.push_back(&kEScaleMuLArND);
  vec.push_back(&kEScaleMuSpectND);
  vec.push_back(&kChargedHadUncorrND);
  vec.push_back(&kNUncorrND);
  vec.push_back(&kEMUncorrND);
  vec.push_back(&kMuonResND);
  vec.push_back(&kEMResND);
  vec.push_back(&kChargedHadResND);
  vec.push_back(&kNResND);

  return vec;
}
} // namespace ana
