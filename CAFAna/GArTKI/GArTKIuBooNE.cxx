#include "CAFAna/GArTKI/GArTKIuBooNE.h"

namespace ana
{

    /* -------------------------------------------------------------------------- */
    /*                               Selection class                              */
    /* -------------------------------------------------------------------------- */

    uBooNETKISelection::uBooNETKISelection(double muon_cut_low,
                                           double muon_cut_hgh,
                                           double prtn_cut_low,
                                           double prtn_cut_hgh,
                                           double pion_cut_hgh,
                                           bool fsi,
                                           bool add_pion_mass) :  
    TKISelection(muon_cut_low, muon_cut_hgh, prtn_cut_low, prtn_cut_hgh, pion_cut_hgh, fsi, add_pion_mass) { }

    const SRCut
    uBooNETKISelection::kuBooNETKICut(){
        const SRCut
        _kuBooNETKICut([this](const caf::SRProxy* sr) {

            // Make sure there is exactly one neutrino interaction in the event
            if(sr->mc.nnu == 0) return false;
            assert(sr->mc.nnu == 1);

            // Check whether the event was already loaded or not
            fCheckEvent(sr);

            // Selection logic
            return (num_cand_muons == 1) && (num_cand_prtns == 1) && (num_cand_chrg_pions == 0) && (num_ntrl_pions == 0);

        });

        return _kuBooNETKICut;
    }

}