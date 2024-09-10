// Make a simple ND-GAr plot
// cafe pid_scores.C

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

void pid_scores()
{

  // Environment variables and wildcards work. As do SAM datasets.
  //const std::string fname = "/exp/dune/data/users/fmlopez/ND_GAr/auto_jobsub_garg4_100k_seed/caf/ndgar_mini_prod_caf_*.root";
  const std::vector<std::string> fname = { "/pnfs/dune/persistent/users/fmlopez/GAr/ndgar_mini_cafs/hadded/ndgar_mini_prod_caf.root" };

  // I think CAFAna gets the total POT normalization automatically...
  const double pot = 5.0e18;
    
  // Source of events
  SpectrumLoader loader(fname);

  TFile *fout = new TFile("pid_scores.root","RECREATE");

  /* const SRVar kProtonToFScore([](const caf::SRProxy* sr) -> float
                        {
                          // Get vector of muon candidates
                          // no need to check there's one, as this happens after cut
                          std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> candidates = fGetMuonCandidates(sr, muon_score_cut);
                          size_t muon_candidate_idx = candidates[0].first;
                          
                          caf::SRInteractionProxy* interaction = &sr->common.ixn.gsft[0];
                          // Get all particles in the interaction
                          size_t nparts = interaction->part.ngsft;
                          std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> parts;
                          for (size_t i=0; i<nparts; ++i) {
                              parts.push_back(std::make_pair(i, &interaction->part.gsft[i]));
                          }

                          for (auto part: parts) {

                          }

                          return *pions_muons_angle;
                        }); */

  const Binning binsScore = Binning::Simple(20, 0, 1);

  Spectrum sProtonToFScoreProton(loader[kIsTrueNuMuCC && kIsTrueContained()], SRHistAxis("Proton ToF Score", binsScore, kProtonToFScorePDG(2212)));
  Spectrum sProtonToFScorePion(loader[kIsTrueNuMuCC && kIsTrueContained()], SRHistAxis("Proton ToF Score", binsScore, kProtonToFScorePDG(211)));
  Spectrum sProtonToFScoreMuon(loader[kIsTrueNuMuCC && kIsTrueContained()], SRHistAxis("Proton ToF Score", binsScore, kProtonToFScorePDG(13)));
  Spectrum sProtonToFScoreElectron(loader[kIsTrueNuMuCC && kIsTrueContained()], SRHistAxis("Proton ToF Score", binsScore, kProtonToFScorePDG(11)));

  Spectrum sProtondEdxScoreProton(loader[kIsTrueNuMuCC && kIsTrueContained()], SRHistAxis("Proton dEdx Score", binsScore, kProtondEdxScorePDG(2212)));
  Spectrum sProtondEdxScorePion(loader[kIsTrueNuMuCC && kIsTrueContained()], SRHistAxis("Proton dEdx Score", binsScore, kProtondEdxScorePDG(211)));
  Spectrum sProtondEdxScoreMuon(loader[kIsTrueNuMuCC && kIsTrueContained()], SRHistAxis("Proton dEdx Score", binsScore, kProtondEdxScorePDG(13)));
  Spectrum sProtondEdxScoreElectron(loader[kIsTrueNuMuCC && kIsTrueContained()], SRHistAxis("Proton dEdx Score", binsScore, kProtondEdxScorePDG(11)));

  Spectrum sMuonScoreProton(loader[kIsTrueNuMuCC && kIsTrueContained()], SRHistAxis("Proton dEdx Score", binsScore, kMuonScorePDG(2212)));
  Spectrum sMuonScorePion(loader[kIsTrueNuMuCC && kIsTrueContained()], SRHistAxis("Proton dEdx Score", binsScore, kMuonScorePDG(211)));
  Spectrum sMuonScoreMuon(loader[kIsTrueNuMuCC && kIsTrueContained()], SRHistAxis("Proton dEdx Score", binsScore, kMuonScorePDG(13)));
  Spectrum sMuonScoreElectron(loader[kIsTrueNuMuCC && kIsTrueContained()], SRHistAxis("Proton dEdx Score", binsScore, kMuonScorePDG(11)));

  loader.Go();

  fout->cd();

  TH1D* hProtonToFScoreProton(sProtonToFScoreProton.ToTH1(pot));
  hProtonToFScoreProton->SetName("ProtonToFScoreProton");
  hProtonToFScoreProton->Write("ProtonToFScoreProton", TObject::kWriteDelete);

  TH1D* hProtonToFScorePion(sProtonToFScorePion.ToTH1(pot));
  hProtonToFScorePion->SetName("ProtonToFScorePion");
  hProtonToFScorePion->Write("ProtonToFScorePion", TObject::kWriteDelete);

  TH1D* hProtonToFScoreMuon(sProtonToFScoreMuon.ToTH1(pot));
  hProtonToFScoreMuon->SetName("ProtonToFScoreMuon");
  hProtonToFScoreMuon->Write("ProtonToFScoreMuon", TObject::kWriteDelete);

  TH1D* hProtonToFScoreElectron(sProtonToFScoreElectron.ToTH1(pot));
  hProtonToFScoreElectron->SetName("ProtonToFScoreElectron");
  hProtonToFScoreElectron->Write("ProtonToFScoreElectron", TObject::kWriteDelete);



  TH1D* hProtondEdxScoreProton(sProtondEdxScoreProton.ToTH1(pot));
  hProtondEdxScoreProton->SetName("ProtondEdxScoreProton");
  hProtondEdxScoreProton->Write("ProtondEdxScoreProton", TObject::kWriteDelete);

  TH1D* hProtondEdxScorePion(sProtondEdxScorePion.ToTH1(pot));
  hProtondEdxScorePion->SetName("ProtondEdxScorePion");
  hProtondEdxScorePion->Write("ProtondEdxScorePion", TObject::kWriteDelete);

  TH1D* hProtondEdxScoreMuon(sProtondEdxScoreMuon.ToTH1(pot));
  hProtondEdxScoreMuon->SetName("ProtondEdxScoreMuon");
  hProtondEdxScoreMuon->Write("ProtondEdxScoreMuon", TObject::kWriteDelete);

  TH1D* hProtondEdxScoreElectron(sProtondEdxScoreElectron.ToTH1(pot));
  hProtondEdxScoreElectron->SetName("ProtondEdxScoreElectron");
  hProtondEdxScoreElectron->Write("ProtondEdxScoreElectron", TObject::kWriteDelete);



  TH1D* hMuonScoreProton(sMuonScoreProton.ToTH1(pot));
  hMuonScoreProton->SetName("MuonScoreProton");
  hMuonScoreProton->Write("MuonScoreProton", TObject::kWriteDelete);

  TH1D* hMuonScorePion(sMuonScorePion.ToTH1(pot));
  hMuonScorePion->SetName("MuonScorePion");
  hMuonScorePion->Write("MuonScorePion", TObject::kWriteDelete);

  TH1D* hMuonScoreMuon(sMuonScoreMuon.ToTH1(pot));
  hMuonScoreMuon->SetName("MuonScoreMuon");
  hMuonScoreMuon->Write("MuonScoreMuon", TObject::kWriteDelete);

  TH1D* hMuonScoreElectron(sMuonScoreElectron.ToTH1(pot));
  hMuonScoreElectron->SetName("MuonScoreElectron");
  hMuonScoreElectron->Write("MuonScoreElectron", TObject::kWriteDelete);

  fout->Close();

  /* THStack *hStacked = new THStack("hStacked","");
  std::vector<TH1 *> vStacked;

  TH1D* hProtonToFScoreProton(sProtonToFScoreProton.ToTH1(pot, TColor::GetColor("#3f90da")));
  hProtonToFScoreProton->SetFillColor(TColor::GetColor("#3f90da"));
  vStacked.push_back(hProtonToFScoreProton);

  TH1D* hProtonToFScorePion(sProtonToFScorePion.ToTH1(pot, TColor::GetColor("#ffa90e")));
  hProtonToFScorePion->SetFillColor(TColor::GetColor("#ffa90e"));
  vStacked.push_back(hProtonToFScorePion);

  TH1D* hProtonToFScoreMuon(sProtonToFScoreMuon.ToTH1(pot, TColor::GetColor("#bd1f01")));
  hProtonToFScoreMuon->SetFillColor(TColor::GetColor("#bd1f01"));
  vStacked.push_back(hProtonToFScoreMuon);

  TH1D* hProtonToFScoreElectron(sProtonToFScoreElectron.ToTH1(pot, TColor::GetColor("#94a4a2")));
  hProtonToFScoreElectron->SetFillColor(TColor::GetColor("#94a4a2"));
  vStacked.push_back(hProtonToFScoreElectron);

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
  legend.AddEntry( hProtonToFScoreProton, "p" );
  legend.AddEntry( hProtonToFScorePion, "\\pi^{\\pm}" );
  legend.AddEntry( hProtonToFScoreMuon, "\\mu^{\\pm}" );
  legend.AddEntry( hProtonToFScoreElectron, "e^{\\pm}" );
  legend.SetFillStyle(0);
  legend.Draw();

  c->Draw();

  c->SaveAs("proton_tof_score.pdf"); */

}
