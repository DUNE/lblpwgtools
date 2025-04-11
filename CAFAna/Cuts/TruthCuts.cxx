#include "CAFAna/Cuts/TruthCuts.h"
#include "duneanaobj/StandardRecord/Navigate.h"
#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"
#include <cassert>
#include <stdexcept>
#include "CAFAna/Core/TruthMatching.h"

// Just make sure that the cuts get built on a regular basis. Could contain
// more complex cut logic in the future.
namespace ana 
{
  const TruthCut kIsNC_NT([](const caf::SRTrueInteractionProxy* nu)
                  {
                    return !nu->iscc;
                  });
  const Cut kIsNC = TruthCutToCut(kIsNC_NT);

  const TruthCut kIsNue_NT([](const caf::SRTrueInteractionProxy* nu)
                   {
                     return abs(nu->pdg) == 12;
                   });
  const Cut kIsNue = TruthCutToCut(kIsNue_NT);

  const TruthCut kIsNumu_NT([](const caf::SRTrueInteractionProxy* nu)
                    {
                      return abs(nu->pdg) == 14;
                    });
  const Cut kIsNumu = TruthCutToCut(kIsNumu_NT);

  const TruthCut kIsNutau_NT([](const caf::SRTrueInteractionProxy* nu)
                    {
                      return abs(nu->pdg) == 16;
                    });
  const Cut kIsNutau = TruthCutToCut(kIsNutau_NT);


  bool CCFlavSel::operator()(const caf::SRTrueInteractionProxy* nu) const
  {
    return (nu->iscc &&
            abs(nu->pdg) == fPdg &&
            abs(nu->pdgorig) == fPdgOrig);
  }

  bool CCFlavSel::operator()(const caf::SRInteractionProxy* srint) const
  {
    if (!kHasNeutrino(srint))
      throw std::runtime_error("CCFlavSel::operator(): no true neutrinos, can't determine true flavor");

    if (srint->truthOverlap[0] < 0.5)
      throw std::runtime_error("CCFlavSel::operator(): best-matched neutrino is < 50% of interaction, can't determine true flavor");

    return (*this)(MatchedTrueInteraction(srint));
  }

  const Cut kIsNueApp    = TruthCutToCut(kIsNueApp_NT);
  const Cut kIsNumuCC    = TruthCutToCut(kIsNumuCC_NT);
  const Cut kIsBeamNue   = TruthCutToCut(kIsBeamNue_NT);
  const Cut kIsNumuApp   = TruthCutToCut(kIsNumuApp_NT);
  const Cut kIsTauFromMu = TruthCutToCut(kIsTauFromMu_NT);
  const Cut kIsTauFromE  = TruthCutToCut(kIsTauFromE_NT);


  const TruthCut kIsAntiNu_NT([](const caf::SRTrueInteractionProxy* nu)
                      {
                        return nu->pdg < 0;
                      });
  const Cut kIsAntiNu = TruthCutToCut(kIsAntiNu_NT);

  const TruthCut kIsNu_NT([](const caf::SRTrueInteractionProxy* nu)
                  {
                    return nu->pdg > 0;
                  });
  const Cut kIsNu = TruthCutToCut(kIsNu_NT);

  const Cut kHasNeutrino(
      [](const caf::SRInteractionProxy* sr)
      { return (sr->truth.size() > 0); }
  );


}