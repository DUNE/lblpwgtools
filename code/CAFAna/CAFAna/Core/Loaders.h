#pragma once

#include "CAFAna/Core/SpectrumLoaderBase.h"

namespace caf{
  typedef int Det_t;
  const int kNEARDER = 1;
  const int kFARDET = 2;
}
//#include "StandardRecord/SRHeader.h" // For Det_t

#include <map>

namespace ana
{
  class SpectrumLoader;

  /// \brief Collection of SpectrumLoaders for many configurations
  class Loaders
  {
  public:
    enum DataMC{kData, kMC};
    //    enum SwappingConfig{kNonSwap, kFluxSwap, kTauSwap};
    enum SwappingConfig{kNonSwap, kNue, kTau, kNC};
    enum FluxType{kFHC, kRHC};

    /// No loaders initialized. Use \ref SetLoaderPath to configure
    Loaders();
    ~Loaders();

    /// Configure loader via wildcard \a path
    void SetLoaderPath(const std::string& path,
                       caf::Det_t det,
                       DataMC datamc,
                       DataSource src = kBeam,
                       SwappingConfig swap = kNonSwap);

    /// Configure loader via explicit file list
    void SetLoaderFiles(const std::vector<std::string>& files,
                        caf::Det_t det,
                        DataMC datamc,
                        DataSource src = kBeam,
                        SwappingConfig swap = kNonSwap);

    void AddLoader(SpectrumLoader*,
                        caf::Det_t det,
                        DataMC datamc,
                        DataSource src = kBeam,
                        SwappingConfig swap = kNonSwap);

    void DisableLoader(caf::Det_t det,
                       DataMC datamc,
                       DataSource src = kBeam,
                       SwappingConfig swap = kNonSwap);

    /// Retrieve a specific loader
    SpectrumLoaderBase& GetLoader(caf::Det_t det,
                                  DataMC datamc,
                                  DataSource src = kBeam,
                                  SwappingConfig swap = kNonSwap);

    /// The type of the callback functions
    typedef void (CallbackFunc_t)();
    /// Register a function to be called when Go() is finished for all loaders
    ///
    /// \param func A global function, lambda, or function object. The
    ///             signature is a void function with no parameters.
    void RegisterCompletionCallback(std::function<CallbackFunc_t> func);

    /// \brief Register a member function to be called when Go() is finished
    /// for all loaders
    ///
    /// \param obj  Pointer to the object whose method to call
    /// \param meth The method to call, void, no arguments
    template<class T> void RegisterCompletionCallback(T* obj, void (T::*meth)())
    {
      // This has to be inline in the header so that the required versions get
      // instantiated when used.
      RegisterCompletionCallback(std::bind(meth, obj));
    }

    /// Call Go() on all the loaders
    void Go();

  protected:
    /// Internal function to keep track if all our loaders have gone yet
    void LoadedCallback();

    typedef std::tuple<caf::Det_t, DataMC, DataSource, SwappingConfig> Key_t;

    // Hold a list of paths that have been set
    std::map<Key_t, std::string> fLoaderPaths;
    std::map<Key_t, std::vector<std::string>> fLoaderFiles;
    // Only reify them when someone actually calls GetLoader()
    std::map<Key_t, SpectrumLoader*> fLoaders;

    /// We give this back when a loader isn't set for some configuration
    NullLoader fNull;

    unsigned int fNumChildCallbacks; ///< Variable for \ref LoadedCallback

    std::vector<std::function<CallbackFunc_t>> fCallbacks;
  };
} // namespace
