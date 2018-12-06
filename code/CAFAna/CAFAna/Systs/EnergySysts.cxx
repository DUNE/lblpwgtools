#include "CAFAna/Systs/EnergySysts.h"

namespace ana {
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

  EnergySystVector GetEnergySysts()
  {
    EnergySystVector vec;
    vec.push_back(&keScaleMuLArSyst);
    vec.push_back(&kEnergyScaleMuSystND);
    vec.push_back(&kEnergyScaleESyst);
    vec.push_back(&kChargedHadCorrSyst);
    vec.push_back(&kChargedHadUncorrFDSyst);
    vec.push_back(&kChargedHadUncorrNDSyst);
    vec.push_back(&kNUncorrFDSyst);
    vec.push_back(&kNUncorrNDSyst);
    vec.push_back(&kEnergyScalePi0Syst);
    vec.push_back(&kPi0UncorrFDSyst);
    vec.push_back(&kPi0UncorrNDSyst);
    // Uncorrelated ND energy scale systs
    //    UncorrNDSyst HadLin(UncorrNDSyst::kChargedHad, UncorrNDSyst::kLinear);
    //    UncorrNDSyst Pi0Lin(UncorrNDSyst::kPi0, UncorrNDSyst::kLinear);
    //    UncorrNDSyst NLin(UncorrNDSyst::kN, UncorrNDSyst::kLinear);
    //    vec.push_back(&HadLin);
    //    vec.push_back(&Pi0Lin);
    //    vec.push_back(&NLin);

    return vec;
  }
} // namespace
