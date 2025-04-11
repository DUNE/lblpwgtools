#pragma once

#include "CAFAna/Core/FitMultiverse.h"

#include "CAFAna/Core/ISyst.h"

namespace ana
{
  /// \brief Equivalent to FitMultiverse, but storing experiment-specific
  /// systematic type SystT
  ///
  /// Internal implementation details: we store the systematics as FitSyst* in
  /// the base class, but since we know they can only be set by the
  /// constructors, which we control, it is safe to cast them back to the
  /// derived class on-demand.
  template<class SystT> class _Multiverse: public FitMultiverse
  {
  public:
    static_assert(std::is_base_of_v<IFitSyst, SystT>);

    /// See FitMultiverse::Hypercross
    static const _Multiverse& Hypercross(const std::vector<const SystT*>& systs,
                                         int nsigma = 3)
    {
      return Cached(FitMultiverse::Hypercross(ConvertSysts(systs), nsigma));
    }

    using FitMultiverse::kTrulyRandom;

    /// See FitMultiverse::RandomGas
    static const _Multiverse& RandomGas(const std::vector<const SystT*>& systs,
                                        int Nuniv,
                                        unsigned int seed)
    {
      return Cached(FitMultiverse::RandomGas(ConvertSysts(systs), Nuniv, seed));
    }

    // TO DO: a member that creates a new multiverse from a vector of multiverses 
    // static const _Multiverse& MergeMultiverse( Multiverse multiverse);

    /// Details of a particular universe
    const _Universe<SystT>& GetUniverse(int i) const
    {
      // TODO I think this cast is safe because the layout is identical?
      return *((const _Universe<SystT>*)&fUnivs[i]);
      //return fUnivs[i];
    }

  protected:
    /// Helper constructor for \ref Cached
    _Multiverse(const FitMultiverse&& m) : FitMultiverse(std::move(m)) {}

    /// Helper function for named constructors
    static std::vector<const IFitSyst*> ConvertSysts(const std::vector<const SystT*> systs)
    {
      return {systs.begin(), systs.end()};
    }

    /// \brief We need to retain the property that identically-defined
    /// multiverses are the same object (get the same pointers)
    ///
    /// If there is already a _Multiverse corresponding to \a m return that,
    /// otherwise construct one
    static const _Multiverse& Cached(const FitMultiverse& m)
    {
      // These will be deleted by the underlying FitMultiverse, so no need for
      // unique_ptr here
      static std::unordered_map<const FitMultiverse*, const _Multiverse*> cache;
      if(!cache.count(&m)){
        // We need to create a _Multiverse inheriting from the passed-in
        // FitMultiverse. Trouble is, we mustn't create a new FitMultiverse
        // with the same definition, so we have to move the input to become the
        // base of this new object. This is OK because 1. we only just created
        // this FitMultiverse in the named constructor and 2. FitMultiverse
        // defines a move constructor for us that takes care of Registry
        // registration etc.
        cache.emplace(&m, new _Multiverse(std::move(m)));
      }
      return *cache[&m];
    }
  };


//  using Universe = _Universe<ISyst<caf::SRInteractionProxy>>;
//  using Multiverse = _Multiverse<ISyst<caf::SRInteractionProxy>>;
//
//  using UniverseTrue = _Universe<ISyst<caf::SRTrueInteractionProxy>>;
//  using MultiverseTrue = _Multiverse<ISyst<caf::SRTrueInteractionProxy>>;
  using Universe = _Universe<ISyst>;
  using Multiverse = _Multiverse<ISyst>;
}
