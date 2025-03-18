#include "CAFAna/Core/Var.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

#include "CAFAna/Core/TruthMatching.h"

#include <iostream>
#include <stdexcept>


namespace ana
{
  Var TruthVarToVar(const TruthVar & truthvar)
  {
    return Var([=](const caf::SRInteractionProxy * sri)
    {
      if (sri->truth.size() < 1)
      {
        std::cerr << "No matched truth for this reco interaction, can't convert TruthVar to Var\n";
        throw std::runtime_error("Truth is empty");
      }
      // todo: FD CAFs apparently aren't filling truthOverlap, but they should be...
      if (sri->truthOverlap.size() > 0 && sri->truthOverlap[0] < 0.5)
      {
        std::cerr << "TruthVarToVar(): best-matched neutrino is < 50% of interaction, can't determine true flavor";
        throw std::runtime_error("No best-matched neutrino");
      }

      return truthvar(MatchedTrueInteraction(sri));
    });
  }

}