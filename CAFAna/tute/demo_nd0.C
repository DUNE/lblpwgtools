// Make a simple spectrum plot
// cafe demo_nd0.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "StandardRecord/SRProxy.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TPad.h"

using namespace ana;

void demo_nd0()
{
  // Environment variables and wildcards work. As do SAM datasets.
  const std::string fname = "/pnfs/dune/persistent/users/marshalc/CAF/CAFv5gas/CAF_FHC_*.root";

  // Source of events
  SpectrumLoader loader(fname);

  // A Var is a little snippet of code that takes a record representing the
  // event record and returns a single number to plot.
  const Var kRecoEnergy([](const caf::SRProxy* sr)
                        {
                          // TODO - why is Ev_reco NaN in a handful of the
                          // input events?
                          if(isnan(sr->Ev_reco)) return -1.;

                          return sr->Ev_reco;
                        });

  // For such a simple variable you can use a shortcut like this
  const Var kTrueEnergy = SIMPLEVAR(Ev);

  // Define a spectrum, ie a histogram with associated POT information
  const Binning binsEnergy = Binning::Simple(40, 0, 10);
  const HistAxis axReco("Reco energy (GeV)", binsEnergy, kRecoEnergy);
  // kIsNumuCC here is a "Cut". Same as a Var but returning a boolean. In this
  // case, we're only keeping events that are truly numu CC interactions.
  Spectrum sReco(loader, axReco, kIsNumuCC);

  Spectrum sRecoNC(loader, axReco, kIsNC);

  // And another
  const HistAxis axTrue("True Energy (GeV)", binsEnergy, kTrueEnergy);
  Spectrum sTrue(loader, axTrue, kIsNumuCC);
  Spectrum sTrueNC(loader, axTrue, kIsNC);

  // This is the call that actually fills in those spectra
  loader.Go();

  // I have no idea what a sensible/conventional POT normalization is
  const double pot = 1e20;

  // For plotting purposes we can convert to TH1s
  sReco.ToTH1(pot)->Draw("hist");
  sRecoNC.ToTH1(pot, kBlue)->Draw("hist same");
  new TCanvas;
  sTrue.ToTH1(pot)->Draw("hist");
  sTrueNC.ToTH1(pot, kBlue)->Draw("hist same");
}
