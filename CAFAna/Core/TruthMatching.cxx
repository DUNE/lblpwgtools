#include "CAFAna/Core/TruthMatching.h"

#include "duneanaobj/StandardRecord/Navigate.h"
#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

namespace ana
{
  const caf::SRTrueInteractionProxy * MatchedTrueInteraction(const caf::SRInteractionProxy * srint, double minOverlap)
  {
    if (srint->truth.size() < 1)
      return nullptr;

    // would prefer to use std::find_if, but caf::Proxy<std::vector>::iterator doesn't have all the right traits,
    // and I'm not in the mood to fix it at the moment
    int matchIdx = -1;
    for (std::size_t idx = 0; idx < srint->truthOverlap.size(); ++idx)
    {
      if (srint->truthOverlap[idx] > minOverlap)
      {
        matchIdx = idx;
        break;
      }
    }
    if (matchIdx < 0)
      return nullptr;

    return caf::FindInteraction(srint->Ancestor<caf::SRProxy>()->mc, matchIdx);
  }
}