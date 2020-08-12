#pragma once

#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/SystShifts.h"

#include "CAFAna/Fit/ISurface.h"
#include "CAFAna/Fit/MinuitFitter.h"

#include "Rtypes.h"
#include "TAttMarker.h"

#include <iostream>
#include <map>

class TGraph;
class TH2;
class TH2F;

namespace ana
{
  class IExperiment;
  class IFitVar;

  /// Log-likelihood scan across two parameters
  class FrequentistSurface : public ISurface
  {
    public:
      friend class NumuSurface;
      friend class NueSurface;

      /// \param expt The experiment object to draw \f$ \chi^2 \f$ values from
      /// \param calc Values for oscillation parameters to be held fixed
      /// \param xvar Oscillation parameter to place on the x axis
      /// \param nbinsx Number of bins along x axis
      /// \param xmin Minimum value of x axis
      /// \param xmax Maximum value of x axis
      /// \param nbinsy Number of bins along y axis
      /// \param ymin Minimum value of y axis
      /// \param ymax Maximum value of y axis
      /// \param profVars Oscillation parameters to profile over
      /// \param profSysts Systematic parameters to profile over
      /// \param seedPts Try all combinations of these params as seeds
      /// \param systSeedPts Try all of these systematic combinations as seeds
      /// \param parallel Use all the cores on this machine? Be careful...
      FrequentistSurface(const IExperiment* expt,
              osc::IOscCalcAdjustable* calc,
              const IFitVar* xvar, int nbinsx, double xmin, double xmax,
              const IFitVar* yvar, int nbinsy, double ymin, double ymax,
              const std::vector<const IFitVar*>& profVars = {},
              const std::vector<const ISyst*>& profSysts = {},
              const SeedList& seedPts = SeedList(),
              const std::vector<SystShifts>& systSeedPts = {},
              bool parallel = false,
              MinuitFitter::FitOpts opts = MinuitFitter::kNormal);

        virtual ~FrequentistSurface();

        /// Maps of the values taken on by the profiled parameters
        std::vector<TH2*> GetProfiledHists() {return fProfHists;}

        void SaveTo(TDirectory* dir, const std::string& name ) const;
        static std::unique_ptr<FrequentistSurface> LoadFrom(TDirectory * dir);

    protected:
      FrequentistSurface(){};

      void CreateHistograms(const IFitVar* xvar, int nbinsx, double xmin, double xmax,
                            const IFitVar* yvar, int nbinsy, double ymin, double ymax,
                            const std::vector<const IFitVar*>& profVars,
                            const std::vector<const ISyst*>& profSysts);

      std::string ProgressBarTitle(const IFitVar* xvar, const IFitVar* yvar,
                                   const std::vector<const IFitVar*>& profVars,
                                   const std::vector<const ISyst*>& profSysts) const;

      virtual void FillSurface(const IExperiment* expt,
                               osc::IOscCalcAdjustable* calc,
                               const IFitVar* xvar, const IFitVar* yvar,
                               const std::vector<const IFitVar*>& profVars,
                               const std::vector<const ISyst*>& profSysts,
                               const SeedList& seedPts,
                               const std::vector<SystShifts>& systSeedPts);

      double FillSurfacePoint(const IExperiment* expt,
                              osc::IOscCalcAdjustable* calc,
                              const IFitVar* xvar, double x,
                              const IFitVar* yvar, double y,
                              const std::vector<const IFitVar*>& profVars,
                              const std::vector<const ISyst*>& profSysts,
                              const SeedList& seedPts,
                              const std::vector<SystShifts>& systSeedPts);

      void FindMinimum(const IExperiment* expt,
                       osc::IOscCalcAdjustable* calc,
                       const IFitVar* xvar, const IFitVar* yvar,
                       const std::vector<const IFitVar*>& profVars,
                       const std::vector<const ISyst*>& profSysts,
                       const SeedList& seedPts,
                       const std::vector<SystShifts>& systSeedPts);

      bool fParallel;

      MinuitFitter::FitOpts fFitOpts;

      // Best fit point
      std::vector<TH2*> fProfHists;
  };

  /// Up-value surface for 68% confidence in 2D in gaussian approximation
  TH2* Gaussian68Percent2D(const FrequentistSurface& s);
  /// Up-value surface for 90% confidence in 2D in gaussian approximation
  TH2* Gaussian90Percent2D(const FrequentistSurface& s);
  /// Up-value surface for 95% confidence in 2D in gaussian approximation
  TH2* Gaussian95Percent2D(const FrequentistSurface& s);
  /// Up-value surface for 2 sigma confidence in 2D in gaussian approximation
  TH2* Gaussian2Sigma2D   (const FrequentistSurface& s);
  /// Up-value surface for 99% confidence in 2D in gaussian approximation
  TH2* Gaussian99Percent2D(const FrequentistSurface& s);
  /// Up-value surface for 3 sigma confidence in 2D in gaussian approximation
  TH2* Gaussian3Sigma2D   (const FrequentistSurface& s);

  // First approximation of the correct up-values to use for ss2th13 vs delta

  /// Up-value surface for 68% confidence in 1D in gaussian approximation
  TH2* Gaussian68Percent1D(const FrequentistSurface& s);
  /// Up-value surface for 90% confidence in 1D in gaussian approximation
  TH2* Gaussian90Percent1D(const FrequentistSurface& s);
  /// Up-value surface for 95% confidence in 1D in gaussian approximation
  TH2* Gaussian95Percent1D(const FrequentistSurface& s);
  /// Up-value surface for 2 sigma confidence in 1D in gaussian approximation
  TH2* Gaussian2Sigma1D   (const FrequentistSurface& s);
  /// Up-value surface for 99% confidence in 1D in gaussian approximation
  TH2* Gaussian99Percent1D(const FrequentistSurface& s);
  /// Up-value surface for 3 sigma confidence in 1D in gaussian approximation
  TH2* Gaussian3Sigma1D   (const FrequentistSurface& s);
}
