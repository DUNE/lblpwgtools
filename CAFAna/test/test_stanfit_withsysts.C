/*
 * test_stanfit_withsysts.C:
 *    Test the use of the MCMC tool Stan for fitting in CAFAna.
 *    This version tests an actual neutrino oscillation fit,
 *    including with systematics.
 *
 *    Original author:  J. Wolcott <jwolcott@fnal.gov>
 *               Date:  January 2019
 */

#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMarker.h"
#include "TSystem.h"

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
#include "CAFAna/Experiment/BinnedLkhdExperiment.h"
#include "CAFAna/Fit/Bayesian1DMarginal.h"
#include "CAFAna/Fit/BayesianSurface.h"
#include "CAFAna/Fit/Priors.h"
#include "CAFAna/Fit/StanConfig.h"
#include "CAFAna/Fit/StanFitter.h"
#include "CAFAna/Fit/MCMCSamples.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"

#include "OscLib/func/OscCalculatorPMNS.h"
#include "OscLib/func/OscCalculatorPMNSOpt.h"
#include "OscLib/func/OscCalculatorDMP.h"

#include "Utilities/func/MathUtil.h"

using namespace ana;

namespace test
{
  double POT = 1e25;  // use a ridiculous exposure so that the systs really control the fitting
  const std::string SAVED_SAMPLES_FILE = "samples_systs.root";

  double MOCKDATA_TH23 = 0.72;     // 0.72 radians --> 41 degrees
  double MOCKDATA_DM32 = 0.0025;

  const std::vector<std::string> SYSTS_TO_THROW
  {
      "ChargedHadResFD",
      "EMResFD",
      "MissingProtonFakeDataGenerator",
      "MaCCRES"
  };

  // ---------------------------------------------
  std::string FullFilename(const std::string & dir, std::string file)
  {
    if (!dir.empty())
      file = dir + "/" + file;
    
    return file;
  }
}

void test_stanfit_withsysts(bool loadSamplesFromFile=true,
                            bool saveSamplesToFile=false,
                            bool drawPlots=true,
                            std::string dirPrefix=".",
                            std::string samplesFilename=test::SAVED_SAMPLES_FILE,
                            std::string predFile="/cvmfs/dune.osgstorage.org/pnfs/fnal.gov/usr/dune/persistent/stash/LongBaseline/state_files/standard_v4/mcc11v4_FD_FHC.root",
                            std::string predName="fd_interp_numu_fhc")
{
  assert (loadSamplesFromFile != saveSamplesToFile);

  auto TFText = [](bool val, std::string trueText) { return val ? trueText : "FALSE"; };
  std::cout << std::endl;
  std::cout << "Loading MCMC samples from file: " << TFText(loadSamplesFromFile, test::FullFilename(dirPrefix, samplesFilename)) << std::endl;
  std::cout << "Saving MCMC samples to file: " << TFText(saveSamplesToFile, test::FullFilename(dirPrefix, samplesFilename)) << std::endl;

  std::cout << std::endl;

  std::unique_ptr<IPrediction> pred;
  osc::IOscCalculatorAdjustable * calc;
  osc::IOscCalculatorAdjustable * calcTruth;
  std::unique_ptr<Spectrum> fakeData;
  std::unique_ptr<SystShifts> systTruePulls;
  std::unique_ptr<SystShifts> shifts;

  // build my fit variables...
  ConstrainedFitVarWithPrior fitSsqTh23_UniformPriorSsqTh23(&kFitSinSqTheta23, PriorUniformInFitVar, "FlatSSTh23");
  ConstrainedFitVarWithPrior fitDmSq32Scaled_UniformPrior(&kFitDmSq32Scaled, PriorUniformInFitVar, "FlatDmSq32");
  std::vector<const IFitVar*> fitVars{&fitSsqTh23_UniformPriorSsqTh23, &fitDmSq32Scaled_UniformPrior};

  TFile f(predFile.c_str());
  assert (!f.IsZombie() && ("Couldn't load prediction file:" + predFile).c_str());
  pred = LoadFrom<PredictionInterp>(dynamic_cast<TDirectory *>(f.Get(predName.c_str())));

  std::unique_ptr<MCMCSamples> samples;
  if (!loadSamplesFromFile)
  {
//    calc = new osc::OscCalculatorPMNSOpt;
//    calc = new osc::OscCalculatorPMNS;
    calc = new osc::OscCalculatorDMP;
    *calc = *(NuFitOscCalc(1, 1, 3));  // NH, max mixing
    std::vector<const ISyst *> systs;

    // store the default vals...
    double oldTh23 = calc->GetTh23();
    double oldDmsq32 = calc->GetDmsq32();

    // pick a few test values for some mock data...
    calc->SetTh23(test::MOCKDATA_TH23);
    calc->SetDmsq32(test::MOCKDATA_DM32);
//    calcTruth = new osc::OscCalculatorPMNSOpt;
    calcTruth = new osc::OscCalculatorPMNS;
    *calcTruth = *calc;

    // choose +/- 1, 2, 3 sigma pulls at random
    TRandom3 rnd;
    rnd.SetSeed(20191217);
    shifts = std::make_unique<SystShifts>();
    for (const auto &systName : test::SYSTS_TO_THROW) {
      systs.emplace_back(Registry<ISyst>::ShortNameToPtr(systName));
      shifts->SetShift(systs.back(), int(rnd.Uniform(1, 4)) * (rnd.Uniform() < 0.5 ? -1 : 1));
    }

    systTruePulls = shifts->Copy();
    Spectrum spec_pred = pred->PredictSyst(calc, *shifts);
    fakeData = std::make_unique<Spectrum>(spec_pred.FakeData(test::POT));
    BinnedLkhdExperiment expt(pred.get(), *fakeData);

    // now put the calc back to normal and reset the systs to nominal
    calc->SetTh23(oldTh23);
    calc->SetDmsq32(oldDmsq32);
    shifts->ResetToNominal();

    StanConfig cfg;
    cfg.num_warmup = 500;
    cfg.num_samples = 1000;
    cfg.max_depth = 15;
    cfg.verbosity = StanConfig::Verbosity::kQuiet;
//    cfg.verbosity = StanConfig::Verbosity::kEverything;
    cfg.save_warmup = false;
    StanFitter fitter(&expt, fitVars, systs);
    fitter.SetStanConfig(cfg);
    fitter.Fit(calc, *shifts);
    
    samples = fitter.ExtractSamples();

    if (saveSamplesToFile)
    {
      TFile outF(test::FullFilename(dirPrefix, samplesFilename).c_str(), "recreate");
      fakeData->SaveTo(outF.mkdir("fakedata"));
      samples->SaveTo(outF.mkdir("samples"));
      SaveTo(static_cast<osc::IOscCalculator&>(*calcTruth), outF.mkdir("calcTruth"));
      systTruePulls->SaveTo(outF.mkdir("systTruth"));
    }
  }
  else
  {
    std::string filepath;
    for (const auto & path : {samplesFilename, test::FullFilename(dirPrefix, samplesFilename)})
    {
      // note: AccessPathName() returns *false* if the path is accessible <facepalm>
      if (!gSystem->AccessPathName(gSystem->ExpandPathName(path.c_str())))
        filepath = path;
    }
    TFile inf(filepath.c_str());
    if (inf.IsZombie())
      return;

    fakeData = LoadFrom<Spectrum>(dynamic_cast<TDirectory*>(inf.Get("fakedata")));
    samples = MCMCSamples::LoadFrom(dynamic_cast<TDirectory*>(inf.Get("samples")));
    systTruePulls = SystShifts::LoadFrom(dynamic_cast<TDirectory*>(inf.Get("systTruth")));
    calcTruth = dynamic_cast<osc::IOscCalculatorAdjustable*>(LoadFrom<osc::IOscCalculator>(dynamic_cast<TDirectory*>(inf.Get("calcTruth"))).release());  // yeah, just leak it
    assert (fakeData && samples && systTruePulls && calcTruth);

    auto bestFitIdx = samples->BestFitSampleIdx();
    *calc = *(NuFitOscCalc(1, 1, 3));  // NH, max mixing
    for (const auto & var : samples->Vars())
      var->SetValue(calc, samples->SampleValue(var, bestFitIdx));
    shifts = std::make_unique<SystShifts>();
    for (const auto & syst : samples->Systs())
      shifts->SetShift(syst, samples->SampleValue(syst, bestFitIdx));

    //auto bfDir = dynamic_cast<TDirectory*>(inf.Get("bestfit"));
    //calc.reset(dynamic_cast<osc::OscCalculatorPMNSOpt*>(LoadFrom<osc::IOscCalculator>(dynamic_cast<TDirectory*>(bfDir->Get("osc"))).release()));
    //shifts = LoadFrom<SystShifts>(dynamic_cast<TDirectory*>(bfDir->Get("systs")));
    //assert(shifts);
  }

  if (!drawPlots)
    return;

  // draw the contour in oscillation parameter space overlaid on the LL surface
  TCanvas c;
  BayesianSurface surf = samples->MarginalizeTo(&fitSsqTh23_UniformPriorSsqTh23, 100, .3, .7,
                                                &fitDmSq32Scaled_UniformPrior, 100, 2.2, 2.8);
  surf.Draw();
  surf.DrawContour(surf.QuantileSurface(Quantile::kGaussian1Sigma), 7, kGreen+2); // dashed
  surf.DrawBestFit(kGray);
  TMarker marker(fitSsqTh23_UniformPriorSsqTh23.GetValue(calcTruth), fitDmSq32Scaled_UniformPrior.GetValue(calcTruth)*1e3, kFullStar);
  marker.SetMarkerColor(kMagenta);
  marker.SetMarkerSize(3);
  marker.Draw();
  c.SaveAs(test::FullFilename(dirPrefix, "test_stanfit_systs_surface_contour.png").c_str());

  // draw the marginals and the pulls
  for (const auto & fitVarPtr : fitVars)
  {
    c.Clear();
    Bayesian1DMarginal marg = samples->MarginalizeTo(fitVarPtr);
    auto h = marg.ToTH1(Binning::Simple(50, samples->MinValue(fitVarPtr), samples->MaxValue(fitVarPtr)));
    h.Draw("hist");

    double truthVal = fitVarPtr->GetValue(calcTruth);
    TLine l(truthVal, h.GetMinimum(), truthVal, h.GetMaximum());
    l.SetLineColor(kRed);
    l.SetLineWidth(2);
    l.SetLineStyle(kDashed);
    l.Draw();

    c.SaveAs(Form(test::FullFilename(dirPrefix, "test_stanfit_systs_marg_%s.png").c_str(), fitVarPtr->ShortName().c_str()));
  }
  for (const auto & systPtr : shifts->ActiveSysts())
  {
    c.Clear();
    Bayesian1DMarginal marg = samples->MarginalizeTo(systPtr);
    auto h = marg.ToTH1(Binning::Simple(50, samples->MinValue(systPtr), samples->MaxValue(systPtr)));
    h.Draw("hist");

    double truthVal = systTruePulls->GetShift(systPtr);
    TLine l(truthVal, h.GetMinimum(), truthVal, h.GetMaximum());
    l.SetLineColor(kRed);
    l.SetLineWidth(2);
    l.SetLineStyle(kDashed);
    l.Draw();

    c.SaveAs(Form(test::FullFilename(dirPrefix, "test_stanfit_systs_marg_%s.png").c_str(), systPtr->ShortName().c_str()));
  }
  // 2D syst space
  auto systs =  shifts->ActiveSysts();
  for (const auto & systPtr1 : systs)
  {
    for (auto itSyst2 = systs.rbegin(); itSyst2 != systs.rend(); itSyst2++)
    {
      const ISyst* systPtr2 = *itSyst2;
      if (systPtr2 == systPtr1)
        break;

      c.Clear();
      BayesianSurface marg = samples->MarginalizeTo(systPtr1, 30, samples->MinValue(systPtr1), samples->MaxValue(systPtr1),
                                                    systPtr2, 30, samples->MinValue(systPtr2), samples->MaxValue(systPtr2));
      auto h2 = marg.ToTH2();
      h2->Draw("colz");
      marg.DrawContour(marg.QuantileSurface(Quantile::kGaussian1Sigma), 7, kGreen+2); // dashed

      TMarker marker(systTruePulls->GetShift(systPtr1), systTruePulls->GetShift(systPtr2), kFullStar);
      marker.SetMarkerColor(kBlack);
      marker.SetMarkerSize(3);
      marker.Draw();

      std::string outf = Form(test::FullFilename(dirPrefix, "test_stanfit_systs_marg_%s-%s.png").c_str(),
                              systPtr1->ShortName().c_str(),
                              systPtr2->ShortName().c_str());
      c.SaveAs(outf.c_str());
    }
  }

  // draw a comparison of the fake data and the best fit prediction
  c.Clear();
  // calc we passed to the surface now contains best fit params
  DataMCComparison(*fakeData, pred.get(), calc, *shifts, kBinDensity);
  c.SaveAs(test::FullFilename(dirPrefix, "test_stanfit_systs_bestfitpred.png").c_str());


  // show that the pulls were what we expect
  TH1D h_fittedPulls("fitted_pulls", ";Systematic;Pull (#sigma)", shifts->ActiveSysts().size(), 0, shifts->ActiveSysts().size());
  TH1D h_truePulls("true_pulls", ";Systematic;Pull (#sigma)", shifts->ActiveSysts().size(), 0, shifts->ActiveSysts().size());
  std::size_t systIdx = 0;
  double maxShift = 0;
  for (const auto & syst : shifts->ActiveSysts())
  {
    h_fittedPulls.SetBinContent(++systIdx, shifts->GetShift(syst));
    maxShift = std::max(maxShift, std::abs(shifts->GetShift(syst)));
    h_truePulls.SetBinContent(systIdx, systTruePulls->GetShift(syst));
    maxShift = std::max(maxShift, std::abs(systTruePulls->GetShift(syst)));
    for (auto & h : {&h_fittedPulls, &h_truePulls})
      h->GetXaxis()->SetBinLabel(systIdx, syst->ShortName().c_str());
  }
  maxShift = std::max(2., 1.2*maxShift);
  h_truePulls.GetYaxis()->SetRangeUser(-maxShift, maxShift);
  c.Clear();
  h_truePulls.Draw("hist x");
  h_fittedPulls.SetMarkerStyle(kFullCircle);
  h_fittedPulls.Draw("p same");
  TLegend leg(0.7, 0.7, 0.9, 0.9);
  leg.SetFillStyle(0);
  leg.SetBorderSize(0);
  leg.AddEntry(&h_truePulls, "True", "l");
  leg.AddEntry(&h_fittedPulls, "Fitted", "p");
  leg.Draw();
  c.SaveAs(test::FullFilename(dirPrefix, "test_stanfit_systs_pulls.png").c_str());

}