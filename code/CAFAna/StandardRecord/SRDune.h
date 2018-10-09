#pragma once

#include <vector>

namespace caf
{
  class SRDune
  {
  public:
    // Reco info
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

    // To prevent errors when compiling DUNENDSysts
    double nue_pid;
    double numu_pid;

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
    int mode;
    int nipi0;
    int nipip;
    int nipim;
    double Q2;
    double W;
    double Y;

    double nuvtxx_truth;
    double nuvtxy_truth;
    double nuvtxz_truth;

    // config
    int run;
    int isFD;
    int isFHC;

    // sigmas
    double sigma_Ev_reco;
    double sigma_Elep_reco;
    double sigma_numu_pid;
    double sigma_nue_pid;

    // First index is systematic ID
    std::vector<std::vector<double>> genie_wgt;
  };
}
