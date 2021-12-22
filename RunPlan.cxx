#include "CAFAna/PRISM/RunPlan.h"

namespace ana {

  DetectorStop const &RunPlan::FindStop(double offaxis_m, int kA) const {
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

  // Only now using this function to run-plan weights RWSpecs.
  PRISMReweightableSpectrum RunPlan::Weight(PRISMReweightableSpectrum const &NDSpec, int kA,
                              HistAxis const &axis, 
                              bool SetErrorsFromPredictedRate) const {
    // Assume this spectrum is in per/POT
    std::unique_ptr<TH2> NDSpec_h(NDSpec.ToTH2(1)); 
    NDSpec_h->SetDirectory(nullptr);

    Eigen::MatrixXd NDSpec_mat = NDSpec.GetEigen(1);
    Eigen::MatrixXd NDSumSq_mat = NDSpec.GetSumSqEigen(1);    

    for (int yit = 1; yit <= NDSpec_h->GetYaxis()->GetNbins(); ++yit) {
      double ypos = NDSpec_h->GetYaxis()->GetBinCenter(yit);
      auto stop = FindStop(ypos, kA);
      double sum(0);
      for (int xit = 1; xit <= NDSpec_h->GetXaxis()->GetNbins(); ++xit) {
        double bc = NDSpec_h->GetBinContent(xit, yit) * stop.POT;
        double bvar = SetErrorsFromPredictedRate      
                    ? bc
                    : NDSumSq_mat(yit, xit) * std::pow(stop.POT, 2);
        NDSpec_mat(yit, xit) = bc;
        NDSumSq_mat(yit, xit) = bvar;
      }
    }
    LabelsAndBins anaAxis = LabelsAndBins(axis.GetLabels().at(0), axis.GetBinnings().at(0));
    LabelsAndBins weightAxis = LabelsAndBins(axis.GetLabels().at(1), axis.GetBinnings().at(1));

    PRISMReweightableSpectrum ret(std::move(NDSpec_mat), std::move(NDSumSq_mat),
                                  anaAxis, weightAxis, GetPlanPOT(), 0);
    return ret;
  }

   // Overload function to take Spectrums as well, still output PRISMRWSpec
   PRISMReweightableSpectrum RunPlan::Weight(Spectrum const &NDSpec, int kA,
                                    HistAxis const &axis, 
                                    bool SetErrorsFromPredictedRate) const {
     // Assume this spectrum is in per/POT
     assert(NDSpec.NDimensions() == 2); // Only gonna work for 2D axis, 3D not implemented yet
     std::unique_ptr<TH2> NDSpec_h(NDSpec.ToTH2(1));
                                                    
     Eigen::MatrixXd NDSpec_mat = ConvertArrayToMatrix(NDSpec.GetEigen(1),
                                                       NDSpec.GetBinnings());
     Eigen::MatrixXd NDSumSq_mat = Eigen::MatrixXd::Zero(NDSpec_mat.rows(),
                                                         NDSpec_mat.cols()); 
                                                                                                 
     for (int yit = 1; yit <= NDSpec_h->GetYaxis()->GetNbins(); ++yit) {
       double ypos = NDSpec_h->GetYaxis()->GetBinCenter(yit);
       auto stop = FindStop(ypos, kA);
       double sum(0);
       for (int xit = 1; xit <= NDSpec_h->GetXaxis()->GetNbins(); ++xit) {
         double bc = NDSpec_h->GetBinContent(xit, yit) * stop.POT;
         double bvar = SetErrorsFromPredictedRate      
                     ? bc
                     : std::pow((NDSpec_h->GetBinError(xit, yit) * stop.POT), 2);
         NDSpec_mat(yit, xit) = bc;
         NDSumSq_mat(yit, xit) = bvar;
       }
     }
     LabelsAndBins anaAxis = LabelsAndBins(axis.GetLabels().at(0), axis.GetBinnings().at(0));
     LabelsAndBins weightAxis = LabelsAndBins(axis.GetLabels().at(1), axis.GetBinnings().at(1));

     PRISMReweightableSpectrum ret(std::move(NDSpec_mat), std::move(NDSumSq_mat),
                                   anaAxis, weightAxis, GetPlanPOT(), 0);
     return ret;
  }


  double RunPlan::GetPlanPOT() const {
    return std::accumulate(
        stops.begin(), stops.end(), 0.0,
        [](double sum, DetectorStop const &ds) { return sum + ds.POT; });
  }

  TH1 *RunPlan::Unweight(TH1 const *h, int kA) const {

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

  TH1D *RunPlan::AsTH1(int kA) const {
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
      rp->SetBinContent(i++, s.POT / 1.1e21); // hardcode POT120 due to linker error
    }
    return rp;
  }

} // namespace ana
