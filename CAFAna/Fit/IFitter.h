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

      /// Minimization and MCMC fits return very different information,
      /// but various places need to know certain minimal things about them.
      /// MinuitFitter and BayesianFitter both contain derived versions of this interface.
      class IFitSummary
      {
        public:
          /// Is \a other a better fit than this one?
          virtual bool    IsBetterFit(const IFitSummary* other) const = 0;

          /// What's the value of the thing being minimized or maximized?  (LL, chi2, etc.)
          /// Interpretation depends on the derived class...
          virtual double  EvalMetricVal() const = 0;
      };

      /// \brief  Master fitting method.  Depends on FitHelper and FitHelperSeeded.
      ///
      /// \param[out] seed Seed parameter and output best-fit point
      /// \param[out] bestSysts Best systematics result returned here
      /// \param      seedPts List of oscillation parameter seeds
      /// \param      systSeedPts If non-empty, try fit starting at each of these
      /// \param      verb If quiet, no printout
      /// \return     -2x the log-likelihood of the best-fit point
      virtual std::unique_ptr<IFitSummary> Fit(osc::IOscCalculatorAdjustable *seed,
                                               SystShifts &bestSysts = junkShifts,
                                               const SeedList& seedPts = SeedList(),
                                               const std::vector<SystShifts>& systSeedPts = {},
                                               Verbosity verb = kVerbose) const;

      /// Variant with no seedPts
      virtual std::unique_ptr<IFitSummary> Fit(osc::IOscCalculatorAdjustable* seed,
                                               SystShifts& systSeed,
                                               Verbosity verb) const
      {
        return Fit(seed, systSeed, {}, std::vector<SystShifts>(1, systSeed), verb);
      }

      /// Variant with no seedPts and no systematics result returned
      virtual std::unique_ptr<IFitSummary> Fit(osc::IOscCalculatorAdjustable* seed,
                                               Verbosity verb) const
      {
        return Fit(seed, junkShifts, {}, {}, verb);
      }

      /// Variant with no oscillations - useful for ND fits
      virtual std::unique_ptr<IFitSummary> Fit(SystShifts& systSeed, Verbosity verb = kVerbose) const
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
      virtual std::unique_ptr<IFitSummary>
      FitHelperSeeded(osc::IOscCalculatorAdjustable *seed,
                      SystShifts &systSeed,
                      Verbosity verb) const = 0;

      /// Helper for \ref Fit
      virtual std::unique_ptr<IFitSummary>
      FitHelper(osc::IOscCalculatorAdjustable* seed,
                SystShifts& bestSysts,
                const SeedList& seedPts,
                const std::vector<SystShifts>& systSeedPts,
                Verbosity verb) const;

      /// Make any internal updates needed after each fit cycle.
      virtual void UpdatePostFit(const IFitSummary * summary) const = 0;


      /// Check that the seeds that were specified are compatible with the vars being fitted
      void ValidateSeeds(osc::IOscCalculatorAdjustable* seed,
                         const SeedList& seedPts,
                         const std::vector<SystShifts>& systSeedPts) const;

      std::vector<const IFitVar*> fVars;
      std::vector<const ISyst*> fSysts;
      mutable std::unique_ptr<SystShifts> fShifts;
      mutable Verbosity fVerb;

  };

}
