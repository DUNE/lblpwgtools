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

// reco particles whose best-match particle is a true muon
const RecoPartCut kMatchedToTrueMuon([](const caf::SRRecoParticleProxy * part) -> float
{
  if (part->truth.empty())
    return false;

  const caf::SRProxy * top = part->Ancestor<caf::SRProxy>();
  return std::abs(caf::FindParticle(top->mc, part->truth[0])->pdg) == 13;
});

// Select reco particles that have a pdg of muon
const RecoPartCut kIsMuon([](const caf::SRRecoParticleProxy* sr)
                          {
                            return abs(sr->pdg) == 13 ;
                          });

void TestAncestry()
{

  const std::string fname = "/exp/dune/data/users/noeroy/mywork/MiniRun5_Fixed_truth/MiniRun5_1E19_RHC.caf/CAF.flat/0000000/*.flat.root";

  // Source of events
  SpectrumLoader loader(fname);

  // Define axes for the spectra we'll make
  const RecoPartHistAxis axEnergy("Best-matched true particle energy (GeV)", Binning::Simple(50, 0, 5), kMatchedTruePartE);

  Spectrum sEnergyMatched(loader.Interactions(RecoType::kDLP).RecoParticles(RecoType::kDLP)[kIsMuon], axEnergy);
  Spectrum sEnergyMatchedTrueMu(loader.Interactions(RecoType::kDLP).RecoParticles(RecoType::kDLP)[kIsMuon && kMatchedToTrueMuon], axEnergy);

  //  ~~~~**** -.-.-.-.-.-.-.-.-.-.-.- *****~~~~
  // Fill in the spectrum
  loader.Go();

  // We are forcing the pot value because cafs dont have this information yet
  const double pot = 1e17;

  sEnergyMatched.OverridePOT(pot);
  sEnergyMatchedTrueMu.OverridePOT(pot);

  TCanvas c;
  TLegend leg(0.6, 0.6, 0.9, 0.9);
  leg.SetFillStyle(0);
  leg.SetBorderSize(0);

  auto h1 = sEnergyMatched.ToTH1(pot);
  h1->Draw("hist");
  leg.AddEntry(h1, "All reco muons");

  auto h2 = sEnergyMatchedTrueMu.ToTH1(pot, kRed);
  h2->Draw("hist same");
  leg.AddEntry(h2, "+ matched to true muons");

  leg.Draw();

  c.SaveAs("recoMu_matchedTrueE.png");
}
