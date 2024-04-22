#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "CAFAna/Core/Cut.h"
#include "mesonless_vars.h"
#include "TVector3.h"

namespace ana{

    double tpc_dist = 8.0; //distance from the tpc walls for containtment cuts
    //Reco interaction cuts
    const Cut kMesonlessSelection([](const caf::SRInteractionProxy* sr)
      {
              int num_muons = 0;
              int num_prtns = 0;
              int num_pions = 0;
              int num_kaons = 0;
              // loop over particles and count each type
              for (const auto & part : sr->part.dlp ){
                  if(!part.primary)
                      continue;
                  if(abs(part.pdg) == 13)
                      num_muons++;
                  if(abs(part.pdg) == 211)
                      num_pions++;
                  if(abs(part.pdg) == 321)
                      num_kaons++;
                  if(abs(part.pdg) == 2212)
                      num_prtns++;

              }
            return ( num_muons==1 && num_pions<1 && num_kaons<1 && num_prtns>0 );
      });

    // A simple selection cut at the level of vertices: i.e. containment
  const Cut kContainment([](const caf::SRInteractionProxy* ixn)
                          {

                            double x = ixn->vtx.x;
                            double y = ixn->vtx.y;
                            double z = ixn->vtx.z;
                            bool cont =  abs(x)<60-tpc_dist &&
                                         abs(x)>tpc_dist &&
                                         abs(y)<60-tpc_dist &&
                                         abs(z)>tpc_dist &&
                                         abs(z)<60-tpc_dist ;
                            return cont;
                          });

  const Cut kShwCut([](const caf::SRInteractionProxy* ixn)
  {
            //Vertex must have more than one shower and 0 tracks.
                 return !(kNumberOfShowers(ixn)==1 &&  kNumberOfTracks(ixn)==0 ) ;
  });

//Reco part cuts
  const  RecoPartCut kContainedPart([](const caf::SRRecoParticleProxy* part){return part->contained;});


  const  RecoPartCut kPartLenCut([](const caf::SRRecoParticleProxy* part)
        {
          auto len = RecoPartDir(part).Mag();
          return len>2;
         });

  const RecoPartCut kPrimary([](const caf::SRRecoParticleProxy* part){return part->primary;});

  // A  generic selection on the pdg of the particle
  const RecoPartCut kPartCut(int PDG){
      const RecoPartCut kIsPDG([PDG](const caf::SRRecoParticleProxy* part)
                        {
                          return abs(part->pdg) == PDG ;
                        });
      return kIsPDG;
  }



  // Select particles that have a pdg of muon
  const RecoPartCut kIsMuon([](const caf::SRRecoParticleProxy* sr)
                        {
                          return abs(sr->pdg) == 13 ;
                        });

  //True interaction cuts
  const TruthCut kTrueContainment([](const caf::SRTrueInteractionProxy* mc_ixn)
                  {
                            double x = mc_ixn->vtx.x;
                            double y = mc_ixn->vtx.y;
                            double z = mc_ixn->vtx.z;
                            bool cont =  abs(x)<60-tpc_dist &&
                                         abs(x)>tpc_dist &&
                                         abs(y)<60-tpc_dist &&
                                         abs(z)>tpc_dist &&
                                         abs(z)<60-tpc_dist ;
                            return cont;
                  });
  const TruthCut kIsCC([](const caf::SRTrueInteractionProxy* mc)
                  {
                    return mc->iscc;
                  });
  const TruthCut kIsArgonTarget([](const caf::SRTrueInteractionProxy* mc)
                  {
                    return mc->targetPDG ==   100018040;
                  });
  const TruthCut kIsMuonAntinu([](const caf::SRTrueInteractionProxy* mc)
                  {
                    return mc->pdg ==   -14;
                  });
  // const TruthCut kTrueMesonlessSelection([](const caf::SRTrueInteractionProxy* mc)
  //     {
  //             int num_muons = 0;
  //             int num_prtns = 0;
  //             int num_pions = 0;
  //             int num_kaons = 0;
  //             // loop over particles and count each type
  //             for (const auto & part : sr->prim ){
  //                 if(abs(part.pdg) == 13)
  //                     num_muons++;
  //                 if(abs(part.pdg) == 211)
  //                     num_pions++;
  //                 if(abs(part.pdg) == 321)
  //                     num_kaons++;
  //
  //             }
  //           return ( num_muons==1 && num_pions<1 && num_kaons<1 && num_prtns>0 );
  //     });




  // // A  generic selection on the pdg of the particle
  // const TruePartCut kTruePartCut(int PDG){
  //     const TruePartCut kIsPDG([PDG](const caf::SRTrueParticleProxy* part)
  //                       {
  //                         return abs(part->pdg) == PDG ;
  //                       });
  //     return kIsPDG;
  // }

}
