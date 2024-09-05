
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
#include "CAFAna/Core/Utilities.h"

#include "demo0eb.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TPad.h"
#include "TColor.h"
#include <iostream>

using namespace ana;

// Make a few basic ensemble spectrum
// based on https://github.com/SBNSoftware/sbnana/blob/feature/ext_cafanacore/sbnana/CAFAna/test/test_ensemble.C 
void demo0eb(std::string option="caf")
{
 
 // test multiverse
  std::vector<const ISyst*> systs;        
  systs.push_back(&kEnergyScaleMu);
  systs.push_back(&kMuonRwgtSyst);
  //systs.push_back(&kDummyRwgtSyst);

  // Two methods of generatig multiverse
  // 100 Random values grom a gaussian distribution
  // using the specific seed = 42
  const Multiverse& gas = Multiverse::RandomGas(systs, 100, 42);
  // scans from -n to n sigma shifts, we'll do only the 1 sigma
  const Multiverse& cross = Multiverse::Hypercross(systs,1);

  std::cout << gas.LatexName() << std::endl << gas.ShortName() << std::endl;
  std::cout << cross.LatexName() << std::endl << cross.ShortName() << std::endl;

  std::cout << "gas has "<< gas.NUniv() << "universes from NUniv()\n";
 std::cout << "cross has "<< cross.NUniv() << "universes from NUniv()\n";
  // we can save our multiverses to file
  TFile fout("test_multiverse.root", "RECREATE");
  gas.SaveTo(&fout, "gas");
  cross.SaveTo(&fout, "cross");
  fout.Close();
  
  // and then retrieve multiverses from file
  TFile fin("test_multiverse.root");
  const FitMultiverse& gas2 = *Multiverse::LoadFrom(&fin, "gas");
  const FitMultiverse& cross2 = *Multiverse::LoadFrom(&fin, "cross");

// you can print out exactly what are the values of each multiverse.
/*  for (int i = 0; i< gas2.NUniv(); i++){
      for (const auto & [ key, value ] : gas2.GetUniverse(i)) {
      std::cout << key->ShortName() << ": " << value << std::endl;
      } 
  }
*/
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

  const std::string fname =  "/exp/dune/data/users/noeroy/mywork/MiniRun5_Fixed_truth/MiniRun5_1E19_RHC.caf/CAF.flat/0000000/*.flat.root";

  // Source of events
  SpectrumLoader loader(fname);

  // Use multiverse object in Ensemble() , in this case randomly generated gaussian shifts
  EnsembleSpectrum sMuonEnsembleGauss(loader.Interactions(RecoType::kDLP).Ensemble(gas)[kHas1RecoMuon],    axMuEnergy);
  EnsembleSpectrum sMuonEnsembleX(loader.Interactions(RecoType::kDLP).Ensemble(cross)[kHas1RecoMuon], axMuEnergy);

  // The same but for a different variable  
  EnsembleSpectrum sAllEnsembleX(loader.Interactions(RecoType::kDLP).Ensemble(cross)[kHas1RecoMuon], axEnergy);
  EnsembleSpectrum sNoMuEnsembleX(loader.Interactions(RecoType::kDLP).Ensemble(cross)[kHas1RecoMuon], axNoMuEnergy);
  
  // Another way to access the same information from the reco particles branch 
  //seems longer to write but it saves you from writing 3 different histaxis (axMuEnergy,axEnergy,axNoMuEnergy)
  EnsembleSpectrum sMuonEnsemblePartX(loader
    .Interactions(RecoType::kDLP)
    .Ensemble(cross)[kHas1RecoMuon]
    .RecoParticles(RecoType::kDLP)[kIsMuon], 
    axRecoPartEnergy);
  // Doing the same for other particles  (not muon) to verify our systematic is working as expected
  EnsembleSpectrum sOtherEnsemblePartX(loader
    .Interactions(RecoType::kDLP)
    .Ensemble(cross)[kHas1RecoMuon]
    .RecoParticles(RecoType::kDLP)[ !kIsMuon ], 
    axRecoPartEnergy);
 

  loader.Go();

  const double pot = sMuonEnsembleGauss.POT();// 3.5 * 1.47e21 * 40/1.13;


  //----------- save to file
  TFile fout2("multiverse_ensemble_test.root", "RECREATE");
  sMuonEnsembleGauss.SaveTo(&fout2, "sMuonEnsembleGauss");
  sMuonEnsembleX.SaveTo(&fout2, "sMuonEnsembleX");
  fout2.Close();


  //----------- Display things in canvases for the gaussian random multiverse
  // exercise load from file as well

  TFile fin2("multiverse_ensemble_test.root");
  EnsembleSpectrum sMuonEnsembleGaussFile= *EnsembleSpectrum::LoadFrom(&fin2, "sMuonEnsembleGauss");
  EnsembleSpectrum sMuonEnsembleXFile= *EnsembleSpectrum::LoadFrom(&fin2, "sMuonEnsembleX");


  new TCanvas;
  TH1 * hsMuonEnsembleGauss =  sMuonEnsembleGaussFile.Nominal().ToTH1(pot, kRed);
  hsMuonEnsembleGauss->Draw("hist");

  // a vector to store ratios to nominal
  std::vector<TH1*> hratios;
  for(unsigned int i = 0; i < sMuonEnsembleGaussFile.NUniverses(); ++i){
    hratios.push_back(sMuonEnsembleGaussFile.Universe(i).ToTH1(pot, kRed-10));

    sMuonEnsembleGaussFile.Universe(i).ToTH1(pot, kRed-10)->Draw("hist same");    
    
    hratios.back()->Divide(hsMuonEnsembleGauss);
  }  
  hsMuonEnsembleGauss->Draw("hist same");
  gPad->SaveAs("multiverse_gauss.pdf");

  hratios[0]->SetMinimum(.6);
  hratios[0]->SetMaximum(1.4);
  new TCanvas;
  for (unsigned int i = 0; i < sMuonEnsembleGaussFile.NUniverses(); ++i)
    hratios[i]->Draw("hist same");
  gPad->SaveAs("multiverse_gauss_ratios.pdf");
  
  new TCanvas;
  // Error band summing in quadrature
  TGraphAsymmErrors* eBand =sMuonEnsembleGaussFile.ErrorBand(pot);
  DrawErrorBand(hsMuonEnsembleGauss, eBand);

  gPad->SaveAs("multiverse_ErrorBand.pdf");


  // Grab covariance matrix
  Eigen::MatrixXd m= sMuonEnsembleGaussFile.CovarianceMatrix(pot);
  std::cout << "The matrix m is of size "  << m.rows() << "x" << m.cols() << std::endl;
  // convert to th2 for drawing         
  TH2F* hCov = EigenMatrixToTH2(m);  
  new TCanvas;
  hCov->Draw("colz");
  gPad->SaveAs("cov_mx.pdf");

  // grab bias matrix
  Eigen::MatrixXd mb= sMuonEnsembleGaussFile.BiasMatrix(pot);
  TH2F* hBias = EigenMatrixToTH2(mb);
  new TCanvas;
  hBias->Draw("colz");
  gPad->SaveAs("bias_mx.pdf");

 // ------- More canvases for the hypercross univ
  new TCanvas;
  TH1 * hsMuonEnsembleX =  sMuonEnsembleXFile.Nominal().ToTH1(pot, kBlue);
  hsMuonEnsembleX->SetMaximum(1.3*hsMuonEnsembleX->GetMaximum());
  hsMuonEnsembleX->Draw("hist");
  // a vector to store ratios to nominal
  std::vector<TH1*> hratiosx;
  std::cout<< "cross has "<<sMuonEnsembleXFile.NUniverses()<< " universes\n";
  for(unsigned int i = 0; i < sMuonEnsembleXFile.NUniverses(); ++i){
    // using different colors per systematic
    Color_t color = kBlue;
    if (i==0) color = kBlue;
    if (i== 1 || i==2) color = kViolet +1;
    if (i==3 || i==4) color = kPink;

    sMuonEnsembleXFile.Universe(i).ToTH1(pot, color,kDashed)->Draw("hist same");     
    hratiosx.push_back(sMuonEnsembleXFile.Universe(i).ToTH1(pot, color,kDashed));
    hratiosx.back()->Divide(hsMuonEnsembleX); 

  }  
  hsMuonEnsembleX->Draw("hist same");
  gPad->SaveAs("multiverse_x_.pdf");

  hratiosx[0]->SetMinimum(.6);
  hratiosx[0]->SetMaximum(1.4);
  new TCanvas;
  for (unsigned int i = 0; i < sMuonEnsembleXFile.NUniverses(); ++i)
    hratiosx[i]->Draw("hist same");
  gPad->SaveAs("multiverse_x_ratios.pdf");
  // we dont get covariance out of this one

//--------------------------------

 new TCanvas;

  TH1 * hsMuonEnsembleX2 =   sMuonEnsemblePartX.Nominal().ToTH1(pot, kGreen+2);
  TH1 * hsNoMuonEnsembleX2 =  sOtherEnsemblePartX.Nominal().ToTH1(pot, kPink);
  

  hsMuonEnsembleX2->SetMaximum(1.3*hsMuonEnsembleX2->GetMaximum());
  hsMuonEnsembleX2->Draw("hist");
  hsNoMuonEnsembleX2->Draw("hist same");
  for(unsigned int i = 0; i < sMuonEnsemblePartX.NUniverses(); ++i){
    // using different colors per systematic
    Color_t color = kGreen+2;
    Color_t color2 =kBlack ;
    if (i==0){ color = kGreen+2; color2 =kBlack;}
    if (i== 1 || i==2) {color = kViolet +1; color2 =kAzure+2;}
    if (i==3 || i==4) {color = kPink; color2=kTeal-7;}

    sMuonEnsemblePartX.Universe(i).ToTH1(pot, color,kDashed)->Draw("hist same");  
    sOtherEnsemblePartX.Universe(i).ToTH1(pot, color2,kDashed)->Draw("hist same");  

  }  


}
