#pragma once

#include <cassert>

#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Fit/SeedList.h"

namespace ana
{
  class IChiSqExperiment;
  class ISyst;

  extern SystShifts junkShifts;

  /// Base class for fitters.
  class IFitter
  {
    public:
      enum Verbosity{kQuiet, kVerbose, kTurbose};

      IFitter(const std::vector<const IFitVar *>& vars,
              const std::vector<const ISyst *>& systs = {});

      IFitter(const IFitter & f);

      virtual ~IFitter();

      /// \brief  Master fitting method.  Depends on FitHelper and FitHelperSeeded.
      ///
      /// \param[out] seed Seed parameter and output best-fit point
      /// \param[out] bestSysts Best systematics result returned here
      /// \param      seedPts List of oscillation parameter seeds
      /// \param      systSeedPts If non-empty, try fit starting at each of these
      /// \param      verb If quiet, no printout
      /// \return     -2x the log-likelihood of the best-fit point
      virtual double Fit(osc::IOscCalculatorAdjustable *seed,
                         SystShifts &bestSysts = junkShifts,
                         const SeedList& seedPts = SeedList(),
                         const std::vector<SystShifts>& systSeedPts = {},
                         Verbosity verb = kVerbose) const;

      /// Variant with no seedPts
      virtual double Fit(osc::IOscCalculatorAdjustable* seed,
                         SystShifts& systSeed,
                         Verbosity verb) const
      {
        return Fit(seed, systSeed, {}, std::vector<SystShifts>(1, systSeed), verb);
      }

      /// Variant with no seedPts and no systematics result returned
      virtual double Fit(osc::IOscCalculatorAdjustable* seed,
                         Verbosity verb) const
      {
        return Fit(seed, junkShifts, {}, {}, verb);
      }

      /// Variant with no oscillations - useful for ND fits
      virtual double Fit(SystShifts& systSeed, Verbosity verb = kVerbose) const
      {
        return Fit(nullptr, systSeed, {}, std::vector<SystShifts>(1, systSeed), verb);
      }


      std::unique_ptr<SystShifts> GetSystShifts() const {return fShifts->Copy();}

    protected:
      struct SeedPt
      {
        SeedPt(const Seed& f, const SystShifts& s) : fitvars(f), shift(s) {}
        Seed fitvars;
        SystShifts shift;
      };
      std::vector<SeedPt> ExpandSeeds(const SeedList& seedPts,
                                      const std::vector<SystShifts>& systSeedPts) const;

      /// Helper for \ref FitHelper -- actually does fitting.  Reimplement in derived classes
      virtual double FitHelperSeeded(osc::IOscCalculatorAdjustable *seed,
                                     SystShifts &systSeed,
                                     Verbosity verb) const = 0;

      /// Helper for \ref Fit
      double FitHelper(osc::IOscCalculatorAdjustable* seed,
                       SystShifts& bestSysts,
                       const SeedList& seedPts,
                       const std::vector<SystShifts>& systSeedPts,
                       Verbosity verb) const;


      /// Check that the seeds that were specified are compatible with the vars being fitted
      void ValidateSeeds(osc::IOscCalculatorAdjustable* seed,
                         const SeedList& seedPts,
                         const std::vector<SystShifts>& systSeedPts) const;

      std::vector<const IFitVar*> fVars;
      std::vector<const ISyst*> fSysts;
      mutable std::unique_ptr<SystShifts> fShifts;

  };

}
