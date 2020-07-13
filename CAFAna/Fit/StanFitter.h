#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "boost/random.hpp"

#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Fit/IFitter.h"
#include "CAFAna/Fit/MCMCSamples.h"
#include "CAFAna/Fit/StanConfig.h"
#include "CAFAna/Core/StanTypedefs.h"
#include "CAFAna/Core/Utilities.h"

#include "OscLib/func/IOscCalculator.h"

#include "Utilities/func/StanUtils.h"


// note, Stan depends on Eigen and one method inherited from the interface
// has an Eigen object in its signature
// (also, note, we're here hiding some warnings that Stan & dependencies trigger)
#pragma GCC diagnostic push
#if __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#endif
#pragma GCC diagnostic ignored "-Wunused-function"
#include "Eigen/Core"
#include "stan/callbacks/interrupt.hpp"
#include "stan/callbacks/writer.hpp"
#include "stan/io/array_var_context.hpp"
#include "stan/model/prob_grad.hpp"
#pragma GCC diagnostic pop

namespace stan
{
  namespace callbacks
  {
    class logger;
    class stream_logger;
  }

  namespace io
  {
    class var_context;

    template<typename T>
    class writer;
  }

  namespace mcmc
  {
    template <typename Model, typename RNG>
    class adapt_diag_e_nuts;
  }

}

namespace ana
{
  class ISyst;

  /// Stan 'writer' callback that allows us to get ahold of the values from the sampling.
  /// Note: doesn't own its own Samples.
  class MemoryTupleWriter : public stan::callbacks::writer
  {
    public:
      enum class WhichSamples
      {
          kWarmup,
          kPostWarmup,
      };

      MemoryTupleWriter(MCMCSamples * samples, MCMCSamples * warmup = nullptr)
        : fSamples(samples), fWarmup(warmup), fWhichSamples(warmup ? WhichSamples::kWarmup : WhichSamples::kPostWarmup)
      {
        if (!samples && !warmup)
        {
          std::cerr << "MemoryTupleWriter passed null MCMCSamples for both warmup and post-warmup.  Abort" << std::endl;
          abort();
        }
      }

      // note: there are other overloads from the base class for writing other things like strings.
      // we're retaining those do-nothing versions here (since anything written that way is not useful here)
      // and only overloading the one we care about
      using stan::callbacks::writer::operator();

      void operator()(const std::vector<double>& state) override
      {
        (fWhichSamples == WhichSamples::kWarmup ? fWarmup : fSamples)->AddSample(state);
      }
      void operator()(const std::vector<std::string>& names) override
      {
        (fWhichSamples == WhichSamples::kWarmup ? fWarmup : fSamples)->SetNames(names);
      }

      template <typename Model, typename RNG>
      void SaveSamplerState(stan::mcmc::adapt_diag_e_nuts<Model, RNG> &sampler)
      {
        // this is from Stan
        sampler.write_sampler_state(*this);

        auto mcmcsamples = fWhichSamples == WhichSamples::kWarmup ? fWarmup : fSamples;
        mcmcsamples->SetHyperparams(sampler.get_nominal_stepsize(),  // the 'nominal' stepsize is updated at the end of warmup
                                    TMatrixDFromEigenMatrixXd(sampler.z().inv_e_metric_));
      }

      void         SetActiveSamples(WhichSamples s) { fWhichSamples = s; }
      WhichSamples ActiveSamples() const            { return fWhichSamples; }


    private:
      MCMCSamples * fSamples;
      MCMCSamples * fWarmup;

      enum WhichSamples fWhichSamples;

  };

  /// \brief Fitter type that bolts the Stan fitting tools onto CAFAna.
  ///
  /// Unfortunately CAFAna and Stan have different case conventions
  /// so the capitalization of the methods here is a haphazard mix :(
  class StanFitter: public IFitter, public stan::model::prob_grad
  {
    public:
      StanFitter(const IExperiment *expt,
                 std::vector<const IFitVar *> vars,
                 std::vector<const ISyst *> systs = {});

      /// \brief Return names of parameters .  (Required by Stan interface.)
      ///
      /// \param param_names      Vector of parameter names to be filled.
      /// \param include_tparams  Unused in this implementation.  (Other types of Stan models need it.)
      /// \param include_gqs      Unused in this implementation.  (Other types of Stan models need it.)
      void constrained_param_names(std::vector<std::string>& param_names,
                                   bool include_tparams = true,
                                   bool include_gqs = true) const;

      /// Override (and call) the base class version, doing some caching of the oscillation parameters
      /// to avoid the problem discussed in the doc for fParamCache
      virtual std::unique_ptr<IFitSummary> Fit(osc::IOscCalculatorAdjustable *seed,
                                               SystShifts &bestSysts = junkShifts,
                                               const SeedList& seedPts = SeedList(),
                                               const std::vector<SystShifts> &systSeedPts = {},
                                               Verbosity verb = kVerbose) const override;

      // un-hide the other versions of Fit() inherited from IFitter
      using IFitter::Fit;

       /// Return vector of dimensions of each parameter.  (Required by Stan interface.)
       ///
       /// \param dims  Return vector of dimensions.  Always 1 for each parameter in our application.
       void get_dims(std::vector<std::vector<size_t> >& dims) const
       {
         dims = std::vector<std::vector<std::size_t>>(fVars.size() + fSysts.size(), std::vector<size_t>{1});
       }

      /// Return names of parameters.  (Required by Stan interface.)
      void get_param_names(std::vector<std::string>& names) const;

      /// Implement workhorse method that actually calculates the posterior probability from Stan interface
      ///
      /// \tparam propto__
      /// \tparam jacobian__
      /// \tparam T__           Template type for "real" parameters.  (Stan uses multiple internal types to allow for matrices and other things.)
      /// \param params_real     Real-valued parameters in Stan's unconstrained parameter space.
      /// \param params_i__     Integer-valued parameters in Stan's unconstrained parameter space.
      /// \param pstream__
      /// \return
      template <bool propto__, bool jacobian__, typename T__>
      T__ log_prob(std::vector<T__>& params_real,
                   std::vector<int>& params_i__,
                   std::ostream* pstream__ = 0) const;

      /// Version with Matrix is req'd by Stan interface, but we won't use it
      template <bool propto, bool jacobian, typename T_>
      T_ log_prob(Eigen::Matrix<T_,Eigen::Dynamic,1>& params_r,
                  std::ostream* pstream = 0) const;

      /// Take the seed points and convert them into Stan's unconstrained internal space
      void transform_inits(const stan::io::var_context& context,
                           std::vector<int>& params_int,
                           std::vector<double>& params_real,
                           std::ostream* pstream__) const;

      /// Method for writing out data inherited from Stan interface
      template <typename RNG>
      void write_array(RNG& base_rng__,
                       std::vector<double>& params_real,
                       std::vector<int>& params_int,
                       std::vector<double>& vars,
                       bool include_tparams__ = true,
                       bool include_gqs__ = true,
                       std::ostream* pstream__ = 0) const;

      /// Peruse the samples the MCMC generated.  If you want to take ownership of them instead, see ExtractSamples()
      const MCMCSamples& GetSamples(MemoryTupleWriter::WhichSamples ws = MemoryTupleWriter::WhichSamples::kPostWarmup) const
      {
        return ws == MemoryTupleWriter::WhichSamples::kWarmup ? fMCMCWarmup : fMCMCSamples;
      }

      /// Extract the MCMC samples from the fitter (you own them now).
      void ExtractSamples(MCMCSamples &samples,
                          MemoryTupleWriter::WhichSamples ws = MemoryTupleWriter::WhichSamples::kPostWarmup)
      {
        samples = std::move(ws == MemoryTupleWriter::WhichSamples::kWarmup ? fMCMCWarmup : fMCMCSamples);
      };

      /// Another way of extracting the MCMC samples (so you own them now) if you prefer unique_ptrs.
      std::unique_ptr<MCMCSamples> ExtractSamples(MemoryTupleWriter::WhichSamples ws = MemoryTupleWriter::WhichSamples::kPostWarmup)
      {
        return std::make_unique<MCMCSamples>(std::move(ws == MemoryTupleWriter::WhichSamples::kWarmup ? fMCMCWarmup : fMCMCSamples));
      }

      /// Supply samples and hyperparameters from a previously run warmup of MCMC.
      /// Burden is on the user to ensure they are compatible with the sampling required.
      /// (Call this prior to calling Fit() in order to use it.)
      void ReuseWarmup(MCMCSamples && warmup);

      /// Change the config used for Stan.  See the StanConfig struct documentation for ideas
      void SetStanConfig(const StanConfig& cfg) { fStanConfig = cfg; }

      /// Run Stan's test of its auto-differentiation (comparing to a finite-differences calculation)
      void TestGradients(osc::IOscCalculatorAdjustable *seed,
                         SystShifts &systSeed) const;

      /// \brief Return names of parameters in Stan's unconstrained space.  (Required by Stan interface.)
      ///
      /// \param param_names      Vector of parameter names to be filled.
      /// \param include_tparams  Unused in this implementation.  (Other types of Stan models need it.)
      /// \param include_gqs      Unused in this implementation.  (Other types of Stan models need it.)
      void unconstrained_param_names(std::vector<std::string>& param_names,
                                     bool include_tparams = true,
                                     bool include_gqs = true) const
      {
        // other kinds of Stan models need more here, but we don't.
        std::vector<std::string> names;
        get_param_names(names);
        param_names.insert(param_names.end(), names.begin(), names.end());
      }

    private:
      class samplecounter_callback : public stan::callbacks::interrupt
      {
        public:
          samplecounter_callback(std::size_t warmupSampleCount, std::size_t regularSampleCount)
              : fWarmupCounts(warmupSampleCount),
                fRegularCounts(regularSampleCount),
                fCurrCount(0)
          {
          }

          void operator()()
          {
            if (fWarmupCounts + fRegularCounts == 0)
              return;

            if (fCurrCount < fWarmupCounts && !fProgressWarmup)
              fProgressWarmup = std::make_unique<ana::Progress>(
                    "Warming up MCMC sampler (" + std::to_string(fWarmupCounts) + " samples)");

            if (fCurrCount >= fWarmupCounts)
            {
              if (fProgressWarmup)
                fProgressWarmup->Done();

              if (!fProgressNormal)
                fProgressNormal = std::make_unique<ana::Progress>(
                    "Sampling using MCMC (" + std::to_string(fRegularCounts) + " samples)");

              if (fCurrCount - fWarmupCounts >= fRegularCounts)
                fProgressNormal->Done();
              else
                fProgressNormal->SetProgress(double(++fCurrCount - fWarmupCounts) / fRegularCounts);
            }
            else if (fWarmupCounts > 0)
              fProgressWarmup->SetProgress(double(++fCurrCount) / fWarmupCounts);
          }

        private:
          // unique_ptrs so that we can initialize as needed later
          std::unique_ptr<ana::Progress> fProgressWarmup;
          std::unique_ptr<ana::Progress> fProgressNormal;
          std::size_t fWarmupCounts;
          std::size_t fRegularCounts;
          std::size_t fCurrCount;
      };

      /// Helper function to build the initial seed point context for Stan
      stan::io::array_var_context
      BuildInitContext(osc::IOscCalculatorAdjustable *seed,
                       const SystShifts &systSeed) const;

      /// Convert a 'normal' calculator into the Stan-aware variant used internally
      void CreateCalculator(osc::IOscCalculatorAdjustable * seed) const;

      /// Unroll the parameters and stuff them into the calculator and/or syst shifts obj.
      /// Needs to be templated because Stan wants to instantiate it with <double>,
      /// even though that'll never actually be used (sigh)...
      template <typename T>
      void DecodePars(const std::vector<T>& pars, osc::IOscCalculatorAdjustableStan * calc) const
      {
        static_assert(std::is_same<T, double>::value || std::is_same<T, stan::math::var>::value,
                      "DecodePars() can only be used with double or stan::math::var");
        assert(pars.size() == fVars.size()+fSysts.size());

        if (fVars.size() > 0)
        {
          assert(calc);
          for(unsigned int i = 0; i < fVars.size(); ++i){
            auto val = pars[i];
            // sadly StanFitSupport<IConstrainedFitVar> is not a derived class StanFitSupport<IFitVar>
            auto var = dynamic_cast<const StanFitSupport<IFitVar>*>(fVars[i]);
            auto constrVar = dynamic_cast<const StanFitSupport<IConstrainedFitVar>*>(fVars[i]);
            assert(var || constrVar);
            if (constrVar)
              constrVar->SetValue(calc, val);
            else
              var->SetValue(calc, val);
          }
        }

        fShifts->ResetToNominal();
        for(unsigned int j = 0; j < fSysts.size(); ++j){
          auto val = pars[fVars.size()+j];
          fShifts->SetShift(fSysts[j], val);
        }
      }

      class StanFitSummary : public IFitSummary
      {
        public:
          StanFitSummary(double bestLL)
            : fBestLL(bestLL)
          {}

          bool IsBetterThan(const IFitSummary* other) const override
          {
            if (!other)
              return true;

            if (auto otherSummary = dynamic_cast<const StanFitSummary*>(other))
              return EvalMetricVal() > otherSummary->EvalMetricVal();
            else
            {
              assert (false && "Can't compare a StanFitSummary to another kind of IFitSummary!");
              return false;  // prevent compiler warnings
            }
          }

          /// What's the value of the thing being minimized or maximized?  (LL, chi2, etc.)
          /// Interpretation depends on the derived class...
          double  EvalMetricVal() const override
          {
            return fBestLL;
          }

        private:
          double fBestLL;
      };

      /// Implement workhorse method from IFitter interface
      std::unique_ptr<IFitSummary> FitHelperSeeded(osc::IOscCalculatorAdjustable *seed,
                                                   SystShifts &systSeed,
                                                   Verbosity verb) const override;

      /// Run Stan with HMC.
      /// Lots of copy-paste from stan::services::sample::hmc_nuts_diag_e_adapt()
      /// (in stan/services/sample/hmc_nuts_diag_e_adapt.hpp)
      /// so that we can customize in order to save the sampler state after warmup is finished.
      ///
      /// \param init_writer    Stream to write initialization info to
      /// \param interrupt      Object whose operator()() will be called after every sample
      /// \param diagStream     Stream to write diagnostic info to
      /// \param logger         General object to write log to
      /// \param init_context   MCMC initialization info
      /// \param procId         Process ID
      /// \return               Stan return code
      int RunHMC(stan::callbacks::writer &init_writer,
                 StanFitter::samplecounter_callback &interrupt,
                 std::ostream &diagStream,
                 const std::unique_ptr<stan::callbacks::stream_logger> &logger,
                 stan::io::array_var_context &init_context,
                 unsigned int procId) const;

      /// Run the HMC sampler explicitly.
      /// Cribbed from stan::services::run_adaptive_sampler() (in stan/services/util/run_adaptive_sampler.hpp)
      /// in order to insert save/restore state code after warmup is done.
      ///
      /// \param sampler             The MCMC sampler object to use.  (In Stan this is a templated type, but we'll only ever use this version)
      /// \param cont_vector         Starting parameter values (transformed into Stan's internal working space)
      /// \param rng                 The random number generator being used
      /// \param interrupt           Object whose operator()() will be called after every sample
      /// \param logger              General object to write log to
      /// \param diagnostic_writer   Object to write diagnostics to
      void RunSampler(stan::mcmc::adapt_diag_e_nuts<StanFitter, boost::ecuyer1988>& sampler,
                      std::vector<double>& cont_vector,
                      boost::ecuyer1988& rng,
                      stan::callbacks::interrupt& interrupt,
                      stan::callbacks::logger& logger,
                      stan::callbacks::writer& diagnostic_writer) const;

      /// Helper function that actually does the unconstraining Stan needs
      template <typename T>
      void transform_helper(const stan::io::var_context& context,
                            stan::io::writer<double>& writer,
                            const T& var) const;

      /// Req'd by IFitter interface, but we don't need it here
      void UpdatePostFit(const IFitSummary *) const override {};

      //  members below
      mutable std::unique_ptr<osc::IOscCalculatorAdjustableStan> fCalc;
      const IExperiment * fExpt;
      StanConfig  fStanConfig;                           ///< Configuration passed to Stan for fitting.   See the StanConfig struct documentation for ideas
      MCMCSamples fMCMCSamples;
      MCMCSamples fMCMCWarmup;

      /// See MemoryTupleWriter class documentation for more info.
      /// Pointer so it can be initialized lazily
      mutable std::unique_ptr<MemoryTupleWriter> fValueWriter;

      /// stan::math::var objects have one 'gotcha' associated with them:
      /// after the gradient of the log-prob is calculated, Stan internally
      /// 'recovers' the memory associated with every stan::math::var's value
      /// by calling recover_memory() (inside log_prob_grad()).
      /// This is no problem for any variables that are computed every fit iteration,
      /// like all the ones we're fitting.  But any unfitted parameters that
      /// happen to have stan::math::var type (i.e: unfitted oscillation params)
      /// get the rug yanked from under them.
      /// This cache is made at the beginning from all those values and any that aren't
      /// part of the values passed by the fitter are rewritten at the beginning of
      /// each iteration.
      mutable std::unique_ptr<osc::IOscCalculatorAdjustable> fOscCalcCache;
  };

}
