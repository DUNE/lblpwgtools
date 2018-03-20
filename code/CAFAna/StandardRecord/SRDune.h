#pragma once

namespace caf
{
  class SRDune
  {
  public:
    // Reco info
    double Ev_reco;
    double Ev_reco_nue;
    double Ev_reco_numu;
    double mvaresult;
    double mvanue;
    double mvanumu;
    double cvnnue;
    double cvnnumu;
    int reco_q;
    double Elep_reco;

    // To prevent errors when compiling DUNENDSysts
    double nue_pid;
    double numu_pid;

    // Truth info
    double Ev;
    double Elep;
    //  float enu_truth; // so what's this one?
    int ccnc;
    int cc;
    int beamPdg;
    int neu;
    int LepPDG;
    int mode;
    int nipi0;
    int nipip;
    int nipim;
    double Q2;
    double W;
    double Y;

    // config
    int run;
    int isFD;
    int isFHC;

    // sigmas
    double sigma_Ev_reco;
    double sigma_Elep_reco;
    double sigma_numu_pid;
    double sigma_nue_pid;
  };
}
