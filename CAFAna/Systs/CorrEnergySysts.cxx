#include "CAFAna/Systs/CorrEnergySysts.h"

// Energy systematics that are 100% correclated between ND and FD

namespace ana
{
  // FD global energy scale syst still uncorrelated

  //------------------------------------------------------------------------------------------------

  // Slope energy scale systematics
  // Charged hadrons
  void CorrHadSqrt::Shift(double sigma,
                          Restorer& restore,
                          caf::SRProxy* sr, double& weight) const {
    restore.Add(sr->Ev_reco_numu,
                sr->Ev_reco_nue,
                sr->EVisReco_numu,
                sr->EVisReco_nue,
                sr->EVisReco_ND,
                sr->RecoHadEnNumu,
                sr->RecoHadEnNue,
                sr->HadEVisReco_FD,
                sr->HadEVisReco_ND,
                sr->eRecoP,
                sr->eRecoPim,
                sr->eRecoPip);

    const double scale = .05 * sigma;
    if (sr->isFD) {
      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
      if (sr->eRecoP < 0) sr->eRecoP = 0.;
      if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
      if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
      double sumE = sr->eRecoP + sr->eRecoPip + sr->eRecoPim;
      double sumEdep = sr->eDepP + sr->eDepPip + sr->eDepPim;
      sr->Ev_reco_numu   += sumE * scale * pow(sumE, 0.5);
      sr->Ev_reco_nue    += sumE * scale * pow(sumE, 0.5);
      sr->EVisReco_numu  += sumEdep * scale * pow(sumEdep, 0.5);
      sr->EVisReco_numu  += sumEdep * scale * pow(sumEdep, 0.5);
      sr->RecoHadEnNumu  += sumE * scale * pow(sumE, 0.5);
      sr->RecoHadEnNue   += sumE * scale * pow(sumE, 0.5);
      sr->HadEVisReco_FD += sumEdep * scale * pow(sumEdep, 0.5);
    } else {
      double sumE = sr->eRecoP + sr->eRecoPip + sr->eRecoPim;
      sr->EVisReco_ND    += sumE * scale * pow(sumE, 0.5);
      sr->HadEVisReco_ND += sumE * scale * pow(sumE, 0.5);
    }
  }

  const CorrHadSqrt kCorrHadSqrt;

  void CorrHadInvSqrt::Shift(double sigma,
                             Restorer& restore,
                             caf::SRProxy* sr, double& weight) const {

    restore.Add(sr->Ev_reco_numu,
                sr->Ev_reco_nue,
                sr->EVisReco_numu,
                sr->EVisReco_nue,
                sr->EVisReco_ND,
                sr->RecoHadEnNumu,
                sr->RecoHadEnNue,
                sr->HadEVisReco_FD,
                sr->HadEVisReco_ND,
                sr->eRecoP,
                sr->eRecoPim,
                sr->eRecoPip);

    const double scale = .05 * sigma;

    if (sr->isFD) {
      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
      if (sr->eRecoP < 0) sr->eRecoP = 0.;
      if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
      if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
      double sumE = sr->eRecoP + sr->eRecoPip + sr->eRecoPim;
      double sumEdep = sr->eDepP + sr->eDepPip + sr->eDepPim;
      sr->Ev_reco_numu   += sumE * scale * pow(sumE+0.1, -0.5);
      sr->Ev_reco_nue    += sumE * scale * pow(sumE+0.1, -0.5);
      sr->EVisReco_numu  += sumEdep * scale * pow(sumEdep+0.1, -0.5);
      sr->EVisReco_numu  += sumEdep * scale * pow(sumEdep+0.1, -0.5);
      sr->RecoHadEnNumu  += sumE * scale * pow(sumE+0.1, -0.5);
      sr->RecoHadEnNue   += sumE * scale * pow(sumE+0.1, -0.5);
      sr->HadEVisReco_FD += sumEdep * scale * pow(sumEdep+0.1, -0.5);
    } else {
      double sumE = sr->eRecoP + sr->eRecoPip + sr->eRecoPim;
      sr->EVisReco_ND    += sumE * scale * pow(sumE+0.1, -0.5);
      sr->HadEVisReco_ND += sumE * scale * pow(sumE+0.1, -0.5);
    }
  }

  const CorrHadInvSqrt kCorrHadInvSqrt;

  //------------------------------------------------------------------------------------

  // Muons
  void CorrMuSqrt::Shift(double sigma,
                         Restorer& restore,
                         caf::SRProxy* sr, double& weight) const {

    restore.Add(sr->Ev_reco_numu,
                sr->EVisReco_numu,
                sr->EVisReco_ND,
                sr->RecoLepEnNumu,
                sr->Elep_reco);

    double var(0);
    if (!sr->isFD && sr->muon_contained) var = 0.005;
    if (!sr->isFD && sr->muon_tracker) var = 0.005;
    else var = 0.005;
    const double scale = var * sigma;
    if (sr->isCC && abs(sr->nuPDG) == 14) {
      if (sr->isFD) {
        sr->Ev_reco_numu  += sr->RecoLepEnNumu * scale * pow(sr->RecoLepEnNumu, 0.5);
        sr->EVisReco_numu += sr->RecoLepEnNumu * scale * pow(sr->RecoLepEnNumu, 0.5);
        sr->RecoLepEnNumu += sr->RecoLepEnNumu * scale * pow(sr->RecoLepEnNumu, 0.5);
      } else {
        sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
        sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
      }
    }
  }

  const CorrMuSqrt kCorrMuSqrt;

  void CorrMuInvSqrt::Shift(double sigma,
                            Restorer& restore,
                            caf::SRProxy* sr, double& weight) const {

    restore.Add(sr->Ev_reco_numu,
                sr->EVisReco_numu,
                sr->EVisReco_ND,
                sr->RecoLepEnNumu,
                sr->Elep_reco);

    double var(0);
    if (!sr->isFD && sr->muon_contained) var = 0.02;
    if (!sr->isFD && sr->muon_tracker) var = 0.01;
    else var = 0.02; // else 2% error in FD
    const double scale = var * sigma;
    if (sr->isCC && abs(sr->nuPDG) == 14) {
      if (sr->isFD) {
        sr->Ev_reco_numu  += sr->RecoLepEnNumu * scale * pow(sr->RecoLepEnNumu+0.1, -0.5);
        sr->EVisReco_numu += sr->RecoLepEnNumu * scale * pow(sr->RecoLepEnNumu+0.1, -0.5);
        sr->RecoLepEnNumu += sr->RecoLepEnNumu * scale * pow(sr->RecoLepEnNumu+0.1, -0.5);
      } else {
        sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
        sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
      }
    }
  }

  const CorrMuInvSqrt kCorrMuInvSqrt;

  //------------------------------------------------------------------------------------

  //Neutrons only relevant for FD variables
  //Basically the same as FD EnergyScale Uncorr, but 
  //keep here for now in case ND shifts need to be added
  void CorrNSqrt::Shift(double sigma,
                        Restorer& restore,
                        caf::SRProxy* sr, double& weight) const {
    restore.Add(sr->Ev_reco_numu,
                sr->Ev_reco_nue,
                sr->RecoHadEnNumu,
                sr->RecoHadEnNue);

    const double scale = .3 * sigma;
    if (sr->isFD) {
      if (sr->eRecoN < 0.) sr->eRecoN = 0.;
      sr->Ev_reco_numu += sr->eRecoN * pow(sr->eRecoN, 0.5) * scale;
      sr->Ev_reco_nue  += sr->eRecoN * pow(sr->eRecoN, 0.5) * scale;
      sr->RecoHadEnNumu += sr->eRecoN * pow(sr->eRecoN, 0.5) * scale;
      sr->RecoHadEnNue  += sr->eRecoN * pow(sr->eRecoN, 0.5) * scale;
    }
  }

  const CorrNSqrt kCorrNSqrt;

  void CorrNInvSqrt::Shift(double sigma,
                           Restorer& restore,
                           caf::SRProxy* sr, double& weight) const {

    restore.Add(sr->Ev_reco_numu,
                sr->Ev_reco_nue,
                sr->RecoHadEnNumu,
                sr->RecoHadEnNue);

    const double scale = .3 * sigma;
    if (sr->isFD) {
      if (sr->eRecoN < 0.) sr->eRecoN = 0.;
      sr->Ev_reco_numu += sr->eRecoN * pow(sr->eRecoN+0.1, -0.5) * scale;
      sr->Ev_reco_nue  += sr->eRecoN * pow(sr->eRecoN+0.1, -0.5) * scale;
      sr->RecoHadEnNumu += sr->eRecoN * pow(sr->eRecoN+0.1, -0.5) * scale;
      sr->RecoHadEnNue  += sr->eRecoN * pow(sr->eRecoN+0.1, -0.5) * scale;
    }
  }

  const CorrNInvSqrt kCorrNInvSqrt;

  //------------------------------------------------------------------------------------

  // Electromagnetic
  void CorrEMSqrt::Shift(double sigma,
                         Restorer& restore,
                         caf::SRProxy* sr, double& weight) const {
    restore.Add(sr->Ev_reco_numu,
                sr->Ev_reco_nue,
                sr->EVisReco_numu,
                sr->EVisReco_nue,
                sr->EVisReco_ND,
                sr->RecoHadEnNumu,
                sr->RecoHadEnNue,
                sr->HadEVisReco_FD,
                sr->HadEVisReco_FD,
                sr->RecoLepEnNue,
                sr->Elep_reco,
                sr->eRecoPi0);

    const double scale = .025 * sigma;
    if (sr->isFD) {
      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
      if (sr->eRecoPi0 < 0) sr->eRecoPi0 = 0.;
      sr->Ev_reco_numu  += sr->eRecoPi0 * pow(sr->eRecoPi0, 0.5) * scale;
      sr->Ev_reco_nue   += sr->eRecoPi0 * pow(sr->eRecoPi0, 0.5) * scale;
      sr->EVisReco_numu += sr->eDepPi0 * pow(sr->eDepPi0, 0.5) * scale;
      sr->EVisReco_nue  += sr->eDepPi0 * pow(sr->eDepPi0, 0.5) * scale;
      sr->RecoHadEnNumu += sr->eRecoPi0 * pow(sr->eRecoPi0, 0.5) * scale;
      sr->RecoHadEnNue  += sr->eRecoPi0 * pow(sr->eRecoPi0, 0.5) * scale;
      sr->HadEVisReco_FD+= sr->eDepPi0 * pow(sr->eDepPi0, 0.5) * scale;
      if (sr->isCC && abs(sr->nuPDG)==12) {
        sr->Ev_reco_nue  += sr->RecoLepEnNue*pow(sr->RecoLepEnNue, 0.5)*scale;
        sr->EVisReco_nue += sr->RecoLepEnNue*pow(sr->RecoLepEnNue, 0.5)*scale;
        sr->RecoLepEnNue += sr->RecoLepEnNue*pow(sr->RecoLepEnNue, 0.5)*scale;
      }
    } else {
      sr->EVisReco_ND += sr->eRecoPi0 * scale * pow(sr->eRecoPi0, 0.5);
      if (sr->isCC && abs(sr->nuPDG) == 12) { 
        sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
        sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco, 0.5);
      }
    }
  }

  const CorrEMSqrt kCorrEMSqrt;

  void CorrEMInvSqrt::Shift(double sigma,
                            Restorer& restore,
                            caf::SRProxy* sr, double& weight) const {

    restore.Add(sr->Ev_reco_numu,
                sr->Ev_reco_nue,
                sr->EVisReco_numu,
                sr->EVisReco_nue,
                sr->EVisReco_ND,
                sr->RecoHadEnNumu,
                sr->RecoHadEnNue,
                sr->HadEVisReco_FD,
                sr->HadEVisReco_FD,
                sr->RecoLepEnNue,
                sr->Elep_reco,
                sr->eRecoPi0);

    const double scale = .025 * sigma;
    if (sr->isFD) {
      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
      if (sr->eRecoPi0 < 0) sr->eRecoPi0 = 0.;
      sr->Ev_reco_numu  += sr->eRecoPi0 * pow(sr->eRecoPi0+0.1, -0.5) * scale;
      sr->Ev_reco_nue   += sr->eRecoPi0 * pow(sr->eRecoPi0+0.1, -0.5) * scale;
      sr->EVisReco_numu += sr->eDepPi0 * pow(sr->eDepPi0+0.1, -0.5) * scale;
      sr->EVisReco_nue  += sr->eDepPi0 * pow(sr->eDepPi0+0.1, -0.5) * scale;
      sr->RecoHadEnNumu += sr->eRecoPi0 * pow(sr->eRecoPi0+0.1, -0.5) * scale;
      sr->RecoHadEnNue  += sr->eRecoPi0 * pow(sr->eRecoPi0+0.1, -0.5) * scale;
      sr->HadEVisReco_FD+= sr->eDepPi0 * pow(sr->eDepPi0+0.1, -0.5) * scale;
      if (sr->isCC && abs(sr->nuPDG)==12) {
        sr->Ev_reco_nue  += sr->RecoLepEnNue*pow(sr->RecoLepEnNue+0.1, -0.5)*scale;
        sr->EVisReco_nue += sr->RecoLepEnNue*pow(sr->RecoLepEnNue+0.1, -0.5)*scale;
        sr->RecoLepEnNue += sr->RecoLepEnNue*pow(sr->RecoLepEnNue+0.1, -0.5)*scale;
      }
    } else {
      sr->EVisReco_ND += sr->eRecoPi0 * scale * pow(sr->eRecoPi0+0.1, -0.5);
      if (sr->isCC && abs(sr->nuPDG) == 12) { 
        sr->EVisReco_ND += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
        sr->Elep_reco += sr->Elep_reco * scale * pow(sr->Elep_reco+0.1, -0.5);
      }
    }
  }

  const CorrEMInvSqrt kCorrEMInvSqrt;

  //-----------------------------------------------------------------------------------

  // muon LAr systematic
  // 2% on CC numu events
  void EScaleMuLAr::Shift(double sigma,
                          Restorer& restore,
                          caf::SRProxy* sr, double& weight) const {
    
    restore.Add(sr->Ev_reco_numu,
                sr->EVisReco_numu,
                sr->EVisReco_ND,
                sr->RecoLepEnNumu,
                sr->Elep_reco);

    double var(0);
    if (!sr->isFD && sr->muon_contained) var = 0.02;
    if (!sr->isFD && sr->muon_tracker) var = 0.01;
    else var = 0.02; // else 2% error in FD
    const double scale = var * sigma;
    if (sr->isCC && abs(sr->nuPDG) == 14) {
      if (sr->isFD) {
        sr->Ev_reco_numu  += sr->RecoLepEnNumu * scale;
        sr->EVisReco_numu += sr->RecoLepEnNumu * scale;
        sr->RecoLepEnNumu += sr->RecoLepEnNumu * scale;
      } else {
        sr->EVisReco_ND += sr->Elep_reco * scale;
        sr->Elep_reco += sr->Elep_reco * scale;
      }
    }
  }

  const EScaleMuLAr kEScaleMuLAr;

  // 5% correlated syst for charged hadrons
  void ChargedHadCorr::Shift(double sigma,
                             Restorer& restore,
                             caf::SRProxy* sr, double& weight) const {

    restore.Add(sr->Ev_reco_numu,
                sr->Ev_reco_nue,
                sr->EVisReco_numu,
                sr->EVisReco_nue,
                sr->EVisReco_ND,
                sr->RecoHadEnNumu,
                sr->RecoHadEnNue,
                sr->HadEVisReco_FD,
                sr->HadEVisReco_ND,
                sr->eRecoP,
                sr->eRecoPip,
                sr->eRecoPim);

    const double scale = .05 * sigma;
    if (sr->isFD) {
      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
      if (sr->eRecoP < 0) sr->eRecoP = 0.;
      if (sr->eRecoPim < 0) sr->eRecoPim = 0.;
      if (sr->eRecoPip < 0) sr->eRecoPip = 0.;
      double sumE = sr->eRecoP + sr->eRecoPip + sr->eRecoPim;
      double sumEdep = sr->eDepP + sr->eDepPip + sr->eDepPim;
      sr->Ev_reco_numu   += sumE * scale;
      sr->Ev_reco_nue    += sumE * scale;
      sr->EVisReco_numu  += sumEdep * scale;
      sr->EVisReco_numu  += sumEdep * scale;
      sr->RecoHadEnNumu  += sumE * scale;
      sr->RecoHadEnNue   += sumE * scale;
      sr->HadEVisReco_FD += sumEdep * scale;
    } else {
      double sumE = sr->eRecoP + sr->eRecoPip + sr->eRecoPim;
      sr->EVisReco_ND    += sumE * scale;
      sr->HadEVisReco_ND += sumE * scale;
    }
  }
  

  const ChargedHadCorr kChargedHadCorr;

  // Neutron energy scale
  // 20% on visible energy
  void NCorr::Shift(double sigma,
                    Restorer& restore,
                    caf::SRProxy* sr, double& weight) const {

    restore.Add(sr->Ev_reco_numu,
                sr->Ev_reco_nue,
                sr->RecoHadEnNumu,
                sr->RecoHadEnNue);

    const double scale = .3 * sigma;
    if (sr->isFD) {
      if (sr->eRecoN < 0.) sr->eRecoN = 0.;
      sr->Ev_reco_numu += sr->eRecoN * scale;
      sr->Ev_reco_nue  += sr->eRecoN * scale;
      sr->RecoHadEnNumu += sr->eRecoN * scale;
      sr->RecoHadEnNue  += sr->eRecoN * scale;
    }
  }

  const NCorr kNCorr;

  // systematic correlated for pi0s and electrons
  // 2.5% on reco energy for electrons and pi0s
  void EMCorr::Shift(double sigma,
                     Restorer& restore,
                     caf::SRProxy* sr, double& weight) const {

    restore.Add(sr->Ev_reco_numu,
                sr->Ev_reco_nue,
                sr->EVisReco_numu,
                sr->EVisReco_nue,
                sr->EVisReco_ND,
                sr->RecoHadEnNumu,
                sr->RecoHadEnNue,
                sr->HadEVisReco_FD,
                sr->HadEVisReco_FD,
                sr->RecoLepEnNue,
                sr->Elep_reco,
                sr->eRecoPi0);

    const double scale = .025 * sigma;
    if (sr->isFD) {
      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
      if (sr->eRecoPi0 < 0) sr->eRecoPi0 = 0.;
      sr->Ev_reco_numu  += sr->eRecoPi0 * scale;
      sr->Ev_reco_nue   += sr->eRecoPi0 * scale;
      sr->EVisReco_numu += sr->eDepPi0 * scale;
      sr->EVisReco_nue  += sr->eDepPi0 * scale;
      sr->RecoHadEnNumu += sr->eRecoPi0 * scale;
      sr->RecoHadEnNue  += sr->eRecoPi0 * scale;
      sr->HadEVisReco_FD+= sr->eDepPi0 * scale;
      if (sr->isCC && abs(sr->nuPDG)==12) {
        sr->Ev_reco_nue  += sr->RecoLepEnNue * scale;
        sr->EVisReco_nue += sr->RecoLepEnNue * scale;
        sr->RecoLepEnNue += sr->RecoLepEnNue * scale;
      }
    } else {
      sr->EVisReco_ND += sr->eRecoPi0 * scale;
      if (sr->isCC && abs(sr->nuPDG) == 12) { 
        sr->EVisReco_ND += sr->Elep_reco * scale;
        sr->Elep_reco += sr->Elep_reco * scale;
      }
    }
  }

  const EMCorr kEMCorr;

  ////////////////////////////////////////////////////////////////////////////////////////////////
  // Resolution systematics
  // Muon energy resolution
  void MuonRes::Shift(double sigma,
                      Restorer& restore,
                      caf::SRProxy* sr, double& weight) const {

    restore.Add(sr->Ev_reco_numu,
                sr->EVisReco_numu,
                sr->RecoLepEnNumu,
                sr->Elep_reco,
                sr->EVisReco_ND);

    const double scale = 0.02 * sigma;
    if (sr->isCC && abs(sr->nuPDG)==14) {
      if (sr->isFD) {
        sr->Ev_reco_numu  += (sr->LepE - sr->RecoLepEnNumu) * scale;
        sr->EVisReco_numu += (sr->LepE - sr->RecoLepEnNumu) * scale;
        sr->RecoLepEnNumu += (sr->LepE - sr->RecoLepEnNumu) * scale;
      } else {
        sr->EVisReco_ND += (sr->LepE - sr->Elep_reco) * scale;
        sr->Elep_reco += (sr->LepE - sr->Elep_reco) * scale;
      }
    }
  }

  const MuonRes kMuonRes;

  // Electron/pi0 energy resolution
  void EMRes::Shift(double sigma,
                    Restorer& restore,
                    caf::SRProxy* sr, double& weight) const {

    restore.Add(sr->Ev_reco_nue,
                sr->Ev_reco_numu,
                sr->EVisReco_nue,
                sr->EVisReco_numu,
                sr->RecoHadEnNumu,
                sr->RecoHadEnNue,
                sr->HadEVisReco_FD,
                sr->RecoLepEnNue,
                sr->eRecoPi0,
                sr->Elep_reco,
                sr->EVisReco_ND);

    const double scale = .02*sigma;
    if (sr->isFD){
      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
      if (sr->eRecoPi0 < 0.) sr->eRecoPi0 = 0.;
      sr->Ev_reco_nue  += (sr->ePi0 - sr->eRecoPi0) * scale;
      sr->Ev_reco_numu += (sr->ePi0 - sr->eRecoPi0) * scale;
      sr->EVisReco_nue += (sr->ePi0 - sr->eDepPi0) * scale;
      sr->EVisReco_numu += (sr->ePi0 - sr->eDepPi0) * scale;
      sr->RecoHadEnNumu += (sr->ePi0 - sr->eRecoPi0) * scale;
      sr->RecoHadEnNue  += (sr->ePi0 - sr->eRecoPi0) * scale;
      sr->HadEVisReco_FD+= (sr->ePi0 - sr->eDepPi0) * scale;
      if (sr->isCC && abs(sr->nuPDG)==12) {
        sr->Ev_reco_nue  += (sr->LepE - sr->RecoLepEnNue) * scale;
        sr->EVisReco_nue += (sr->LepE - sr->RecoLepEnNue) * scale;
        sr->RecoLepEnNue += (sr->LepE - sr->RecoLepEnNue) * scale;
      }
    } else {
      sr->EVisReco_ND += (sr->ePi0 - sr->eRecoPi0) * scale;
      if (sr->isCC && abs(sr->nuPDG)==12) {
        sr->EVisReco_ND += (sr->LepE - sr->Elep_reco) * scale;
        sr->Elep_reco += (sr->LepE - sr->Elep_reco) * scale;
      }
    }
  }

  const EMRes kEMRes;

  // Charged hadron energy resolution
  void ChargedHadRes::Shift(double sigma,
                            Restorer& restore,
                            caf::SRProxy* sr, double& weight) const {
    restore.Add(sr->Ev_reco_nue,
                sr->Ev_reco_numu,
                sr->EVisReco_nue,
                sr->EVisReco_numu,
                sr->RecoHadEnNue,
                sr->RecoHadEnNumu,
                sr->HadEVisReco_FD,
                sr->EVisReco_ND,
                sr->HadEVisReco_ND,
                sr->eRecoP,
                sr->eRecoPip,
                sr->eRecoPim);

    const double scale = .02*sigma;
    if (sr->isFD) {
      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
      if (sr->eRecoP < 0.) sr->eRecoP = 0.;
      if (sr->eRecoPip < 0.) sr->eRecoPip = 0.;
      if (sr->eRecoPim < 0.) sr->eRecoPim = 0.;
      const double trueSum = sr->ePip + sr->ePim + sr->eP;
      const double recoSum = sr->eRecoPip + sr->eRecoPim + sr->eRecoP;
      const double depSum = sr->eDepP + sr->eDepPip + sr->eDepPim;
      sr->Ev_reco_nue    += (trueSum - recoSum) * scale;
      sr->Ev_reco_numu   += (trueSum - recoSum) * scale;
      sr->EVisReco_nue   += (trueSum - depSum)  * scale;
      sr->EVisReco_numu  += (trueSum - depSum)  * scale;
      sr->RecoHadEnNue   += (trueSum - recoSum) * scale;
      sr->RecoHadEnNumu  += (trueSum - recoSum) * scale;
      sr->HadEVisReco_FD += (trueSum - depSum)  * scale;
    } else {
      if (sr->eRecoP < 0.) sr->eRecoP = 0.;
      if (sr->eRecoPip < 0.) sr->eRecoPip = 0.;
      if (sr->eRecoPim < 0.) sr->eRecoPim = 0.;
      const double trueSum = sr->ePip + sr->ePim + sr->eP;
      const double recoSum = sr->eRecoPip + sr->eRecoPim + sr->eRecoP;
      sr->EVisReco_ND += (trueSum - recoSum)   * scale;
      sr->HadEVisReco_ND += (trueSum - recoSum)   * scale;
    }
  }

  const ChargedHadRes kChargedHadRes;

  // Neutron energy resolution
  void NRes::Shift(double sigma,
                   Restorer& restore,
                   caf::SRProxy* sr, double& weight) const {

    restore.Add(sr->Ev_reco_nue,
                sr->Ev_reco_numu,
                sr->RecoHadEnNue,
                sr->RecoHadEnNumu,
                sr->eRecoN);

    const double scale = .1*sigma;
    if (sr->isFD) {
      if (sr->RecoHadEnNue < 0.) sr->RecoHadEnNue = 0.;
      if (sr->eRecoN < 0.) sr->eRecoN = 0.;
      sr->Ev_reco_nue   += (sr->eN - sr->eRecoN) * scale;
      sr->Ev_reco_numu  += (sr->eN - sr->eRecoN) * scale;
      sr->RecoHadEnNue  += (sr->eN - sr->eRecoN) * scale;
      sr->RecoHadEnNumu += (sr->eN - sr->eRecoN) * scale;
    }
  }

  const NRes kNRes;

  std::vector<const ISyst*> GetCorrEnergySysts() {
    std::vector<const ISyst*> vec;
    vec.push_back(&kCorrHadSqrt);
    vec.push_back(&kCorrHadInvSqrt);
    vec.push_back(&kCorrMuSqrt);
    vec.push_back(&kCorrMuInvSqrt);
    // Only implemented for the FD
    //vec.push_back(&kCorrNSqrt);
    //vec.push_back(&kCorrNInvSqrt);
    vec.push_back(&kCorrEMSqrt);
    vec.push_back(&kCorrEMInvSqrt);
    vec.push_back(&kEScaleMuLAr);
    vec.push_back(&kChargedHadCorr);
    vec.push_back(&kNCorr);
    vec.push_back(&kEMCorr);
    // Keep the resolution uncertainties uncorrelated
    //vec.push_back(&kMuonRes);
    //vec.push_back(&kEMRes);
    //vec.push_back(&kChargedHadRes);
    //vec.push_back(&kNRes);

    return vec;
  }

}
