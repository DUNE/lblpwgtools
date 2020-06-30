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
  const std::string fname = "/dune/data/users/marshalc/CAFs/mcc11_v3/FD_FHC_nonswap.root";

  // Source of events
  SpectrumLoader loader(fname);

  // A Var is a little snippet of code that takes a record representing the
  // event record and returns a single number to plot.
  const Var kRecoEnergy([](const caf::StandardRecord* sr)
                        {
                          return sr->Ev_reco_numu;
                        });

  // For such a simple variable you can use a shortcut like this
  const Var kCVNNumu = SIMPLEVAR(cvnnumu);

  // Define a spectrum, ie a histogram with associated POT information
  const Binning binsEnergy = Binning::Simple(40, 0, 10);
  const HistAxis axEnergy("Reco energy (GeV)", binsEnergy, kRecoEnergy);
  // kIsNumuCC here is a "Cut". Same as a Var but returning a boolean. In this
  // case, we're only keeping events that are truly numu CC interactions.
  Spectrum sEnergy(loader, axEnergy, kIsNumuCC);

  Spectrum sEnergyNC(loader, axEnergy, kIsNC);

  // And another
  const Binning binsCVN = Binning::Simple(50, 0, 1);
  const HistAxis axCVN("CVN_{#mu}", binsCVN, kCVNNumu);
  Spectrum sCVN(loader, axCVN, kIsNumuCC);
  Spectrum sCVNNC(loader, axCVN, kIsNC);

  // This is the call that actually fills in those spectra
  loader.Go();

  // POT/yr * 3.5yrs * mass correction for the workspace geometry
  const double pot = 3.5 * 1.47e21 * 40/1.13;

  // For plotting purposes we can convert to TH1s
  sEnergy.ToTH1(pot)->Draw("hist");
  sEnergyNC.ToTH1(pot, kBlue)->Draw("hist same");
  new TCanvas;
  sCVN.ToTH1(pot)->Draw("hist");
  sCVNNC.ToTH1(pot, kBlue)->Draw("hist same");
  gPad->SetLogy();
}
