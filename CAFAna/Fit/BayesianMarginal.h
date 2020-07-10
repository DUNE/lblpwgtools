#pragma once

#include <map>

#include "TMVA/MethodKNN.h"
#include "TMVA/DataLoader.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Fit/MCMCSample.h"

class TDirectory;

namespace ana
{
  class IFitVar;
  class ISyst;
  class MCMCSamples;

  /// Type to specify quantile values, so that we don't run into floating-point matching issues
  enum class Quantile
  {
      kGaussian1Sigma,
      kGaussian2Sigma,
      kGaussian3Sigma,
      k0pc,   // the very lowest LL point
      k90pc,
      k95pc,
      k100pc  // the very highest LL point
  };

  class IFitVarOrISyst
  {
  public:
    IFitVarOrISyst(const ISyst* s )  : isyst(s), ifitvar(0) {}
    IFitVarOrISyst(const IFitVar* v) : isyst(0), ifitvar(v) {}
    const ISyst* isyst;
    const IFitVar* ifitvar;
  };

  /// The result of marginalizing over a set of Bayesian MCMC samples down to a few dimensions
  class BayesianMarginal
  {
    public:
      /// How to construct marginal distributions
      enum class MarginalMode
      {
        kHistogram,
        kLLWgtdHistogram,
        kKNN,
      };

      // See eg the statistics section of the PDG for the values.  Note these are two-tailed versions...
      static const std::vector<std::pair<Quantile, double>> QuantileUpVals;

      BayesianMarginal(const MCMCSamples &samples,
                       const std::vector<IFitVarOrISyst> & orderedNames,
                       MarginalMode mode = MarginalMode::kHistogram);

      /// Retrieve the value above which the remaining points comprise the given quantile.
      ///
      /// \param quantile  The quantile value desired
      /// \param pdf       If fMode==MarginalMode::kHistogram, the returned value
      ///                  depends on the histogram that results from the marginalization (see on return val, below).
      ///                  Thus the relevant histogram must be supplied for that mode.
      ///                  For the other modes it may be left as nullptr.
      /// \return  Interpretation of return value depends on the MarginalMode \a fMode:
      ///            * for \a MarginalMode::kHistogram, it's the associated pdf value;
      ///            * for \a MarginalMode::kLLWgtdHistogram or MarginalMode::kkNN, it's the associated LL.
      ///          In either case it should be suitable for use with the derived class methods's for this instance.
      double QuantileThreshold(Quantile quantile, const TH1 * pdf=nullptr) const;

      /// See other variant of QuantileThreshold() for discussion.
      /// This version computes the threshold for an arbitrary quantile,
      /// rather than the prestored ones, and so requires the original MCMCSamples.
      double QuantileThreshold(double quantile, const MCMCSamples & samples, const TH1 * pdf=nullptr) const;

      std::vector<const ISyst*>   Systs() const { return fSysts; }
      std::vector<const IFitVar*> Vars()  const { return fVars; }

    protected:
      BayesianMarginal();

      double EstimateLLFromKNN(const std::vector<float> &vals) const;

      /// Determine the threshold for the 'histogram' mode from the appropriate MCMC sample and given histogram
      double ThresholdFromTH1(const MCMCSample & threshSample, const TH1* pdf) const;

      /// Note that the signature here is different from usual.
      /// This version expects to be passed a derived class instance
      /// created by a derived LoadFrom() as its argument...
      static void LoadInto(BayesianMarginal * marg, TDirectory * dir);
      void SaveTo(TDirectory * dir) const;

      std::unique_ptr<TH1> ToHistogram(const std::vector<Binning> & bins) const;

      /// What mode are we using?
      MarginalMode fMode;

      /// Ref to the MCMCSamples.  Will <b>not</b> be persisted.
      const MCMCSamples * fMCMCSamples;

      /// the variables/systs to be marginalized over, in axis order (x-axis, y-axis, ...)
      std::vector<std::string> fOrderedBrNames;

    private:
      void SetupKNN(TMVA::DataLoader * loader = nullptr);

      /// The samples that are the boundaries for various commonly-used quantiles
      std::map<Quantile, MCMCSample>  fQuantileSampleMap;

      /// the kNN used for approximating the surface from the points when in kNN mode
      mutable std::unique_ptr<TMVA::MethodKNN> fkNN;
      /// Needed for kNN
      mutable std::unique_ptr<TMVA::DataLoader> fdataLoader;

      /// Variables we want to marginalize to (see also fSysts)
      std::vector<const IFitVar*> fVars;
      /// Systs we want to marginalize to (see also fVars)
      std::vector<const ISyst*> fSysts;


      static const std::string sKNNName;

  };

}
