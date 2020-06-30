/*
 * test_stanfit_systpulls.C:
 *    Test the use of the MCMC tool Stan for fitting in CAFAna.
 *    This test specifically examines the systematics fitting
 *    with a series of pull studies.
 *
 *    Original author:  J. Wolcott <jwolcott@fnal.gov>
 *               Date:  November 2018
 */

#include "TCanvas.h"
#include "TGraph.h"
#include "TMarker.h"
#include "TROOT.h"

#include "CAFAna/Analysis/Plots.h"
#include "CAFAna/Analysis/CalcsNuFit.h"
#include "CAFAna/Analysis/Plots.h"
#include "CAFAna/Analysis/common_fit_definitions.h"
#include "CAFAna/Core/FitVarWithPrior.h"
#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/StanTypedefs.h"
#include "CAFAna/Core/StanUtils.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"
#include "CAFAna/Fit/Bayesian1DMarginal.h"
#include "CAFAna/Fit/BayesianSurface.h"
#include "CAFAna/Fit/StanConfig.h"
#include "CAFAna/Fit/StanFitter.h"
#include "CAFAna/Fit/MCMCSamples.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"

#include "OscLib/func/OscCalculatorPMNSOpt.h"

#include "Utilities/func/MathUtil.h"

using namespace ana;

namespace test
{
  const std::vector<std::string> SYSTS_TO_CHECK
      {
          "ChargedHadResFD",
          "EMResFD",
          "MissingProtonFakeDataGenerator",
          "MaCCRES"
      };
  // ---------------------------------------------
}

/////////////////////////////////////////////
/////////////////////////////////////////////


void test_stanfit_systpulls(const std::string &workDir = ".",
                            const std::string &predFile = "/cvmfs/dune.osgstorage.org/pnfs/fnal.gov/usr/dune/persistent/stash/LongBaseline/state_files/standard_v4/mcc11v4_FD_FHC.root",
                            const std::string &predName = "fd_interp_numu_fhc")
{
  auto stock_calc = NuFitOscCalc(1, 1, 3);  // NH, max mixing
  std::unique_ptr<osc::IOscCalculatorAdjustable> calc = std::make_unique<osc::OscCalculatorPMNSOpt>();
  osc::CopyParams(stock_calc, calc.get());

  std::vector<const ISyst *> systs;
  for (const auto &syst : test::SYSTS_TO_CHECK)
    systs.push_back(Registry<ISyst>::ShortNameToPtr(syst));

  TFile f(predFile.c_str());
  assert (!f.IsZombie() && ("Couldn't load prediction file:" + predFile).c_str());
  auto pred = LoadFrom<PredictionInterp>(&f, predName);

  Spectrum nominal = pred->Predict(calc.get());

  // loop over the systs.
  // make a fake data spectrum and fit to it for each one.
  // store the best fits...
  std::map<const ISyst *, double> bestFits;
  for (const auto &syst : systs)
  {
    std::cout << "\n\nTesting syst: " << syst->ShortName() << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << std::endl;

    // do a +2-sigma test.
    SystShifts shifts(syst, +2);
    Spectrum spec_pred = pred->PredictSyst(calc.get(), shifts);
    Spectrum fakeData = spec_pred.FakeData(pot_fd);
    SingleSampleExperiment expt(pred.get(), fakeData);

    shifts.SetShift(syst, 0);  // seed the fit at 0

    TCanvas c;
    Spectrum CV = pred->Predict(calc.get());
    SpectrumStan shiftedStan = pred->PredictSyst(calc.get(), shifts);
    DataMCComparison(fakeData, CV);
    spec_pred.ToTH1(fakeData.POT(), kBlue)->Draw("hist same");
    std::cout << " Before fitting, LL between spectra is "
              << LogLikelihood(shiftedStan.ToBins(fakeData.POT()), fakeData.ToTH1(fakeData.POT())) / -2.
              << std::endl;
    osc::OscCalculatorPMNSOptStan stanCalc;
    osc::CopyParams(calc.get(), &stanCalc);
    std::cout << "  Experiment object says it's " << expt.LogLikelihood(&stanCalc, shifts) << std::endl;
    c.SaveAs(Form((workDir + "/test_stanfit_syst_%s_prefit.png").c_str(), syst->ShortName().c_str()));

    StanFitter fitter(&expt, {}, {syst});
    StanConfig config;
    config.num_warmup = 1000;
    config.num_samples = 10000;
//    config.verbosity = StanConfig::Verbosity::kEverything;
    fitter.SetStanConfig(config);

    TGraph g;
    for (int pullStep = -50; pullStep <= 50; pullStep++)
    {
      double pull = pullStep * 0.1;
      shifts.ResetToNominal();
      shifts.SetShift(syst, stan::math::var(
          pull));  // explicitly seed with Stan version to bypass double-to-Stan conversion warning
      double ll = util::GetValAs<double>(expt.LogLikelihood(&stanCalc, shifts));
//      std::cout << "for sigma = " << pull << ", LL = " << ll << std::endl;
      g.SetPoint(g.GetN(), pull, ll);
    }
    c.Clear();
    g.Draw("apl");
    g.SetTitle(Form(";%s pull (#sigma); LL", syst->ShortName().c_str()));
    c.SaveAs(Form((workDir + "/test_stanfit_syst_%s_LLcurve.png").c_str(), syst->ShortName().c_str()));

    shifts.ResetToNominal();
    shifts.SetShift(syst, 0);
    fitter.Fit(calc.get(), shifts);
//    fitter.TestGradients(calc, shifts);
//    continue;

    TFile outF(Form((workDir + "/samples_%s.root").c_str(), syst->ShortName().c_str()), "recreate");
    fitter.GetSamples().SaveTo(&outF, "samples");
    outF.Close();

    c.Clear();
    auto marg_syst = fitter.GetSamples().MarginalizeTo(syst);
    auto bin_marg_ll = Binning::Simple(600, -6, 6);
    auto h_marg_syst = marg_syst.ToTH1(bin_marg_ll);
    h_marg_syst.SetTitle(Form(";%s pull (#sigma);LL", syst->ShortName().c_str()));
    h_marg_syst.Draw("hist");
    c.SaveAs(Form((workDir + "/test_stanfit_syst_%s_marg.png").c_str(), syst->ShortName().c_str()));

    c.Clear();
    double bestSyst = fitter.GetSamples().SampleValue(syst, fitter.GetSamples().BestFitSampleIdx());
    std::cout << "Best fit for syst " << syst->ShortName() << " is at pull = " << bestSyst << std::endl;
    bestFits[syst] = bestSyst;
    shifts.SetShift(syst, bestSyst);
    DataMCComparison(fakeData, pred->PredictSyst(calc.get(), shifts));
    nominal.ToTH1(nominal.POT(), kGray)->Draw("hist same");
    c.SaveAs(Form((workDir + "/test_stanfit_syst_%s_bestfitpred.png").c_str(), syst->ShortName().c_str()));

  } // for (systName)

  // make a plot illustrating that everything worked
  TH1D h_bestFits("bestfits", ";Systematic;Best fit pull (#sigma)", bestFits.size(), 0, bestFits.size());
  std::size_t systIdx = 0;
  for (const auto &systPair : bestFits)
  {
    h_bestFits.SetBinContent(++systIdx, systPair.second);
    h_bestFits.GetXaxis()->SetBinLabel(systIdx, systPair.first->ShortName().c_str());
  }
  TCanvas c;
  h_bestFits.SetMarkerStyle(kFullCircle);
  h_bestFits.Draw("p");
  c.SaveAs((workDir + "/test_stanfit_syst_bestfits.png").c_str());
}
