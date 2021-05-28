#pragma once

#include "CAFAna/Core/SystShifts.h"

#include <map>
#include <string>
#include <vector>

namespace ana
{
  /// Desired match type in UniverseOracle::ClosestShiftIndex
  enum class ESide{
    kAbove, kBelow, kEither
  };

  class UniverseOracle
  {
  public:
    static UniverseOracle& Instance();

    bool SystExists(const std::string& name) const;
    /// List of all known syst names
    std::vector<std::string> Systs() const;

    /// List of shifts for this syst in each universe
    const std::vector<float>& ShiftsForSyst(const std::string& name) const;
    /// Get SystShifts objects representing each universe in a multiverse
    /// consisting of the specificed systs.
    std::vector<SystShifts> ShiftsForSysts(const std::vector<const ISyst*>& systs, int nUniv) const;

    /// Which index in the weights array corresponds to this syst?
    unsigned int SystIndex(const std::string& name) const;

    /// Within that entry, which index corresponds most closely to 'shift'?
    unsigned int ClosestShiftIndex(const std::string& name,
                                   double shift,
                                   ESide side = ESide::kEither,
                                   double* trueShift = 0) const;
  protected:
    UniverseOracle();

    std::map<std::string, unsigned int> fSystIdxs;
    std::map<std::string, std::vector<float>> fShiftVals;
  };
}
