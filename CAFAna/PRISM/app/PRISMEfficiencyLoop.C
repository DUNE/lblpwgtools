#include "CAFAna/Cuts/AnaCuts.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/PRISM/Cuts.h"
#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"

#include "StandardRecord/StandardRecord.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TH2.h"
#include "TStyle.h"
#include "TTree.h"

#include <cassert>
#include <iostream>
#include <string>

//----------------------------------------------------------------------
// Helper function that can give us a friendlier error message
template <class T>
bool SetBranchChecked(TTree *tr, const std::string &bname, T *dest) {
  if (tr->FindBranch(bname.c_str())) {
    tr->SetBranchAddress(bname.c_str(), dest);
    return true;
  } else {
    std::cout << "Warning: Branch '" << bname
              << "' not found, field will not be filled" << std::endl;
  }
  return false;
}

int main(int argc, char const *argv[]) {

  if (argc != 2) {
    std::cout << "[ERROR]: Expected to be passed a single input CAFFile"
              << std::endl;
  }

  TFile *f = TFile::Open(argv[1]);

  TTree *tr;
  tr = (TTree *)f->Get("caf");
  if (!tr) {
    tr = (TTree *)f->Get("cafTree");
  }
  assert(tr);

  Long64_t Nent = tr->GetEntries();

  caf::StandardRecord sr;
  SetBranchChecked(tr, "Ev_reco", &sr.Ev_reco);
  SetBranchChecked(tr, "Ev_reco_nue", &sr.Ev_reco_nue);
  SetBranchChecked(tr, "Ev_reco_numu", &sr.Ev_reco_numu);
  SetBranchChecked(tr, "Elep_reco", &sr.Elep_reco);
  SetBranchChecked(tr, "theta_reco", &sr.theta_reco);
  SetBranchChecked(tr, "mvaresult", &sr.mvaresult);
  SetBranchChecked(tr, "mvanue", &sr.mvanue);
  SetBranchChecked(tr, "mvanumu", &sr.mvanumu);
  SetBranchChecked(tr, "cvnnue", &sr.cvnnue);
  SetBranchChecked(tr, "cvnnumu", &sr.cvnnumu);
  SetBranchChecked(tr, "numu_pid", &sr.numu_pid);
  SetBranchChecked(tr, "nue_pid", &sr.nue_pid);
  SetBranchChecked(tr, "reco_q", &sr.reco_q);
  SetBranchChecked(tr, "RecoLepEnNue", &sr.RecoLepEnNue);
  SetBranchChecked(tr, "RecoHadEnNue", &sr.RecoHadEnNue);
  SetBranchChecked(tr, "RecoLepEnNumu", &sr.RecoLepEnNumu);
  SetBranchChecked(tr, "RecoHadEnNumu", &sr.RecoHadEnNumu);
  // ND pseudo-reconstruction flags
  SetBranchChecked(tr, "reco_numu", &sr.reco_numu);
  SetBranchChecked(tr, "reco_nue", &sr.reco_nue);
  SetBranchChecked(tr, "reco_nc", &sr.reco_nc);
  // CW: add variables that Chris (M) wants for ND selections
  SetBranchChecked(tr, "muon_exit", &sr.muon_exit);
  SetBranchChecked(tr, "muon_contained", &sr.muon_contained);
  SetBranchChecked(tr, "muon_ecal", &sr.muon_ecal);
  SetBranchChecked(tr, "muon_tracker", &sr.muon_tracker);
  SetBranchChecked(tr, "Ehad_veto", &sr.Ehad_veto);

  SetBranchChecked(tr, "Ev", &sr.Ev);
  SetBranchChecked(tr, "Elep", &sr.Elep);
  //    SetBranchChecked(tr, "ccnc", &sr.ccnc);
  SetBranchChecked(tr, "isCC", &sr.isCC);
  //    SetBranchChecked(tr, "beamPdg", &sr.beamPdg);
  //    SetBranchChecked(tr, "neu", &sr.neu);
  SetBranchChecked(tr, "nuPDG", &sr.nuPDG);
  SetBranchChecked(tr, "nuPDGunosc", &sr.nuPDGunosc);
  SetBranchChecked(tr, "LepPDG", &sr.LepPDG);
  SetBranchChecked(tr, "mode", &sr.mode);
  SetBranchChecked(tr, "nP", &sr.nP);
  SetBranchChecked(tr, "nN", &sr.nN);
  SetBranchChecked(tr, "nipi0", &sr.nipi0);
  SetBranchChecked(tr, "nipip", &sr.nipip);
  SetBranchChecked(tr, "nipim", &sr.nipim);
  SetBranchChecked(tr, "niem", &sr.niem);
  SetBranchChecked(tr, "Q2", &sr.Q2);
  SetBranchChecked(tr, "W", &sr.W);
  SetBranchChecked(tr, "Y", &sr.Y);
  SetBranchChecked(tr, "X", &sr.X);
  //    SetBranchChecked(tr, "cc", &sr.cc);
  SetBranchChecked(tr, "NuMomX", &sr.NuMomX);
  SetBranchChecked(tr, "NuMomY", &sr.NuMomY);
  SetBranchChecked(tr, "NuMomZ", &sr.NuMomZ);
  SetBranchChecked(tr, "LepMomX", &sr.LepMomX);
  SetBranchChecked(tr, "LepMomY", &sr.LepMomY);
  SetBranchChecked(tr, "LepMomZ", &sr.LepMomZ);
  SetBranchChecked(tr, "LepE", &sr.LepE);
  SetBranchChecked(tr, "LepNuAngle", &sr.LepNuAngle);

  // Numu track containment flag
  SetBranchChecked(tr, "LongestTrackContNumu", &sr.LongestTrackContNumu);

  SetBranchChecked(tr, "vtx_x", &sr.vtx_x);
  SetBranchChecked(tr, "vtx_y", &sr.vtx_y);
  SetBranchChecked(tr, "vtx_z", &sr.vtx_z);

  SetBranchChecked(tr, "det_x", &sr.det_x);

  SetBranchChecked(tr, "eP", &sr.eP);
  SetBranchChecked(tr, "eN", &sr.eN);
  SetBranchChecked(tr, "ePip", &sr.ePip);
  SetBranchChecked(tr, "ePim", &sr.ePim);
  SetBranchChecked(tr, "ePi0", &sr.ePi0);
  SetBranchChecked(tr, "eOther", &sr.eOther);
  SetBranchChecked(tr, "eRecoP", &sr.eRecoP);
  SetBranchChecked(tr, "eRecoN", &sr.eRecoN);
  SetBranchChecked(tr, "eRecoPip", &sr.eRecoPip);
  SetBranchChecked(tr, "eRecoPim", &sr.eRecoPim);
  SetBranchChecked(tr, "eRecoPi0", &sr.eRecoPi0);
  SetBranchChecked(tr, "eRecoOther", &sr.eRecoOther);

  SetBranchChecked(tr, "eDepP", &sr.eDepP);
  SetBranchChecked(tr, "eDepN", &sr.eDepN);
  SetBranchChecked(tr, "eDepPip", &sr.eDepPip);
  SetBranchChecked(tr, "eDepPim", &sr.eDepPim);
  SetBranchChecked(tr, "eDepPi0", &sr.eDepPi0);
  SetBranchChecked(tr, "eDepOther", &sr.eDepOther);

  SetBranchChecked(tr, "run", &sr.run);
  SetBranchChecked(tr, "isFD", &sr.isFD);
  SetBranchChecked(tr, "isFHC", &sr.isFHC);

  SetBranchChecked(tr, "sigma_Ev_reco", &sr.sigma_Ev_reco);
  SetBranchChecked(tr, "sigma_Elep_reco", &sr.sigma_Elep_reco);
  SetBranchChecked(tr, "sigma_numu_pid", &sr.sigma_numu_pid);
  SetBranchChecked(tr, "sigma_nue_pid", &sr.sigma_nue_pid);

  ana::Cut IsNue = ana::kActuallyIsNueCC;
  ana::Cut IsSignal = ana::kIsTrueFV && PRISM::kIsOutOfTheDesert && IsNue;
  ana::Cut IsSignalSelected = IsSignal && ana::kPassND_FHC_NUE;

  TH2D *sig = new TH2D("sig", ";enu (GeV);offaxis_x (m);count", 100, 0, 10,
                       64 * 2, -2, 30);
  TH2D *sig_sel = new TH2D("sig_sel", ";enu (GeV);offaxis_x (m);count", 100, 0,
                           10, 64 * 2, -2, 30);

  std::cout << std::endl;
  for (Long64_t ent = 0; ent < Nent; ++ent) {
    tr->GetEntry(ent);

    if (!(ent % 1000)) {
      std::cout << "\r[READ] " << ent << "/" << Nent << std::flush;
    }

    if (!IsNue((caf::SRProxy *)&sr)) {
      continue;
    }

    std::cout << "\rEntry: " << ent << ", nuPDG: " << sr.nuPDG
              << ", Enu = " << sr.Ev << ", vtx_x: " << sr.vtx_x
              << ", det_x: " << sr.det_x
              << "\n\tinFV: " << ana::kIsTrueFV((caf::SRProxy *)&sr)
              << ", outofdesert: "
              << PRISM::kIsOutOfTheDesert((caf::SRProxy *)&sr)
              << ", NDNueSig: " << IsSignal((caf::SRProxy *)&sr)
              << ", NDNueSel: " << IsSignalSelected((caf::SRProxy *)&sr)
              << std::endl;

    sig->Fill(sr.Ev, sr.det_x + sr.vtx_x * 1.0E-2,
              IsSignal((caf::SRProxy *)&sr));
    sig_sel->Fill(sr.Ev, sr.det_x + sr.vtx_x * 1.0E-2,
                  IsSignalSelected((caf::SRProxy *)&sr));
  }
  std::cout << "\r[READ] " << Nent << "/" << Nent << std::endl;

  gStyle->SetOptStat(false);
  TCanvas *c1 = new TCanvas("c1", "");

  sig_sel->Divide(sig);
  sig_sel->Draw("COLZ");

  c1->Print("nue_eff.pdf");

  f->Close();
}
