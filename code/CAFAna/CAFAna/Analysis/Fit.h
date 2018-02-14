#pragma once

#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Core/SystShifts.h"

#include "Minuit2/MnMigrad.h"
#include "Minuit2/FCNBase.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnUserParameters.h"

class TGraph;

namespace osc{class IOscCalculatorAdjustable;}

namespace ana
{
  class IExperiment;
  class IFitVar;

  /// \brief Figure-of-merit with no systematics, for binned data
  ///
  /// \param obs The observed data
  /// \param unosc A spectrum representing the null hypothesis
  /// \param pot POT to scale to. Leave at default to adopt POT from \a obs
  ///
  /// \returns Sum in quadrature over the bins
  /// \f[ \sqrt{\sum_i^{\rm bins}\left({s_i\over\sqrt{s_i+b_i}}\right)^2} \f]
  double SimpleFOM(const Spectrum& obs, const Spectrum& unosc, double pot = 0);

  /// Perform MINUIT fits in one or two dimensions
  class Fitter: public ROOT::Minuit2::FCNBase
  {
  public:
    Fitter(const IExperiment* expt,
           std::vector<const IFitVar*> vars,
           std::vector<const ISyst*> systs = {});

    enum Verbosity{kQuiet, kVerbose};
    /// \param[out] seed Seed parameter and output best-fit point
    /// \param      systSeed Seed for systematics fit
    /// \param      seedPts Set each var to each of the values. Try all
    ///                     combinations. Beware of combinatorical explosion...
    /// \param      verb If quiet, no printout
    /// \return     -2x the log-likelihood of the best-fit point
    double Fit(osc::IOscCalculatorAdjustable* seed,
               SystShifts& systSeed,
               const std::map<const IFitVar*, std::vector<double>>& seedPts = {},
               Verbosity verb = kVerbose) const;

    /// Variant with no seedPts
    double Fit(osc::IOscCalculatorAdjustable* seed,
               SystShifts& systSeed,
               Verbosity verb) const
    {
      return Fit(seed, systSeed, {}, verb);
    }

    /// Variant with no seedPts and no systematics result returned
    double Fit(osc::IOscCalculatorAdjustable* seed,
               Verbosity verb = kVerbose) const
    {
      SystShifts systs = SystShifts::Nominal();
      return Fit(seed, systs, {}, verb);
    }

    /// Variant with no oscillations - useful for ND fits
    double Fit(SystShifts& systSeed, Verbosity verb = kVerbose) const
    {
      return Fit(0, systSeed, {}, verb);
    }

    /// Evaluate the log-likelihood, as required by MINUT interface
    virtual double operator()(const std::vector<double>& pars) const;

    /// Definition of one-sigma, required by MINUIT
    virtual double Up() const {return 1;}
  protected:
    /// Helper for \ref Fit
    double FitHelper(osc::IOscCalculatorAdjustable* seed,
                     SystShifts& systSeed,
                     const std::map<const IFitVar*, std::vector<double>>& seedPts,
                     Verbosity verb) const;

    const IExperiment* fExpt;
    std::vector<const IFitVar*> fVars;
    std::vector<const ISyst*> fSysts;
    mutable osc::IOscCalculatorAdjustable* fCalc;
    mutable SystShifts fShifts;
  };

  // Default values for Profile()
  static std::map<const IFitVar*, TGraph*> empty_vars_map;
  static std::map<const ISyst*,   TGraph*> empty_syst_map;

  /// \brief \f$\chi^2\f$ scan in one variable, profiling over all others
  ///
  /// \param expt   The experiment to retrieve chisq values from
  /// \param calc   Initial values of all oscillation parameters
  /// \param v      Scan over this variable
  /// \param nbinsx Binning
  /// \param minx   Binning
  /// \param maxx   Binning
  /// \param minchi Set non-default to force a chisq value to evaluate delta
  ///               chisqs against. Useful for comparing two profiles. If set
  ///               to zero it will not zero-adjust and the axis will be
  ///               labelled without "delta"
  /// \param profVars  Profile over these variables
  /// \param profSysts Profile over these systematics
  /// \param seedPts   Set each var to each of the values. Try all
  ///                  combinations. Beware of combinatorical explosion...
  /// \param[out] profVarsMap Pass empty map. Returns best values of each var.
  /// \param[out] systsMap    Pass empty map. Returns best values of each syst.
  ///
  /// \return The best fit delta chisq as a function of \a a
  TH1* Profile(const IExperiment* expt,
	       osc::IOscCalculatorAdjustable* calc,
               const IFitVar* v,
	       int nbinsx, double minx, double maxx,
	       double minchi = -1,
               const std::vector<const IFitVar*>& profVars = {},
               const std::vector<const ISyst*>& profSysts = {},
               const std::map<const IFitVar*, std::vector<double>>& seedPts = {},
               std::map<const IFitVar*, TGraph*>& profVarsMap = empty_vars_map,
               std::map<const ISyst*, TGraph*>& systsMap = empty_syst_map);

  TH1* Profile(const IExperiment* expt,
	       osc::IOscCalculatorAdjustable* calc,
               const ISyst* s,
	       int nbinsx, double minx, double maxx,
	       double minchi = -1,
               const std::vector<const IFitVar*>& profVars = {},
               const std::vector<const ISyst*>& profSysts = {},
               const std::map<const IFitVar*, std::vector<double>>& seedPts = {},
               std::map<const IFitVar*, TGraph*>& profVarsMap = empty_vars_map,
               std::map<const ISyst*, TGraph*>& systsMap = empty_syst_map);

  /// Forward to \ref Profile but sqrt the result for a crude significance
  TH1* SqrtProfile(const IExperiment* expt,
                   osc::IOscCalculatorAdjustable* calc,
                   const IFitVar* v,
                   int nbinsx, double minx, double maxx,
                   double minchi = -1,
                   std::vector<const IFitVar*> profVars = {},
                   std::vector<const ISyst*> profSysts = {},
                   const std::map<const IFitVar*, std::vector<double>>& seedPts = {});

  /// \f$\chi^2\f$ scan in one variable, holding all others constant
  TH1* Slice(const IExperiment* expt,
             osc::IOscCalculatorAdjustable* calc, const IFitVar* v,
             int nbinsx, double minx, double maxx,
             double minchi = -1);

  /// \f$\chi^2\f$ scan in one systematic variable, holding all others constant
  TH1* Slice(const IExperiment* expt,
             osc::IOscCalculatorAdjustable* calc, const ISyst* s,
             int nbinsx, double minx, double maxx,
             double minchi = -1);

  /// Forward to \ref Slice but sqrt the result for a crude significance
  TH1* SqrtSlice(const IExperiment* expt,
                 osc::IOscCalculatorAdjustable* calc, const IFitVar* v,
                 int nbinsx, double minx, double maxx, double minchi = -1);

  /// \brief Find the minimum in one variable as a function of another
  ///
  /// \param transpose plot \a scanVar on the y axis
  TGraph* FindValley(const IExperiment* expt,
		     osc::IOscCalculatorAdjustable* calc,
		     const IFitVar& scanVar,
		     const IFitVar& fitVar,
		     int nbinsx, double xmin, double xmax,
		     const std::vector<const IFitVar*>& profVars = {},
		     const std::vector<const ISyst*>& profSysts = {},
                     const std::map<const IFitVar*, std::vector<double>>& seedPts = {},
		     bool transpose = false);

  /// \brief Intended for use on the output of \ref Profile
  ///
  /// Returns a list of all the x-coordinates at which the curve described by
  /// \a h crosses \a critVal. eg using critVal=1 will find the 1sigma lower
  /// and upper bounds.
  std::vector<double> FindCurveCrossings(TH1* h, double critVal);
}
