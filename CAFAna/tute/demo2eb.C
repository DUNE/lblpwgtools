
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
#include "CAFAna/Core/ISyst.h"
#include "demo0eb.C"

#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TPad.h"
#include "TColor.h"
#include <iostream>

using namespace ana;



 class DummySyst: public ISyst
  {
  public:
  DummySyst(const std::string& shortName, const std::string& longName)
      :ISyst(shortName, longName) {}
      virtual void Shift(double, caf::SRInteractionProxy*, double&) const {}
  };


   // declare a particular systematic and names
  // varied the ionization potential (W_ion) parameter in larnd-sim to +/-3 sigma 
  // (to 25.1 and 22.7 eV respectively) from its nominal value (23.6 eV).
  extern const DummySyst kWIonSyst("WIon", "Ionization Potential"); 
  extern const DummySyst kDummyNominalSyst("Nominal","Nominal");

  std::vector<int> GetDummyShifts(const ISyst* s)
  {
    // do for each different file syst, some may be one-sided, etc...
    // not sure if should label it as +/- 1 or 3.. anyways it is easily changed
    if (s == &kWIonSyst ) return {-1,0,1};
    else return {0};
  };

struct ShiftedSpec
{
  Spectrum spec;
  SystShifts shift;
};


// Script to make a basic ensemble spectrum 
// Based on file systematics
// So fill up a nominal
// Fill Plus and minus 1 sigma versions
// This is a cross systematic type I guess
// Instantiate a dummy systematic to construct Multiverse
// Get a wrapper that merges various spectra into a simgle ensemble spectrum thing...

void demo2eb()
{
 


 // test multiverse
  std::vector<const ISyst*> systs;            //,systs2;
  systs.push_back(& );


  // scans from -n to n sigma shifts, we'll do only the 1 sigma
  const Multiverse& cross = Multiverse::Hypercross(systs,1);

  std::cout << cross.LatexName() << std::endl << cross.ShortName() << std::endl;

  // we can save our multiverses to file
  TFile fout("test_multiverse.root", "RECREATE");
  cross.SaveTo(&fout, "cross");
  fout.Close();
  
  // and then retrieve multiverses from file
  TFile fin("test_multiverse.root");
  const FitMultiverse& cross2 = *Multiverse::LoadFrom(&fin, "cross");


  // you can print the hypercross universe to see what is going on
  for (int i = 0; i< cross.NUniv(); i++){
      for (const auto & [ key, value ] : cross.GetUniverse(i)) {
      std::cout << key->ShortName() << ": " << value << std::endl;
      } 
  }
    fin.Close();
  //----------------------------------------------------------------------//
  // now  make ensemble spectra

  const Binning binsEnergy = Binning::Simple(10, 0, 1);

  const HistAxis axMuEnergy("Muon energy (GeV)", binsEnergy, kEnergyInteractionMu);
  const HistAxis axEnergy("Interaction energy (GeV)", binsEnergy, kEnergyInteractionAll);
  const HistAxis axNoMuEnergy("non-muon energy (GeV)", binsEnergy,kEnergyInteractionButMu);

  const RecoPartHistAxis axRecoPartEnergy("Reco Particle Energy", binsEnergy, kRecoParticleEnergy);


  //const std::string fname_nom = "/exp/dune/data/users/mcasales/test_cafs/MiniRun5_1E19_RHC.caf.0001013.CAF.root";
  // also beta2a
  const std::string fname_nom = "/exp/dune/data/users/noeroy/mywork/MiniRun5_Fixed_truth/MiniRun5_1E19_RHC.caf/CAF.flat/0000000/*.flat.root";
//"/exp/dune/data/users/noeroy/prod/MiniRun5_1E19_RHC/MiniRun5_1E19_RHC.caf.beta1/CAF/0000000/MiniRun5_1E19_RHC.caf.0000*";
  const std::string fname_up = "/pnfs/dune/persistent/users/cuddandr/2x2_production/MiniRun5_Systematics_W_ION_22.7/MiniRun5_Systematics_W_ION_22.7.caf/CAF/0000000/MiniRun5_Systematics_W_ION_22.7.caf.*";
  const std::string fname_down = "/pnfs/dune/persistent/users/cuddandr/2x2_production/MiniRun5_Systematics_W_ION_25.1/MiniRun5_Systematics_W_ION_25.1.caf/CAF/0000000/MiniRun5_Systematics_W_ION_25.1.caf.*";

  // Source of events
  SpectrumLoader loader_nom(fname_nom);
  SpectrumLoader loader_up(fname_up);
  SpectrumLoader loader_down(fname_down);

  // Fill various spectra with these...
  Spectrum sEnergyMatchedTrueMu_nom(loader_nom.Interactions(RecoType::kDLP)[kHas1RecoMuon],    axMuEnergy);
  Spectrum sEnergyMatchedTrueMu_up(loader_up.Interactions(RecoType::kDLP)[kHas1RecoMuon],    axMuEnergy);
  Spectrum sEnergyMatchedTrueMu_down(loader_down.Interactions(RecoType::kDLP)[kHas1RecoMuon],    axMuEnergy);

//  Spectrum sEnergyMatchedTrueMu_nom(loader_nom.Interactions(RecoType::kDLP).RecoParticles(RecoType::kDLP)[kIsMuon && kMatchedToTrueMuon], axEnergy);
//  Spectrum sEnergyMatchedTrueMu_up(loader_up.Interactions(RecoType::kDLP).RecoParticles(RecoType::kDLP)[kIsMuon && kMatchedToTrueMuon], axEnergy);
//  Spectrum sEnergyMatchedTrueMu_down(loader_down.Interactions(RecoType::kDLP).RecoParticles(RecoType::kDLP)[kIsMuon && kMatchedToTrueMuon], axEnergy);
 
  loader_nom.Go();
  loader_up.Go();
  loader_down.Go();

  // Save the three spectra to file
  TFile fout2("multiverse_ensemble_spec.root", "RECREATE");
  sEnergyMatchedTrueMu_nom.SaveTo(&fout2, "sEnergyMatchedTrueMu_nom");
  sEnergyMatchedTrueMu_up.SaveTo(&fout2, "sEnergyMatchedTrueMu_up");
  sEnergyMatchedTrueMu_down.SaveTo(&fout2, "sEnergyMatchedTrueMu_down");  
  cross.SaveTo(&fout2, "cross");
  fout2.Close();

  TFile * fIn = new TFile("multiverse_ensemble_spec.root");
  std::vector<Spectrum> specs= {};
  // make sure to place nominal first...
  specs.push_back(*Spectrum::LoadFrom(fIn, "sEnergyMatchedTrueMu_nom").release() );
  // then -1 shift
  specs.push_back(*Spectrum::LoadFrom(fIn, "sEnergyMatchedTrueMu_down").release());
  // then +1 shift
  specs.push_back(*Spectrum::LoadFrom(fIn, "sEnergyMatchedTrueMu_up").release() );


  // get from file
  const FitMultiverse& cross22 = *Multiverse::LoadFrom(fIn, "cross");
 
  EnsembleSpectrum sEnergyMatchedWIon = EnsembleSpectrum::MergeSpectra(specs, &cross22);//, axMuEnergy );


  TFile fout3("multiverse_ensemble_spec_2.root", "RECREATE");
  sEnergyMatchedWIon.SaveTo(&fout3, "sEnergyMatchedWIon");
  fout3.Close();

  // Read spectra from file
  TFile fin2("multiverse_ensemble_spec_2.root");

  EnsembleSpectrum sEnergyMatchedWIon2  =*EnsembleSpectrum::LoadFrom(&fin2, "sEnergyMatchedWIon");

 // ------- More canvases for the hypercross univ
  double pot = sEnergyMatchedTrueMu_nom.POT();
  new TCanvas;
  TH1 * hsMuonEnsembleX =  sEnergyMatchedWIon2.Nominal().ToTH1(pot, kBlue);
  hsMuonEnsembleX->SetMaximum(1.3*hsMuonEnsembleX->GetMaximum());
  hsMuonEnsembleX->Draw("hist");
  // a vector to store ratios to nominal
  std::vector<TH1*> hratiosx;
  std::cout<< "cross has "<<sEnergyMatchedWIon2.NUniverses()<< " universes\n";
  for(unsigned int i = 0; i < sEnergyMatchedWIon2.NUniverses(); ++i){
    // using different colors per systematic
    Color_t color = kBlue;
    if (i==0) color = kBlue;
    if (i== 1 || i==2) color = kViolet +1;
    if (i==3 || i==4) color = kPink;

    sEnergyMatchedWIon2.Universe(i).ToTH1(pot, color,kDashed)->Draw("hist same");     
    hratiosx.push_back(sEnergyMatchedWIon2.Universe(i).ToTH1(pot, color,kDashed));
    hratiosx.back()->Divide(hsMuonEnsembleX); 

  }  
  hsMuonEnsembleX->Draw("hist same");
  gPad->SaveAs("spec_ensemble_test.pdf");
 
  hratiosx[0]->SetMinimum(.6);
  hratiosx[0]->SetMaximum(1.4);
  new TCanvas;
  for (unsigned int i = 0; i < sEnergyMatchedWIon2.NUniverses(); ++i)
    hratiosx[i]->Draw("hist same");
  gPad->SaveAs("spec_ensemble_test_ratios.pdf");

  
  new TCanvas;
    // Grab covariance matrix
  Eigen::MatrixXd m= sEnergyMatchedWIon2.CovarianceMatrix(pot);
  std::cout << "The matrix m is of size "  << m.rows() << "x" << m.cols() << std::endl;
  // convert to th2 for drawing 
  TH2F* hCov = EigenMatrixToTH2(m);  
  new TCanvas;
  hCov->Draw("colz");
  gPad->SaveAs("cov_mx_ion.pdf");

  /// Drawing the spectra directly 
 new TCanvas; 
 TH1 * hsMuonEnsembleX2 = sEnergyMatchedTrueMu_nom.ToTH1(pot, kBlack);
 hsMuonEnsembleX2->SetMaximum(1.3*hsMuonEnsembleX2->GetMaximum());
 hsMuonEnsembleX2->Draw("hist");
 sEnergyMatchedTrueMu_up.ToTH1(pot, kBlue)->Draw("hist same");;
 sEnergyMatchedTrueMu_down.ToTH1(pot, kPink)->Draw("hist same");;

}
