#include "CAFAna/Systs/EnergySysts.h"

namespace ana {
  const EnergyScaleFDSyst kEnergyScaleFDSyst;
  const ChargedHadUncorrFDSyst kChargedHadUncorrFDSyst;
  const NUncorrFDSyst kNUncorrFDSyst;  
  const EMUncorrFDSyst kEMUncorrFDSyst;
  const eScaleMuLArFDSyst keScaleMuLArFDSyst;
  const UncorrFDTotLinSyst kUncorrFDTotLinSyst;
  const UncorrFDTotSqrtSyst kUncorrFDTotSqrtSyst;
  const UncorrFDMuLinSyst kUncorrFDMuLinSyst;
  const UncorrFDMuSqrtSyst kUncorrFDMuSqrtSyst;
  const UncorrFDHadLinSyst kUncorrFDHadLinSyst;
  const UncorrFDHadSqrtSyst kUncorrFDHadSqrtSyst;
  const UncorrFDNLinSyst kUncorrFDNLinSyst;
  const UncorrFDNSqrtSyst kUncorrFDNSqrtSyst;
  const UncorrFDEMLinSyst kUncorrFDEMLinSyst;
  const UncorrFDEMSqrtSyst kUncorrFDEMSqrtSyst;

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
    vec.push_back(&kUncorrFDTotLinSyst);
    vec.push_back(&kUncorrFDTotSqrtSyst);
    vec.push_back(&kUncorrFDMuLinSyst);
    vec.push_back(&kUncorrFDMuSqrtSyst);
    vec.push_back(&kUncorrFDHadLinSyst);
    vec.push_back(&kUncorrFDHadSqrtSyst);
    vec.push_back(&kUncorrFDNLinSyst);
    vec.push_back(&kUncorrFDNSqrtSyst);
    vec.push_back(&kUncorrFDEMLinSyst);
    vec.push_back(&kUncorrFDEMSqrtSyst);

    vec.push_back(&kMuonResFDSyst);
    vec.push_back(&kEMResFDSyst);
    vec.push_back(&kChargedHadResFDSyst);
    vec.push_back(&kNResFDSyst);

    return vec;
  }
} // namespace
