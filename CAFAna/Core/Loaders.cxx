#include "CAFAna/Core/Loaders.h"

#include "CAFAna/Core/Utilities.h"

#include <cassert>
#include <iostream>

namespace ana
{
  //----------------------------------------------------------------------
  template<class SrcT> Sources<SrcT>::Sources()
  {
  }

  //----------------------------------------------------------------------
  template<class SrcT> Sources<SrcT>::~Sources()
  {
    // for(auto it: fSources) delete it.second;
  }

  //----------------------------------------------------------------------
  template<class SrcT> void Sources<SrcT>::AddLoader(SrcT* src,
                                                     DataMC datamc,
                                                     SwappingConfig swap)
  {
    assert(datamc == kMC || swap == kNonSwap);

    const Key_t key(datamc, swap);

    // Clear out the old one if necessary
    DisableLoader(datamc, swap);

    fSources[key] = src;
  }

  //----------------------------------------------------------------------
  template<class SrcT> void Sources<SrcT>::DisableLoader(DataMC datamc,
                                                         SwappingConfig swap)
  {
    assert(datamc == kMC || swap == kNonSwap);

    const Key_t key(datamc, swap);

    // Clear out the current one if possible
    auto it = fSources.find(key);
    if(it != fSources.end()){
      delete it->second;
      fSources.erase(it);
    }
  }

  //----------------------------------------------------------------------
  template<class SrcT> SrcT& Sources<SrcT>::GetLoader(DataMC datamc,
                                                      SwappingConfig swap)
  {
    assert(datamc == kMC || swap == kNonSwap);

    const Key_t key(datamc, swap);

    // Look up and return. Use kNullLoader if no loader is set for this config
    auto itLoader = fSources.find(key);
    if(itLoader != fSources.end()) return *itLoader->second;

    if constexpr(std::is_same_v<SrcT, SpectrumLoader>){
      return kNullLoader;
    }
    else{
      static NullSource<typename SrcT::Record_t> null;
      return null;
    }
  }

  // Instantiate the ones we need
  template class Sources<ISRSource>;
  template class Sources<IInteractionSource>;
}