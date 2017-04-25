#pragma once

namespace caf
{
  class SRDune
  {
  public:
    // Reco info
    double Ev_reco;
    double mvaresult;

    // Truth info
    double Ev;
    //  float enu_truth; // so what's this one?
    int ccnc;
    int beamPdg;
    int neu;

    int run;

    int LepPDG;
    int mode;
    int nipi0;
    int nipip;
    int nipim;
    double Q2;

    double numu_pid;
    double nue_pid;
  };
}
