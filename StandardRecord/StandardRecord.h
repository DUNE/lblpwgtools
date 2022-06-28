////////////////////////////////////////////////////////////////////////
// \file    StandardRecord.h
// \brief   The StandardRecord is the primary top-level object in the 
//          Common Analysis File trees.   
// \version $Id: StandardRecord.h,v 1.7 2012-12-06 20:18:33 rocco Exp $
// \author  $Author: rocco $
// \date    $Date: 2012-12-06 20:18:33 $
////////////////////////////////////////////////////////////////////////
#ifndef STANDARDRECORD_H
#define STANDARDRECORD_H

#include <vector>

/// Common Analysis Files
namespace caf
{
  
  static std::size_t const kMaxSystUniverses = 10;

  /// \brief   The StandardRecord is the primary top-level object in the 
  ///          Common Analysis File trees.   
  
  class StandardRecord  
  {
    
  public:
    StandardRecord();
    ~StandardRecord();

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

    double RecoLepE_NDFD;
    double RecoHadE_NDFD;
    double VisReco_NDFD;
    double ProxyRecoLepE;

    // ND pseudo-reconstruction flags
    int reco_numu;
    int reco_nue;
    int reco_nc;

    int simple_reco_numu;

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

    // True sum of hadronic energy
    double HadE;
    // True sum of charged pion KE
    double ePipm;
    // True total energy of pi0 (KE + mass)
    double eTotalPi0;

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
    int nikp;
    int nikm;
    int nik0;
    int niem;
    int nNucleus;
    double Q2;
    double W;
    double Y;
    double X;

    double vtx_x;
    double vtx_y;
    double vtx_z;

    // Near detector offset in m
    double det_x;
    // Absolute position in ND 
    double abspos_x;

    // True energy of particles by species
    double eP;
    double eN;
    double ePip;
    double ePim;
    double ePi0;
    double eOther;
    
    // A true-defined calorimetric erec (Elep + TProt + Epi0 + TPiChg + Tother)
    double eRecProxy;

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
    std::vector<double> wgt_CrazyFlux;

    // First index is systematic ID
    std::vector<std::vector<double>> xsSyst_wgt;
    double total_xsSyst_cv_wgt;
    // For correctly combining off axis samples
    double perPOTWeight;
    double perFileWeight;
    double NDMassCorrWeight;

    // For fakeing/consuming non-standard HC runs.
    double SpecialRunWeight;
    // Allows for multiple special runs to be included, but we will likely only use one @ 280kA.
    int SpecialHCRunId;

#ifdef USE_TH2JAGGED
    // Cache the ND flux bin when using off-axis flux matrices.
    int OffAxisFluxBin;
    int OffAxisFluxConfig;
#endif
  };
  
} // end namespace

#endif // STANDARDRECORD_H
//////////////////////////////////////////////////////////////////////////////
