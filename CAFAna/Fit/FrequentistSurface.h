#pragma once

#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/SystShifts.h"

#include "CAFAna/Fit/ISurface.h"
#include "CAFAna/Fit/MinuitFitter.h"
#include "CAFAna/Fit/FitAxis.h"

#include "Rtypes.h"
#include "TAttMarker.h"

#include <iostream>
#include <map>

class TFile;
class TGraph;
class TH2;
class TH2F;

namespace ana
{
  class IExperiment;
  class IFittable;
  class IFitVar;

  /// Log-likelihood scan across two parameters
  class FrequentistSurface : public ISurface
  {
  public:
    enum EGlobalMinOpt {
      kDisabled = 0,
      kSeeded = 1,
      kStandalone = 2
    };
    /// \param expt The experiment object to draw \f$ \chi^2 \f$ values from
    /// \param calc Values for oscillation parameters to be held fixed
    /// \param xax Definition of the x-axis
    /// \param yax Definition of the y-axis
    /// \param profVars Oscillation parameters to profile over
    /// \param profSysts Systematic parameters to profile over
    /// \param seedPts Try all combinations of these params as seeds
    /// \param systSeedPts Try all of these systematic combinations as seeds
    /// \param parallel Use all the cores on this machine? Be careful...
    /// \param minopt Find the global minimum as well as fill the surface?
    FrequentistSurface(const IExperiment* expt,
                       osc::IOscCalcAdjustable* calc,
                       const FitAxis& xax, const FitAxis& yax,
                       const std::vector<const IFitVar*>& profVars = {},
                       const std::vector<const ISyst*>& profSysts = {},
                       const SeedList& seedPts = SeedList(),
                       const std::vector<SystShifts>& systSeedPts = {},
                       bool parallel = false,
                       MinuitFitter::FitOpts opts = MinuitFitter::kNormal, 
                       EGlobalMinOpt minopt = kSeeded);

    /// \param expt The experiment object to draw \f$ \chi^2 \f$ values from
    /// \param calc Values for oscillation parameters to be held fixed
    /// \param xvar Oscillation parameter to place on the x axis
    /// \param yvar Oscillation parameter to place on the y axis
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
    /// \param minopt Find the global minimum as well as fill the surface?
    FrequentistSurface(const IExperiment* expt,
                       osc::IOscCalcAdjustable* calc,
                       const IFitVar* xvar, int nbinsx, double xmin, double xmax,
                       const IFitVar* yvar, int nbinsy, double ymin, double ymax,
                       const std::vector<const IFitVar*>& profVars = {},
                       const std::vector<const ISyst*>& profSysts = {},
                       const SeedList& seedPts = SeedList(),
                       const std::vector<SystShifts>& systSeedPts = {},
                       bool parallel = false,
                       MinuitFitter::FitOpts opts = MinuitFitter::kNormal,
                       EGlobalMinOpt minopt = kSeeded)
      : FrequentistSurface(expt, calc,
                           FitAxis(xvar, nbinsx, xmin, xmax, false),
                           FitAxis(yvar, nbinsy, ymin, ymax, false),
                           profVars, profSysts, seedPts, systSeedPts, parallel, opts, minopt)
    {
    }

    virtual ~FrequentistSurface();

    /// Maps of the values taken on by the profiled parameters
    std::vector<TH2*> GetProfiledHists() {return fProfHists;}
    void DrawProfHist(size_t idx) const;
    double BestFitValue(const IFittable* var);
    /// Return histogram with fit time per surface bin (in seconds)
    TH2* GetTimeHist();
    /// Get total surface fit time (in seconds)
    double Time();

    void SaveTo(TDirectory* dir, const std::string& name) const;
    static std::unique_ptr<FrequentistSurface> LoadFrom(TDirectory* dir, const std::string& name);
    static std::unique_ptr<FrequentistSurface> LoadFromMulti(
      const std::vector<TFile*> files, const std::string& label,
      bool allowFail=false, int nSeparateSeedPoints=1);
    static std::unique_ptr<FrequentistSurface> LoadFromMulti(
      const std::vector<std::string>& files, const std::string& label,
      bool allowFail=false, int nSeparateSeedPoints=1);
    static std::unique_ptr<FrequentistSurface> LoadFromMulti(
      const std::string& wildcard, const std::string& label,
      bool allowFail=false, int nSeparateSeedPoints=1);
    void SetBestFitInformation(double likelihood,
                               double bestFitX,
                               double bestFitY,
                               std::map<const IFittable*, double> bestFitVars={});

  protected:
    FrequentistSurface(){};

    FrequentistSurface(osc::IOscCalcAdjustable* calc,
                       const FitAxis& xax,
                       const FitAxis& yax,
                       const std::vector<const IFitVar*>& profVars,
                       const std::vector<const ISyst*>& profSysts,
                       bool parallel,
                       MinuitFitter::FitOpts opts,
                       EGlobalMinOpt minopt = kSeeded);
    
    void CreateHistograms(const FitAxis& xax, const FitAxis& yax,
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

    virtual double FillSurfacePoint(const IExperiment* expt,
                                    osc::IOscCalcAdjustable* calc,
                                    const IFitVar* xvar, double x,
                                    const IFitVar* yvar, double y,
                                    const std::vector<const IFitVar*>& profVars,
                                    const std::vector<const ISyst*>& profSysts,
                                    const SeedList& seedPts,
                                    const std::vector<SystShifts>& systSeedPts);

    virtual void FindMinimum(const IExperiment* expt,
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
    std::map<const IFittable*, double> fBestFitVals;

    // Fit time
    TH2* fTimeHist;

    // Do we want to find the global minimum.
    EGlobalMinOpt fMinOpt;
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
  /// Up-value surface for 4 sigma confidence in 2D in gaussian approximation
  TH2* Gaussian4Sigma2D   (const FrequentistSurface& s);
  /// Up-value surface for 5 sigma confidence in 2D in gaussian approximation
  TH2* Gaussian5Sigma2D   (const FrequentistSurface& s);

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
  /// Up-value surface for 4 sigma confidence in 1D in gaussian approximation
  TH2* Gaussian4Sigma1D   (const FrequentistSurface& s);
  /// Up-value surface for 5 sigma confidence in 1D in gaussian approximation
  TH2* Gaussian5Sigma1D   (const FrequentistSurface& s);
}
