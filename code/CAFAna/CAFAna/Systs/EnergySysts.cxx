#include "CAFAna/Systs/EnergySysts.h"

namespace ana {
  const EnergyScaleNDSyst kEnergyScaleNDSyst;
  const EnergyScaleFDSyst kEnergyScaleFDSyst;
  const eScaleMuLArSyst keScaleMuLArSyst;
  const EnergyScaleMuSystND kEnergyScaleMuSystND;
  const EnergyScaleESyst kEnergyScaleESyst;
  const ChargedHadCorrSyst kChargedHadCorrSyst;
  const ChargedHadUncorrFDSyst kChargedHadUncorrFDSyst;
  const ChargedHadUncorrNDSyst kChargedHadUncorrNDSyst;
  const NUncorrNDSyst kNUncorrNDSyst;  
  const NUncorrFDSyst kNUncorrFDSyst;  
  const Pi0CorrSyst kEnergyScalePi0Syst;
  const Pi0UncorrNDSyst kPi0UncorrNDSyst;
  const Pi0UncorrFDSyst kPi0UncorrFDSyst;
  const ChargedHadAnticorrSyst kChargedHadAnticorrSyst;
  const Pi0AnticorrSyst kPi0AnticorrSyst;

  const EMUncorrNDSyst kEMUncorrNDSyst;
  const EMUncorrFDSyst kEMUncorrFDSyst;
  const eScaleMuLArFDSyst keScaleMuLArFDSyst;
  const eScaleMuLArNDSyst keScaleMuLArNDSyst;

  const UncorrNDHadLinSyst kUncorrNDHadLinSyst;
  const UncorrNDPi0LinSyst kUncorrNDPi0LinSyst;
  const UncorrNDNLinSyst kUncorrNDNLinSyst;
  const UncorrNDHadSqrtSyst kUncorrNDHadSqrtSyst;
  const UncorrNDPi0SqrtSyst kUncorrNDPi0SqrtSyst;
  const UncorrNDNSqrtSyst kUncorrNDNSqrtSyst;

  const MuonResFDSyst kMuonResFDSyst;
  const EMResFDSyst kEMResFDSyst;
  const ChargedHadResFDSyst kChargedHadResFDSyst;
  const NResFDSyst kNResFDSyst;
  const MuonResNDSyst kMuonResNDSyst;
  const EMResNDSyst kEMResNDSyst;
  const ChargedHadResNDSyst kChargedHadResNDSyst;
  const NResNDSyst kNResNDSyst;

  EnergySystVector GetEnergySysts()
  {
    EnergySystVector vec;
    vec.push_back(&kEnergyScaleNDSyst);
    vec.push_back(&kEnergyScaleFDSyst);
    vec.push_back(&keScaleMuLArFDSyst);
    vec.push_back(&keScaleMuLArNDSyst);
    vec.push_back(&kEnergyScaleMuSystND);

    vec.push_back(&kChargedHadCorrSyst);
    vec.push_back(&kChargedHadAnticorrSyst);

    vec.push_back(&kNUncorrFDSyst);
    vec.push_back(&kNUncorrNDSyst);
    vec.push_back(&kEMUncorrNDSyst);
    vec.push_back(&kEMUncorrFDSyst);

    vec.push_back(&kMuonResFDSyst);
    vec.push_back(&kMuonResNDSyst);
    vec.push_back(&kEMResFDSyst);
    vec.push_back(&kEMResNDSyst);
    vec.push_back(&kChargedHadResFDSyst);
    vec.push_back(&kChargedHadResNDSyst);

    vec.push_back(&kUncorrNDHadLinSyst);
    vec.push_back(&kUncorrNDPi0LinSyst);
    vec.push_back(&kUncorrNDNLinSyst);
    vec.push_back(&kUncorrNDHadSqrtSyst);
    vec.push_back(&kUncorrNDPi0SqrtSyst);
    vec.push_back(&kUncorrNDNSqrtSyst);

    return vec;
  }
} // namespace
