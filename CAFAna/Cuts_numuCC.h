#ifndef MY_CUTS_H
#define MY_CUTS_H

#include "CAFAna/Core/Var.h"
#include "CAFAna/Core/HistAxis.h"
#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/Cut.h"
#include "CAFAna/Core/Ratio.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/TruthMatching.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"


// minimum and maximum values in the CAFs to define ND-LAr AV
#define NDLArXLo -346.9
#define NDLArXHi  346.9
#define NDLArYLo -215.5
#define NDLArYHi   81.7
#define NDLArZLo  418.2
#define NDLArZHi  913.3


namespace selcuts {
    
    // VERTEX
    // reco vertices
    const Var kVtxX = SIMPLEVAR(vtx.x);
    const Var kVtxY = SIMPLEVAR(vtx.y);
    const Var kVtxZ = SIMPLEVAR(vtx.z);
    // true vertices
    const TruthVar kTrueVtxX = SIMPLETRUTHVAR(vtx.x);
    const TruthVar kTrueVtxY = SIMPLETRUTHVAR(vtx.y);
    const TruthVar kTrueVtxZ = SIMPLETRUTHVAR(vtx.z);
    // truth matched vertices
    const Var kMatchedTrueVtxX([](const caf::SRInteractionProxy *ixn) {
        if(ixn->truth.empty()) return -1000.0;
        const caf::SRProxy *sr = ixn->Ancestor<caf::SRProxy>();
        size_t tidx = 0;
        float maxOverlap = 0;
        for(size_t i = 0; i < ixn->truthOverlap.size(); ++i) {
            if(ixn->truthOverlap[i] > maxOverlap) {
                tidx = i;
                maxOverlap = ixn->truthOverlap[i];
            }
        }
        if(maxOverlap < 0.0) return -1000.0;
        const caf::SRTrueInteractionProxy *tixn = caf::FindInteraction(sr->mc, ixn->truth[tidx]);
        return kTrueVtxX(tixn);
    });
    const Var kMatchedTrueVtxY([](const caf::SRInteractionProxy *ixn) {
        if(ixn->truth.empty()) return -1000.0;
        const caf::SRProxy *sr = ixn->Ancestor<caf::SRProxy>();
        size_t tidx = 0;
        float maxOverlap = 0;
        for(size_t i = 0; i < ixn->truthOverlap.size(); ++i) {
            if(ixn->truthOverlap[i] > maxOverlap) {
                tidx = i;
                maxOverlap = ixn->truthOverlap[i];
            }
        }
        if(maxOverlap < 0.0) return -1000.0;
        const caf::SRTrueInteractionProxy *tixn = caf::FindInteraction(sr->mc, ixn->truth[tidx]);
        return kTrueVtxY(tixn);
    });
    const Var kMatchedTrueVtxZ([](const caf::SRInteractionProxy *ixn) {
        if(ixn->truth.empty()) return -1000.0;
        const caf::SRProxy *sr = ixn->Ancestor<caf::SRProxy>();
        size_t tidx = 0;
        float maxOverlap = 0;
        for(size_t i = 0; i < ixn->truthOverlap.size(); ++i) {
            if(ixn->truthOverlap[i] > maxOverlap) {
                tidx = i;
                maxOverlap = ixn->truthOverlap[i];
            }
        }
        if(maxOverlap < 0.0) return -1000.0;
        const caf::SRTrueInteractionProxy *tixn = caf::FindInteraction(sr->mc, ixn->truth[tidx]);
        return kTrueVtxZ(tixn);
    });

    // START AND END POSITION
    //Truth
    const TruthPartVar kTruePartStartX = SIMPLETRUTHPARTVAR(start_pos.x);
    const TruthPartVar kTruePartStartY = SIMPLETRUTHPARTVAR(start_pos.y);
    const TruthPartVar kTruePartStartZ = SIMPLETRUTHPARTVAR(start_pos.z);
    const TruthPartVar kTruePartEndX = SIMPLETRUTHPARTVAR(end_pos.x);
    const TruthPartVar kTruePartEndY = SIMPLETRUTHPARTVAR(end_pos.y);
    const TruthPartVar kTruePartEndZ = SIMPLETRUTHPARTVAR(end_pos.z);

    // GENERAL
    const RecoPartVar kPartEndX = SIMPLEPARTVAR(end.x);
    const RecoPartVar kPartEndY = SIMPLEPARTVAR(end.y);
    const RecoPartVar kPartEndZ = SIMPLEPARTVAR(end.z);
    const RecoPartCut kIsPrimary = SIMPLEPARTVAR(primary) == 1;
    const RecoPartVar kPartPDG = SIMPLEPARTVAR(pdg);
    const RecoPartCut kIsMuon = kPartPDG == 13;
    const RecoPartCut kIsPrimaryMuon = kIsMuon && kIsPrimary;
    const RecoPartCut kPartEscapesToTMS = kPartEndX < NDLArXHi - 25 && kPartEndX > NDLArXLo + 25
                                        && kPartEndY < NDLArYHi - 25 && kPartEndY > NDLArYLo + 25
                                        && kPartEndZ > NDLArZLo + 25;
    const RecoPartCut kIsPrimaryMuonEscaping = kIsPrimaryMuon && kPartEscapesToTMS;

    // CONTAINMENT
    // for now use is_contained, variable from SPINE
    // Looks like it is defined as vtx and all enegy contained
    // But don't know which volume it is defined with, maybe ND-LAr active volume?
    const RecoPartCut kPartNDLArContained = SIMPLEPARTVAR(contained) == 1;
    const RecoPartCut kPartNDLArContainedExceptMuonDownstream = kPartNDLArContained || kIsPrimaryMuonEscaping;
    // all non-primary particles from interaction contianed
    const Cut kAllPartContained([](const caf::SRInteractionProxy *ixn){
        for(const auto &p: ixn->part.dlp){
            if((!kIsPrimary && !kPartNDLArContained)(&p)){
                return false;
            }
        }
        return true;
    });
    // vertex contained in ND-LAr active volume
    const Cut kVtxContained = kVtxX > NDLArXLo && kVtxX < NDLArXHi
                            && kVtxY > NDLArYLo && kVtxY < NDLArYHi
                            && kVtxZ > NDLArZLo && kVtxZ < NDLArZHi;
    // full event contained
    const Cut kEventContained([](const caf::SRInteractionProxy * ixn){
        if (!kVtxContained(ixn)) return false;
        for(const auto &p: ixn->part.dlp){
            if ((!kPartNDLArContainedExceptMuonDownstream)(&p)){
                return false;
            }
        }
        return true;
    });
    // true containement
    const TruthPartCut kTruePartContained = kTruePartEndX < NDLArXHi - 5 && kTruePartEndX > NDLArXLo + 5
                                        && kTruePartEndY < NDLArYHi - 5 && kTruePartEndY > NDLArYLo + 5
                                        && kTruePartEndZ < NDLArZHi - 5 && kTruePartEndZ > NDLArZLo + 5;
    const TruthPartCut kTruePartContainedButCanEscapeToTMS = kTruePartEndX < NDLArXHi - 25 && kTruePartEndX > NDLArXLo + 25
                                                            && kTruePartEndY < NDLArYHi - 25 && kTruePartEndY > NDLArYLo + 25
                                                            && kTruePartEndZ > NDLArZLo + 25;
    const TruthCut kAllTrueContained([](const caf::SRTrueInteractionProxy *truth){
    for(const auto &p: truth->prim)
        if(!kTruePartContained(&p)) return false;
    return true;
    });
    const TruthPartCut kTrueMuon = SIMPLETRUTHPARTVAR(pdg) == 13;
    const TruthPartCut kTrueMuonEscapesToTMS = kTruePartEndX < NDLArXHi - 25 && kTruePartEndX > NDLArXLo + 25
                                            && kTruePartEndY < NDLArYHi - 25 && kTruePartEndY > NDLArYLo + 25
                                            && kTruePartEndZ > NDLArZLo + 25;
    const TruthPartCut kTrueMuonEscaping = kTrueMuon && kTrueMuonEscapesToTMS;
    const TruthPartCut kTrueContainedExceptMuonDownstream = kTruePartContained || kTrueMuonEscaping;
    const TruthCut kAllTrueContainedExceptMuonDownstream([](const caf::SRTrueInteractionProxy *truth){
    for(const auto &p: truth->prim){
        if(!kTrueContainedExceptMuonDownstream(&p)) return false;
    }
    return true;
    });

    const TruthCut kAllTrueContainedNDLArAndEscapeToTMS([](const caf::SRTrueInteractionProxy *truth){
    for(const auto &p: truth->prim)
        if(!kTruePartContainedButCanEscapeToTMS(&p)) return false;
    return true;
    });

    // DIFFERENT VOLUMES
    // start with original FV cut (i.e. 25 cm form wall edge) and then vary Z axis
    const Cut kVtxInFV = kVtxX > NDLArXLo + 25 && kVtxX < NDLArXHi - 25
                        && kVtxY > NDLArYLo + 25 && kVtxY < NDLArYHi - 25
                        && kVtxZ > NDLArZLo + 25 && kVtxZ < NDLArZHi - 25;
    const TruthCut kTrueVtxInFV = kTrueVtxX > NDLArXLo + 25 && kTrueVtxX < NDLArXHi - 25
                                && kTrueVtxY > NDLArYLo + 25 && kTrueVtxY < NDLArYHi - 25
                                && kTrueVtxZ > NDLArZLo + 25 && kTrueVtxZ < NDLArZHi - 25;

    // qo AND q3 VARIABLES
    // In truth for now 
    const TruthVar kq0([](const caf::SRTrueInteractionProxy * sr){
        return sr->q0;
    });
    const TruthVar kq3([](const caf::SRTrueInteractionProxy * sr){
        return sr->modq;
    });
    const Var kTruthMatchedQ0([](const caf::SRInteractionProxy * ixn){
        if (ixn->truth.empty()) return -10000.0;
        const caf::SRProxy *sr = ixn->Ancestor<caf::SRProxy>();
        size_t tidx = 0;
        float maxOverlap = 0;
        for(size_t i = 0; i < ixn->truthOverlap.size(); ++i) {
            if(ixn->truthOverlap[i] > maxOverlap) {
                tidx = i;
                maxOverlap = ixn->truthOverlap[i];
            }
        }
        const caf::SRTrueInteractionProxy *tixn = caf::FindInteraction(sr->mc, ixn->truth[tidx]);
        return kq0(tixn);
    });
    const Var kTruthMatchedQ3([](const caf::SRInteractionProxy * ixn){
        if (ixn->truth.empty()) return -10000.0;
        const caf::SRProxy *sr = ixn->Ancestor<caf::SRProxy>();
        size_t tidx = 0;
        float maxOverlap = 0;
        for(size_t i = 0; i < ixn->truthOverlap.size(); ++i) {
            if(ixn->truthOverlap[i] > maxOverlap) {
                tidx = i;
                maxOverlap = ixn->truthOverlap[i];
            }
        }
        const caf::SRTrueInteractionProxy *tixn = caf::FindInteraction(sr->mc, ixn->truth[tidx]);
        return kq3(tixn);
    });

    // QUALITY CUT
    const RecoPartVar kPartLen([](const caf::SRRecoParticleProxy * p){
        return std::hypot(p->end.x - p->start.x, p->end.y - p->start.y, p->end.z - p->start.z);
    });
    const Cut kPartLenInInteractionCut_LongestTrack([](const caf::SRInteractionProxy * ixn){
        if (ixn->part.dlp.empty()) return false;
        size_t longestIdx = 1e6;
        double longest = -1;
        for (size_t i = 0; i < ixn->part.dlp.size(); ++i){
            const auto &p = ixn->part.dlp[i];
            if (kIsPrimary(&p) && kPartLen(&p) > longest){
                longestIdx = i;
                longest = kPartLen(&p);
            }
        }
        if (longestIdx > ixn->part.dlp.size()) return false;
        return longest > 0.1;
    });

    // TRUE NUMU CC EVENT
    const TruthCut kTrueNumuCC = SIMPLETRUTHVAR(iscc) == 1 && (SIMPLETRUTHVAR(pdg) == 14 || SIMPLETRUTHVAR(pdg) == -14);
    // contained true numu CC
    const TruthCut kTrueNumuCCContained = kTrueNumuCC && kTrueVtxInFV && kAllTrueContained;
    const TruthCut kTrueNumuCCContainedNDLArAndEscapeToTMS = kTrueNumuCC && kTrueVtxInFV && kAllTrueContainedExceptMuonDownstream;

    // TRUTH MATCHED NUMU CC EVENT
    // const Cut kNuMuTrueSignalMatch([](const caf::SRInteractionProxy *ixn) {
    //   if(ixn->truth.empty()) return false;
    //   const caf::SRProxy *sr = ixn->Ancestor<caf::SRProxy>();
    //   size_t tidx = 0;
    //   float maxOverlap = 0;
    //   for(size_t i = 0; i < ixn->truthOverlap.size(); ++i) {
    //     if(ixn->truthOverlap[i] > maxOverlap) {
    //       tidx = i; // id of event with highest truth-overlap so far
    //       maxOverlap = ixn->truthOverlap[i];
    //     }
    //   }
    //   const caf::SRTrueInteractionProxy *tixn = caf::FindInteraction(sr->mc, ixn->truth[tidx]);
    //   return kTrueNumuCCContainedNDLArAndEscapeToTMS(tixn);
    // });
    Cut MakeNuMuTrueSignalMatch(const TruthCut& trueFV){
        return Cut([trueFV](const caf::SRInteractionProxy *ixn) -> bool {
            if(ixn->truth.empty()) return false;
            const caf::SRProxy *sr = ixn->Ancestor<caf::SRProxy>();
            size_t tidx = 0;
            float maxOverlap = 0;
            for(size_t i = 0; i < ixn->truthOverlap.size(); ++i) {
                if(ixn->truthOverlap[i] > maxOverlap) {
                    tidx = i;
                    maxOverlap = ixn->truthOverlap[i];
                }
            }
            const caf::SRTrueInteractionProxy *tixn = caf::FindInteraction(sr->mc, ixn->truth[tidx]);
            TruthCut signal = kTrueNumuCC && trueFV && kAllTrueContainedExceptMuonDownstream;
            return signal(tixn);
        });
    }

    Cut MakeNuMuTrueMatchNoContainment(const TruthCut& trueFV){
        return Cut([trueFV](const caf::SRInteractionProxy *ixn) -> bool {
            if(ixn->truth.empty()) return false;
            const caf::SRProxy *sr = ixn->Ancestor<caf::SRProxy>();
            size_t tidx = 0;
            float maxOverlap = 0;
            for(size_t i = 0; i < ixn->truthOverlap.size(); ++i) {
                if(ixn->truthOverlap[i] > maxOverlap) {
                    tidx = i;
                    maxOverlap = ixn->truthOverlap[i];
                }
            }
            const caf::SRTrueInteractionProxy *tixn = caf::FindInteraction(sr->mc, ixn->truth[tidx]);

            return (kTrueNumuCC && trueFV)(tixn);
        });
    }

    Cut OutsideFVMatch(const TruthCut& trueFV){
        return Cut([trueFV](const caf::SRInteractionProxy *ixn){
            if(ixn->truth.empty()) return false;
            const caf::SRProxy *sr = ixn->Ancestor<caf::SRProxy>();
            size_t tidx = 0;
            float maxOverlap = 0;
            for(size_t i = 0; i < ixn->truthOverlap.size(); ++i){
                if(ixn->truthOverlap[i] > maxOverlap){
                    tidx = i;
                    maxOverlap = ixn->truthOverlap[i];
                }
            }
            const caf::SRTrueInteractionProxy *tixn = caf::FindInteraction(sr->mc, ixn->truth[tidx]);

            return kTrueNumuCC(tixn) && !trueFV(tixn);
        });
    }

    TruthCut TrueOutsideFV_Z(const TruthCut& fullFV){
        return TruthCut([fullFV](const caf::SRTrueInteractionProxy* t){
            if(fullFV(t)) return false;

            // only fail Z
            bool failZ = !(kTrueVtxZ(t) > NDLArZLo + 25 && kTrueVtxZ(t) < NDLArZHi - 25);

            return failZ;
        });
    }
    Cut OutsideFVMatch_Z(const TruthCut& fullFV){
        TruthCut zCut = TrueOutsideFV_Z(fullFV);
        return Cut([zCut](const caf::SRInteractionProxy *ixn){
            if(ixn->truth.empty()) return false;

            const caf::SRProxy *sr = ixn->Ancestor<caf::SRProxy>();

            size_t tidx = 0;
            float maxOverlap = 0;
            for(size_t i = 0; i < ixn->truthOverlap.size(); ++i){
                if(ixn->truthOverlap[i] > maxOverlap){
                    tidx = i;
                    maxOverlap = ixn->truthOverlap[i];
                }
            }

            const caf::SRTrueInteractionProxy *tixn = caf::FindInteraction(sr->mc, ixn->truth[tidx]);
            return (kTrueNumuCC && zCut)(tixn);
        });
    }

    TruthCut TrueOutsideFV_Z_upstream(const TruthCut& fullFV){
        return TruthCut([fullFV](const caf::SRTrueInteractionProxy* t){
            if(fullFV(t)) return false;
            return kTrueVtxZ(t) < NDLArZLo + 25;
        });
    }
    Cut OutsideFVMatch_Z_upstream(const TruthCut& fullFV){
        TruthCut zCut = TrueOutsideFV_Z_upstream(fullFV);
        return Cut([zCut](const caf::SRInteractionProxy *ixn){
            if(ixn->truth.empty()) return false;
            const caf::SRProxy *sr = ixn->Ancestor<caf::SRProxy>();
            size_t tidx = 0;
            float maxOverlap = 0;
            for(size_t i = 0; i < ixn->truthOverlap.size(); ++i){
                if(ixn->truthOverlap[i] > maxOverlap){
                    tidx = i;
                    maxOverlap = ixn->truthOverlap[i];
                }
            }
            const caf::SRTrueInteractionProxy *tixn = caf::FindInteraction(sr->mc, ixn->truth[tidx]);
            return (kTrueNumuCC && zCut)(tixn);
        });
    }

    TruthCut TrueOutsideFV_Z_downstream(const TruthCut& fullFV){
        return TruthCut([fullFV](const caf::SRTrueInteractionProxy* t){
            if(fullFV(t)) return false;
            return kTrueVtxZ(t) > NDLArZHi - 25;
        });
    }
    Cut OutsideFVMatch_Z_downstream(const TruthCut& fullFV){
        TruthCut zCut = TrueOutsideFV_Z_downstream(fullFV);
        return Cut([zCut](const caf::SRInteractionProxy *ixn){
            if(ixn->truth.empty()) return false;
            const caf::SRProxy *sr = ixn->Ancestor<caf::SRProxy>();
            size_t tidx = 0;
            float maxOverlap = 0;
            for(size_t i = 0; i < ixn->truthOverlap.size(); ++i){
                if(ixn->truthOverlap[i] > maxOverlap){
                    tidx = i;
                    maxOverlap = ixn->truthOverlap[i];
                }
            }
            const caf::SRTrueInteractionProxy *tixn = caf::FindInteraction(sr->mc, ixn->truth[tidx]);
            return (kTrueNumuCC && zCut)(tixn);
        });
    }

    TruthCut TrueOutsideFV_XY(const TruthCut& fullFV){
        return TruthCut([fullFV](const caf::SRTrueInteractionProxy* t){
            if(fullFV(t)) return false;

            bool inZ = (kTrueVtxZ(t) > NDLArZLo + 25 && kTrueVtxZ(t) < NDLArZHi - 25);

            bool failX = !(kTrueVtxX(t) > NDLArXLo + 25 && kTrueVtxX(t) < NDLArXHi - 25);
            bool failY = !(kTrueVtxY(t) > NDLArYLo + 25 && kTrueVtxY(t) < NDLArYHi - 25);

            return inZ && (failX || failY);
        });
    }
    Cut OutsideFVMatch_XY(const TruthCut& fullFV){
        TruthCut xyCut = TrueOutsideFV_XY(fullFV);
        return Cut([xyCut](const caf::SRInteractionProxy *ixn){
            if(ixn->truth.empty()) return false;

            const caf::SRProxy *sr = ixn->Ancestor<caf::SRProxy>();

            size_t tidx = 0;
            float maxOverlap = 0;
            for(size_t i = 0; i < ixn->truthOverlap.size(); ++i){
                if(ixn->truthOverlap[i] > maxOverlap){
                    tidx = i;
                    maxOverlap = ixn->truthOverlap[i];
                }
            }

            const caf::SRTrueInteractionProxy *tixn = caf::FindInteraction(sr->mc, ixn->truth[tidx]);
            return (kTrueNumuCC && xyCut)(tixn);
        });
    }

    // RECO NUMU CC EVENT
    const Cut kRecoNumuCC([](const caf::SRInteractionProxy * ixn){
        if (ixn->part.dlp.empty()) return false;
        for (size_t i = 0; i < ixn->part.dlp.size(); i++){
            const auto &p = ixn->part.dlp[i];
            if (kIsPrimary(&p) && kIsMuon(&p)){
                return true;
            }
        }
        return false;
    });

    // TRUTH OVERLAP
    const Cut OverlapCut(double overlapThreshold){
    const Cut kOverlapCut([overlapThreshold](const caf::SRInteractionProxy * sr)
    {
        if (sr->truthOverlap.empty()) return false;
        return *std::max_element(sr->truthOverlap.begin(), sr->truthOverlap.end()) > overlapThreshold;
    });
    return kOverlapCut;
    }

    const Cut kIsBestMatchForTruth([](const caf::SRInteractionProxy* ixn)
    {
        if(ixn->truth.empty()) return false;
        const caf::SRProxy* sr = ixn->Ancestor<caf::SRProxy>();
        size_t tidx = 0;
        float myOverlap = -1.0;
        for(size_t i = 0; i < ixn->truthOverlap.size(); ++i){
            if(ixn->truthOverlap[i] > myOverlap){
                tidx = i;
                myOverlap = ixn->truthOverlap[i];
            }
        }
        int myTruthID = ixn->truth[tidx];
        for(const auto& other : sr->common.ixn.dlp){
            if(&other == ixn) continue;
            if(other.truth.empty()) continue;
            size_t oidx = 0;
            float otherOverlap = -1.0;
            for(size_t j = 0; j < other.truthOverlap.size(); ++j){
                if(other.truthOverlap[j] > otherOverlap){
                    otherOverlap = other.truthOverlap[j];
                    oidx = j;
                }
            }
            if(other.truth[oidx] == myTruthID){
                if(otherOverlap > myOverlap)
                    return false;
            }
        }
        return true;
    });

}

#endif