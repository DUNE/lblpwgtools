#pragma once

#include "CAFAna/Fit/SeedList.h"

namespace ana
{
  class IExperiment;

  /// \brief Refine an initial list of seeds by exploring stats-only fits
  ///
  /// The idea is that the experiment is fit starting at each seed point, but
  /// without any systematics. The set of unique solutions is returned, which
  /// are likely to be very promising starting points for the full systematics
  /// fit.
  ///
  /// \param dchisq_max If set, don't return any solutions more than this far
  ///                   in chisq from the best minimum found
  SeedList RefineSeeds(const SeedList& seeds,
                       const IExperiment* expt,
                       const std::vector<const IFitVar*>& vars,
                       const osc::IOscCalcAdjustable* calc,
                       double dchisq_max = -1);
}
