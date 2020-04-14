/*
 * fit_3flavor_MCMC.C:
 *    Fit numu and nue samples, with systs,
 *    under 3-flavor hypothesis using Bayesian Markov Chain Monte Carlo.
 *
 *    Original author:  J. Wolcott <jwolcott@fnal.gov>
 *               Date:  April 2020
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
#include "OscLib/func/OscCalculatorPMNSOptDMP.h"

#include "Utilities/func/MathUtil.h"

using namespace ana;

namespace mcmc_ana
{
  double POT = pot_fd;
  const std::string SAVED_SAMPLES_FILE = "samples_systs.root";

  double MOCKDATA_TH23 = 0.72;     // 0.72 radians --> 41 degrees
  double MOCKDATA_DM32 = 0.0025;   // normal hierarchy
  double MOCKDATA_DCP = (5./4.) * TMath::Pi();  // halfway between no CPV and max CPV

  const std::vector<std::string> PRED_FILES {
      "/cvmfs/dune.osgstorage.org/pnfs/fnal.gov/usr/dune/persistent/stash/LongBaseline/state_files/standard_v4/mcc11v4_FD_FHC.root",
      "/cvmfs/dune.osgstorage.org/pnfs/fnal.gov/usr/dune/persistent/stash/LongBaseline/state_files/standard_v4/mcc11v4_FD_RHC.root",
  };

  // ---------------------------------------------
  std::string FullFilename(const std::string & dir, std::string file)
  {
    if (!dir.empty())
      file = dir + "/" + file;
    
    return file;
  }
}

void fit_3flavor_MCMC(bool loadSamplesFromFile=true,
                      bool saveSamplesToFile=false,
                      bool drawPlots=true,
                      std::string dirPrefix=".",
                      std::string samplesFilename=mcmc_ana::SAVED_SAMPLES_FILE,
                      std::string systList="allsysts")
{
  assert (loadSamplesFromFile != saveSamplesToFile);

  auto TFText = [](bool val, std::string trueText) { return val ? trueText : "FALSE"; };
  std::cout << std::endl;
  std::cout << "Loading MCMC samples from file: " << TFText(loadSamplesFromFile, mcmc_ana::FullFilename(dirPrefix, samplesFilename)) << std::endl;
  std::cout << "Saving MCMC samples to file: " << TFText(saveSamplesToFile, mcmc_ana::FullFilename(dirPrefix, samplesFilename)) << std::endl;

  std::cout << std::endl;

  osc::IOscCalculatorAdjustable * calc;
  osc::IOscCalculatorAdjustable * calcTruth;
  std::map<std::string, std::unique_ptr<Spectrum>> fakeData;
  std::unique_ptr<SystShifts> systTruePulls;
  std::unique_ptr<SystShifts> shifts;

  // build my fit variables...
  std::map<std::string, std::unique_ptr<IPrediction>> preds;
  ConstrainedFitVarWithPrior fitSsqTh23_UniformPriorSsqTh23(&kFitSinSqTheta23, PriorUniformInFitVar, "FlatSSTh23");
  ConstrainedFitVarWithPrior fitDmSq32Scaled_UniformPrior(&kFitDmSq32Scaled, PriorUniformInFitVar, "FlatDmSq32");
  FitVarWithPrior fitDeltaInPiUnits_UniformPriordCP(&kFitDeltaInPiUnits, PriorUniformInFitVar, "FlatdcP");
  std::vector<const IFitVar*> fitVars{&fitSsqTh23_UniformPriorSsqTh23, &fitDmSq32Scaled_UniformPrior, &fitDeltaInPiUnits_UniformPriordCP};
  const std::map<const IFitVar*, std::pair<double, double>> fitVarDrawRanges
  {
    {&fitSsqTh23_UniformPriorSsqTh23,    {.3, .7}},
    {&fitDmSq32Scaled_UniformPrior,      {2.2, 2.8}},
    {&fitDeltaInPiUnits_UniformPriordCP, {0,   2}},
  };

  for (const auto & predFileName : mcmc_ana::PRED_FILES)
  {
    TFile f(predFileName.c_str());
    assert (!f.IsZombie() && ("Couldn't open prediction file:" + predFileName).c_str());
    for (const auto &predName : {"fd_interp_numu_fhc", "fd_interp_nue_fhc", "fd_interp_numu_rhc", "fd_interp_nue_rhc"})
    {
      auto dir = f.GetDirectory(predName);
      if (!dir)
        continue;
      preds.emplace(predName, ana::LoadFrom<PredictionInterp>(dir));
    }
  }
  assert(preds.size() == 4);

//    calc = new osc::OscCalculatorPMNSOpt;
//    calc = new osc::OscCalculatorPMNS;
  calc = new osc::OscCalculatorPMNSOptDMP;
  *calc = *(NuFitOscCalc(1, 1, 3));  // NH, max mixing

  std::unique_ptr<MCMCSamples> samples;
  if (!loadSamplesFromFile)
  {
    // store the default vals...
    double oldTh23 = calc->GetTh23();
    double oldDmsq32 = calc->GetDmsq32();

    // pick a few test values for some mock data...
    calc->SetTh23(mcmc_ana::MOCKDATA_TH23);
    calc->SetDmsq32(mcmc_ana::MOCKDATA_DM32);
    calc->SetdCP(mcmc_ana::MOCKDATA_DCP);
    calcTruth = new osc::OscCalculatorPMNS;  // type of this one doesn't matter; just for storage
    *calcTruth = *calc;

    // choose pulls at random
    TRandom3 rnd;
    rnd.SetSeed(20200413);
    shifts = std::make_unique<GaussianPriorSystShifts>();
    std::vector<const ISyst *> systs;
    for (const auto &syst : GetListOfSysts(systList)) {
      systs.emplace_back(syst);
      shifts->SetShift(systs.back(), rnd.Gaus());
    }

    std::vector<std::unique_ptr<SingleSampleExperiment>> expts;
    systTruePulls = shifts->Copy();
    MultiExperiment expt;
    for (const auto & predPair : preds)
    {
      fakeData.emplace(predPair.first, std::make_unique<Spectrum>(predPair.second->Predict(calc).FakeData(mcmc_ana::POT)));
      expts.emplace_back(std::make_unique<SingleSampleExperiment>(predPair.second.get(), *fakeData.at(predPair.first)));
      expt.Add(expts.back().get());
    }

    // now put the calc back to normal and reset the systs to nominal
    calc->SetTh23(oldTh23);
    calc->SetDmsq32(oldDmsq32);
    shifts->ResetToNominal();

    StanConfig cfg;
    cfg.num_warmup = 1000;
    cfg.num_samples = 10000;
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
      TFile outF(mcmc_ana::FullFilename(dirPrefix, samplesFilename).c_str(), "recreate");
      for (const auto & fdPair : fakeData)
        fdPair.second->SaveTo(outF.mkdir(("fakedata_" + fdPair.first).c_str()));
      samples->SaveTo(outF.mkdir("samples"));
      SaveTo(static_cast<osc::IOscCalculator&>(*calcTruth), outF.mkdir("calcTruth"));
      systTruePulls->SaveTo(outF.mkdir("systTruth"));
    }
  }
  else
  {
    std::string filepath("");
    for (const auto & path : {samplesFilename, mcmc_ana::FullFilename(dirPrefix, samplesFilename)})
    {
      // note: AccessPathName() returns *false* if the path is accessible <facepalm>
      if (!gSystem->AccessPathName(gSystem->ExpandPathName(path.c_str())))
        filepath = path;
    }
    TFile inf(filepath.c_str());
    if (inf.IsZombie())
      return;
    std::cout << "Opened file: " << filepath << std::endl;

    for (const auto & k : *inf.GetListOfKeys())
    {
      auto key = dynamic_cast<TKey*>(k);
      if (!key || !key->GetName()) continue;
      std::string name(key->GetName());
      if (name.size() < 9 || name.substr(0, 9) != "fakedata_")
        continue;
      fakeData.emplace(name.substr(9), LoadFrom<Spectrum>(dynamic_cast<TDirectory*>(key->ReadObj())));
    }
    std::cout << "   --->  loaded " << fakeData.size() << " fake data spectra." << std::endl;

    samples = MCMCSamples::LoadFrom(dynamic_cast<TDirectory*>(inf.Get("samples")));
    std::cout << "   --->  loaded " << samples->NumSamples() << " MCMC samples." << std::endl;

    systTruePulls = SystShifts::LoadFrom(dynamic_cast<TDirectory*>(inf.Get("systTruth")));
    calcTruth = dynamic_cast<osc::IOscCalculatorAdjustable*>(LoadFrom<osc::IOscCalculator>(dynamic_cast<TDirectory*>(inf.Get("calcTruth"))).release());  // yeah, just leak it
    std::cout << "   --->  loaded osc parameters & syst pulls used for fake data. " << std::endl;

    assert (!fakeData.empty() && samples && systTruePulls && calcTruth);

    auto bestFitIdx = samples->BestFitSampleIdx();
    for (const auto & var : samples->Vars())
      var->SetValue(calc, samples->SampleValue(var, bestFitIdx));
    shifts = std::make_unique<SystShifts>();
    for (const auto & syst : samples->Systs())
      shifts->SetShift(syst, samples->SampleValue(syst, bestFitIdx));

    //auto bfDir = dynamic_cast<TDirectory*>(inf.Get("bestfit"));
    //calc.reset(dynamic_cast<osc::OscCalculatorPMNSOpt*>(LoadFrom<osc::IOscCalculator>(dynamic_cast<TDirectory*>(bfDir->Get("osc"))).release()));
    //shifts = LoadFrom<SystShifts>(dynamic_cast<TDirectory*>(bfDir->Get("systs")));
    //assert(shifts);

    std::cout << "Finished loading from file." << std::endl;
  }

  if (!drawPlots)
    return;

  // draw the contour in oscillation parameter space overlaid on the LL surface
  TCanvas c;

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

    c.SaveAs(Form(mcmc_ana::FullFilename(dirPrefix, "marg_%s." + systList + ".png").c_str(), fitVarPtr->ShortName().c_str()));
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

    c.SaveAs(Form(mcmc_ana::FullFilename(dirPrefix, "marg_%s." + systList + ".png").c_str(), systPtr->ShortName().c_str()));
  }
  // pairs of fit vars
  for (const auto fitVar1 : fitVars)
  {
    for (auto itVar2 = fitVars.rbegin(); itVar2 != fitVars.rend(); itVar2++)
    {
      const IFitVar* fitVar2 = *itVar2;
      if (fitVar2 == fitVar1)
        break;

      c.Clear();
      std::pair<double, double> drawRange1 = fitVarDrawRanges.at(fitVar1);
      std::pair<double, double> drawRange2 = fitVarDrawRanges.at(fitVar2);
      BayesianSurface surf = samples->MarginalizeTo(fitVar1, 100, drawRange1.first, drawRange1.second,
	                                                  fitVar2, 100, drawRange2.first, drawRange2.second);
      surf.Draw();
      surf.DrawContour(surf.QuantileSurface(Quantile::kGaussian1Sigma), 7, kGreen+2); // dashed
      surf.DrawBestFit(kGray);
      TMarker marker(fitVar1->GetValue(calcTruth), fitVar2->GetValue(calcTruth), kFullStar);
      marker.SetMarkerColor(kMagenta);
      marker.SetMarkerSize(3);
      marker.Draw();
      c.SaveAs(Form(mcmc_ana::FullFilename(dirPrefix, "surface_contour_%s-%s." + systList + ".png").c_str(),
                    fitVar1->ShortName().c_str(),
                    fitVar2->ShortName().c_str()));
    }
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

      std::string outf = Form(mcmc_ana::FullFilename(dirPrefix, "marg_%s-%s." + systList + ".png").c_str(),
                              systPtr1->ShortName().c_str(),
                              systPtr2->ShortName().c_str());
      c.SaveAs(outf.c_str());
    }
  }

  // draw a comparison of the fake data and the best fit prediction
  for (const auto & predPair : preds)
  {
    c.Clear();
    // calc we passed to the surface now contains best fit params
    DataMCComparison(*fakeData.at(predPair.first), predPair.second.get(), calc, *shifts, kBinDensity);
    c.SaveAs(mcmc_ana::FullFilename(dirPrefix, "bestfitpred." + predPair.first + "." + systList + ".png").c_str());
  }

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
  c.SaveAs(mcmc_ana::FullFilename(dirPrefix, "pulls." + systList + ".png").c_str());

}
