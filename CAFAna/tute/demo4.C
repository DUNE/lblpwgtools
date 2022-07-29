// Make a simple spectrum plot with systematic shifts
// cafe demo4.C

#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"

#include "CAFAna/Cuts/TruthCuts.h"

#include "StandardRecord/Proxy/SRProxy.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TPad.h"

using namespace ana;

void demo4()
{
  // Copying basic setup from demo0.C
  const std::string fname = "/pnfs/dune/persistent/users/LBL_TDR/CAFs/v4/FD_FHC_nonswap.root";
  SpectrumLoader loader(fname);
  const Var kRecoEnergy = SIMPLEVAR(Ev_reco_numu);
  const Binning binsEnergy = Binning::Simple(40, 0, 10);
  const HistAxis axEnergy("Reco energy (GeV)", binsEnergy, kRecoEnergy);
  const double pot = 3.5 * 1.47e21 * 40/1.13;

  // This is the nominal energy spectrum
  Spectrum sEnergy(loader, axEnergy, kIsNumuCC);

  // Systematics work by modifying the event record before it's filled into the
  // spectrum. These generally should be added into a header like Systs.h
  // rather than implemented inline like this.
  class ToyEnergyScaleSyst: public ISyst
  {
  public:
    ToyEnergyScaleSyst() : ISyst("toyEScale", "Toy Energy Scale")
    {
    }

    // Function that will be called to actually do the shift
    void Shift(double sigma,
               caf::SRProxy* sr,
               double& weight) const override
    {
      // Edit the event record
      const double scale = 1 + .1*sigma;
      sr->Ev_reco_numu *= scale;
    }
  };
  const ToyEnergyScaleSyst eSyst;

  // Make systematically shifted variants of the spectrum above
  Spectrum sEnergyUp(loader, axEnergy, kIsNumuCC, SystShifts(&eSyst, +1));
  Spectrum sEnergyDn(loader, axEnergy, kIsNumuCC, SystShifts(&eSyst, -1));

  class ToyNormSyst: public ISyst
  {
  public:
    ToyNormSyst() : ISyst("toyNorm", "Toy Norm Syst")
    {
    }

    void Shift(double sigma,
               caf::SRProxy* sr,
               double& weight) const override
    {
      // A systematic can also reweight events, based on whatever criteria you
      // want.
      if(sr->Ev_reco_numu < 2) weight *= 1+0.2*sigma;
    }
  };
  const ToyNormSyst nSyst;

  Spectrum sNormUp(loader, axEnergy, kIsNumuCC, SystShifts(&nSyst, +1));
  Spectrum sNormDn(loader, axEnergy, kIsNumuCC, SystShifts(&nSyst, -1));

  // Fill all the various shifted spectra
  loader.Go();


  sEnergyDn.ToTH1(pot, kBlue)->Draw("hist");
  sEnergyUp.ToTH1(pot, kRed)->Draw("hist same");
  sEnergy.ToTH1(pot)->Draw("hist same");

  new TCanvas;
  sNormUp.ToTH1(pot, kRed)->Draw("hist");
  sNormDn.ToTH1(pot, kBlue)->Draw("hist same");
  sEnergy.ToTH1(pot)->Draw("hist same");
}
