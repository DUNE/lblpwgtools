#include "CAFAna/Systs/RecoEnergyFDSysts.h"

namespace ana {

  const RecoEnergyScaleFD kRecoEnergyScaleFD;
  const RecoEnergySqrtFD kRecoEnergySqrtFD;
  const RecoEnergyInvSqrtFD kRecoEnergyInvSqrtFD;
  const EMRecoUncorrFD kEMRecoUncorrFD;
  const EMRecoUncorrSqrtFD kEMRecoUncorrSqrtFD;
  const EMRecoUncorrInvSqrtFD kEMRecoUncorrInvSqrtFD;
  const ChargedHadRecoUncorrFD kChargedHadRecoUncorrFD;
  const ChargedHadRecoUncorrSqrtFD kChargedHadRecoUncorrSqrtFD;
  const ChargedHadRecoUncorrInvSqrtFD kChargedHadRecoUncorrInvSqrtFD;
  const ERecoScaleMuLArFD kERecoScaleMuLArFD;
  const ERecoScaleMuLArSqrtFD kERecoScaleMuLArSqrtFD;
  const ERecoScaleMuLArInvSqrtFD kERecoScaleMuLArInvSqrtFD;
  // Resolution Syst
  const MuonRecoResFD kMuonRecoResFD;
  const EMRecoResFD kEMRecoResFD;
  const ChargedHadRecoResFD kChargedHadRecoResFD;

  RecoEnergyFDSystVector GetRecoEFDSysts() {

    RecoEnergyFDSystVector vec;
    vec.push_back(&kRecoEnergyScaleFD);
    vec.push_back(&kRecoEnergySqrtFD);
    vec.push_back(&kRecoEnergyInvSqrtFD);
    vec.push_back(&kEMRecoUncorrFD);
    vec.push_back(&kEMRecoUncorrSqrtFD);
    vec.push_back(&kEMRecoUncorrInvSqrtFD);
    vec.push_back(&kChargedHadRecoUncorrFD);
    vec.push_back(&kChargedHadRecoUncorrSqrtFD);
    vec.push_back(&kChargedHadRecoUncorrInvSqrtFD);
    vec.push_back(&kERecoScaleMuLArFD);
    vec.push_back(&kERecoScaleMuLArSqrtFD);
    vec.push_back(&kERecoScaleMuLArInvSqrtFD);
    vec.push_back(&kMuonRecoResFD);
    vec.push_back(&kEMRecoResFD);
    vec.push_back(&kChargedHadRecoResFD);

    return vec;
  }

  bool IsFDdetRecoSyst(std::string name) {
  
    std::vector<std::string> syst_names;
    for (const auto &syst : GetRecoEFDSysts()) {
      syst_names.emplace_back(syst->ShortName());
    }

    return (std::find(syst_names.begin(),
                      syst_names.end(), name) !=
                      syst_names.end());
  }

  std::vector<ana::ISyst const *> FilterOutFDSysts(std::vector<ana::ISyst const *> systs) {
    std::vector<ana::ISyst const *> outs;
    std::vector<ana::ISyst const *> fsysts = GetRecoEFDSysts();

    for (auto syst : systs) {
      if (std::find(fsysts.begin(), fsysts.end(), syst) == fsysts.end()) {
        outs.emplace_back(syst);
      }
    }
    return outs;
  }

}
