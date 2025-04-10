#include <iostream>

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
#include "TLegend.h"

using namespace ana;


const RecoPartVar kMatchedTruePartE([](const caf::SRRecoParticleProxy * part) -> float
{
  if (part->truth.empty())
    return -999.;

  // Ancestor() returns the closest ancestor in the SR hierarchy with the template type.
  // Once we have the top-level object, we then can connect its
  const caf::SRProxy * top = part->Ancestor<caf::SRProxy>();
  return caf::FindParticle(top->mc, part->truth[0])->p.E;
});

const RecoPartVar kMatchedTruePartPDG([](const caf::SRRecoParticleProxy * part) -> int
{
  if (part->truth.empty())
    return -999;

  // Ancestor() returns the closest ancestor in the SR hierarchy with the template type.
  // Once we have the top-level object, we then can connect its
  const caf::SRProxy * top = part->Ancestor<caf::SRProxy>();
  int pdg = caf::FindParticle(top->mc, part->truth[0])->pdg;
  // recast in a different way for easier visualization
  //(1) muon
  if ( abs(pdg)==13) return 1.;
  //(2) electron
  if ( abs(pdg)==11) return 2.;
  //(3) proton
  if ( pdg==2212) return 3.;
  // (4) neutron
  if ( pdg==2112) return 4.;
  // (5) pi +
  if ( pdg==211) return 5.;
  // (6) pi -
  if ( pdg==-211) return 6.;
  // (7) pi0
  if ( pdg==111) return 7.;
  // (8) photon
  if (pdg==22) return 8.;
  // (9) Any K0S, K0L K0 or K+/-
  if (abs(pdg)==321 || pdg== 311|| pdg== 310|| pdg== 130) return 9.;
  else return 10.;
});
std::vector<std::string> PDGlabels = {"#mu", "e","p","n","#pi^{+}","#pi^{-}","#pi^{0}","#gamma","K","other"};

// reco particles whose best-match particle is a true pion
const RecoPartCut kMatchedToTruePion([](const caf::SRRecoParticleProxy * part) -> float
{
  if (part->truth.empty())
    return false;

  const caf::SRProxy * top = part->Ancestor<caf::SRProxy>();
  return std::abs(caf::FindParticle(top->mc, part->truth[0])->pdg) == 211;
});

// Select reco particles that have a pdg of muon
const RecoPartCut kIsPion([](const caf::SRRecoParticleProxy* sr)
                          {
                            return abs(sr->pdg) == 211 ;
                          });


// some truth variables
const TruthPartVar kPrimEnergy([](const  caf::SRTrueParticleProxy * part) -> float
{
  float ret = -999.;
  ret = part->p.E;
  return ret;

});

const TruthCut kIsTrueNuMu ([](const caf::SRTrueInteractionProxy* nu){
                        int pdg = abs(nu->pdg);
                         return pdg==14;
  });

const TruthPartCut kIsPionPrim([](const  caf::SRTrueParticleProxy * part){
  int pdg = abs(part->pdg);
  return pdg==211;
});

 
void demoPions()
{

  const std::string fname = "/exp/dune/data/users/noeroy/mywork/MiniRun5_Fixed_truth/MiniRun5_1E19_RHC.caf/CAF.flat/0000000/*.flat.root";

  // Source of events
  SpectrumLoader loader(fname);

  // Define axes for the spectra we'll make
  const RecoPartHistAxis axEnergy("Best-matched true particle energy (GeV)", Binning::Simple(50, 0, 5), kMatchedTruePartE);
  const TruthPartHistAxis axTrueEnergy("true pion primary energy (GeV)", Binning::Simple(50,0,5), kPrimEnergy);
  const RecoPartHistAxis myHistTruthPartAxisPDG("true particles pdg", Binning::Simple(10, 1, 11), kMatchedTruePartPDG);

  // Reconstructed energy of best matched pions
  Spectrum sEnergyMatched(loader.Interactions(RecoType::kDLP).RecoParticles(RecoType::kDLP)[kIsPion], axEnergy);
  Spectrum sEnergyMatchedTrueMu(loader.Interactions(RecoType::kDLP).RecoParticles(RecoType::kDLP)[kIsPion && kMatchedToTruePion], axEnergy);
  // We want to see what else is there (pdg of the rest identified pions)
  Spectrum sNonMatchedPionPDG(loader.Interactions(RecoType::kDLP).RecoParticles(RecoType::kDLP)[kIsPion && !kMatchedToTruePion], myHistTruthPartAxisPDG);

  // True energy of pions...
  Spectrum sEnergyTruthPions(loader.NuTruths()[kIsTrueNuMu].TruthParticles(TruePType::kPrim)[kIsPionPrim], axTrueEnergy);

  //  ~~~~**** -.-.-.-.-.-.-.-.-.-.-.- *****~~~~
  // Fill in the spectrum
  loader.Go();

  // We are forcing the pot value because cafs dont have this information yet
  const double pot = 1e17;

  //sEnergyMatched.OverridePOT(pot);
  //sEnergyMatchedTrueMu.OverridePOT(pot);

  TCanvas c;
  TLegend leg(0.6, 0.6, 0.9, 0.9);
  leg.SetFillStyle(0);
  leg.SetBorderSize(0);

  auto h1 = sEnergyMatched.ToTH1(pot);
  h1->Draw("hist");
  leg.AddEntry(h1, "All reco pions");

  auto h2 = sEnergyMatchedTrueMu.ToTH1(pot, kRed);
  auto h3 = sEnergyTruthPions.ToTH1(pot, kBlue);
  h2->Draw("hist same");
 // h3->Draw("hist same");
  leg.AddEntry(h2, "+ matched to true pions");
  //leg.AddEntry(h3, "all true pions energy");
  leg.Draw();
  c.SaveAs("recoPion_matchedTrueE.png");



  new TCanvas;
  h3->Draw("hist")  ;
  gPad->SaveAs("truePion_TrueE.png");


  auto h4 = sNonMatchedPionPDG.ToTH1(pot);
  for (unsigned int i  = 0; i<PDGlabels.size(); i++)  
    h4->GetXaxis()->SetBinLabel(i+1,PDGlabels[i].c_str());
  h4->GetXaxis()->SetTitle("non-matched reco pions true pdg");
  new TCanvas;
  h4->Draw("hist");
  gPad->SaveAs("nonTruePions_PDG.png");

}
