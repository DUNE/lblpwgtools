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
#include "THashList.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMarker.h"
#include "TProfile.h"
#include "TSystem.h"

#include "CAFAna/Analysis/Plots.h"
#include "CAFAna/Analysis/CalcsNuFit.h"
#include "CAFAna/Analysis/Plots.h"
#include "CAFAna/Analysis/common_fit_definitions.h"
#include "CAFAna/Core/FitVarWithPrior.h"
#include "CAFAna/Core/ISyst.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/StanTypedefs.h"
#include "CAFAna/Core/StanUtils.h"
#include "CAFAna/Core/SystShifts.h"
#include "CAFAna/Fit/Bayesian1DMarginal.h"
#include "CAFAna/Fit/BayesianSurface.h"
#include "CAFAna/Fit/Priors.h"
#include "CAFAna/Fit/StanConfig.h"
#include "CAFAna/Fit/StanFitter.h"
#include "CAFAna/Fit/MCMCSamples.h"
#include "CAFAna/Prediction/PredictionNoExtrap.h"

#include "OscLib/OscCalcPMNS.h"
#include "OscLib/OscCalcPMNSOpt.h"
#include "OscLib/OscCalcDMP.h"
#include "OscLib/OscCalcAnalytic.h"

#include "Utilities/func/MathUtil.h"

using namespace ana;

namespace mcmc_ana
{
  const double POT = pot_fd;
  const std::string SAVED_SAMPLES_FILE = "samples_systs.root";

  const double MOCKDATA_TH23 = 0.72;     // 0.72 radians --> 41 degrees
  const double MOCKDATA_DM32 = 0.0024;   // normal hierarchy
  const double MOCKDATA_DCP = (5./4.) * TMath::Pi();  // halfway between no CPV and max CPV

//  const int MAX_SYSTS = 0;
  const int MAX_SYSTS = -1;
//  const int MAX_SYSTS = 5;

  const std::string STASH_DIR = "/cvmfs/dune.osgstorage.org/pnfs/fnal.gov/usr/dune/persistent/stash/LongBaseline/state_files/standard_v4";
//  const std::string STASH_DIR = "/dune/data/users/jwolcott";
  const std::vector<std::string> PRED_FILES {
      STASH_DIR + "/mcc11v4_FD_FHC.root",
      STASH_DIR + "/mcc11v4_FD_RHC.root",
      STASH_DIR + "/mcc11v4_ND_FHC.root",
      STASH_DIR + "/mcc11v4_ND_RHC.root",
  };

  // ---------------------------------------------

  // ---------------------------------------------
  int GetRedHeatPalette()
  {
    const int NRGBs = 9;
    const int n_color_contours = 999;
    static bool initialized=false;
    static int* colors=new int[n_color_contours];
    static int colmin = 0;

    if(!initialized){
      // White -> red
      Double_t stops[NRGBs] = { 0.00, 0.125, 0.250, 0.375, 0.500, 0.625, 0.750, 0.875, 1.000};
      Double_t red[NRGBs]   = { 1.00, 1.00, 0.99, 0.99, 0.98, 0.94, 0.80, 0.65, 0.40 };
      Double_t green[NRGBs] = { 0.96, 0.88, 0.73, 0.57, 0.42, 0.23, 0.09, 0.06, 0.00 };
      Double_t blue[NRGBs]  = { 0.94, 0.82, 0.63, 0.45, 0.29, 0.17, 0.11, 0.08, 0.05 };
      colmin=TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, n_color_contours);
      for(uint i=0; i<n_color_contours; ++i) colors[i]=colmin+i;

      initialized=true;
    }

    return colmin;
  }

  std::string FullFilename(const std::string & dir, std::string file)
  {
    if (!dir.empty())
      file = dir + "/" + file;
    
    return file;
  }

  // ---------------------------------------------
  template <typename VarOrSyst>
  void ScanLL(const VarOrSyst * varOrSyst,
              double begin,
              double end,
              double step,
              const osc::IOscCalcAdjustable * calcTruth,
              const SystShifts * systTruePulls,
              const IExperiment * expt,
              const std::string & outDir)
  {

    TGraph g;
    int nStep = 0;
    for (double val = begin; val <= end; val += step)
    {
      osc::OscCalcPMNSOptStan stanCalc;
      osc::CopyParams(calcTruth, &stanCalc);

      if constexpr (std::is_base_of_v<IFitVar,VarOrSyst>)
        varOrSyst->SetValue(&stanCalc, val);
      SystShifts shifts_ = *systTruePulls;
      if constexpr (std::is_base_of_v<ISyst,VarOrSyst>)
        shifts_.SetShift(varOrSyst, util::GetValAs<stan::math::var>(val));

      double ll = util::GetValAs<double>(expt->LogLikelihood(&stanCalc, shifts_));
      g.SetPoint(g.GetN(), val, ll);

      nStep++;
      if (nStep % 100 == 0)
        stan::math::recover_memory();
    }

    //for (int dm32Step = -100; dm32Step <= 100; dm32Step++)
    //{
    //  double dm32 = dm32Step * 0.001;
    //   const double stepSize = 2*TMath::Pi()/100.;
    //  double ll = -std::numeric_limits<double>::infinity();
    //  for (double th23 = 0; th23 < 100*stepSize; th23 += stepSize)
    //  {
    //    osc::OscCalcPMNSOptStan stanCalc;
    //    osc::CopyParams(calcTruth, &stanCalc);
    //    stanCalc.SetDmsq32(dm32);
    //    stanCalc.SetTh23(th23);
    //    SystShifts shifts_;
    //    for (const auto & s : systTruePulls->ActiveSysts())
    //      shifts_.SetShift(s, util::GetValAs<stan::math::var>(systTruePulls->GetShift(s)));
    //    for (double delta = 0; delta < 100*stepSize; delta += stepSize)
    //    {
    //      stanCalc.SetdCP(delta);
    //      ll = std::max(ll, util::GetValAs<double>(expt->LogLikelihood(&stanCalc, shifts_)));
    //    }
    //    stan::math::recover_memory();
    //  }
    //  g.SetPoint(g.GetN(), dm32*1000, ll);
    //}
    TCanvas c;
    g.Draw("apl");
    g.SetTitle(Form(";%s;LL", varOrSyst->LatexName().c_str()));
    c.SaveAs(Form("%s/scan_%s_LLcurve.png", outDir.c_str(), varOrSyst->ShortName().c_str()));
    abort();

  }
}

// ---------------------------------------------
// ---------------------------------------------
// ---------------------------------------------

void fit_3flavor_MCMC(bool loadSamplesFromFile=true,
                      bool saveSamplesToFile=false,
                      bool drawPlots=true,
                      std::string dirPrefix=".",
                      std::string samplesFilename=mcmc_ana::SAVED_SAMPLES_FILE,
                      bool fitND=false,
                      int fastAdaptSamples=-1,
                      double adaptDelta=-1,
                      double nd_scale=1.0)
{
  assert (loadSamplesFromFile != saveSamplesToFile);

  auto TFText = [](bool val, std::string trueText) { return val ? trueText : "FALSE"; };
  std::cout << std::endl;
  std::cout << "Loading MCMC samples from file: " << TFText(loadSamplesFromFile, mcmc_ana::FullFilename(dirPrefix, samplesFilename)) << std::endl;
  std::cout << "Saving MCMC samples to file: " << TFText(saveSamplesToFile, mcmc_ana::FullFilename(dirPrefix, samplesFilename)) << std::endl;

  std::cout << std::endl;

  osc::IOscCalcAdjustable * calc;
  osc::IOscCalcAdjustable * calcTruth;
  std::map<std::string, std::unique_ptr<Spectrum>> fakeData;
  std::unique_ptr<SystShifts> systTruePulls;
  std::unique_ptr<SystShifts> shifts;

  // build my fit variables...
  std::map<std::string, std::unique_ptr<IPrediction>> preds;
  ConstrainedFitVarWithPrior fitSsqTh23_UniformPriorSsqTh23(&kFitSinSqTheta23,
                                                            PriorUniformInFitVar,
                                                            "FlatSSTh23");
  ConstrainedFitVarWithPrior fitDmSq32Scaled_UniformPrior(&kFitDmSq32Scaled,
                                                          PriorUniformInFitVar,
                                                          "FlatDmSq32");
  ConstrainedFitVarWithPrior fitDmSq32Scaled_GaussianPrior(&kFitDmSq32Scaled,
                                                           GaussianPriorDm32Scaled,
                                                           "GaussianDmSq32");
  ConstrainedFitVarWithPrior
  fitDmSq32Scaled_SmallTopHatPrior(&kFitDmSq32Scaled,
                                   TopHatPriorInFitVar(2.5, 0.5),
                                   "SmallTopHatDmSq32");
  ConstrainedFitVarWithPrior
  fitDmSq32Scaled_BigTopHatPrior(&kFitDmSq32Scaled,
                                   TopHatPriorInFitVar(2.5, 2.5),
                                   "BigTopHatDmSq32");
  FitVarWithPrior fitDeltaInPiUnits_UniformPriordCP(&kFitDeltaInPiUnits,
                                                    PriorUniformInFitVar,
                                                    "FlatdcP");
  std::vector<const IFitVar*> fitVars{&fitSsqTh23_UniformPriorSsqTh23,
//                                      &fitDmSq32Scaled_UniformPrior,
//                                      &fitDmSq32Scaled_GaussianPrior,
                                      &fitDmSq32Scaled_SmallTopHatPrior,
//                                      &fitDmSq32Scaled_BigTopHatPrior,
                                      &fitDeltaInPiUnits_UniformPriordCP
  };
  const std::map<const IFitVar*, std::pair<double, double>> fitVarDrawRanges
  {
    {&fitSsqTh23_UniformPriorSsqTh23,       {.3, .7}},
    {&fitDmSq32Scaled_UniformPrior,         {2.2, 2.8}},
    {&fitDmSq32Scaled_GaussianPrior,        {2.2, 2.8}},
    {&fitDmSq32Scaled_SmallTopHatPrior, {2.2, 2.8}},
    {&fitDmSq32Scaled_BigTopHatPrior,   {2.2, 2.8}},
    {&fitDeltaInPiUnits_UniformPriordCP,    {0,   2}},
  };

  // load the predictions and work out which systs they support
  for (const auto & predFileName : mcmc_ana::PRED_FILES)
  {
    TFile f(predFileName.c_str());
    assert (!f.IsZombie() && ("Couldn't open prediction file:" + predFileName).c_str());
    for (const auto &predName : {"fd_interp_numu_fhc",
                                 "fd_interp_nue_fhc",
                                 "fd_interp_numu_rhc",
                                 "fd_interp_nue_rhc",
                                 "nd_interp_numu_fhc",
                                 "nd_interp_nue_fhc",
                                 "nd_interp_numu_rhc",
                                 "nd_interp_nue_rhc"})
    {
      auto dir = f.GetDirectory(predName);
      if (!dir)
        continue;
      if (std::string(predName).substr(0, 2) == "nd" && !fitND)
        continue;
      preds.emplace(predName, ana::LoadFrom<PredictionInterp>(&f, predName));
    }
  }
  std::unordered_set<const ISyst*> allSysts;
  for (const auto & predPair : preds)
  {
    if (auto predInterp = dynamic_cast<const PredictionInterp*>(predPair.second.get()))
    {
      for (const auto & syst : predInterp->GetAllSysts())
        allSysts.insert(syst);
    }
  }

  if (mcmc_ana::MAX_SYSTS >= 0 && !allSysts.empty())
  {
    TRandom3 r;
    while (int(allSysts.size()) > mcmc_ana::MAX_SYSTS)
    {
      auto s = allSysts.begin();
      std::advance(s, r.Integer(allSysts.size()));
      allSysts.erase(s);
    }
  }

  // so that multiple chains with same syst seeds can be easily merged
  std::vector<const ISyst*> allSystsSorted(allSysts.begin(), allSysts.end());
  std::sort(allSystsSorted.begin(), allSystsSorted.end(),
            [](const ISyst* a, const ISyst* b){ return a->ShortName() < b->ShortName(); });


  std::cout << preds.size() << " predictions:" << std::endl;
  for (const auto & predPair : preds)
    std::cout << " " << predPair.first  << " (" << DemangledTypeName(predPair.second.get()) << ")" << std::endl;
  assert(preds.size() == (fitND ? 6 : 4));

//    calc = new osc::OscCalcPMNSOpt;
//    calc = new osc::OscCalcPMNS;
//   calc = new osc::OscCalcDMP;
  calc = new osc::OscCalcAnalytic;
  *calc = *(NuFitOscCalc(1, 1, 3));  // NH, max mixing

  std::unique_ptr<MCMCSamples> samples, warmup;
  if (!loadSamplesFromFile)
  {
    // store the default vals...
    double oldTh23 = calc->GetTh23();
    double oldDmsq32 = calc->GetDmsq32();
    double oldDelta = calc->GetdCP();

    // pick a few test values for some mock data...
    calc->SetTh23(mcmc_ana::MOCKDATA_TH23);
    calc->SetDmsq32(mcmc_ana::MOCKDATA_DM32);
    calc->SetdCP(mcmc_ana::MOCKDATA_DCP);
    calcTruth = new osc::OscCalcPMNS;  // type of this one doesn't matter; just for storage
    *calcTruth = *calc;

    // choose syst pulls at random
    TRandom3 rnd;
    rnd.SetSeed(20200806);
    shifts = std::make_unique<GaussianPriorSystShifts>();
    for (const auto &syst : allSystsSorted)
      shifts->SetShift(syst, rnd.Gaus());
    systTruePulls = shifts->Copy();

    // make sure we fit all the systs, but those that aren't shared by every expt
    // are ignored by the ones that don't support them
    std::unordered_map<std::string, std::unique_ptr<SingleSampleExperiment>> expts;
    std::vector<std::string> exptNames;
    MultiExperiment expt;
    for (const auto & predPair : preds)
    {
      std::unordered_set<const ISyst*> unsupportedSysts(allSysts);
      const auto  pred = dynamic_cast<PredictionInterp*>(predPair.second.get());
      SystShifts thisPredShifts;
      for (const auto & syst : pred->GetAllSysts())
      {
        unsupportedSysts.erase(syst);
        thisPredShifts.SetShift(syst, systTruePulls->GetShift(syst));
      }
      std::vector<std::pair<const ISyst*, const ISyst*>> corrs;
      for (const auto & syst : unsupportedSysts)
      {
        std::cout << "  PredInterp '" << predPair.first << "' does not support syst: " << syst->ShortName() << std::endl;
        corrs.push_back(std::make_pair(syst, static_cast<const ISyst *>(nullptr)));
      }

      double scale = predPair.first.substr(0, 2) == "nd" ? nd_scale : 1.0;
      fakeData.emplace(predPair.first,
                       std::make_unique<Spectrum>(predPair.second->PredictSyst(calcTruth, thisPredShifts).FakeData(mcmc_ana::POT)));
      fakeData[predPair.first]->Scale(scale);
      expts.emplace(std::make_pair(predPair.first,
                                   std::make_unique<SingleSampleExperiment>(predPair.second.get(), *fakeData.at(predPair.first), scale)));
      expt.Add(expts.at(predPair.first).get());
      expt.SetSystCorrelations(std::distance(exptNames.begin(),
                                             std::find(exptNames.begin(), exptNames.end(), predPair.first)),
                               corrs);
      exptNames.emplace_back(predPair.first);
    }

    // now put the calc back to normal and reset the systs to nominal
    // (unless we're not fitting those vars, in which case we need to leave them alone)
    if (std::find(fitVars.begin(), fitVars.end(), &fitSsqTh23_UniformPriorSsqTh23) != fitVars.end())
      calc->SetTh23(oldTh23);
    if (std::find(fitVars.begin(), fitVars.end(), &fitDmSq32Scaled_UniformPrior) != fitVars.end()
        || std::find(fitVars.begin(), fitVars.end(), &fitDmSq32Scaled_GaussianPrior) != fitVars.end()
        || std::find(fitVars.begin(), fitVars.end(), &fitDmSq32Scaled_SmallTopHatPrior) != fitVars.end()
        || std::find(fitVars.begin(), fitVars.end(), &fitDmSq32Scaled_BigTopHatPrior) != fitVars.end())
      calc->SetDmsq32(oldDmsq32);
    if (std::find(fitVars.begin(), fitVars.end(), &fitDeltaInPiUnits_UniformPriordCP) != fitVars.end())
      calc->SetdCP(oldDelta);

    shifts->ResetToNominal();

//    mcmc_ana::ScanLL(allSystsSorted.front(), -5, 5, 0.1, calcTruth, systTruePulls.get(), &expt, dirPrefix);

    StanConfig cfg;
    cfg.num_warmup = 1000;
    cfg.num_samples = 1000;
    cfg.max_depth = 15;
    //cfg.stepsize = 0.001;
    //cfg.denseMassMx = true;
    if (adaptDelta > 0 && adaptDelta <= 1)
      cfg.delta = adaptDelta;
    if (fastAdaptSamples > 0)
      cfg.init_buffer = fastAdaptSamples;
    cfg.verbosity = StanConfig::Verbosity::kQuiet;
//    cfg.verbosity = StanConfig::Verbosity::kEverything;
    StanFitter fitter(&expt, fitVars, allSystsSorted);
    fitter.SetStanConfig(cfg);
    fitter.Fit(calc, *shifts);

    warmup = fitter.ExtractSamples(MemoryTupleWriter::WhichSamples::kWarmup);
    samples = fitter.ExtractSamples(MemoryTupleWriter::WhichSamples::kPostWarmup);

    if (saveSamplesToFile)
    {
      TFile outF(mcmc_ana::FullFilename(dirPrefix, samplesFilename).c_str(), "recreate");
      for (const auto & fdPair : fakeData)
        fdPair.second->SaveTo(&outF, "fakedata_" + fdPair.first);
      warmup->SaveTo(&outF, "warmup");
      samples->SaveTo(&outF, "samples");
      SaveTo(static_cast<osc::IOscCalc&>(*calcTruth), &outF, "calcTruth");
      systTruePulls->SaveTo(&outF, "systTruth");
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
      fakeData.emplace(name.substr(9), LoadFrom<Spectrum>(&inf, key->GetName()));
    }
    std::cout << "   --->  loaded " << fakeData.size() << " fake data spectra." << std::endl;

    warmup = MCMCSamples::LoadFrom(&inf, "warmup");
    if (warmup)
      std::cout << "   --->  loaded " << warmup->NumSamples() << " MCMC samples from warmup." << std::endl;

    samples = MCMCSamples::LoadFrom(&inf, "samples");
    std::cout << "   --->  loaded " << samples->NumSamples() << " MCMC samples." << std::endl;

    systTruePulls = LoadFrom<SystShifts>(&inf, "systTruth");
    calcTruth = dynamic_cast<osc::IOscCalcAdjustable*>(LoadFrom<osc::IOscCalc>(&inf, "calcTruth").release());  // yeah, just leak it
    std::cout << "   --->  loaded osc parameters & syst pulls used for fake data. " << std::endl;

    assert (!fakeData.empty() && samples && systTruePulls && calcTruth);

    auto bestFitIdx = samples->BestFitSampleIdx();
    for (const auto & var : samples->Vars())
      var->SetValue(calc, samples->SampleValue(var, bestFitIdx));
    shifts = std::make_unique<SystShifts>();
    for (const auto & syst : samples->Systs())
      shifts->SetShift(syst, samples->SampleValue(syst, bestFitIdx));

    //auto bfDir = dynamic_cast<TDirectory*>(inf.Get("bestfit"));
    //calc.reset(dynamic_cast<osc::OscCalcPMNSOpt*>(LoadFrom<osc::IOscCalc>(dynamic_cast<TDirectory*>(bfDir->Get("osc"))).release()));
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

    c.SaveAs(Form(mcmc_ana::FullFilename(dirPrefix, "marg_%s.systs.png").c_str(), fitVarPtr->ShortName().c_str()));
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

    c.SaveAs(Form(mcmc_ana::FullFilename(dirPrefix, "marg_%s.systs.png").c_str(), systPtr->ShortName().c_str()));
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
      c.SaveAs(Form(mcmc_ana::FullFilename(dirPrefix, "surface_contour_%s-%s.systs.png").c_str(),
                    fitVar1->ShortName().c_str(),
                    fitVar2->ShortName().c_str()));
    }
  }

  // 2D syst space

  auto systs =  shifts->ActiveSysts();
  std::set<std::string> systNames;
  std::transform(systs.begin(), systs.end(),
                 std::inserter(systNames, systNames.end()), [](const ISyst* s){ return s->ShortName();});
  assert(systNames.size() == systs.size());
  for (const auto & systName1 : systNames)
  {
    auto systPtr1 = *std::find_if(systs.begin(), systs.end(),
                                  [&systName1](const ISyst * s){ return s->ShortName() == systName1; });
    for (auto itSystName2 = systNames.rbegin(); itSystName2 != systNames.rend(); itSystName2++)
    {
      if (*itSystName2 == systName1)
        break;

      const ISyst* systPtr2 = *std::find_if(systs.begin(), systs.end(),
                                            [&itSystName2](const ISyst * s){ return s->ShortName() == *itSystName2; });

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

      std::string outf = Form(mcmc_ana::FullFilename(dirPrefix, "marg_%s-%s.allSysts.png").c_str(),
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
    c.SaveAs(mcmc_ana::FullFilename(dirPrefix, "bestfitpred." + predPair.first + ".systs.png").c_str());
  }

  // show that the pulls were what we expect
  const Binning margBins = Binning::Simple(100, -2.95, 2.95);  // then labels don't collide in the ratio plot
  TH2D h_fittedPulls("fitted_pulls", ";Systematic;Pull (#sigma)",
                     shifts->ActiveSysts().size(), 0, shifts->ActiveSysts().size(),
                     margBins.NBins(), margBins.Min(), margBins.Max());
  TH1D h_truePulls("true_pulls", ";Systematic;Pull (#sigma)", shifts->ActiveSysts().size(), 0, shifts->ActiveSysts().size());
  std::size_t systIdx = 0;
  double maxShift = 0;
  auto systsSorted = shifts->ActiveSysts();
  std::sort(systsSorted.begin(),
            systsSorted.end(),
            [](const auto syst1, const auto syst2)
            {
              return syst1->ShortName() < syst2->ShortName();
            });
  for (const auto & syst : systsSorted)
  {
    ++systIdx;
    auto systMarginal = samples->MarginalizeTo(syst);
    TH1D h_systMarginal = systMarginal.ToTH1(margBins);
    for (int binIdx=0; binIdx <= h_systMarginal.GetNbinsX(); binIdx++)
      h_fittedPulls.SetBinContent(systIdx, binIdx, h_systMarginal.GetBinContent(binIdx));
    maxShift = std::max(maxShift, std::abs(shifts->GetShift(syst)));
    h_truePulls.SetBinContent(systIdx, systTruePulls->GetShift(syst));
    maxShift = std::max(maxShift, std::abs(systTruePulls->GetShift(syst)));
    for (auto & h : {dynamic_cast<TH1*>(&h_fittedPulls), dynamic_cast<TH1*>(&h_truePulls)})
      h->GetXaxis()->SetBinLabel(systIdx, syst->ShortName().c_str());
  }
  maxShift = std::max(3., 1.2*maxShift);
  h_truePulls.GetYaxis()->SetRangeUser(-maxShift, maxShift);
  h_fittedPulls.GetYaxis()->SetRangeUser(-maxShift, maxShift);

  std::unique_ptr<TProfile> h_fittedPulls_prof(h_fittedPulls.ProfileX(ana::UniqueName().c_str(), 1, -1, "s"));
  h_fittedPulls_prof->SetLineColor(kBlue);

  c.Clear();
  c.SetCanvasSize(900,600);
  TAxis ax(*h_fittedPulls.GetXaxis());
  h_fittedPulls.GetXaxis()->GetLabels()->Clear();

  TPad upper("", "", 0, 0, 1.0, 1);
  upper.cd();
  h_fittedPulls.SetMaximum(0.25);  // so that more of the color winds up red
  mcmc_ana::GetRedHeatPalette();
  h_fittedPulls.SetFillColor(kRed+1);  // for the legend
  h_fittedPulls.GetZaxis()->SetTitle("Fraction of MCMC samples");
  h_fittedPulls.GetZaxis()->RotateTitle();
  h_fittedPulls.Draw("colz");
  h_truePulls.Draw("hist x same");
  h_fittedPulls_prof->Draw("pe same");
  upper.SetLeftMargin(0.15);
  upper.SetRightMargin(0.15);
  upper.SetBottomMargin(0.4);

  TPad lower("", "", 0, 0, 1.0, 1);
  lower.SetFillStyle(0);
  lower.cd();

  // a "pull" plot
  TGraph fitPulls;
  for (int bin = 1; bin <= h_fittedPulls_prof->GetNbinsX(); bin++)
  {
    double pull = 0;
    if (h_fittedPulls_prof->GetBinError(bin) != 0)
      pull = (h_fittedPulls_prof->GetBinContent(bin) - h_truePulls.GetBinContent(bin))
             / h_fittedPulls_prof->GetBinError(bin);
    fitPulls.SetPoint(fitPulls.GetN(), double(bin) - 0.5, pull);
  }
  fitPulls.Draw("al");
  *fitPulls.GetXaxis() = ax;
  fitPulls.SetTitle(";;#frac{True - Fit mean}{Fit RMS}");
  fitPulls.GetYaxis()->SetRangeUser(-0.95, 0.95);
  lower.SetLeftMargin(0.15);
  lower.SetRightMargin(0.15);
  lower.SetTopMargin(0.6);
  lower.SetBottomMargin(0.3);

  c.cd();
  upper.Draw();
  lower.Draw();
  TLegend leg(0.2, 0.9, 0.8, 1.0);
  leg.SetFillStyle(0);
  leg.SetBorderSize(0);
  leg.SetNColumns(3);
  leg.AddEntry(&h_truePulls, "True", "l");
  leg.AddEntry(&h_fittedPulls, "MCMC samples", "f");
  leg.AddEntry(h_fittedPulls_prof.get(), "MCMC (mean+RMS)", "lpe");
  leg.SetColumnSeparation(0.15);
  leg.Draw();

//  c.SetBottomMargin(0.5);
  c.SaveAs(mcmc_ana::FullFilename(dirPrefix, "pulls.systs.png").c_str());

}
