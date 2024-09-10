#include "CAFAna/Core/Cut.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include "duneanaobj/StandardRecord/SRVector3D.h"
#include "duneanaobj/StandardRecord/SRLorentzVector.h"
#include "duneanaobj/StandardRecord/SREnums.h"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdexcept>

#include "TLorentzVector.h"

/**
*
* All the code needed for the heavy lifting in the ND-GAr macros
* I know this is NOT a long term solution, but it is what it is...
*
* F. Martinez Lopez (f.martinezlopez@qmul.ac.uk)
* August 2024
*
**/

namespace ana
{

    /* -------------------------------------------------------------------------- */
    /*                           Useful numerical values                          */
    /* -------------------------------------------------------------------------- */

    // Centre of the HPgTPC, in the ND hall coordinate system
    const float kHPgTPCX = 0.0;    // cm
    const float kHPgTPCY = -150.0; // cm
    const float kHPgTPCZ = 1486.0; // cm

    // Radius and half lenght of the HPgTPC
    const float kHPgTPCRadius = 277.02; // cm
    const float kHPgTPCLength = 259.00; // cm

    // Standard HPgTPC fiducial volume (FDV) cuts (from the ND CDR)
    const float kHPgTPCFidCutR = 50.0;
    const float kHPgTPCFidCutX = 50.0;

    // Radius and half lenght of the HPgTPC FDV (ND CDR standard)
    const float kHPgTPCFidRadius = kHPgTPCRadius - kHPgTPCFidCutR;
    const float kHPgTPCFidLength = kHPgTPCLength - kHPgTPCFidCutX;

    // Centre positions of left and right HPgTPC faces
    const std::vector<float> kHPgTPCLeftFace  = {kHPgTPCX-kHPgTPCLength, kHPgTPCY, kHPgTPCZ};
    const std::vector<float> kHPgTPCRightFace = {kHPgTPCX+kHPgTPCLength, kHPgTPCY, kHPgTPCZ};

    // Centre positions of left and right HPgTPC FDV faces (ND CDR standard)
    const std::vector<float> kHPgTPCFidLeftFace  = {kHPgTPCX-kHPgTPCFidLength, kHPgTPCY, kHPgTPCZ};
    const std::vector<float> kHPgTPCFidRightFace = {kHPgTPCX+kHPgTPCFidLength, kHPgTPCY, kHPgTPCZ};

    // Masses of the particles
    const float kMassElectron     =   0.511*1e-3; // GeV/c2
    const float kMassMuon         = 105.658*1e-3; // GeV/c2
    const float kMassNeutralPion  = 134.977*1e-3; // GeV/c2
    const float kMassPion         = 139.570*1e-3; // GeV/c2
    const float kMassKaon         = 493.677*1e-3; // GeV/c2
    const float kMassProton       = 938.272*1e-3; // GeV/c2

    /* ----------------------- ALEPH dE/dx parametrisation ---------------------- */

    const float kALEPHp1 = 3.30;
    const float kALEPHp2 = 8.80;
    const float kALEPHp3 = 0.27;
    const float kALEPHp4 = 0.75;
    const float kALEPHp5 = 0.82;

    float fBetaMomentum(const float x, const float m) {
        return (x/m)/std::sqrt(1+(x/m)*(x/m));
    }

    float fGammaMomentum(const float x, const float m) {
        return std::sqrt(1+(x/m)*(x/m));
    }

    float fAlephParamMomentum(const float x, const float m) {
        return kALEPHp1*(kALEPHp2-std::pow(fBetaMomentum(x, m), kALEPHp4)-std::log(kALEPHp3+1/std::pow(fBetaMomentum(x, m)*fGammaMomentum(x, m), kALEPHp5)))/std::pow(fBetaMomentum(x, m), kALEPHp4);
    }

    /* -------------------------------------------------------------------------- */
    /*                             Useful definitions                             */
    /* -------------------------------------------------------------------------- */

    std::vector<float> kBinCenters(std::vector<float> vec) {

        std::vector<float> centres;

        for(std::size_t i = 0; i < vec.size()-1; i++) {
            centres.push_back((vec[i+1] + vec[i])/2);
        }

        return centres;
    }

    template<typename T>
    std::vector<float> linspace(T start_in, T end_in, int num_in) {

        std::vector<float> linspaced;

        float start = static_cast<float>(start_in);
        float end = static_cast<float>(end_in);
        float num = static_cast<float>(num_in);

        if (num == 0) { return linspaced; }
        if (num == 1) 
            {
            linspaced.push_back(start);
            return linspaced;
            }

        float delta = (end - start) / (num - 1);

        for(int i=0; i < num-1; ++i)
            {
            linspaced.push_back(start + delta * i);
            }
        linspaced.push_back(end); // I want to ensure that start and end
                                    // are exactly the same as the input
        return linspaced;
    }

    /**
    * Cross product of two vectors
    * 
    * @param a first vector to multiply, must be non - empty
    * @param b second vector to multiply, must be non - empty
    *
    * @return vector resulting from the cross product axb
    */
    template <typename T>
    std::vector<T> CrossProduct(const std::vector<T> &a, const std::vector<T> &b) {
    std::vector<T> r (a.size());  
    r[0] = a[1]*b[2]-a[2]*b[1];
    r[1] = a[2]*b[0]-a[0]*b[2];
    r[2] = a[0]*b[1]-a[1]*b[0];
    return r;
    }

    /**
    * Checks if a point lies inside a cylinder
    * 
    * @param pt1 central point of cylinder's left face
    * @param pt2 central point of cylinder's left face
    * @param r radius of the cylinder
    * @param q point to check
    * 
    * @return true if the point lies inside the cylinder
    */
    bool fPointInCylinder(const std::vector<float> &pt1, const std::vector<float> &pt2, const float r, const std::vector<float> &q) {

        std::vector<float> vec;
        std::transform(pt2.begin(), pt2.end(), pt1.begin(), std::back_inserter(vec), std::minus<float>());
        float constant = r * std::sqrt(std::inner_product(vec.begin(), vec.end(), vec.begin(), 0.0));

        std::vector<float> diff1;
        std::transform(q.begin(), q.end(), pt1.begin(), std::back_inserter(diff1), std::minus<float>());
        float product1 = std::inner_product(diff1.begin(), diff1.end(), vec.begin(), 0.0);

        std::vector<float> diff2;
        std::transform(q.begin(), q.end(), pt2.begin(), std::back_inserter(diff2), std::minus<float>());
        float product2 = std::inner_product(diff2.begin(), diff2.end(), vec.begin(), 0.0);

        std::vector<float> cross_product = CrossProduct(diff1, vec);
        float norm_cross_product = std::sqrt(std::inner_product(cross_product.begin(), cross_product.end(), cross_product.begin(), 0.0));
        
        return (product1 >= 0) && (product2 <= 0) && (norm_cross_product <= constant);
        
    }

    /**
    * Checks if a point lies within the HPgTPC FDV
    * 
    * @param q point to check
    * @param delta_r radial FDV cut, defaults to CDR standard
    *
    * @return true if the point is inside FDV
    */
    bool fContainedInFiducial(const std::vector<float> &q, const float delta_r = kHPgTPCFidCutR) {
        return fPointInCylinder(kHPgTPCFidLeftFace, kHPgTPCFidRightFace, kHPgTPCRadius - delta_r, q);
    }

    /**
    * Returns a std::vector<float> from a pointer to a caf::SRVector3DProxy
    * 
    * @param vec pointer to the vector proxy
    *
    * @return 
    */
    const std::vector<float> fGetVectorStd(const caf::SRVector3DProxy* vec) {
        const std::vector<float> _vec = {vec->x, vec->y, vec->z};
        return _vec;
    }
    
    /**
    * Returns a caf::SRVector3D from a pointer to a caf::SRVector3DProxy
    * 
    * @param vec pointer to the vector proxy
    *
    * @return 
    */
    const caf::SRVector3D fGetVector3D(const caf::SRVector3DProxy* vec) {
        const caf::SRVector3D _vec(vec->x, vec->y, vec->z);
        return _vec;
    }

    /**
    * Returns a caf::SRLorentzVector from a pointer to a caf::SRLorentzVectorProxy
    * 
    * @param vec pointer to the vector proxy
    *
    * @return 
    */
    const caf::SRLorentzVector fGetVectorLorentz(const caf::SRLorentzVectorProxy* vec) {
        const TLorentzVector _tvec(vec->px, vec->py, vec->pz, vec->E);
        const caf::SRLorentzVector _vec(_tvec);
        return _vec;
    }

    /**
    * Returns a float* from a std::vector<float>
    * 
    * @param vec std::vector<float>
    *
    * @return 
    */
    const float *fArrayFromVectorStd(const std::vector<float> vec) {

        size_t len = vec.size();
        float *ret = new float[len];

        for (size_t i=0; i<len; ++i) {
            ret[i] = vec[i];
        }

        return ret;

    }

    /**
    * Gets the true particle matching a reco particle
    * 
    * @param true_interaction true interaction object
    * @param part reco particle to check
    *
    * @return caf::SRTrueParticleProxy* associated to the particle
    */
    caf::SRTrueParticleProxy* fGetTrueParticleFromRecoParticle(caf::SRTrueInteractionProxy* true_interaction, caf::SRRecoParticleProxy* part) {

        caf::SRTrueParticleProxy* true_part = nullptr;

        if (part->truth.size() == 0)
            return true_part;

        int idx = part->truth[0].part;
        caf::TrueParticleID::PartType type = part->truth[0].type;

        if (type == caf::TrueParticleID::PartType::kPrimary) {
            true_part = &true_interaction->prim[idx];
        } else if (type == caf::TrueParticleID::PartType::kPrimaryBeforeFSI) {
            true_part = &true_interaction->prefsi[idx];
        } else if (type == caf::TrueParticleID::PartType::kSecondary) {
            true_part = &true_interaction->sec[idx];
        } else {
            throw std::domain_error("Unknown PartType: " + std::to_string(type));
        }

        return true_part;

    }

    /**
    * Gets the track associated to a reco particle
    * 
    * @param sr the SR object we are working with
    * @param idx index of reco particle to check
    *
    * @return caf::SRGArTrackProxy* associated to the particle
    */
    caf::SRGArTrackProxy* fGetTrackFromIndex(const caf::SRProxy* sr, size_t idx) {
        return &sr->nd.gar.ixn[0].tracks[idx];
    }

    /**
    * Gets the association associated to a reco particle
    * 
    * @param sr the SR object we are working with
    * @param idx index of reco particle to check
    *
    * @return caf::SRGArAssociationProxy* associated to the particle
    */
    caf::SRGArAssociationProxy* fGetAssociationFromIndex(const caf::SRProxy* sr, size_t idx) {
        return &sr->nd.gar.ixn[0].associations[idx];
    }

    /**
    * Gets vector of muon candidates
    *
    * This is a fairly simple function to get all the reco particles that pass the muon score cut
    * 
    * @param sr the SR object we are working with
    * @param muon_score_cut cut on the muon score
    * @param momentum_method sort vector based on momentum (true) or muon score (false)
    * @param sign_selection select particles with reconstructed positive (+1) or negative (-1) charge or do nothing (0)
    *
    * @return vector of pairs std::pair<size_t, caf::SRRecoParticleProxy*> passing the cut
    */

    const int default_flow_vec[4] = { 1, 1, 1, 1 };

    std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> fGetMuonCandidates(const caf::SRProxy* sr, float const muon_score_cut, bool const momentum_method = true, int const sign_selection = -1, const int *flow_vec = default_flow_vec) {

        std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> candidates;

        // Check that the SR contains 1 reco interaction
        if(sr->common.ixn.ngsft == 0) return candidates;
        assert(sr->common.ixn.ngsft == 1);

        // Make sure the interaction contains some reco particles
        caf::SRInteractionProxy* interaction = &sr->common.ixn.gsft[0];
        if(interaction->part.ngsft == 0) return candidates;

        // Get all particles in the interaction
        size_t nparts = interaction->part.ngsft;
        //std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> parts;
        for (size_t i=0; i<nparts; ++i) {
            candidates.push_back(std::make_pair(i, &interaction->part.gsft[i]));
        }

        // Stop at this cut if indicated
        if (flow_vec[0] == 0) return candidates;

        if (sign_selection != 0) {
            // Try to select only the particles with a given reconstructed charge
            std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> new_candidates;

            std::copy_if(candidates.begin(), candidates.end(), std::back_inserter(new_candidates),
                        [sr, sign_selection](std::pair<size_t, caf::SRRecoParticleProxy*> p) {
                            return fGetAssociationFromIndex(sr, p.first)->charge == sign_selection;
                        }
            );

            candidates = new_candidates;
        }

        // Stop at this cut if indicated
        if (flow_vec[1] == 0) return candidates;

        // Select only the ones that pass the muon score cut (or have a momentum >= 10 GeV/c)
        std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> new_candidates;

        std::copy_if(candidates.begin(), candidates.end(), std::back_inserter(new_candidates),
                    [muon_score_cut](std::pair<size_t, caf::SRRecoParticleProxy*> p) {
                        return (p.second->score.gsft_pid.muon_score >= muon_score_cut) || (fGetVector3D(&(p.second)->p).Mag() >= 10.0);
                    }
        );

        candidates = new_candidates;

        if (momentum_method) {
            // Sort candidates in descending momentum order
            std::sort(candidates.begin(), candidates.end(),
                        [](std::pair<size_t, caf::SRRecoParticleProxy*> a, std::pair<size_t, caf::SRRecoParticleProxy*> b) {
                        return fGetVector3D(&(a.second)->p).Mag() > fGetVector3D(&(b.second)->p).Mag();
                        });
        } else {
            // Alternatively, sort candidates in descending muon score order
            std::sort(candidates.begin(), candidates.end(),
                        [](std::pair<size_t, caf::SRRecoParticleProxy*> a, std::pair<size_t, caf::SRRecoParticleProxy*> b) {
                        return a.second->score.gsft_pid.muon_score > b.second->score.gsft_pid.muon_score;
                        });
        }

        return candidates;

    }

    /**
    * Gets vector of "raw" muon candidates
    *
    * This is a fairly simple function to get all the reco particles
    * 
    * @param sr the SR object we are working with
    * @param momentum_method sort vector based on momentum (true) or muon score (false)
    *
    * @return vector of pairs std::pair<size_t, caf::SRRecoParticleProxy*>
    */
    std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> fGetRawMuonCandidates(const caf::SRProxy* sr, bool const momentum_method = true) {

        std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> candidates;

        // Check that the SR contains 1 reco interaction
        if(sr->common.ixn.ngsft == 0) return candidates;
        assert(sr->common.ixn.ngsft == 1);

        // Make sure the interaction contains some reco particles
        caf::SRInteractionProxy* interaction = &sr->common.ixn.gsft[0];
        if(interaction->part.ngsft == 0) return candidates;

        // Get all particles in the interaction
        size_t nparts = interaction->part.ngsft;
        std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> parts;
        for (size_t i=0; i<nparts; ++i) {
            parts.push_back(std::make_pair(i, &interaction->part.gsft[i]));
        }

        if (momentum_method) {
            // Sort candidates in descending momentum order
            std::sort(parts.begin(), parts.end(),
                        [](std::pair<size_t, caf::SRRecoParticleProxy*> a, std::pair<size_t, caf::SRRecoParticleProxy*> b) {
                        return fGetVector3D(&(a.second)->p).Mag() > fGetVector3D(&(b.second)->p).Mag();
                        });

            return parts;

        } else {

            // Select only the ones with muon score > 0
            std::copy_if(parts.begin(), parts.end(), std::back_inserter(candidates),
                        [](std::pair<size_t, caf::SRRecoParticleProxy*> p) {
                            return p.second->score.gsft_pid.muon_score > 0.0;
                        }
            );

            // Sort candidates in descending muon score order
            std::sort(candidates.begin(), candidates.end(),
                        [](std::pair<size_t, caf::SRRecoParticleProxy*> a, std::pair<size_t, caf::SRRecoParticleProxy*> b) {
                        return a.second->score.gsft_pid.muon_score > b.second->score.gsft_pid.muon_score;
                        });

            return candidates;
        }

    }

    /**
    * Gets the true (primary) particles with a given PDG code
    * 
    * @param sr the SR object we are working with
    * @param pdg code of the particle
    * @param sign whether or not to retrieve positive and negative
    *
    * @return std::vector<caf::SRTrueParticleProxy*>
    */
    std::vector<caf::SRTrueParticleProxy*> fGetTruePDG(const caf::SRProxy* sr, int const pdg, bool const sign = true, float const p_thres = 0.0) {

        std::vector<caf::SRTrueParticleProxy*> particle_vec;

        // Get all true particles in the interaction
        caf::SRTrueInteractionProxy* true_interaction = &sr->mc.nu[0];

        size_t nparts = true_interaction->nprim;
        for (size_t idx=0; idx<nparts; ++idx) {

            caf::SRTrueParticleProxy* true_part = &true_interaction->prim[idx];
            
            int tpdg = true_part->pdg;
            if (sign) {
                tpdg = std::abs(tpdg);
            }

            if (tpdg != pdg) continue;

            float momentum = fGetVectorLorentz(&true_part->p).Mag();
            if (momentum < p_thres) continue;

            particle_vec.push_back(true_part);
        }

        return particle_vec;

    }

    /**
    * Checks if a reco particle matches a certain true PDG code
    *
    * @param sr the SR object we are working with
    * @param part particle to check
    * @param pdg code to check
    * @param sign whether or not to check for positive and negative
    *
    * @return bool true if it matches, false if not or couldn't backtrack
    */
    bool fBackTrackedToPDG(const caf::SRProxy* sr, caf::SRRecoParticleProxy* part, int const pdg, bool const sign = true) {

        caf::SRTrueInteractionProxy* true_interaction = &sr->mc.nu[0];
        caf::SRTrueParticleProxy* tpart = fGetTrueParticleFromRecoParticle(true_interaction, part);
        if (tpart == nullptr) return false; // unable to backtrack
        
        int tpdg = tpart->pdg;
        if (sign) {
            //std::cout << "    Check both signs" << std::endl;
            tpdg = std::abs(tpdg);
        }

        //std::cout << "    PDG: " << tpdg << std::endl;

        if (tpdg != pdg) return false;

        return true;
    }

    /**
    * Gets vector of reco pions
    * 
    * @param sr the SR object we are working with
    * @param muon_score_cut cut on the muon score
    * @param p_thres
    * @param proton_dEdx_cut
    * @param proton_tof_cut
    * @param delta_calo
    * @param distance_cut
    *
    * @return std::vector<caf::SRRecoParticleProxy*> with particles passing the cuts
    */
    std::vector<caf::SRRecoParticleProxy*> fGetPions(const caf::SRProxy* sr, float const muon_score_cut, float const p_thres, float const proton_dEdx_cut, float const proton_tof_cut, float const delta_calo, float const distance_cut, const int *flow_vec = default_flow_vec) {

        std::vector<caf::SRRecoParticleProxy*> pion_vec;

        // Get the index and start position of the muon candidate
        std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> candidates = fGetMuonCandidates(sr, muon_score_cut);
        size_t muon_candidate_idx = candidates[0].first;
        const caf::SRVector3D muon_candidate_vtx = fGetVector3D(&(candidates[0].second)->start);

        // Get all particles in the interaction
        caf::SRInteractionProxy* interaction = &sr->common.ixn.gsft[0];
        caf::SRTrueInteractionProxy* true_interaction = &sr->mc.nu[0];

        size_t nparts = interaction->part.ngsft;
        for (size_t idx=0; idx<nparts; ++idx) {

            // Continue if it's the muon candidate
            if (idx == muon_candidate_idx) continue;

            caf::SRRecoParticleProxy* part = &interaction->part.gsft[idx];
            float momentum = fGetVector3D(&part->p).Mag();

            //pion_vec.push_back(part);
            //continue;

            // Continue if momentum below threshold
            if (momentum < p_thres) continue;
            if (flow_vec[0] == 0) {
                pion_vec.push_back(part);
                continue;
            }

            float proton_dEdx_score = part->score.gsft_pid.proton_dEdx_score;
            float proton_tof_score  = part->score.gsft_pid.proton_tof_score;

            if (proton_dEdx_score > proton_dEdx_cut) continue;
            if (flow_vec[1] == 0) {
                pion_vec.push_back(part);
                continue;
            }

            if (proton_tof_score > proton_tof_cut) continue;
            if (flow_vec[2] == 0) {
                pion_vec.push_back(part);
                continue;
            }

            // Check if the dE/dx makes sense with the pion hypothesis
            caf::SRGArTrackProxy* track = fGetTrackFromIndex(sr, idx);
            float reco_dEdx = track->dEdx_fwd;
            float expt_dEdx = fAlephParamMomentum(momentum, kMassPion);

            if ((reco_dEdx < expt_dEdx*(1-delta_calo))||(reco_dEdx >= expt_dEdx*(1+delta_calo))) continue;
            if (flow_vec[3] == 0) {
                pion_vec.push_back(part);
                continue;
            }

            // Check the distance to the primary muon candidate
            const caf::SRVector3D vtx = fGetVector3D(&part->start);
            float distance_to_muon = (muon_candidate_vtx-vtx).Mag();

            if (distance_to_muon > distance_cut) continue;

            pion_vec.push_back(part);

        }

        return pion_vec;
    }

    std::vector<caf::SRRecoParticleProxy*> fGetPionsCheated(const caf::SRProxy* sr, float const muon_score_cut, float const p_thres, bool const muon_skip = true, bool const check_momentum_true = true, bool const keep_unique = true) {

        std::vector<caf::SRRecoParticleProxy*> pion_vec;

        // Get the index of the muon candidate
        std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> candidates = fGetMuonCandidates(sr, muon_score_cut);
        size_t muon_candidate_idx = candidates[0].first;

        // Get all particles in the interaction
        caf::SRInteractionProxy* interaction = &sr->common.ixn.gsft[0];
        caf::SRTrueInteractionProxy* true_interaction = &sr->mc.nu[0];

        std::vector<int> pion_tindeces;

        size_t nparts = interaction->part.ngsft;
        for (size_t idx=0; idx<nparts; ++idx) {

            // (Maybe) continue if it's the muon candidate
            if ((muon_skip) && (idx == muon_candidate_idx)) continue;

            caf::SRRecoParticleProxy* part = &interaction->part.gsft[idx];

            // Continue if associated true particle is not a charged pion (or wasn't backtracked)
            caf::SRTrueParticleProxy* tpart = fGetTrueParticleFromRecoParticle(true_interaction, part);
            if (tpart == nullptr) continue;
            if (std::abs(tpart->pdg) != 211) continue;

            // Check if momentum is over threshold
            float momentum;
            if (check_momentum_true) {
                momentum = fGetVectorLorentz(&tpart->p).Mag();
            } else {
                momentum = fGetVector3D(&part->p).Mag();
            }
            if (momentum < p_thres) continue;

            // (Maybe) continue if the true particle was already counted
            if (keep_unique) {
                int tidx = part->truth[0].part;
                if(std::find(pion_tindeces.begin(), pion_tindeces.end(), tidx) != pion_tindeces.end()) {
                    continue;
                } else {
                    pion_tindeces.push_back(tidx);
                }
            }

            pion_vec.push_back(part);

        }

        return pion_vec;
    }

    float fGetTruePrimaryMuonMomentum(const caf::SRProxy* sr, const char* type = "total") {

        std::vector<caf::SRTrueParticleProxy*> muon_vec = fGetTruePDG(sr, 13, true, 0.0);

        // There should be only one primary muon (if any), but check just in case
        size_t nmuons = muon_vec.size();

        if (nmuons != 1) {
            // This covers the case of no muon (contamination) and any weirdness
            return -9999.;
        }

        float momentum;

        if (type == "total") {
            momentum = fGetVectorLorentz(&muon_vec[0]->p).Mag();
        } else if (type == "longitudinal") {
            momentum = fGetVectorLorentz(&muon_vec[0]->p).Z();
        } else if (type == "transverse") {
            caf::SRVector3D momentum_vec(fGetVectorLorentz(&muon_vec[0]->p).X(), fGetVectorLorentz(&muon_vec[0]->p).Y(), 0.0);
            momentum = momentum_vec.Mag();
        } else {
            throw std::runtime_error("Case not known!");
        }

        return momentum;

    }

    float fGetTruePrimaryMuonAngle(const caf::SRProxy* sr) {

        std::vector<caf::SRTrueParticleProxy*> muon_vec = fGetTruePDG(sr, 13, true, 0.0);

        // There should be only one primary muon (if any), but check just in case
        size_t nmuons = muon_vec.size();

        if (nmuons != 1) {
            // This covers the case of no muon (contamination) and any weirdness
            return -9999.;
        }

        const caf::SRVector3D beam(0, 0, 1);
        const caf::SRLorentzVector muon_momentum = fGetVectorLorentz(&muon_vec[0]->p);
        const caf::SRVector3D muon_direction(muon_momentum.X(), muon_momentum.Y(), muon_momentum.Z());

        float angle = muon_direction.Unit().Dot(beam);

        return angle;

    }

    float fGetTrueVertexPosition(const caf::SRProxy* sr, const char* type = "X") {

        caf::SRVector3D vtx = fGetVector3D(&sr->mc.nu[0].vtx);

        std::vector<caf::SRTrueParticleProxy*> muon_vec = fGetTruePDG(sr, 13, true, 0.0);

       /*  // There should be only one primary muon (if any), but check just in case
        size_t nmuons = muon_vec.size();

        if (nmuons != 1) {
            // This covers the case of no muon (contamination) and any weirdness
            return -9999.;
        } */

        float position;

        if (type == "X") {
            position = vtx.X();
        } else if (type == "Y") {
            position = vtx.Y();
        } else if (type == "Z") {
            position = vtx.Z();
        } else {
            throw std::runtime_error("Case not known!");
        }

        return position;

    }

    /* -------------------------------------------------------------------------- */
    /*                                 Truth cuts                                 */
    /* -------------------------------------------------------------------------- */

    const SRCut kIsNoCut([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    return true;
    });
    
    const SRCut kIsTrueNuMu([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    int pdg = sr->mc.nu[0].pdg;
                    return pdg==14;
    });

    const SRCut kIsTrueANuMu([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    int pdg = sr->mc.nu[0].pdg;
                    return pdg==-14;
    });

    const SRCut kIsTrueCC([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    bool cc = sr->mc.nu[0].iscc;
                    return cc;
    });

    const SRCut kIsTrueNuMuCC([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    int pdg = sr->mc.nu[0].pdg;
                    bool cc = sr->mc.nu[0].iscc;
                    return (pdg==14)&&cc;
    });

    const SRCut kIsTrueANuMuCC([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    int pdg = sr->mc.nu[0].pdg;
                    bool cc = sr->mc.nu[0].iscc;
                    return (pdg==-14)&&cc;
    });

    const SRCut kIsTrueNC([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    bool cc = sr->mc.nu[0].iscc;
                    return !cc;
    });
    
    const SRCut kIsTrueNuMuNC([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    int pdg = sr->mc.nu[0].pdg;
                    bool cc = sr->mc.nu[0].iscc;
                    return (pdg==14)&&!cc;
    });

    const SRCut kIsTrueQE([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    return sr->mc.nu[0].mode == caf::kQE;
    });

    const SRCut kIsTrueDIS([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    return sr->mc.nu[0].mode == caf::kDIS;
    });

    const SRCut kIsTrueRES([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    return sr->mc.nu[0].mode == caf::kRes;
    });

    const SRCut kIsTrueMEC([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    return sr->mc.nu[0].mode == caf::kMEC;
    });

    const SRCut kIsTrueOther([](const caf::SRProxy* sr) {
                    if(sr->mc.nnu == 0) return false;
                    assert(sr->mc.nnu == 1);
                    caf::ScatteringMode mode = sr->mc.nu[0].mode;
                    return (mode != caf::kQE)&&(mode != caf::kDIS)&&(mode != caf::kRes)&&(mode != caf::kMEC);
    });

    const SRCut kIsTrueContained(float delta_r = kHPgTPCFidCutR){
        const SRCut kIsThisTrueContained([delta_r](const caf::SRProxy* sr) {
                        if(sr->mc.nnu == 0) return false;
                        assert(sr->mc.nnu == 1);
                        const std::vector<float> vtx = fGetVectorStd(&sr->mc.nu[0].vtx);
                        bool contained = fContainedInFiducial(vtx, delta_r);
                        return contained;
        });

        return kIsThisTrueContained;

    }

    /* -------------------------------------------------------------------------- */
    /*                                  Reco cuts                                 */
    /* -------------------------------------------------------------------------- */

    const SRCut kIsContainedNuMuCC(float muon_score_cut, float delta_r = kHPgTPCFidCutR, bool momentum_method = true, int sign_selection = -1, const int *flow_vec = default_flow_vec){
        const SRCut kIsThisContainedNuMuCC([muon_score_cut, delta_r, momentum_method, sign_selection, flow_vec](const caf::SRProxy* sr) {

                    // Get vector of muon candidates
                    std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> candidates = fGetMuonCandidates(sr, muon_score_cut, momentum_method, sign_selection, flow_vec);
                    size_t n_candidates = candidates.size();
                    if (n_candidates == 0) return false;

                    // Stop at this cut if indicated
                    if (flow_vec[2] == 0) return true;

                    // Reco vertex is the start point of the highest momentum muon candidate
                    const std::vector<float> vtx = fGetVectorStd(&(candidates[0].second)->start);
                    bool contained = fContainedInFiducial(vtx, delta_r);

                    return contained;

        });

        return kIsThisContainedNuMuCC;
    }

    const SRCut kIsContainedNuMuCCRaw(float delta_r = kHPgTPCFidCutR, bool momentum_method = true){
        const SRCut kIsThisContainedNuMuCCRaw([delta_r, momentum_method](const caf::SRProxy* sr) {

                    // Get vector of muon candidates
                    std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> candidates = fGetRawMuonCandidates(sr, momentum_method);
                    size_t n_candidates = candidates.size();
                    if (n_candidates == 0) return false;

                    // Reco vertex is the start point of the highest momentum muon candidate
                    const std::vector<float> vtx = fGetVectorStd(&(candidates[0].second)->start);
                    bool contained = fContainedInFiducial(vtx, delta_r);

                    return contained;

        });

        return kIsThisContainedNuMuCCRaw;
    }

    const SRCut kIsTrueNPi(int const n, float const p_thres){
        const SRCut kIsThisTrueNPi([n, p_thres](const caf::SRProxy* sr) {

                    // Get vector of pion candidates
                    std::vector<caf::SRTrueParticleProxy*> pion_vec = fGetTruePDG(sr, 211, true, p_thres);

                    return pion_vec.size() == n;

        });

        return kIsThisTrueNPi;
    }

    const SRCut kIsNPi(int const n, float const muon_score_cut, float const p_thres, float const proton_dEdx_cut, float const proton_tof_cut, float const delta_calo, float const distance_cut, const int *flow_vec = default_flow_vec){
        const SRCut kIsThisNPi([n, muon_score_cut, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut, flow_vec](const caf::SRProxy* sr) {

                    // Get vector of pion candidates
                    std::vector<caf::SRRecoParticleProxy*> pion_vec = fGetPions(sr, muon_score_cut, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut, flow_vec);

                    return pion_vec.size() == n;

        });

        return kIsThisNPi;
    }

    const SRCut kIsNParticles(int const n){
        const SRCut kIsThisNParticles([n](const caf::SRProxy* sr) {

                    int n_particles;

                    // Check that the SR contains 1 reco interaction
                    if(sr->common.ixn.ngsft == 0) {
                        n_particles = 0;
                    } else {
                        assert(sr->common.ixn.ngsft == 1);

                        caf::SRInteractionProxy* interaction = &sr->common.ixn.gsft[0];
                        n_particles = interaction->part.ngsft;
                    }

                    return n_particles == n;

        });

        return kIsThisNParticles;
    }

    /* -------------------------------------------------------------------------- */
    /*                               Reco Variables                               */
    /* -------------------------------------------------------------------------- */

    const SRVar kTruePrimaryMuonMomentum(const char* type = "total") {
        const SRVar _kTruePrimaryMuonMomentum([type](const caf::SRProxy* sr) -> float {

            return fGetTruePrimaryMuonMomentum(sr, type);

        });

        return _kTruePrimaryMuonMomentum;

    }

    const SRVar kPrimaryMuonMomentum(float muon_score_cut, float delta_r = kHPgTPCFidCutR, bool momentum_method = true, int sign_selection = 0, const char* type = "total") {
        const SRVar _kPrimaryMuonMomentum([muon_score_cut, delta_r, momentum_method, sign_selection, type](const caf::SRProxy* sr) -> float {

            // Get vector of muon candidates
            std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> candidates = fGetMuonCandidates(sr, muon_score_cut, momentum_method, sign_selection);

            // Don't need to check that there's a candidate or if it's in the FDV
            // as the variable will be evaluated in the events passing the cuts only
            if (candidates.size() == 0) {
                return -999.;
            }

            caf::SRRecoParticleProxy* muon = candidates[0].second;

            float momentum;

            if (type == "total") {
                momentum = fGetVector3D(&muon->p).Mag();
            } else if (type == "longitudinal") {
                momentum = fGetVector3D(&muon->p).Z();
            } else if (type == "transverse") {
                caf::SRVector3D momentum_vec(fGetVector3D(&muon->p).X(), fGetVector3D(&muon->p).Y(), 0.0);
                momentum = momentum_vec.Mag();
            } else {
                throw std::runtime_error("Case not known!");
            }

            return momentum;

        });

        return _kPrimaryMuonMomentum;

    }

    const SRVar kPrimaryMuonAngle(float muon_score_cut, float delta_r = kHPgTPCFidCutR, bool momentum_method = true, int sign_selection = 0) {
        const SRVar _kPrimaryMuonAngle([muon_score_cut, delta_r, momentum_method, sign_selection](const caf::SRProxy* sr) -> float {

            // Get vector of muon candidates
            std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> candidates = fGetMuonCandidates(sr, muon_score_cut, momentum_method, sign_selection);

            // Don't need to check that there's a candidate or if it's in the FDV
            // as the variable will be evaluated in the events passing the cuts only
            if (candidates.size() == 0) {
                return -999.;
            }

            caf::SRRecoParticleProxy* muon = candidates[0].second;

            const caf::SRVector3D beam(0, 0, 1);
            float angle = fGetVector3D(&muon->p).Unit().Dot(beam);

            return angle;

        });

        return _kPrimaryMuonAngle;

    }

    const SRVar kTruePrimaryMuonAngle() {
        const SRVar _kTruePrimaryMuonAngle([](const caf::SRProxy* sr) -> float {

            return fGetTruePrimaryMuonAngle(sr);

        });

        return _kTruePrimaryMuonAngle;

    }

    const SRVar kTrueVertexPosition(const char* type = "total") {
        const SRVar _kTrueVertexPosition([type](const caf::SRProxy* sr) -> float {

            return fGetTrueVertexPosition(sr, type);

        });

        return _kTrueVertexPosition;

    }

    const SRVar kVertexPosition(float muon_score_cut, float delta_r = kHPgTPCFidCutR, bool momentum_method = true, int sign_selection = 0, const char* type = "X") {
        const SRVar _kVertexPosition([muon_score_cut, delta_r, momentum_method, sign_selection, type](const caf::SRProxy* sr) -> float {

            // Get vector of muon candidates
            std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> candidates = fGetMuonCandidates(sr, muon_score_cut, momentum_method, sign_selection);

            // Don't need to check that there's a candidate or if it's in the FDV
            // as the variable will be evaluated in the events passing the cuts only
            if (candidates.size() == 0) {
                return -999.;
            }

            caf::SRRecoParticleProxy* muon = candidates[0].second;

            float position;

            if (type == "X") {
                position = fGetVector3D(&muon->start).X();
            } else if (type == "Y") {
                position = fGetVector3D(&muon->start).Y();
            } else if (type == "Z") {
                position = fGetVector3D(&muon->start).Z();
            } else {
                throw std::runtime_error("Case not known!");
            }

            return position;

        });

        return _kVertexPosition;

    }

    const SRVar kVertexPositionFractionalResidual(float muon_score_cut, float delta_r = kHPgTPCFidCutR, bool momentum_method = true, int sign_selection = 0, const char* type = "X") {
        const SRVar _kVertexPositionFractionalResidual([muon_score_cut, delta_r, momentum_method, sign_selection, type](const caf::SRProxy* sr) -> float {

            // Get vector of muon candidates
            std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> candidates = fGetMuonCandidates(sr, muon_score_cut, momentum_method, sign_selection);

            // Don't need to check that there's a candidate or if it's in the FDV
            // as the variable will be evaluated in the events passing the cuts only
            if (candidates.size() == 0) {
                return -999.;
            }

            caf::SRRecoParticleProxy* muon = candidates[0].second;

            float reco_position;

            if (type == "X") {
                reco_position = fGetVector3D(&muon->start).X();
            } else if (type == "Y") {
                reco_position = fGetVector3D(&muon->start).Y();
            } else if (type == "Z") {
                reco_position = fGetVector3D(&muon->start).Z();
            } else {
                throw std::runtime_error("Case not known!");
            }

            float true_position = fGetTrueVertexPosition(sr, type);

            return (true_position-reco_position)/true_position;

        });

        return _kVertexPositionFractionalResidual;

    }

    const SRVar kMuonScorePDG(int pdg) {
        const SRVar kMuonScore([pdg](const caf::SRProxy* sr) -> float {
        
            std::vector<float> muon_score_vec;

            // Get all particles in the interaction
            caf::SRInteractionProxy* interaction = &sr->common.ixn.gsft[0];
            caf::SRTrueInteractionProxy* true_interaction = &sr->mc.nu[0];

            size_t nparts = interaction->part.ngsft;
            for (size_t i=0; i<nparts; ++i) {

                caf::SRRecoParticleProxy* part = &interaction->part.gsft[i];
                float momentum = fGetVector3D(&part->p).Mag();
                if ((momentum < 0.1)||(momentum >= 10.0)) continue;

                caf::SRTrueParticleProxy* tpart = fGetTrueParticleFromRecoParticle(true_interaction, part);
                if (tpart == nullptr) continue;
                if (std::abs(tpart->pdg) != pdg) continue;

                caf::SRGArAssociationProxy* assn = fGetAssociationFromIndex(sr, i);
                if (assn->ECAL_n_hits < 1) continue;

                muon_score_vec.push_back(part->score.gsft_pid.muon_score);

            }

            if (muon_score_vec.size() == 0) {
                const float muon_score[1] = {-999.};
                return *muon_score;
            }

            const float *muon_score = fArrayFromVectorStd(muon_score_vec);

            return *muon_score;

        });

        return kMuonScore;

    }

    const SRVar kProtonToFScorePDG(int pdg) {
        const SRVar kProtonToFScore([pdg](const caf::SRProxy* sr) -> float {
        
            std::vector<float> proton_tof_score_vec;

            // Get all particles in the interaction
            caf::SRInteractionProxy* interaction = &sr->common.ixn.gsft[0];
            caf::SRTrueInteractionProxy* true_interaction = &sr->mc.nu[0];

            size_t nparts = interaction->part.ngsft;
            for (size_t i=0; i<nparts; ++i) {

                caf::SRRecoParticleProxy* part = &interaction->part.gsft[i];
                float momentum = fGetVector3D(&part->p).Mag();
                if ((momentum < 0.5)||(momentum >= 3.0)) continue;

                caf::SRTrueParticleProxy* tpart = fGetTrueParticleFromRecoParticle(true_interaction, part);
                if (tpart == nullptr) continue;
                if (std::abs(tpart->pdg) != pdg) continue;

                caf::SRGArAssociationProxy* assn = fGetAssociationFromIndex(sr, i);
                if (assn->ECAL_n_hits < 1) continue;

                proton_tof_score_vec.push_back(part->score.gsft_pid.proton_tof_score);

            }

            if (proton_tof_score_vec.size() == 0) {
                const float proton_tof_score[1] = {-999.};
                return *proton_tof_score;
            }

            const float *proton_tof_score = fArrayFromVectorStd(proton_tof_score_vec);

            return *proton_tof_score;

        });

        return kProtonToFScore;

    }

    const SRVar kProtondEdxScorePDG(int pdg) {
        const SRVar kProtondEdxScore([pdg](const caf::SRProxy* sr) -> float {
        
            std::vector<float> proton_dEdx_score_vec;

            // Get all particles in the interaction
            caf::SRInteractionProxy* interaction = &sr->common.ixn.gsft[0];
            caf::SRTrueInteractionProxy* true_interaction = &sr->mc.nu[0];

            size_t nparts = interaction->part.ngsft;
            for (size_t i=0; i<nparts; ++i) {

                caf::SRRecoParticleProxy* part = &interaction->part.gsft[i];
                float momentum = fGetVector3D(&part->p).Mag();
                if (momentum > 1.5) continue;

                caf::SRTrueParticleProxy* tpart = fGetTrueParticleFromRecoParticle(true_interaction, part);
                if (tpart == nullptr) continue;
                if (std::abs(tpart->pdg) != pdg) continue;

                proton_dEdx_score_vec.push_back(part->score.gsft_pid.proton_dEdx_score);

            }

            if (proton_dEdx_score_vec.size() == 0) {
                const float proton_dEdx_score[1] = {-999.};
                return *proton_dEdx_score;
            }

            const float *proton_dEdx_score = fArrayFromVectorStd(proton_dEdx_score_vec);

            return *proton_dEdx_score;

        });

        return kProtondEdxScore;

    }

    const SRVar kGetPionKEnergyPDG(float const muon_score_cut, float const pdg, float const p_thres, float const proton_dEdx_cut, float const proton_tof_cut, float const delta_calo, float const distance_cut, const int *flow_vec = default_flow_vec) {
        const SRVar kGetPionKEnergy([muon_score_cut, pdg, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut, flow_vec](const caf::SRProxy* sr) -> float
        {
            
            // Get vector with selected reco pions
            std::vector<caf::SRRecoParticleProxy*> pion_vec = fGetPions(sr, muon_score_cut, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut, flow_vec);

            std::vector<float> pion_ke_vec;

            //std::cout << "Number of pions: " << pion_vec.size() << std::endl;

            for (auto part: pion_vec) {
                
                // Check the particle backtracks to desired PDG
                if (pdg != -1){
                    //std::cout << "    Check true PDG, target: " << pdg << std::endl;
                    if (!fBackTrackedToPDG(sr, part, pdg)) continue;
                }

                float momentum = fGetVector3D(&part->p).Mag();
                float kinetic_energy = std::sqrt(momentum*momentum+kMassPion*kMassPion) - kMassPion;
                pion_ke_vec.push_back(kinetic_energy);

            }

            // If no particles at all return -999.
            if (pion_ke_vec.size() == 0) {
                const float pion_ke[1] = {-999.};
                return *pion_ke;
            }

            const float *pion_ke = fArrayFromVectorStd(pion_ke_vec);

            return *pion_ke;

        });

        return kGetPionKEnergy;
    }

    const SRVar kGetPionMuAnglePDG(float const muon_score_cut, float const pdg, float const p_thres, float const proton_dEdx_cut, float const proton_tof_cut, float const delta_calo, float const distance_cut, const int *flow_vec = default_flow_vec) {
        const SRVar kGetPionMuAngle([muon_score_cut, pdg, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut, flow_vec](const caf::SRProxy* sr) -> float
        {
            
            // Get vector of muon candidates
            std::vector<std::pair<size_t, caf::SRRecoParticleProxy*>> candidates = fGetMuonCandidates(sr, muon_score_cut, true, -1);

            // Don't need to check that there's a candidate or if it's in the FDV
            // as the variable will be evaluated in the events passing the cuts only
            if (candidates.size() == 0) {
                return -999.;
            }

            caf::SRRecoParticleProxy* muon = candidates[0].second;
            const caf::SRVector3D direction_muon = fGetVector3D(&muon->p).Unit();
            
            // Get vector with selected reco pions
            std::vector<caf::SRRecoParticleProxy*> pion_vec = fGetPions(sr, muon_score_cut, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut, flow_vec);

            std::vector<float> pion_ang_vec;

            for (auto part: pion_vec) {
                
                // Check the particle backtracks to desired PDG
                if (pdg != -1){
                    if (!fBackTrackedToPDG(sr, part, pdg)) continue;
                }

                const caf::SRVector3D direction_pion = fGetVector3D(&part->p).Unit();

                pion_ang_vec.push_back(direction_pion.Dot(direction_muon));

            }

            // If no particles at all return -999.
            if (pion_ang_vec.size() == 0) {
                const float pion_ang[1] = {-999.};
                return *pion_ang;
            }

            const float *pion_ang = fArrayFromVectorStd(pion_ang_vec);

            return *pion_ang;

        });

        return kGetPionMuAngle;
    }

    const SRVar kGetTruePionMultiplicity(float const p_thres) {
        const SRVar _kGetTruePionMultiplicity([p_thres](const caf::SRProxy* sr) -> float
        {
            
            std::vector<caf::SRTrueParticleProxy*> pion_vec = fGetTruePDG(sr, 211, true, p_thres);

            return pion_vec.size();

        });

        return _kGetTruePionMultiplicity;
    }

    const SRVar kGetPionMultiplicity(float const muon_score_cut, float const p_thres, float const proton_dEdx_cut, float const proton_tof_cut, float const delta_calo, float const distance_cut, const int *flow_vec = default_flow_vec) {
        const SRVar _kGetPionMultiplicity([muon_score_cut, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut, flow_vec](const caf::SRProxy* sr) -> float
        {
            
            // Get vector with selected reco pions
            std::vector<caf::SRRecoParticleProxy*> pion_vec = fGetPions(sr, muon_score_cut, p_thres, proton_dEdx_cut, proton_tof_cut, delta_calo, distance_cut, default_flow_vec);

            return pion_vec.size();

        });

        return _kGetPionMultiplicity;
    }

    const SRVar kGetPionMultiplicityCheated(float const muon_score_cut, float const p_thres, bool const muon_skip = true, bool const check_momentum_true = true, bool const keep_unique = true) {
        const SRVar _kGetPionMultiplicityCheated([muon_score_cut, p_thres, muon_skip, check_momentum_true, keep_unique](const caf::SRProxy* sr) -> float
        {
            
            // Get vector with cheated reco pions
            std::vector<caf::SRRecoParticleProxy*> pion_vec = fGetPionsCheated(sr, muon_score_cut, p_thres, muon_skip, check_momentum_true, keep_unique);

            return pion_vec.size();

        });

        return _kGetPionMultiplicityCheated;
    }

}