#pragma once

#include "CAFAna/Core/Binning.h"

#include <map>

#include "TMVA/MethodKNN.h"
#include "TMVA/DataLoader.h"

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
        kKNN,
      };

      BayesianMarginal(const MCMCSamples &samples,
                       const std::vector<IFitVarOrISyst> & orderedNames,
                       MarginalMode mode = MarginalMode::kHistogram);

      double QuantileLL(Quantile quantile) const;
      double QuantileLL(double quantile, const MCMCSamples & samples) const;

      std::vector<const ISyst*>   Systs() const { return fSysts; }
      std::vector<const IFitVar*> Vars()  const { return fVars; }

    protected:
      BayesianMarginal();

      double EstimateLLFromKNN(const std::vector<float> &vals) const;

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

      /// The LLs for various commonly-used quantiles
      std::map<Quantile, double>  fQuantileLLMap;

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
