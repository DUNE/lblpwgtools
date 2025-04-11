#include "CAFAna/Core/Sources.h"

namespace ana
{
  // ------------------------------------------------------------------------
  template<typename SrcT, typename ... KeyTypes>
  void Sources<SrcT, KeyTypes...>::AddSource(std::unique_ptr<SrcT>&& src, KeyTypes ... kt)
  {
    const Key_t key(kt...);
    auto successPair = this->fOwnedSources.try_emplace(std::make_tuple(kt...), std::move(src));
    if (!successPair.second)
    {
      std::cerr << "Sources::AddSource(): attempt to add source with same key as already existing source!  Abort.\n";
      throw std::runtime_error("Duplicate source");
    }
    this->AddSource(successPair.first->second.get(), kt...);
  }

  // ------------------------------------------------------------------------
  /// Add a new non-owned source
  template<typename SrcT, typename ... KeyTypes>
  void Sources<SrcT, KeyTypes...>::AddSource(SrcT * src, KeyTypes ... kt)
  {
    const Key_t key(kt...);

    // we don't own anything here, so we don't need to worry about disabling first, just overwrite
    fSourceViews[key] = src;
  }

  // ------------------------------------------------------------------------
  template<typename SrcT, typename ... KeyTypes>
  void Sources<SrcT, KeyTypes...>::DisableSource(KeyTypes ... kt)
  {
    const Key_t key(kt...);
    fSourceViews.erase(key);    // erase() is a no-op if the key doesn't exist
    fOwnedSources.erase(key);
  }

  // ------------------------------------------------------------------------
  /// Retrieve a specific loader
  template<typename SrcT, typename ... KeyTypes>
  SrcT& Sources<SrcT, KeyTypes...>::GetSource(KeyTypes ... kt)
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

}