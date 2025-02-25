#pragma once

#include "duneanaobj/StandardRecord/Navigate.h"

namespace caf
{
  class SRInteraction;
  using SRInteractionProxy = caf::Proxy<caf::SRInteraction>;

  class SRTrueInteraction;
  using SRTrueInteractionProxy = caf::Proxy<caf::SRTrueInteraction>;
}

namespace ana
{
  /// \brief Find the associated true interaction for this reco interaction.
  ///
  /// \param srint       the reco interaction in question
  /// \param minOverlap  minimum fractional overlap between the reco interaction and a found true interaction
  /// \return            pointer to the matched true interaction, unless no suitable match was found, in which case, nullptr
  const caf::SRTrueInteractionProxy * MatchedTrueInteraction(const caf::SRInteractionProxy * srint, double minOverlap=0.5);
}