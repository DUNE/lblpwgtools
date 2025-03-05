#include "CAFAna/GAr/GArUtils.h"

namespace ana
{

    /* -------------------------------------------------------------------------- */
    /*                                 Operations                                 */
    /* -------------------------------------------------------------------------- */

    std::vector<double> fLinspace(const double start, const double end, const int num_in) {

        std::vector<double> linspaced;

        if (num_in == 0) { return linspaced; }
        if (num_in == 1) {
            linspaced.push_back(start);
            return linspaced;
        }

        const double num = static_cast<double>(num_in);
        double delta = (end - start) / (num - 1);

        for(int i=0; i < num_in-1; ++i) {
            linspaced.push_back(start + delta * i);
        }

        linspaced.push_back(end); // I want to ensure that start and end
                                    // are exactly the same as the input
        return linspaced;
    }

    std::vector<double> fBinCenters(const std::vector<double> &vec) {

        std::vector<double> centres;

        for(std::size_t i = 0; i < vec.size()-1; i++) {
            centres.push_back((vec[i+1] + vec[i])/2);
        }

        return centres;
    }

    std::vector<double> fCrossProduct(const std::vector<double> &a, const std::vector<double> &b) {
        std::vector<double> r (a.size());  
        r[0] = a[1]*b[2]-a[2]*b[1];
        r[1] = a[2]*b[0]-a[0]*b[2];
        r[2] = a[0]*b[1]-a[1]*b[0];
        return r;
    }

    /* -------------------------------------------------------------------------- */
    /*                              Geometry-related                              */
    /* -------------------------------------------------------------------------- */

    bool fPointInCylinder(const std::vector<double> &pt1, const std::vector<double> &pt2, const double r, const std::vector<double> &q) {

        std::vector<double> vec;
        std::transform(pt2.begin(), pt2.end(), pt1.begin(), std::back_inserter(vec), std::minus<double>());
        double constant = r * std::sqrt(std::inner_product(vec.begin(), vec.end(), vec.begin(), 0.0));

        std::vector<double> diff1;
        std::transform(q.begin(), q.end(), pt1.begin(), std::back_inserter(diff1), std::minus<double>());
        double product1 = std::inner_product(diff1.begin(), diff1.end(), vec.begin(), 0.0);

        std::vector<double> diff2;
        std::transform(q.begin(), q.end(), pt2.begin(), std::back_inserter(diff2), std::minus<double>());
        double product2 = std::inner_product(diff2.begin(), diff2.end(), vec.begin(), 0.0);

        std::vector<double> cross_product = fCrossProduct(diff1, vec);
        double norm_cross_product = std::sqrt(std::inner_product(cross_product.begin(), cross_product.end(), cross_product.begin(), 0.0));
        
        return (product1 >= 0) && (product2 <= 0) && (norm_cross_product <= constant);
    }

    bool fContainedInFiducial(const std::vector<double> &q, const double delta_l, const double delta_r) {

        const double kFVLength = kHPgTPCLength - delta_l;

        const std::vector<double> kFVLeftFace  = {kHPgTPCX-kFVLength, kHPgTPCY, kHPgTPCZ};
        const std::vector<double> kFVRightFace = {kHPgTPCX+kFVLength, kHPgTPCY, kHPgTPCZ};

        const double kFVRadius = kHPgTPCRadius - delta_r;

        return fPointInCylinder(kFVLeftFace, kFVRightFace, kFVRadius, q);
    }

    /* -------------------------------------------------------------------------- */
    /*                           Vector transformations                           */
    /* -------------------------------------------------------------------------- */

    const std::vector<double> fGetVectorStd(const caf::SRVector3DProxy* vec) {
        const std::vector<double> _vec = {vec->x, vec->y, vec->z};
        return _vec;
    }

    const caf::SRVector3D fGetVector3D(const caf::SRVector3DProxy* vec) {
        const caf::SRVector3D _vec(vec->x, vec->y, vec->z);
        return _vec;
    }

    const caf::SRLorentzVector fGetVectorLorentz(const caf::SRLorentzVectorProxy* vec) {
        const TLorentzVector _tvec(vec->px, vec->py, vec->pz, vec->E);
        const caf::SRLorentzVector _vec(_tvec);
        return _vec;
    }

    const float *fArrayFromVectorStd(const std::vector<float> vec) {

        size_t len = vec.size();
        float *ret = new float[len];

        for (size_t i=0; i<len; ++i) {
            ret[i] = vec[i];
        }

        return ret;
    }

    /* -------------------------------------------------------------------------- */
    /*                               Object getters                               */
    /* -------------------------------------------------------------------------- */

    std::vector<caf::SRTrueParticleProxy*> fGetTruePDG(const caf::SRProxy* sr, const int pdg, const bool sign, const float p_thres) {

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

}