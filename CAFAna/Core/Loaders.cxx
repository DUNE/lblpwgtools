#include "CAFAna/Core/Loaders.h"

namespace ana
{
  Loaders::operator InteractionSources&()
  {
    if (fRecoIxnType == RecoType::kUnknown)
    {
      std::cerr << "You must configure a Reco Interaction type when you build your Loaders in order to iterate over Interactions!\n";
      throw std::runtime_error("Reco Interaction type unspecified");
    }

    static std::unique_ptr<InteractionSources> ret;
    if (!ret)
    {
      for(const ana::DataMC & dmc : enum_range(static_cast<ana::DataMC>(0), ana::DataMC::kNumDataMCs))
      {
        for(const caf::Det_t & det : enum_range(static_cast<caf::Det_t>(0), caf::Det_t::kNumDet_ts))
        {
          for(const ana::SwappingConfig & swap : enum_range(static_cast<ana::SwappingConfig>(0), ana::SwappingConfig::kNumSwappingConfigs))
          {
            if(dmc == DataMC::kData && swap != SwappingConfig::kNonSwap) continue;
            ret->AddSource(&GetSource(dmc, det, swap).Interactions(fRecoIxnType),
                           dmc, det, swap);
          }
        }
      }
    }
    return *ret;
  } // operator InteractionSources&()

  void Loaders::Go()
  {
    for (auto it: this->fSourceViews) it.second->Go();
  }


}