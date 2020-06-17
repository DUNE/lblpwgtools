/*
 * test_stanfit_statsonly.C:
 *    Test the use of the MCMC tool Stan for fitting in CAFAna.
 *    This version tests an actual neutrino oscillation fit,
 *    albeit a statistics-only one.
 *
 *    Original author:  J. Wolcott <jwolcott@fnal.gov>
 *               Date:  November 2018
 */

#include "TCanvas.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TMarker.h"

#include "CAFAna/Analysis/AnalysisBinnings.h"
#include "CAFAna/Analysis/AnalysisVars.h"
#include "CAFAna/Analysis/CalcsNuFit.h"
#include "CAFAna/Analysis/Plots.h"
#include "CAFAna/Analysis/TDRLoaders.h"
#include "CAFAna/Analysis/common_fit_definitions.h"
#include "CAFAna/Core/FitVarWithPrior.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Cuts/AnaCuts.h"
#include "CAFAna/Experiment/BinnedLkhdExperiment.h"
#include "CAFAna/Fit/Bayesian1DMarginal.h"
#include "CAFAna/Fit/BayesianSurface.h"
#include "CAFAna/Fit/Priors.h"
#include "CAFAna/Fit/StanConfig.h"
#include "CAFAna/Fit/StanFitter.h"
#include "CAFAna/Fit/MCMCSamples.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Vars/FitVars.h"

#include "OscLib/func/OscCalculatorPMNSOpt.h"
#include "OscLib/func/OscCalculatorDMP.h"

#include "Utilities/func/MathUtil.h"

namespace test
{
  double MOCKDATA_TH23 = 0.72;     // 0.72 radians --> 41 degrees
  double MOCKDATA_DM32 = 0.0025;
}

void test_stanfit_statsonly(bool loadPredFromFile=true, bool savePredToFile=false, bool loadSamplesFromFile=true, bool saveSamplesToFile=false,
                            const std::string & workDir = ".",
                            const std::string & predFile = "/cvmfs/dune.osgstorage.org/pnfs/fnal.gov/usr/dune/persistent/stash/LongBaseline/state_files/standard_v4/mcc11v4_FD_FHC.root",
                            const std::string & predName = "fd_interp_numu_fhc/pred_nom")
{
  auto TFText = [](bool val) { return val ? "TRUE" : "FALSE"; };
  std::cout << std::endl;
  std::cout << "Loading predictions from file: " << TFText(loadPredFromFile) << std::endl;
  std::cout << "Storing predictions to file: " << TFText(savePredToFile) << std::endl;
  std::cout << "Loading MCMC samples from file: " << TFText(loadSamplesFromFile) << std::endl;
  std::cout << "Saving MCMC samples to file: " << TFText(saveSamplesToFile) << std::endl;
  std::cout << std::endl;

  // let's try a nice "easy" problem: numu disappearance.
  auto stock_calc = ana::NuFitOscCalc(1, 1, 3);  // NH, max mixing
  std::unique_ptr<osc::IOscCalculatorAdjustable> calc = std::make_unique<osc::OscCalculatorDMP>();
  osc::CopyParams(stock_calc, calc.get());

  std::unique_ptr<ana::PredictionExtrap> pred;
  if (!loadPredFromFile)
  {
    ana::TDRLoaders loaders(ana::Loaders::kFHC);
    pred = std::make_unique<ana::PredictionNoExtrap>(
      loaders,
      *(GetAxisBlob("v4").FDAx_numu),
      ana::kPassFD_CVN_NUMU,
      ana::kNoShift,
      kCVXSecWeights);
    loaders.Go();
  }
  else
  {
    // change this to suit whatever input you like
    TFile inf(predFile.c_str());
    pred = ana::PredictionExtrap::LoadFrom(dynamic_cast<TDirectory*>(inf.Get(predName.c_str())));
  }

  if (savePredToFile && !loadPredFromFile)
  {
    TFile outf( predFile.c_str(), "recreate");
    pred->SaveTo(outf.mkdir("pred"));
  }

  // store the default vals...
  double oldTh23 = calc->GetTh23();
  double oldDmsq32 = calc->GetDmsq32();

  // pick a few test values for some mock data...
  calc->SetTh23(test::MOCKDATA_TH23);
  calc->SetDmsq32(test::MOCKDATA_DM32);
  ana::Spectrum spec_pred = pred->Predict(calc.get());
  ana::Spectrum fakeData = spec_pred.FakeData(pot_fd);   // high-statistics fake data, i.e., Asimov
  ana::BinnedLkhdExperiment expt(pred.get(), fakeData);

  // make a 'no-oscillations' spectrum to set the scale
  calc->SetTh23(0);  // not _exactly_ no-oscillations (th13 is still nonzero) but whatever
  ana::Spectrum spec_noosc = pred->Predict(calc.get());
  TCanvas c;
  auto h_noosc = spec_noosc.ToTH1(pot_fd, kRed, kSolid, ana::kPOT, ana::kBinDensity);
  auto h_fakeData = fakeData.ToTH1(pot_fd, kBlack, kSolid, ana::kPOT, ana::kBinDensity);
  h_noosc->Draw("hist");
  h_noosc->GetYaxis()->SetTitle("Events / GeV");
  h_fakeData->Draw("pe same");
  TLegend leg(0.6, 0.6, 0.9, 0.9);
  leg.SetFillStyle(0);
  leg.SetBorderSize(0);
  leg.AddEntry(h_noosc, "No oscillations", "l");
  leg.AddEntry(h_fakeData, "Fake data", "lpe");
  leg.Draw();
  c.SaveAs( (workDir + "/test_stanfit_statsonly_fakeData.pdf").c_str() );


  // now put the calc back to normal
  calc->SetTh23(oldTh23);
  calc->SetDmsq32(oldDmsq32);

  // build my fit variables...
  ana::ConstrainedFitVarWithPrior fitSsqTh23_UniformPriorSsqTh23(&ana::kFitSinSqTheta23, ana::PriorUniformInFitVar, "FlatSSTh23");
  ana::ConstrainedFitVarWithPrior fitDmSq32Scaled_UniformPrior(&ana::kFitDmSq32Scaled, ana::PriorUniformInFitVar, "FlatDmSq32");

  std::unique_ptr<ana::MCMCSamples> samples;
  if (!loadSamplesFromFile)
  {
    ana::StanFitter fitter(&expt, {&fitSsqTh23_UniformPriorSsqTh23, &fitDmSq32Scaled_UniformPrior}, {});
    ana::StanConfig config;
    config.num_warmup =  500;
    config.num_samples = 2000;
//    config.verbosity = ana::StanConfig::Verbosity::kEverything;
    config.verbosity = ana::StanConfig::Verbosity::kQuiet;

    fitter.SetStanConfig(config);
    ana::SystShifts systs;
    fitter.Fit(calc.get(), systs);
    samples = fitter.ExtractSamples();

    if (saveSamplesToFile)
    {
      TFile outf( (workDir + "/mcmcsamples.root").c_str(), "recreate");
      samples->SaveTo(outf.mkdir("samples"));
    }
  }
  else
  {
    TFile inf((workDir + "/mcmcsamples.root").c_str());
    samples = ana::MCMCSamples::LoadFrom(dynamic_cast<TDirectory*>(inf.Get("samples")));
  }

  c.Clear();
  ana::BayesianSurface surf = samples->MarginalizeTo(&fitSsqTh23_UniformPriorSsqTh23, 30, .3, .7,
                                                     &fitDmSq32Scaled_UniformPrior, 30, 2.2, 2.8);
  surf.Draw();
  surf.DrawContour(surf.QuantileSurface(ana::Quantile::kGaussian1Sigma), 7, kGreen+2); // dashed
  surf.DrawBestFit(kGray);
  TMarker marker(util::sqr(sin(test::MOCKDATA_TH23)), test::MOCKDATA_DM32*1e3, kFullStar);
  marker.SetMarkerColor(kMagenta);
  marker.SetMarkerSize(3);
  marker.Draw();
  c.SaveAs( (workDir + "/test_stanfit_statsonly_surface_contour.png").c_str() );

  c.Clear();
  auto marg_dm2 = samples->MarginalizeTo(&fitDmSq32Scaled_UniformPrior);
  auto bin_marg_dm2 = ana::Binning::Simple(100, 2, 3);
  auto h_marg_dm2 = marg_dm2.ToTH1(bin_marg_dm2);
  h_marg_dm2.Draw("hist");
  c.SaveAs( (workDir + "/test_stanfit_statsonly_dm2_marg.png").c_str() );
  std::cout << "1sigma credible interval(s) for dm2:" << std::endl;
  for (const auto & range : marg_dm2.QuantileRanges(ana::Quantile::kGaussian1Sigma, bin_marg_dm2))
    std::cout << "  (" << range.first << ", " << range.second << ")" << std::endl;

  c.Clear();
  auto marg_ssth23 = samples->MarginalizeTo(&fitSsqTh23_UniformPriorSsqTh23);
  auto bin_marg_ssth23 = ana::Binning::Simple(100, 0, 1);
  auto h_marg_ssth23 = marg_ssth23.ToTH1(bin_marg_ssth23);
  h_marg_ssth23.Draw("hist");
  c.SaveAs( (workDir + "/test_stanfit_statsonly_ssth23_marg.png").c_str() );
  std::cout << "1sigma credible interval(s) for ssth23:" << std::endl;
  for (const auto & range : marg_ssth23.QuantileRanges(ana::Quantile::kGaussian1Sigma, bin_marg_ssth23))
    std::cout << "  (" << range.first << ", " << range.second << ")" << std::endl;

  c.Clear();
  fitSsqTh23_UniformPriorSsqTh23.SetValue(calc.get(), surf.GetBestFitX());
  fitDmSq32Scaled_UniformPrior.SetValue(calc.get(), surf.GetBestFitY());
  ana::DataMCComparison(fakeData, pred.get(), calc.get(), ana::kNoShift, ana::kBinDensity);
  c.SaveAs( (workDir + "test_stanfit_statsonly_bestfitpred.png").c_str() );

//  tree->Scan("*");

}