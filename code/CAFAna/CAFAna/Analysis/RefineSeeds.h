#pragma once

#include "CAFAna/Analysis/SeedList.h"

namespace ana
{
  class IExperiment;

  /// \brief Refine an initial list of seeds by exploring stats-only fits
  ///
  /// The idea is that the experiment is fit starting at each seed point, but
  /// without any systematics. The set of unique solutions is returned, which
  /// are likely to be very promising starting points for the full systematics
  /// fit.
  SeedList RefineSeeds(const SeedList& seeds,
                       const IExperiment* expt,
                       const std::vector<const IFitVar*>& vars,
                       const osc::IOscCalculatorAdjustable* calc);
}
