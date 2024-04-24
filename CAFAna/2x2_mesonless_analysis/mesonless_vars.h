#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "CAFAna/Core/Var.h"
#include "TVector3.h"

namespace ana{

       const auto beam_dir = TVector3(0.0, -0.05836, 1.0);


       TVector3 RecoPartDir(const caf::SRRecoParticleProxy* part){
         if(part->pdg == 22 || part->pdg == 11){ //end points of showers are -inf
       	   TVector3 ret = TVector3(-11111., -11111., -11111.);
       	   return ret;
         }
         else{
       	   TVector3 ret = TVector3(part->end.x,  part->end.y,  part->end.z) -
                        TVector3(part->start.x,part->start.y,part->start.z);
         	 return ret;
         }
       }


       const  RecoPartVar kPartLen([](const caf::SRRecoParticleProxy* part)
        {
                return RecoPartDir(part).Mag();
        });
       const RecoPartVar kPartAngle([](const caf::SRRecoParticleProxy* part)
         {
                   //auto dir = TVector3(part->end.x,  part->end.y,  part->end.z) -
                   //           TVector3(part->start.x,part->start.y,part->start.z);
             //      auto angle = RecoPartDir(part).Angle(beam_dir) * 180.0 / TMath::Pi();
                   auto angle =  TVector3(part->p.x, part->p.y, part->p.z).Angle(beam_dir) * 180.0 / TMath::Pi();
                   return angle;
         });
         //  momentum
       const RecoPartVar kPartMomentum([](const caf::SRRecoParticleProxy* part)
         {
                   float p = std::sqrt((part->p.x * part->p.x) + (part->p.y * part->p.y) + (part->p.z * part->p.z));
                   return p;
         });


       // Using E_method to count tracks/showers, because there's currently no implementation to combine
       // entreis from SRNDLarIntProxy and SRInteractionProxy
       const Var kNumberOfTracks([](const caf::SRInteractionProxy* ixn)
         {
             int num_tracks=0;

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
             int num_shw=0;
             for (int j=0; j<ixn->part.ndlp; j++){
               if ( ixn->part.dlp[j].E_method  == caf::PartEMethod::kCalorimetry)
                 num_shw++;

             }
             return num_shw;
         });

      // const Var kPrimPart(int PDG)
      // {
      //   int num_prim=0;
      //   const Var kNumberOfPrimParticles([PDG](const caf::SRInteractionProxy* ixn)
      //    {
      //        for (int j=0; j<ixn->part.ndlp; j++){
      //          if ( ixn->part.dlp[j].primary && abs(ixn->part.dlp[j].pdg) == PDG)
      //            num_prim++;
      //
      //        }
      //    });
      //     return num_prim;
      //  }


       const Var kNumberOfTrkShw([](const caf::SRInteractionProxy* ixn)
         {
             return ixn->part.ndlp;
         });


         // We will also use two different Vars. For such simple ones we can use this
         // Variables at the level of Particles
      const RecoPartVar kRecoParticleEnergy = SIMPLEPARTVAR(E);
      const RecoPartVar kRecoParticlePDG = SIMPLEPARTVAR(pdg);


      const TruthPartVar kTruthPartMomentum([](const caf::SRTrueParticleProxy* part)
      {
        float p = std::sqrt( (part->p.px * part->p.px) + (part->p.py * part->p.py) + (part->p.pz * part->p.pz));
        return p;
      });
      
      TVector3 TruthPartDir(const caf::SRTrueParticleProxy* part)
      {
        if(part->pdg == 22 || part->pdg == 11){ //end points of showers are -inf
          TVector3 ret = TVector3(-11111., -11111., -11111.);
          return ret;
        }
        else{
          TVector3 ret = TVector3(part->end_pos.x, part->end_pos.y,  part->end_pos.z) -
                       TVector3(part->start_pos.x, part->start_pos.y, part->start_pos.z);
          return ret;
        }
      }
      
      const TruthPartVar kTruthPartAngle([](const caf::SRTrueParticleProxy* part)
        {
                 // auto angle = PartDir(part).Angle(beam_dir) * 180.0 / TMath::Pi();
                  auto angle =  TVector3(part->p.px,part->p.py,part->p.pz).Angle(beam_dir) * 180.0 / TMath::Pi();
                  return angle;
        });









}
