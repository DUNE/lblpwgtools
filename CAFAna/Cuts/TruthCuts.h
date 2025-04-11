#pragma once

#include "CAFAna/Core/Cut.h"


namespace ana
{

  extern const TruthCut kIsNC_NT;    ///<  Is this SRTrueInteraction an NC interaction?
  extern const TruthCut kIsNue_NT;   ///<  Is this SRTrueInteraction a nue CC interaction?
  extern const TruthCut kIsNumu_NT;  ///<  Is this SRTrueInteraction a numu CC interaction?
  extern const TruthCut kIsNutau_NT; ///<  Is this SRTrueInteraction a nutau CC interaction?

  extern const Cut kIsNC;       ///<  Adapt kIsNC_NT for (reco) SRInteraction with truth matching
  extern const Cut kIsNue;      ///<  Adapt kIsNue_NT for (reco) SRInteraction with truth matching
  extern const Cut kIsNumu;     ///<  Adapt kIsNumu_NT for (reco) SRInteraction with truth matching
  extern const Cut kIsNutau;    ///<  Adapt kIsNutau_NT for (reco) SRInteraction with truth matching


  //----------------------------------------------------------------------
  /// Helper for defining true CC event cuts
  class CCFlavSel
  {
  public:
    CCFlavSel(int pdg, int pdgorig) : fPdg(pdg), fPdgOrig(pdgorig)
    {
    }

    bool operator()(const caf::SRTrueInteractionProxy* nu) const;
    bool operator()(const caf::SRInteractionProxy* nu) const;

  protected:
    int fPdg, fPdgOrig;
  };

  // Finally, the function argument to the Cut constructor can be a "functor"
  // object (one with operator()). This allows similar logic but with different
  // constants to be easily duplicated.

  /// Select CC \f$ \nu_\mu\to\nu_e \f$
  const TruthCut kIsNueApp_NT (CCFlavSel(12, 14));
  extern const Cut kIsNueApp;  // defined in .cxx
  
  /// Select CC \f$ \nu_\mu\to\nu_\mu \f$
  const TruthCut kIsNumuCC_NT (CCFlavSel(14, 14));
  extern const Cut kIsNumuCC;

  /// Select CC \f$ \nu_e\to\nu_e \f$
  const TruthCut kIsBeamNue_NT(CCFlavSel(12, 12));
  extern const Cut kIsBeamNue;

  /// Select CC \f$ \nu_e\to\nu_\mu \f$
  const TruthCut kIsNumuApp_NT(CCFlavSel(14, 12));
  extern const Cut kIsNumuApp;

  /// Select CC \f$ \nu_\mu\to\nu_\tau \f$
  const TruthCut kIsTauFromMu_NT(CCFlavSel(16, 14));
  extern const Cut kIsTauFromMu;

  /// Select CC \f$ \nu_e\to\nu_\tau \f$
  const TruthCut kIsTauFromE_NT(CCFlavSel(16, 12));
  extern const Cut kIsTauFromE;

  /// Is this truly an antineutrino?
  extern const TruthCut kIsAntiNu_NT;
  extern const Cut kIsAntiNu;

  extern const TruthCut kIsNu_NT;
  extern const Cut kIsNu;


  /// Check if MC slice has neutrino information (useful for in-and-out tests)
  extern const Cut kHasNeutrino;
} // namespace ana
