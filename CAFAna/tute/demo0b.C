// Make a simple spectrum plot
// cafe demo0.C

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"

//#include "CAFAna/Cuts/TruthCuts.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TPad.h"

using namespace ana;

// A more generic selection on the pdg of the particle 

 const RecoPartCut kPartCut(int pdg){
    const RecoPartCut kIsPDG([pdg](const caf::SRRecoParticleProxy* sr)
                      {
                        return abs(sr->pdg) == pdg ;
                      });
    return kIsPDG;
  }

void demo0b()
{
  // Environment variables and wildcards work. As do SAM datasets.
  const std::string fname = "/dune/data/users/skumara/Datafiles_2x2/CAF_rootfiles/minirun4/notruth/outputCAF_notruth_*";

  // Source of events
  SpectrumLoader loader(fname);

  // We will also use two different Vars. For such simple ones we can use this
  // Variables at the level of Particles
  const RecoPartVar kRecoParticleEnergy = SIMPLEPARTVAR(E);
  const RecoPartVar kRecoParticlePDG = SIMPLEPARTVAR(pdg);

  // Define axes for the spectra we'll make
  const RecoPartHistAxis axEnergy("Muon energy (GeV)", Binning::Simple(50, 0, 1), kRecoParticleEnergy);
  const RecoPartHistAxis axPDG("PDG of reco particle", Binning::Simple(14, 0, 14), kRecoParticlePDG);


// Select particles that have a pdg of muon 
  const RecoPartCut kIsMuon([](const caf::SRRecoParticleProxy* sr)
                      {
                        return abs(sr->pdg) == 13 ;
                      });

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
  //2D histaxis with interaction level variables
  const HistAxis vtxPosition( "x(cm)", Binning::Simple(60,-60,60), SIMPLEVAR(vtx.x), 
                              "z(cm)", Binning::Simple(60,-60,60), SIMPLEVAR(vtx.z-1300));

//  ~~~~**** where the magic happens *****~~~~
  // we use kNoCut at interactions level, meaning no selection on vertices
  Spectrum sEnergyMuon(loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(RecoType::kDLP)[kIsMuon], axEnergy);
  Spectrum sEnergyMuonContX(loader.Interactions(RecoType::kDLP)[kContainment].RecoParticles(RecoType::kDLP)[kIsMuon], axEnergy);
  Spectrum sEnergyElectron(loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(RecoType::kDLP)[kPartCut(11)], axEnergy);
 
  Spectrum sVtxPositionAll(loader.Interactions(RecoType::kDLP)[kNoCut], vtxPosition);
  Spectrum sVtxPositionCont(loader.Interactions(RecoType::kDLP)[kContainment], vtxPosition);

  // This will be empty because the files are still empty
  //Spectrum sEnergySigPandora(loader.Interactions(RecoType::kPandora)[kNoCut].RecoParticles(RecoType::kPandora)[kIsMuon], axEnergy);
 
  // This will break the macro because there is no pida reconstruction for interactions. Still need a more explicit error message to pop out here
  //Spectrum sEnergySigPIDA(loader.Interactions(RecoType::kPIDA)[kNoCut].RecoParticles(RecoType::kDLP)[kIsMuon], axEnergy);

  Spectrum sPDGMuon(  loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(RecoType::kDLP)[kIsMuon], axPDG);
  Spectrum sPDGAll(   loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(RecoType::kDLP)[kNoPartCut], axPDG);
//  ~~~~**** -.-.-.-.-.-.-.-.-.-.-.- *****~~~~
  // Fill in the spectrum
  loader.Go();

  // We are forcing the pot value because cafs dont have this information yet
  // POT/yr * 3.5yrs * mass correction for the workspace geometry
  const double pot = 3.5 * 1.47e21 * 40/1.13;

  sEnergyMuon.OverridePOT(pot);
  sEnergyMuonContX.OverridePOT(pot);
  sEnergyElectron.OverridePOT(pot);

  sVtxPositionAll.OverridePOT(pot);
  sVtxPositionCont.OverridePOT(pot);
  //sEnergySigPandora.OverridePOT(pot);
  sPDGMuon.OverridePOT(pot);
  sPDGAll.OverridePOT(pot);

  //ta da!
  sEnergyMuon.ToTH1(pot)->Draw("hist");
  sEnergyMuonContX.ToTH1(pot,kMagenta+2)->Draw("hist same");
  sEnergyElectron.ToTH1(pot,kGreen+1)->Draw("hist same");
  //sEnergySigPandora.ToTH1(pot, kRed)->Draw("hist same");
  new TCanvas;
  sVtxPositionAll.ToTH2(pot)->Draw("colz");
  new TCanvas; 
  sVtxPositionCont.ToTH2(pot)->Draw("colz");
  new TCanvas;
  sPDGAll.ToTH1(pot,kBlue)->Draw("hist");
  sPDGMuon.ToTH1(pot,kRed)->Draw("hist same");


}
