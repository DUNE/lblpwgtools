#include "CAFAna/Systs/EnergySysts.h"

namespace ana {
//
//  void EnergyScaleFD::Shift(double sigma,
//                            caf::SRProxy* sr, double& weight) const {
//
//    double scale = .02 * sigma;
//    if (sr->isFD) {
//      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
//      if (sr->Ev_reco_nue < 0.) sr->Ev_reco_nue = 0.;
//      sr->Ev_reco_numu  += scale * sr->RecoHadEnNumu;
//      sr->Ev_reco_nue   += scale * sr->RecoHadEnNue;
//      sr->EVisReco_numu += scale * sr->HadEVisReco_FD;
//      sr->EVisReco_nue  += scale * sr->HadEVisReco_FD;
//      sr->RecoHadEnNumu *= 1. + scale;
//      sr->RecoHadEnNue  *= 1. + scale;
//      sr->HadEVisReco_FD*= 1. + scale;
//      if ( !(sr->isCC==1 && abs(sr->nuPDG) == 14) ) {
//        sr->Ev_reco_nue  += scale * sr->RecoLepEnNue;
//        sr->EVisReco_nue += scale * sr->RecoLepEnNue;
//        sr->RecoLepEnNue *= 1. + scale;
//      }
//    }
//  }
//  
//  const EnergyScaleFD kEnergyScaleFD;
//
//  void UncorrFDTotSqrt::Shift(double sigma,
//                              caf::SRProxy* sr, double& weight) const {
//
//    const double scale = .01 * sigma;
//    if (sr->isFD) {
//      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
//      sr->Ev_reco_numu  += scale*pow(sr->RecoHadEnNumu, 0.5)*sr->RecoHadEnNumu;
//      sr->Ev_reco_nue   += scale*pow(sr->RecoHadEnNue, 0.5)*sr->RecoHadEnNue;
//      sr->EVisReco_numu += scale*pow(sr->HadEVisReco_FD, 0.5)*sr->HadEVisReco_FD;
//      sr->EVisReco_nue  += scale*pow(sr->HadEVisReco_FD, 0.5)*sr->HadEVisReco_FD; 
//      sr->RecoHadEnNumu += scale*pow(sr->RecoHadEnNumu, 0.5)*sr->RecoHadEnNumu;
//      sr->RecoHadEnNue  += scale*pow(sr->RecoHadEnNue, 0.5)*sr->RecoHadEnNue;
//      sr->HadEVisReco_FD+= scale*pow(sr->HadEVisReco_FD, 0.5)*sr->HadEVisReco_FD;
//      if ( !(sr->isCC==1 && abs(sr->nuPDG) == 14) ) {
//        sr->Ev_reco_nue  += scale*pow(sr->RecoLepEnNue, 0.5)*sr->RecoLepEnNue;
//        sr->EVisReco_nue += scale*pow(sr->RecoLepEnNue, 0.5)*sr->RecoLepEnNue;
//        sr->RecoLepEnNue += scale*pow(sr->RecoLepEnNue, 0.5)*sr->RecoLepEnNue;
//      }
//    }
//  }
//
//  const UncorrFDTotSqrt kUncorrFDTotSqrt;
//
//  void UncorrFDTotInvSqrt::Shift(double sigma,
//                                 caf::SRProxy* sr, double& weight) const {
//
//    if (sr->isFD) {
//      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
//      const double scale = .02 * sigma;
//      sr->Ev_reco_numu  += sr->RecoHadEnNumu*scale*pow(sr->RecoHadEnNumu+0.1, -0.5);
//      sr->Ev_reco_nue   += sr->RecoHadEnNue*scale*pow(sr->RecoHadEnNue+0.1, -0.5);
//      sr->EVisReco_numu += sr->HadEVisReco_FD*scale*pow(sr->HadEVisReco_FD+0.1, -0.5);
//      sr->RecoHadEnNumu += sr->RecoHadEnNumu*scale*pow(sr->RecoHadEnNumu+0.1, -0.5);
//      sr->RecoHadEnNue  += sr->RecoHadEnNue*scale*pow(sr->RecoHadEnNue+0.1, -0.5);
//      sr->HadEVisReco_FD+= sr->HadEVisReco_FD*scale*pow(sr->HadEVisReco_FD+0.1, -0.5);
//      if ( !(sr->isCC==1 && abs(sr->nuPDG) == 14) ) {
//        sr->Ev_reco_nue  += sr->RecoLepEnNue * scale * pow(sr->RecoLepEnNue+0.1, -0.5);
//        sr->EVisReco_nue += sr->RecoLepEnNue * scale * pow(sr->RecoLepEnNue+0.1, -0.5);
//        sr->RecoLepEnNue += sr->RecoLepEnNue * scale * pow(sr->RecoLepEnNue+0.1, -0.5);
//      }
//    }
//  }
//
//  const UncorrFDTotInvSqrt kUncorrFDTotInvSqrt;
//
//  void UncorrFDHadSqrt::Shift(double sigma,
//                              caf::SRProxy* sr, double& weight) const {
//
//    const double scale = .05 * sigma;
//    if (sr->isFD) {
//      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
//      if (sr->eRecoP < 0) sr->eRecoP = 0.;
//      if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
//      if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
//      double sumE = sr->eRecoP + sr->eRecoPip + sr->eRecoPim;
//      double sumEdep = sr->eDepP + sr->eDepPip + sr->eDepPim;
//      sr->eRecoP   += sr->eRecoP * scale * pow(sumE, 0.5);
//      sr->eRecoPim += sr->eRecoPim * scale * pow(sumE, 0.5);
//      sr->eRecoPip += sr->eRecoPip * scale * pow(sumE, 0.5);
//      sr->Ev_reco_numu += sumE * scale * pow(sumE, 0.5);
//      sr->Ev_reco_nue  += sumE * scale * pow(sumE, 0.5);
//      sr->EVisReco_numu+= sumEdep * scale * pow(sumEdep, 0.5);
//      sr->EVisReco_numu+= sumEdep * scale * pow(sumEdep, 0.5);
//      sr->RecoHadEnNumu += sumE * scale * pow(sumE, 0.5);
//      sr->RecoHadEnNue  += sumE * scale * pow(sumE, 0.5);
//      sr->HadEVisReco_FD+= sumEdep * scale * pow(sumEdep, 0.5);
//    }
//  }
//
//  const UncorrFDHadSqrt kUncorrFDHadSqrt;
//
//  void UncorrFDHadInvSqrt::Shift(double sigma,
//                                 caf::SRProxy* sr, double& weight) const {
//
//    const double scale = .05 * sigma;
//    if (sr->isFD) {
//      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
//      if (sr->eRecoP < 0) sr->eRecoP = 0.;
//      if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
//      if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
//      double sumE = sr->eRecoP + sr->eRecoPip + sr->eRecoPim;
//      double sumEdep = sr->eDepP + sr->eDepPip + sr->eDepPim;
//      sr->Ev_reco_numu += sumE * scale * pow(sumE+0.1, -0.5);
//      sr->Ev_reco_nue  += sumE * scale * pow(sumE+0.1, -0.5);
//      sr->EVisReco_numu+= sumEdep * scale * pow(sumEdep+0.1, -0.5);
//      sr->EVisReco_nue+= sumEdep * scale * pow(sumEdep+0.1, -0.5);
//      sr->eRecoP   += sr->eRecoP * scale * pow(sumE+0.1, -0.5);
//      sr->eRecoPim += sr->eRecoPim * scale * pow(sumE+0.1, -0.5);
//      sr->eRecoPip += sr->eRecoPip * scale * pow(sumE+0.1, -0.5);
//      sr->RecoHadEnNumu += sumE * scale * pow(sumE+0.1, -0.5);
//      sr->RecoHadEnNue  += sumE * scale * pow(sumE+0.1, -0.5);
//      sr->HadEVisReco_FD+= sumEdep * scale * pow(sumEdep+0.1, -0.5);
//    }
//  }
//
//  const UncorrFDHadInvSqrt kUncorrFDHadInvSqrt;
//
//  void UncorrFDMuSqrt::Shift(double sigma,
//                             caf::SRProxy* sr, double& weight) const {
//
//    if (sr->isFD && sr->isCC==1 && abs(sr->nuPDG)==14) {
//      const double scale = .005 * sigma;
//      sr->Ev_reco_numu  += sr->RecoLepEnNumu * pow(sr->RecoLepEnNumu, 0.5) * scale;
//      sr->EVisReco_numu += sr->RecoLepEnNumu * pow(sr->RecoLepEnNumu, 0.5) * scale;
//      sr->RecoLepEnNumu += sr->RecoLepEnNumu * pow(sr->RecoLepEnNumu, 0.5) * scale;
//    }
//  }
//
//  const UncorrFDMuSqrt kUncorrFDMuSqrt;
//
//  void UncorrFDMuInvSqrt::Shift(double sigma,
//                                caf::SRProxy* sr, double& weight) const {
//
//    if (sr->isFD && sr->isCC==1 && abs(sr->nuPDG)==14) {
//      const double scale = .02 * sigma;
//      sr->Ev_reco_numu  += sr->RecoLepEnNumu * scale * pow(sr->RecoLepEnNumu+0.1, -0.5);
//      sr->EVisReco_numu += sr->RecoLepEnNumu * scale * pow(sr->RecoLepEnNumu+0.1, -0.5);
//      sr->RecoLepEnNumu += sr->RecoLepEnNumu * scale * pow(sr->RecoLepEnNumu+0.1, -0.5);
//    }
//  }
//
//  const UncorrFDMuInvSqrt kUncorrFDMuInvSqrt;
//
//  void UncorrFDNSqrt::Shift(double sigma,
//                            caf::SRProxy* sr, double& weight) const {
//
//    const double scale = .3 * sigma;
//    if (sr->isFD) {
//      if (sr->eRecoN < 0.) { sr->eRecoN = 0.; }
//      sr->Ev_reco_numu += sr->eRecoN * pow(sr->eRecoN, 0.5) * scale;
//      sr->Ev_reco_nue  += sr->eRecoN * pow(sr->eRecoN, 0.5) * scale;
//      sr->RecoHadEnNumu += sr->eRecoN * pow(sr->eRecoN, 0.5) * scale;
//      sr->RecoHadEnNue  += sr->eRecoN * pow(sr->eRecoN, 0.5) * scale;
//    }
//  }
//
//  const UncorrFDNSqrt kUncorrFDNSqrt;
//
//  void UncorrFDNInvSqrt::Shift(double sigma,
//                               caf::SRProxy* sr, double& weight) const {
//
//    const double scale = .3 * sigma;
//    if (sr->isFD) {
//      if (sr->eRecoN < 0.) { sr->eRecoN = 0.; }
//      sr->Ev_reco_numu += sr->eRecoN * scale * pow(sr->eRecoN+0.1, -0.5);
//      sr->Ev_reco_nue  += sr->eRecoN * scale * pow(sr->eRecoN+0.1, -0.5);
//      sr->RecoHadEnNumu += sr->eRecoN * scale * pow(sr->eRecoN+0.1, -0.5);
//      sr->RecoHadEnNue  += sr->eRecoN * scale * pow(sr->eRecoN+0.1, -0.5);
//    }
//  }
//
//  const UncorrFDNInvSqrt kUncorrFDNInvSqrt;
//
//  void UncorrFDEMSqrt::Shift(double sigma,
//                             caf::SRProxy* sr, double& weight) const {
//
//    const double scale = .025 * sigma;
//    if (sr->isFD) {
//      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
//      if (sr->eRecoPi0 < 0) sr->eRecoPi0 = 0.;
//      sr->Ev_reco_numu += sr->eRecoPi0 * pow(sr->eRecoPi0, 0.5) * scale;
//      sr->Ev_reco_nue  += sr->eRecoPi0 * pow(sr->eRecoPi0, 0.5) * scale;
//      sr->EVisReco_numu+= sr->eDepPi0 * pow(sr->eDepPi0, 0.5) * scale;
//      sr->EVisReco_nue += sr->eDepPi0 * pow(sr->eDepPi0, 0.5) * scale;
//      sr->RecoHadEnNumu += sr->eRecoPi0 * pow(sr->eRecoPi0, 0.5) * scale;
//      sr->RecoHadEnNue  += sr->eRecoPi0 * pow(sr->eRecoPi0, 0.5) * scale;
//      sr->HadEVisReco_FD+= sr->eDepPi0 * pow(sr->eDepPi0, 0.5) * scale;
//      sr->eRecoPi0 += sr->eRecoPi0 * pow(sr->eRecoPi0, 0.5) * scale;
//      if (sr->eRecoPi0 < 0) sr->eRecoPi0 = 0.;
//      if (sr->isCC==1 && abs(sr->nuPDG)==12) {
//        sr->Ev_reco_nue  += sr->RecoLepEnNue*pow(sr->RecoLepEnNue, 0.5)*scale;
//        sr->EVisReco_nue += sr->RecoLepEnNue*pow(sr->RecoLepEnNue, 0.5)*scale;
//        sr->RecoLepEnNue += sr->RecoLepEnNue*pow(sr->RecoLepEnNue, 0.5)*scale;
//      }
//    }
//  }
//
//  const UncorrFDEMSqrt kUncorrFDEMSqrt;
//
//
//  void UncorrFDEMInvSqrt::Shift(double sigma,
//                                caf::SRProxy* sr, double& weight) const {
//
//    const double scale = .025 * sigma;
//    if (sr->isFD) {
//      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
//      if (sr->eRecoPi0 < 0) sr->eRecoPi0 = 0.;
//      sr->Ev_reco_numu += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
//      sr->Ev_reco_nue  += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
//      sr->EVisReco_numu+= sr->eDepPi0 * scale * pow(sr->eDepPi0+0.1, -0.5);
//      sr->EVisReco_nue += sr->eDepPi0 * scale * pow(sr->eDepPi0+0.1, -0.5);
//      sr->RecoHadEnNumu += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
//      sr->RecoHadEnNue  += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
//      sr->HadEVisReco_FD+= sr->eDepPi0 * scale * pow(sr->eDepPi0+0.1, -0.5);
//      sr->eRecoPi0 += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
//      if (sr->isCC==1 && abs(sr->nuPDG)==12) {
//        sr->Ev_reco_nue  += sr->RecoLepEnNue * scale * pow(sr->RecoLepEnNue+0.1, -0.5);
//        sr->EVisReco_nue += sr->RecoLepEnNue * scale * pow(sr->RecoLepEnNue+0.1, -0.5);
//        sr->RecoLepEnNue += sr->RecoLepEnNue * scale * pow(sr->RecoLepEnNue+0.1, -0.5);
//      }
//    }
//  }
//
//  const UncorrFDEMInvSqrt kUncorrFDEMInvSqrt;
//
//  void EScaleMuLArFD::Shift(double sigma,
//                            caf::SRProxy* sr, double& weight) const {
//
//    const double scale = .02 * sigma;
//    // Checks if FD  and select only CC muon neutrino events
//    if(sr->isFD && abs(sr->nuPDG) == 14 && sr->isCC == 1){
//      sr->Ev_reco_numu   += sr->RecoLepEnNumu * scale;
//      sr->EVisReco_numu  += sr->RecoLepEnNumu * scale;
//      sr->RecoLepEnNumu  *= 1. + scale;
//    }
//  }
//
//  const EScaleMuLArFD kEScaleMuLArFD;
//
//  void ChargedHadUncorrFD::Shift(double sigma,
//                                 caf::SRProxy* sr, double& weight) const {
//
//    const double scale = .05 * sigma;
//    if(sr->isFD) {
//      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
//      if (sr->eRecoP < 0) sr->eRecoP = 0.;
//      if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
//      if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
//      const double sumE = sr->eRecoP + sr->eRecoPim + sr->eRecoPip;
//      const double sumEdep = sr->eDepP + sr->eDepPip + sr->eDepPim;
//      sr->eRecoP   += sr->eRecoP * scale;
//      sr->eRecoPim += sr->eRecoPim * scale;
//      sr->eRecoPip += sr->eRecoPip * scale;
//      sr->Ev_reco_numu += sumE * scale;
//      sr->Ev_reco_nue  += sumE * scale;
//      sr->EVisReco_numu += sumEdep * scale;
//      sr->EVisReco_nue  += sumEdep * scale;
//      sr->RecoHadEnNumu += sumE * scale;
//      sr->RecoHadEnNue  += sumE * scale;
//      sr->HadEVisReco_FD += sumEdep * scale; 
//    }
//  }
//
//  const ChargedHadUncorrFD kChargedHadUncorrFD;
//
//  void NUncorrFD::Shift(double sigma,
//                        caf::SRProxy* sr, double& weight) const {
//
//    const double scale = .20 * sigma;
//    if(sr->isFD) {
//      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
//      if (sr->eRecoN < 0.) sr->eRecoN = 0.;
//      sr->Ev_reco_numu  += sr->eRecoN * scale;
//      sr->Ev_reco_nue   += sr->eRecoN * scale;
//      sr->RecoHadEnNumu += sr->eRecoN * scale;
//      sr->RecoHadEnNue  += sr->eRecoN * scale;
//      sr->eRecoN += sr->eRecoN * scale;
//    }
//  }
//
//  const NUncorrFD kNUncorrFD;
//
//  void EMUncorrFD::Shift(double sigma,
//                         caf::SRProxy* sr, double& weight) const {
//
//    const double scale = 0.025 * sigma;
//    if (sr->isFD) {
//      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
//      if (sr->eRecoPi0 < 0.) sr->eRecoPi0 = 0.;
//      sr->Ev_reco_nue  += sr->eRecoPi0 * scale;
//      sr->Ev_reco_numu += sr->eRecoPi0 * scale;
//      sr->EVisReco_numu += sr->eDepPi0 * scale;
//      sr->EVisReco_nue  += sr->eDepPi0 * scale;
//      sr->RecoHadEnNue  += sr->eRecoPi0 * scale;
//      sr->RecoHadEnNumu += sr->eRecoPi0 * scale;
//      sr->HadEVisReco_FD += sr->eDepPi0 * scale;
//      sr->eRecoPi0 += sr->eRecoPi0 * scale;
//      if (sr->isCC && abs(sr->nuPDG) == 12) {
//        sr->Ev_reco_nue  += sr->RecoLepEnNue * scale;
//        sr->EVisReco_nue += sr->RecoLepEnNue * scale;
//        sr->RecoLepEnNue += sr->RecoLepEnNue * scale;
//      }
//    }
//  }
//
//  const EMUncorrFD kEMUncorrFD;
//
//  void MuonResFD::Shift(double sigma,
//                        caf::SRProxy* sr, double& weight) const {
//
//    const double scale = .02*sigma;
//    // FD charged current numus only
//    if (sr->isFD && sr->isCC && abs(sr->nuPDG)==14) {
//      sr->Ev_reco_numu  += (sr->LepE - sr->RecoLepEnNumu) * scale;
//      sr->EVisReco_numu += (sr->LepE - sr->RecoLepEnNumu) * scale;
//      sr->RecoLepEnNumu += (sr->LepE - sr->RecoLepEnNumu) * scale;
//    }
//  }
//
//  const MuonResFD kMuonResFD;
//
//  void EMResFD::Shift(double sigma,
//                      caf::SRProxy* sr, double& weight) const {
//
//    const double scale = .02*sigma;
//    if (sr->isFD){
//      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
//      if (sr->eRecoPi0 < 0.) sr->eRecoPi0 = 0.;
//      sr->Ev_reco_nue  += (sr->ePi0 - sr->eRecoPi0) * scale;
//      sr->Ev_reco_numu += (sr->ePi0 - sr->eRecoPi0) * scale;
//      sr->EVisReco_nue += (sr->ePi0 - sr->eDepPi0) * scale;
//      sr->EVisReco_numu += (sr->ePi0 - sr->eDepPi0) * scale;
//      sr->RecoHadEnNumu += (sr->ePi0 - sr->eRecoPi0) * scale;
//      sr->RecoHadEnNue  += (sr->ePi0 - sr->eRecoPi0) * scale;
//      sr->HadEVisReco_FD+= (sr->ePi0 - sr->eDepPi0) * scale;
//      sr->eRecoPi0     += (sr->ePi0 - sr->eRecoPi0) * scale;
//      if (sr->isCC && abs(sr->nuPDG)==12) {
//        sr->Ev_reco_nue  += (sr->LepE - sr->RecoLepEnNue) * scale;
//        sr->EVisReco_nue += (sr->LepE - sr->RecoLepEnNue) * scale;
//        sr->RecoLepEnNue += (sr->LepE - sr->RecoLepEnNue) * scale;
//      }
//    }
//  }
//
//  const EMResFD kEMResFD;
//
//  void ChargedHadResFD::Shift(double sigma,
//                              caf::SRProxy* sr, double& weight) const {
//
//    const double scale = .02*sigma;
//    if (sr->isFD) {
//      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
//      if (sr->eRecoP < 0.) sr->eRecoP = 0.;
//      if (sr->eRecoPip < 0.) sr->eRecoPip = 0.;
//      if (sr->eRecoPim < 0.) sr->eRecoPim = 0.;
//      const double trueSum = sr->ePip + sr->ePim + sr->eP;
//      const double recoSum = sr->eRecoPip + sr->eRecoPim + sr->eRecoP;
//      const double depSum = sr->eDepP + sr->eDepPip + sr->eDepPim;
//      sr->Ev_reco_nue   += (trueSum - recoSum) * scale;
//      sr->Ev_reco_numu  += (trueSum - recoSum) * scale;
//      sr->EVisReco_nue  += (trueSum - depSum) * scale;
//      sr->EVisReco_numu += (trueSum - depSum) * scale;
//      sr->RecoHadEnNue  += (trueSum - recoSum) * scale;
//      sr->RecoHadEnNumu += (trueSum - recoSum) * scale;
//      sr->HadEVisReco_FD+= (trueSum - depSum) * scale;
//    }
//  }
//
//  const ChargedHadResFD kChargedHadResFD;
//
//  void NResFD::Shift(double sigma,
//                     caf::SRProxy* sr, double& weight) const {
//
//   const double scale = .1*sigma;
//    // FD charged current numus only
//    if (sr->isFD) {
//      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
//      if (sr->eRecoN < 0.) sr->eRecoN = 0.;
//      sr->Ev_reco_nue   += (sr->eN - sr->eRecoN) * scale;
//      sr->Ev_reco_numu  += (sr->eN - sr->eRecoN) * scale;
//      sr->RecoHadEnNue  += (sr->eN - sr->eRecoN) * scale;
//      sr->RecoHadEnNumu += (sr->eN - sr->eRecoN) * scale;
//      sr->eRecoN += (sr->eN - sr->eRecoN) * scale;
//    }
//  }
//
//  const NResFD kNResFD;
//
//  std::vector<const ISyst*> GetEnergySysts() {
//
//    std::vector<const ISyst*> vec;
//    vec.push_back(&kEnergyScaleFD);
//    vec.push_back(&kUncorrFDTotSqrt);//v4
//    vec.push_back(&kUncorrFDTotInvSqrt);//v4
//    vec.push_back(&kUncorrFDHadSqrt);//v4
//    vec.push_back(&kUncorrFDHadInvSqrt);//v4
//    vec.push_back(&kUncorrFDMuSqrt);//v4
//    vec.push_back(&kUncorrFDMuInvSqrt);//v4
//    vec.push_back(&kUncorrFDNSqrt);//v4
//    vec.push_back(&kUncorrFDNInvSqrt);//v4
//    vec.push_back(&kUncorrFDEMSqrt);//v4
//    vec.push_back(&kUncorrFDEMInvSqrt);//v4
//    vec.push_back(&kEScaleMuLArFD);
//    vec.push_back(&kChargedHadUncorrFD);//v4
//    vec.push_back(&kNUncorrFD);
//    vec.push_back(&kEMUncorrFD);
//    vec.push_back(&kMuonResFD);
//    vec.push_back(&kEMResFD);
//    vec.push_back(&kChargedHadResFD);
//    vec.push_back(&kNResFD);
//
//    return vec;
//  }
//
//  std::vector<ana::ISyst const *> FilterOutFDSysts(std::vector<ana::ISyst const *> systs) {
//    std::vector<ana::ISyst const *> outs;
//    std::vector<ana::ISyst const *> fsysts = GetEnergySysts();
//
//    for (auto syst : systs) {
//      if (std::find(fsysts.begin(), fsysts.end(), syst) == fsysts.end()) {
//        outs.emplace_back(syst);
//      }
//    }
//    return outs;
//  }
//    
} // namespace ana
