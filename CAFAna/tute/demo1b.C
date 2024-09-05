
#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "demo1b.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TPad.h"
#include "TFile.h"
#include "TStyle.h"
#include "TLorentzVector.h"
#include "TLegend.h"
using namespace ana;

// see demo1b.h for variables, cuts

void demo1b( std::string option="caf")
{

  // you can comment this if you dont want to see stats box
  gStyle->SetOptStat("emi");
  // Environment variables and wildcards work. As do SAM datasets.
  std::string fname;
  if(option=="caf") fname = "/exp/dune/data/users/noeroy/prod/MiniRun5_1E19_RHC/MiniRun5_1E19_RHC.caf.beta1/CAF/0000000/*.root";
  // "/pnfs/dune/persistent/users/mkramer/productions/MiniRun4.5_1E19_RHC/CAF_beta3/CAF/0000000/MiniRun4.5_1E19_RHC.caf.0000";
  if (option=="flat") fname = "/exp/dune/data/users/noeroy/prod/MiniRun5_1E19_RHC/MiniRun5_1E19_RHC.caf.beta1/CAF.flat/0000000/MiniRun5_1E19_RHC.caf.0000*root";
    //"/pnfs/dune/persistent/users/mkramer/productions/MiniRun4.5_1E19_RHC/CAF_beta3/CAF.flat/0000000/MiniRun4.5_1E19_RHC.caf.0000*";

  // Source of events
  SpectrumLoader loader(fname);

  // We will also use two different Vars. For such simple ones we can use this
  // Variables at the level of Particles
  const RecoPartVar kRecoParticleEnergy = SIMPLEPARTVAR(E);
  const RecoPartVar kRecoParticlePDG = SIMPLEPARTVAR(pdg);

  // Define axes for the spectra we'll make
  const RecoPartHistAxis axEnergy("Muon energy (GeV)", Binning::Simple(50, 0, 1), kRecoParticleEnergy);
  const RecoPartHistAxis axPDG("PDG of reco particle", Binning::Simple(14, 0, 14), kRecoParticlePDG);


  const TruthPartVar kTruePartPDG([](const caf::SRTrueParticleProxy* part){return part->pdg;});


// Select particles that have a pdg of muon 
  const RecoPartCut kIsMuon([](const caf::SRRecoParticleProxy* sr)
                      {
                        return abs(sr->pdg) == 13 ;
                      });

 //Vertex must have more than one shower and 0 tracks. 
const Cut kShwCut([](const caf::SRInteractionProxy* ixn)
                      {                                              
                           return !(kNumberOfShowers(ixn)==1 &&  kNumberOfTracks(ixn)==0 ) ;
                      });

//  Multiplicity of all particles reconstructed from selected vertices
  // tracks 
  // showers
  const HistAxis trackMult("Number of particles", Binning::Simple(20,0,20), kNumberOfTracks);
  const HistAxis shwMult("Number of particles", Binning::Simple(20,0,20), kNumberOfShowers);
  const HistAxis trkshwMult("Number of particles", Binning::Simple(20,0,20), kNumberOfTrkShw);

  // - Track length of all reconstructed tracks binned in 1 cm bins.
  const RecoPartHistAxis trkLen("Track length (cm)", Binning::Simple(100,0,100),kPartLen);


//  ~~~~**** where the magic happens *****~~~~
  // Not including cuts at interactions level, meaning no selection on vertices
  Spectrum sEnergyMuon(loader.Interactions(RecoType::kDLP).RecoParticles(RecoType::kDLP)[kIsMuon], axEnergy);
  Spectrum sEnergyMuonContX(loader.Interactions(RecoType::kDLP)[kContainedVertex].RecoParticles(RecoType::kDLP)[kIsMuon], axEnergy);
  Spectrum sEnergyElectron(loader.Interactions(RecoType::kDLP).RecoParticles(RecoType::kDLP)[kPartCut(11)], axEnergy);

  // These are at interaction level. 
  Spectrum sTrackMultContained( loader.Interactions(RecoType::kDLP)[kContainedVertex && kShwCut], trackMult);
  Spectrum sShowerMultContained(loader.Interactions(RecoType::kDLP)[kContainedVertex && kShwCut], shwMult);
  Spectrum sAllMultContained(   loader.Interactions(RecoType::kDLP)[kContainedVertex && kShwCut], trkshwMult);

  Spectrum sTrackLen(loader.Interactions(RecoType::kDLP)[kContainedVertex && kShwCut].RecoParticles(RecoType::kDLP)[kNoPartCut], trkLen);

  // Get something similar  equivalent but with true variables 
  // Spectrum sTrueEnergyMuon(loader
  //  .NuTruths()
  //  .TruthParticles(TruePType::kPrim)[ kTruePartPDGcut(13)||kTruePartPDGcut(-13) ], 
  //   kTruePartEnergy);
  // Spectrum sTrueEnergyMuonContX(loader
  //  .NuTruths()[kTruthContainedVertex]
  //  .TruthParticles(TruePType::kPrim)[ kTruePartPDGcut(13)||kTruePartPDGcut(-13) ],
  //   kTruePartEnergy);

  // primaries and pre-fsi  multiplicity
  Spectrum sTruePrimsMult(loader.NuTruths(), TruthHistAxis("Primaries multiplicity", Binning::Simple(20,0,20), SIMPLETRUTHVAR(nprim) ));
  Spectrum sTruePrimsMultCont(loader.NuTruths()[kTruthContainedVertex],  TruthHistAxis("Primaries multiplicity", Binning::Simple(20,0,20), SIMPLETRUTHVAR(nprim) ));

////// Mesonless analysis -like  plots // see header for variables and cut definitions
  const RecoPartHistAxis axPartMomentum("Momentum (GeV)", Binning::Simple(50, 0.0, 1.0), kPartMomentum);
  const RecoPartHistAxis axPartAngle("angle(deg)",Binning::Simple(60, 0.0, 180),kPartAngle);
  //Candidate = primary protons in selected interactions
  // All 
  Spectrum CandidateProtonMomentum(loader
    .Interactions(RecoType::kDLP)[kMesonlessSelection]
    .RecoParticles(RecoType::kDLP)[kPartCut(2212)],
    axPartMomentum );
  Spectrum AllProtonMomentum(loader
    .Interactions(RecoType::kDLP)[kNoCut]
    .RecoParticles(RecoType::kDLP)[kPartCut(2212)],
    axPartMomentum );

  Spectrum CandidateProtonMomentumCont(loader
    .Interactions(RecoType::kDLP)[kMesonlessSelection]
    .RecoParticles(RecoType::kDLP)[kPartCut(2212) && kContainedPart && kPartLenCut],
    axPartMomentum );
  Spectrum AllProtonMomentumCont( loader
    .Interactions(RecoType::kDLP)[kNoCut]
    .RecoParticles(RecoType::kDLP)[kPartCut(2212)&&kContainedPart&&kPartLenCut],
    axPartMomentum );

  Spectrum CandidateProtonAngle(loader
    .Interactions(RecoType::kDLP)[kMesonlessSelection]
    .RecoParticles(RecoType::kDLP)[kPartCut(2212)],
    axPartAngle );
  Spectrum AllProtonAngle(loader
    .Interactions(RecoType::kDLP)[kNoCut]
    .RecoParticles(RecoType::kDLP)[kPartCut(2212)],
    axPartAngle );

  Spectrum CandidateProtonAngleCont(loader
    .Interactions(RecoType::kDLP)[ kMesonlessSelection]
    .RecoParticles(RecoType::kDLP)[kPartCut(2212) && kContainedPart&&kPartLenCut],
    axPartAngle );
  Spectrum AllProtonAngleCont( loader
    .Interactions(RecoType::kDLP)[kNoCut]
    .RecoParticles(RecoType::kDLP)[ kPartCut(2212) && kContainedPart && kPartLenCut],
    axPartAngle );
  

//  ~~~~**** -.-.-.-.-.-.-.-.-.-.-.- *****~~~~
  // Fill in the spectrum
  loader.Go();

  const double pot = sEnergyMuon.POT();

  //ta da!
  sEnergyMuon.ToTH1(pot)->Draw("hist");
  sEnergyMuonContX.ToTH1(pot,kMagenta+2)->Draw("hist same");
  sEnergyElectron.ToTH1(pot,kGreen+1)->Draw("hist same");
  gPad->SaveAs(("demo1b_sEnergyMuon"+option+".pdf").c_str());
 
  new TCanvas;
  sTrackMultContained.ToTH1(pot,kBlue,kDashed)->Draw("hist same");
  sAllMultContained.ToTH1(pot,kRed,kDotted)->Draw("hist same");
  // sShowerMultContained.ToTH1(pot,kRed,kDotted)->Draw("hist same");  
  gPad->SaveAs(("demo1b_ContainedMultiplicity"+option+".pdf").c_str());
  new TCanvas;
  sTruePrimsMult.ToTH1(pot,kAzure+2,kSolid)->Draw("hist same");
  sTruePrimsMultCont.ToTH1(pot,kPink,kSolid)->Draw("hist same");
  gPad->SaveAs(("demo1b_ContainedMultiplicity_wTruth"+option+".pdf").c_str());

  new TCanvas;
  sTrackLen.ToTH1(pot,kRed,kDotted)->Draw("hist same");
  gPad->SaveAs(("demo1b_ContainedTrackLen"+option+".pdf").c_str());


  ////// Mesonless analysis -like  plots
  new TCanvas;
  CandidateProtonMomentum.ToTH1(pot)->Draw("hist");
  CandidateProtonMomentumCont.ToTH1(pot,kRed)->Draw("hist same");
  gPad->SaveAs(("demo1b_mesonless_CandidateProtonMomentum"+option+".pdf").c_str());

  new TCanvas;
  AllProtonMomentum.ToTH1(pot)->Draw("hist");
  AllProtonMomentumCont.ToTH1(pot,kRed)->Draw("hist same");
  gPad->SaveAs(("demo1b_All_CandidateProtonMomentum"+option+".pdf").c_str());

  new TCanvas;
  CandidateProtonAngle.ToTH1(pot)->Draw("hist");
  CandidateProtonAngleCont.ToTH1(pot,kRed)->Draw("hist same");
  gPad->SaveAs(("demo1b_mesonless_CandidateProtonAngle"+option+".pdf").c_str());

  new TCanvas;
  AllProtonAngle.ToTH1(pot)->Draw("hist");
  AllProtonAngleCont.ToTH1(pot,kRed)->Draw("hist same");
  gPad->SaveAs(("demo1b_All_CandidateProtonAngle"+option+".pdf").c_str());

  // save to file
  TFile file( "demo1b_output.root", "recreate" );
  file.cd();  
  sEnergyMuon.SaveTo( &file, "sEnergyMuon");
  sEnergyMuonContX.SaveTo( &file, "sEnergyMuonContX");
  file.Close();

  // Open file and plot again
  // you could do this in a separate macro

   TFile *inFile =TFile::Open("demo1b_output.root");
   Spectrum temp = *Spectrum::LoadFrom(inFile,"sEnergyMuon").release();
   Spectrum temp2 = *Spectrum::LoadFrom(inFile,"sEnergyMuonContX").release();
   inFile->Close();
  // make your plot pretty
   TH1* htemp = temp.ToTH1(pot,kMagenta);
   TH1* htemp2 = temp2.ToTH1(pot, kAzure+2, kDashed);  
   htemp->GetXaxis()->CenterTitle();
   new TCanvas;
   htemp->Draw("hist");
   htemp2->Draw("hist same");
   auto *leg = new TLegend(.55,.4,.9,.8);
   leg->AddEntry(htemp,"All reco #mu","l");
   leg->AddEntry(htemp2,"#mu with contained vtx","l");
   leg->Draw();
  // we rewrite the older plot without legends
   gPad->SaveAs(("demo1b_sEnergyMuon"+option+".pdf").c_str());


}
