#pragma once

#include "CAFAna/Core/IRecordSource.h"
#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Utils/EnumIter.h"

#include <map>

#include "IRecordSource.h"

namespace caf
{
  // this needs to start at 0 or the Sources::operator[] thing below won't work.
  // similarly, last element needs to be named `kNum[data type]s`
  enum class Det_t : unsigned short { kNEARDET = 0, kFARDET, kNumDet_ts };
}

namespace ana
{
  class SpectrumLoader;

  // these need to start at 0 or the Sources::operator[] thing below won't work
  // similarly, last element needs to be named `kNum[data type]s`
  enum class DataMC : unsigned short { kData = 0, kMC, kNumDataMCs };
  enum class SwappingConfig : unsigned short { kNonSwap = 0, kNueSwap, kNuTauSwap, kIntrinsic, kNumSwappingConfigs };
  enum class FluxType : unsigned short {kFHC = 0, kRHC, kNumFluxTypes };

  /// \brief Collection of SpectrumLoaders for many configurations
  ///
  /// \type SrcT     the source type
  /// \type KeyTypes
  template<class SrcT, typename ... KeyTypes>
  class Sources
  {
  public:
    /// No sources initialized by default. Use \ref AddSource() to configure one
    Sources() = default;
    ~Sources() = default;

    /// Hand over ownership of a new source
    void AddSource(std::unique_ptr<SrcT>&& src, KeyTypes ... kt)
    {
      this->fOwnedSources.emplace(std::make_tuple(kt...), std::move(src));
      this->AddSource(src.get(), kt...);
    }

    /// Add a new non-owned source
    void AddSource(SrcT * src, KeyTypes ... kt)
    {
      const Key_t key(kt...);

      // Clear out the old one if necessary
      DisableSource(kt...);

      fSourceViews[key] = src;
    }

    void DisableSource(KeyTypes ... kt)
    {
      const Key_t key(kt...);
      fSourceViews.erase(key);    // erase() is a no-op if the key doesn't exist
      fOwnedSources.erase(key);
    }

    /// Retrieve a specific loader
    SrcT& GetSource(KeyTypes ... kt)
    {
      const Key_t key(kt...);

      // Look up and return. Use kNullLoader if no loader is set for this config
      auto itLoader = fSourceViews.find(key);
      if(itLoader != fSourceViews.end()) return *itLoader->second;

      if constexpr(std::is_same_v<SrcT, SpectrumLoader>){
        return kNullLoader;
      }
      else{
        static NullSource<typename SrcT::Record_t> null;
        return null;
      }
    }


    /// Chain a selector across all the known sources
    template<class T> auto& operator[](const T& x)
    {
      using SourceType = std::remove_reference_t<decltype((*fSourceViews.begin()->second)[x])>;
      static std::unique_ptr<Sources<SourceType, KeyTypes...>> ret;

      if (!ret)
      {
        ret = std::make_unique<Sources<SourceType, KeyTypes...>>();
        Expand<KeyTypes...>(*ret, x);
      // for(int dmc = 0; dmc < kNumDataMCs; ++dmc){
      //   for(int swap = 0; swap < kNumSwappingConfigs; ++swap){
      //     ret->AddSource(&GetSource(DataMC(dmc), SwappingConfig(swap))[x],
      //                    DataMC(dmc), SwappingConfig(swap));
      //   }
      // }
      }

      return ret;
    }

  protected:
    // This is one of two workhorse methods that unroll the parameter pack
    // corresponding to the complete set of types in Key_t.
    // The typenames used here are:
    //    S -> the type of the Sources<> object.  (Deduced from the function signature.)
    //    X -> the type of the result of calling operator[] on this Sources object.  (Deduced from the function signature.)
    //    KeyTypesInArgs --> the types of the key args that are being passed along
    //                       (built *up* successively as the types are being unrolled),
    //                       since we need to save these args to pass to the function once we reach bottom.
    //                       These are deduced from the function signature.
    //    KeyTypesRemain --> all but the last type in this subcollection from the recursive the Key_t unrolling
    //    LastKeyType --> the last one, peeled off from the previous iteration's parameter pack
    // This version of the function will be called recurively
    // for all versions of the key type collection
    // (as types are progressively stripped out of it),
    // *except* for when it has a single type remaining---
    // at which point the other variant will get called.
    template <typename S, typename X, typename ... KeyTypesInArgs, typename ... KeyTypesRemain, typename LastKeyType>
    void Expand(S& sources, const X& x, const KeyTypesInArgs & ... prevVals)
    {
      for (const LastKeyType & t : enum_range(static_cast<LastKeyType>(0), LastKeyType::LAST_ELEM(T)))
      {
        Expand<KeyTypesRemain...>(sources, prevVals..., t);
      }
    }

    // the variant of the unwrapper method that gets called all the way at the bottom,
    // when there's only a single arg type left
    template <typename S, typename X, typename ... KeyTypesInArgs, typename LastKeyType>
    void Expand(S& sources, const X& x, const KeyTypesInArgs & ... vals)
    {
      for (const LastKeyType & t : enum_range(static_cast<LastKeyType>(0), LastKeyType::LAST_ELEM(T)))
      {
        sources.AddSource(this->GetSource(vals...)[x], vals...);
      }
    }


    typedef std::tuple<KeyTypes...> Key_t;

    std::map<Key_t, SrcT*> fSourceViews;
    std::map<Key_t, std::unique_ptr<SrcT>> fOwnedSources;
  };

  using SRSources = Sources<ISRSource, ana::DataMC, caf::Det_t, ana::FluxType, ana::SwappingConfig>;
  using InteractionSources = Sources<IInteractionSource, ana::DataMC, caf::Det_t, ana::FluxType, ana::SwappingConfig>;

  class Loaders: public Sources<SpectrumLoader, ana::DataMC, caf::Det_t, ana::FluxType, ana::SwappingConfig>
  {
    public:
      Loaders(ana::RecoType ixnType = RecoType::kUnknown)
        : fRecoIxnType(ixnType)
      {}

      // todo: relying on a pre-configured Interaction type is probably not the endpoint solution
      //       (what if ND & FD use different recos?  what if I want to compare multiple? etc.)
      operator InteractionSources&()
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
              for(const ana::FluxType & ft : enum_range(static_cast<ana::FluxType>(0), ana::FluxType::kNumFluxTypes))
              {
                for(const ana::SwappingConfig & swap : enum_range(static_cast<ana::SwappingConfig>(0), ana::SwappingConfig::kNumSwappingConfigs))
                {
                  if(dmc == DataMC::kData && swap != SwappingConfig::kNonSwap) continue;
                  ret->AddSource(&GetSource(dmc, det, ft, swap).Interactions(fRecoIxnType),
                                 dmc, det, ft, swap);
                }
              }
            }
          }
        }
        return *ret;
      } // operator InteractionSources&()

    /// Call Go() on all the loaders
    void Go()
    {
      for(auto it: fSourceViews) it.second->Go();
    }

    private:
      ana::RecoType fRecoIxnType;  ///< which reco interactions will be passed to spectra?
  }; // class Loaders

} // namespace