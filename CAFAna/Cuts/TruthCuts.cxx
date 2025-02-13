#include "CAFAna/Cuts/TruthCuts.h"
#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include <cassert>
// Just make sure that the cuts get built on a regular basis. Could contain
// more complex cut logic in the future.
namespace ana 
{
  const TruthCut kIsNC([](const caf::SRTrueInteractionProxy* nu)
                  {
                    return !nu->iscc;
                  });

  const TruthCut kIsNue([](const caf::SRTrueInteractionProxy* nu)
                   {
                     return abs(nu->pdg) == 12;
                   });

  const TruthCut kIsNumu([](const caf::SRTrueInteractionProxy* nu)
                    {
                      return abs(nu->pdg) == 14;
                    });


  const TruthCut kIsNutau([](const caf::SRTrueInteractionProxy* nu)
                    {
                      return abs(nu->pdg) == 16;
                    });


  bool CCFlavSel::operator()(const caf::SRTrueInteractionProxy* nu) const
  {
    return (nu->iscc &&
            abs(nu->pdg) == fPdg &&
            abs(nu->pdgorig) == fPdgOrig);
  }


  const TruthCut kIsAntiNu([](const caf::SRTrueInteractionProxy* nu)
                      {
                        return nu->pdg < 0;
                      });

  const TruthCut kIsNu([](const caf::SRTrueInteractionProxy* nu)
                  {
                    return nu->pdg > 0;
                  });

  const Cut kHasNeutrino(
      [](const caf::SRInteractionProxy* sr)
      { return (sr->truth.size() > 0); }
  );


}