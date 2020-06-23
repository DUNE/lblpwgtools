#pragma once

#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Fit/MinuitFitter.h"
#include "CAFAna/Fit/SeedList.h"
#include "CAFAna/Prediction/IPrediction.h"

#include "Math/Minimizer.h"

#include <chrono>
#include <memory>

class TGraph;

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

  // Default values for Profile()
  static std::map<const IFitVar*, TGraph*> empty_vars_map;
  static std::map<const ISyst*,   TGraph*> empty_syst_map;

  /// \brief \f$\chi^2\f$ scan in one variable, profiling over all others
  ///
  /// \param expt   The experiment to retrieve chisq values from
  /// \param calc   Initial values of all oscillation parametes
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
  /// \param      systSeedPts If non-empty, try fit starting at each of these
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
                  const SeedList& seedPts = SeedList(),
                  const std::vector<SystShifts>& systsSeedPts = {},
                  std::map<const IFitVar*, TGraph*>& profVarsMap = empty_vars_map,
                  std::map<const ISyst*, TGraph*>& systsMap = empty_syst_map,
                  MinuitFitter::FitOpts opts = MinuitFitter::kNormal);

  /// Forward to \ref Profile but sqrt the result for a crude significance
  TH1* SqrtProfile(const IExperiment* expt,
                   osc::IOscCalculatorAdjustable* calc,
                  const IFitVar* v,
                   int nbinsx, double minx, double maxx,
                   double minchi = -1,
                   std::vector<const IFitVar*> profVars = {},
                   std::vector<const ISyst*> profSysts = {},
                   const SeedList& seedPts = SeedList(),
                   const std::vector<SystShifts>& systsSeedPts = {},
                   std::map<const IFitVar*, TGraph*>& profVarsMap = empty_vars_map,
                   std::map<const ISyst*, TGraph*>& systsMap = empty_syst_map,
                   MinuitFitter::FitOpts opts = MinuitFitter::kNormal);

  /// \f$\chi^2\f$ scan in one variable, holding all others constant
  TH1* Slice(const IExperiment* expt,
             osc::IOscCalculatorAdjustable* calc, const IFitVar* v,
             int nbinsx, double minx, double maxx,
             double minchi = -1,
             MinuitFitter::FitOpts opts = MinuitFitter::kNormal);

  /// Forward to \ref Slice but sqrt the result for a crude significance
  TH1* SqrtSlice(const IExperiment* expt,
                 osc::IOscCalculatorAdjustable* calc, const IFitVar* v,
                 int nbinsx, double minx, double maxx, double minchi = -1,
                 MinuitFitter::FitOpts opts = MinuitFitter::kNormal);

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
                     const SeedList& seedPts = SeedList(),
                     const std::vector<SystShifts>& systsSeedPts = {},
		           bool transpose = false,
                     MinuitFitter::FitOpts opts = MinuitFitter::kNormal);

  /// \brief Intended for use on the output of \ref Profile
  ///
  /// Returns a list of all the x-coordinates at which the curve described by
  /// \a h crosses \a critVal. eg using critVal=1 will find the 1sigma lower
  /// and upper bounds.
  std::vector<double> FindCurveCrossings(TH1* h, double critVal);
}
