#include "CAFAna/Systs/RecoEnergyNDSysts.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

namespace ana {

//  void RecoEnergyScaleND::Shift(double sigma,
//                                caf::SRProxy* sr,
//                                double& weight) const {
//  
//    const double scale = 0.02 * sigma;
//    if (!sr->isFD) {
//      // To match FD:
//      // Whether NC or CC, Numu or Nue, we want to shift total "reconstructed neutrino energy"
//      // by the "reconstructed hadronic energy".
//      sr->EVisReco_ND += (sr->EVisReco_ND - sr->Elep_reco) * scale;
//      // Also shift the hadronic energy variable, if we don't do this then shifted 
//      // plots of the reconstructed hadronic energy will not be different to nominal.
//      sr->HadEVisReco_ND *= 1. + scale;
//      // If it isn't Numu-CC, also shift the "reconstructed neutrino energy" by the
//      // "reconstructed leptonic energy".
//      // Also shift the reconstructed leptonic energy itself.
//      if (!(sr->isCC && abs(sr->nuPDG) == 14)) {
//        sr->EVisReco_ND += sr->Elep_reco * scale; 
//        sr->Elep_reco *= 1. + scale;
//      }
//    }
//  }
//
//  const RecoEnergyScaleND kRecoEnergyScaleND;
//
//  void RecoEnergySqrtND::Shift(double sigma,
//                               caf::SRProxy* sr,
//                               double& weight) const {
//
//    const double scale = 0.01 * sigma;
//    if (!sr->isFD) {
//      // To match LBL TDR:
//      // Whether NC or CC, Numu or Nue, we want to shift total "reconstructed neutrino energy"
//      // by the "reconstructed hadronic energy".
//      sr->EVisReco_ND += (sr->EVisReco_ND - sr->Elep_reco) * scale *
//        pow((sr->EVisReco_ND - sr->Elep_reco), 0.5);
//      // Also shift the hadronic energy variable, if we don't do this then shifted 
//      // plots of the reconstructed hadronic energy will not be different to nominal.
//      sr->HadEVisReco_ND += sr->HadEVisReco_ND * scale * pow(sr->HadEVisReco_ND, 0.5);
//      // If it isn't Numu-CC, also shift the "reconstructed neutrino energy" by the
//      // "reconstructed leptonic energy".
//      // Also shift the reconstructed leptonic energy itself.
//      if (!(sr->isCC && abs(sr->nuPDG) == 14)) {
//        sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
//        sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
//      }
//    }
//  }
//
//  const RecoEnergySqrtND kRecoEnergySqrtND;
//
//  void RecoEnergyInvSqrtND::Shift(double sigma,
//                                  caf::SRProxy* sr,
//                                  double& weight) const {
//
//    const double scale = 0.02 * sigma;
//    if (!sr->isFD) {
//      // To match LBL TDR:
//      // Whether NC or CC, Numu or Nue, we want to shift total "reconstructed neutrino energy"
//      // by the "reconstructed hadronic energy".
//      sr->EVisReco_ND += (sr->EVisReco_ND - sr->Elep_reco) * scale *
//        pow((sr->EVisReco_ND - sr->Elep_reco+0.1), -0.5);
//      // Also shift the hadronic energy variable, if we don't do this then shifted 
//      // plots of the reconstructed hadronic energy will not be different to nominal.
//      sr->HadEVisReco_ND += sr->HadEVisReco_ND * scale * pow(sr->HadEVisReco_ND+0.1, -0.5);
//      // If it isn't Numu-CC, also shift the "reconstructed neutrino energy" by the
//      // "reconstructed leptonic energy".
//      // Also shift the reconstructed leptonic energy itself.
//      if (!(sr->isCC && abs(sr->nuPDG) == 14)) {
//        sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
//        sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
//      }
//    }
//  }
//
//  const RecoEnergyInvSqrtND kRecoEnergyInvSqrtND;
//
//
//  void EMRecoUncorrND::Shift(double sigma,
//             caf::SRProxy* sr, 
//             double& weight) const {
//
//    const double scale = 0.025 * sigma;
//    if (!sr->isFD) { // in the ND
//      sr->eRecoPi0 += sr->eRecoPi0 * scale;
//      sr->EVisReco_ND += sr->eRecoPi0 * scale;
//      if (sr->isCC && abs(sr->nuPDG) == 12) {
//        sr->Elep_reco += sr->Elep_reco * scale;
//        sr->EVisReco_ND += sr->Elep_reco * scale;
//      }
//    }
//  }
//
//  const EMRecoUncorrND kEMRecoUncorrND;
//
//  void EMRecoUncorrSqrtND::Shift(double sigma,
//                                 caf::SRProxy* sr, 
//                                 double& weight) const {
//
//    const double scale = 0.025 * sigma;
//    if (!sr->isFD) { // in the ND
//      sr->eRecoPi0 += sr->eRecoPi0 * scale * pow(sr->eRecoPi0, 0.5);
//      sr->EVisReco_ND += sr->eRecoPi0 * scale * pow(sr->eRecoPi0, 0.5); 
//      if (sr->isCC && abs(sr->nuPDG) == 12) {
//        sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
//        sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
//      }
//    }
//  }
//
//  const EMRecoUncorrSqrtND kEMRecoUncorrSqrtND;
//
//  void EMRecoUncorrInvSqrtND::Shift(double sigma,
//                                    caf::SRProxy* sr,
//                                    double& weight) const {
//
//    const double scale = 0.025 * sigma;
//    if (!sr->isFD) { // in the ND
//      sr->eRecoPi0 += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
//      sr->EVisReco_ND += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
//      if (sr->isCC && abs(sr->nuPDG) == 12) {
//        sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
//        sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
//      }
//    }
//  }
//
//  const EMRecoUncorrInvSqrtND kEMRecoUncorrInvSqrtND;
//
//  void ChargedHadRecoUncorrND::Shift(double sigma,
//                                     caf::SRProxy* sr,
//                                     double& weight) const {
//
//    const double scale = 0.05 * sigma;
//    if (!sr->isFD) { // in the ND
//      if (sr->eRecoP < 0) sr->eRecoP = 0.;
//      if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
//      if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
//      const double sumE = sr->eRecoP + sr->eRecoPim + sr->eRecoPip;
//      sr->eRecoPip += sr->eRecoPip * scale;
//      sr->eRecoPim += sr->eRecoPim * scale;
//      sr->eRecoP += sr->eRecoP * scale;
//      sr->EVisReco_ND += sumE * scale;
//      sr->HadEVisReco_ND += sumE * scale;
//    }
//  }
//
//  const ChargedHadRecoUncorrND kChargedHadRecoUncorrND;
//
//  void ChargedHadRecoUncorrSqrtND::Shift(double sigma,
//                                         caf::SRProxy* sr,
//                                         double& weight) const {
//
//    const double scale = 0.05 * sigma;
//    if (!sr->isFD) { // in the ND
//      if (sr->eRecoP < 0) sr->eRecoP = 0.;
//      if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
//      if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
//      const double sumE = sr->eRecoPip + sr->eRecoPim + sr->eRecoP;
//      sr->eRecoPip += sr->eRecoPip * scale * pow(sr->eRecoPip, 0.5);
//      sr->eRecoPim += sr->eRecoPim * scale * pow(sr->eRecoPim, 0.5);
//      sr->eRecoP += sr->eRecoP * scale * pow(sr->eRecoP, 0.5);
//      sr->EVisReco_ND += sumE * scale * pow(sumE, 0.5);
//      sr->HadEVisReco_ND += sumE * scale * pow(sumE, 0.5);
//    }
//  }
//
//  const ChargedHadRecoUncorrSqrtND kChargedHadRecoUncorrSqrtND;
//
//  void ChargedHadRecoUncorrInvSqrtND::Shift(double sigma,
//                                            caf::SRProxy* sr,
//                                            double& weight) const {
//
//    const double scale = 0.05 * sigma;
//    if (!sr->isFD) { // in the ND
//      if (sr->eRecoP < 0) sr->eRecoP = 0.; 
//      if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
//      if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
//      const double sumE = sr->eRecoPip + sr->eRecoPim + sr->eRecoP;
//      sr->eRecoPip += sr->eRecoPip * scale * pow(sr->eRecoPip+0.1, -0.5);
//      sr->eRecoPim += sr->eRecoPim * scale * pow(sr->eRecoPim+0.1, -0.5);
//      sr->eRecoP += sr->eRecoP * scale * pow(sr->eRecoP+0.1, -0.5);
//      sr->EVisReco_ND += sumE * scale * pow(sumE+0.1, -0.5);
//      sr->HadEVisReco_ND += sumE * scale * pow(sumE+0.1, -0.5);
//    }
//  }
//
//  const ChargedHadRecoUncorrInvSqrtND kChargedHadRecoUncorrInvSqrtND;
//
//  void ERecoScaleMuLArND::Shift(double sigma,
//                                caf::SRProxy* sr,
//                                double& weight) const {
//
//    // Different EScale uncertainty for reconstruction by range or in
//    // GAr tracker
//    double var(0);
//    if (sr->muon_contained) var = 0.02;
//    else if (sr->muon_tracker) var = 0.01;
//    else var = 0.02; // selection requires muon to be inLAr or GAr so never get here
//    const double scale = var * sigma;
//    if (!sr->isFD) { // in the ND
//      if (sr->isCC && abs(sr->nuPDG) == 14) { 
//        sr->Elep_reco += sr->Elep_reco * scale;
//        sr->EVisReco_ND += sr->Elep_reco * scale;
//      }
//    }
//  } 
//
//  const ERecoScaleMuLArND kERecoScaleMuLArND;
//
//  void ERecoScaleMuLArSqrtND::Shift(double sigma,
//                                    caf::SRProxy* sr,
//                                    double& weight) const {
//
//    double var(0);
//    if (sr->muon_contained) var = 0.02;
//    else if (sr->muon_tracker) var = 0.01;
//    else var = 0.02; // selection requires muon to be inLAr or GAr so never get here
//    const double scale = var * sigma; // is 0.005 in FD
//    if (!sr->isFD) { // in the ND
//      if (sr->isCC && abs(sr->nuPDG) == 14) {
//        sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
//        sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
//      }
//    }
//  }
//
//  const ERecoScaleMuLArSqrtND kERecoScaleMuLArSqrtND;
//
//  void ERecoScaleMuLArInvSqrtND::Shift(double sigma,
//                                       caf::SRProxy* sr,
//                                       double& weight) const {
//
//    double var(0);
//    if (sr->muon_contained) var = 0.02;
//    else if (sr->muon_tracker) var = 0.01;
//    else var = 0.02; // selection requires muon to be inLAr or GAr so never get here
//    const double scale = var * sigma;
//    if (!sr->isFD) { // in the ND
//      if (sr->isCC && abs(sr->nuPDG) == 14) {
//        sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
//        sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
//      }
//    }
//  }
//
//  const ERecoScaleMuLArInvSqrtND kERecoScaleMuLArInvSqrtND;
//  
//  // Resolution Syst
//  
//  void MuonRecoResND::Shift(double sigma, 
//                            caf::SRProxy* sr,
//                            double& weight) const {
//
//    const double scale = 0.02 * sigma;
//    if (!sr->isFD) { // in the ND
//      sr->Elep_reco += (sr->LepE - sr->Elep_reco) * scale;
//      sr->EVisReco_ND += (sr->LepE - sr->Elep_reco) * scale;          
//    }
//  }
//
//  const MuonRecoResND kMuonRecoResND;
//
//  void EMRecoResND::Shift(double sigma,
//                          caf::SRProxy* sr, 
//                          double& weight) const {
//
//    const double scale = .02*sigma;
//    if (!sr->isFD){
//      sr->EVisReco_ND  += (sr->ePi0 - sr->eRecoPi0) * scale;
//      sr->eRecoPi0      += (sr->ePi0 - sr->eRecoPi0) * scale;
//      if (sr->isCC && abs(sr->nuPDG)==12) {
//        sr->EVisReco_ND  += (sr->LepE - sr->Elep_reco) * scale;
//        sr->Elep_reco += (sr->LepE - sr->Elep_reco) * scale;
//      }
//    }
//  }
//
//  const EMRecoResND kEMRecoResND;
//
//
//   void ChargedHadRecoResND::Shift(double sigma,
//                                   caf::SRProxy* sr, 
//                                   double& weight) const {
//
//    const double scale = .02*sigma;
//    if (!sr->isFD) {
//      if (sr->eRecoP < 0.) sr->eRecoP = 0.;
//      if (sr->eRecoPip < 0.) sr->eRecoPip = 0.;
//      if (sr->eRecoPim < 0.) sr->eRecoPim = 0.;
//      const double trueSum = sr->ePip + sr->ePim + sr->eP;
//      const double recoSum = sr->eRecoPip + sr->eRecoPim + sr->eRecoP;
//      sr->eRecoPip += (sr->ePip - sr->eRecoPip) * scale;
//      sr->eRecoPim += (sr->ePim - sr->eRecoPim) * scale;
//      sr->eRecoP   += (sr->eP   - sr->eRecoP)   * scale;
//      sr->EVisReco_ND += (trueSum - recoSum)   * scale;
//      sr->HadEVisReco_ND += (trueSum - recoSum)   * scale;
//    }
//  }
//
//  const ChargedHadRecoResND kChargedHadRecoResND;
//
//  RecoEnergyNDSystVector GetRecoENDSysts() {
//
//    RecoEnergyNDSystVector vec;
//    vec.push_back(&kRecoEnergyScaleND);
//    vec.push_back(&kRecoEnergySqrtND);
//    vec.push_back(&kRecoEnergyInvSqrtND);
//    vec.push_back(&kEMRecoUncorrND);
//    vec.push_back(&kEMRecoUncorrSqrtND);
//    vec.push_back(&kEMRecoUncorrInvSqrtND);
//    vec.push_back(&kChargedHadRecoUncorrND);
//    vec.push_back(&kChargedHadRecoUncorrSqrtND);
//    vec.push_back(&kChargedHadRecoUncorrInvSqrtND);
//    vec.push_back(&kERecoScaleMuLArND);
//    vec.push_back(&kERecoScaleMuLArSqrtND);
//    vec.push_back(&kERecoScaleMuLArInvSqrtND);
//    vec.push_back(&kMuonRecoResND);
//    vec.push_back(&kEMRecoResND);
//    vec.push_back(&kChargedHadRecoResND);
//
//    return vec;
//  }
//
//  bool IsNDdetRecoSyst(std::string name) {
//
//    std::vector<std::string> syst_names;
//    for (const auto &syst : GetRecoENDSysts()) {
//      syst_names.emplace_back(syst->ShortName());
//    }
//
//    return (std::find(syst_names.begin(), 
//                      syst_names.end(), name) != 
//                      syst_names.end()); 
//  }
//
//  std::vector<ana::ISyst const *> FilterOutNDSysts(std::vector<ana::ISyst const *> systs) {
//    std::vector<ana::ISyst const *> outs;
//    std::vector<ana::ISyst const *> fsysts = GetRecoENDSysts();
//
//    for (auto syst : systs) {
//      if (std::find(fsysts.begin(), fsysts.end(), syst) == fsysts.end()) {
//        outs.emplace_back(syst);
//      }
//    }
//    return outs;
//  }
}

