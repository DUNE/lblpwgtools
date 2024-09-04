
#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "duneanaobj/StandardRecord/Navigate.h"
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



// Modify the record of reconstructed muon particles that match with a true muon
// scale by say 5% at 1 sigma.
 class EnergyScaleTrueMu: public ISyst {
 public:
   EnergyScaleTrueMu() : ISyst("EnergyScaleTrueMu", "Muon Energy Scale Syst") {}
   void Shift(double sigma,
              caf::SRInteractionProxy* ixn, double& weight) const override;
 };
  void EnergyScaleTrueMu::Shift(double sigma,
                            caf::SRInteractionProxy* ixn, double& weight) const {
   double scale = .05 * sigma;

   // loop through particles and change record to shift energy
   // only if there is a matched true muon
    for (int i=0; i<ixn->part.ndlp; i++){
      if (! ixn->part.dlp[0].truth.empty()){ 
             // get top level ancestpr of this particle
             const caf::SRProxy * top = ixn->part.Ancestor<caf::SRProxy>();
             if ( abs(ixn->part.dlp[i].pdg)==13 &&   // check if identified as a muon
                  // we scale only if the top match has the same pdg as reco particle
                  (caf::FindParticle(top->mc, ixn->part.dlp[0].truth[0]) ->pdg == ixn->part.dlp[i].pdg) ) 
              {         
                ixn->part.dlp[i].E *= 1. + scale;
              }
            }
    }
 }
 
 const EnergyScaleTrueMu kEnergyScaleTrueMu;


//// lets think about a MEC scaling systematic...
/////// Lets try a systematic that can affect both true and reco:: i.e. MEC flat scale
//class MECScaleSyst : public ISyst//<caf::SRTrueInteractionProxy>
//{
//public: 
//  MECScaleSyst(double scale, const std::string shortName, const std::string latexName)
//    : ISyst(shortName, latexName), fScale(scale) {}
//
//    void Shift( double sigma, caf::SRInteractionProxy* ixn, double& weight) const override;
//
//  private:
//  double fScale;
//};
//void MECScaleSyst::Shift(double sigma, caf::SRInteractionProxy* ixn, double& weight) const
//{
//  
//  const caf::SRProxy * top = ixn->Ancestor<caf::SRProxy>();
//  if ((caf::FindInteraction(top->mc, ixn->truth[0]) )->mode == ScatteringMode::kMEC)
//    weight = 1 + sigma * fScale;
//  else 
//    weight=1;
//}
//const MECScaleSyst kMECScaleSyst(0.10, "MECScaleSyst", "MEC scale systematic");

// Script to make a basic ensemble spectrum 
// Based on file systematics
// So fill up a nominal
// Fill Plus and minus 1 sigma versions
// This is a cross systematic type I guess
// Instantiate a dummy systematic to construct Multiverse
// Get a wrapper that merges various spectra into a simgle ensemble spectrum thing...

void demo_systs()
{
 

  std::vector<const ISyst*> systs;     
  systs.push_back(&kWIonSyst);
  std::vector<const ISyst*> systs2;
  systs2.push_back(&kEnergyScaleTrueMu);

  // scans from -n to n sigma shifts, we'll do only the 1 sigma
  const Multiverse& cross = Multiverse::Hypercross(systs,1);
  const Multiverse& cross_rwgt = Multiverse::Hypercross(systs2,1);

  // we can save our multiverses to file
  TFile fout("test_multiverse.root", "RECREATE");
  cross.SaveTo(&fout, "cross");
  cross_rwgt.SaveTo(&fout, "cross_rwgt");
  fout.Close();
  
  // and then retrieve multiverses from file
  TFile fin("test_multiverse.root");
  const FitMultiverse& cross2 = *Multiverse::LoadFrom(&fin, "cross");
  const FitMultiverse& cross_rwgt2 = *Multiverse::LoadFrom(&fin, "cross_rwgt");


  // you can print the hypercross universe to see what is going on
    std::cout << cross.LatexName() << ",   "<< cross.ShortName() << std::endl;
  for (int i = 0; i< cross.NUniv(); i++){
      for (const auto & [ key, value ] : cross.GetUniverse(i)) {
      std::cout << key->ShortName() << ": " << value << std::endl;
      } 
  }
    std::cout << cross_rwgt.LatexName() << ",   "<< cross_rwgt.ShortName() << std::endl;
  for (int i = 0; i< cross_rwgt.NUniv(); i++){
      for (const auto & [ key, value ] : cross_rwgt.GetUniverse(i)) {
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

 // const std::string fname_nom = "/exp/dune/data/users/noeroy/mywork/MiniRun5_Fixed_truth/MiniRun5_1E19_RHC.caf/CAF.flat/0000000/*.flat.root";
  const std::string fname_nom = "/exp/dune/data/users/noeroy/prod/MiniRun5_1E19_RHC/MiniRun5_1E19_RHC.caf.beta1/CAF.flat/0000000/MiniRun5_1E19_RHC.caf.*";
  const std::string fname_up =  "/pnfs/dune/persistent/users/cuddandr/2x2_production/MiniRun5_Systematics_W_ION_22.7/MiniRun5_Systematics_W_ION_22.7.caf/CAF.flat/0000000/MiniRun5_Systematics_W_ION_22.7.caf.*";
  const std::string fname_down = "/pnfs/dune/persistent/users/cuddandr/2x2_production/MiniRun5_Systematics_W_ION_25.1/MiniRun5_Systematics_W_ION_25.1.caf/CAF.flat/0000000/MiniRun5_Systematics_W_ION_25.1.caf.*";

  // Source of events
  SpectrumLoader loader_nom(fname_nom);
  SpectrumLoader loader_up(fname_up);
  SpectrumLoader loader_down(fname_down);

  // Fill various spectra with these...
  Spectrum sEnergyMatchedTrueMu_nom(loader_nom.Interactions(RecoType::kDLP)[kHas1RecoMuon],    axMuEnergy);
  Spectrum sEnergyMatchedTrueMu_up(loader_up.Interactions(RecoType::kDLP)[kHas1RecoMuon],    axMuEnergy);
  Spectrum sEnergyMatchedTrueMu_down(loader_down.Interactions(RecoType::kDLP)[kHas1RecoMuon],    axMuEnergy);

  // Another way to access the same information from the reco particles branch 
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

  // get from file
  const FitMultiverse& cross22 = *Multiverse::LoadFrom(fIn, "cross");
 
  EnsembleSpectrum sEnergyMatchedWIon = EnsembleSpectrum::MergeSpectra(specs, &cross22);//, axMuEnergy );

//----------------------------------------------------------------------
  // These lines dont crash but I dont know if they are working properly
  const FitMultiverse& crossCombined = Multiverse::Hypercross({&kWIonSyst, &kEnergyScaleTrueMu},1);
  EnsembleSpectrum sEnergyMatchedIonandEnergyScale = EnsembleSpectrum::MergeEnsembles( 
                                                      {sEnergyMatchedWIon,sMuonEnegyEnsemble}, &crossCombined);
//----------------------------------------------------------------------

  TFile fout3("multiverse_ensemble_spec_2.root", "RECREATE");
  sEnergyMatchedWIon.SaveTo(&fout3, "sEnergyMatchedWIon");
  sMuonEnegyEnsemble.SaveTo( &fout3, "sMuonEnegyEnsemble");
  sEnergyMatchedIonandEnergyScale.SaveTo( &fout3, "sEnergyMatchedIonandEnergyScale");
  fout3.Close();

  // Read spectra from file
  TFile fin2("multiverse_ensemble_spec_2.root");
  EnsembleSpectrum sEnergyMatchedWIon2  =*EnsembleSpectrum::LoadFrom(&fin2, "sEnergyMatchedWIon");
  EnsembleSpectrum sEnergyMatchedIonandEnergyScale2  =*EnsembleSpectrum::LoadFrom(&fin2, "sEnergyMatchedIonandEnergyScale");

 // -- draw various histograms
  double pot = sEnergyMatchedTrueMu_nom.POT();
  new TCanvas;
  TH1 * hsMuonEnsembleX =  sEnergyMatchedWIon2.Nominal().ToTH1(pot, kBlue);
  hsMuonEnsembleX->SetMaximum(1.3*hsMuonEnsembleX->GetMaximum());
  hsMuonEnsembleX->Draw("hist");

  // you can draw the individual spectra for each universe 
  // in this case our spectra from different files
  std::cout<< "cross has "<<sEnergyMatchedWIon2.NUniverses()<< " universes\n";
  for(unsigned int i = 0; i < sEnergyMatchedWIon2.NUniverses(); ++i){
    // using different colors per systematic
    Color_t color = kBlue;
    if (i==0) color = kBlue;
    if (i== 1 || i==2) color = kViolet +1;
    // if you draw either of these they should look the same,
    // Univ 1 and 2 of the 2nd ensemble are the same as the ones in sEnergyMatchedWIon2
    sEnergyMatchedWIon2.Universe(i).ToTH1(pot, color,kDashed)->Draw("hist same");     
    //sEnergyMatchedIonandEnergyScale2.Universe(i).ToTH1(pot, color+1,kDotted)->Draw("hist same");     
  }  
  gPad->SaveAs("spec_ensemble_test.pdf");
  
  
  // or draw as TGraph 
  new TCanvas;
  // Error band summing in quadrature
  TGraphAsymmErrors* eBand =sEnergyMatchedWIon2.ErrorBand(pot);
  DrawErrorBand(hsMuonEnsembleX, eBand);
  gPad->SaveAs("spec_ensemble_test_ErrorBand.pdf");  

  // Draw covariance matrix
  new TCanvas;
  Eigen::MatrixXd m= sEnergyMatchedWIon2.CovarianceMatrix(pot);
  std::cout << "The matrix m is of size "  << m.rows() << "x" << m.cols() << std::endl;
  // convert to th2 for drawing         
  TH2F* hCov = EigenMatrixToTH2(m);
  new TCanvas;
  hCov->Draw("colz");
  gPad->SaveAs("cov_mx_ion.pdf");


  /// Drawing the spectra directly 
  new TCanvas; 
  TH1 * hsMuonEnsembleX2 = sEnergyMatchedTrueMu_nom.ToTH1(pot, kBlue);
  hsMuonEnsembleX2->SetMaximum(1.3*hsMuonEnsembleX2->GetMaximum());
  hsMuonEnsembleX2->Draw("hist");
  sEnergyMatchedTrueMu_up.ToTH1(pot, kPink)->Draw("hist same");;
  sEnergyMatchedTrueMu_down.ToTH1(pot, kViolet+1)->Draw("hist same");;
  gPad->SaveAs("specs_non-ensemble_test.png");
  
  // check differences
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
 EnsembleSpectrum sMuonEnegyEnsemble2  =*EnsembleSpectrum::LoadFrom(&fin2, "sMuonEnegyEnsemble");
  for(unsigned int i = 0; i < sMuonEnegyEnsemble2.NUniverses(); ++i){
    // using different colors per shift
    Color_t color = kBlue;
    if (i==0) color = kBlue;
    if (i== 1 ) color = kViolet +1;
    if (i==2 ) color = kPink;
    sMuonEnegyEnsemble2.Universe(i).ToTH1(pot, color,kDashed)->Draw("hist same");     
  }  
  gPad->SaveAs("rwgt_ensemble_test.png");
  
  new TCanvas;
    // Grab covariance matrix
  Eigen::MatrixXd m2= sMuonEnegyEnsemble2.CovarianceMatrix(pot);
  std::cout << "The matrix m is of size "  << m2.rows() << "x" << m2.cols() << std::endl;
  // convert to th2 for drawing          
  TH2F* hCov2 = EigenMatrixToTH2(m2);  

  new TCanvas;
  hCov2->Draw("colz");
  gPad->SaveAs("cov_mx_rwgt.pdf");


}
