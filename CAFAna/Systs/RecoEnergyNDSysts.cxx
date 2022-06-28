#include "CAFAna/Systs/RecoEnergyNDSysts.h"

namespace ana {

  const RecoEnergyScaleND kRecoEnergyScaleND;
  const RecoEnergySqrtND kRecoEnergySqrtND;
  const RecoEnergyInvSqrtND kRecoEnergyInvSqrtND;
  const EMRecoUncorrND kEMRecoUncorrND;
  const EMRecoUncorrSqrtND kEMRecoUncorrSqrtND;
  const EMRecoUncorrInvSqrtND kEMRecoUncorrInvSqrtND;
  const ChargedHadRecoUncorrND kChargedHadRecoUncorrND;
  const ChargedHadRecoUncorrSqrtND kChargedHadRecoUncorrSqrtND;
  const ChargedHadRecoUncorrInvSqrtND kChargedHadRecoUncorrInvSqrtND;
  const ERecoScaleMuLArND kERecoScaleMuLArND;
  const ERecoScaleMuLArSqrtND kERecoScaleMuLArSqrtND;
  const ERecoScaleMuLArInvSqrtND kERecoScaleMuLArInvSqrtND;
  // Resolution Syst
  const MuonRecoResND kMuonRecoResND;
  const EMRecoResND kEMRecoResND;
  const ChargedHadRecoResND kChargedHadRecoResND;

  RecoEnergyNDSystVector GetRecoENDSysts() {

    RecoEnergyNDSystVector vec;
    vec.push_back(&kRecoEnergyScaleND);
    vec.push_back(&kRecoEnergySqrtND);
    vec.push_back(&kRecoEnergyInvSqrtND);
    vec.push_back(&kEMRecoUncorrND);
    vec.push_back(&kEMRecoUncorrSqrtND);
    vec.push_back(&kEMRecoUncorrInvSqrtND);
    vec.push_back(&kChargedHadRecoUncorrND);
    vec.push_back(&kChargedHadRecoUncorrSqrtND);
    vec.push_back(&kChargedHadRecoUncorrInvSqrtND);
    vec.push_back(&kERecoScaleMuLArND);
    vec.push_back(&kERecoScaleMuLArSqrtND);
    vec.push_back(&kERecoScaleMuLArInvSqrtND);
    vec.push_back(&kMuonRecoResND);
    vec.push_back(&kEMRecoResND);
    vec.push_back(&kChargedHadRecoResND);

    return vec;
  }

  bool IsNDdetRecoSyst(std::string name) {

    std::vector<std::string> syst_names;
    for (const auto &syst : GetRecoENDSysts()) {
      syst_names.emplace_back(syst->ShortName());
    }

    return (std::find(syst_names.begin(), 
                      syst_names.end(), name) != 
                      syst_names.end()); 
  }

  std::vector<ana::ISyst const *> FilterOutNDSysts(std::vector<ana::ISyst const *> systs) {
    std::vector<ana::ISyst const *> outs;
    std::vector<ana::ISyst const *> fsysts = GetRecoENDSysts();

    for (auto syst : systs) {
      if (std::find(fsysts.begin(), fsysts.end(), syst) == fsysts.end()) {
        outs.emplace_back(syst);
      }
    }
    return outs;
  }
}

