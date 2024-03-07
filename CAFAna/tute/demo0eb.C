
#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/EnsembleSpectrum.h"
#include "CAFAna/Core/EnsembleRatio.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Multiverse.h"
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

   // loop through particles and change record to shift energy 
    for (int i=0; i<ixn->part.ndlp; i++){
       if ( abs(ixn->part.dlp[i].pdg)==13) {
          ixn->part.dlp[i].E *= 1. + scale;
        }
    }
 }
 
 const EnergyScaleMu kEnergyScaleMu;

//// A second dummy syst to make a multiverse make sense

// Var that takes energy of the most energetic muon in interaction
const Var kEnergyInteractionMu([](const caf::SRInteractionProxy* ixn)
  { 
      double e = -5.;

      int numMuons = 0;
      for (int i=0; i<ixn->part.ndlp; i++){
        // check if muon
        //std::cout<< "we are in particle "<< i<<std::endl;
        if ( abs(ixn->part.dlp[i].pdg)==13) {
          numMuons ++;          
          //this muons energy
          double tempE = ixn->part.dlp[i].E;
          //std::cout<< "found muon # "<< numMuons<< ", idx "<<i<<" with energy "<< tempE<<std::endl;
          // in case there is another muon,           
          if( numMuons>1 ){
            // check which one has more energy
            if (tempE>e)
              e=tempE; // store if this other muon has more energy
          }else{ e = tempE;}
        }
                    
      }              
      return e;     
  });


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
  fin.Close();
  //----------------------------------------------------------------------//
  // now try to figure out how to make ensemble spectra
  // SBN Vars and Cuts still
 // const Var kTrueE = SIMPLEVAR(truth.E);

  //  const Cut kNumuSel = kSlcNuScoreCut && kInFV && kSlcFlashMatchTimeCut && kSlcFlashMatchScoreCut;
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


  const Binning binsEnergy = Binning::Simple(30, 0, 3);

  const HistAxis axMuEnergy("Muon energy (GeV)", binsEnergy, kEnergyInteractionMu);

  // this is key line
  //std::vector<Weight> weis;
  //weis.reserve(101);
  //weis.push_back(kUnweighted); // nominal
  // TO DO: generate multiverse weights
  //for(int i = 0; i < 99; ++i) weis.push_back(GetUniverseWeight("multisim_Genie", i));

  const std::string fname = "/pnfs/dune/persistent/users/mkramer/productions/MiniRun4.5_1E19_RHC/CAF_beta3/CAF/0000000/MiniRun4.5_1E19_RHC.caf.0000*";
  // Source of events
  SpectrumLoader loader(fname);
  // This is another key line // dont know what is the 2nd [cut] applied to...
  //EnsembleSpectrum sCC(loader.Interactions(RecoType::kDLP).Ensemble(weis)[kContainment][kIsNumuCC], axEnergy);
  // weis is a vector of weights that come from GetUniverseWeight
  // or whatever multiverse object, in this case randomly generated gaussian shifts
  EnsembleSpectrum sCC(loader.Interactions(RecoType::kDLP).Ensemble(gas)[kNoCut][kNoCut], axMuEnergy);

  //Spectrum sMuE(loader.Interactions(RecoType::kDLP)[kNoCut], axMuEnergy);
  //Spectrum sMuEcont(loader.Interactions(RecoType::kDLP)[kContainment], axMuEnergy);

  loader.Go();


 const double pot = 3.5 * 1.47e21 * 40/1.13;
  // We are forcing the pot value because cafs dont have this information yet
  sCC.OverridePOT(pot);
  //sMuE.OverridePOT(pot);
  //sMuEcont.OverridePOT(pot);

  //sMuE.ToTH1(pot, kRed)->Draw("hist");
  //sMuEcont.ToTH1(pot, kBlue)->Draw("hist same");

  new TCanvas;
  TH1 * hsCC =  sCC.Nominal().ToTH1(pot, kRed);
  hsCC->Draw("hist");

  // a vector to store ratios to nominal
  std::vector<TH1*> hratios;
  for(unsigned int i = 0; i < sCC.NUniverses(); ++i){
    hratios.push_back( (TH1*)hsCC->Clone( ("ratio_"+std::to_string(i)).c_str() ) );
    sCC.Universe(i).ToTH1(pot, kRed-10)->Draw("hist same");    
    hratios.back()->Divide(sCC.Universe(i).ToTH1(pot, kRed-10));
  }  
  hsCC->Draw("hist same");
  
  hratios[0]->SetMinimum(.8);
  hratios[0]->SetMaximum(1.2);
  new TCanvas;
  for (unsigned int i = 0; i < sCC.NUniverses(); ++i)
    hratios[i]->Draw("hist same");


  new TCanvas;
  TGraphAsymmErrors* eBand =sCC.ErrorBand(pot);
  DrawErrorBand(hsCC, eBand);



  //TFile fout(state_fname.c_str(), "RECREATE");
  //sCC.SaveTo(&fout, "cc");

  //----------------------------------------------------------------------//
  // Reopen file and plot a systematic error band. 

}
