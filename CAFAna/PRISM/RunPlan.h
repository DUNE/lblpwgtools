#pragma once

#include "CAFAna/Core/ReweightableSpectrum.h"
#include "CAFAna/Core/Spectrum.h"

#include "CAFAna/Analysis/Exposures.h"

#include "TH2.h"
#include "TH3.h"

#include <numeric>
#include <vector>

namespace ana {
struct DetectorStop {
  double min, max;
  double POT;
  int horn_current;
};

struct RunPlan {

  DetectorStop const &FindStop(double offaxis_m, int kA) const {
    static DetectorStop dummy{0, 0, 0, 0};

    auto found =
        std::find_if(stops.cbegin(), stops.cend(), [=](DetectorStop const &st) {
          return (st.horn_current == kA) && (offaxis_m >= st.min) &&
                 (offaxis_m < st.max);
        });

    if (found == stops.cend()) {
      std::cout << "[ERROR]: Failed to find stop for " << offaxis_m << ", "
                << kA << " kA" << std::endl;

      for (auto s : stops) {
        std::cout << "\t[" << s.min << ", " << s.max << "], POT: " << s.POT
                  << ", HC: " << s.horn_current << " kA" << std::endl;
      }

      abort();
    }
    return *found;
  }

  // I think this Spectrum Weight function is now **redundant**.
  // Only use the ReweightableSpectrum Weight function.
  /*Spectrum Weight(Spectrum NDSpec, int kA,
                  bool SetErrorsFromPredictedRate = false) const {

    std::unique_ptr<TH2> NDSpec_h(NDSpec.ToTH2(1));
    NDSpec_h->SetDirectory(nullptr);

    for (int yit = 0; yit < NDSpec_h->GetYaxis()->GetNbins(); ++yit) {
      double ypos = NDSpec_h->GetYaxis()->GetBinCenter(yit + 1);
      auto stop = FindStop(ypos, kA);
      for (int xit = 0; xit < NDSpec_h->GetXaxis()->GetNbins(); ++xit) {
        double bc = NDSpec_h->GetBinContent(xit + 1, yit + 1) * stop.POT;
        double be = SetErrorsFromPredictedRate
                        ? sqrt(bc)
                        : (NDSpec_h->GetBinError(xit + 1, yit + 1) * stop.POT);
        NDSpec_h->SetBinContent(xit + 1, yit + 1, bc);
        NDSpec_h->SetBinError(xit + 1, yit + 1, be);
      }
    }

    NDSpec.Clear();
    std::cout << "Filling from hist!" << std::endl;
    NDSpec.FillFromHistogram(NDSpec_h.get()); 
    NDSpec.OverrideLivetime(0);
    NDSpec.OverridePOT(GetPlanPOT());
    //HistCache::Delete(NDSpec_h);
    return NDSpec;
  }*/

  // Only now using this function to run-plan weights RWSpecs.
  ReweightableSpectrum Weight(ReweightableSpectrum const &NDSpec, int kA,
                              bool SetErrorsFromPredictedRate = false) const {
    // Assume this spectrum is in per/POT

    std::unique_ptr<TH2> NDSpec_h(NDSpec.ToTH2(1));

    NDSpec_h->SetDirectory(nullptr);

    for (int yit = 0; yit < NDSpec_h->GetYaxis()->GetNbins(); ++yit) {
      double ypos = NDSpec_h->GetYaxis()->GetBinCenter(yit + 1);
      auto stop = FindStop(ypos, kA);
      double sum(0);
      for (int xit = 0; xit < NDSpec_h->GetXaxis()->GetNbins(); ++xit) {
        double bc = NDSpec_h->GetBinContent(xit + 1, yit + 1) * stop.POT;
        double be = SetErrorsFromPredictedRate      
                    ? sqrt(bc)
                    : (NDSpec_h->GetBinError(xit + 1, yit + 1) * stop.POT);
        sum += bc; 
        NDSpec_h->SetBinContent(xit + 1, yit + 1, bc);
        NDSpec_h->SetBinError(xit + 1, yit + 1, be);         
      }
    }

    std::vector<std::string> labels = NDSpec.GetLabels();
    std::vector<Binning> bins = NDSpec.GetBinnings();

    ReweightableSpectrum ret = ReweightableSpectrum(ana::Constant(1), NDSpec_h.get(), 
                                                    labels, bins, GetPlanPOT(), 0);

    return ret;
  }

  double GetPlanPOT() const {
    return std::accumulate(
        stops.begin(), stops.end(), 0.0,
        [](double sum, DetectorStop const &ds) { return sum + ds.POT; });
  }

  TH1 *Unweight(TH1 const *h, int kA) const {

    TH1 *unweighted = static_cast<TH1 *>(h->Clone("unweighted"));
    unweighted->SetDirectory(nullptr);

    for (int xit = 0; xit < unweighted->GetXaxis()->GetNbins(); ++xit) {
      double xpos = unweighted->GetXaxis()->GetBinCenter(xit + 1);
      auto stop = FindStop(xpos, kA);
      double bc = unweighted->GetBinContent(xit + 1);
      if (!std::isnormal(bc)) {
        std::cout << "[WARN]: When un-runplan weighting histogram found bad "
                     "bin content: "
                  << bc << " @ "
                  << unweighted->GetXaxis()->GetBinCenter(xit + 1) << std::endl;
      }

      if (!std::isnormal(stop.POT)) {
        std::cout << "[WARN]: Stop " << stop.min << " -- " << stop.max << "("
                  << xpos << " m) had bad POT: " << stop.POT << std::endl;
      }
      double be = unweighted->GetBinError(xit + 1);

      unweighted->SetBinContent(xit + 1, bc / stop.POT);
      /*std::cout << "[unweight]: Stop " << stop.min << " -- " << stop.max << "("
                << xpos << " m) unweighting " << bc << " by " << stop.POT
                << " @ " << kA << " kA" << std::endl;*/
      unweighted->SetBinError(xit + 1, be / stop.POT);
    }

    return unweighted;
  }

  TH1D *AsTH1(int kA) const {
    std::vector<double> bins;

    for (auto const &s : stops) {
      if (s.horn_current != kA) {
        continue;
      }
      if (!bins.size()) {
        bins.push_back(s.min);
      }
      bins.push_back(s.max);
    }

    if (!bins.size()) {
      std::cout << "[ERROR]: Found no run plan bins with horn_current = " << kA
                << std::endl;
      abort();
    }

    TH1D *rp = new TH1D("run_plan", ";Off axis (m);POT-years", bins.size() - 1,
                        bins.data());
    int i = 1;
    for (auto const &s : stops) {
      if (s.horn_current != kA) {
        continue;
      }
      rp->SetBinContent(i++, s.POT / POT120);
    }
    return rp;
  }

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
    rp.stops.push_back({std::min(xrange[0], xrange[1]),
                        std::max(xrange[0], xrange[1]), rel_time,
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
