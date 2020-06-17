#pragma once

#include "CAFAna/Core/ISyst.h"

class TDirectory;

#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace ana
{
  /// Simple record of shifts applied to systematic parameters
  class SystShifts
  {
  public:
    SystShifts();
    SystShifts(const ISyst* syst, double shift);
    SystShifts(const std::map<const ISyst*, double>& shifts);

    static SystShifts Nominal(){return SystShifts();}

    bool IsNominal() const {return fSysts.empty();}

    /// Allow derived classes to overload so they can copy themselves
    /// in case they overload Penalty().  Used in IFitter.
    /// Note that you own the copy...
    virtual std::unique_ptr<SystShifts> Copy() const;

    /// Shifts are 0=nominal, -1,+1 = 1 sigma shifts
    void SetShift(const ISyst* syst, double shift);
    double GetShift(const ISyst* syst) const;
    void ResetToNominal();

    void Shift(Restorer& restore,
               caf::StandardRecord* sr,
               double& weight) const;

    /// Brief description of component shifts, for printing to screen
    std::string ShortName() const;
    /// Long description of component shifts, for plot labels
    std::string LatexName() const;

    /// SystShifts with the same set of systs should have the same ID
    int ID() const {return fID;}

    std::vector<const ISyst*> ActiveSysts() const;

    void SaveTo(TDirectory* dir, const std::string& name) const;
    std::unique_ptr<SystShifts> LoadFrom(TDirectory* dir, const std::string& naem);

  protected:
    std::unordered_map<const ISyst*, double> fSysts;

    double Clamp(double x, const ISyst* s);

    int fID;
    /// The next unused ID
    static int fgNextID;
  };

  const SystShifts kNoShift = SystShifts::Nominal();
}
