#pragma once

namespace ana
{
  /// Configuration parameters for the Stan MCMC fitter, bundled up together for easy storage and parameter passing
  struct StanConfig
  {
    enum class Verbosity
    {
        kSilent,
        kQuiet,
        kVerbose,
        kEverything
    };

    /// Constructor initializes to Stan's default values.  See the documentation on the members for what the parameters mean
    /// (defaults lifted from CmdStan output; see, e.g., examples in CmdStan manual, ch. 2.)
    StanConfig(unsigned int _random_seed = 4294967295,
               unsigned int _chain = 1,
               double _init_radius = 2.0,
               int _num_warmup = 1000,
               int _num_samples = 1000,
               int _num_thin = 1,
               bool _save_warmup = true,  // not Stan default, but we save warmup separately from regular samples, so no chance of getting confused
               int _refresh = 100,
               double _stepsize = 1,
               double _stepsize_jitter = 0,
               int _max_depth = 10,
               double _delta = 0.8,
               double _kappa = 0.75,
               double _gamma = 0.05,
               double _t0 = 10,
               unsigned int _init_buffer = 75,
               unsigned int _term_buffer = 50,
               unsigned int _window = 25,
               StanConfig::Verbosity _verbosity = StanConfig::Verbosity::kQuiet,
               bool _denseMassMx = false)
      : random_seed(_random_seed),
        chain(_chain),
        init_radius(_init_radius),
        num_warmup(_num_warmup),
        num_samples(_num_samples),
        num_thin(_num_thin),
        save_warmup(_save_warmup),
        refresh(_refresh),
        stepsize(_stepsize),
        stepsize_jitter(_stepsize_jitter),
        max_depth(_max_depth),
        delta(_delta),
        kappa(_kappa),
        gamma(_gamma),
        t0(_t0),
        init_buffer(_init_buffer),
        term_buffer(_term_buffer),
        window(_window),
        verbosity(_verbosity),
        denseMassMx(_denseMassMx)
      {}

    unsigned int random_seed;  ///< Random seed used by Stan internally
    unsigned int chain;        ///< Number of Markov chains to run (1 per core)
    double init_radius;        ///< Size of the range in *unconstrained* parameter space where the initial point for un-specified parameters is randomly seeded
    int num_warmup;            ///< Number of initial steps in the Markov chain (used to enter the typical set).  These are usually discarded because they may not be sampled proportionally to the likelihood.
    int num_samples;           ///< Number of steps in the Markov chain retained for analysis (after warmup).
    int num_thin;              ///< Number of Markov chain steps between saved samples when sampling.
    bool save_warmup;          ///< Save the warmup steps?
    int refresh;               ///< Number of iterations between printout updates.  Only relevant when fitting in 'verbose' mode
    double stepsize;           ///< Integrator step size used in each simulation.  Typically this is adaptive and the default value is a fine initial guess.
    double stepsize_jitter;    ///< Randomly "jitter" the step size by this fraction of the step size.  (Sometimes can help if the integrator is getting "stuck" in regions of high curvature, but usually not needed.)
    int max_depth;             ///< Depth of the binary tree used by the HMC when taking its leapfrog steps.  Note: the number of leapfrog steps taken is 2**(tree_depth), so increase with care!

    ///@{
    /// Parameters controlling the automatic MCMC step size adaptation
    /// that happens during the warmup period.
    /// See the Stan documentation for more insight on what they do
    /// and how to set them properly if you need to adjust them beyond the defaults.
    /// (The CmdStan manual has a nice quick synopsis of each; the full Stan manual
    ///  has more statistics references and explanation of what's going on if you need.
    ///  In general you probably don't want to adjust these unless you know what you're doing.)
    double delta = 0.8;            ///< Adaptation target acceptance statistic; 0 < delta < 1
    double kappa = 0.75;           ///< Adaptation relaxation exponent; kappa > 0
    double gamma = 0.05;           ///< Adaptation regularization scale; gamma > 0
    double t0 = 10;                ///< Adaptation iteration offset; t0 > 0
    unsigned int init_buffer = 75; ///< Width of initial fast adaptation interval
    unsigned int term_buffer = 50; ///< Width of final fast adaptation interval
    unsigned int window = 25;      ///< Initial width of slow adaptation interval
    ///@}

    Verbosity verbosity;     ///< How verbose do you want me to be?

    bool denseMassMx;        ///< Should the mass matrix used in HMC be diagonal (default) or dense?

  };

  /// Allow for comparing them, since kQuiet is definitely "less" verbose than kVerbose
  inline bool operator<(StanConfig::Verbosity a, StanConfig::Verbosity b)
  {
    return static_cast<int>(a) < static_cast<int>(b);
  }

  inline bool operator>(StanConfig::Verbosity a, StanConfig::Verbosity b)
  {
    return a != b && !(static_cast<int>(a) < static_cast<int>(b));
  }

}