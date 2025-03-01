#include "CAFAna/Core/TruthMatching.h"

#include "duneanaobj/StandardRecord/Navigate.h"
#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

namespace ana
{
  const caf::SRTrueInteractionProxy * MatchedTrueInteraction(const caf::SRInteractionProxy * srint, double minOverlap)
  {
    if (srint->truth.size() < 1)
      return nullptr;

    int matchIdx = -1;

    // todo: FD CAFs aren't filling the truthOverlap at the moment, but should be.
    //       for now, just take the first entry in the match list...
    if (srint->truthOverlap.empty())
      matchIdx = srint->truth[0];
    else
    {
      // would prefer to use std::find_if, but caf::Proxy<std::vector>::iterator doesn't have all the right traits,
      // and I'm not in the mood to fix it at the moment
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
    }

    return caf::FindInteraction(srint->Ancestor<caf::SRProxy>()->mc, matchIdx);
  }
}