// Make a simple spectrum plot with systematic shifts
// cafe demo4.C

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

void demo4()
{
  // Copying basic setup from demo0.C
  const std::string fname = "/pnfs/dune/persistent/TaskForce_AnaTree/far/train/v3.2/nu.mcc10.1_def.root";
  SpectrumLoader loader(fname);
  auto* loaderBeam  = loader.LoaderForRunPOT(20000001);
  auto* loaderNue   = loader.LoaderForRunPOT(20000002);
  auto* loaderNuTau = loader.LoaderForRunPOT(20000003);
  auto* loaderNC    = loader.LoaderForRunPOT(0);
  const Var kRecoEnergy = SIMPLEVAR(dune.Ev_reco_numu);
  const Binning binsEnergy = Binning::Simple(40, 0, 10);
  const HistAxis axEnergy("Reco energy (GeV)", binsEnergy, kRecoEnergy);
  const double pot = 3.5 * 1.47e21 * 40/1.13;

  // This is the nominal energy spectrum
  Spectrum sEnergy(*loaderBeam, axEnergy, kIsNumuCC);

  // Systematics work by modifying the event record before it's filled into the
  // spectrum. These generally should be added into a header like Systs.h
  // rather than implemented inline like this.
  class ToyEnergyScaleSyst: public ISyst
  {
  public:
    std::string ShortName() const override {return "toyEScale";}
    std::string LatexName() const override {return "Toy Energy Scale";}

    // Function that will be called to actually do the shift
    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override
    {
      // First - register all the variables that will need to be restored to
      // return the record to nominal
      restore.Add(sr->dune.Ev_reco_numu);

      // Then edit the event record
      const double scale = 1 + .1*sigma;
      sr->dune.Ev_reco_numu *= scale;
    }
  };
  const ToyEnergyScaleSyst eSyst;

  // Make systematically shifted variants of the spectrum above
  Spectrum sEnergyUp(*loaderBeam, axEnergy, kIsNumuCC, SystShifts(&eSyst, +1));
  Spectrum sEnergyDn(*loaderBeam, axEnergy, kIsNumuCC, SystShifts(&eSyst, -1));

  class ToyNormSyst: public ISyst
  {
  public:
    std::string ShortName() const override {return "toyNorm";}
    std::string LatexName() const override {return "Toy Norm syst";}

    void Shift(double sigma,
               Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const override
    {
      // A systematic can also reweight events, based on whatever criteria you
      // want.
      if(sr->dune.Ev_reco_numu < 2) weight *= 1+0.2*sigma;
    }
  };
  const ToyNormSyst nSyst;

  Spectrum sNormUp(*loaderBeam, axEnergy, kIsNumuCC, SystShifts(&nSyst, +1));
  Spectrum sNormDn(*loaderBeam, axEnergy, kIsNumuCC, SystShifts(&nSyst, -1));

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
