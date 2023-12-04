#pragma once

#include <memory>

#include "CAFAna/Fit/BayesianMarginal.h"
#include "CAFAna/Fit/ISurface.h"

namespace ana
{
  class IFitVar;
  class ISyst;
  class MCMCSamples;

  class BayesianSurface : public ISurface, public BayesianMarginal
  {
    public:
      /// \brief Build a surface from the output of an MCMC fit.
      ///
      /// \param samples \a MCMCSamples object from the fit.
      /// \param xvar Oscillation parameter to place on the x axis
      /// \param nbinsx Number of bins along x axis
      /// \param xmin Minimum value of x axis
      /// \param xmax Maximum value of x axis
      /// \param yvar Oscillation parameter to place on the y axis
      /// \param nbinsy Number of bins along y axis
      /// \param ymin Minimum value of y axis
      /// \param ymax Maximum value of y axis
      BayesianSurface(const MCMCSamples &samples, const IFitVar *xvar, int nbinsx, double xmin, double xmax,
                      const IFitVar *yvar, int nbinsy, double ymin, double ymax,
                      MarginalMode mode=MarginalMode::kHistogram);

      /// Same as other constructors, but with one IFitVar and one ISyst
      BayesianSurface(const MCMCSamples &samples, const IFitVar *xsyst, int nbinsx, double xmin, double xmax,
                      const ISyst *ysyst, int nbinsy, double ymin, double ymax,
                      MarginalMode mode=MarginalMode::kHistogram);

      /// Same as other constructors, but with one ISyst and one IFitVar
      BayesianSurface(const MCMCSamples &samples, const ISyst *xsyst, int nbinsx, double xmin, double xmax,
                      const IFitVar *ysyst, int nbinsy, double ymin, double ymax,
                      MarginalMode mode=MarginalMode::kHistogram);

      /// Same as other constructors, but with two ISysts
      BayesianSurface(const MCMCSamples &samples, const ISyst *xsyst, int nbinsx, double xmin, double xmax,
                      const ISyst *ysyst, int nbinsy, double ymin, double ymax,
                      MarginalMode mode=MarginalMode::kHistogram);

      /// Up-value surface for a given quantile (e.g. 90%)
      /// Idea: Traditional 'highest posterior density' (HPD) estimate using the PDF of the LL we made in FillSurface.
      ///       Use the "Density Quantile Approach" (R. Hyndman, Am. Statist. 50, 120):
      ///       i.e., determine the 'critical' LL for significance alpha by taking the N sampled points,
      ///       ordering them by their LL, and choosing the (1-alpha)*(N points)th point's LL as the cutoff.
      ///       (Technically this is an *estimator* for the true critical LL, but it is normally distributed around the
      ///        true value -- see Hyndman -- and can be made arbitrarily close by increasing the number of samples.
      ///        Supposedly it has lots of nice properties, for instance, exact Frequentist coverage at level 1-alpha;
      ///        see, e.g., http://astrostatistics.psu.edu/su11scma5/BayesComp11-CollectedSlides.pdf, pp. 270ff.)
      ///       Since the surface's fHist already contains estimates of the PDF in each bin,
      ///       we can just add a flat surface with the correct LL level (like is done in the Frequentist version)
      ///       to get the right contours at LL = 0 in the sum.
      /// \param   quantile  The quantile you want the surface made at (see Quantile)
      /// \return            A TH2 filled with the correct LL that can be added to fHist by GetContour() inherited from ISurface
      ///
      /// Note that this is a method instead of a free function
      /// because it needs access to the stored LL values for each quantile.
      /// There is another version you can use if you need an abitrary quantile
      /// (though it requires you to supply the MCMCSamples to determine the LL).
      TH2 *QuantileSurface(Quantile quantile) const;

      /// Create quantile surface for an arbitrary quantile.
      /// Prefer the version above when the value is one of the known quantiles
      /// as those have been stored and don't require recomputation.
      ///
      /// \param quantile    The _fractional_ quantile the surface should be made at (i.e. 90% --> 0.9).
      /// \param mcmcsamples The MCMCSamples object originally used to make this surface (must match exactly!)
      TH2 *QuantileSurface(double quantile, const MCMCSamples & mcmsamples) const;

      void SaveTo(TDirectory * dir) const;
      static std::unique_ptr<BayesianSurface> LoadFrom(TDirectory * dir, const std::string& name);

    private:
      BayesianSurface() = default;

      /// Build the TH2 from the kNN
      template <typename SystOrVar1, typename SystOrVar2>
      void BuildHist(const MCMCSamples &samples,
                     const SystOrVar1 *x, int nbinsx, double xmin, double xmax,
                     const SystOrVar2 *y, int nbinsy, double ymin, double ymax);

  };

}
