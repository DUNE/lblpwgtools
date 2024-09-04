
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
#include "TLegend.h"
#include <iostream>
using namespace ana;

// Make a few basic spectra from the cafs
// Reconstructed 
//    - One with Interaction Level vars/cuts
//    - One with particle-level vars/cuts
// Truth 
//   - True interaction Level Vars/Cuts
//   - True particles Level Vars/Cuts


void demo0b( std::string option = "flat")
{
 
  std::string fname;
  if(option=="caf") fname = "/exp/dune/data/users/skumara/Datafiles_2x2/CAF_rootfiles/MR5_beta2a_flash/test_flash.root";

  if (option=="flat")  fname = "/exp/dune/data/users/noeroy/mywork/MiniRun5_Fixed_truth/MiniRun5_1E19_RHC.caf/CAF.flat/0000000/*.flat.root";


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
  // Interaction level (SRInteractionProxy)  Simple vars 
  const Var kVtxX = SIMPLEVAR(vtx.x);
  //  we can also write vars with additional logic/operations
  const Var kVtxZ([](const caf::SRInteractionProxy* sr)
                      {                        
                        double z = sr-> vtx.z;//-1300;
                        // you could do more operations here
                        return z;
                      });

  // Histaxis with interaction level variables
  // Note you can setup a 2D axis, you can directly write simple vars here
  const HistAxis vtxPosition( "x(cm)", Binning::Simple(70,-70,70), kVtxX, 
                              "z(cm)", Binning::Simple(70,-70,70), SIMPLEVAR(vtx.z)); //or kVtxZ

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
  
   ///////////////////////////////////////////////////////////////// 

 const TruthVar kTrueId([](const caf::SRTrueInteractionProxy* nu){return nu->id;});

  // lets try a reco histaxis wuth truth selection ...
  const TruthCut kIsTrueNuMu ([](const caf::SRTrueInteractionProxy* nu){
                        int pdg = nu->pdg;
                         return pdg==14;
    });

 const TruthCut kTruthContainedVertex ([](const caf::SRTrueInteractionProxy* nu){
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
    if ( abs(pdg)==13) return 1.;
    //(2) electron
    if ( abs(pdg)==11) return 2.;
    //(3) proton
    if ( pdg==2212) return 3.;
    // (4) neutron
    if ( pdg==2112) return 4.;
    // (5) pi +
    if ( pdg==211) return 5.;
    // (6) pi -
    if ( pdg==-211) return 6.;
    // (7) pi0
    if ( pdg==111) return 7.;
    // (8) photon
    if (pdg==22) return 8.;
    // (9) Any K0S, K0L K0 or K+/-
    if (abs(pdg)==321 || pdg== 311|| pdg== 310|| pdg== 130) return 9.;
    else{
      //std::cout<<pdg<<std::endl;
      return 10.;
    }
    // (9) anything else
  });
 std::vector<std::string> PDGlabels = {"#mu", "e","p","n","#pi^{+}","#pi^{-}","#pi^{0}","#gamma","K","other"};

  const TruthHistAxis myHistTruthAxisPDG("true pdg", Binning::Simple(100, 0, 100), SIMPLETRUTHVAR(pdg));
  const TruthPartHistAxis myHistTruthPartAxisPDG("true particles pdg", Binning::Simple(10, 1, 11), kTruePartPDG);

  ///////////////////////////////////////////////////////////////// 
  //  ~~~~**** now  make some more histograms  *****~~~~
  // this kind of an ugly plot
  Spectrum PDGTrue(loader.NuTruths(), myHistTruthAxisPDG);
  Spectrum sTrueNumuPDGtest(loader.NuTruths()[kIsTrueNuMu], myHistTruthAxisPDG);
  
  Spectrum PDGTruePrims(loader.NuTruths().TruthParticles(TruePType::kPrim)[kNoTruthPartCut], myHistTruthPartAxisPDG);
  Spectrum PDGTrueSecs(loader.NuTruths()[kNoTruthCut].TruthParticles(TruePType::kSec)[kNoTruthPartCut], myHistTruthPartAxisPDG);
  Spectrum PDGTruePreFSI(loader.NuTruths()[kNoTruthCut].TruthParticles(TruePType::kPreFSI)[kNoTruthPartCut], myHistTruthPartAxisPDG);

  Spectrum sTrueVtxPositionAll( loader.NuTruths(), 
                TruthHistAxis("Truth x(cm)", Binning::Simple(70,-70,70), SIMPLETRUTHVAR(vtx.x), 
                              "Truth z(cm)", Binning::Simple(70,-70,70), SIMPLETRUTHVAR(vtx.z)));
  Spectrum sTrueVtxPositionCont(loader.NuTruths()[kTruthContainedVertex], 
                TruthHistAxis("Truth x(cm)", Binning::Simple(70,-70,70), SIMPLETRUTHVAR(vtx.x), 
                              "Truth z(cm)", Binning::Simple(70,-70,70), SIMPLETRUTHVAR(vtx.z)));

  Spectrum sTrueNumuEnergy( loader.NuTruths(), TruthHistAxis("True neutrino energy [GeV]", Binning::Simple(50,0,10),SIMPLETRUTHVAR(E)) );
  
  ///////////////////////////////////////////////////////////////// 

  // Fill in the spectra
  loader.Go();

  std::cout<< "Spectrum sTrueNumuPDGtest has"<< sTrueNumuPDGtest.ToTH1(sTrueNumuPDGtest.POT())->GetNbinsX() << "bins, and the axis "<< sTrueNumuPDGtest.GetBinnings()[0].NBins()<< " NBins\n";
  std::cout<< "eigen has size: "<< sTrueNumuPDGtest.GetEigen().rows() << "," << sTrueNumuPDGtest.GetEigen().cols()<<std::endl;
  // grab pot from any
  const double pot = sEnergyMuon.POT();

  //ta da! Draw tour histograms
  TH1 * emu = sEnergyMuon.ToTH1(pot); emu->Draw("hist");
  TH1 *emuCont=sEnergyMuonCont.ToTH1(pot,kMagenta+2); emuCont->Draw("hist same");
  auto *lege = new TLegend(.55,.5,.88,.88);    
  lege->AddEntry(emu, "All #mu energy","L");
  lege->AddEntry(emuCont, "Cont. #mu energy","L");
  lege->Draw();
  gPad->SaveAs(("demo0_sEnergyMuon"+option+".pdf").c_str());

  new TCanvas;
  PDGTrue.ToTH1(pot,kBlue)->Draw("hist same");
  sTrueNumuPDGtest.ToTH1(pot,kRed, kDashed)->Draw("hist same");

  new TCanvas;
  // you can also retrieve the TH1 * object and manipulate as usual in root
  TH1* prims = PDGTruePrims.ToTH1(pot,kRed);
  for (unsigned int i  = 0; i<PDGlabels.size(); i++)  
  prims->GetXaxis()->SetBinLabel(i+1,PDGlabels[i].c_str());
  prims->SetTitle("True particles ID");
  prims->Draw("hist");
  TH1* secs = PDGTrueSecs.ToTH1(pot,kBlue); secs->Draw("hist same");
  TH1 *prefsi=  PDGTruePreFSI.ToTH1(pot,kGreen+2); prefsi->Draw("hist same");
  auto *leg = new TLegend(.55,.7,.88,.88);    
  leg->AddEntry(prims, "Primaries","L");
  leg->AddEntry(secs, "Secondaries","L");
  leg->AddEntry(prefsi, "Pre-FSI","L");
  leg->SetFillStyle(0);
  leg->Draw();
  gPad->SaveAs(("demo0_PDGprims_"+option+".pdf").c_str());

  new TCanvas;
  sTrueNumuEnergy.ToTH1(sTrueNumuEnergy.POT())->Draw("hist");

  new TCanvas;
  sVtxPositionAll.ToTH2(pot)->Draw("colz");
  gPad->SaveAs(("demo0_vtxPosAll_"+option+".pdf").c_str());
  new TCanvas; 
  sVtxPositionCont.ToTH2(pot)->Draw("colz");
  gPad->SaveAs(("demo0_vtxPosCont_"+option+".pdf").c_str());


  new TCanvas;
  sTrueVtxPositionAll.ToTH2(sTrueVtxPositionAll.POT())->Draw("colz");
  gPad->SaveAs(("demo0_TruevtxPosAll_"+option+".pdf").c_str());
  new TCanvas;
  sTrueVtxPositionCont.ToTH2(sTrueVtxPositionCont.POT())->Draw("colz");
  gPad->SaveAs(("demo0_TruevtxPosCont_"+option+".pdf").c_str());

}
