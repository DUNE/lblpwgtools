#pragma once

#include <vector>

namespace caf
{
  class SRDune
  {
  public:
    // Reco info
    double eRec_FromDep; // Unified parameterized reco that can be used at near and far. Should only be used for missing proton energy fake data studies that cannot use the CVN FD Reco
    double Ev_reco; // for ND?
    double Ev_reco_nue;
    double Ev_reco_numu;
    double mvaresult;
    double mvanue;
    double mvanumu;
    double cvnnue;
    double cvnnumu;
    double cvnnutau;
    int reco_q;
    double Elep_reco;
    double theta_reco;

    double RecoLepEnNue;
    double RecoHadEnNue;
    double RecoLepEnNumu;
    double RecoHadEnNumu;

    // ND pseudo-reconstruction flags
    int reco_numu;
    int reco_nue;
    int reco_nc;

    // CW: added for the ND cuts Chris (M) wants
    // ND containment flags
    int muon_contained;
    int muon_tracker;
    int muon_ecal;
    int muon_exit;
    double Ehad_veto;

    // To prevent errors when compiling DUNENDSysts
    double nue_pid;
    double numu_pid;

    // Containment flag
    int LongestTrackContNumu;

    // Truth info
    double Ev;
    double Elep;
    //  float enu_truth; // so what's this one?
    int isCC;
    //    int ccnc;
    //    int cc;
    //    int beamPdg;
    //    int neu;
    int nuPDG;
    int nuPDGunosc;
    int LepPDG;

    // This mode depends on whether the file is ND/FD
    // See converter in SpectrumLoader.cxx that fills GENIE_ScatteringMode
    int mode;

    /// Modes list:
    /// * QE: 1
    /// * Single Kaon: 2
    /// * DIS: 3
    /// * RES: 4
    /// * COH: 5
    /// * Diffractive: 6
    /// * Nu-e El: 7
    /// * IMD: 8
    /// * AMnuGamma: 9
    /// * MEC: 10
    /// * COHEl: 11
    /// * IBD: 12
    /// * GlashowRES: 13
    /// * IMDAnnihalation: 14
    int GENIE_ScatteringMode;
    int nP;
    int nN;
    int nipi0;
    int nipip;
    int nipim;
    double Q2;
    double W;
    double Y;
    double X;

    double vtx_x;
    double vtx_y;
    double vtx_z;

    // Near detector offset in m
    double det_x;

    // True energy of particles by species
    double eP;
    double eN;
    double ePip;
    double ePim;
    double ePi0;
    double eOther;

    // Reconstructed energy of particles by species
    double eRecoP;
    double eRecoN;
    double eRecoPip;
    double eRecoPim;
    double eRecoPi0;
    double eRecoOther;

    //At FD
    double eDepP;
    double eDepN;
    double eDepPip;
    double eDepPim;
    double eDepPi0;
    double eDepOther;

    double NuMomX;
    double NuMomY;
    double NuMomZ;
    double LepMomX;
    double LepMomY;
    double LepMomZ;
    double LepE;
    double LepNuAngle;

    // config
    int run;
    int isFD;
    int isFHC;

    // sigmas
    double sigma_Ev_reco;
    double sigma_Elep_reco;
    double sigma_numu_pid;
    double sigma_nue_pid;
    double total_cv_wgt;

    // First index is systematic ID
    std::vector<std::vector<double>> genie_wgt;
    std::vector<double> genie_cv_wgt;
  };
}
