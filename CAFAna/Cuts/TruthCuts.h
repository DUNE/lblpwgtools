#pragma once

#include "CAFAna/Core/Cut.h"


namespace ana
{




  /// \brief Is this a Neutral %Current event?
  extern const TruthCut kIsNC;
  extern const TruthCut kIsNue;
  extern const TruthCut kIsNumu;
  extern const TruthCut kIsNutau;

  //----------------------------------------------------------------------
  /// Helper for defining true CC event cuts
  class CCFlavSel
  {
  public:
    CCFlavSel(int pdg, int pdgorig) : fPdg(pdg), fPdgOrig(pdgorig)
    {
    }

    bool operator()(const caf::SRTrueInteractionProxy* nu) const;

  protected:
    int fPdg, fPdgOrig;
  };

  // Finally, the function argument to the Cut constructor can be a "functor"
  // object (one with operator()). This allows similar logic but with different
  // constants to be easily duplicated.

  /// Select CC \f$ \nu_\mu\to\nu_e \f$
  const TruthCut kIsNueApp (CCFlavSel(12, 14));
  /// Select CC \f$ \nu_\mu\to\nu_\mu \f$
  const TruthCut kIsNumuCC (CCFlavSel(14, 14));
  /// Select CC \f$ \nu_e\to\nu_e \f$
  const TruthCut kIsBeamNue(CCFlavSel(12, 12));
  /// Select CC \f$ \nu_e\to\nu_\mu \f$
  const TruthCut kIsNumuApp(CCFlavSel(14, 12));
  /// Select CC \f$ \nu_\mu\to\nu_\tau \f$
  const TruthCut kIsTauFromMu(CCFlavSel(16, 14));
  /// Select CC \f$ \nu_e\to\nu_\tau \f$
  const TruthCut kIsTauFromE(CCFlavSel(16, 12));

  /// Is this truly an antineutrino?
  extern const TruthCut kIsAntiNu;
  extern const TruthCut kIsNu;


  /// Check if MC slice has neutrino information (useful for in-and-out tests)
  extern const Cut kHasNeutrino;
} // namespace ana
