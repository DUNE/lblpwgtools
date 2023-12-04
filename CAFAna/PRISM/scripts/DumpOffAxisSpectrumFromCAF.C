#include "CAFAna/Core/Progress.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

#include <iostream>

void DumpOffAxisSpectrumFromCAF(std::string const &prism_file) {
  TFile f(prism_file.c_str());

  TH1D *POTExposure = nullptr;
  f.GetObject("POTExposure", POTExposure);
  assert(POTExposure);

  TH2D *POTExposure_stop = nullptr;
  f.GetObject("POTExposure_stop", POTExposure_stop);
  assert(POTExposure_stop);

  TTree *caf = nullptr;
  f.GetObject("cafTree", caf);
  assert(caf);

  TTree *POTWeightFriend = nullptr;
  f.GetObject("POTWeightFriend", POTWeightFriend);
  assert(POTWeightFriend);

  caf->AddFriend(POTWeightFriend);

  double vtx_x;
  double det_x;

  int nuPDG;
  int isCC;

  int reco_numu;
  int muon_contained;
  int muon_tracker;
  int reco_q;
  double Ehad_veto;

  caf->SetBranchAddress("vtx_x", &vtx_x);
  caf->SetBranchAddress("det_x", &det_x);

  caf->SetBranchAddress("nuPDG", &nuPDG);
  caf->SetBranchAddress("isCC", &isCC);

  caf->SetBranchAddress("reco_numu", &reco_numu);
  caf->SetBranchAddress("muon_contained", &muon_contained);
  caf->SetBranchAddress("muon_tracker", &muon_tracker);
  caf->SetBranchAddress("reco_q", &reco_q);
  caf->SetBranchAddress("Ehad_veto", &Ehad_veto);

  double perPOT, perFile;
  POTWeightFriend->SetBranchAddress("perPOT", &perPOT);
  POTWeightFriend->SetBranchAddress("perFile", &perFile);

  size_t nevs = caf->GetEntries();
  size_t nevs_pwf = POTWeightFriend->GetEntries();

  if (nevs != nevs_pwf) {
    std::cout
        << "[WARN]: Mismatched entries between event tree and POT weight tree: "
        << nevs << " != " << nevs_pwf << std::endl;
  }

  double min_m = -4;
  double max_m = 40;
  double det_min_m = -4;
  double det_max_m = 4;
  double step_m = 0.25; // 50 cm
  double offset = 0;    //- (step_m / 2.0);
  size_t NStep = (max_m - min_m) / step_m;
  size_t NDetSteps = (det_max_m - det_min_m) / step_m;

  // Add FV mass correction.
  TH1D *FVMassCorr = new TH1D("FVMassCorr", "", NDetSteps, det_min_m - offset,
                              det_max_m - offset);
  double avg_step = 1E-6;
  size_t const navg_steps = step_m / avg_step;

  for (int bi_it = 0; bi_it < FVMassCorr->GetXaxis()->GetNbins(); ++bi_it) {
    double bin_low_edge = FVMassCorr->GetXaxis()->GetBinLowEdge(bi_it + 1);
    for (size_t avg_step_it = 0; avg_step_it < navg_steps; ++avg_step_it) {
      double pos_m = bin_low_edge + (double(avg_step_it) * avg_step);
      if (!ana::IsInNDFV(pos_m * 1E2, 0, 150)) {
        continue;
      }
      FVMassCorr->Fill(pos_m, 1.0 / double(navg_steps));
    }
  }

  TH1D *oap_unweighted =
      new TH1D("oap_unweighted", "", NStep, min_m - offset, max_m - offset);
  TH1D *oap = new TH1D("oap", "", NStep, min_m - offset, max_m - offset);

  TH1D *oap_numu =
      new TH1D("oap_numu", "", NStep, min_m - offset, max_m - offset);

  TH1D *oap_numu_VertDesert = new TH1D("oap_numu_VertDesert", "", NStep,
                                       min_m - offset, max_m - offset);

  TH1D *oap_numu_fw =
      new TH1D("oap_numu_fw", "", NStep, min_m - offset, max_m - offset);

  TH1D *oap_numu_VertDesert_fw = new TH1D("oap_numu_VertDesert_fw", "", NStep,
                                          min_m - offset, max_m - offset);

  TH1D *oap_numu_sel =
      new TH1D("oap_numu_sel", "", NStep, min_m - offset, max_m - offset);

  TH1D *oap_numu_sel_LAr_cont = new TH1D("oap_numu_sel_LAr_cont", "", NStep,
                                         min_m - offset, max_m - offset);

  TH1D *oap_numu_sel_tracker = new TH1D("oap_numu_sel_tracker", "", NStep,
                                        min_m - offset, max_m - offset);

  TH2D *oap_numu_FV_2D = new TH2D("oap_numu_FV_2D", "", NStep, min_m - offset,
                                  max_m - offset, 10, 0, 40);
  TH2D *oap_numu_VertDesert_2D =
      new TH2D("oap_numu_VertDesert_2D", "", NStep, min_m - offset,
               max_m - offset, 10, 0, 40);

  TH2D *oap_numu_sel_VertDesert_2D =
      new TH2D("oap_numu_sel_VertDesert_2D", "", NStep, min_m - offset,
               max_m - offset, 10, 0, 40);

  FVMassCorr->SetDirectory(nullptr);
  oap_unweighted->SetDirectory(nullptr);
  oap->SetDirectory(nullptr);
  oap_numu->SetDirectory(nullptr);
  oap_numu_VertDesert->SetDirectory(nullptr);
  oap_numu_sel->SetDirectory(nullptr);
  oap_numu_sel_LAr_cont->SetDirectory(nullptr);
  oap_numu_sel_tracker->SetDirectory(nullptr);
  oap_numu_FV_2D->SetDirectory(nullptr);
  oap_numu_VertDesert_2D->SetDirectory(nullptr);
  oap_numu_sel_VertDesert_2D->SetDirectory(nullptr);

  int xoverflw = POTExposure_stop->GetXaxis()->GetNbins() + 1;
  int yoverflw = POTExposure_stop->GetYaxis()->GetNbins() + 1;

  ana::Progress prog("Events processing.");

  for (size_t i = 0; i < nevs; ++i) {
    caf->GetEntry(i);

    double abs_pos_m = det_x + (vtx_x * 1.0E-2);

    if (i && !(i % 50000)) {
      prog.SetProgress(double(i) / double(nevs));
    }

    if (!ana::IsInNDFV(vtx_x, 0, 150)) {
      continue;
    }

    oap_unweighted->Fill(abs_pos_m);

    size_t xbi = POTExposure_stop->GetXaxis()->FindFixBin(abs_pos_m);
    size_t ybi = POTExposure_stop->GetYaxis()->FindFixBin(det_x);

    double w = 1.0 / POTExposure_stop->GetBinContent(xbi, ybi);

    size_t det_xbi = FVMassCorr->GetXaxis()->FindFixBin(vtx_x*1E-2);
    double fvmassw = 1.0 / FVMassCorr->GetBinContent(det_xbi);

    oap->Fill(abs_pos_m, w);

    bool vertexDesert = (fabs(vtx_x) < 200);

    bool sel_LAr_cont =
        reco_numu && muon_contained && reco_q == -1 && (Ehad_veto < 30);

    bool sel_LAr_trkr =
        reco_numu && muon_tracker && reco_q == -1 && (Ehad_veto < 30);

    bool sel = sel_LAr_cont || sel_LAr_trkr;

    bool numu = (nuPDG == 14) && isCC;

    oap_numu->Fill(abs_pos_m, w * numu);
    oap_numu_fw->Fill(abs_pos_m, perPOT * perFile * fvmassw * numu);
    oap_numu_FV_2D->Fill(abs_pos_m, det_x, w * numu);
    oap_numu_VertDesert->Fill(abs_pos_m, w * numu * vertexDesert);
    oap_numu_VertDesert_fw->Fill(abs_pos_m, perPOT * perFile * fvmassw * numu *
                                                vertexDesert);
    oap_numu_VertDesert_2D->Fill(abs_pos_m, det_x, w * numu * vertexDesert);
    oap_numu_sel->Fill(abs_pos_m, w * sel * numu);
    oap_numu_sel_LAr_cont->Fill(abs_pos_m, w * sel_LAr_cont * numu);
    oap_numu_sel_tracker->Fill(abs_pos_m, w * sel_LAr_trkr * numu);
  }
  prog.Done();

  TFile fout("OAPSpec_cafcomb.root", "RECREATE");
  FVMassCorr->Write("FVMassCorr");
  oap_unweighted->Write("oap_unweighted");
  oap->Write("oap");
  oap_numu->Write("oap_numu");
  oap_numu_VertDesert->Write("oap_numu_VertDesert");
  oap_numu_fw->Write("oap_numu_fw");
  oap_numu_VertDesert_fw->Write("oap_numu_VertDesert_fw");
  oap_numu_sel->Write("oap_numu_sel");
  oap_numu_sel_LAr_cont->Write("oap_numu_sel_LAr_cont");
  oap_numu_sel_tracker->Write("oap_numu_sel_tracker");
  oap_numu_FV_2D->Write("oap_numu_FV_2D");
  oap_numu_VertDesert_2D->Write("oap_numu_VertDesert_2D");
  fout.Write();
}
