
#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TPad.h"

using namespace ana;

// Make a few basic spectra of reconstructed info from the cafs
// One with Interaction Level vars/cuts
// One with particle-level vars/cuts
void demo0b()
{
  // Environment variables and wildcards work. As do SAM datasets.
  const std::string fname = "/dune/data/users/skumara/Datafiles_2x2/CAF_rootfiles/Picorun4.1/PicoRun4.1_1E17_RHC.flow*";

  // Source of events
  SpectrumLoader loader(fname);

  /////////////////////////////////////////////////////////////////
  // Reco-Particles (SRRecoParticleProxy)  level vars and cuts
  const RecoPartVar kRecoParticleEnergy = SIMPLEPARTVAR(E);
  // Define axes for the spectra we'll make
  const RecoPartHistAxis axEnergy("Muon energy (GeV)", Binning::Simple(50, 0, 1), kRecoParticleEnergy);
  // Select particles that have a pdg of muon, 
  const RecoPartCut kIsMuon([](const caf::SRRecoParticleProxy* sr)
                      {
                        return abs(sr->pdg) == 13 ;
                      });

  /////////////////////////////////////////////////////////////////
  // Interaction level (SRInteractionProxy)  vars 
  const Var kVtxX = SIMPLEVAR(vtx.x);
  //  we can also write vars with additional logic/operations
  // note this particular var is actually simple so it can also be written as SIMPLEVAR(vtx.z-1300)
  const Var kVtxZOffset([](const caf::SRInteractionProxy* sr)
                      {                        
                        double z = sr-> vtx.z-1300;
                        // you could do more operations here
                        return z;
                      });

  //2D histaxis with interaction level variables
  const HistAxis vtxPosition( "x(cm)", Binning::Simple(70,-70,70), kVtxX, 
                              "z(cm)", Binning::Simple(70,-70,70), kVtxZOffset);

  // A simple selection cut at the level of vertices: i.e. containment
  const Cut kContainment([](const caf::SRInteractionProxy* sr)
                      {

                        double x = sr->vtx.x;
                        double y = sr->vtx.y;
                        double z = sr->vtx.z;
                        bool cont =  abs(x)<50 && 
                                     abs(x)>10 && 
                                     abs(y+310)<50 &&
                                     abs(z-1300)>10 && 
                                     abs(z-1300)<50 ;
                        return cont;
                      });

  ///////////////////////////////////////////////////////////////// 
  //  ~~~~**** now actually make some histograms  *****~~~~
  // we use kNoCut at interactions level, meaning no selection on vertices, then with containment
  // You can select RecoType::kDLP,kPandora at vtx/interaction level and kPIDA as well for particles
  Spectrum sEnergyMuon(     loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(      RecoType::kDLP)[kIsMuon], axEnergy);
  Spectrum sEnergyMuonCont( loader.Interactions(RecoType::kDLP)[kContainment].RecoParticles(RecoType::kDLP)[kIsMuon], axEnergy);
  // Interaction level vars
  Spectrum sVtxPositionAll( loader.Interactions(RecoType::kDLP)[kNoCut], vtxPosition);
  Spectrum sVtxPositionCont(loader.Interactions(RecoType::kDLP)[kContainment], vtxPosition);
  
  // Fill in the spectrum
  loader.Go();

  // POT/yr * 3.5yrs * mass correction for the workspace geometry
  const double pot = 3.5 * 1.47e21 * 40/1.13;

  // We are forcing the pot value because cafs dont have this information yet
  // normally you can obtain the stored pot
  // with spec.POT();
  sEnergyMuon.OverridePOT(pot);
  sEnergyMuonCont.OverridePOT(pot);
  sVtxPositionAll.OverridePOT(pot);
  sVtxPositionCont.OverridePOT(pot);

  //ta da! Draw tour histograms
  sEnergyMuon.ToTH1(pot)->Draw("hist");
  sEnergyMuonCont.ToTH1(pot,kMagenta+2)->Draw("hist same");

  new TCanvas;
  sVtxPositionAll.ToTH2(pot)->Draw("colz");
  new TCanvas; 
  sVtxPositionCont.ToTH2(pot)->Draw("colz");

}
