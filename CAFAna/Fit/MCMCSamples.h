#pragma once

#include <vector>

#include "TMatrixD.h"
#include "TTree.h"

#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/StanTypedefs.h"
#include "CAFAna/Fit/BayesianMarginal.h"   // for MarginalMode enum
#include "CAFAna/Fit/MCMCSample.h"
#include "CAFAna/Fit/StanConfig.h"

namespace ana
{
  // forward declaration
  class Bayesian1DMarginal;
  class BayesianSurface;

  /// Storage for a list of MCMC samples.
  ///
  /// The internal storage is as a TTree (which simplifies persistence);
  /// for efficiency and simplicity reasons the tree branches are simply doubles
  /// (rather than MCMCSample objects), but MCMCSample objects can be obtained
  /// using the Sample() method.
  ///
  /// \param varOffset  The offset to the first Var value.  (Previous values are the LL and internal fitter vars.)
  /// \param vars       The ana::Vars passed to the fitter
  /// \param systs      The ana::ISysts passed to the fitter
  class MCMCSamples
  {
    public:
      /// Name of branch in internal TTree corresponding to the log-likelihood.
      /// Used in several places, so centralized here.
      static const std::string LOGLIKELIHOOD_BRANCH_NAME;

      struct Hyperparameters
      {
        double stepSize;
        std::unique_ptr<TMatrixD> invMetric;   // use TMatrix rather than Eigen for peristability.  unique_ptr so it can be sized later

        explicit Hyperparameters(double stepsize = std::numeric_limits<double>::signaling_NaN(),
                                 std::unique_ptr<TMatrixD> && invmetric = nullptr)
          : stepSize(stepsize), invMetric(std::move(invmetric))
        {}

        Hyperparameters(const Hyperparameters & h)
          : stepSize(h.stepSize), invMetric(h.invMetric ? std::make_unique<TMatrixD>(*h.invMetric) : nullptr)
        {}

        Hyperparameters& operator=(Hyperparameters&& other)
        {
          stepSize = std::move(other.stepSize);
          invMetric = std::move(other.invMetric);
          return *this;
        }

      };

      /// Build the MCMCSamples object.
      ///
      /// \param vars            Vars you want fitted by the MCMC
      /// \param systs           Systematics that should be marginalized
      MCMCSamples(const std::vector<const IFitVar *> &vars = {},
                  const std::vector<const ana::ISyst *> &systs = {});

      /// Move constructor
      MCMCSamples(MCMCSamples&& other);

      /// No copying for now (not interested in getting the memory semantics right)
      MCMCSamples(const MCMCSamples&) = delete;

      /// Move assignment: need to call SetupTree(), so the default move assignment operator won't work
      MCMCSamples &operator=(MCMCSamples &&other);

      /// Copying still forbidden
      MCMCSamples & operator=(const MCMCSamples&) = delete;

      void AddSample(const std::vector<double> &sample);

      /// Add the samples from \ref other into this MCMCSamples.  Warning: \ref other will be cleared!
      void AdoptSamples(MCMCSamples&& other);

      /// Retrieve the index of the best-fit sample (lowest LL), which can then be used with the SampleLL() or SampleValue() methods
      std::size_t BestFitSampleIdx() const;

      /// Discard any samples
      void Clear() { fSamples->Clear(); }

      const Hyperparameters & Hyperparams() const   { return fHyperparams; }


      /// Marginalize over all other variables to obtain a 1D profile in \a var
      Bayesian1DMarginal MarginalizeTo(const IFitVar * var,
                                       BayesianMarginal::MarginalMode marginalMode=BayesianMarginal::MarginalMode::kHistogram) const;

      /// Marginalize over all other variables to obtain a 1D profile in \a syst
      Bayesian1DMarginal MarginalizeTo(const ISyst * syst,
                                       BayesianMarginal::MarginalMode marginalMode=BayesianMarginal::MarginalMode::kHistogram) const;

      /// Marginalize over all other variables to obtain a 2D surface in \a x and \a y
      template <typename SystOrVar1, typename SystOrVar2>
      BayesianSurface MarginalizeTo(const SystOrVar1 * x, int nbinsx, double xmin, double xmax,
                                    const SystOrVar2 * y, int nbinsy, double ymin, double ymax,
                                    BayesianMarginal::MarginalMode marginalMode=BayesianMarginal::MarginalMode::kHistogram) const;

      /// Max value of this var/syst in the set
      template <typename T>
      double MaxValue(const T *var) const;

      /// Min value of this var/syst in the set
      template <typename T>
      double MinValue(const T *var) const;

      /// How many samples do we have?
      std::size_t NumSamples() const { return fSamples->GetEntries(); };

      /// Determine the LL at given quantile
      ///
      /// \param quantile e.g. 90% smallest LL of set of samples --> 0.9
      /// \return the LL at given quantile
      ///
      /// Note that the necessary ordered list is constructed and destroyed each time this method is called,
      /// so if you know you need multiple quantiles, use one of the other signatures.
      std::pair<std::size_t, double> QuantileLL(double quantile) const;

      /// Like QuantileLL(double) except that the LL vector is returned to you.
      /// If you pass a non-empty vector, that vector is assumed to already contain
      /// the sorted LL list (facilitating re-use).
      std::pair<std::size_t, double> QuantileLL(double quantile, std::vector<std::pair<std::size_t, double>>& LLs) const;

      /// Like QuantileLL(double) except requesting multiple LLs at once
      std::map<double, std::pair<std::size_t, double>> QuantileLL(const std::vector<double>& quantiles) const;

      /// Do some checks on the post-fit samples
      void RunDiagnostics(const StanConfig & cfg) const;

      /// The entire sample at index \idx.
      /// If you just want one value prefer SampleLL() or SampleValue() (less overhead).
      MCMCSample Sample(std::size_t idx) const;

      /// Get the LL for sample number \idx
      double SampleLL(std::size_t idx) const
      {
        fSamples->GetEntry(idx);
        return fEntryLL;
      }

      /// Get the value of Var \a var for sample number \idx
      double SampleValue(const IFitVar *var, std::size_t idx) const
      {
        return SampleValue(idx, var->ShortName(), VarOffset(var));
      }

      /// Get the value of Syst \a syst for sample number \idx
      double SampleValue(const ana::ISyst *syst, std::size_t idx) const
      {
        return SampleValue(idx, syst->ShortName(), VarOffset(syst));
      }

      /// Get the values of FitVars \a vars for sample number \a idx
      void SampleValues(std::size_t idx,
                        const std::vector<const ana::IFitVar *> &vars,
                        std::map<const ana::IFitVar *, double> &varVals) const;

      /// Get the values of FitVars \a vars and ISysts \a systs for sample number \a idx
      void SampleValues(std::size_t idx,
                        const std::vector<const ana::IFitVar *> &vars,
                        std::map<const ana::IFitVar *, double> &varVals,
                        const std::vector<const ana::ISyst *> &systs, std::map<const ana::ISyst *, double> &systVals) const;

      double SamplingTime() const { return fSamplingTime; }

      void SetHyperparams(double stepSize, const TMatrixD& invMassMatrix)
      {
        fHyperparams = Hyperparameters(stepSize, std::make_unique<TMatrixD>(invMassMatrix));
      }

      void SetHyperparams(Hyperparameters h)
      {
        fHyperparams = std::move(h);
      };

      void SetNames(const std::vector<std::string>& names);

      void SetSamplingTime(double s)  { fSamplingTime = s; }

      /// Get a TTree with the MCMC samples in it
      const TTree *ToTTree() const;

      /// Which Systs are sampled in these samples?
      const std::vector<const ana::ISyst *> &Systs() const { return fSysts; }

      /// Which Vars are sampled in these samples?
      const std::vector<const IFitVar *> & Vars() const { return fVars; }

      /// Save this guy to a file so we don't have to rerun the MCMC
      void SaveTo(TDirectory * dir, const std::string& name) const;

      /// Load from file
      static std::unique_ptr<MCMCSamples> LoadFrom(TDirectory * dir, const std::string& name);

    private:
      /// Internal-use constructor needed for LoadFrom()
      MCMCSamples(std::size_t offset, const std::vector<std::string> &diagBranchNames,
                  const std::vector<const IFitVar *> &vars, const std::vector<const ana::ISyst *> &systs,
                  std::unique_ptr<TTree> &tree, const Hyperparameters &hyperParams, double samplingTime);

      /// Where in fDiagnosticVals is the given diagnostic?
      std::size_t DiagOffset(const std::string& diagName) const;

      /// Internal method for getting the value of a var when you know where it is in fEntryVals
      double SampleValue(std::size_t rowIdx, const std::string & branchName, std::size_t varIdx) const;

      /// Set up the storage tree based on the branch names given us by Stan
      void ParseDiagnosticBranches(const std::vector<std::string>& names);

      /// Hook up the storage tree to our internal variables
      void SetupTree();

      /// Construct a sorted vector of the LLs of all the samples (ordered lowest to highest)
      ///
      /// \return The ordered vector
      std::vector<std::pair<std::size_t, double>> SortedLLs() const;

      /// Where in fEntryVals is the given var?
      std::size_t VarOffset(const IFitVar *var) const;
      /// Where in fEntryVals is the given syst?
      std::size_t VarOffset(const ana::ISyst * syst) const;

      std::size_t fOffset;   //< number of branches read out before the fitted parameters start
      std::vector<std::string> fDiagBranches;
      std::vector<const ana::IFitVar *> fVars;
      std::vector<const ana::ISyst *> fSysts;

      mutable std::size_t fBestFitSampleIdx;
      mutable bool fBestFitFound;

      // would prefer to keep this on the stack, but no move assignment...
      std::unique_ptr<TTree> fSamples;
      double fEntryLL;  // for use reading the TTree
      std::vector<double> fEntryVals;  // ditto
      std::vector<double> fDiagnosticVals;

      mutable Hyperparameters fHyperparams; ///< Hyperparameters deduced after adaptation, or manually set
      double fSamplingTime;                 ///< how long did we spend sampling?
  };

}
