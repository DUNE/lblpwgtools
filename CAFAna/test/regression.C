#include "CAFAna/Analysis/common_fit_definitions.h"
using namespace ana;

#include "CAFAna/Analysis/AnalysisVersion.h"

#include "CAFAna/Analysis/AnalysisBinnings.h"
#include "CAFAna/Analysis/AnalysisVars.h"
#include "CAFAna/Analysis/Calcs.h"
#include "CAFAna/Analysis/CalcsNuFit.h"
#include "CAFAna/Analysis/Exposures.h"
#include "CAFAna/Analysis/Plots.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/SpectrumLoader.h"
#include "CAFAna/Core/Var.h"

#include "CAFAna/Cuts/AnaCuts.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/Experiment/MultiExperiment.h"
#include "CAFAna/Experiment/SingleSampleExperiment.h"

#include "CAFAna/Prediction/PredictionNoExtrap.h"
#include "CAFAna/Prediction/PredictionNoOsc.h"

#include "CAFAna/Vars/FitVars.h"

#include "CAFAna/Systs/DUNEFluxSysts.h"
#include "CAFAna/Systs/EnergySysts.h"
#include "CAFAna/Systs/FDRecoSysts.h"
#include "CAFAna/Systs/NDRecoSysts.h"
#include "CAFAna/Systs/NuOnESysts.h"
#include "CAFAna/Systs/Systs.h"
#include "CAFAna/Systs/XSecSysts.h"
#include "CAFAna/Systs/CrazyFluxFakeData.h"

#include "OscLib/IOscCalc.h"
#include "OscLib/OscCalcPMNSOpt.h"
#include "OscLib/OscCalcGeneral.h"

#include "StandardRecord/StandardRecord.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TMatrixDSym.h"
#include "TSystem.h"
#include "TTree.h"
#include "TPad.h"

namespace ana{

TMatrixD *GetNDCovMat(bool UseV3NDCovMat, bool TwoBeams, bool isFHC){

  auto AnaV = GetAnaVersion();

    const std::string detCovPath =
        "/pnfs/dune/persistent/users/LBL_TDR/CAFs/v4/";
#ifndef DONT_USE_FQ_HARDCODED_SYST_PATHS
    std::string covFileName =
        detCovPath + ((AnaV == kV3) ? "/Systs/det_sys_cov_v3binning.root"
                                    : "/det_sys_cov.root");
#else
    std::string covFileName =
        FindCAFAnaDir() + ((AnaV == kV3) ? "/Systs/det_sys_cov_v3binning.root"
                                         : "/Systs/det_sys_cov.root");
#endif

  std::string this_beam = "all";
  if(!TwoBeams){
    if(isFHC) this_beam = "fhc";
    else this_beam = "rhc";
  }
  // TDirectory *thisDir = gDirectory->CurrentDirectory();
  TFile covMatFile(covFileName.c_str());
  TString covObjectName = "nd_" + this_beam + "_frac_cov";
  TMatrixD *fake_uncorr = (TMatrixD *)covMatFile.Get(covObjectName);
  // TMatrixD *fake_uncorr = (TMatrixD *)covMatFile.Get("nd_all_frac_cov");

  fake_uncorr->Draw("colz");
  gPad->SetLogz();
  gPad->Print("mat.pdf");

  return fake_uncorr;
}

double RunFitPoint2(std::string stateFileName, std::string sampleString,
                   osc::IOscCalcAdjustable *fakeDataOsc,
                   SystShifts fakeDataSyst, bool fakeDataStats,
                   std::vector<const IFitVar *> oscVars,
                   std::vector<const ISyst *> systlist,
                   osc::IOscCalcAdjustable *fitOsc, SystShifts fitSyst,
                    ana::SeedList oscSeeds = ana::SeedList()) {

  //  assert(systlist.size() + oscVars.size());

  auto AnaV = GetAnaVersion();

  bool turbose = false;
  if (getenv("CAFANA_FIT_TURBOSE") &&
      bool(atoi(getenv("CAFANA_FIT_TURBOSE")))) {
    turbose = true;
  }

  static auto start_load = std::chrono::system_clock::now();

  static std::vector<std::unique_ptr<PredictionInterp>> interp_list =
    GetPredictionInterps(stateFileName, {});

  static PredictionInterp &predNDNumuFHC = *interp_list[4].release();
  static PredictionInterp &predNDNumuRHC = *interp_list[5].release();

  static auto end_load = std::chrono::system_clock::now();
  std::cerr << "[LOAD]: Done in "
            << std::chrono::duration_cast<std::chrono::seconds>(end_load -
                                                                start_load)
    .count()
            << " s " << BuildLogInfoString();

  // String parsing time!
  double pot_nd_fhc, pot_nd_rhc, pot_fd_fhc_nue, pot_fd_rhc_nue,
      pot_fd_fhc_numu, pot_fd_rhc_numu;
  ParseDataSamples(sampleString, pot_nd_fhc, pot_nd_rhc, pot_fd_fhc_nue,
                   pot_fd_rhc_nue, pot_fd_fhc_numu, pot_fd_rhc_numu);

  std::vector<seeded_spectra> *spectra = new std::vector<seeded_spectra>;

  if (!spectra->size()) {

    if (turbose) {
      std::cout << "[INFO]: Loading spectra. " << BuildLogInfoString()
                << std::endl;
    }

    std::vector<unsigned> seeds;
    for (size_t i = 0; i < 6; ++i) {
      seeds.push_back(gRandom->Integer(std::numeric_limits<unsigned>::max()));
    }

    (*spectra) = BuildSpectra(0, 0, 0, 0,
                              &predNDNumuFHC, &predNDNumuRHC,
                              fakeDataOsc, fakeDataSyst, fakeDataStats,
                              pot_fd_fhc_nue, pot_fd_fhc_numu, pot_fd_rhc_nue,
                              pot_fd_rhc_numu, pot_nd_fhc, pot_nd_rhc, seeds);
  }

  if (turbose) {
    std::cout << "[INFO]: Have fit spectra. " << BuildLogInfoString()
              << std::endl;
  }
  if (turbose) {
    std::cout << "[INFO]: Setting up experiments. " << BuildLogInfoString()
              << std::endl;
  }

  SingleSampleExperiment nd_expt_fhc(&predNDNumuFHC,
                                     *spectra->at(0/*kNDNumuFHC*/).spect);
  nd_expt_fhc.SetMaskHist(0.5, 10, 0, -1);

  SingleSampleExperiment nd_expt_rhc(&predNDNumuRHC,
                                     *spectra->at(1/*kNDNumuRHC*/).spect);
  nd_expt_rhc.SetMaskHist(0.5, 10, 0, -1);

  if (turbose) {
    std::cout << "[INFO]: Have experiments. " << BuildLogInfoString()
              << std::endl;
  }

  // Now sort out the experiment
  MultiExperiment this_expt;
  //  if (pot_nd_fhc > 0)
  //    this_expt.Add(&nd_expt_fhc);
  if (pot_nd_rhc > 0)
    this_expt.Add(&nd_expt_rhc);

  if (turbose) {
    std::cout << "[INFO]: Built multi-experiment " << BuildLogInfoString()
              << std::endl;
  }

  //  TMatrixD *fhc_ndmatrix = GetNDCovMat(false, false, true);
  TMatrixD *rhc_ndmatrix = GetNDCovMat(false, true/*false*/, false);
  //  nd_expt_fhc.AddCovarianceMatrix(fhc_ndmatrix, kCovMxChiSqPreInvert);
  nd_expt_rhc.AddCovarianceMatrix(rhc_ndmatrix, kCovMxChiSqPreInvert);

  auto start_fit = std::chrono::system_clock::now();
  // Now set up the fit itself
  std::cerr << "[INFO]: Beginning fit. " << BuildLogInfoString();
  MinuitFitter this_fit(&this_expt, oscVars, systlist);//, fitStrategy);
  double thischisq =
      this_fit.Fit(fitOsc, fitSyst, oscSeeds, {}, MinuitFitter::kVerbose)->EvalMetricVal();
  auto end_fit = std::chrono::system_clock::now();
  std::time_t end_fit_time = std::chrono::system_clock::to_time_t(end_fit);
  std::cerr << "[FIT]: Finished fit in "
            << std::chrono::duration_cast<std::chrono::seconds>(end_fit -
                                                                start_fit)
                   .count()
            << " s after " << this_fit.GetNFCN() << " iterations "
            << BuildLogInfoString();
  std::cerr << "   chisq = " << thischisq << std::endl;
  return thischisq;
}
}

void regression(std::string state = "/pnfs/dune/persistent/stash/LongBaseline/state_files/final_v4/State")
{
  // RunFitPoint needs both of these things. The osc is unused for our nd-only
  // test, and the systs are nominal (and unused in the fit).
  osc::IOscCalcAdjustable* dummyOsc = NuFitOscCalc(+1, 1, "0");
  SystShifts dummySyst;

  RunFitPoint2(state, "nd", dummyOsc, dummySyst, false, {}, {}, dummyOsc, dummySyst);
}
