
#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/Multiverse.h"
#include "CAFAna/Core/ISyst.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TPad.h"

#include <iostream>

using namespace ana;

// Setup a dummy systematic that shifts Particle (Muon) Enegy by +/- 10 percent

 class EnergyScaleMu: public ISyst {
 public:
   EnergyScaleMu() : ISyst("EnergyScaleMu", "Muon Energy Scale Syst") {}
   void Shift(double sigma,
              caf::SRInteractionProxy* ixn, double& weight) const override;
 };
  void EnergyScaleMu::Shift(double sigma,
                            caf::SRInteractionProxy* ixn, double& weight) const {
   double scale = .1 * sigma;

   // loop through interactions and 
    for (int i=0; i<ixn->part.ndlp; i++){
       if ( abs(ixn->part.dlp[i].pdg)==13) {
          ixn->part.dlp[i].E *= 1. + scale;
        }
    }
 }
 
 const EnergyScaleMu kEnergyScaleMu;

//// A second dummy syst to make a multiverse make sense




// Make a basic ensemble spectrum?
// based on https://github.com/SBNSoftware/sbnana/blob/feature/ext_cafanacore/sbnana/CAFAna/test/test_ensemble.C 
void demo0eb()
{
 

 // test multiverse

  std::vector<const ISyst*> systs;
  systs.push_back(&kEnergyScaleMu);

  const Multiverse& gas = Multiverse::RandomGas(systs, 100, 42);//Multiverse::kTrulyRandom);

  std::cout << gas.LatexName() << std::endl << gas.ShortName() << std::endl;

  TFile fout("test_multiverse.root", "RECREATE");

  gas.SaveTo(&fout, "gas");

  fout.Close();
  

  TFile fin("test_multiverse.root");
  const FitMultiverse& gas2 = *Multiverse::LoadFrom(&fin, "gas");

  std::cout << gas2.LatexName() << std::endl << gas2.ShortName() << std::endl;

  //----------------------------------------------------------------------//
  // now try to figure out how to make ensemble spectra
  const Var kTrueE = SIMPLEVAR(truth.E);

  const Cut kNumuSel = kSlcNuScoreCut && kInFV && kSlcFlashMatchTimeCut && kSlcFlashMatchScoreCut;

  const Binning binsEnergy = Binning::Simple(30, 0, 3);

  const HistAxis axEnergy("True energy (GeV)", binsEnergy, kTrueE);

  std::vector<Weight> weis;
  weis.reserve(101);

  weis.push_back(kUnweighted); // nominal
  // TO DO: generate multiverse weights
  for(int i = 0; i < 99; ++i) weis.push_back(GetUniverseWeight("multisim_Genie", i));

  // This is the key line 
  EnsembleSpectrum sCC(loader.Slices().Ensemble(weis)[kNumuSel][kIsNumuCC], axEnergy);


  loader.Go();

  TFile fout(state_fname.c_str(), "RECREATE");
  sCC.SaveTo(&fout, "cc");

  //----------------------------------------------------------------------//
  // Reopen file and plot a systematic error band. 

}
