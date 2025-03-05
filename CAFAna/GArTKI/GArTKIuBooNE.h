#pragma once

#include "CAFAna/Core/Cut.h"
#include "CAFAna/GAr/GArParams.h"
#include "CAFAna/GAr/GArUtils.h"
#include "CAFAna/GArTKI/GArTKIBase.h"

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TMath.h"

namespace ana
{

    /* -------------------------------------------------------------------------- */
    /*                                  Constants                                 */
    /* -------------------------------------------------------------------------- */

    // Default momentum cut values for MicroBooNE CC1p0pi selection (in GeV/c)
    const double default_muon_cut_low = 0.10;
    const double default_muon_cut_hgh = 1.20;
    const double default_prtn_cut_low = 0.30;
    const double default_prtn_cut_hgh = 1.00;
    const double default_pion_cut_hgh = 0.07;

    /* -------------------------------------------------------------------------- */
    /*                               Selection class                              */
    /* -------------------------------------------------------------------------- */

    class uBooNETKISelection : public TKISelection {
        public:

            uBooNETKISelection(double muon_cut_low,
                               double muon_cut_hgh,
                               double prtn_cut_low,
                               double prtn_cut_hgh,
                               double pion_cut_hgh,
                               bool fsi,
                               bool add_pion_mass);

            const SRCut kuBooNETKICut();

    };

}