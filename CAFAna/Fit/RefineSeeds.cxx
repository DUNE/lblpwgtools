#include "CAFAna/Fit/RefineSeeds.h"

#include "CAFAna/Core/IFitVar.h"
#include "CAFAna/Fit/MinuitFitter.h"

#include "OscLib/IOscCalc.h"

#include "CAFAna/Core/MathUtil.h"

#include <algorithm>

namespace ana
{
  // --------------------------------------------------------------------------
  SeedList RefineSeeds(const SeedList& seeds,
                       const IExperiment* expt,
                       const std::vector<const IFitVar*>& vars,
                       const osc::IOscCalcAdjustable* calc0,
                       double dchisq_max)
  {
    struct Result
    {
      Result(double c, const Seed& p) : chisq(c), pt(p) {}

      bool operator<(const Result& r) const {return chisq < r.chisq;}

      double Distance(const Seed& s) const
      {
        auto a = pt.GetVals();
        auto b = s.GetVals();

        // Compute Euclidean distance naively. Approximately OK since we
        // normally scale our parameters to have human-ish scale.
        double dist2 = 0;
        for(auto it: a) dist2 += util::sqr(it.second - b[it.first]);
        return sqrt(dist2);
      }

      double chisq;
      Seed pt;
    };

    std::vector<Result> results;

    // Explore all the seeds to find out where a no-systs minimization of them
    // ends up
    for(const Seed& seed: seeds.GetSeeds()){
      osc::IOscCalcAdjustable* calc = calc0->Copy();
      seed.ResetCalc(calc);

      MinuitFitter fit(expt, vars);
      const double chi = fit.Fit(calc, IFitter::kQuiet)->EvalMetricVal();

      std::map<const IFitVar*, double> valmap;
      for(const IFitVar* v: vars) valmap[v] = v->GetValue(calc);
      results.emplace_back(chi, valmap);
    }

    // Lowest chisq first
    std::sort(results.begin(), results.end());

    const double bestChisq = results[0].chisq;

    // Cull the list to a list of unique results not too far above the best
    // result
    std::vector<Seed> ret;
    for(const Result& result: results){
      // Stop once seed results become too bad
      if(dchisq_max > 0 && result.chisq > bestChisq + dchisq_max) break;

      bool ok = true;
      for(const Seed& already: ret){
        // Reject any points that are very nearby other previously added
        // seeds. The threshold distance is arbitrary. We're really trying to
        // determine if these are the same minimum.
        if(result.Distance(already) < 1e-3){
          ok = false;
          break;
        }
      } // end for already
      if(ok) ret.push_back(result.pt);
    }

    return ret;
  }
}
