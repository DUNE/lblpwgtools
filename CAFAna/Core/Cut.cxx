#include "CAFAna/Core/Cut.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "CAFAna/Core/TruthMatching.h"

#include <iostream>
#include <stdexcept>


namespace ana
{
  Cut TruthCutToCut(const TruthCut & truthcut)
  {
    return Cut([=](const caf::SRInteractionProxy * sri)
    {
      if (sri->truth.size() < 1)
      {
        std::cerr << "No matched truth for this reco interaction, can't convert Cut to TruthCut\n";
        throw std::runtime_error("Truth is empty");
      }
      if (sri->truthOverlap[0] < 0.5)
      {
        std::cerr << "TruthCutToCut(): best-matched neutrino is < 50% of interaction, can't determine true flavor";
        throw std::runtime_error("No best-matched neutrino");
      }

      return truthcut(MatchedTrueInteraction(sri));
    });
  }

}