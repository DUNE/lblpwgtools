#pragma once

#include "CAFAna/Core/PRISMReweightableSpectrum.h"

#include "CAFAna/PRISM/EigenUtils.h"

#include "fhiclcpp/ParameterSet.h"
#include "TH2.h"
#include "TH3.h"

#include <iostream>
#include <numeric>
#include <vector>

namespace ana {

  struct DetectorStop {
    //double min, max;
    double max, min;
    double POT;
    int horn_current;
  };

  struct RunPlan {
  
    DetectorStop const &FindStop(double offaxis_m, int kA) const;

    // Only now using this function to run-plan weights RWSpecs.
    PRISMReweightableSpectrum Weight(PRISMReweightableSpectrum const &NDSpec, int kA,
                                     HistAxis const &axis, 
                                     bool SetErrorsFromPredictedRate = false) const;

     // Overload function to take Spectrums as well, still output PRISMRWSpec
     PRISMReweightableSpectrum Weight(Spectrum const &NDSpec, int kA,
                                      HistAxis const &axis, 
                                      bool SetErrorsFromPredictedRate = false) const;

     double GetPlanPOT() const;

     Eigen::ArrayXd Unweight(Eigen::ArrayXd const &arr, 
                             int kA, LabelsAndBins const &LBs) const;

     TH1D *AsTH1(int kA) const;

     std::vector<DetectorStop> stops;
  };

  inline RunPlan make_RunPlan(fhicl::ParameterSet const &ps, double total_POT) {
    // flat: {
    //        plan: [ { xrange: [-2,2] time: 1, horn_current: 280 },
    //                { xrange: [-2,2] time: 1 },
    //                { xrange: [2,6] time: 1 },
    //                { xrange: [6,10] time: 1 },
    //                { xrange: [10,14] time: 1 },
    //                { xrange: [14,18] time: 1 },
    //                { xrange: [18,22] time: 1 },
    //                { xrange: [22,26] time: 1 },
    //                { xrange: [26,30] time: 1 },
    //                { xrange: [30,34] time: 1 } ]
    //    }

    RunPlan rp;

    for (fhicl::ParameterSet const &stop :
         ps.get<std::vector<fhicl::ParameterSet>>("plan")) {

      auto xrange = stop.get<std::array<double, 2>>("xrange");
      double rel_time = stop.get<double>("time");
      int horn_current = stop.get<int>("horn_current", 293);
      /*rp.stops.push_back({std::min(xrange[0], xrange[1]),
                          std::max(xrange[0], xrange[1]), rel_time,
                          horn_current});*/
      rp.stops.push_back({std::max(xrange[0], xrange[1]),
                          std::min(xrange[0], xrange[1]), rel_time,
                          horn_current});
    }

    std::stable_sort(rp.stops.begin(), rp.stops.end(),
                     [](DetectorStop const &l, DetectorStop const &r) {
                       if (l.horn_current != r.horn_current) {
                         return l.horn_current < r.horn_current;
                       }
                       return l.min < r.min;
                     });

    double sumtime = rp.GetPlanPOT();
    std::cout << "total_POT = " << total_POT << ", total time = " << sumtime
              << std::endl;

    for (auto &stop : rp.stops) {
      stop.POT *= (total_POT / sumtime);
    }
  
    return rp;
  }

} // namespace ana
