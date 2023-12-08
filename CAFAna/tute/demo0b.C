// Make a simple spectrum plot
// cafe demo0.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TPad.h"

using namespace ana;

void demo0b()
{
  // Environment variables and wildcards work. As do SAM datasets.
  const std::string fname = "/dune/data/users/skumara/Datafiles_2x2/CAF_rootfiles/minirun4/notruth/outputCAF_notruth_*";

  // Source of events
  SpectrumLoader loader(fname);

  // We will also use two different Vars. For such simple ones we can use this
  // shortcut macro.
  const Var kParticleEnergy = SIMPLEVAR(truth.E);


  // Define axes for the spectra we'll make
  const HistAxis axEnergy("Muon energy (GeV)", Binning::Simple(50, 0, 5), kParticleEnergy);

  // A Cut is the same as a Var but returns a boolean. Here we're selecting
  // truly numu CC interactions.
  const Cut kIsNumuCC([](const caf::SRInteractionProxy* sr)
                      {
                        //todo
                        return (sr->truth.iscc &&
                                abs(sr->truth.pdg) == 14);
                      });
// Select particles that have a pdg of muon
  const Cut kIsMuon([](const caf::SRRecoParticleProxy* sr)
                      {
                        //todo
                        return abs(sr->pdg) == 13 ;
                      });

// where the magic happens //   .dlp or .pandora ...
  Spectrum sEnergySig(loader.Interactions()[kIsNumuCC].dlp.Particles()[kIsMuon], axEnergy);
// or InteractionsDLP/Pandora
  // Fill in the spectrum
  loader.Go();

  //ta da!
  sEnergySig.ToTH1(kPOTnominal)->Draw("hist");

}
