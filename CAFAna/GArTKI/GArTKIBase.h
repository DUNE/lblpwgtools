#pragma once

#include "CAFAna/Core/Var.h"
#include "CAFAna/GAr/GArParams.h"
#include "CAFAna/GAr/GArUtils.h"

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TMath.h"

namespace ana
{

    /* -------------------------------------------------------------------------- */
    /*                                  Constants                                 */
    /* -------------------------------------------------------------------------- */

    // Average removal energy for Ar (in GeV)
    const double average_binding_energy = 0.031;

    // Beam direction (i.e. the z axis)
    const TVector3 beam_direction(0, 0, 1);

    /* -------------------------------------------------------------------------- */
    /*                              Helper functions                              */
    /* -------------------------------------------------------------------------- */

    const TVector3
    fGetTransverseVector(const TVector3 p, const TVector3 n);

    const TVector3
    fGetLongitudinalVector(const TVector3 p, const TVector3 n);

    const std::pair<TVector3, TVector3>
    fGetThreeMomentumTKI(const std::vector<std::pair<int, TLorentzVector>> cand_vector);

    /* -------------------------------------------------------------------------- */
    /*                           Observables definition                           */
    /* -------------------------------------------------------------------------- */

    double
    fComputeTKIdeltapT(const std::vector<std::pair<int, TLorentzVector>> cand_vector);

    double
    fComputeTKIdeltaAlphaT(const std::vector<std::pair<int, TLorentzVector>> cand_vector);

    double
    fComputeVisibleEnergy(const std::vector<std::pair<int, TLorentzVector>> cand_vector, const bool add_pion_mass = false);

    double
    fComputeAvailableEnergy(const std::vector<std::pair<int, TLorentzVector>> cand_vector, const bool add_pion_mass);

    double
    fComputeTKIdeltapL(const std::vector<std::pair<int, TLorentzVector>> cand_vector, const bool add_pion_mass = false);

    double
    fComputeNeutronMomentum(const std::vector<std::pair<int, TLorentzVector>> cand_vector, const bool add_pion_mass = false);

    /* -------------------------------------------------------------------------- */
    /*                               Selection class                              */
    /* -------------------------------------------------------------------------- */

    class TKISelection {
        protected:

            // Selection cuts
            double muon_cut_low;
            double muon_cut_hgh;
            double prtn_cut_low;
            double prtn_cut_hgh;
            double pion_cut_hgh;

            // Use FSI
            bool fsi;

            // Add charged pion mass
            bool add_pion_mass;

            // Was the event already loaded in memory(?)
            long int idx = -1;
            float enu = -999.0;

            // Particle multiplicities
            int num_muons;
            int num_cand_muons;
            int num_prtns;
            int num_cand_prtns;
            int num_ntrns;
            int num_chrg_pions;
            int num_cand_chrg_pions;
            int num_ntrl_pions;

            // Particle vector
            std::vector<std::pair<int, TLorentzVector>> particle_vector;

        public:

            TKISelection(double muon_cut_low, double muon_cut_hgh, double prtn_cut_low, double prtn_cut_hgh, double pion_cut_hgh, bool fsi, bool add_pion_mass);
            ~TKISelection();

            void fClean();
            void fPrepareEvent(const caf::SRProxy* sr);
            void fCheckEvent(const caf::SRProxy* sr);

            const SRVar kEnu();
            const SRVar kDeltapT();
            const SRVar kDeltaAlphaT();
            const SRVar kVisibleE();
            const SRVar kAvailableE();
            const SRVar kNeutronMomentum();
            const SRVar kProtonMultiplicity();

    };

}