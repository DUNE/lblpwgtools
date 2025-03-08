#pragma once

#include "CAFAna/Core/IRecordSource.h"
#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Utils/EnumIter.h"

#include <map>

#include "IRecordSource.h"

namespace ana
{
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
    void AddSource(std::unique_ptr<SrcT>&& src, KeyTypes ... kt);

    /// Add a new non-owned source
    void AddSource(SrcT * src, KeyTypes ... kt);

    void DisableSource(KeyTypes ... kt);

    /// Retrieve a specific loader
    SrcT& GetSource(KeyTypes ... kt);


    /// Chain a selector across all the known sources
    template<class T> auto& operator[](const T& x)
    {
      using SourceType = std::remove_reference_t<decltype((*fSourceViews.begin()->second)[x])>;
      static std::unique_ptr<Sources<SourceType, KeyTypes...>> ret;

      if (!ret)
      {
        ret = std::make_unique<Sources<SourceType, KeyTypes...>>();
        Expand<KeyTypes...>(*ret, x);
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

}