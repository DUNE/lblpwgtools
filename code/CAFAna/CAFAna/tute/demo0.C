// Make a simple spectrum plot
// cafe demo0.C

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "StandardRecord/StandardRecord.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TPad.h"

using namespace ana;

void demo0()
{
  // Environment variables and wildcards work. As do SAM datasets.
  const std::string fname = "/pnfs/dune/persistent/TaskForce_AnaTree/far/train/v3.2/nu.mcc10.1_def.root";

  // Source of events
  SpectrumLoader loader(fname);

  // Various ways of swapping the neutrino flavours are all mixed together in
  // the current files. Extract each specific swapping type by its particular
  // run number.
  auto* loaderBeam  = loader.LoaderForRunPOT(20000001);
  auto* loaderNue   = loader.LoaderForRunPOT(20000002);
  auto* loaderNuTau = loader.LoaderForRunPOT(20000003);
  // NC is special, you can safely sum the contribution from all swappings 
  auto* loaderNC    = loader.LoaderForRunPOT(0);

  // A Var is a little snippet of code that takes a record representing the
  // event record and returns a single number to plot.
  const Var kRecoEnergy({},
                        [](const caf::StandardRecord* sr)
                        {
                          return sr->dune.Ev_reco_numu;
                        });

  // For such a simple variable you can use a shortcut like this
  const Var kMVANumu = SIMPLEVAR(dune.mvanumu);

  // Define a spectrum, ie a histogram with associated POT information
  const Binning binsEnergy = Binning::Simple(40, 0, 10);
  const HistAxis axEnergy("Reco energy (GeV)", binsEnergy, kRecoEnergy);
  // kIsNumuCC here is a "Cut". Same as a Var but returning a boolean. In this
  // case, we're only keeping events that are truly numu CC interactions.
  Spectrum sEnergy(*loaderBeam, axEnergy, kIsNumuCC);

  Spectrum sEnergyNC(*loaderNC, axEnergy, kIsNC);

  // And another
  const Binning binsMVA = Binning::Simple(50, -1, 1);
  const HistAxis axMVA("MVA_{#mu}", binsMVA, kMVANumu);
  Spectrum sMVA(*loaderBeam, axMVA, kIsNumuCC);
  Spectrum sMVANC(*loaderNC, axMVA, kIsNC);

  // This is the call that actually fills in those spectra
  loader.Go();

  // POT/yr * 3.5yrs * mass correction for the workspace geometry
  const double pot = 3.5 * 1.47e21 * 40/1.13;

  // For plotting purposes we can convert to TH1s
  sEnergy.ToTH1(pot)->Draw("hist");
  sEnergyNC.ToTH1(pot, kBlue)->Draw("hist same");
  new TCanvas;
  sMVA.ToTH1(pot)->Draw("hist");
  sMVANC.ToTH1(pot, kBlue)->Draw("hist same");
  gPad->SetLogy();
}
