#pragma once

#include "CAFAna/Core/ReweightableSpectrum.h"
#include "CAFAna/Core/Spectrum.h"

#include "CAFAna/Analysis/Exposures.h"

#include "TH2.h"

#include <numeric>
#include <vector>

namespace ana {
struct DetectorStop {
  double min, max;
  double POT;
};

struct RunPlan {

  DetectorStop const &FindStop(double offaxis_m) const {
    static DetectorStop dummy{0, 0, 0};

    auto found =
        std::find_if(stops.cbegin(), stops.cend(), [=](DetectorStop const &st) {
          return (offaxis_m >= st.min) && (offaxis_m < st.max);
        });

    if (found == stops.cend()) {
      std::cout << "[WARN]: Failed to find stop for " << offaxis_m << std::endl;
      return dummy;
    }
    return *found;
  }

  Spectrum Weight(Spectrum NDSpec,
                  bool SetErrorsFromPredictedRate = false) const {
    // Assume this spectrum is in per/POT
    std::unique_ptr<TH2> NDSpec_h(NDSpec.ToTH2(1));
    NDSpec_h->SetDirectory(nullptr);

    for (int yit = 0; yit < NDSpec_h->GetYaxis()->GetNbins(); ++yit) {
      double ypos = NDSpec_h->GetYaxis()->GetBinCenter(yit + 1);
      auto stop = FindStop(ypos);
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
    NDSpec.FillFromHistogram(NDSpec_h.get());
    NDSpec.OverrideLivetime(0);
    NDSpec.OverridePOT(GetPlanPOT());
    return NDSpec;
  }

  ReweightableSpectrum Weight(ReweightableSpectrum const &NDSpec,
                              bool SetErrorsFromPredictedRate = false) const {
    // Assume this spectrum is in per/POT

    Spectrum NDSpec_s = Weight(NDSpec.ToSpectrum(), SetErrorsFromPredictedRate);
    std::unique_ptr<TH2> NDSpec_h(NDSpec_s.ToTH2(GetPlanPOT()));
    NDSpec_h->SetDirectory(nullptr);

    std::vector<std::string> labels = NDSpec_s.GetLabels();
    std::vector<Binning> bins = NDSpec_s.GetBinnings();

    // Get rid of the reweightable axis.
    labels.pop_back();
    bins.pop_back();

    return ReweightableSpectrum(ana::Constant(1), NDSpec_h.get(), labels, bins,
                                GetPlanPOT(), 0);
  }

  double GetPlanPOT() const {
    return std::accumulate(
        stops.begin(), stops.end(), 0.0,
        [](double sum, DetectorStop const &ds) { return sum + ds.POT; });
  }

  TH1 *Unweight(TH1 const *h) const {

    TH1 *unweighted = static_cast<TH1 *>(h->Clone("unweighted"));
    unweighted->SetDirectory(nullptr);

    for (int xit = 0; xit < unweighted->GetXaxis()->GetNbins(); ++xit) {
      double xpos = unweighted->GetXaxis()->GetBinCenter(xit + 1);
      auto stop = FindStop(xpos);
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
      unweighted->SetBinError(xit + 1, be / stop.POT);
    }

    return unweighted;
  }

  TH1D *AsTH1() const {
    std::vector<double> bins;

    bins.push_back(stops.front().min);
    for (auto const &s : stops) {
      bins.push_back(s.max);
    }

    TH1D *rp =
        new TH1D("run_plan", ";Off axis (m);POT-years", bins.size() - 1, bins.data());
    int i = 1;
    for (auto const &s : stops) {
      rp->SetBinContent(i++, s.POT/POT120);
    }
    return rp;
  }

  std::vector<DetectorStop> stops;
};

inline RunPlan make_RunPlan(fhicl::ParameterSet const &ps, double total_POT) {
  // flat: {
  //        plan: [ { range: [-2,0] time: 1 },
  //                { range: [0,2] time: 1 },
  //                { range: [2,6] time: 1 },
  //                { range: [6,10] time: 1 },
  //                { range: [10,14] time: 1 },
  //                { range: [14,18] time: 1 },
  //                { range: [18,22] time: 1 },
  //                { range: [22,26] time: 1 },
  //                { range: [26,30] time: 1 },
  //                { range: [30,34] time: 1 } ]
  //    }

  RunPlan rp;

  for (fhicl::ParameterSet const &stop :
       ps.get<std::vector<fhicl::ParameterSet>>("plan")) {

    auto xrange = stop.get<std::array<double, 2>>("xrange");
    double rel_time = stop.get<double>("time");
    rp.stops.push_back({xrange[0], xrange[1], rel_time});
  }

  double sumtime = rp.GetPlanPOT();
  std::cout << "total_POT = " << total_POT << ", total time = " << sumtime
            << std::endl;

  for (auto &stop : rp.stops) {
    stop.POT *= (total_POT / sumtime);
  }

  return rp;
}
} // namespace ana