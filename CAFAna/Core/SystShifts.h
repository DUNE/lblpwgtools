#pragma once

#include "CAFAna/Core/ISyst.h"

#ifdef CAFANA_USE_STAN
#include "CAFAna/Core/StanVar.h"
#endif

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class TDirectory;

namespace ana
{
  /// Simple record of shifts applied to systematic parameters
  class SystShifts
  {
  public:
    SystShifts();
    SystShifts(const ISyst* syst, double shift);
    SystShifts(const std::map<const ISyst*, double>& shifts);
    SystShifts(const std::unordered_map<const ISyst*, double>& shifts);
#ifdef CAFANA_USE_STAN
      SystShifts(const ISyst* syst, stan::math::var shift);
      SystShifts(const std::map<const ISyst*, stan::math::var>& shifts);
#endif
    //virtual ~SystShifts() = default;

    static SystShifts Nominal() {return SystShifts();}
    static SystShifts RandomThrow(const std::vector<const ISyst*>& systs);
    //static SystShifts RandomThrow(const std::vector<const ISyst<caf::SRInteractionProxy>*>& systs);
    //static SystShifts RandomThrow(const std::vector<const ISyst<caf::SRTrueInteractionProxy>*>& systs);

    bool IsNominal() const {return fSystsDbl.empty(); }  // since there's always a 'double' copy of any stan ones too

    /// shift: 0 = nominal; +-1 = 1sigma shifts etc. Arbitrary shifts allowed
    /// set force=true to insert a syst even if the shift is 0
    void SetShift(const ISyst* syst, double shift, bool force=false);
#ifdef CAFANA_USE_STAN
    void SetShift(const ISyst* syst, stan::math::var shift);
#endif
    
    /// Templated so that Stan- and not-stan versions have the same interface.
    /// If you don't specify anything it'll just give you back the double,
    /// which is probably what you want anyway.
    /// If you know you want the Stan version write `GetShift<stan::math::var>(syst)`
    template <typename T=double>
    T GetShift(const ISyst* syst) const;

    void ResetToNominal();

    /// Penalty term for (frequentist) chi-squared fits
    double Penalty() const;


    //not sure if this is the right change
    //void Shift(caf::SRProxy* sr,
    void Shift(caf::SRInteractionProxy* sr,
               double& weight) const;


    /// Brief description of component shifts, for printing to screen
    std::string ShortName() const;
    /// Long description of component shifts, for plot labels
    std::string LatexName() const;


    /// Allow derived classes to overload so they can copy themselves
    /// in case they overload Penalty().  Used in IFitter.
    /// Note that you own the copy...
    virtual std::unique_ptr<SystShifts> Copy() const;

#ifdef CAFANA_USE_STAN
    bool HasStan(const ISyst* s) const {return fSystsStan.count(s);}
    bool HasAnyStan() const {return !fSystsStan.empty();}

    /// Prior used in Bayesian fitting.  Override as needed.
    /// If it's more efficient to calculate log(prior)
    /// explicitly, override LogPrior() as well
    virtual stan::math::var Prior() const { return 1.; }

    /// If it's more efficient to implement log(prior) directly,
    /// override this
    virtual stan::math::var LogPrior() const;
#else
    constexpr bool HasStan(const ISyst* s) const { return false; }
    constexpr bool HasAnyStan() const {return false; }
#endif

    /// SystShifts with the same set of systs should have the same ID
    int ID() const {return fID;}

    std::vector<const ISyst*> ActiveSysts() const;

    void SaveTo(TDirectory* dir, const std::string& name) const;
    static std::unique_ptr<SystShifts> LoadFrom(TDirectory* dir, const std::string& name);

  protected:
    template <typename T>
    T Clamp(const T & t, const ISyst* s);

    std::unordered_map<const ISyst*, double> fSystsDbl;
#ifdef CAFANA_USE_STAN
    mutable std::unordered_map<const ISyst*, stan::math::var> fSystsStan;
#endif

  private:
    int fID;
    /// The next unused ID
    static int fgNextID;
  };

  //----------------------------------------------------------------------
  /// Implement a Gaussian prior on syst shift pulls.
  /// (SystShifts default is uniform)
  class GaussianPriorSystShifts : public SystShifts
  {
    public:
      // inherit base constructors...
      using SystShifts::SystShifts;

      std::unique_ptr<SystShifts> Copy() const override;
#ifdef CAFANA_USE_STAN
      stan::math::var LogPrior() const override;
      stan::math::var Prior() const override;
#endif
  };

  //----------------------------------------------------------------------
  extern const SystShifts kNoShift;
}
