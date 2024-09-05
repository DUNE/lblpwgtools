#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "TFile.h"
//#include "CAFAna/Cuts/TruthCuts.h"
#include "mesonless_histaxis.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TPad.h"

using namespace ana;



void mesonless_main()
{
  // Environment variables and wildcards work. As do SAM datasets.
  // older files
  const std::string fname = "/exp/dune/data/users/noeroy/prod/MiniRun5_1E19_RHC/MiniRun5_1E19_RHC.caf.beta1/CAF.flat/0000000/MiniRun5_1E19_RHC.caf.*";//"/pnfs/dune/scratch/users/mkramer/prod/MiniRun5_1E19_RHC/MiniRun5_1E19_RHC.caf.beta1/CAF.flat/0000000/MiniRun5_1E19_RHC.caf.00001*"; //"/dune/data/users/skumara/Datafiles_2x2/CAF_rootfiles/minirun4/notruth/outputCAF_notruth_*";
  //  "/dune/data/users/skumara/Datafiles_2x2/CAF_rootfiles/Picorun4.2/PicoRun4.2_1E17_RHC.flow*";
//   "/dune/data/users/skumara/Datafiles_2x2/CAF_rootfiles/Picorun4.1/PicoRun4.1_1E17_RHC.flow*";


  // Source of events
  SpectrumLoader loader(fname);


//  ~~~~**** where the magic happens *****~~~~

//// This wont work with a cut because the information of number of tracks
//// is in a different branch than the vtx position information
  Spectrum sTrackMultContained(loader.Interactions(RecoType::kDLP)[kContainment && kShwCut], trackMult);
  Spectrum sShowerMultContained(loader.Interactions(RecoType::kDLP)[kContainment && kShwCut], shwMult);
  Spectrum sAllMultContained(loader.Interactions(RecoType::kDLP)[kContainment && kShwCut], trkshwMult);


  Spectrum sVtxPositionAll(loader.Interactions(RecoType::kDLP)[kNoCut], vtxPosition);
  Spectrum sVtxPositionCont(loader.Interactions(RecoType::kDLP)[kContainment&&kMesonlessSelection], vtxPosition);
  Spectrum sTrueVtxPositionCont(loader.NuTruths()[kTrueContainment&&kTrueMesonlessSelection&&kIsCC&&kIsMuonAntinu&&kIsArgonTarget], TruevtxPosition);
  Spectrum sTrueVtxPositionAll(loader.NuTruths()[kNoTruthCut], TruevtxPosition);
  // we use kNoCut at interactions level, meaning no selection on vertices
  Spectrum sEnergyMuon(loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(RecoType::kDLP)[kIsMuon], axEnergy);
  Spectrum sEnergyMuonContX(loader.Interactions(RecoType::kDLP)[kContainment].RecoParticles(RecoType::kDLP)[kIsMuon], axEnergy);
  Spectrum sEnergyElectron(loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(RecoType::kDLP)[kPartCut(11)], axEnergy);

//


  //Spectrum sTrackLen(loader.Interactions(RecoType::kDLP)[kContainment && kShwCut].RecoParticles(RecoType::kDLP)[kNoPartCut], trkLen);

////// Mesonless analysis plots

  //Candidate = primary protons in selected interactions
  // All
  Spectrum CandidateProtonMomentum(loader.Interactions(RecoType::kDLP)[kMesonlessSelection].RecoParticles(RecoType::kDLP)[kPartCut(2212)],axPartMomentum );
  Spectrum AllProtonMomentum(loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(RecoType::kDLP)[kPartCut(2212)],axPartMomentum );

  Spectrum CandidateProtonMomentumCont(loader.Interactions(RecoType::kDLP)[kMesonlessSelection&&kContainment].RecoParticles(RecoType::kDLP)[kPrimary && kPartCut(2212)],axPartMomentum );
  Spectrum CandidateMuonMomentumCont(loader.Interactions(RecoType::kDLP)[kMesonlessSelection&&kContainment].RecoParticles(RecoType::kDLP)[kPrimary && kIsMuon],axPartMomentum );

  Spectrum TrueCandidateProtonMomentumCont(loader.NuTruths()[kTrueMesonlessSelection&&kTrueContainment&&kIsCC&&kIsMuonAntinu&&kIsArgonTarget].TruthParticles(TruePType::kPrim)[kTruthPartCut(2212)],axTruthPartMomentum );
  Spectrum TrueCandidateMuonMomentumCont(loader.NuTruths()[kTrueMesonlessSelection&&kTrueContainment&&kIsCC&&kIsMuonAntinu&&kIsArgonTarget].TruthParticles(TruePType::kPrim)[kTruthPartCut(13)],axTruthPartMomentum );


  Spectrum AllProtonMomentumCont(loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(RecoType::kDLP)[kPartCut(2212)&&kContainedPart&&kPartLenCut],axPartMomentum );


  Spectrum CandidateProtonAngle(loader.Interactions(RecoType::kDLP)[kMesonlessSelection].RecoParticles(RecoType::kDLP)[kPartCut(2212)],axPartAngle );
  Spectrum AllProtonAngle(loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(RecoType::kDLP)[kPartCut(2212)],axPartAngle );

  Spectrum CandidateProtonAngleCont(loader.Interactions(RecoType::kDLP)[kMesonlessSelection&&kContainment].RecoParticles(RecoType::kDLP)[kPrimary && kPartCut(2212)],axPartAngle);
  Spectrum CandidateMuonAngleCont(loader.Interactions(RecoType::kDLP)[kMesonlessSelection&&kContainment].RecoParticles(RecoType::kDLP)[kPrimary && kIsMuon],axPartAngle);

   Spectrum TrueCandidateProtonAngleCont(loader.NuTruths()[kTrueMesonlessSelection&&kTrueContainment&&kIsCC&&kIsMuonAntinu&&kIsArgonTarget].TruthParticles(TruePType::kPrim)[kTruthPartCut(2212)],axTruthPartAngle );
   Spectrum TrueCandidateMuonAngleCont(loader.NuTruths()[kTrueMesonlessSelection&&kTrueContainment&&kIsCC&&kIsMuonAntinu&&kIsArgonTarget].TruthParticles(TruePType::kPrim)[kTruthPartCut(13)],axTruthPartAngle );

   Spectrum AllProtonAngleCont(loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(RecoType::kDLP)[kPartCut(2212)&&kContainedPart&&kPartLenCut],axPartAngle );
  //
  // Spectrum PrimProtons(loader.Interactions(RecoType::kDLP)[kNoCut], primProtonMult);
  // Spectrum PrimMuons(loader.Interactions(RecoType::kDLP)[kNoCut], primMuonMult);
  // Spectrum PrimPions(loader.Interactions(RecoType::kDLP)[kNoCut], primPionMult);
  // Spectrum PrimPhotons(loader.Interactions(RecoType::kDLP)[kNoCut], primPhotonMult);
  // Spectrum PrimElectrons(loader.Interactions(RecoType::kDLP)[kNoCut], primElectronMult);
//

//  ~~~~**** -.-.-.-.-.-.-.-.-.-.-.- *****~~~~
  // Fill in the spectrum
  loader.Go();

  // We are forcing the pot value because cafs dont have this information yet
  // POT/yr * 3.5yrs * mass correction for the workspace geometry
  const double pot = 1.;//1e19;///3.5 * 1.47e21 * 40/1.13;

  sEnergyMuon.OverridePOT(pot);
  sEnergyMuonContX.OverridePOT(pot);
  sEnergyElectron.OverridePOT(pot);

  sVtxPositionAll.OverridePOT(pot);
  sVtxPositionCont.OverridePOT(pot);
  sTrueVtxPositionAll.OverridePOT(pot);
  sTrueVtxPositionCont.OverridePOT(pot);


  sTrackMultContained.OverridePOT(pot);
  sShowerMultContained.OverridePOT(pot);
  sAllMultContained.OverridePOT(pot);

  //sTrackLen.OverridePOT(pot);
  CandidateProtonMomentum.OverridePOT(pot);
  CandidateProtonMomentumCont.OverridePOT(pot);
  CandidateMuonMomentumCont.OverridePOT(pot);
  TrueCandidateProtonMomentumCont.OverridePOT(pot);
  TrueCandidateMuonMomentumCont.OverridePOT(pot);

  AllProtonMomentum.OverridePOT(pot);
  AllProtonMomentumCont.OverridePOT(pot);

  CandidateProtonAngle.OverridePOT(pot);
  CandidateProtonAngleCont.OverridePOT(pot);
  CandidateMuonAngleCont.OverridePOT(pot);
  TrueCandidateProtonAngleCont.OverridePOT(pot);
  TrueCandidateMuonAngleCont.OverridePOT(pot);

  AllProtonAngle.OverridePOT(pot);
  AllProtonAngleCont.OverridePOT(pot);


  // PrimMuons.OverridePOT(pot);
  // PrimProtons.OverridePOT(pot);
  // PrimPions.OverridePOT(pot);
  // PrimPhotons.OverridePOT(pot);
  // PrimElectrons.OverridePOT(pot);


 // ta da!
   sEnergyMuon.ToTH1(pot)->Draw("hist");
   sEnergyMuonContX.ToTH1(pot,kMagenta+2)->Draw("hist same");
   sEnergyElectron.ToTH1(pot,kGreen+1)->Draw("hist same");

  new TCanvas;
  sVtxPositionAll.ToTH2(pot)->Draw("colz");
  gPad->SaveAs("mesonless_AllVtxPosition.png");
  new TCanvas;
  sVtxPositionCont.ToTH2(pot)->Draw("colz");
  gPad->SaveAs("mesonless_ContainedVtxPosition.png");

  new TCanvas;
  sTrueVtxPositionAll.ToTH2(pot)->Draw("colz");
  gPad->SaveAs("mesonless_TrueAllVtxPosition.png");

  new TCanvas;
  sTrueVtxPositionCont.ToTH2(pot)->Draw("colz");
  gPad->SaveAs("mesonless_TrueContainedVtxPosition.png");

  //
  // new TCanvas;
  // sTrackMultContained.ToTH1(pot,kBlue,kDashed)->Draw("hist same");
  // sAllMultContained.ToTH1(pot,kBlack,kDotted)->Draw("hist same");
  // sShowerMultContained.ToTH1(pot,kRed,kDotted)->Draw("hist same");
  // gPad->SaveAs("mesonless_ContainedMultiplicity.png");
  //
  // new TCanvas;
  // sTrackLen.ToTH1(pot,kRed,kDotted)->Draw("hist same");
  // gPad->SaveAs("mesonless_ContainedTrackLen.png");
//
  new TCanvas;
  //CandidateProtonMomentum.ToTH1(pot)->Draw("hist");
  TrueCandidateProtonMomentumCont.ToTH1(pot,kBlue)->Draw("hist");
  CandidateProtonMomentumCont.ToTH1(pot,kRed)->Draw("hist same");
  gPad->SaveAs("mesonless_mesonless_CandidateProtonMomentum.png");

  new TCanvas;
  //CandidateProtonMomentum.ToTH1(pot)->Draw("hist");
  TrueCandidateMuonMomentumCont.ToTH1(pot,kBlue)->Draw("hist");
  CandidateMuonMomentumCont.ToTH1(pot,kRed)->Draw("hist same");
  gPad->SaveAs("mesonless_mesonless_CandidateMuonMomentum.png");

  // new TCanvas;
  // //AllProtonMomentum.ToTH1(pot)->Draw("hist");
  // AllProtonMomentumCont.ToTH1(pot,kRed)->Draw("hist same");
  // gPad->SaveAs("mesonless_All_CandidateProtonMomentum.png");

  new TCanvas;
  //CandidateProtonAngle.ToTH1(pot)->Draw("hist");
  TrueCandidateProtonAngleCont.ToTH1(pot,kBlue)->Draw("hist");
  CandidateProtonAngleCont.ToTH1(pot,kRed)->Draw("hist same");
  gPad->SaveAs("mesonless_mesonless_CandidateProtonAngle.png");

  new TCanvas;
  TrueCandidateMuonAngleCont.ToTH1(pot,kBlue)->Draw("hist");
  CandidateMuonAngleCont.ToTH1(pot,kRed)->Draw("hist same");
  gPad->SaveAs("mesonless_mesonless_CandidateMuonAngle.png");

  // new TCanvas;
  // AllProtonAngle.ToTH1(pot)->Draw("hist");
  // AllProtonAngleCont.ToTH1(pot,kRed)->Draw("hist same");
  // gPad->SaveAs("mesonless_All_CandidateProtonAngle.png");
  //
  // new TCanvas;
  // PrimMuons.ToTH1(pot, kRed)->Draw("hist same");
  // PrimProtons.ToTH1(pot, kBlue)->Draw("hist same");
  // PrimPions.ToTH1(pot, kMagenta)->Draw("hist same");
  // PrimPhotons.ToTH1(pot, kGreen)->Draw("hist same");
  // PrimElectrons.ToTH1(pot, kYellow)->Draw("hist same");
  // save to file
 // TFile file( "mesonless_output.root", "recreate" );
 // file.cd();
 // CandidateMuonMomentumCont.SaveTo( &file, "CandidateMuonMomentumCont");
 // CandidateProtonMomentumCont.SaveTo( &file, "CandidateProtonMomentumCont");
 // CandidateMuonAngleCont.SaveTo( &file, "CandidateMuonAngleCont");
 // CandidateProtonAngleCont.SaveTo( &file, "CandidateProtonAngleCont");
 // file.Close();
  std::cout<< "No. of reco interactions "<< sVtxPositionCont.ToTH2(pot)->Integral(1, 60, 1, 60) <<std::endl;

  std::cout<< "No. of true interactions "<< sTrueVtxPositionCont.ToTH2(pot)->Integral(1, 60, 1, 60) <<std::endl;
  
  std::cout<< "No. of reco protons "<< CandidateProtonMomentumCont.ToTH1(pot)->Integral() <<std::endl;

  std::cout<< "No. of reco muons "<< CandidateMuonMomentumCont.ToTH1(pot)->Integral() <<std::endl;
  
  std::cout<< "No. of true protons "<< TrueCandidateProtonMomentumCont.ToTH1(pot)->Integral() <<std::endl;

  std::cout<< "No. of true muons "<< TrueCandidateMuonMomentumCont.ToTH1(pot)->Integral() <<std::endl;

}
