#include "CAFAna/PRISM/SimpleChi2Experiment.h"

#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/LoadFromRegistry.h"
#include "CAFAna/Core/Stan.h"
#include "CAFAna/Core/StanUtils.h"
#include "CAFAna/Core/Utilities.h"

#include "OscLib/IOscCalc.h"

namespace ana {

  REGISTER_LOADFROM("PRISMChi2CovarExperiment", IExperiment, PRISMChi2CovarExperiment);

  // Chi2Experiment for Chi2 calculation using full
  // PRISM covariance

  //----------------------------------------------------------------------
  PRISMChi2CovarExperiment::PRISMChi2CovarExperiment(const PredictionPRISM* pred,
                                                     const Spectrum& data,
                                                     bool UseCovariance, double NDPOT, double FDPOT,
                                                     PRISM::MatchChan match_chan)
                                                     : fPred(pred), fData(data), 
                                                       fUseCovariance(UseCovariance),
                                                       fPOTND(NDPOT), fPOTFD(FDPOT),
                                                       fMatchChannel(match_chan) 
  {
    pFitBoundry = {1, fData.GetEigen(1).size() - 2};
  }

  //----------------------------------------------------------------------
  PRISMChi2CovarExperiment::~PRISMChi2CovarExperiment()
  {
  }

  //----------------------------------------------------------------------
  double PRISMChi2CovarExperiment::ChiSq(osc::IOscCalcAdjustable* calc,
                                         const SystShifts& syst) const
  {
    auto PRISMComp_map = fPred->PredictPRISMComponents(calc, syst, fMatchChannel);

    Eigen::ArrayXd pred = PRISMComp_map.at(PredictionPRISM::kNDDataCorr_FDExtrap)
                              .GetEigen(fPOTFD); //kNDDataCorr_FDExtrap 
    Eigen::ArrayXd data = fData.GetEigen(fPOTFD);
  
    Eigen::VectorXd vpred = pred.segment(pFitBoundry.first, pFitBoundry.second).matrix();
    Eigen::VectorXd vdata = data.segment(pFitBoundry.first, pFitBoundry.second).matrix();
 
    Eigen::MatrixXd CovMat = Eigen::MatrixXd::Zero(vdata.size(), vdata.size());

    for (int x = 0; x < CovMat.rows(); x++) {
      // Poisson error on number of events in bin
      // Test alternative Chi2 covariance from [ref]
      // Nucl. Instrum. Meth. A, vol. 961, p. P163677, 2020.
      CovMat(x, x) = 3 / ((1 / vdata(x)) + (2 / vpred(x)));
    }
    // Add Poisson errors in quadrature to covariance (if you want to)
    if (fUseCovariance) {
      auto cov_s = PRISMComp_map.at(PredictionPRISM::kExtrapCovarMatrix);
     
      Eigen::MatrixXd cov_lc = ConvertArrayToMatrix(cov_s.GetEigen(fPOTND),
                                                    cov_s.GetBinnings())
                               .block(pFitBoundry.first, pFitBoundry.first, 
                                      pFitBoundry.second, pFitBoundry.second);
      
      cov_lc *= std::pow(fPOTFD/fPOTND, 2);
      CovMat += cov_lc;
    }

    return (vpred - vdata).transpose() * CovMat.inverse() * (vpred - vdata);
  }

  //----------------------------------------------------------------------
  void PRISMChi2CovarExperiment::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = dir;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("PRISMChi2CovarExperiment").Write("type");

    fPred->SaveTo(dir, "pred");
    fData.SaveTo(dir, "data");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<PRISMChi2CovarExperiment> PRISMChi2CovarExperiment::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    TObjString* ptag = (TObjString*)dir->Get("type");
    assert(ptag);
    assert(ptag->GetString() == "PRISMChi2CovarExperiment");
    delete ptag;

    const PredictionPRISM* pred = ana::LoadFrom<PredictionPRISM>(dir, "pred").release();
    const std::unique_ptr<Spectrum> data = Spectrum::LoadFrom(dir, "data");

    delete dir;

    auto ret = std::make_unique<PRISMChi2CovarExperiment>(pred, *data);
    return ret;
  }

  //----------------------------------------------------------------------
  void PRISMChi2CovarExperiment::SetFitBoundaries(double xmin, double xmax, double ymin, double ymax)
  {
    Eigen::ArrayXd fMaskA = GetMaskArray(fData, xmin, xmax, ymin, ymax);
    int xlowbin(1), xhighbin(-1), it(1); // remove under/overflow bins
    while (fMaskA(it) == 0) {
      xlowbin++;
      it++;
    }
    xhighbin += xlowbin;
    while (fMaskA(it) == 1) {
     xhighbin++;
     it++;
    }
    pFitBoundry = {xlowbin, xhighbin};
  }

} // namespace ana
