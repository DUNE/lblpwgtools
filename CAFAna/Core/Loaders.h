#pragma once

#include "CAFAna/Core/Sources.h"

namespace caf
{
  // this needs to start at 0 or the Sources::operator[] thing below won't work.
  // similarly, last element needs to be named `kNum[data type]s`
  enum class Det_t : unsigned short { kNEARDET = 0, kFARDET, kNumDet_ts };
}

namespace ana
{

  using SRSources = Sources<ISRSource, ana::DataMC, caf::Det_t, ana::SwappingConfig>;
  using InteractionSources = Sources<IInteractionSource, ana::DataMC, caf::Det_t, ana::SwappingConfig>;

  /// A bundle of SpectrumLoaders that have the same neutrino source configuration (e.g., FHC) but differ over data/MC, detector type, swap, etc.
  class Loaders: public Sources<SpectrumLoader, ana::DataMC, caf::Det_t, ana::SwappingConfig>
  {
    public:
      Loaders(ana::RecoType ixnType = RecoType::kUnknown)
        : fRecoIxnType(ixnType)
      {}

      // todo: relying on a pre-configured Interaction type is probably not the endpoint solution
      //       (what if ND & FD use different recos?  what if I want to compare multiple? etc.)
      operator InteractionSources&();

      /// Call Go() on all the loaders
      void Go();

    private:
      ana::RecoType fRecoIxnType;  ///< which reco interactions will be passed to spectra?
  }; // class Loaders

} // namespace