// Make a simple spectrum plot
// cafe demo0.C

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"

//#include "CAFAna/Cuts/TruthCuts.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TPad.h"
#include "TVector3.h"

using namespace ana;

const auto beam_dir = TVector3(0.0, -0.05836, 1.0);




// Mesonless analysis cuts 
  // From https://indico.fnal.gov/event/61771/contributions/277573/attachments/172154/
  //Require 1 muon (either +/-), 0 pions, and N protons as primary particles (no cuts on photons or electrons tagged as primaries) 
  //No requirement on the muon entering/exiting MINERvA 
  //No fiducial volume cuts,  No containment cuts
const Cut kMesonlessSelection([](const caf::SRInteractionProxy* sr)
  {
          int num_muons = 0;
          int num_prtns = 0;
          int num_pions = 0;
          int num_phtns = 0;
          int num_elecs = 0;
          // loop over particles and count each type
          for (const auto & part : sr->part.dlp ){
              if(part.primary == false)
                  continue;
              if( abs(part.pdg) == 13)
                  num_muons++;
              if(part.pdg == 211)
                  num_pions++;
              if(part.pdg == 2212)
                  num_prtns++;
              if(part.pdg == 22)
                  num_phtns++;
              if(part.pdg == 11)
                  num_elecs++;
          }
        return ( num_muons==1 && num_pions<1 && num_prtns>0 );
  });

const  RecoPartCut kContainedPart([](const caf::SRRecoParticleProxy* part){return part->contained;});

TVector3 RecoPartDir(const caf::SRRecoParticleProxy* part){

  TVector3 ret = TVector3(part->end.x,  part->end.y,  part->end.z) - 
                 TVector3(part->start.x,part->start.y,part->start.z);
  return ret;
}

const  RecoPartCut kPartLenCut([](const caf::SRRecoParticleProxy* part)
      {
        auto len=-5.;
        // prevent showers to be accounted 
        if (part->E_method  != caf::PartEMethod::kCalorimetry) len = RecoPartDir(part).Mag();        
        return len>2;
       });

const  RecoPartVar kPartLen([](const caf::SRRecoParticleProxy* part)
      {
        auto len=-5.;
        // prevent trying to calculate shw lenght, as they have infinity end-point
        if (part->E_method  != caf::PartEMethod::kCalorimetry) len = RecoPartDir(part).Mag();   
        return len;
       });

// A  generic selection on the pdg of the particle 
 const RecoPartCut kPartCut(int PDG){
    const RecoPartCut kIsPDG([PDG](const caf::SRRecoParticleProxy* part)
                      {
                        return abs(part->pdg) == PDG ;
                      });
    return kIsPDG;
  }
// vars

const RecoPartVar kPartAngle([](const caf::SRRecoParticleProxy* part)
  {
            //auto dir = TVector3(part->end.x,  part->end.y,  part->end.z) - 
            //           TVector3(part->start.x,part->start.y,part->start.z);
            auto angle = RecoPartDir(part).Angle(beam_dir) * 180.0 / TMath::Pi();
            //auto angle =  TVector3(part->p.x,part->p.y,part->p.z).Angle(beam_dir) * 180.0 / TMath::Pi();
            return angle;
  });
  //  momentum
const RecoPartVar kPartMomentum([](const caf::SRRecoParticleProxy* part)
  {
            float p = std::sqrt( (part->p.x * part->p.x) + (part->p.y * part->p.y) + (part->p.z * part->p.z));           
            return p;
  });


// Using E_method to count tracks/showers, because there's currently no implementation to combine
// entreis from SRNDLarIntProxy and SRInteractionProxy
const Var kNumberOfTracks([](const caf::SRInteractionProxy* ixn)
  { 
      double num_tracks=0;

      for (int j=0; j<ixn->part.ndlp; j++){
        if ( ixn->part.dlp[j].E_method  == caf::PartEMethod::kRange ||
             ixn->part.dlp[j].E_method  == caf::PartEMethod::kMCS )
          num_tracks++;
                    
      }              
      return num_tracks;     
  });
//
const Var kNumberOfShowers([](const caf::SRInteractionProxy* ixn)
  { 
      double num_shw=0;
      for (int j=0; j<ixn->part.ndlp; j++){
        if ( ixn->part.dlp[j].E_method  == caf::PartEMethod::kCalorimetry)
          num_shw++;
                    
      }              
      return num_shw;     
  });

const Var kNumberOfTrkShw([](const caf::SRInteractionProxy* ixn)
  { 
      return ixn->part.ndlp; 
  });



void demo1b()
{
  // Environment variables and wildcards work. As do SAM datasets.
  const std::string fname = "/exp/dune/data/users/noeroy/prod/MiniRun5_1E19_RHC/MiniRun5_1E19_RHC.caf.beta1/CAF/0000000/*.root";
  //"/pnfs/dune/persistent/users/mkramer/productions/MiniRun4.5_1E19_RHC/CAF_beta3/CAF/0000000/MiniRun4.5_1E19_RHC.caf.0000*";
  //"/dune/data/users/skumara/Datafiles_2x2/CAF_rootfiles/Picorun4.2/flat/PicoRun4.2_1E17_RHC.flow.0000*";
  /// "/dune/data/users/skumara/Datafiles_2x2/CAF_rootfiles/minirun4/notruth/outputCAF_notruth_*";
  //  "/dune/data/users/skumara/Datafiles_2x2/CAF_rootfiles/Picorun4.2/PicoRun4.2_1E17_RHC.flow*";
//   "/dune/data/users/skumara/Datafiles_2x2/CAF_rootfiles/Picorun4.1/PicoRun4.1_1E17_RHC.flow*";
  

  // Source of events
  SpectrumLoader loader(fname);

  // We will also use two different Vars. For such simple ones we can use this
  // Variables at the level of Particles
  const RecoPartVar kRecoParticleEnergy = SIMPLEPARTVAR(E);
  const RecoPartVar kRecoParticlePDG = SIMPLEPARTVAR(pdg);

  // Define axes for the spectra we'll make
  const RecoPartHistAxis axEnergy("Muon energy (GeV)", Binning::Simple(50, 0, 1), kRecoParticleEnergy);
  const RecoPartHistAxis axPDG("PDG of reco particle", Binning::Simple(14, 0, 14), kRecoParticlePDG);


// Select particles that have a pdg of muon 
  const RecoPartCut kIsMuon([](const caf::SRRecoParticleProxy* sr)
                      {
                        return abs(sr->pdg) == 13 ;
                      });

// A simple selection cut at the level of vertices: i.e. containment
const Cut kContainment([](const caf::SRInteractionProxy* ixn)
                      {
                        double x = ixn->vtx.x;
                        double y = ixn->vtx.y;
                        double z = ixn->vtx.z;
                        bool cont =  abs(x)<50 && 
                                     abs(x)>10 && 
                                     abs(y)<50 &&
                                     abs(z)>10 && 
                                     abs(z)<50 ;
                        return cont;
                      });


const Cut kShwCut([](const caf::SRInteractionProxy* ixn)
                      {
                      //Vertex must have more than one shower and 0 tracks.                          
                           return !(kNumberOfShowers(ixn)==1 &&  kNumberOfTracks(ixn)==0 ) ;
                      });



  //2D histaxis with interaction level variables
  const HistAxis vtxPosition( "x(cm)", Binning::Simple(60,-60,60), SIMPLEVAR(vtx.x), 
                              "z(cm)", Binning::Simple(60,-60,60), SIMPLEVAR(vtx.z));

//  Multiplicity of all particles reconstructed from selected vertices
  // tracks 
  // showers
  const HistAxis trackMult("Number of particles", Binning::Simple(20,0,20), kNumberOfTracks);
  const HistAxis shwMult("Number of particles", Binning::Simple(20,0,20), kNumberOfShowers);
  const HistAxis trkshwMult("Number of particles", Binning::Simple(20,0,20), kNumberOfTrkShw);

  // - Track length of all reconstructed tracks binned in 1 cm bins.
  const RecoPartHistAxis trkLen("Track length (cm)", Binning::Simple(100,0,100),kPartLen);


//  ~~~~**** where the magic happens *****~~~~
  // we use kNoCut at interactions level, meaning no selection on vertices
  Spectrum sEnergyMuon(loader.Interactions(RecoType::kDLP).RecoParticles(RecoType::kDLP)[kIsMuon], axEnergy);
  Spectrum sEnergyMuonContX(loader.Interactions(RecoType::kDLP)[kContainment].RecoParticles(RecoType::kDLP)[kIsMuon], axEnergy);
  Spectrum sEnergyElectron(loader.Interactions(RecoType::kDLP).RecoParticles(RecoType::kDLP)[kPartCut(11)], axEnergy);
 
  Spectrum sVtxPositionAll(loader.Interactions(RecoType::kDLP), vtxPosition);
  Spectrum sVtxPositionCont(loader.Interactions(RecoType::kDLP)[kContainment], vtxPosition);
//
//// This wont work with a cut because the information of number of tracks 
//// is in a different branch than the vtx position information
  Spectrum sTrackMultContained( loader.Interactions(RecoType::kDLP)[kContainment && kShwCut], trackMult);
  Spectrum sShowerMultContained(loader.Interactions(RecoType::kDLP)[kContainment && kShwCut], shwMult);
  Spectrum sAllMultContained(   loader.Interactions(RecoType::kDLP)[kContainment && kShwCut], trkshwMult);

  Spectrum sTrackLen(loader.Interactions(RecoType::kDLP)[kContainment && kShwCut].RecoParticles(RecoType::kDLP)[kNoPartCut], trkLen);

////// Mesonless analysis plots
//  /// Mesonless analysis axes
//  const RecoPartHistAxis axPartMomentum("Momentum (GeV)", Binning::Simple(50, 0.0, 1.0), kPartMomentum);
//  const RecoPartHistAxis axPartAngle("angle(deg)",Binning::Simple(60, 0.0, 180),kPartAngle);
//  //Candidate = primary protons in selected interactions
//  // All 
//  Spectrum CandidateProtonMomentum(loader.Interactions(RecoType::kDLP)[kMesonlessSelection].RecoParticles(RecoType::kDLP)[kPartCut(2212)],axPartMomentum );
//  Spectrum AllProtonMomentum(loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(RecoType::kDLP)[kPartCut(2212)],axPartMomentum );
//
//  Spectrum CandidateProtonMomentumCont(loader.Interactions(RecoType::kDLP)[kMesonlessSelection].RecoParticles(RecoType::kDLP)[kPartCut(2212)&&kContainedPart&&kPartLenCut],axPartMomentum );
//  Spectrum AllProtonMomentumCont(      loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(RecoType::kDLP)[kPartCut(2212)&&kContainedPart&&kPartLenCut],axPartMomentum );
//  
//
//  Spectrum CandidateProtonAngle(loader.Interactions(RecoType::kDLP)[kMesonlessSelection].RecoParticles(RecoType::kDLP)[kPartCut(2212)],axPartAngle );
//  Spectrum AllProtonAngle(loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(RecoType::kDLP)[kPartCut(2212)],axPartAngle );
//
//  Spectrum CandidateProtonAngleCont(loader.Interactions(RecoType::kDLP)[kMesonlessSelection].RecoParticles(RecoType::kDLP)[kPartCut(2212)&&kContainedPart&&kPartLenCut],axPartAngle );
//  Spectrum AllProtonAngleCont(      loader.Interactions(RecoType::kDLP)[kNoCut].RecoParticles(RecoType::kDLP)[kPartCut(2212)&&kContainedPart&&kPartLenCut],axPartAngle );
//  

//  ~~~~**** -.-.-.-.-.-.-.-.-.-.-.- *****~~~~
  // Fill in the spectrum
  loader.Go();

  // We are forcing the pot value because cafs dont have this information yet
  const double pot = 1e17;

  sEnergyMuon.OverridePOT(pot);
  sEnergyMuonContX.OverridePOT(pot);
  sEnergyElectron.OverridePOT(pot);

  sVtxPositionAll.OverridePOT(pot);
  sVtxPositionCont.OverridePOT(pot);

  sTrackMultContained.OverridePOT(pot);
  sShowerMultContained.OverridePOT(pot);
  sAllMultContained.OverridePOT(pot);

  sTrackLen.OverridePOT(pot);
//  CandidateProtonMomentum.OverridePOT(pot);
//  CandidateProtonMomentumCont.OverridePOT(pot);
//  AllProtonMomentum.OverridePOT(pot);
//  AllProtonMomentumCont.OverridePOT(pot);
//
//  CandidateProtonAngle.OverridePOT(pot);
//  CandidateProtonAngleCont.OverridePOT(pot);
//  AllProtonAngle.OverridePOT(pot);
//  AllProtonAngleCont.OverridePOT(pot);
  //ta da!
  sEnergyMuon.ToTH1(pot)->Draw("hist");
  sEnergyMuonContX.ToTH1(pot,kMagenta+2)->Draw("hist same");
  sEnergyElectron.ToTH1(pot,kGreen+1)->Draw("hist same");

  new TCanvas;
  sVtxPositionAll.ToTH2(pot)->Draw("colz");
  gPad->SaveAs("demo1b_AllVtxPosition.pdf");
  new TCanvas; 
  sVtxPositionCont.ToTH2(pot)->Draw("colz");
  gPad->SaveAs("demo1b_ContainedVtxPosition.pdf");
  
  new TCanvas;
  sTrackMultContained.ToTH1(pot,kBlue,kDashed)->Draw("hist same");
   sAllMultContained.ToTH1(pot,kRed,kDotted)->Draw("hist same");
//  sShowerMultContained.ToTH1(pot,kRed,kDotted)->Draw("hist same");  
  gPad->SaveAs("demo1b_ContainedMultiplicity.pdf");

  new TCanvas;
  sTrackLen.ToTH1(pot,kRed,kDotted)->Draw("hist same");
  gPad->SaveAs("demo1b_ContainedTrackLen.pdf");
//
//  new TCanvas;
//  CandidateProtonMomentum.ToTH1(pot)->Draw("hist");
//  CandidateProtonMomentumCont.ToTH1(pot,kRed)->Draw("hist same");
//  gPad->SaveAs("demo1b_mesonless_CandidateProtonMomentum.pdf");
//
//  new TCanvas;
//  AllProtonMomentum.ToTH1(pot)->Draw("hist");
//  AllProtonMomentumCont.ToTH1(pot,kRed)->Draw("hist same");
//  gPad->SaveAs("demo1b_All_CandidateProtonMomentum.pdf");
//
//  new TCanvas;
//  CandidateProtonAngle.ToTH1(pot)->Draw("hist");
//  CandidateProtonAngleCont.ToTH1(pot,kRed)->Draw("hist same");
//  gPad->SaveAs("demo1b_mesonless_CandidateProtonAngle.pdf");
//
//  new TCanvas;
//  AllProtonAngle.ToTH1(pot)->Draw("hist");
//  AllProtonAngleCont.ToTH1(pot,kRed)->Draw("hist same");
//  gPad->SaveAs("demo1b_All_CandidateProtonAngle.pdf");

}
