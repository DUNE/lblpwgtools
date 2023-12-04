#include "CAFAna/PRISM/RunPlan.h"
#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Core/MathUtil.h"

namespace ana {

  //------------------------------------------------------------------------------
  void RunPlan::FindStop(double offaxis_m, int kA, std::vector<DetectorStop> &ret_stops) const {
    std::for_each(stops.cbegin(), stops.cend(), [=, &ret_stops](DetectorStop const &st) {
      if ((st.horn_current == kA) && (offaxis_m >= st.min) && (offaxis_m < st.max)) {
        ret_stops.push_back(st);
      }
    });
  }

  //------------------------------------------------------------------------------
  // Only now using this function to run-plan weights RWSpecs.
  PRISMReweightableSpectrum RunPlan::Weight(PRISMReweightableSpectrum const &NDSpec, int kA,
                              HistAxis const &axis, 
                              bool SetErrorsFromPredictedRate) const {
    // Assume this spectrum is in per/POT
    Eigen::MatrixXd NDSpec_mat = NDSpec.GetEigen(1);
    Eigen::MatrixXd NDSumSq_mat = NDSpec.GetSumSqEigen(1);    
    std::vector<double> edges = (axis.GetBinnings().size() == 2) ? 
                                axis.GetBinnings().at(1).Edges() : 
                                axis.GetBinnings().at(2).Edges(); // Is 3D.
    for (int yit = 1; yit <= NDSpec_mat.rows() - 2; ++yit) {
      double ypos = edges.at(yit - 1) + ((edges.at(yit) - edges.at(yit - 1)) / 2);
      std::vector<DetectorStop> ret_stops;
      FindStop(ypos, kA, ret_stops);
      double yPOT(0);
      for (const auto &st : ret_stops) {
        yPOT += st.POT;
      }
      double sum(0);
      for (int xit = 1; xit <= NDSpec_mat.cols() - 2; ++xit) {
        double bc = NDSpec_mat(yit, xit) * yPOT;
        double bvar = SetErrorsFromPredictedRate      
                    ? bc
                    : NDSumSq_mat(yit, xit) * std::pow(yPOT, 2);
        NDSpec_mat(yit, xit) = bc;
        NDSumSq_mat(yit, xit) = bvar;
      }
    }

    std::vector<std::string> anaLabels = { axis.GetLabels().at(0) };
    std::vector<Binning> anaBins = { axis.GetBinnings().at(0) };
    if (axis.GetBinnings().size() == 3) {
      anaLabels.push_back(axis.GetLabels().at(1));
      anaBins.push_back(axis.GetBinnings().at(1));
    }
    LabelsAndBins anaAxis = LabelsAndBins(anaLabels, anaBins);
    LabelsAndBins weightAxis = (axis.GetBinnings().size() == 2) ?
      LabelsAndBins(axis.GetLabels().at(1), axis.GetBinnings().at(1)) : // 2D
      LabelsAndBins(axis.GetLabels().at(2), axis.GetBinnings().at(2)); // 3D.

    PRISMReweightableSpectrum ret(std::move(NDSpec_mat), std::move(NDSumSq_mat),
                                  anaAxis, weightAxis, GetPlanPOT(), 0);
    return ret;
  }

  //------------------------------------------------------------------------------
  // Overload function to take Spectrums as well, still output PRISMRWSpec
  PRISMReweightableSpectrum RunPlan::Weight(Spectrum const &NDSpec, int kA,
                                   HistAxis const &axis, 
                                   bool SetErrorsFromPredictedRate) const {
    // Assume this spectrum is in per/POT
    std::unique_ptr<TH1> NDSpec_h(NDSpec.ToTH1(1));
    NDSpec_h->SetDirectory(nullptr);    
    Eigen::ArrayXd ErrorArr = NDSpec.GetEigen(1);
    ErrorArr.setZero();
    for (int el = 1; el <= ErrorArr.size() - 2; ++el) {
      ErrorArr(el) = NDSpec_h->GetBinError(el);
    }                      
    Eigen::MatrixXd NDErrors_mat = ConvertArrayToMatrix(ErrorArr, 
                                                        NDSpec.GetBinnings());

    Eigen::MatrixXd NDSpec_mat = ConvertArrayToMatrix(NDSpec.GetEigen(1),
                                                      NDSpec.GetBinnings());

    Eigen::MatrixXd NDSumSq_mat = Eigen::MatrixXd::Zero(NDSpec_mat.rows(),
                                                        NDSpec_mat.cols()); 
    std::vector<double> edges = (axis.GetBinnings().size() == 2) ? 
                                axis.GetBinnings().at(1).Edges() :
                                axis.GetBinnings().at(2).Edges();         
                                                                                  
    for (int yit = 1; yit <= NDSpec_mat.rows() - 2; ++yit) {
      double ypos = edges.at(yit - 1) + ((edges.at(yit) - edges.at(yit - 1)) / 2);
      std::vector<DetectorStop> ret_stops;
      FindStop(ypos, kA, ret_stops);
      double yPOT(0);
      for (const auto &st : ret_stops) {
        yPOT += st.POT;
      }
      double sum(0);
      for (int xit = 1; xit <= NDSpec_mat.cols() - 2; ++xit) {
        double bc = NDSpec_mat(yit, xit) * yPOT;
        double bvar = SetErrorsFromPredictedRate      
                    ? bc
                    : std::pow((NDErrors_mat(yit, xit) * yPOT), 2);
        NDSpec_mat(yit, xit) = bc;
        NDSumSq_mat(yit, xit) = bvar;
      }
    }

    std::vector<std::string> anaLabels = { axis.GetLabels().at(0) };
    std::vector<Binning> anaBins = { axis.GetBinnings().at(0) };
    if (axis.GetBinnings().size() == 3) {
      anaLabels.push_back(axis.GetLabels().at(1));
      anaBins.push_back(axis.GetBinnings().at(1));
    }

    LabelsAndBins anaAxis = LabelsAndBins(anaLabels, anaBins);
    LabelsAndBins weightAxis = (axis.GetBinnings().size() == 2) ?
      LabelsAndBins(axis.GetLabels().at(1), axis.GetBinnings().at(1)) : // 2D
      LabelsAndBins(axis.GetLabels().at(2), axis.GetBinnings().at(2)); // 3D

    PRISMReweightableSpectrum ret(std::move(NDSpec_mat), std::move(NDSumSq_mat),
                                  anaAxis, weightAxis, GetPlanPOT(), 0);
    return ret;
  }

  //------------------------------------------------------------------------------
  double RunPlan::GetPlanPOT() const {
    return std::accumulate(
        stops.begin(), stops.end(), 0.0,
        [](double sum, DetectorStop const &ds) { return sum + ds.POT; });
  }

  //------------------------------------------------------------------------------
  Eigen::ArrayXd RunPlan::Unweight(Eigen::ArrayXd const &arr, 
                                   int kA, LabelsAndBins const &LBs) const {
    // Be careful to return an array which can be used to construct a Spectrum
    Eigen::ArrayXd unweighted = Eigen::ArrayXd::Zero(arr.size() + 2);
    unweighted.segment(1, arr.size()) = arr;

    std::vector<double> edges = LBs.GetBinnings().at(0).Edges();
    for (int xit = 1; xit <= unweighted.size() - 2; ++xit) {
      // A bit ugly, but no ROOT!
      double xpos = edges.at(xit - 1) + ((edges.at(xit) - edges.at(xit - 1)) / 2);
      std::vector<DetectorStop> ret_stops;
      FindStop(xpos, kA, ret_stops);
      double xPOT(0);
      for (const auto &st : ret_stops) {
        xPOT += st.POT;
      }
      double bc = unweighted(xit); 
      if (!std::isnormal(bc)) {
        std::cout << "[WARN]: When un-runplan weighting histogram found bad "
                     "bin content: "
                  << bc << " @ "
                  << xpos << std::endl;
      }

      if (!std::isnormal(xPOT)) {
        std::cout << "[WARN]: "
                  << xpos << " m) had bad POT: " << xPOT << std::endl;
      }
      unweighted(xit) = bc / xPOT;
    }

    return unweighted;
  }

  //------------------------------------------------------------------------------
  TH1D *RunPlan::AsTH1(int kA) const {
    std::vector<double> bins;

    double previous_max(0);
    for (auto const &s : stops) {
      if (s.horn_current != kA) {
        continue;
      }
      // Fill vector with all bin max and min
      bins.push_back(s.min);
      bins.push_back(s.max);
    }
    // Remove duplicate bin edges
    std::sort(bins.begin(), bins.end());
    auto last = std::unique(bins.begin(), bins.end());
    bins.erase(last, bins.end());

    if (!bins.size()) {
      std::cout << "[ERROR]: Found no run plan bins with horn_current = " << kA
                << std::endl;
      abort();
    }

    TH1D *rp = new TH1D("run_plan", ";Off axis (m);POT-years", bins.size() - 1,
                        bins.data());

    double offaxis_m(bins.at(0));
    while (offaxis_m < rp->GetXaxis()->GetBinUpEdge(bins.size() - 1)) {
      std::vector<DetectorStop> ret_stops;
      FindStop(offaxis_m, kA, ret_stops);
      double yPOT(0);
      for (const auto &st : ret_stops) {
        yPOT += st.POT;
      }
      int bin = rp->FindBin(offaxis_m);
      rp->SetBinContent(bin, yPOT / POT120);
      offaxis_m += 0.5;
    }    

    return rp;
  }

} // namespace ana
