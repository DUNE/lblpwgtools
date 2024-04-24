#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/HistAxis.h"
#include "TVector3.h"

namespace ana{

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
 const TruthCut kTruthContainedVertex([](const caf::SRTrueInteractionProxy* nu)
                      {
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
        if (part->E_method != caf::PartEMethod::kCalorimetry) len = RecoPartDir(part).Mag();        
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
 const TruthPartCut kTruePartPDGcut(int PDG){
    const TruthPartCut kIsPDG([PDG](const caf::SRTrueParticleProxy* part)
                      {
                        return abs(part->pdg) == PDG ;
                      });
    return kIsPDG;
  }
// same but for truth particles
  // A  generic selection on the pdg of the particle 
 const RecoPartCut kPartCut(int PDG){
    const RecoPartCut kIsPDG([PDG](const caf::SRRecoParticleProxy* part)
                      {
                        return abs(part->pdg) == PDG ;
                      });
    return kIsPDG;
  }

// angle of particle with respect to beam
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
// entries from SRNDLarIntProxy and SRInteractionProxy
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


}
