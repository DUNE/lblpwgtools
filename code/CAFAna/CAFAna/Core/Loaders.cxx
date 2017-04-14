#include "CAFAna/Core/Loaders.h"

#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Utilities.h"

#include <cassert>
#include <iostream>

namespace ana
{
  //----------------------------------------------------------------------
  Loaders::Loaders()
    : fNumChildCallbacks(0)
  {
  }

  //----------------------------------------------------------------------
  Loaders::~Loaders()
  {
    for(auto it: fLoaders) delete it.second;
  }

  //----------------------------------------------------------------------
  void Loaders::SetLoaderPath(const std::string& path,
                              caf::Det_t det,
                              DataMC datamc,
                              DataSource src,
                              SwappingConfig swap)
  {
    assert(datamc == kMC || swap == kNonSwap);
    assert(det == caf::kFARDET || swap == kNonSwap);
    assert(src == kBeam || swap == kNonSwap);

    const Key_t key(det, datamc, src, swap);

    // Clear out the old one if necessary
    DisableLoader(det, datamc, src, swap);

    fLoaderPaths[key] = path;
  }

  //----------------------------------------------------------------------
  void Loaders::SetLoaderFiles(const std::vector<std::string>& files,
                               caf::Det_t det,
                               DataMC datamc,
                               DataSource src,
                               SwappingConfig swap)
  {
    assert(datamc == kMC || swap == kNonSwap);
    assert(det == caf::kFARDET || swap == kNonSwap);
    assert(src == kBeam || swap == kNonSwap);

    const Key_t key(det, datamc, src, swap);

    // Clear out the old one if necessary
    DisableLoader(det, datamc, src, swap);

    fLoaderFiles[key] = files;
  }

  //----------------------------------------------------------------------
  void Loaders::AddLoader(SpectrumLoader* file,
                               caf::Det_t det,
                               DataMC datamc,
                               DataSource src,
                               SwappingConfig swap)
  {
    assert(datamc == kMC || swap == kNonSwap);
    assert(det == caf::kFARDET || swap == kNonSwap);
    assert(src == kBeam || swap == kNonSwap);

    const Key_t key(det, datamc, src, swap);

    // Clear out the old one if necessary
    DisableLoader(det, datamc, src, swap);

    fLoaders[key] = file;
  }

  //----------------------------------------------------------------------
  void Loaders::DisableLoader(caf::Det_t det,
                              DataMC datamc,
                              DataSource src,
                              SwappingConfig swap)
  {
    assert(datamc == kMC || swap == kNonSwap);
    assert(det == caf::kFARDET || swap == kNonSwap);
    assert(src == kBeam || swap == kNonSwap);

    const Key_t key(det, datamc, src, swap);

    // Clear out the current one if possible
    auto it = fLoaders.find(key);
    if(it != fLoaders.end()){
      delete it->second;
      fLoaders.erase(it);
    }

    fLoaderPaths.erase(key);
    fLoaderFiles.erase(key);
  }

  //----------------------------------------------------------------------
  SpectrumLoaderBase& Loaders::GetLoader(caf::Det_t det,
                                         DataMC datamc,
                                         DataSource src,
                                         SwappingConfig swap)
  {
    assert(datamc == kMC || swap == kNonSwap);
    assert(det == caf::kFARDET || swap == kNonSwap);
    assert(src == kBeam || swap == kNonSwap);

    const Key_t key(det, datamc, src, swap);

    // Look up and return. Use fNull if no loader is set for this config
    auto itLoader = fLoaders.find(key);
    if(itLoader != fLoaders.end()) return *itLoader->second;

    auto itPath = fLoaderPaths.find(key);
    if(itPath != fLoaderPaths.end()){
      fLoaders[key] = new SpectrumLoader(itPath->second, src);
      fLoaders[key]->RegisterCompletionCallback(this, &Loaders::LoadedCallback);
      return *fLoaders[key];
    }
    auto itFiles = fLoaderFiles.find(key);
    if(itFiles != fLoaderFiles.end()){
      fLoaders[key] = new SpectrumLoader(itFiles->second, src);
      fLoaders[key]->RegisterCompletionCallback(this, &Loaders::LoadedCallback);
      return *fLoaders[key];
    }

    return fNull;
  }

  //----------------------------------------------------------------------
  void Loaders::RegisterCompletionCallback(std::function<CallbackFunc_t> func)
  {
    fCallbacks.push_back(func);
  }

  //----------------------------------------------------------------------
  void Loaders::Go()
  {
    for(auto it: fLoaders) it.second->Go();

    // Ensure anyone who signed up to one of these gets properly notified
    fNull.Go();

    // No need to notify our callbacks here. Our LoadedCallback() should have
    // noticed we're done and taken care of it.
  }

  //----------------------------------------------------------------------
  void Loaders::LoadedCallback()
  {
    ++fNumChildCallbacks;
    // Someone got loaded more than once??
    assert(fNumChildCallbacks <= fLoaders.size());

    // Once all of our children have notified, it's time to notify people
    // waiting on us
    if(fNumChildCallbacks == fLoaders.size()){
      for(auto c: fCallbacks) c();
    }
  }
}
