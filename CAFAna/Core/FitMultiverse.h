#pragma once

#include "CAFAna/Core/IFitSyst.h"

#include <unordered_map>
#include <vector>

class TDirectory;

namespace ana
{
  template<class SystT> using _Universe = std::unordered_map<const SystT*, double>;

  enum EMultiverseType{kHypercross, kRandomGas};

  using FitUniverse = _Universe<IFitSyst>;

  /// \brief Collection of "universes" (SystShifts)
  ///
  /// Two Multiverses are equivalent if-and-only-if they have the same address
  ///
  /// Universe zero is always the nominal universe (no systematic shifts)
  class FitMultiverse: public INamed
  {
  public:
    // User should not delete these though
    virtual ~FitMultiverse() {}

    /// \brief Named constructor. Scan each parameter sequentially
    ///
    /// \param systs  The list of systematic parameters to scan
    /// \param nsigma Each parameter will be scanned from -nsigma to +nsigma
    static const FitMultiverse& Hypercross(const std::vector<const IFitSyst*>& systs,
                                           int nsigma = 3);

    static const unsigned int kTrulyRandom = 123456789;

    /// \brief Named constructor. Throw all parameters as gaussians
    ///
    /// \param systs The list of systematic parameters to vary
    /// \param Nuniv Number of universes to generate
    /// \param seed  Pseudo-random seed. Pass Multiverse::kTrulyRandom for a
    ///              real random seed. Beware incompatibility between universes
    ///              with different seeds.
    static const FitMultiverse& RandomGas(const std::vector<const IFitSyst*>& systs,
                                          int Nuniv,
                                          unsigned int seed);

    /// Total number of universes, including nominal at index 0
    unsigned int NUniv() const {return fUnivs.size();}

    /// Details of a particular universe
    const FitUniverse& GetUniverse(int i) const {return fUnivs[i];}

    // TO DO: a member that returns the systematic shifts used to create this Multiverse
    //  std::vector<const IFitSyst*> GetSysts(){return }

    void SaveTo(TDirectory* dir, const std::string& name) const;

    /// Usually these return unique_ptr, but Multiverses are globally managed
    static const FitMultiverse* LoadFrom(TDirectory* dir, const std::string& name);

    /// Enum that specifies the type of multiverse
    // remove const?
    EMultiverseType MultiverseType() const {return fMultiverseType;};

  protected:
    FitMultiverse(const std::string& shortName,
                  const std::string& latexName,
                  const std::vector<FitUniverse>& univs,
                  const EMultiverseType multiverseType);

    FitMultiverse(const FitMultiverse&) = delete;

    /// Special move constructor ONLY to help derived _Multiverse classes
    FitMultiverse(const FitMultiverse&&);

    std::string Checksum() const;

    std::vector<FitUniverse> fUnivs;
    
    EMultiverseType fMultiverseType;
  };

}