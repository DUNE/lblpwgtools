#pragma once

#include <cassert>
#include "CAFAna/Core/Cut.h"
#include "StandardRecord/StandardRecord.h"

namespace ana
{

  /// \brief Is this a Neutral %Current event?
  ///
  /// We use uniform-initializer syntax to concisely pass the list of necessary
  /// branches. In this case the selection function is simple enough that we
  /// can include it inline as a lambda function.
  const Cut kIsNC({"dune.ccnc"},
                  [](const caf::StandardRecord* sr)
                  {
                    return sr->dune.ccnc == 1;
                  });

  //----------------------------------------------------------------------
  /// Helper for defining true CC event cuts
  class CCFlavSel
  {
  public:
    CCFlavSel(int pdg, int pdgorig) : fPdg(pdg), fPdgOrig(pdgorig)
    {
    }

    bool operator()(const caf::StandardRecord* sr) const
    {
      return sr->dune.ccnc == 0 && abs(sr->dune.beamPdg) == fPdgOrig && abs(sr->dune.neu) == fPdg;
    }
  protected:
    int fPdg, fPdgOrig;
  };

  /// List of CAF variables that are required to select on CC flavour
  const std::set<std::string> kCCFlavorReqs = {"dune.ccnc",
                                               "dune.beamPdg",
                                               "dune.neu"};

  // Finally, the function argument to the Cut constructor can be a "functor"
  // object (one with operator()). This allows similar logic but with different
  // constants to be easily duplicated.

  /// Select CC \f$ \nu_\mu\to\nu_e \f$
  const Cut kIsSig    (kCCFlavorReqs, CCFlavSel(12, 14));
  /// Select CC \f$ \nu_\mu\to\nu_\mu \f$
  const Cut kIsNumuCC (kCCFlavorReqs, CCFlavSel(14, 14));
  /// Select CC \f$ \nu_e\to\nu_e \f$
  const Cut kIsBeamNue(kCCFlavorReqs, CCFlavSel(12, 12));
  /// Select CC \f$ \nu_e\to\nu_\mu \f$
  const Cut kIsNumuApp(kCCFlavorReqs, CCFlavSel(14, 12));
  /// Select CC \f$ \nu_\mu\to\nu_\tau \f$
  const Cut kIsTauFromMu(kCCFlavorReqs, CCFlavSel(16, 14));
  /// Select CC \f$ \nu_e\to\nu_\tau \f$
  const Cut kIsTauFromE(kCCFlavorReqs, CCFlavSel(16, 12));

  /// Is this truly an antineutrino?
  const Cut kIsAntiNu({"dune.neu"},
                      [](const caf::StandardRecord* sr)
                      {
                        return sr->dune.neu < 0;
                      });

  const Cut kIsTrueFV({},
                      [](const caf::StandardRecord* sr)
                      {
                        return ( abs(sr->dune.nuvtxx_truth) < 310 &&
				 abs(sr->dune.nuvtxy_truth) < 550 &&
				 sr->dune.nuvtxz_truth >  50      &&
				 sr->dune.nuvtxz_truth < 1244 );
                      });

}
