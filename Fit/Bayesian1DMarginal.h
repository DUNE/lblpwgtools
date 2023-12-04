#pragma once

#include "TSpline.h"

#include "CAFAna/Fit/BayesianMarginal.h"

namespace ana
{
  class IFitVar;
  class MCMCSamples;

  class Bayesian1DMarginal : public BayesianMarginal
  {
    public:
      /// \brief Build a 1D marginal distribution from the output of an MCMC fit.
      ///
      /// \param samples \a MCMCSamples object from the fit.
      /// \param var   IFitVar to retain (all others will be marginalized)
      Bayesian1DMarginal(const MCMCSamples &samples, const IFitVar *var, MarginalMode mode=MarginalMode::kHistogram);

      /// \brief Build a 1D marginal distribution from the output of an MCMC fit.
      ///
      /// \param samples \a MCMCSamples object from the fit.
      /// \param syst   ISyst to retain (all others will be marginalized)
      Bayesian1DMarginal(const MCMCSamples &samples, const ISyst *syst, MarginalMode mode=MarginalMode::kHistogram);

      /// \brief Return a list of ranges of the variable where LL is above the specified quantile
      ///
      /// \param quantile  Fraction of LLs for threshold (e.g., 0.9 -> range of upper 90% of LLs)
      /// \param bins      The range of interest must be subdivided somehow to sample the kNN.  You tell me how...
      /// \return Vector of ranges (begin, end).
      std::vector<std::pair<double, double>> QuantileRanges(Quantile quantile, const Binning& bins) const;

      TH1D ToTH1(const Binning & bins) const;

    private:
      mutable std::unique_ptr<Binning> fCachedBinning;
      mutable std::unique_ptr<TH1D>    fCachedHist;
  };

}