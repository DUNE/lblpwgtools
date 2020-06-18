#include <string>
#include <vector>

// Stan's dependencies trigger some warnings...
#pragma GCC diagnostic push
#if __GNUC__ >= 6
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#endif
#pragma GCC diagnostic ignored "-Wunused-function"
#include "stan/callbacks/stream_logger.hpp"
#include "stan/callbacks/stream_writer.hpp"
#include "stan/io/reader.hpp"
#include "stan/io/writer.hpp"
#include "stan/services/sample/hmc_nuts_diag_e_adapt.hpp"
#include "stan/services/diagnose/diagnose.hpp"
#pragma GCC diagnostic pop

#include "CAFAna/Fit/StanFitter.h"

#include "CAFAna/Core/Utilities.h"

#include "CAFAna/Experiment/ILkhdExperiment.h"

#include "OscLib/func/IOscCalculator.h"
#include "OscLib/func/OscCalculator.h"
#include "OscLib/func/OscCalculatorPMNS.h"
#include "OscLib/func/OscCalculatorPMNSOpt.h"
#include "OscLib/func/OscCalculatorDMP.h"

#include "Utilities/func/MathUtil.h"
#include "Utilities/func/StanUtils.h"

namespace ana
{

  //----------------------------------------------------------------------
  StanFitter::StanFitter(const ILkhdExperiment *expt,
                         std::vector<const IFitVar *> vars,
                         std::vector<const ISyst *> systs)
    : IFitter(vars, systs),
      stan::model::prob_grad(fVars.size() + fSysts.size()),
      fCalc(nullptr),
      fExpt(expt),
      fMCMCSamples(vars, systs),
      fValueWriter(fMCMCSamples)
  {}

  //----------------------------------------------------------------------
  stan::io::array_var_context
  StanFitter::BuildInitContext(osc::IOscCalculatorAdjustable *seed,
                               const SystShifts &systSeed) const
  {
    std::vector<double> vals;
    std::vector<std::string> names;
    if (fStanConfig.verbosity > StanConfig::Verbosity::kQuiet)
      std::cout << "Building init context for StanFitter object.  Var vals:" << std::endl;
    for (const IFitVar *v: fVars)
    {
      if (fStanConfig.verbosity > StanConfig::Verbosity::kQuiet)
        std::cout << "   " << v->ShortName() << " = " << v->GetValue(seed) << std::endl;

      vals.push_back(util::GetValAs<double>(v->GetValue(seed)));
      names.push_back(v->ShortName());
    }
    // One way this can go wrong is if two variables have the same ShortName
    assert(vals.size() == fVars.size());
    for (const ISyst *s: fSysts)
    {
      vals.push_back(util::GetValAs<double>(systSeed.GetShift<stan::math::var>(s)));
      names.push_back(s->ShortName());
    }
    // One way this can go wrong is if two variables have the same ShortName
    assert(vals.size() == (fVars.size() + fSysts.size()) && vals.size() == names.size());

    // in principle you can pass 'array' vars,
    // and they're read from the vector of doubles
    // by counting off var_sizes[var idx] doubles from vals.
    // each of our params stands on its own though so we don't need that complication
    std::vector<std::vector<std::size_t>> var_sizes(names.size(), std::vector<size_t>{1});
    stan::io::array_var_context ret(names, vals, var_sizes);

    return std::move(ret);
  }

  //----------------------------------------------------------------------
  void StanFitter::constrained_param_names(std::vector<std::string> &param_names, bool include_tparams,
                                           bool include_gqs) const
  {
    // other kinds of Stan models need more here, but we don't.
    std::vector<std::string> names;
    get_param_names(names);
    param_names.insert(param_names.end(), names.begin(), names.end());
  }

  //----------------------------------------------------------------------
  void StanFitter::CreateCalculator(osc::IOscCalculatorAdjustable *seed) const
  {
    // default to PMNSOpt
    if (!seed || dynamic_cast<osc::OscCalculatorPMNSOpt*>(seed))
      fCalc = std::make_unique<osc::OscCalculatorPMNSOptStan>();
    else if (dynamic_cast<osc::OscCalculatorPMNS*>(seed))
      fCalc = std::make_unique<osc::OscCalculatorPMNSStan>();
    else if (dynamic_cast<osc::OscCalculatorDMP*>(seed))
      fCalc = std::make_unique<osc::OscCalculatorDMPStan>();
    else
    {
      std::cerr << "Unexpected oscillation calculator type: " << DemangledTypeName(seed) << std::endl;
      std::cerr << "No conversion to Stan-aware calculator available!" << std::endl;
      std::cerr << "Abort." << std::endl;
      abort();
    }

    if (seed)
      CopyParams(seed, fCalc.get());

  }

  //----------------------------------------------------------------------
  std::unique_ptr<IFitter::IFitSummary>
  StanFitter::Fit(osc::IOscCalculatorAdjustable *seed, SystShifts &bestSysts,
                  const SeedList& seedPts,
                  const std::vector<SystShifts> &systSeedPts, Verbosity verb) const
  {
    if (seed)
    {
      if (!fOscCalcCache)
        fOscCalcCache = std::make_unique<osc::OscCalculator>();
      *fOscCalcCache = *seed;
    }

    return IFitter::Fit(seed, bestSysts, seedPts, systSeedPts, verb);
  }


  //----------------------------------------------------------------------
  std::unique_ptr<IFitter::IFitSummary>
  StanFitter::FitHelperSeeded(osc::IOscCalculatorAdjustable *seed,
                              SystShifts &systSeed,
                              Verbosity verb) const
  {
    CreateCalculator(seed);

    fShifts = systSeed.Copy();

    // status and other stuff that get passed back & forth between us and Stan
    stan::callbacks::writer init_writer;
    samplecounter_callback interrupt(std::size_t(fStanConfig.num_warmup),
                                     std::size_t(fStanConfig.num_samples));  // creates a nice CAFAna-style Progress bar

    std::ostream nullStream(nullptr);
    std::unique_ptr<stan::callbacks::stream_logger> logger;
    switch (fStanConfig.verbosity)
    {
      // if ultraverbose mode, run all the printouts to the screen...
      case StanConfig::Verbosity::kEverything:
        logger = std::make_unique<stan::callbacks::stream_logger>(std::cout, std::cout, std::cout,
                                                                  std::cerr, std::cerr);
        break;

      // and work our way down from there
      case StanConfig::Verbosity::kVerbose:
        logger = std::make_unique<stan::callbacks::stream_logger>(nullStream, std::cout, std::cout,
                                                                  std::cerr, std::cerr);
        break;

      case StanConfig::Verbosity::kQuiet:
        logger = std::make_unique<stan::callbacks::stream_logger>(nullStream, nullStream, std::cout,
                                                                  std::cerr, std::cerr);
        break;

      // always keep errors & fatal?
      case StanConfig::Verbosity::kSilent:
        logger = std::make_unique<stan::callbacks::stream_logger>(nullStream, nullStream, nullStream,
                                                                  std::cerr, std::cerr);
        break;
    }

    // for now, just write the diagnostics to stdout.
    // we can do something fancier later...
    std::ostream & diagStream = (fStanConfig.verbosity < StanConfig::Verbosity::kQuiet) ? std::cout : nullStream;
    stan::callbacks::stream_writer diagnostic_writer(diagStream, "# ");

    // Actually run Stan!
    // this is the init vals of all the parameters
    auto init_context = BuildInitContext(seed, systSeed);

    // id: only needed when running multiple chains to combine.
    // if the grid var $PROCESS is defined, use that
    unsigned int procId = 0;
    const char* process = getenv("PROCESS");
    if(process)
      procId = std::stoul(process);

    // n.b. there are _lots_ more options for ways to call Stan but let's start here.
    //      this is an exploration using the "no-u-turn sampler" (NUTS)
    //      within the Hamiltonian MC algorithm with a simple Euclidian metric
    //      in unbounded parameter space.
    auto return_code = stan::services::sample::hmc_nuts_diag_e_adapt(
        *this,
        init_context,
        fStanConfig.random_seed,
        procId,
        fStanConfig.init_radius,
        fStanConfig.num_warmup,
        fStanConfig.num_samples,
        fStanConfig.num_thin,
        fStanConfig.save_warmup,
        fStanConfig.refresh,
        fStanConfig.stepsize,
        fStanConfig.stepsize_jitter,
        fStanConfig.max_depth,
        fStanConfig.delta,
        fStanConfig.gamma,
        fStanConfig.kappa,
        fStanConfig.t0,
        fStanConfig.init_buffer,
        fStanConfig.term_buffer,
        fStanConfig.window,
        interrupt,
        *logger,
        init_writer,
        fValueWriter,
        diagnostic_writer);

    // todo: something smarter here?  or just more output?
    // also todo: need to check the Stan diagnostics for divergences, autocorrelation, etc.
    if (return_code != 0)
      std::cerr << "warning: Stan fit did not converge..." << std::endl;

    auto bestSampleIdx = fMCMCSamples.BestFitSampleIdx();

    // Store results back to the "seed" variable
    for (auto & var : fVars)
      var->SetValue(seed, fMCMCSamples.SampleValue(var, bestSampleIdx));

    // Store systematic results back into "systSeed".
    // cast to stan-var so that we don't lose the value here
    for (const auto & syst : fSysts)
      systSeed.SetShift(syst, stan::math::var(fMCMCSamples.SampleValue(syst, bestSampleIdx)));

    fMCMCSamples.RunDiagnostics(fStanConfig);

    return std::make_unique<StanFitSummary>(fMCMCSamples.SampleLL(bestSampleIdx));
  } // StanFitter::Fit()

  //----------------------------------------------------------------------
  void StanFitter::get_param_names(std::vector<std::string>& names) const
  {
    names.resize(0);
    for (const auto & var : fVars)
      names.push_back(var->ShortName());
    for (const auto & syst : fSysts)
      names.push_back(syst->ShortName());
  }

  //----------------------------------------------------------------------
  template <bool propto__, bool jacobian__, typename T>
  T StanFitter::log_prob(std::vector<T>& params_real,
                         std::vector<int>& params_int,   // we don't have any integer parameters, but req'd by interface
                         std::ostream*) const
  {
    T logprob = 0;

    try
    {
      // read the model parameters.
      // they'll come out in the order they're fed in here.
      // we were careful to feed them in in the same order
      // as in the loops below, so hopefully no wires
      // get crossed anywhere...
      // (boy do I wish there were another way to do this...)
      stan::io::reader<T> reader(params_real, params_int);

      if (fStanConfig.verbosity == StanConfig::Verbosity::kEverything)
        std::cout << "-----------" << std::endl;

      std::size_t numVals = fVars.size() + fSysts.size();
      std::vector<T> params;
      params.reserve(numVals);
      T val;
      for (const auto & var : fVars)
      {
        // note: fVars always contains stan::math::vars...
        auto constrVar =  dynamic_cast<const IConstrainedFitVar*>(var);
        if (constrVar)
        {
          if (jacobian__)
            // GetValAs() because the version of this function where T is double gets instantiated and is req'd by Stan
            val = reader.scalar_lub_constrain(util::GetValAs<T>(constrVar->LowLimit()),
                                              util::GetValAs<T>(constrVar->HighLimit()), logprob);
          else
            val = reader.scalar_lub_constrain(util::GetValAs<T>(constrVar->LowLimit()),
                                              util::GetValAs<T>(constrVar->HighLimit()));
        }
        else
        {
          if (jacobian__)
            val = reader.scalar_constrain(logprob);
          else
            val = reader.scalar_constrain();
        }
        if (fStanConfig.verbosity == StanConfig::Verbosity::kEverything)
          std::cout << "var " << var->ShortName() << " has val = " << util::GetValAs<double>(val) << std::endl;
        params.push_back(val);
      }
      // systs are always unbounded
      for (std::size_t idx = 0; idx < fSysts.size(); idx++)
      {
        if (jacobian__)
          val = reader.scalar_constrain(logprob);
        else
          val = reader.scalar_constrain();

        if (fStanConfig.verbosity == StanConfig::Verbosity::kEverything)
          std::cout << "syst " << fSysts[idx]->ShortName() << " has val = " << util::GetValAs<double>(val) << std::endl;
        params.push_back(val);
      }

      if (fStanConfig.verbosity == StanConfig::Verbosity::kEverything)
        std::cout << "log-prob from re-entering constrained space = " << logprob << std::endl;

      // reset all the parameters.
      // the fitted ones will be set according to the fitted values by DecodePars().
      if (fOscCalcCache)
        CopyParams(fOscCalcCache.get(), fCalc.get());

      // applies the parameters to the vars & shifts.
      // again assumes the ordering is right... (shudder)
      DecodePars(params, fCalc.get());

      for (unsigned int i = 0; i < fVars.size(); ++i)
      {
        // sadly StanFitSupport<IConstrainedFitVar> is not a derived class StanFitSupport<IFitVar>
        auto var = dynamic_cast<const StanFitSupport<IFitVar>*>(fVars[i]);
        auto constrVar = dynamic_cast<const StanFitSupport<IConstrainedFitVar>*>(fVars[i]);
        assert(var || constrVar);
        auto prior = constrVar ? constrVar->LogPrior(params[i], fCalc.get())
                               : var->LogPrior(params[i], fCalc.get());
        if (fStanConfig.verbosity == StanConfig::Verbosity::kEverything)
        {
          std::cout << "  var '" << fVars[i]->ShortName() << "' has value " << params[i]
                    << " (in calc = " << (constrVar ? constrVar->GetValue(fCalc.get()) : var->GetValue(fCalc.get())) << ")"
                    << " and ll from prior = " << prior << std::endl;
        }

        logprob += util::GetValAs<T>(prior);
      }
      logprob += util::GetValAs<T>(fShifts->LogPrior());
      if (fStanConfig.verbosity == StanConfig::Verbosity::kEverything)
        std::cout << "log-prob from syst prior " << util::GetValAs<double>(fShifts->LogPrior()) << std::endl;

      auto ll = fExpt->LogLikelihood(fCalc.get(), *fShifts);
      if (fStanConfig.verbosity == StanConfig::Verbosity::kEverything)
        std::cout << "log-prob from spectrum comparison = " << ll << std::endl;
      logprob += util::GetValAs<T>(ll);

    }
    catch (const std::exception& e)
    {
      // just catch to add a bit more info to make it easier to find
      std::cerr << "Exception during evaluation of likelihood in Stan fitting (see StanFitter::log_prob()): " << std::endl;
      std::cerr << e.what();
      throw;
    }

    if (fStanConfig.verbosity == StanConfig::Verbosity::kEverything)
      std::cout << "accumulated prob = " << logprob << std::endl;

    // Stan will "recover memory" (i.e., clear the var cache)
    // after we're done here.
    // Invalidate the cache so that it can't be re-used.
    fCalc->InvalidateCache();

    return logprob;

  } // StanFitter::log_prob()

  //----------------------------------------------------------------------
  template <bool propto, bool jacobian, typename T_>
  T_ StanFitter::log_prob(Eigen::Matrix<T_,Eigen::Dynamic,1>& params_r,
                          std::ostream* pstream) const
  {
    std::vector<T_> vec_params_r;
    vec_params_r.reserve(params_r.size());
    for (int i = 0; i < params_r.size(); ++i)
      vec_params_r.push_back(params_r(i));
    std::vector<int> vec_params_i;
    return log_prob<propto,jacobian,T_>(vec_params_r, vec_params_i, pstream);
  }


  //----------------------------------------------------------------------
  void StanFitter::TestGradients(osc::IOscCalculatorAdjustable *seed,
                                 SystShifts &systSeed) const
  {
    if (seed)
    {
      if (!fOscCalcCache)
        fOscCalcCache = std::make_unique<osc::OscCalculator>();
      *fOscCalcCache = *seed;
    }

    CreateCalculator(seed);
    fShifts = systSeed.Copy();

    // status and other stuff that get passed back & forth between us and Stan
    stan::callbacks::writer init_writer;

    stan::callbacks::interrupt interrupt;

    // always go to screen since this is always a test
    stan::callbacks::stream_logger logger(std::cout, std::cout, std::cout, std::cerr, std::cerr);
    stan::callbacks::stream_writer diagnostic_writer(std::cout, "# ");

    // this is the init vals of all the parameters
    auto init_context = BuildInitContext(seed, systSeed);

    // diagnostic mode, where the model's gradients calculated via Stan's autodiff
    // are compared to those from finite difference calculations
    const double error = 1e-6;
    auto return_code = stan::services::diagnose::diagnose(*this,
                                                          init_context,
                                                          fStanConfig.random_seed,
                                                          0,
                                                          fStanConfig.init_radius,
                                                          1e-6,  // epsilon (finite diff step size) -- see CmdStan manual
                                                          error,
                                                          interrupt,
                                                          logger,
                                                          init_writer,
                                                          fValueWriter);
    if (return_code != 0)
      std::cerr << "warning: Stan diagnostic gradient test reports " << return_code
                << " parameters were not within " << error << " of finite diff expectation..." << std::endl;
  }

  //----------------------------------------------------------------------
  template <typename T>
  void StanFitter::transform_helper(const stan::io::var_context& context,
                                    stan::io::writer<double>& writer,
                                    const T& var) const
  {
    static_assert(std::is_same<T, const IFitVar *>::value || std::is_same<T, const ISyst *>::value,
                  "transform_helper() can only be instantiated with IFitVar* or ISyst* as the type");

    if (!(context.contains_r(var->ShortName())))
      throw std::runtime_error("variable '" + var->ShortName() + "' missing from context given to Stan!");

    std::vector<double> vals_real = context.vals_r(var->ShortName());
    double val = vals_real[0];  // we always have single-valued variables
    try
    {
      // note: T is either IFitVar* or ISyst*, and systs are always unbounded
      const IConstrainedFitVar * constrFitVar;
      if ( (constrFitVar = dynamic_cast<const IConstrainedFitVar*>(var)) )
      {
        if (fStanConfig.verbosity == StanConfig::Verbosity::kEverything)
        {
          std::cout << " transforming variable " << constrFitVar->ShortName()
                    << " from constrained between " <<  constrFitVar->LowLimit() << " and " << constrFitVar->HighLimit()
                    << " to unbounded space" << std::endl;
        }
        writer.scalar_lub_unconstrain(util::GetValAs<double>(constrFitVar->LowLimit()),
                                      util::GetValAs<double>(constrFitVar->HighLimit()), val);
      }
      else
        writer.scalar_unconstrain(val);

    }
    catch (const std::exception& e)
    {
      throw std::runtime_error(std::string("Error transforming variable '" + var->ShortName() + "': ") + e.what());
    }

  }

  // explicitly instantiate for the two relevant cases
  template void StanFitter::transform_helper(const stan::io::var_context&,
                                             stan::io::writer<double>&,
                                             const IFitVar * const &) const;
  template void StanFitter::transform_helper(const stan::io::var_context&,
                                             stan::io::writer<double>&,
                                             const ISyst * const &) const;

  //----------------------------------------------------------------------
  void StanFitter::transform_inits(const stan::io::var_context& context,
                                   std::vector<int>& params_int,
                                   std::vector<double>& params_real,
                                   std::ostream*) const
  {

    stan::io::writer<double> stanWriter(params_real, params_int);

    for (const auto & var : fVars)
      transform_helper(context, stanWriter, var);
    for (const auto & syst : fSysts)
      transform_helper(context, stanWriter, syst);

    params_real = stanWriter.data_r();
    params_int = stanWriter.data_i();
  }


  //----------------------------------------------------------------------
  template <typename RNG>
  void StanFitter::write_array(RNG&,
                               std::vector<double>& params_real,
                               std::vector<int>& params_int,
                               std::vector<double>& vars,
                               bool,  // these are used in autogenerated models,
                               bool,  // but aren't needed in our version
                               std::ostream*) const
  {
    vars.resize(0);
    stan::io::reader<double> paramReader(params_real, params_int);
    for (std::size_t i = 0; i < params_real.size(); i++)
    {
      const IConstrainedFitVar * constrFitVar = nullptr;
      if (i < fVars.size())
        constrFitVar = dynamic_cast<const IConstrainedFitVar*>(fVars[i]);
      double parVal;
      if (constrFitVar)
      {
        if (fStanConfig.verbosity == StanConfig::Verbosity::kEverything)
        {
          std::cout << " transforming variable " << constrFitVar->ShortName()
                    << " from unbounded space"
                    << " to constrained between " <<  constrFitVar->LowLimit() << " and " << constrFitVar->HighLimit()
                    << std::endl;
        }
        parVal = paramReader.scalar_lub_constrain(util::GetValAs<double>(constrFitVar->LowLimit()),
                                                  util::GetValAs<double>(constrFitVar->HighLimit()));
        if (fStanConfig.verbosity == StanConfig::Verbosity::kEverything)
          std::cout << "   got value = " << parVal << std::endl;
      }
      else
        parVal = paramReader.scalar_constrain();
      vars.push_back(parVal);
    }
  }


} // namespace ana
