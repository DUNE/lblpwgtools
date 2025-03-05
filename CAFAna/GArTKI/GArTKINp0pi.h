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

    // Default momentum cut values for ND-GAr CCNp0pi selection (in GeV/c)
    const double default_muon_cut_low = 0.05;
    const double default_muon_cut_hgh = 5.00;
    const double default_prtn_cut_low = 0.10;
    const double default_prtn_cut_hgh = 5.00;
    const double default_pion_cut_hgh = 0.05;

    /* -------------------------------------------------------------------------- */
    /*                               Selection class                              */
    /* -------------------------------------------------------------------------- */

    class Np0piTKISelection : public TKISelection {
        public:

            Np0piTKISelection(double muon_cut_low,
                               double muon_cut_hgh,
                               double prtn_cut_low,
                               double prtn_cut_hgh,
                               double pion_cut_hgh,
                               bool fsi,
                               bool add_pion_mass);

            const SRCut kNp0piTKICut();

    };

}