// Make a simple ND-GAr plot
// cafe pion_kinetic.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"

#include "GArUtils.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "duneanaobj/StandardRecord/SRGAr.h"

#include <TFile.h>
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TRatioPlot.h"
#include "TLegend.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TColor.h"

#include <string>
#include <vector>
#include <unordered_map>

using namespace ana;

void pion_kinetic(float proton_dEdx_cut, float proton_tof_cut, float delta_calo, float distance_cut)
{

  // Environment variables and wildcards work. As do SAM datasets.
  const std::vector<std::string> fname = { "/pnfs/dune/persistent/users/fmlopez/GAr/ndgar_mini_cafs/hadded/ndgar_mini_prod_caf.root" };
  //const std::string fname = "/pnfs/dune/persistent/users/fmlopez/GAr/ndgar_mini_cafs/ndgar_mini_prod_caf_*.root";

  // I think CAFAna gets the total POT normalization automatically...
  const double pot = 5.0e18;
    
  // Source of events
  SpectrumLoader loader(fname);

  TFile *fout = new TFile("pion_kinetic.root","RECREATE");

  const Binning binsKE  = Binning::Simple(40, 0.0, 1.5);
  const Binning binsAng = Binning::Simple(40, -1.0, 1.0);

  const int flow_vec[4] = { 1, 1, 1, 1 };

  const float muon_score_cut = 0.20;
  const float delta_r = 35.0;

  const float p_thres = 0.08;

  const SRCut kSelectedCut  = kIsContainedNuMuCC(muon_score_cut, delta_r, true, -1);
  //const SRCut kTruthCut = kIsTrueNPi(1, p_thres);
  const SRCut kRecoCut  = kIsNPi(1, muon_score_cut, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut, flow_vec);

  Spectrum sPionKineticEnergyProton(loader[kSelectedCut && kRecoCut], SRHistAxis("Kinetic energy [GeV]", binsKE, kGetPionKEnergyPDG(muon_score_cut, 2212, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut)));
  Spectrum sPionKineticEnergyPion(loader[kSelectedCut && kRecoCut], SRHistAxis("Kinetic energy [GeV]", binsKE, kGetPionKEnergyPDG(muon_score_cut, 211, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut)));
  Spectrum sPionKineticEnergyMuon(loader[kSelectedCut && kRecoCut], SRHistAxis("Kinetic energy [GeV]", binsKE, kGetPionKEnergyPDG(muon_score_cut, 13, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut)));
  Spectrum sPionKineticEnergyElectron(loader[kSelectedCut && kRecoCut], SRHistAxis("Kinetic energy [GeV]", binsKE, kGetPionKEnergyPDG(muon_score_cut, 11, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut)));

  Spectrum sPionMuonAngleProton(loader[kSelectedCut && kRecoCut], SRHistAxis("Cosine angle", binsAng, kGetPionMuAnglePDG(muon_score_cut, 2212, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut)));
  Spectrum sPionMuonAnglePion(loader[kSelectedCut && kRecoCut], SRHistAxis("Cosine angle", binsAng, kGetPionMuAnglePDG(muon_score_cut, 211, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut)));
  Spectrum sPionMuonAngleMuon(loader[kSelectedCut && kRecoCut], SRHistAxis("Cosine angle", binsAng, kGetPionMuAnglePDG(muon_score_cut, 13, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut)));
  Spectrum sPionMuonAngleElectron(loader[kSelectedCut && kRecoCut], SRHistAxis("Cosine angle", binsAng, kGetPionMuAnglePDG(muon_score_cut, 11, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut)));

  loader.Go();

  fout->cd();
  TString dir_name = TString::Format("proton_dEdx_%.2f,proton_tof_%.2f,delta_calo_%.2f,distance_cut_%.2f", proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut);
  fout->mkdir(dir_name);
  fout->cd(dir_name);

  TH1D* hPionKineticEnergyProton(sPionKineticEnergyProton.ToTH1(pot));
  hPionKineticEnergyProton->SetName("PionKineticEnergyProtonSmall");
  hPionKineticEnergyProton->Write("PionKineticEnergyProtonSmall", TObject::kWriteDelete);

  TH1D* hPionKineticEnergyPion(sPionKineticEnergyPion.ToTH1(pot));
  hPionKineticEnergyPion->SetName("PionKineticEnergyPionSmall");
  hPionKineticEnergyPion->Write("PionKineticEnergyPionSmall", TObject::kWriteDelete);

  TH1D* hPionKineticEnergyMuon(sPionKineticEnergyMuon.ToTH1(pot));
  hPionKineticEnergyMuon->SetName("PionKineticEnergyMuonSmall");
  hPionKineticEnergyMuon->Write("PionKineticEnergyMuonSmall", TObject::kWriteDelete);

  TH1D* hPionKineticEnergyElectron(sPionKineticEnergyElectron.ToTH1(pot));
  hPionKineticEnergyElectron->SetName("PionKineticEnergyElectronSmall");
  hPionKineticEnergyElectron->Write("PionKineticEnergyElectronSmall", TObject::kWriteDelete);

  /* TH1D* hPionMuonAngleProton(sPionMuonAngleProton.ToTH1(pot));
  hPionMuonAngleProton->SetName("PionMuonAngleProton");
  hPionMuonAngleProton->Write("PionMuonAngleProton", TObject::kWriteDelete);

  TH1D* hPionMuonAnglePion(sPionMuonAnglePion.ToTH1(pot));
  hPionMuonAnglePion->SetName("PionMuonAnglePion");
  hPionMuonAnglePion->Write("PionMuonAnglePion", TObject::kWriteDelete);

  TH1D* hPionMuonAngleMuon(sPionMuonAngleMuon.ToTH1(pot));
  hPionMuonAngleMuon->SetName("PionMuonAngleMuon");
  hPionMuonAngleMuon->Write("PionMuonAngleMuon", TObject::kWriteDelete);

  TH1D* hPionMuonAngleElectron(sPionMuonAngleElectron.ToTH1(pot));
  hPionMuonAngleElectron->SetName("PionMuonAngleElectron");
  hPionMuonAngleElectron->Write("PionMuonAngleElectron", TObject::kWriteDelete); */

  fout->Close();

  /* THStack *hStacked = new THStack("hStacked","");
  std::vector<TH1 *> vStacked;

  hPionKineticEnergyProton->SetFillColor(TColor::GetColor("#3f90da"));
  vStacked.push_back(hPionKineticEnergyProton);

  hPionKineticEnergyPion->SetFillColor(TColor::GetColor("#ffa90e"));
  vStacked.push_back(hPionKineticEnergyPion);

  hPionKineticEnergyMuon->SetFillColor(TColor::GetColor("#bd1f01"));
  vStacked.push_back(hPionKineticEnergyMuon);

  hPionKineticEnergyElectron->SetFillColor(TColor::GetColor("#94a4a2"));
  vStacked.push_back(hPionKineticEnergyElectron);

  std::sort(vStacked.begin(), vStacked.end(),
            [](TH1 *a, TH1 *b) { return a->Integral() < b->Integral(); });

  
  for (auto h: vStacked) {
    hStacked->Add(h);
  }

  TCanvas *c = new TCanvas("c", "canvas", 800, 600);

  c->SetLeftMargin(0.15);
  c->SetBottomMargin(0.10);
  c->SetGrid(); // Grid

  hStacked->Draw("hist");

  // Y axis ratio plot settings
  hStacked->GetYaxis()->SetTitle("Counts");
  hStacked->GetYaxis()->SetNdivisions(505);
  hStacked->GetYaxis()->SetTitleSize(25);
  hStacked->GetYaxis()->SetTitleFont(43);
  hStacked->GetYaxis()->SetTitleOffset(2.5);
  hStacked->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  hStacked->GetYaxis()->SetLabelSize(25);

  // X axis ratio plot settings
  hStacked->GetXaxis()->SetTitle("Proton ToF Score");
  hStacked->GetXaxis()->SetTitleSize(25);
  hStacked->GetXaxis()->SetTitleFont(43);
  hStacked->GetXaxis()->SetTitleOffset(1.0);
  hStacked->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
  hStacked->GetXaxis()->SetLabelSize(25);

  TLegend legend( .45, .50, .85, .85 );
  legend.SetTextSize(0.05);
  legend.AddEntry( hPionKineticEnergyProton, "p" );
  legend.AddEntry( hPionKineticEnergyPion, "\\pi^{\\pm}" );
  legend.AddEntry( hPionKineticEnergyMuon, "\\mu^{\\pm}" );
  legend.AddEntry( hPionKineticEnergyElectron, "e^{\\pm}" );
  legend.SetFillStyle(0);
  legend.Draw();

  c->Draw();

  c->SaveAs("pion_kinetic_energy.pdf"); */

}
