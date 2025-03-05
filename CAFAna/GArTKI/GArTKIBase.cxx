#include "CAFAna/GArTKI/GArTKIBase.h"

namespace ana
{

    /* -------------------------------------------------------------------------- */
    /*                              Helper functions                              */
    /* -------------------------------------------------------------------------- */

    const TVector3
    fGetTransverseVector(const TVector3 p, const TVector3 n) {
        return p - (p.Dot(n) * n);
    }

    const TVector3
    fGetLongitudinalVector(const TVector3 p, const TVector3 n) {
        return p.Dot(n) * n;
    }

    const std::pair<TVector3, TVector3>
    fGetThreeMomentumTKI(const std::vector<std::pair<int, TLorentzVector>> cand_vector) {

        // Get 3-momentum of muon and hadron systems
        TVector3 muon_three_momentum(0.0, 0.0, 0.0);
        TVector3 hdrn_three_momentum(0.0, 0.0, 0.0);
        for (auto candidate: cand_vector) {

            int pdg = candidate.first;
            TLorentzVector four_momentum = candidate.second;

            if ( abs(pdg) == 13 ) muon_three_momentum += four_momentum.Vect();
            if ( pdg == 2212 ) hdrn_three_momentum += four_momentum.Vect();
            if ( abs(pdg) == 211 ) hdrn_three_momentum += four_momentum.Vect();

        }

        return std::make_pair(muon_three_momentum, hdrn_three_momentum);

    }

    /* -------------------------------------------------------------------------- */
    /*                           Observables definition                           */
    /* -------------------------------------------------------------------------- */

    double
    fComputeTKIdeltapT(const std::vector<std::pair<int, TLorentzVector>> cand_vector) {

        // Get the three-momentum of the muon and hadronic systems
        const std::pair<TVector3, TVector3> cand_momenta = fGetThreeMomentumTKI(cand_vector);

        // Get the transverse momenta
        const TVector3 muon_trans_momentum = fGetTransverseVector(cand_momenta.first,  beam_direction);
        const TVector3 hdrn_trans_momentum = fGetTransverseVector(cand_momenta.second, beam_direction);

        // Compute delta pT
        const TVector3 delta_pT = muon_trans_momentum + hdrn_trans_momentum;

        // Return only magnitude
        return delta_pT.Mag();

    }

    double
    fComputeTKIdeltaAlphaT(const std::vector<std::pair<int, TLorentzVector>> cand_vector) {

         // Get the three-momentum of the muon and hadronic systems
        const std::pair<TVector3, TVector3> cand_momenta = fGetThreeMomentumTKI(cand_vector);

        // Compute the transverse momenta
        const TVector3 muon_trans_momentum = fGetTransverseVector(cand_momenta.first,  beam_direction);
        const TVector3 hdrn_trans_momentum = fGetTransverseVector(cand_momenta.second, beam_direction);

        // Compute delta pT
        const TVector3 delta_pT = muon_trans_momentum + hdrn_trans_momentum;

        // Compute delta alphaT
        double deltaAlphaT = TMath::ACos(-muon_trans_momentum.Dot(delta_pT) / (delta_pT.Mag() * muon_trans_momentum.Mag()));

        // Make sure to return it in deg
        return deltaAlphaT * 180.0 * TMath::InvPi();
    
    }

    double
    fComputeVisibleEnergy(const std::vector<std::pair<int, TLorentzVector>> cand_vector, const bool add_pion_mass) {

        // Get visible energy of the event (start adding the binding energy)
        double visible_energy = average_binding_energy;
        for (auto candidate: cand_vector) {

            int pdg = candidate.first;
            TLorentzVector four_momentum = candidate.second;

            if ( abs(pdg) == 13 ) visible_energy += four_momentum.E();
            if ( pdg == 2212 ) visible_energy += four_momentum.E() - kMassProton;
            if ( abs(pdg) == 211 ) {
                if (add_pion_mass) {
                    visible_energy += four_momentum.E();
                } else {
                    visible_energy += four_momentum.E() - kMassPion;
                }
            }

        }

        return visible_energy;

    }

    double
    fComputeAvailableEnergy(const std::vector<std::pair<int, TLorentzVector>> cand_vector, const bool add_pion_mass) {

        // Get visible energy of the event (start adding the binding energy)
        double available_energy = 0.0;
        for (auto candidate: cand_vector) {

            int pdg = candidate.first;
            TLorentzVector four_momentum = candidate.second;

            if ( pdg == 2212 ) available_energy += four_momentum.E() - kMassProton;
            if ( abs(pdg) == 211 ) {
                if (add_pion_mass) {
                    available_energy += four_momentum.E();
                } else {
                    available_energy += four_momentum.E() - kMassPion;
                }
            }

        }

        return available_energy;

    }

    double
    fComputeTKIdeltapL(const std::vector<std::pair<int, TLorentzVector>> cand_vector, const bool add_pion_mass) {

        // Get the three-momentum of the muon and hadronic systems
        const std::pair<TVector3, TVector3> cand_momenta = fGetThreeMomentumTKI(cand_vector);

        // Get the longitudinal momenta
        const TVector3 muon_lngtdnl_momentum = fGetLongitudinalVector(cand_momenta.first,  beam_direction);
        const TVector3 hdrn_lngtdnl_momentum = fGetLongitudinalVector(cand_momenta.second, beam_direction);

        // Get the visible energy
        double visible_energy = fComputeVisibleEnergy(cand_vector, add_pion_mass);

        // Compute delta pL
        double delta_pL = muon_lngtdnl_momentum.Mag() + hdrn_lngtdnl_momentum.Mag() - visible_energy;

        return delta_pL;

    }

    double
    fComputeNeutronMomentum(const std::vector<std::pair<int, TLorentzVector>> cand_vector, const bool add_pion_mass) {

        double delta_pT = fComputeTKIdeltapT(cand_vector);
        double delta_pL = fComputeTKIdeltapL(cand_vector, add_pion_mass);

        return TMath::Sqrt(TMath::Power(delta_pT, 2) + TMath::Power(delta_pL, 2));

    }

    /* -------------------------------------------------------------------------- */
    /*                               Selection class                              */
    /* -------------------------------------------------------------------------- */

    TKISelection::TKISelection(double muon_cut_low,
                               double muon_cut_hgh,
                               double prtn_cut_low,
                               double prtn_cut_hgh,
                               double pion_cut_hgh,
                               bool fsi,
                               bool add_pion_mass) {

        this->muon_cut_low = muon_cut_low;
        this->muon_cut_hgh = muon_cut_hgh;
        this->prtn_cut_low = prtn_cut_low;
        this->prtn_cut_hgh = prtn_cut_hgh;
        this->pion_cut_hgh = pion_cut_hgh;
        this->fsi = fsi;
        this->add_pion_mass = add_pion_mass;

    }

    TKISelection::~TKISelection() {

    }

    void TKISelection::fClean() {

        // Particle multiplicities
        num_muons = 0;
        num_cand_muons = 0;
        num_prtns = 0;
        num_cand_prtns = 0;
        num_ntrns = 0;
        num_chrg_pions = 0;
        num_cand_chrg_pions = 0;
        num_ntrl_pions = 0;

        // Particle vector
        particle_vector.clear();

    }

    void TKISelection::fPrepareEvent(const caf::SRProxy* sr) {

        // Clear member variables
        fClean();

        // Get a pointer to true interaction object
        caf::SRTrueInteractionProxy* true_interaction = &sr->mc.nu[0];

        // Get the appropriate particle collection
        size_t nparts = 0;
        caf::Proxy<std::vector<caf::SRTrueParticle>>* parts = nullptr;

        if (fsi) {

            nparts = true_interaction->nprim;
            parts = &true_interaction->prim;

        } else {

            nparts = true_interaction->nprefsi;
            parts = &true_interaction->prefsi;

            // Pre-FSI collection doesn't include the primary lepton
            // We need to put it by hand, good thing it's always the
            // first element of the post-FSI collection :S
            
            caf::SRTrueParticleProxy* true_part = &true_interaction->prim[0];

            int pdg = true_part->pdg;
            TLorentzVector four_momentum = TLorentzVector(fGetVectorLorentz(&true_part->p));
            double momentum = four_momentum.Vect().Mag();

            if ( abs(pdg) == 13 ) {
                num_muons++;
                if ( (momentum >= muon_cut_low) && (momentum < muon_cut_hgh) ) {
                    num_cand_muons++;
                    particle_vector.push_back(std::make_pair(pdg, four_momentum));
                }
            }

        }

        // Loop over collection of primary particles
        for (size_t idx=0; idx<nparts; ++idx) {

            // Get true particle and its PDG code
            caf::SRTrueParticleProxy* true_part = &parts->at(idx);
            int pdg = true_part->pdg;

            // Get 4-momentum and |p|
            TLorentzVector four_momentum = TLorentzVector(fGetVectorLorentz(&true_part->p));
            double momentum = four_momentum.Vect().Mag();

            if ( abs(pdg) == 13 ) {
                num_muons++;
                if ( (momentum >= muon_cut_low) && (momentum < muon_cut_hgh) ) {
                    num_cand_muons++;
                    particle_vector.push_back(std::make_pair(pdg, four_momentum));
                }
            } else if ( pdg == 2212 ) {
                num_prtns++;
                if ( (momentum >= prtn_cut_low) && (momentum < prtn_cut_hgh) ) {
                    num_cand_prtns++;
                    particle_vector.push_back(std::make_pair(pdg, four_momentum));
                }
            } else if ( pdg == 2112 ) {
                num_ntrns++;
            } else if ( abs(pdg) == 211 ) {
                num_chrg_pions++;
                if ( momentum > pion_cut_hgh ) {
                    num_cand_chrg_pions++;
                    particle_vector.push_back(std::make_pair(pdg, four_momentum));
                }
            } else if ( pdg == 111 ) {
                num_ntrl_pions++;
            }

        }

        idx = (&sr->mc.nu[0])->genieIdx;
        enu = (&sr->mc.nu[0])->E;

    }

    void TKISelection::fCheckEvent(const caf::SRProxy* sr) {

        // Check GENIE ID and Enu
        long int current_idx = (&sr->mc.nu[0])->genieIdx;
        float current_enu = (&sr->mc.nu[0])->E;

        // If the ID or the E doesn't match the previous we need to load the event
        if ( (current_idx != idx) || (current_enu != enu) ) {
            fPrepareEvent(sr);
        }

    }

    const SRVar TKISelection::kEnu() {
        const SRVar _kEnu([this](const caf::SRProxy* sr) -> float {
            return enu;
        });
        return _kEnu;
    }

    const SRVar TKISelection::kDeltapT() {
        const SRVar _kDeltapT([this](const caf::SRProxy* sr) -> float {
            return fComputeTKIdeltapT(particle_vector);
        });
        return _kDeltapT;
    }

    const SRVar TKISelection::kDeltaAlphaT() {
        const SRVar _kDeltaAlphaT([this](const caf::SRProxy* sr) -> float {
            return fComputeTKIdeltaAlphaT(particle_vector);
        });
        return _kDeltaAlphaT;
    }

    const SRVar TKISelection::kVisibleE() {
        const SRVar _kVisibleE([this](const caf::SRProxy* sr) -> float {
            return fComputeVisibleEnergy(particle_vector, false);
        });
        return _kVisibleE;
    }

    const SRVar TKISelection::kAvailableE() {
        const SRVar _kAvailableE([this](const caf::SRProxy* sr) -> float {
            return fComputeAvailableEnergy(particle_vector, false);
        });
        return _kAvailableE;
    }

    const SRVar TKISelection::kNeutronMomentum() {
        const SRVar _kNeutronMomentum([this](const caf::SRProxy* sr) -> float {
            return fComputeNeutronMomentum(particle_vector, false);
        });
        return _kNeutronMomentum;
    }

    const SRVar TKISelection::kProtonMultiplicity() {
        const SRVar _kProtonMultiplicity([this](const caf::SRProxy* sr) -> float {
            return num_cand_prtns;
        });
        return _kProtonMultiplicity;
    }

}