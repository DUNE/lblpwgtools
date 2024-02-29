
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

// Make a basic ensemble spectrum?
void demo0eb()
{
 
  const std::string fname = "/dune/data/users/skumara/Datafiles_2x2/CAF_rootfiles/minirun4/notruth/outputCAF_notruth_*";

  // Source of events
  SpectrumLoader loader(fname);

  // Reco-Particles (SRRecoParticleProxy)  level vars and cuts
  const RecoPartVar kRecoParticleEnergy = SIMPLEPARTVAR(E);
  // Define axes for the spectra we'll make
  const RecoPartHistAxis axEnergy("Muon energy (GeV)", Binning::Simple(50, 0, 1), kRecoParticleEnergy);
  // Select particles that have a pdg of muon, 
  const RecoPartCut kIsMuon([](const caf::SRRecoParticleProxy* sr)
                      {
                        return abs(sr->pdg) == 13 ;
                      });

  // Interaction level (SRInteractionProxy)  vars 
  const Var kVtxX = SIMPLEVAR(vtx.x);
  //  we can also write vars with additional logic/operations
  const Var kVtxZOffset([](const caf::SRInteractionProxy* sr)
                      {                        
                        double z = sr-> vtx.z-1300;
                        // you could do more operations here
                        return z;
                      });
  // note this particular var is actually simple so it can also be written as SIMPLEVAR(vtx.z-1300)

  // Histaxis with interaction level variables
  // Note you can setup a 2D axis 
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

  ////////////// actually setup the ensemble spectra


  // Fill in the spectra
  loader.Go();

  // POT/yr * 3.5yrs * mass correction for the workspace geometry
  const double pot = 3.5 * 1.47e21 * 40/1.13;

  // We are forcing the pot value because cafs dont have this information yet
  sEnergyMuon.OverridePOT(pot);

  new TCanvas; 
  sVtxPositionCont.ToTH2(pot)->Draw("colz");

}
