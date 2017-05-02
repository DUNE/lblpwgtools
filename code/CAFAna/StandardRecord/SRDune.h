#pragma once

namespace caf
{
  class SRDune
  {
  public:
    // Reco info
    double Ev_reco;
    double mvaresult;
    double numu_pid;
    double nue_pid;
    int reco_q;
    double Elep_reco;

    // Truth info
    double Ev;
    double Elep;
    //  float enu_truth; // so what's this one?
    int ccnc;
    int beamPdg;
    int neu;
    int LepPDG;
    int mode;
    int nipi0;
    int nipip;
    int nipim;
    double Q2;
    double W;

    // config
    int run;
    int isFD;
    int isFHC;

  };
}
