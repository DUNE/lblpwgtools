#include "CAFAna/Systs/EnergySysts.h"

namespace ana {
  const EnergyScaleFDSyst kEnergyScaleFDSyst;
  const ChargedHadUncorrFDSyst kChargedHadUncorrFDSyst;
  const NUncorrFDSyst kNUncorrFDSyst;
  const EMUncorrFDSyst kEMUncorrFDSyst;
  const eScaleMuLArFDSyst keScaleMuLArFDSyst;
  const UncorrFDTotSqrtSyst kUncorrFDTotSqrtSyst;
  const UncorrFDTotInvSqrtSyst kUncorrFDTotInvSqrtSyst;
  const UncorrFDMuSqrtSyst kUncorrFDMuSqrtSyst;
  const UncorrFDMuInvSqrtSyst kUncorrFDMuInvSqrtSyst;
  const UncorrFDHadSqrtSyst kUncorrFDHadSqrtSyst;
  const UncorrFDHadInvSqrtSyst kUncorrFDHadInvSqrtSyst;
  const UncorrFDNSqrtSyst kUncorrFDNSqrtSyst;
  const UncorrFDNInvSqrtSyst kUncorrFDNInvSqrtSyst;
  const UncorrFDEMSqrtSyst kUncorrFDEMSqrtSyst;
  const UncorrFDEMInvSqrtSyst kUncorrFDEMInvSqrtSyst;

  const MuonResFDSyst kMuonResFDSyst;
  const EMResFDSyst kEMResFDSyst;
  const ChargedHadResFDSyst kChargedHadResFDSyst;
  const NResFDSyst kNResFDSyst;

  EnergySystVector GetEnergySysts()
  {
    EnergySystVector vec;
    vec.push_back(&kEnergyScaleFDSyst);
    vec.push_back(&keScaleMuLArFDSyst);
    vec.push_back(&kChargedHadUncorrFDSyst);
    vec.push_back(&kNUncorrFDSyst);
    vec.push_back(&kEMUncorrFDSyst);
    vec.push_back(&kUncorrFDTotSqrtSyst);
    vec.push_back(&kUncorrFDTotInvSqrtSyst);
    vec.push_back(&kUncorrFDMuSqrtSyst);
    vec.push_back(&kUncorrFDMuInvSqrtSyst);
    vec.push_back(&kUncorrFDHadSqrtSyst);
    vec.push_back(&kUncorrFDHadInvSqrtSyst);
    vec.push_back(&kUncorrFDNSqrtSyst);
    vec.push_back(&kUncorrFDNInvSqrtSyst);
    vec.push_back(&kUncorrFDEMSqrtSyst);
    vec.push_back(&kUncorrFDEMInvSqrtSyst);

    vec.push_back(&kMuonResFDSyst);
    vec.push_back(&kEMResFDSyst);
    vec.push_back(&kChargedHadResFDSyst);
    vec.push_back(&kNResFDSyst);

    return vec;
  }
} // namespace
