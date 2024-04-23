
#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TPad.h"

using namespace ana;

// Make a few basic spectra from the cafs
// Reconstructed 
//    - One with Interaction Level vars/cuts
//    - One with particle-level vars/cuts
// Truth 
//   - True interaction Level Vars/Cuts
//   - True particles Level Vars/Cuts


void demo0b()
{
 
  const std::string fname ="/exp/dune/data/users/noeroy/prod/MiniRun5_1E19_RHC/MiniRun5_1E19_RHC.caf.beta1/CAF/0000000/*.root";
  //() still figuring out the problem with flatcafs)
  //"/exp/dune/data/users/noeroy/prod/MiniRun5_1E19_RHC/MiniRun5_1E19_RHC.caf.beta1/CAF.flat/0000000/MiniRun5_1E19_RHC*";

  // Source of events
  SpectrumLoader loader(fname);

  /////////////////////////////////////////////////////////////////
  // Reco-Particles (SRRecoParticleProxy)  level vars and cuts
  const RecoPartVar kRecoParticleEnergy = SIMPLEPARTVAR(E);
  // Define axes for the spectra we'll make
  const RecoPartHistAxis axEnergy("Muon energy (GeV)", Binning::Simple(50, 0, 1), kRecoParticleEnergy);
  // Select particles that have a pdg of muon, 
  const RecoPartCut kIsMuon([](const caf::SRRecoParticleProxy* sr)
                      {
                        return abs(sr->pdg) == 13 ;
                      });

  /////////////////////////////////////////////////////////////////
  // Interaction level (SRInteractionProxy)  vars 
  const Var kVtxX = SIMPLEVAR(vtx.x);
  const Var kVtxY = SIMPLEVAR(vtx.y);
  //  we can also write vars with additional logic/operations

  const Var kVtxZ([](const caf::SRInteractionProxy* sr)
                      {                        
                        double z = sr-> vtx.z;//-1300;
                        // you could do more operations here
                        return z;
                      });

  // Histaxis with interaction level variables
  // Note you can setup a 2D axis 
  const HistAxis vtxPosition( "x(cm)", Binning::Simple(70,-70,70), kVtxX, 
                              "z(cm)", Binning::Simple(70,-70,70), kVtxZ);
  const HistAxis vtxPosition2("y(cm)", Binning::Simple(70,-70,70), kVtxY, 
                              "z(cm)", Binning::Simple(70,-70,70), kVtxZ);
  // A simple selection cut at the level of vertices: i.e. containment
  const Cut kContainedVertex([](const caf::SRInteractionProxy* sr)
                      {

                        double x = sr->vtx.x;
                        double y = sr->vtx.y;
                        double z = sr->vtx.z;
                        bool cont =  abs(x)<50 && 
                                     abs(x)>10 && 
                                     abs(y)<50 &&
                                     abs(z)>10 && 
                                     abs(z)<50 ;
                        return cont;
                      });


  ///////////////////////////////////////////////////////////////// 
  //  ~~~~**** now actually make some histograms  *****~~~~
  // we use kNoCut at interactions level, meaning no selection on vertices, then with containment
  // You can select RecoType::kDLP,kPandora at vtx/interaction level and kPIDA as well for particles
  Spectrum sEnergyMuon(     loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(      RecoType::kDLP)[kIsMuon], axEnergy);
  Spectrum sEnergyMuonCont( loader.Interactions(RecoType::kDLP)[kContainedVertex].RecoParticles(RecoType::kDLP)[kIsMuon], axEnergy);
  // Interaction level vars
  Spectrum sVtxPositionAll( loader.Interactions(RecoType::kDLP)[kNoCut], vtxPosition);
  Spectrum sVtxPositionCont(loader.Interactions(RecoType::kDLP)[kContainedVertex], vtxPosition);

  // You need the histaxis to match the Cut level (i.e: the next one will not work becausePosition is an SRInteraction entry
  // but your spectrumLoader points to entries from SRRecoParticle
  // Spectrum sVtxPositionCont(loader.Interactions(RecoType::kDLP)[kContainedVertex].RecoParticles(RecoType::kDLP)[kIsMuon], vtxPosition);
  
  // try plottingwhatever to see what ge get from the indices in SRInteraction.truth
  Spectrum sVtxPositionAll2( loader.Interactions(RecoType::kDLP)[kNoCut], vtxPosition2);
  Spectrum sVtxPositionAllCont2( loader.Interactions(RecoType::kDLP)[kContainedVertex], vtxPosition2);


 const TruthVar kTrueId([](const caf::SRTrueInteractionProxy* nu){return nu->id;});

 const TruthCut kMyCut ([](const caf::SRTrueInteractionProxy* nu){
                        double x = nu->vtx.x;
                        double y = nu->vtx.y;
                        double z = nu->vtx.z;
                        bool cont =  abs(x)<50 && 
                                     abs(x)>10 && 
                                     abs(y)<50 &&
                                     abs(z)>10 && 
                                     abs(z)<50 ;
                                         return cont;
    });

 // This can be used for prefsi, primary or secondary particles. See spectrum constructor later.
 const TruthPartVar kTruePartPDG([](const caf::SRTrueParticleProxy* part){ 
    int pdg = part->pdg;
    // recast in a different way 
    //(1) muon
    if ( abs(pdg)==13) return 1;
    //(2) electron
    if ( abs(pdg)==11) return 2;
    //(3) proton
    if ( pdg==2212) return 3;
    // (4) neutron
    if ( pdg==2112) return 4;
    // (5) pi +
    if ( pdg==211) return 5;
    // (6) pi -
    if ( pdg==-211) return 6;
    // (7) pi0
    if ( pdg==111) return 7;
    else
      return 8;
    // (8) anything else
  });
 std::vector<std::string> PDGlabels = {"#mu", "e","p","n","#pi^{+}","#pi^{-}","#pi^{0}","other"};

  const TruthHistAxis myHistTruthAxis("true id", Binning::Simple(100, 0, 100), kTrueId);
  const TruthHistAxis myHistTruthAxisPDG("true pdg", Binning::Simple(100, 0, 100), SIMPLETRUTHVAR(pdg));
  const TruthPartHistAxis myHistTruthPartAxisPDG("true particles pdg", Binning::Simple(8, 0, 8), kTruePartPDG);

  const HistAxis ixnId( "id", Binning::Simple(20,-1,19), SIMPLEVAR(id));
  
  Spectrum SixnId(loader.Interactions(RecoType::kDLP)[kNoCut], ixnId);
  Spectrum SixnIdTrue(loader.NuTruths()[kNoTruthCut], myHistTruthAxis);
  Spectrum PDGTrue(loader.NuTruths()[kNoTruthCut], myHistTruthAxisPDG);
  
  Spectrum PDGTruePrims(loader.NuTruths().TruthParticles(TruePType::kPrim)[kNoTruthPartCut], myHistTruthPartAxisPDG);
  Spectrum PDGTrueSecs(loader.NuTruths()[kNoTruthCut].TruthParticles(TruePType::kSec)[kNoTruthPartCut], myHistTruthPartAxisPDG);
  Spectrum PDGTruePreFSI(loader.NuTruths()[kNoTruthCut].TruthParticles(TruePType::kPreFSI)[kNoTruthPartCut], myHistTruthPartAxisPDG);

  // lets try a reco histaxis wuth truth selection ...
   const TruthCut kIsTrueNuMu ([](const caf::SRTrueInteractionProxy* nu){
                        int pdg = nu->pdg;
                         return pdg==14;
    });


  Spectrum sTrueNumuEnergy( loader.NuTruths(), TruthHistAxis("True energy [GeV]", Binning::Simple(20,0,5),SIMPLETRUTHVAR(E)) );
  // double check that the SR cut is actually cutting the right thing 
  Spectrum sTrueNumuPDGtest(loader.NuTruths()[kIsTrueNuMu], myHistTruthAxisPDG);
  // Fill in the spectrum
  loader.Go();

  const double pot = sEnergyMuon.POT();

  //ta da! Draw tour histograms
  sEnergyMuon.ToTH1(pot)->Draw("hist");
  sEnergyMuonCont.ToTH1(pot,kMagenta+2)->Draw("hist same");


  new TCanvas;
  sVtxPositionAll2.ToTH2(pot)->Draw("colz");
  new TCanvas;
  sVtxPositionAllCont2.ToTH2(pot)->Draw("colz");
  new TCanvas;
  SixnId.ToTH1(pot)->Draw("hist");
    std::cout<< " Integral of reco indices "<< SixnId.ToTH1(pot)->Integral() <<std::endl;
  new TCanvas;
  SixnIdTrue.ToTH1(pot,kBlue)->Draw("hist");
  std::cout<< " Integral of true indices "<< SixnIdTrue.ToTH1(pot)->Integral() <<std::endl;

  new TCanvas;
  PDGTrue.ToTH1(pot,kBlue)->Draw("hist");
  sTrueNumuPDGtest.ToTH1(pot,kRed)->Draw("hist same");
 new TCanvas;
  TH1* prims = PDGTruePrims.ToTH1(pot,kRed);
  for (unsigned int i  = 0; i<PDGlabels.size(); i++)  prims->GetXaxis()->SetBinLabel(i+1,PDGlabels[i].c_str());
  prims->Draw("hist");
  PDGTrueSecs.ToTH1(pot,kBlue)->Draw("hist same");
  PDGTruePreFSI.ToTH1(pot,kGreen+2)->Draw("hist same");

  new TCanvas;
  sTrueNumuEnergy.ToTH1(pot)->Draw("hist");

  new TCanvas;
  sVtxPositionAll.ToTH2(pot)->Draw("colz");
  new TCanvas; 
  sVtxPositionCont.ToTH2(pot)->Draw("colz");

}
