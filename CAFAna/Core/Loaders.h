#pragma once

#include "CAFAna/Core/SpectrumLoader.h"

#include <map>

namespace ana
{
  class SpectrumLoader;

  // TODO should these be in some sort of namespace?
  enum DataMC{kData, kMC, kNumDataMCs};
  enum SwappingConfig{kNonSwap, kNueSwap, kNuTauSwap, kIntrinsic, kNumSwappingConfigs};

  /// \brief Collection of SpectrumLoaders for many configurations
  template<class SrcT> class Sources
  {
  public:
    /// No loaders initialized. Use \ref SetLoaderPath to configure
    Sources();
    ~Sources();

    void AddLoader(SrcT*,
                   DataMC datamc,
                   SwappingConfig swap = kNonSwap);

    void DisableLoader(DataMC datamc,
                       SwappingConfig swap = kNonSwap);

    /// Retrieve a specific loader
    SrcT& GetLoader(DataMC datamc,
                    SwappingConfig swap = kNonSwap);

    template<class T> auto& operator[](const T& x)
    {
      auto ret = new Sources<std::remove_reference_t<decltype((*fSources.begin()->second)[x])>>;

      for(int dmc = 0; dmc < kNumDataMCs; ++dmc){
        for(int swap = 0; swap < kNumSwappingConfigs; ++swap){
          if(dmc == kData && swap != kNonSwap) continue;
          ret->AddLoader(&GetLoader(DataMC(dmc), SwappingConfig(swap))[x],
                         DataMC(dmc), SwappingConfig(swap));
        }
      }

      return *ret;
    }

  protected:
    typedef std::tuple<DataMC, SwappingConfig> Key_t;

    std::map<Key_t, SrcT*> fSources;
  };


  using SRSources = Sources<ISRSource>;
  using InteractionSources = Sources<IInteractionSource>;

  class Loaders: public Sources<SpectrumLoader>
  {
  public:
    operator InteractionSources&()
    {
      InteractionSources* ret = new InteractionSources;
      for(int dmc = 0; dmc < kNumDataMCs; ++dmc){
        for(int swap = 0; swap < kNumSwappingConfigs; ++swap){
          if(dmc == kData && swap != kNonSwap) continue;
          // using only DLP, but I guess we should have an option to use Pandora?
          ret->AddLoader(&GetLoader(DataMC(dmc), SwappingConfig(swap)).Interactions(RecoType::kDLP),
                         DataMC(dmc), SwappingConfig(swap));
        }
      }
      return *ret;
    }
    // I guess we will need an operator for Particles.. and soo on as we add more nested levels?

    /// Call Go() on all the loaders
    void Go()
    {
      for(auto it: fSources) it.second->Go();
    }
  };

} // namespace