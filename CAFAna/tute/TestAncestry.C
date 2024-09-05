#include <iostream>

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "duneanaobj/StandardRecord/Navigate.h"
#include "duneanaobj/StandardRecord/SREnums.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"
#include "truth_matched.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h"

using namespace ana;

void TestAncestry()
{

  const std::string fname = "/pnfs/dune/persistent/users/abooth/nd-production/MicroProdN1p2/output/run-cafmaker/MicroProdN1p2_NDLAr_1E18_RHC.caf.nu/CAF.flat/0000000/0000000/MicroProdN1p2_NDLAr_1E18_RHC.caf.nu.00000*";
  //"/exp/dune/data/users/noeroy/mywork/MiniRun5_Fixed_truth/MiniRun5_1E19_RHC.caf/CAF.flat/0000000/*.flat.root";

  // Source of events
  SpectrumLoader loader(fname);

  // Define axes for the spectra we'll make
  const RecoPartHistAxis axEnergy("Best-matched true particle energy (GeV)", Binning::Simple(50, 0, 5), kMatchedTruePartE);

  Spectrum sEnergyMatched(loader.Interactions(RecoType::kDLP).RecoParticles(RecoType::kDLP)[kIsMuon], axEnergy);
  Spectrum sEnergyMatchedTrueMu(loader.Interactions(RecoType::kDLP).RecoParticles(RecoType::kDLP)[kIsMuon && kMatchedToTrueMuon], axEnergy);

  const HistAxis axNuEnergy("True matched neutrino energy (GeV",  Binning::Simple(50, 0, 20), kMatchedTrueInteractionEnergy);
  Spectrum sNuEnergyMatched(loader.Interactions(RecoType::kDLP), axNuEnergy);
  Spectrum sNuEnergyMatchedTrue(loader.Interactions(RecoType::kDLP)[kMatchedToTrueRES], axNuEnergy);

  //  ~~~~**** -.-.-.-.-.-.-.-.-.-.-.- *****~~~~
  // Fill in the spectrum
  loader.Go();

  const double pot =   sEnergyMatched.POT();
  //1e17;

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


  TCanvas c2;
  TLegend leg2(0.6, 0.6, 0.9, 0.9);
  leg2.SetFillStyle(0);
  leg2.SetBorderSize(0);

  auto h3 = sNuEnergyMatched.ToTH1(pot);
  h3->Draw("hist");
  leg2.AddEntry(h3, "All neutrino ixn");

  auto h4 = sNuEnergyMatchedTrue.ToTH1(pot, kRed);
  h4->Draw("hist same");
  leg2.AddEntry(h2, "+ matched to RES");
  leg2.Draw();

  c2.SaveAs("TrueE_matchedRES.png");
}
