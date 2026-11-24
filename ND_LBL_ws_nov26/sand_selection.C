// my_analysis_stack.C
// Plot: (1) stack reco Enu per categoria vera (numuCC / numuNC / altro)
//       (2) confronto totale vs numuCC nel SAND FV
//       (3) vertici: totale, numuCC in FV, e divisione CC / NC / altro
// Solo campione nominale (niente varied -> un solo loader, più veloce).

#include <iostream>
#include <iomanip>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "duneanaobj/StandardRecord/Navigate.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TSystem.h"

using namespace ana;


caf::SRVector3D NDHallToSANDCoordinates(const caf::SRVector3DProxy& ndhall_coords)
{
  caf::SRVector3D sand_coords;
  sand_coords.x = ndhall_coords.x;
  sand_coords.y = ndhall_coords.y + 238.473;  // cm
  sand_coords.z = ndhall_coords.z - 2391.0;   // cm
  return sand_coords;
}

namespace mysel {

  // TRUE selection
  enum TrueSelection { kNuMuCCTrue = 0, kNuMuNCTrue = 1, kOtherTrue = 2 };

  TrueSelection GetTrueCategory(const caf::SRInteractionProxy* recoint)
  {
    if (recoint->truth.size() == 0) return kOtherTrue;   // no match

    const caf::SRProxy* top = recoint->Ancestor<caf::SRProxy>();
    const caf::SRTrueInteractionProxy* t = caf::FindInteraction(top->mc, recoint->truth[0]);
    if (t == nullptr) return kOtherTrue;

    if (std::abs(static_cast<int>(t->pdg)) == 14) {      // nu_mu, nu_mu bar
      if (t->iscc) return kNuMuCCTrue;
      return kNuMuNCTrue;
    }
    return kOtherTrue;                                    // nue, nutau, ...
  }

  // RECO selection
  bool HasSandParticles(const caf::SRInteractionProxy* recoint)
  {
    return recoint->part.sandreco.size() > 0;
  }

  int NPrimaryMuons(const caf::SRInteractionProxy* recoint)
  {
    int n = 0;
    for (const auto& recopart : recoint->part.sandreco) {
      if (recopart.primary == 1 && static_cast<int>(recopart.pdg) == 13) n++;
    }
    return n;
  }

  int NPrimaryAntiMuons(const caf::SRInteractionProxy* recoint)
  {
    int n = 0;
    for (const auto& recopart : recoint->part.sandreco) {
      if (recopart.primary == 1 && static_cast<int>(recopart.pdg) == -13) n++;
    }
    return n;
  }

  int NPrimaryElectrons(const caf::SRInteractionProxy* recoint)
  {
    int n = 0;
    for (const auto& recopart : recoint->part.sandreco) {
      if (recopart.primary == 1 && std::abs(static_cast<int>(recopart.pdg)) == 11) n++;
    }
    return n;
  }

  // fiducial cut for the front ECAL:
  // considering the center of SAND, we get a region with 2.0 < r < 2.3 m
  // and z < 0.910 (which actually is z < 23 m in global NDHall coordinates)
  bool inFrontECAL(const caf::SRInteractionProxy* recoint)
  {
    auto nu_vtx = NDHallToSANDCoordinates(recoint->vtx);
    auto radius = std::sqrt(std::pow(nu_vtx.y, 2) + std::pow(nu_vtx.z, 2));
    const double z_cut_loc = 2300.0 - 2391.0;
    return radius < 230 && radius > 200 && nu_vtx.z < z_cut_loc && std::abs(nu_vtx.x) < 169;
  }

  bool inSANDInnerVol(const caf::SRInteractionProxy* recoint)
  {
    auto nu_vtx = NDHallToSANDCoordinates(recoint->vtx);
    auto radius = std::sqrt(std::pow(nu_vtx.y, 2) + std::pow(nu_vtx.z, 2));
    return radius < 190 && std::abs(nu_vtx.x) < 159;
  }

  // SAND FV = volume interno OR front ECAL
  bool inSANDFV(const caf::SRInteractionProxy* recoint)
  {
    return inSANDInnerVol(recoint) || inFrontECAL(recoint);
  }

} // mysel

// Define Var and Cut
const Var kRecoNuEnergy = SIMPLEVAR(Enu.calo);
const Var kRecoXVertex  = SIMPLEVAR(vtx.x);
const Var kRecoYVertex  = SIMPLEVAR(vtx.y);
const Var kRecoZVertex  = SIMPLEVAR(vtx.z);

const Cut kAll([](const caf::SRInteractionProxy*) { return true; });

const Cut kInSANDFV([](const caf::SRInteractionProxy* recoint)
                    { return mysel::inSANDFV(recoint); });

// numuCC in SAND FV reco: exactly 1 primary muon, in the SAND FV
const Cut kRecoNuMuCC_FV([](const caf::SRInteractionProxy* recoint)
                    { return mysel::inSANDFV(recoint) && mysel::NPrimaryMuons(recoint) >= 1; });

// NC-like in SAND FV reco: no primary muons, in the SAND FV
const Cut kRecoNC_FV([](const caf::SRInteractionProxy* recoint)
                    { return mysel::inSANDFV(recoint) && mysel::HasSandParticles(recoint) && mysel::NPrimaryMuons(recoint) == 0 && mysel::NPrimaryElectrons(recoint) == 0; });

// true categories
const Cut kTrueNuMuCC([](const caf::SRInteractionProxy* recoint)
                    { return mysel::GetTrueCategory(recoint) == mysel::kNuMuCCTrue; });
const Cut kTrueNuMuNC([](const caf::SRInteractionProxy* recoint)
                    { return mysel::GetTrueCategory(recoint) == mysel::kNuMuNCTrue; });
const Cut kTrueOther([](const caf::SRInteractionProxy* recoint)
                    { return mysel::GetTrueCategory(recoint) == mysel::kOtherTrue; });

// build 3 spectra: cc, nc, other
struct CatSpectra {
  Spectrum cc, nc, other;
  CatSpectra(SpectrumLoader& loader, const Cut& base, const HistAxis& ax)
    : cc   (loader.Interactions(RecoType::kSandreco)[base && kTrueNuMuCC], ax),
      nc   (loader.Interactions(RecoType::kSandreco)[base && kTrueNuMuNC], ax),
      other(loader.Interactions(RecoType::kSandreco)[base && kTrueOther],  ax) {}
};

double Integral(const Spectrum& s, double pot)
{
  std::unique_ptr<TH1D> h(s.ToTH1(pot));
  return h->Integral(0, h->GetNbinsX() + 1);
}

// draw stack histo
void DrawStack(const CatSpectra& s, double pot, const std::string& tag,
               const std::string& title, const std::string& outpng, bool logy)
{
  TH1D* hcc    = s.cc.ToTH1(pot);
  TH1D* hnc    = s.nc.ToTH1(pot);
  TH1D* hother = s.other.ToTH1(pot);

  hcc->SetFillColor(kAzure + 1);   hcc->SetLineColor(kAzure + 1);
  hnc->SetFillColor(kRed - 4);     hnc->SetLineColor(kRed - 4);
  hother->SetFillColor(kGray + 1); hother->SetLineColor(kGray + 1);

  auto c = new TCanvas(("c_" + tag).c_str(), tag.c_str(), 900, 650);
  if (logy) c->SetLogy();
  auto st = new THStack(("st_" + tag).c_str(),
                        (title + ";Reco Nu Energy (GeV);Events").c_str());
  st->Add(hother);   // bottom
  st->Add(hnc);
  st->Add(hcc);      // top
  st->Draw("hist");

  auto leg = new TLegend(0.55, 0.62, 0.88, 0.88);
  leg->AddEntry((TObject*)0, Form("POT = %.3g", pot), "");
  leg->AddEntry(hcc,    Form("#nu_{#mu} CC (%.0f ev)", hcc->Integral(0, hcc->GetNbinsX() + 1)), "f");
  leg->AddEntry(hnc,    Form("#nu_{#mu} NC (%.0f ev)", hnc->Integral(0, hnc->GetNbinsX() + 1)), "f");
  leg->AddEntry(hother, Form("Other (%.0f ev)", hother->Integral(0, hother->GetNbinsX() + 1)), "f");
  leg->SetTextFont(42);
  leg->SetBorderSize(0);
  leg->Draw();
  c->SaveAs(outpng.c_str());
}

// draw vertices
void DrawVtxByCategory(const CatSpectra& s, double pot, const std::string& tag,
                       const std::string& outpng)
{
  TH2* h[3] = { s.cc.ToTH2(pot), s.nc.ToTH2(pot), s.other.ToTH2(pot) };
  const char* names[3] = { "#nu_{#mu} CC", "#nu_{#mu} NC", "Other" };

  auto c = new TCanvas(("c_" + tag).c_str(), tag.c_str(), 1800, 600);
  c->Divide(3, 1);
  for (int k = 0; k < 3; ++k) {
    c->cd(k + 1);
    gPad->SetRightMargin(0.15);
    h[k]->SetTitle(Form("%s (%.0f ev)", names[k], h[k]->Integral(0, h[k]->GetNbinsX() + 1, 0, h[k]->GetNbinsY() + 1)));
    h[k]->Draw("COLZ");
  }
  c->SaveAs(outpng.c_str());
}

// print useful info
void PrintSelection(const std::string& name, const CatSpectra& sel,
                    const Spectrum& targetAllInFV, double pot, bool targetIsCC)
{
  const double n_cc = Integral(sel.cc, pot);
  const double n_nc = Integral(sel.nc, pot);
  const double n_ot = Integral(sel.other, pot);
  const double n_tot = n_cc + n_nc + n_ot;
  const double n_target = targetIsCC ? n_cc : n_nc;
  const double n_target_fv = Integral(targetAllInFV, pot);

  std::cout << std::fixed << std::setprecision(1)
            << "\n=== " << name << " ===\n"
            << "  numuCC: " << n_cc << "  numuNC: " << n_nc << "  other: " << n_ot << "\n"
            << std::setprecision(3)
            << "  purity    = " << (n_tot > 0 ? n_target / n_tot : 0.) << "\n"
            << "  efficiency = " << (n_target_fv > 0 ? n_target / n_target_fv : 0.)
            << "  (rispetto agli eventi veri dello stesso tipo con vertice reco nel SAND FV)\n";
}


// 6 exclusive categories stack - only reco quantities
//   numuCC-like : >= one primary muon
//   nueCC-like  : no primary muon, >= 1 primary electron reco
//   NC-like     : no muon and no electron
//   FV = reco vertex in the SAND FV
// check that the total sum matches the total nr of events
enum StackCategories { kSC_NuMuCC_FV = 0, kSC_NuMuCC_OOFV, kSC_NuMuBarCC, kSC_NueCC, kSC_NC, kSC_N };

StackCategories GetStackCat(const caf::SRInteractionProxy* recoint)
{
  const bool hasMu = mysel::NPrimaryMuons(recoint) >= 1;
  const bool hasAntiMu = mysel::NPrimaryAntiMuons(recoint) >= 1;
  const bool hasElectron  = !hasMu && mysel::NPrimaryElectrons(recoint) >= 1;
  const bool fv    = mysel::inSANDFV(recoint);

  // if a prim mu is present, and it's in SANDFV -> numuCC FV
  if (hasMu) return fv ? kSC_NuMuCC_FV : kSC_NuMuCC_OOFV;
  // if a prim mu is present --> numubarCC, doesn't matter if it's in the FV
  if (hasAntiMu) return kSC_NuMuBarCC;
  // if a prim e-/e+ is present, and it's in SANDFV -> nueCC FV
  if (hasElectron)  return kSC_NueCC;
  // otherwise, if no mu nor e, NC event
  return kSC_NC;
}

Cut MakeStackCut(StackCategories k)
{
  return Cut([k](const caf::SRInteractionProxy* recoint) { return GetStackCat(recoint) == k; });
}

struct StackSpectra {
  std::vector<std::unique_ptr<Spectrum>> s;
  StackSpectra(SpectrumLoader& loader, const HistAxis& ax)
  {
    for (int k = 0; k < kSC_N; ++k)
      s.emplace_back(new Spectrum(
        loader.Interactions(RecoType::kSandreco)[MakeStackCut(static_cast<StackCategories>(k))], ax));
  }
};

void DrawStack6(const StackSpectra& ss, const Spectrum& total, double pot,
                const std::string& tag, const std::string& outpng, bool logy)
{
  const char* labels[kSC_N] = {
    "#nu_{#mu} CC-like (reco #mu-), FV",
    "#nu_{#mu} CC-like (reco #mu-), OOFV",
    "#bar{#nu}_{#mu} CC-like (reco #mu+), SANDvol",
    "#nu_{e} CC-like (reco e+/e-), SANDvol",
    "NC-like (no #mu, no e), SANDvol",
  };
  const int colors[kSC_N] = { kAzure + 2, kGreen + 2, kRed + 1, kAzure - 9, kRed - 9};

  TH1D* h[kSC_N];
  double sum = 0;
  for (int k = 0; k < kSC_N; ++k) {
    h[k] = ss.s[k]->ToTH1(pot);
    h[k]->SetFillColor(colors[k]);
    h[k]->SetLineColor(colors[k]);
    sum += h[k]->Integral(0, h[k]->GetNbinsX() + 1);
  }

  TH1D* hT = total.ToTH1(pot);
  hT->SetLineColor(kBlack);
  hT->SetLineWidth(2);
  hT->SetFillStyle(0);
  const double nT = hT->Integral(0, hT->GetNbinsX() + 1);
  std::cout << "Check " << tag << ": somma categorie = " << sum << ", totale = " << nT << std::endl;

  auto c = new TCanvas(("c_" + tag).c_str(), tag.c_str(), 900, 650);
  if (logy) c->SetLogy();

  auto st = new THStack(("st_" + tag).c_str(), ";Reco Nu Energy (GeV);Events");
  for (int k = 0; k < kSC_N; ++k) st->Add(h[k]);   // k=0 at the bottom
  st->SetMaximum(logy ? 20 * hT->GetMaximum() : 1.3 * hT->GetMaximum());
  if (logy) st->SetMinimum(0.5);
  st->Draw("hist");
  hT->Draw("hist same");

  auto leg = new TLegend(0.45, 0.60, 0.89, 0.89);
  leg->SetTextFont(42);
  leg->SetTextSize(0.025);
  leg->SetBorderSize(0);
  leg->AddEntry((TObject*)0, Form("POT = %.3g", pot), "");
  leg->AddEntry(hT, Form("Total (%.0f ev)", nT), "l");
  for (int k = kSC_N - 1; k >= 0; --k)             // same order as stack
    leg->AddEntry(h[k], Form("%s (%.0f ev)", labels[k], h[k]->Integral(0, h[k]->GetNbinsX() + 1)), "f");
  leg->Draw();
  c->SaveAs(outpng.c_str());
}

// ---------------------------------------------------------------------------
void sand_selection()
{
  const std::string outdir = "workspace/bm_results/stack/";
  gSystem->mkdir(outdir.c_str(), true);

  const std::string fname_n =
    "/pnfs/dune/persistent/users/gsantoni/beam_monitoring/data/fast_reco/nominal/CAF.flat/*.root";
  SpectrumLoader loader_n(fname_n);

  // axes
  const HistAxis axE("Reco Nu Energy (GeV)", Binning::Simple(100, 0, 10), kRecoNuEnergy);
  const HistAxis axVtxYZ("z (cm)", Binning::Simple(100, 2000, 2900), kRecoZVertex,
                         "y (cm)", Binning::Simple(100, -600, 100),  kRecoYVertex);

  // ---- all ints + all ints in the SAND FV ----
  CatSpectra stackAll(loader_n, kAll,       axE);
  CatSpectra stackFV (loader_n, kInSANDFV,  axE);

  // ---- all ints from SANDRECO + all ints in SANDFV + numuCC ints in SANDFV ----
  Spectrum sTotal    (loader_n.Interactions(RecoType::kSandreco),                  axE);
  Spectrum sTotalFV  (loader_n.Interactions(RecoType::kSandreco)[kInSANDFV],       axE);
  Spectrum sNuMuCCFV (loader_n.Interactions(RecoType::kSandreco)[kRecoNuMuCC_FV],  axE);

  // ---- purity / efficiency of the two reco selections ----
  CatSpectra selCC(loader_n, kRecoNuMuCC_FV, axE);
  CatSpectra selNC(loader_n, kRecoNC_FV,     axE);

  // ---- all vtx from SANDRECO + numuCC vtx in SANDFV ----
  Spectrum sVtxTotal   (loader_n.Interactions(RecoType::kSandreco),                 axVtxYZ);
  Spectrum sVtxNuMuCCFV(loader_n.Interactions(RecoType::kSandreco)[kRecoNuMuCC_FV], axVtxYZ);
  CatSpectra vtxByCat(loader_n, kAll, axVtxYZ);

  // ---- 6 categories stack histo ----
  StackSpectra ss6(loader_n, axE);

  loader_n.Go();

  const double pot = sTotal.POT();
  std::cout << "nominal POT: " << pot << std::endl;

  // ================= (1) STACK =================
  DrawStack(stackAll, pot, "stack_all", "Reco #nu energy, all interactions",
            outdir + "reco_nu_energy_stack_all.png", false);
  DrawStack(stackFV, pot, "stack_fv", "Reco #nu energy, vertex in SAND FV",
            outdir + "reco_nu_energy_stack_SANDFV.png", false);

  // ================= (2) en spectrum ALL SANDRECO + ALL FV + numuCC ALL FV =================
  {
    TH1D* hT  = sTotal.ToTH1(pot);
    TH1D* hTF = sTotalFV.ToTH1(pot);
    TH1D* hC  = sNuMuCCFV.ToTH1(pot);
    hT->SetLineColor(kBlack);      hT->SetLineWidth(2);
    hTF->SetLineColor(kGray + 2);  hTF->SetLineWidth(2); hTF->SetLineStyle(2);
    hC->SetLineColor(kOrange + 1); hC->SetLineWidth(2);

    auto c = new TCanvas("c_cmp", "c_cmp", 900, 650);
    // c->SetLogy();
    hT->SetTitle(";Reco Nu Energy (GeV);Events");
    hT->SetMaximum(1.2 * hT->GetMaximum());
    hT->Draw("hist");
    hTF->Draw("hist same");
    hC->Draw("hist same");

    auto leg = new TLegend(0.50, 0.65, 0.88, 0.88);
    leg->AddEntry((TObject*)0, Form("POT = %.3g", pot), "");
    leg->AddEntry(hT,  Form("Total (%.0f ev)", hT->Integral(0, hT->GetNbinsX() + 1)), "l");
    leg->AddEntry(hTF, Form("Total, SAND FV (%.0f ev)", hTF->Integral(0, hTF->GetNbinsX() + 1)), "l");
    leg->AddEntry(hC,  Form("#nu_{#mu} CC selected, SAND FV (%.0f ev)", hC->Integral(0, hC->GetNbinsX() + 1)), "l");
    leg->SetTextFont(42);
    leg->SetBorderSize(0);
    leg->Draw();
    c->SaveAs((outdir + "reco_nu_energy_total_vs_numuCC_SANDFV.png").c_str());
  }

  // ================= (3) ALL VERTICES SANDRECO + NuMuCC SAND FV =================
  {
    TH2* hVT = sVtxTotal.ToTH2(pot);
    TH2* hVC = sVtxNuMuCCFV.ToTH2(pot);
    hVT->SetTitle("All reco interactions");
    hVC->SetTitle("#nu_{#mu} CC selected, SAND FV");

    auto c_total = new TCanvas("c_vtx", "c_vtx");
    hVT->Draw("COLZ");
    c_total->SaveAs((outdir + "reco_vtx_YZ_total.png").c_str());
    auto c_sel = new TCanvas("c_vtx_sel", "c_vtx_sel");
    hVC->Draw("COLZ");
    c_sel->SaveAs((outdir + "reco_vtx_YZ_numuCC_SANDFV.png").c_str());

    std::cout << "Total: entries=" << hVT->GetEntries()
          << " integral(all)=" << hVT->Integral(0, hVT->GetNbinsX()+1, 0, hVT->GetNbinsY()+1) << "\n"
          << "Sel:   entries=" << hVC->GetEntries()
          << " integral(all)=" << hVC->Integral(0, hVC->GetNbinsX()+1, 0, hVC->GetNbinsY()+1) << std::endl;
  }

  DrawVtxByCategory(vtxByCat, pot, "vtx_cat", outdir + "reco_vtx_YZ_by_true_category.png");

  // ================= purity / efficiency =================
  PrintSelection("Selezione numuCC reco (FV + 1 muone primario)", selCC, stackFV.cc, pot, true);
  PrintSelection("Selezione NC-like reco (FV + 0 muoni primari)",  selNC, stackFV.nc, pot, false);

  // ================= 6 CATEGORIES STACK =================
  DrawStack6(ss6, sTotal, pot, "stack6_lin", outdir + "reco_nu_energy_stack6.png", false);
  DrawStack6(ss6, sTotal, pot, "stack6_log", outdir + "reco_nu_energy_stack6_log.png", true);
}