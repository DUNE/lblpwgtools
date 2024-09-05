
#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "duneanaobj/StandardRecord/Navigate.h"
#include "duneanaobj/StandardRecord/SREnums.h"
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
#include "demo0eb.C"
#include "demo_systs.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TPad.h"
#include "TColor.h"
#include <iostream>

using namespace ana;


void demo_systs_file()
{
 

  std::vector<const ISyst*> systs;     
  systs.push_back(&kWIonSyst);

  std::vector<const ISyst*> systs2;
  systs2.push_back(&kEnergyScaleTrueMu);
  systs2.push_back(&kMECScaleSyst);

  // scans from -n to n sigma shifts, we'll do only the 1 sigma
  const Multiverse& cross = Multiverse::Hypercross(systs,1);
  const Multiverse& cross_rwgt = Multiverse::Hypercross(systs2,1);
  
  std::cout << cross.LatexName() << ",   "<< cross.ShortName() << std::endl;
  for (int i = 0; i< cross.NUniv(); i++){
      for (const auto & [ key, value ] : cross.GetUniverse(i)) {
      std::cout << key->ShortName() << ": " << value << std::endl;
      } 
  }
  //----------------------------------------------------------------------//
  // now  make ensemble spectra

  const Binning binsEnergy = Binning::Simple(10, 0, 1);

  const HistAxis axMuEnergy("Muon energy (GeV)", binsEnergy, kEnergyInteractionMu);

 // const std::string fname_nom = "/exp/dune/data/users/noeroy/mywork/MiniRun5_Fixed_truth/MiniRun5_1E19_RHC.caf/CAF.flat/0000000/*.flat.root";
  const std::string fname_nom = "/exp/dune/data/users/noeroy/prod/MiniRun5_1E19_RHC/MiniRun5_1E19_RHC.caf.beta1/CAF.flat/0000000/MiniRun5_1E19_RHC.caf.*";
  const std::string fname_up =  "/pnfs/dune/persistent/users/cuddandr/2x2_production/MiniRun5_Systematics_W_ION_22.7/MiniRun5_Systematics_W_ION_22.7.caf/CAF.flat/0000000/MiniRun5_Systematics_W_ION_22.7.caf.*";
  const std::string fname_down = "/pnfs/dune/persistent/users/cuddandr/2x2_production/MiniRun5_Systematics_W_ION_25.1/MiniRun5_Systematics_W_ION_25.1.caf/CAF.flat/0000000/MiniRun5_Systematics_W_ION_25.1.caf.*";

  // Source of events
  SpectrumLoader loader_nom(fname_nom);
  SpectrumLoader loader_up(fname_up);
  SpectrumLoader loader_down(fname_down);

  // Fill various spectra with these...
  Spectrum sEnergyMatchedTrueMu_nom(  loader_nom.Interactions(RecoType::kDLP)[kHas1RecoMuon],    axMuEnergy);
  Spectrum sEnergyMatchedTrueMu_up(   loader_up.Interactions(RecoType::kDLP)[kHas1RecoMuon],    axMuEnergy);
  Spectrum sEnergyMatchedTrueMu_down( loader_down.Interactions(RecoType::kDLP)[kHas1RecoMuon],    axMuEnergy);

  // other ensemble with two reweight systematics
  EnsembleSpectrum sMuonEnegyEnsemble(loader_nom.Interactions(RecoType::kDLP).Ensemble(cross_rwgt)[kHas1RecoMuon], axMuEnergy );

  loader_nom.Go();
  loader_up.Go();
  loader_down.Go();

  // Save the three spectra to file
  TFile fout2("multiverse_ensemble_spec.root", "RECREATE");
  sEnergyMatchedTrueMu_nom.SaveTo(&fout2, "sEnergyMatchedTrueMu_nom");
  sEnergyMatchedTrueMu_up.SaveTo(&fout2, "sEnergyMatchedTrueMu_up");
  sEnergyMatchedTrueMu_down.SaveTo(&fout2, "sEnergyMatchedTrueMu_down");  
  sMuonEnegyEnsemble.SaveTo( &fout2, "sMuonEnegyEnsemble");

  cross.SaveTo(&fout2, "cross");
  cross_rwgt.SaveTo(&fout2, "cross_rwgt");
  fout2.Close();

  TFile * fIn = new TFile("multiverse_ensemble_spec.root");
  std::vector<Spectrum> specs= {};
  // make sure to place nominal first,then -1 shift,  then +1 shift
  specs.push_back(*Spectrum::LoadFrom(fIn, "sEnergyMatchedTrueMu_nom").release() );
  specs.push_back(*Spectrum::LoadFrom(fIn, "sEnergyMatchedTrueMu_down").release());
  specs.push_back(*Spectrum::LoadFrom(fIn, "sEnergyMatchedTrueMu_up").release() );

  // get from file and merge into a single specEnsmbl
  const FitMultiverse& crossFile = *Multiverse::LoadFrom(fIn, "cross");
  EnsembleSpectrum sEnergyMatchedWIon = EnsembleSpectrum::MergeSpectra(specs, &crossFile);

  //----------------------------------------------------------------------
  //Merge ensembles. This Multiverse has to have the systs in proper order 
  //(same as the ensembles you're gvign it)
  const FitMultiverse& crossCombined = Multiverse::Hypercross(
    {&kWIonSyst, &kEnergyScaleTrueMu, &kMECScaleSyst},1);
  EnsembleSpectrum sEnsembleMerged = EnsembleSpectrum::MergeEnsembles( 
                                                      {sEnergyMatchedWIon,sMuonEnegyEnsemble}, &crossCombined);
//----------------------------------------------------------------------

  TFile fout3("multiverse_ensemble_spec_2.root", "RECREATE");
  sEnergyMatchedWIon.SaveTo( &fout3, "sEnergyMatchedWIon");
  sMuonEnegyEnsemble.SaveTo( &fout3, "sMuonEnegyEnsemble");
  sEnsembleMerged.SaveTo(    &fout3, "sEnsembleMerged");
  fout3.Close();

  // Read spectra from file
  TFile fin2("multiverse_ensemble_spec_2.root");
  EnsembleSpectrum sEnergyMatchedWIon2  =*EnsembleSpectrum::LoadFrom(&fin2, "sEnergyMatchedWIon");
  EnsembleSpectrum sEnsembleMerged2  =*EnsembleSpectrum::LoadFrom(&fin2, "sEnsembleMerged");

 // -- draw various histograms
  double pot = sEnergyMatchedTrueMu_nom.POT();
  new TCanvas;
  TH1 * hsMuonEnsembleX =  sEnergyMatchedWIon2.Nominal().ToTH1(pot, kBlue);
  hsMuonEnsembleX->SetMaximum(1.3*hsMuonEnsembleX->GetMaximum());
  hsMuonEnsembleX->Draw("hist");

  // you can draw the individual spectra for each universe 
  for(unsigned int i = 0; i < sEnergyMatchedWIon2.NUniverses(); ++i){
    // using different colors per systematic
    Color_t color = kBlue;
    if (i==0) color = kBlue;
    if (i== 1 || i==2) color = kViolet +1;
    // if you draw either of these they should look the same,
    // Univ 1 and 2 of the 2nd ensemble are the same as the ones in sEnergyMatchedWIon2
    sEnergyMatchedWIon2.Universe(i).ToTH1(pot, color,kDashed)->Draw("hist same");     
    sEnsembleMerged2.Universe(i).ToTH1(pot, color+1,kDotted)->Draw("hist same");     
  }  
  gPad->SaveAs("spec_ensemble_test.pdf");
  
  // for the systematics
  new TCanvas;
  for(unsigned int i = 0; i < sEnsembleMerged2.NUniverses(); ++i){
    // using different colors per shift
    Color_t color = kBlue;
    if (i==0) color = kBlue;
    if (i== 1 || i== 2) color = kViolet +1;
    if (i==3 || i==4 ) color = kPink;
    if (i==5 || i==6 ) color = kRed;
    sEnsembleMerged2.Universe(i).ToTH1(pot, color,kDashed)->Draw("hist same");     
  }  
  gPad->SaveAs("ion_rwgt_ensemble_test.png");
  

  // or draw as TGraph 
  new TCanvas;
  // Error band summing in quadrature
  TGraphAsymmErrors* eBand =sEnergyMatchedWIon2.ErrorBand(pot);
  DrawErrorBand(hsMuonEnsembleX, eBand);
  gPad->SaveAs("spec_ensemble_test_ErrorBand.pdf");  


  new TCanvas;
  // Error band summing in quadrature
  TGraphAsymmErrors* eBand2 =sEnsembleMerged2.ErrorBand(pot);
  TH1 * hsMuonEnsembleMerged =  sEnsembleMerged2.Nominal().ToTH1(pot, kBlue);
  DrawErrorBand( hsMuonEnsembleMerged , eBand2);
  gPad->SaveAs("spec_ensemble_test_ion_rwgt_ErrorBand.pdf");  


  // Draw covariance matrix
  Eigen::MatrixXd m= sEnergyMatchedWIon2.CovarianceMatrix(pot);
  std::cout << "The matrix m is of size "  << m.rows() << "x" << m.cols() << std::endl;
  // convert to th2 for drawing         
  TH2F* hCov = EigenMatrixToTH2(m);
  new TCanvas;
  hCov->Draw("colz");
  gPad->SaveAs("cov_mx_ion.pdf");

  // Draw full cov 
  Eigen::MatrixXd m_full= sEnsembleMerged2.CovarianceMatrix(pot);
  TH2F* hCovFull = EigenMatrixToTH2(m_full);
  new TCanvas;
  hCovFull->Draw("colz");
  gPad->SaveAs("cov_mx_ion_rwgt.pdf");


  /// Drawing the spectra directly to check ensemble spec did the correc thing
  /*
  new TCanvas; 
  TH1 * hsMuonEnsembleX2 = sEnergyMatchedTrueMu_nom.ToTH1(pot, kBlue);
  hsMuonEnsembleX2->SetMaximum(1.3*hsMuonEnsembleX2->GetMaximum());
  hsMuonEnsembleX2->Draw("hist");
  sEnergyMatchedTrueMu_up.ToTH1(pot, kPink)->Draw("hist same");;
  sEnergyMatchedTrueMu_down.ToTH1(pot, kViolet+1)->Draw("hist same");;
  gPad->SaveAs("specs_non-ensemble_test.png");
  
  // check differences to verify they do what they should
  new TCanvas; 
  TH1* hdiffNom = sEnergyMatchedTrueMu_nom.ToTH1(pot, kBlue);
  hdiffNom->Divide(  sEnergyMatchedWIon2.Universe(0).ToTH1(pot));
  TH1* hdiffUp =  sEnergyMatchedTrueMu_up.ToTH1(pot, kPink);
  hdiffUp->Divide(   sEnergyMatchedWIon2.Universe(2).ToTH1(pot));
  TH1* hdiffDown =sEnergyMatchedTrueMu_down.ToTH1(pot, kViolet+1);
  hdiffDown->Divide( sEnergyMatchedWIon2.Universe(1).ToTH1(pot));
  hdiffNom->SetMaximum(1.5); hdiffNom->SetMinimum(.5);
  hdiffNom->Draw("hist");
  hdiffUp->Draw("hist same");
  hdiffDown->Draw("hist same");
  gPad->SaveAs("specs_ensembe_diff.png");
  

  new TCanvas;
    // Grab covariance matrix
  Eigen::MatrixXd m2= sMuonEnegyEnsemble2.CovarianceMatrix(pot);
  std::cout << "The matrix m is of size "  << m2.rows() << "x" << m2.cols() << std::endl;
  // convert to th2 for drawing          
  TH2F* hCov2 = EigenMatrixToTH2(m2);  

  new TCanvas;
  hCov2->Draw("colz");
  gPad->SaveAs("cov_mx_rwgt.pdf");

  */
}
