#include "CAFAna/Analysis/common_fit_definitions.h"
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
#include "CAFAna/Experiment/CovarianceExperiment.h"

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

#ifdef USE_PREDINTERP_OMP
#include <omp.h>
#endif

#include <chrono>
#include <tuple>

using namespace ana;

unsigned gRNGSeed = 0;

// A smaller APA geometry was used to simmulate the FD due to computational requirements.
// Scale the simulated sample to match the actuall 40 kt detector.
const double scale_fdmc = 40 / 1.13;
// POT for 3.5 years
const double nom_years = 3.5;
const double pot_fd = POT120 * nom_years * scale_fdmc;
const double pot_nd = POT120 * nom_years;
// This is pretty annoying, but the above is for 7 years staged, which is 336 kT / MW yr
const double nom_exposure = 336.;

double GetBoundedGausThrow(double min, double max) {
  double val = -999;
  while (val > max || val < min)
    val = gRandom->Gaus();
  return val;
}

// For ease of penalty terms...
IExperiment *GetPenalty(int hie, int oct, std::string penalty,
                        std::string asimov_set, bool modConstraint) {

  // First, decide which to use
  std::vector<std::string> penalties = SplitString(penalty, ':');
  bool useTh13 = false;
  bool useDmsq32 = false;
  bool useTh23 = false;

  for (auto &str : penalties) {
    if (str == "th13" || str == "allpen")
      useTh13 = true;
    if (str == "dmsq32" || str == "allpen")
      useDmsq32 = true;
    if (str == "th23" || str == "allpen")
      useTh23 = true;
  }

  Penalizer_GlbLike *ret =
      new Penalizer_GlbLike(hie, oct, useTh13, useDmsq32, useTh23, 0);
  ret->SetAsimovPoint(asimov_set, modConstraint);
  return ret;
}

std::vector<const IFitVar *> GetOscVars(std::string oscVarString, int hie,
                                        int oct) {

  std::vector<std::string> osc_vars = SplitString(oscVarString, ':');

  std::vector<const IFitVar *> rtn_vars;

  for (auto &v : osc_vars) {
    if (v == "th13" || v == "alloscvars") {
      rtn_vars.push_back(&kFitTheta13);
    }
    // Deal with bounded dmsq32
    if (v == "dmsq32" || v == "alloscvars") {
      if (hie == -1)
        rtn_vars.push_back(&kFitDmSq32IHScaled);
      else if (hie == 1)
        rtn_vars.push_back(&kFitDmSq32NHScaled);
      else
        rtn_vars.push_back(&kFitDmSq32Scaled);
    }

    // Deal with octant boundaries
    if (v == "th23" || v == "alloscvars") {
      if (oct == -1)
        rtn_vars.push_back(&kFitSinSqTheta23LowerOctant);
      else if (oct == 1)
        rtn_vars.push_back(&kFitSinSqTheta23UpperOctant);
      else
        rtn_vars.push_back(&kFitSinSqTheta23);
    }
    if (v == "deltapi" || v == "alloscvars") {
      rtn_vars.push_back(&kFitDeltaInPiUnits);
    }

    // Add back in the 21 parameters
    if (v == "dmsq21" || v == "alloscvars") {
      rtn_vars.push_back(&kFitDmSq21Scaled);
    }
    if (v == "th12" || v == "alloscvars") {
      rtn_vars.push_back(&kFitSinSq2Theta12);
    }
    // Rho rho rho your boat...
    if (v == "rho" || v == "alloscvars") {
      rtn_vars.push_back(&kFitRho);
    }
  }
  return rtn_vars;
}

/*
  The behaviour os fake data dials is a bit clunky. Instead of the multiple spline 
  points used for normal dials, they have fixed values for non-zero values of the 
  dial. So, for some, +/-3, +/-2, +/-1 values of the dial will all give the same 
  value. Some are set so all negative (positive) values give a -3 (+3) sigma 
  variation, which is why there's some messing around in the function below.
  The spline behaviour is forced (for XSec dials) in XSecSyst::FakeDataDialShift, 
  which is called only when creating the input CAFs. This is almost certainly not 
  optimal, but is easy to extend for other dials.
 */
SystShifts GetFakeDataGeneratorSystShift(std::string input) {

  // Default to nominal
  SystShifts thisShift = kNoShift;

  if (input.empty() || input == "nominal")
    return thisShift;

  // Allow _pos and _neg endings 
  std::vector<std::string> split_input = SplitString(input, ':');
  std::vector<std::string> fake_data_names;
  std::vector<int> dial_vals;

  for (auto in_name : split_input){
    std::string name = in_name;
    int val = 1;

    if (in_name.compare(in_name.length()-4, 4, "_pos") == 0){
      name = in_name.substr(0, in_name.length()-4);
      val  = 1;
    } else if (in_name.compare(in_name.length()-4, 4, "_neg") == 0){
      name = in_name.substr(0, in_name.length()-4);
      val  = -1;
    } 

    // Check nobody did anything dumb...
    assert(IsFakeDataGenerationSyst(name) || IsCrazyFluxSyst(name));
    fake_data_names.push_back(name);
    dial_vals.push_back(val);
  }

  std::vector<ISyst const *> FDSyst = GetListOfSysts();
  KeepSysts(FDSyst, fake_data_names);

  for (uint i=0; i<FDSyst.size(); i++){
    thisShift.SetShift(FDSyst[i], dial_vals[i]);
  }

  return thisShift;
}

std::string GetSampleName(SampleType sample) {
  switch (sample) {
  case kFDFHC:
    return "FD_FHC";
  case kFDRHC:
    return "FD_RHC";
  case kNDFHC:
    return "ND_FHC";
  case kNDRHC:
    return "ND_RHC";
  case kNDNue:
    return "ND_nue";
  case kNDFHC_OA /*how you like this space, Callum?*/:
    return "ND_FHC_OA";
  case /* LOVE IT M8                */ kUnknown:
  default:
    return "NONE";
  }
}

SampleType GetSampleType(std::string const &sample) {

  if (sample == "FD_FHC")
    return kFDFHC;
  if (sample == "FD_RHC")
    return kFDRHC;
  if (sample == "ND_FHC")
    return kNDFHC;
  if (sample == "ND_RHC")
    return kNDRHC;
  if (sample == "ND_nue")
    return kNDNue;
  if (sample == "ND_FHC_OA")
    return kNDFHC_OA;
  return kUnknown;
}

void MakePredictionInterp(TDirectory *saveDir, SampleType sample,
                          std::vector<const ISyst *> systlist,
                          AxisBlob const &axes,
                          std::vector<std::string> const &non_swap_file_list,
                          std::vector<std::string> const &nue_swap_file_list,
                          std::vector<std::string> const &tau_swap_file_list,
                          int max) {

  bool use_cv_weights = true;
  if (getenv("CAFANA_IGNORE_CV_WEIGHT")) {
    use_cv_weights = !atoi(getenv("CAFANA_IGNORE_CV_WEIGHT"));
  }

  bool use_selection = true;
  if (getenv("CAFANA_IGNORE_SELECTION")) {
    use_selection = !atoi(getenv("CAFANA_IGNORE_SELECTION"));
  }

  // Move to the save directory
  saveDir->cd();
  osc::IOscCalcAdjustable *this_calc = NuFitOscCalc(1);

  bool isfhc =
      ((sample == kNDFHC) || (sample == kNDFHC_OA) || (sample == kFDFHC));

  // FD samples
  if ((sample == kFDFHC) || (sample == kFDRHC)) {

    Loaders these_loaders;
    SpectrumLoader loaderNumu(non_swap_file_list, max);
    SpectrumLoader loaderNue(nue_swap_file_list, max);
    SpectrumLoader loaderNutau(tau_swap_file_list, max);

    these_loaders.AddLoader(&loaderNumu, caf::kFARDET, Loaders::kMC,
                            Loaders::kNonSwap);
    these_loaders.AddLoader(&loaderNue, caf::kFARDET, Loaders::kMC,
                            Loaders::kNueSwap);
    these_loaders.AddLoader(&loaderNutau, caf::kFARDET, Loaders::kMC,
                            Loaders::kNuTauSwap);

    NoExtrapPredictionGenerator genFDNumu(
        *axes.FDAx_numu,
        use_selection ? kPassFD_CVN_NUMU && kIsTrueFV : kIsTrueFV,
        use_cv_weights ? kCVXSecWeights : kUnweighted);
    NoExtrapPredictionGenerator genFDNue(
        *axes.FDAx_nue,
        use_selection ? kPassFD_CVN_NUE && kIsTrueFV : kIsTrueFV,
        use_cv_weights ? kCVXSecWeights : kUnweighted);
    PredictionInterp predInterpFDNumu(systlist, this_calc, genFDNumu,
                                      these_loaders);
    PredictionInterp predInterpFDNue(systlist, this_calc, genFDNue,
                                     these_loaders);
    these_loaders.Go();

    std::cout << "Saving " << GetSampleName(sample) << std::endl;
    predInterpFDNumu.SaveTo(saveDir,
                            std::string("fd_interp_numu_") + std::string(isfhc ? "fhc" : "rhc"));
    std::cout << "Saving " << GetSampleName(sample) << std::endl;
    predInterpFDNue.SaveTo(saveDir,
                           std::string("fd_interp_nue_") + std::string(isfhc ? "fhc" : "rhc"));

  } else if ((sample == kNDFHC) || (sample == kNDRHC) ||
             (sample == kNDFHC_OA)) {

    // Now ND
    Loaders these_loaders;
    SpectrumLoader loaderNumu(non_swap_file_list, max);
    these_loaders.AddLoader(&loaderNumu, caf::kNEARDET, Loaders::kMC);

    NoOscPredictionGenerator genNDNumu(
        *axes.NDAx,
        use_selection
            ? (isfhc ? kPassND_FHC_NUMU : kPassND_RHC_NUMU) && kIsTrueFV
            : kIsTrueFV,
        use_cv_weights ? kCVXSecWeights : kUnweighted);

    PredictionInterp predInterpNDNumu(systlist, this_calc, genNDNumu,
                                      these_loaders);
    these_loaders.Go();

    std::cout << "Saving " << GetSampleName(sample) << std::endl;
    predInterpNDNumu.SaveTo(saveDir,
                            std::string("nd_interp_numu_") + std::string(isfhc ? "fhc" : "rhc"));
  }
}

std::vector<std::string> const sample_dir_order = {
    "fd_interp_numu_fhc", "fd_interp_nue_fhc",  "fd_interp_numu_rhc",
    "fd_interp_nue_rhc",  "nd_interp_numu_fhc", "nd_interp_numu_rhc"};
std::vector<std::string> const sample_suffix_order = {
    "FD_FHC", "FD_FHC", "FD_RHC", "FD_RHC", "ND_FHC", "ND_RHC"};

std::vector<std::unique_ptr<ana::PredictionInterp>>
GetPredictionInterps(std::string fileName,
                     std::vector<const ISyst *> systlist) {

  // Make sure the syst registry has been populated with all the systs we could
  // want to use
  static std::vector<const ISyst *> systs = GetListOfSysts();

  // Hackity hackity hack hack
  bool fileNameIsStub = (fileName.find(".root") == std::string::npos);

  std::vector<std::unique_ptr<PredictionInterp>> return_list;

  for (size_t s_it = 0; s_it < sample_dir_order.size(); ++s_it) {
    std::string state_fname =
        fileNameIsStub ? fileName + "_" + sample_suffix_order[s_it] + ".root"
                       : fileName;
    TFile *fin =
        TFile::Open(state_fname.c_str(), "READ"); // Allows xrootd streaming
    assert(fin && !fin->IsZombie());
    std::cout << "[LOAD]: Retrieving " << sample_dir_order[s_it] << " from "
              << state_fname << ":" << sample_dir_order[s_it] << std::endl;
    return_list.emplace_back(LoadFrom<PredictionInterp>(fin, sample_dir_order[s_it]));
    delete fin;

    std::vector<ana::ISyst const *> systs_to_remove =
        return_list.back()->GetAllSysts();
    std::vector<std::string> used_syst_names;
    for (auto s : systlist) {
      used_syst_names.push_back(s->ShortName());
    }
    RemoveSysts(systs_to_remove, used_syst_names);
    if (systs_to_remove.size()) {
      return_list.back()->DiscardSysts(systs_to_remove);
    }
  }
  return return_list;
}

TH2D *make_corr_from_covar(TH2D *covar) {

  TH2D *corr = (TH2D *)covar->Clone();
  corr->SetNameTitle("corr", "corr");

  for (int i = 0; i < covar->GetNbinsX(); ++i) {
    double istddev = sqrt(covar->GetBinContent(i + 1, i + 1));
    for (int j = 0; j < covar->GetNbinsX(); ++j) {
      double jstddev = sqrt(covar->GetBinContent(j + 1, j + 1));
      double new_corr = covar->GetBinContent(i + 1, j + 1) / istddev / jstddev;
      corr->SetBinContent(i + 1, j + 1, new_corr);
    }
  }
  return corr;
}

// Yet another string parser that does far too much. I can't be stopped!
void ParseDataSamples(std::string cmdLineInput, 
                      double &pot_nd_fhc, double &pot_nd_rhc,
                      double &pot_fd_fhc_nue, double &pot_fd_rhc_nue,
                      double &pot_fd_fhc_numu, double &pot_fd_rhc_numu) {

  // Did somebody say overextend the command line arguments even further?
  // Well okay!
  std::vector<std::string> input_vect = SplitString(cmdLineInput, ':');

  // Default to 7 years staged. Value is actually in kt MW yr
  double exposure = nom_exposure;
  if (input_vect.size() > 1)
    exposure = stod(input_vect[1]);
  std::string input = input_vect[0];

  // LoWeR cAsE sO i CaN bE sIlLy WiTh InPuTs
  std::transform(input.begin(), input.end(), input.begin(), ::tolower);

  // Look for some other magic information
  for (auto str : input_vect) {
    if (str.find("full") != std::string::npos or
        str.find("15year") != std::string::npos)
      exposure = 1104;

    if (str.find("nom") != std::string::npos or
        str.find("7year") != std::string::npos)
      exposure = 336;

    if (str.find("10year") != std::string::npos)
      exposure = 624;
  }

  double exposure_ratio = exposure / nom_exposure;

  // Now sort out which samples to include                                                                                                             
  pot_nd_fhc = pot_nd_rhc = 0;
  pot_fd_fhc_nue = pot_fd_fhc_numu = 0;
  pot_fd_rhc_nue = pot_fd_rhc_numu = 0;

  // Hacky McHackerson is here to stay!                                                                                                                
  if (input.find("nd") != std::string::npos) {
    pot_nd_fhc = pot_nd * exposure_ratio;
    pot_nd_rhc = pot_nd * exposure_ratio;
  }
  if (input.find("fd") != std::string::npos) {
    pot_fd_fhc_nue = pot_fd_fhc_numu = pot_fd * exposure_ratio;
    pot_fd_rhc_nue = pot_fd_rhc_numu = pot_fd * exposure_ratio;
  }

  // Now allow specific subsets
  if (input.find("fhc") != std::string::npos) {
    pot_nd_rhc = pot_fd_rhc_nue = pot_fd_rhc_numu = 0;
  }

  if (input.find("rhc") != std::string::npos) {
    pot_nd_fhc = pot_fd_fhc_nue = pot_fd_fhc_numu = 0;
  }

  if (input.find("numu") != std::string::npos) {
    pot_fd_fhc_nue = pot_fd_rhc_nue = 0;
  }

  if (input.find("nue") != std::string::npos) {
    pot_fd_fhc_numu = pot_fd_rhc_numu = 0;
  }
  return;
}

void ParseThrowInstructions(std::string throwString, bool &stats, bool &fakeOA,
                            bool &fakeNuis, bool &start, bool &central) {

  std::vector<std::string> instructions = SplitString(throwString, ':');

  stats = false;
  fakeOA = false;
  fakeNuis = false;
  start = false;
  central = false;

  for (auto &str : instructions) {

    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    if (str == "stat" || str == "all")
      stats = true;
    if (str == "fake" || str == "all")
      fakeOA = fakeNuis = true;
    if (str == "fakeoa" || str == "all")
      fakeOA = true;
    if (str == "fakenuis" || str == "all")
      fakeNuis = true;
    if (str == "start" || str == "all")
      start = true;
    if (str == "central" || str == "all")
      central = true;
  }
  return;
}

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
  if (!fake_uncorr) {
    std::cout << "Could not obtain covariance matrix named "
    << covObjectName <<  " from " << covFileName << std::endl;
    abort();
  }

  if(!UseV3NDCovMat){
    return fake_uncorr;
  }
  else{
    std::cout << "[INFO]: Using v3-like ND covmat treadment." << std::endl;

    TMatrixD *covmx_fhc_only = (TMatrixD *)covMatFile.Get("nd_fhc_frac_cov");

    assert(fake_uncorr->GetNrows() == 2 * covmx_fhc_only->GetNrows());

    size_t NRows = fake_uncorr->GetNrows();
    size_t NRows_FHC = covmx_fhc_only->GetNrows();

    for (size_t row_it = 0; row_it < NRows; ++row_it) {
      for (size_t col_it = 0; col_it < NRows; ++col_it) {

      // Could use TMatrix::SetSub but I don't trust TMatrix...
        if (((row_it >= NRows_FHC) && (col_it < NRows_FHC)) ||
          ((row_it < NRows_FHC) && (col_it >= NRows_FHC))) {
          (*fake_uncorr)[row_it][col_it] = 0;}
        else {
          size_t row_fhc_only_it = row_it % NRows_FHC;
          size_t col_fhc_only_it = col_it % NRows_FHC;
          (*fake_uncorr)[row_it][col_it] =
          (*covmx_fhc_only)[row_fhc_only_it][col_fhc_only_it];
        }
      }
    }

    return fake_uncorr;
  }
}

TMatrixD *MakeCovmat(PredictionInterp const &prediction,
                     std::vector<ISyst const *> const &systs,
                     osc::IOscCalcAdjustable *calc, size_t NToys,
                     TDirectory *outdir) {
  std::vector<std::vector<double>> ThrownSpectra;
  std::vector<double> MeanSpectra;

  SystShifts shift;

  if (outdir) {
    outdir->cd();
    std::unique_ptr<TH1> nominal_spectra(
        prediction.PredictSyst(calc, kNoShift).ToTH1(1));
    nominal_spectra->Write("nominal_throw_spectra");
    nominal_spectra->SetDirectory(nullptr);
  }

  size_t NBins = std::numeric_limits<size_t>::max();
  for (size_t t_it = 0; t_it < NToys; ++t_it) { // throws

    shift.ResetToNominal();
    // Throw new param values
    for (auto s : systs) {
      double v = GetBoundedGausThrow(s->Min(), s->Max());
      shift.SetShift(s, v);
    }

    // Make prediction TH1
    std::unique_ptr<TH1> thrown_spectra(
        prediction.PredictSyst(calc, shift).ToTH1(1));
    if (outdir) {
      thrown_spectra->Write(
          (std::string("thrown_spectra_") + std::to_string(t_it)).c_str());
    }
    thrown_spectra->SetDirectory(nullptr);

    if (NBins == std::numeric_limits<size_t>::max()) {
      NBins = thrown_spectra->GetXaxis()->GetNbins();

      ThrownSpectra = std::vector<std::vector<double>>(
          NToys, std::vector<double>(NBins, 0));
      MeanSpectra = std::vector<double>(NBins, 0);
    }

    for (size_t bi_it = 0; bi_it < NBins; ++bi_it) { // spectra bins
      double bin_throw = thrown_spectra->GetBinContent(bi_it + 1);
      MeanSpectra[bi_it] += bin_throw;
      ThrownSpectra[t_it][bi_it] = bin_throw;
    }
  }

  for (double &bc : MeanSpectra) {
    bc /= double(NToys);
  }

  // Build covmat
  TMatrixD *mat = new TMatrixD(NBins, NBins);

  for (size_t rbi_it = 0; rbi_it < NBins; ++rbi_it) {
    for (size_t cbi_it = 0; cbi_it < NBins; ++cbi_it) {
      mat->operator()(rbi_it, cbi_it) = 0;
    }
  }
  double nthrow_fact = 1.0 / double(NToys - 1);

  for (size_t t_it = 0; t_it < NToys; ++t_it) { // throws
    for (size_t rbi_it = 0; rbi_it < NBins; ++rbi_it) {
      for (size_t cbi_it = 0; cbi_it < NBins; ++cbi_it) {
        double rdiff = (ThrownSpectra[t_it][rbi_it] / MeanSpectra[rbi_it]) - 1;
        double cdiff = (ThrownSpectra[t_it][cbi_it] / MeanSpectra[cbi_it]) - 1;
        mat->operator()(rbi_it, cbi_it) += (rdiff * cdiff * nthrow_fact);
      }
    }
  }
  return mat;
}

void SaveTrueOAParams(TDirectory *outDir, osc::IOscCalcAdjustable *calc,
                      std::string tree_name) {

  outDir->cd();
  double L = calc->GetL();
  double rho = calc->GetRho();
  double dmsq21 = calc->GetDmsq21();
  double dmsq32 = calc->GetDmsq32();
  double th12 = calc->GetTh12();
  double th13 = calc->GetTh13();
  double th23 = calc->GetTh23();
  double ssth23 = sin(calc->GetTh23()) * sin(calc->GetTh23());
  double deltapi = calc->GetdCP() / TMath::Pi();

  TTree *tree = new TTree(tree_name.c_str(), tree_name.c_str());
  tree->Branch("L", &L);
  tree->Branch("rho", &rho);
  tree->Branch("dmsq21", &dmsq21);
  tree->Branch("dmsq32", &dmsq32);
  tree->Branch("th12", &th12);
  tree->Branch("th13", &th13);
  tree->Branch("th23", &th23);
  tree->Branch("ssth23", &ssth23);
  tree->Branch("deltapi", &deltapi);
  tree->Fill();
  tree->Write();
  return;
}

void SaveParams(TDirectory *outDir, std::vector<const ISyst *> systlist) {
  outDir->cd();
  std::vector<std::string> *fParamNames = new std::vector<std::string>();
  for (auto it : systlist)
    fParamNames->push_back(it->ShortName());
  TTree *t = new TTree("meta", "Parameter meta-data");
  t->Branch("fParamNames", &fParamNames);
  t->Fill();
  t->Write();
  delete fParamNames;
  delete t;
}

FitTreeBlob::FitTreeBlob(std::string tree_name, std::string meta_tree_name)
    : fMeta_filled(false), throw_tree(nullptr), meta_tree(nullptr), fNFills(0) {

  fFakeDataVals = new std::vector<double>();
  fParamNames = new std::vector<std::string>();
  fPreFitValues = new std::vector<double>();
  fPreFitErrors = new std::vector<double>();
  fPostFitValues = new std::vector<double>();
  fPostFitErrors = new std::vector<double>();
  fCentralValues = new std::vector<double>();
  fEnvVarNames = new std::vector<std::string>();
  fEnvVarValues = new std::vector<std::string>();
  fSpectraRNGSeeds = new std::vector<unsigned>();

  TDirectory *odir = gDirectory;

  if (tree_name.size()) {

    throw_tree = new TTree(tree_name.c_str(), "Fit information");
    throw_tree->SetAutoSave(10);
    throw_tree->Branch("chisq", &fChiSq);
    throw_tree->Branch("NSeconds", &fNSeconds);
    throw_tree->Branch("ResMemUsage", &fResMemUsage);
    throw_tree->Branch("VirtMemUsage", &fVirtMemUsage);
    throw_tree->Branch("NFCN", &fNFCN);
    throw_tree->Branch("EDM", &fEDM);
    throw_tree->Branch("IsValid", &fIsValid);
    throw_tree->Branch("fFakeDataVals", &fFakeDataVals);
    throw_tree->Branch("fPreFitValues", &fPreFitValues);
    throw_tree->Branch("fPreFitErrors", &fPreFitErrors);
    throw_tree->Branch("fPostFitValues", &fPostFitValues);
    throw_tree->Branch("fPostFitErrors", &fPostFitErrors);
    throw_tree->Branch("fCentralValues", &fCentralValues);
    throw_tree->Branch("LoopRNGSeed", &fLoopRNGSeed);
    throw_tree->Branch("JobRNGSeed", &fJobRNGSeed);
    throw_tree->Branch("SpectraRNGSeeds", &fSpectraRNGSeeds);
    throw_tree->Branch("ProcFitN", &fNFills);
    throw_tree->Branch("NOscSeeds", &fNOscSeeds);

    if (meta_tree_name.size()) {

      meta_tree = new TTree(meta_tree_name.c_str(), "Parameter meta-data");
      meta_tree->SetAutoSave(10);
      meta_tree->Branch("fParamNames", &fParamNames);
      meta_tree->Branch("fEnvVarNames", &fEnvVarNames);
      meta_tree->Branch("fEnvVarValues", &fEnvVarValues);
      meta_tree->Branch("JobRNGSeed", &fJobRNGSeed);
#ifdef USE_PREDINTERP_OMP
      meta_tree->Branch("NMaxThreads", &fNMaxThreads);
#endif
    }
  }

  if (odir) {
    odir->cd();
  }
}

FitTreeBlob *FitTreeBlob::MakeReader(TTree *t, TTree *m) {
  FitTreeBlob *ftb = new FitTreeBlob();
  t->SetBranchAddress("chisq", &ftb->fChiSq);
  t->SetBranchAddress("NSeconds", &ftb->fNSeconds);
  t->SetBranchAddress("ResMemUsage", &ftb->fResMemUsage);
  t->SetBranchAddress("VirtMemUsage", &ftb->fVirtMemUsage);
  t->SetBranchAddress("NFCN", &ftb->fNFCN);
  t->SetBranchAddress("EDM", &ftb->fEDM);
  t->SetBranchAddress("IsValid", &ftb->fIsValid);
  t->SetBranchAddress("fFakeDataVals", &ftb->fFakeDataVals);
  t->SetBranchAddress("fPreFitValues", &ftb->fPreFitValues);
  t->SetBranchAddress("fPreFitErrors", &ftb->fPreFitErrors);
  t->SetBranchAddress("fPostFitValues", &ftb->fPostFitValues);
  t->SetBranchAddress("fPostFitErrors", &ftb->fPostFitErrors);
  t->SetBranchAddress("fCentralValues", &ftb->fCentralValues);
  t->SetBranchAddress("LoopRNGSeed", &ftb->fLoopRNGSeed);
  t->SetBranchAddress("JobRNGSeed", &ftb->fJobRNGSeed);
  t->SetBranchAddress("SpectraRNGSeeds", &ftb->fSpectraRNGSeeds);
  t->SetBranchAddress("ProcFitN", &ftb->fNFills);
  t->SetBranchAddress("NOscSeeds", &ftb->fNOscSeeds);

  m->SetBranchAddress("fParamNames", &ftb->fParamNames);
  m->SetBranchAddress("fEnvVarNames", &ftb->fEnvVarNames);
  m->SetBranchAddress("fEnvVarValues", &ftb->fEnvVarValues);
  m->SetBranchAddress("JobRNGSeed", &ftb->fJobRNGSeed);
#ifdef USE_PREDINTERP_OMP
  m->SetBranchAddress("NMaxThreads", &ftb->fNMaxThreads);
#endif
  return ftb;
}
FitTreeBlob::~FitTreeBlob() {
  delete fFakeDataVals;
  delete fParamNames;
  delete fPreFitValues;
  delete fPreFitErrors;
  delete fPostFitValues;
  delete fPostFitErrors;
  delete fCentralValues;
  delete fEnvVarNames;
  delete fEnvVarValues;
  delete fSpectraRNGSeeds;
}
void FitTreeBlob::CopyVals(FitTreeBlob const &fb) {
  (*fFakeDataVals) = (*fb.fFakeDataVals);
  (*fParamNames) = (*fb.fParamNames);
  (*fEnvVarNames) = (*fb.fEnvVarNames);
  (*fEnvVarValues) = (*fb.fEnvVarValues);
  (*fPreFitValues) = (*fb.fPreFitValues);
  (*fPreFitErrors) = (*fb.fPreFitErrors);
  (*fPostFitValues) = (*fb.fPostFitValues);
  (*fPostFitErrors) = (*fb.fPostFitErrors);
  (*fCentralValues) = (*fb.fCentralValues);
  (*fSpectraRNGSeeds) = (*fb.fSpectraRNGSeeds);
  fChiSq = fb.fChiSq;
  fNFCN = fb.fNFCN;
  fEDM = fb.fEDM;
  fIsValid = fb.fIsValid;
  fNSeconds = fb.fNSeconds;
  fResMemUsage = fb.fResMemUsage;
  fVirtMemUsage = fb.fVirtMemUsage;
  fLoopRNGSeed = fb.fLoopRNGSeed;
  fJobRNGSeed = fb.fJobRNGSeed;
  fNFills = fb.fNFills;
  fNOscSeeds = fb.fNOscSeeds;
#ifdef USE_PREDINTERP_OMP
  fNMaxThreads = fb.fNMaxThreads;
#endif
}
void FitTreeBlob::Fill() {
  if (throw_tree) {
    throw_tree->Fill();
  }

  if (meta_tree && !fMeta_filled) {
    meta_tree->Fill();
    fMeta_filled = true;
  }

  fNFills++;
}
void FitTreeBlob::SetDirectory(TDirectory *d) {
  if (throw_tree) {
    throw_tree->SetDirectory(d);
  }
  if (meta_tree) {
    meta_tree->SetDirectory(d);
  }
}
void FitTreeBlob::SetDirectoryClone(TDirectory *d) {
  if (!d) {
    return;
  }
  if (throw_tree) {
    TTree *clone = throw_tree->CloneTree();
    clone->ResetBranchAddresses();
    clone->SetDirectory(d);
    throw_tree->SetDirectory(nullptr);
  }
  if (meta_tree) {
    TTree *clone = meta_tree->CloneTree();
    clone->ResetBranchAddresses();
    clone->SetDirectory(d);
    meta_tree->SetDirectory(nullptr);
  }
}
void FitTreeBlob::Write() {
  if (throw_tree) {
    throw_tree->Write();
  }
  if (meta_tree) {
    meta_tree->Write();
  }
}
void FitTreeBlob::WriteClone() {
  if (throw_tree) {
    TTree *clone = throw_tree->CloneTree();
    clone->ResetBranchAddresses();
    clone->Write();
    throw_tree->SetDirectory(nullptr);
  }
  if (meta_tree) {
    TTree *clone = meta_tree->CloneTree();
    clone->ResetBranchAddresses();
    clone->Write();
    meta_tree->SetDirectory(nullptr);
  }
}

std::string BuildLogInfoString() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  ProcInfo_t procinfo;
  gSystem->GetProcInfo(&procinfo);
  MemInfo_t meminfo;
  gSystem->GetMemInfo(&meminfo);

  std::stringstream ss("");
  ss << " (ProcResMem: " << (double(procinfo.fMemResident) / 1E6) << " GB) @ "
     << std::ctime(&now_time);

  return ss.str();
}

Spectrum MockOrAsimov(const Spectrum& s, bool mock, double pot, int seed)
{
  if(mock > 0) return s.MockData(pot, seed); else return s.AsimovData(pot);
}

std::vector<seeded_spectra>
BuildSpectra(PredictionInterp *predFDNumuFHC, PredictionInterp *predFDNueFHC,
             PredictionInterp *predFDNumuRHC, PredictionInterp *predFDNueRHC,
             PredictionInterp *predNDNumuFHC, PredictionInterp *predNDNumuRHC,
             osc::IOscCalcAdjustable *fakeDataOsc,
             SystShifts fakeDataSyst, bool fakeDataStats, double pot_fd_fhc_nue,
             double pot_fd_fhc_numu, double pot_fd_rhc_nue,
             double pot_fd_rhc_numu, double pot_nd_fhc, double pot_nd_rhc,
             std::vector<unsigned> const &Seeds) {
  std::vector<seeded_spectra> spectra;
  size_t iseed = 0;

  // Ordering of these is important
  // kFDNueFHC
  if (predFDNueFHC) {
    spectra.emplace_back(Seeds[iseed],
                         std::make_unique<Spectrum>(MockOrAsimov(predFDNueFHC->PredictSyst(fakeDataOsc, fakeDataSyst), fakeDataStats, pot_fd_fhc_nue, Seeds[iseed])));
    iseed++;
  }
  // kFDNumuFHC
  if (predFDNumuFHC) {
    spectra.emplace_back(Seeds[iseed],
                         std::make_unique<Spectrum>(MockOrAsimov(predFDNumuFHC->PredictSyst(fakeDataOsc, fakeDataSyst), fakeDataStats, pot_fd_fhc_numu, Seeds[iseed])));
    iseed++;
  }
  // kFDNueRHC
  if (predFDNueRHC) {
    spectra.emplace_back(Seeds[iseed],
                         std::make_unique<Spectrum>(MockOrAsimov(predFDNueRHC->PredictSyst(fakeDataOsc, fakeDataSyst), fakeDataStats, pot_fd_rhc_nue, Seeds[iseed])));
    iseed++;
  }
  // kFDNumuRHC
  if (predFDNumuRHC) {
    spectra.emplace_back(Seeds[iseed],
                         std::make_unique<Spectrum>(MockOrAsimov(predFDNumuRHC->PredictSyst(fakeDataOsc, fakeDataSyst), fakeDataStats, pot_fd_rhc_numu, Seeds[iseed])));
    iseed++;
  }
  // kNDNumuFHC
  if (predNDNumuFHC) {
    spectra.emplace_back(Seeds[iseed],
                         std::make_unique<Spectrum>(MockOrAsimov(predNDNumuFHC->PredictSyst(fakeDataOsc, fakeDataSyst), fakeDataStats, pot_nd_fhc, Seeds[iseed])));
    iseed++;
  }
  // kNDNumuRHC
  if (predNDNumuRHC) {
    spectra.emplace_back(Seeds[iseed],
                         std::make_unique<Spectrum>(MockOrAsimov(predNDNumuRHC->PredictSyst(fakeDataOsc, fakeDataSyst), fakeDataStats, pot_nd_rhc, Seeds[iseed])));
    iseed++;
  }

  return spectra;
}

double RunFitPoint(std::string stateFileName, std::string sampleString,
                   osc::IOscCalcAdjustable *fakeDataOsc,
                   SystShifts fakeDataSyst, bool fakeDataStats,
                   std::vector<const IFitVar *> oscVars,
                   std::vector<const ISyst *> systlist,
                   osc::IOscCalcAdjustable *fitOsc, SystShifts fitSyst,
                   ana::SeedList oscSeeds, IExperiment *penaltyTerm,
                   MinuitFitter::FitOpts fitStrategy, TDirectory *outDir,
                   FitTreeBlob *PostFitTreeBlob,
                   std::vector<seeded_spectra> *spectra, SystShifts &bf) {

  assert(systlist.size() + oscVars.size());

  auto AnaV = GetAnaVersion();

  bool turbose = false;
  if (getenv("CAFANA_FIT_TURBOSE") &&
      bool(atoi(getenv("CAFANA_FIT_TURBOSE")))) {
    turbose = true;
  }

#ifdef USE_PREDINTERP_OMP
  size_t maxthreads = omp_get_max_threads();
  if (maxthreads > 4) {
    std::cerr << "[INFO]: Cannot run with OMP_NUM_THREADS > 4" << std::endl;
    abort();
  }
  if (PostFitTreeBlob) {
    PostFitTreeBlob->fNMaxThreads = maxthreads;
  }
#endif

  // Start by getting the PredictionInterps... better that this is done here
  // than elsewhere as they aren't smart enough to know what they are (so the
  // order matters) Note that all systs are used to load the PredictionInterps
  static bool first_load = true;
  static auto start_load = std::chrono::system_clock::now();

  static bool PI_load = true;
  static std::vector<ana::ISyst const *> syststoload = systlist;
  if (PI_load) {

    for (auto &env_str :
         {"CAFANA_ANALYSIS_VERSION", "CAFANA_USE_UNCORRNDCOVMAT",
          "CAFANA_USE_NDCOVMAT", "CAFANA_IGNORE_CV_WEIGHT",
          "CAFANA_IGNORE_SELECTION", "CAFANA_DISABLE_DERIVATIVES",
          "CAFANA_DONT_CLAMP_SYSTS", "CAFANA_FIT_TURBOSE",
          "CAFANA_FIT_FORCE_HESSE", "CAFANA_PRED_MINMCSTATS", "FIT_PRECISION",
          "FIT_TOLERANCE", "SLURM_JOB_ID", "SLURM_PROCID", "SLURM_NODEID",
          "SLURM_LOCALID"}) {
      if (getenv(env_str)) {
        std::cout << "[ENV]: " << env_str << " = " << getenv(env_str)
                  << std::endl;
        if (PostFitTreeBlob) {
          PostFitTreeBlob->fEnvVarNames->push_back(env_str);
          PostFitTreeBlob->fEnvVarValues->push_back(getenv(env_str));
        }
      }
    }

    std::vector<ana::ISyst const *> fdlos =
        GetListOfSysts(false, false, false, false, false, false,
                       true /*add fake data*/, false);
    syststoload.insert(syststoload.end(), fdlos.begin(), fdlos.end());
    PI_load = false;
  }

  static std::vector<std::unique_ptr<PredictionInterp>> interp_list =
      GetPredictionInterps(stateFileName, syststoload);

  static PredictionInterp &predFDNumuFHC = *interp_list[0].release();
  static PredictionInterp &predFDNueFHC = *interp_list[1].release();
  static PredictionInterp &predFDNumuRHC = *interp_list[2].release();
  static PredictionInterp &predFDNueRHC = *interp_list[3].release();
  static PredictionInterp &predNDNumuFHC = *interp_list[4].release();
  static PredictionInterp &predNDNumuRHC = *interp_list[5].release();
  if (first_load) {
    static auto end_load = std::chrono::system_clock::now();
    std::cerr << "[LOAD]: Done in "
              << std::chrono::duration_cast<std::chrono::seconds>(end_load -
                                                                  start_load)
                     .count()
              << " s " << BuildLogInfoString();
    first_load = false;
  }

  // String parsing time!
  double pot_nd_fhc, pot_nd_rhc, pot_fd_fhc_nue, pot_fd_rhc_nue,
      pot_fd_fhc_numu, pot_fd_rhc_numu;
  ParseDataSamples(sampleString, pot_nd_fhc, pot_nd_rhc, pot_fd_fhc_nue,
                   pot_fd_rhc_nue, pot_fd_fhc_numu, pot_fd_rhc_numu);

  // If a directory has been given, a whole mess of stuff will be saved there.
  if (outDir) {
    outDir->cd();
  }

  // Need to make a fake throw list for later book-keeping
  std::vector<double> fFakeDataVals;
  for (const IFitVar *v : oscVars)
    fFakeDataVals.push_back(v->GetValue(fakeDataOsc));
  for (const ISyst *s : systlist)
    fFakeDataVals.push_back(fakeDataSyst.GetShift(s));

  // One problem with this method is that the experiments are reproduced for
  // every single call... Set up the fake data histograms, and save them if
  // relevant... This came back to bite me. I need to do multiple fits with the
  // same fake data throw, so I need these to persist between calls to this
  // function Sadly, the locked down ownership etc means I can't think of a
  // better solution to this And the alternative CAFAna is probably just
  // copy-pasta-ing this function in all of the scripts that use it...
  std::vector<seeded_spectra> LastShredsOfMyDignityAndSanity;
  if (!spectra) {
    spectra = &LastShredsOfMyDignityAndSanity;
  }

  if (!spectra->size()) {

    if (turbose) {
      std::cout << "[INFO]: Loading spectra. " << BuildLogInfoString()
                << std::endl;
    }

    std::vector<unsigned> seeds;
    for (size_t i = 0; i < 6; ++i) {
      seeds.push_back(gRandom->Integer(std::numeric_limits<unsigned>::max()));
    }

    (*spectra) = BuildSpectra(&predFDNumuFHC, &predFDNueFHC, &predFDNumuRHC,
                              &predFDNueRHC, &predNDNumuFHC, &predNDNumuRHC,
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

  // If using the multi sample covariances then they must be added to the
  // MultiExperiment
  SingleSampleExperiment app_expt_fhc(&predFDNueFHC,
                                      *spectra->at(kFDNueFHC).spect);
  app_expt_fhc.SetMaskHist(0.5, (AnaV == kV4) ? 10 : 8);

  SingleSampleExperiment dis_expt_fhc(&predFDNumuFHC,
                                      *spectra->at(kFDNumuFHC).spect);
  dis_expt_fhc.SetMaskHist(0.5, (AnaV == kV4) ? 10 : 8);

  SingleSampleExperiment app_expt_rhc(&predFDNueRHC,
                                      *spectra->at(kFDNueRHC).spect);
  app_expt_rhc.SetMaskHist(0.5, (AnaV == kV4) ? 10 : 8);

  SingleSampleExperiment dis_expt_rhc(&predFDNumuRHC,
                                      *spectra->at(kFDNumuRHC).spect);
  dis_expt_rhc.SetMaskHist(0.5, (AnaV == kV4) ? 10 : 8);

  IExperiment *nd_expt_fhc = 0, *nd_expt_rhc = 0;
  CovarianceExperiment* nd_expt_joint = 0;

  bool UseNDCovMat = true;
  if (getenv("CAFANA_USE_NDCOVMAT")) {
    UseNDCovMat = bool(atoi(getenv("CAFANA_USE_NDCOVMAT")));
  }

  bool UseV3NDCovMat = (AnaV == kV3);
  if (getenv("CAFANA_USE_UNCORRNDCOVMAT")) {
    UseV3NDCovMat = bool(atoi(getenv("CAFANA_USE_UNCORRNDCOVMAT")));
  }

  // JOINT COVARIANCE CASE HERE.
  // The analyses were only performed with both beams when using the covariance matrix.
  // So there was no option for joint or no joint matrix case. 

  if (UseNDCovMat && (pot_nd_rhc > 0) && (pot_nd_fhc > 0)) {
    if (turbose) {
      std::cout << "[INFO]: Opening ND covmat file for CovarianceExperiment" << BuildLogInfoString()
                << std::endl;
    }

    TMatrixD* joint_matrix = GetNDCovMat(UseV3NDCovMat, true, true);

    nd_expt_joint = new CovarianceExperiment({&predNDNumuFHC, &predNDNumuRHC},
                                             {*spectra->at(kNDNumuFHC).spect,
                                              *spectra->at(kNDNumuRHC).spect},
                                             joint_matrix,
                                             kCovMxChiSqPreInvert);
    
  //   //// Invert the assignation. Let's see if the old AddCovMatrix to MultiExperiment was messing thing up (?)
  //   nd_expt_joint = new CovarianceExperiment({&predNDNumuRHC, &predNDNumuFHC},
  //                                            {*spectra->at(kNDNumuRHC).spect,
  //                                             *spectra->at(kNDNumuFHC).spect},
  //                                            joint_matrix,
  //                                            kCovMxChiSqPreInvert);
    nd_expt_joint->SetMaskHist(0, 0.5, (AnaV == kV4) ? 10 : 8, 0, -1);
    nd_expt_joint->SetMaskHist(1, 0.5, (AnaV == kV4) ? 10 : 8, 0, -1);
  }
  else{
    auto e = new SingleSampleExperiment(&predNDNumuFHC,
                                        *spectra->at(kNDNumuFHC).spect);

    e->SetMaskHist(0.5, (AnaV == kV4) ? 10 : 8, 0, -1);
    nd_expt_fhc = e;

    e = new SingleSampleExperiment(&predNDNumuRHC,
                                   *spectra->at(kNDNumuRHC).spect);

    e->SetMaskHist(0.5, (AnaV == kV4) ? 10 : 8, 0, -1);
    nd_expt_rhc = e;
  }

  if (PostFitTreeBlob) {
    // Save the seeds used to do the stats throws
    (*PostFitTreeBlob->fSpectraRNGSeeds) = std::vector<unsigned>{
        spectra->at(kFDNueFHC).stats_seed,  spectra->at(kFDNumuFHC).stats_seed,
        spectra->at(kFDNueRHC).stats_seed,  spectra->at(kFDNumuRHC).stats_seed,
        spectra->at(kNDNumuFHC).stats_seed, spectra->at(kNDNumuRHC).stats_seed};
  }

  if (turbose) {
    std::cout << "[INFO]: Have experiments. " << BuildLogInfoString()
              << std::endl;
  }

  // Save prefit starting distributions
  if (outDir) {

    if (turbose) {
      std::cout << "[INFO]: Saving prefit info. " << BuildLogInfoString()
                << std::endl;
    }

    if (pot_fd_fhc_nue > 0) {
      TH1 *data_nue_fhc_hist =
          spectra->at(kFDNueFHC).spect->ToTHX(pot_fd_fhc_nue);
      data_nue_fhc_hist->SetName("data_fd_nue_fhc");
      data_nue_fhc_hist->Write();
      TH1 *pre_fd_nue_fhc = GetMCSystTotal(&predFDNueFHC, fitOsc, fitSyst,
                                           "prefit_fd_nue_fhc", pot_fd_fhc_nue);
      pre_fd_nue_fhc->SetTitle(
          std::to_string(app_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_fd_nue_fhc->Write();
    }
    if (pot_fd_fhc_numu > 0) {
      TH1 *data_numu_fhc_hist =
          spectra->at(kFDNumuFHC).spect->ToTHX(pot_fd_fhc_numu);
      data_numu_fhc_hist->SetName("data_fd_numu_fhc");
      data_numu_fhc_hist->Write();
      TH1 *pre_fd_numu_fhc =
          GetMCSystTotal(&predFDNumuFHC, fitOsc, fitSyst, "prefit_fd_numu_fhc",
                         pot_fd_fhc_numu);
      pre_fd_numu_fhc->SetTitle(
          std::to_string(dis_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_fd_numu_fhc->Write();
    }
    if (pot_fd_rhc_nue > 0) {
      TH1 *data_nue_rhc_hist =
          spectra->at(kFDNueRHC).spect->ToTHX(pot_fd_rhc_nue);
      data_nue_rhc_hist->SetName("data_fd_nue_rhc");
      data_nue_rhc_hist->Write();
      TH1 *pre_fd_nue_rhc = GetMCSystTotal(&predFDNueRHC, fitOsc, fitSyst,
                                           "prefit_fd_nue_rhc", pot_fd_rhc_nue);
      pre_fd_nue_rhc->SetTitle(
          std::to_string(app_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_fd_nue_rhc->Write();
    }
    if (pot_fd_rhc_numu > 0) {
      TH1 *data_numu_rhc_hist =
          spectra->at(kFDNumuRHC).spect->ToTHX(pot_fd_rhc_numu);
      data_numu_rhc_hist->SetName("data_fd_numu_rhc");
      data_numu_rhc_hist->Write();
      TH1 *pre_fd_numu_rhc =
          GetMCSystTotal(&predFDNumuRHC, fitOsc, fitSyst, "prefit_fd_numu_rhc",
                         pot_fd_rhc_numu);
      pre_fd_numu_rhc->SetTitle(
          std::to_string(dis_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      pre_fd_numu_rhc->Write();
    }
    if (pot_nd_fhc > 0) {
      TH1 *nd_data_numu_fhc_hist =
          spectra->at(kNDNumuFHC).spect->ToTHX(pot_nd_fhc);
      TH1 *nd_data_numu_fhc_hist_1D =
          spectra->at(kNDNumuFHC).spect->ToTH1(pot_nd_fhc);
      nd_data_numu_fhc_hist->SetName("data_nd_numu_fhc");
      nd_data_numu_fhc_hist_1D->SetName("data_nd_numu_fhc_1D");
      nd_data_numu_fhc_hist->Write();
      nd_data_numu_fhc_hist_1D->Write();

      TH1 *pre_nd_numu_fhc = GetMCSystTotal(&predNDNumuFHC, fitOsc, fitSyst,
                                            "prefit_nd_numu_fhc", pot_nd_fhc);
      pre_nd_numu_fhc->SetTitle(
          std::to_string(nd_expt_fhc->ChiSq(fitOsc, fitSyst)).c_str());
      pre_nd_numu_fhc->Write();
      TH1 *pre_nd_numu_fhc_1D =
          GetMCSystTotal(&predNDNumuFHC, fitOsc, fitSyst,
                         "prefit_nd_numu_fhc_1D", pot_nd_fhc, true);
      pre_nd_numu_fhc_1D->SetTitle(
          std::to_string(nd_expt_fhc->ChiSq(fitOsc, fitSyst)).c_str());
      pre_nd_numu_fhc_1D->Write();
    }
    if (pot_nd_rhc) {
      TH1 *nd_data_numu_rhc_hist =
          spectra->at(kNDNumuRHC).spect->ToTHX(pot_nd_rhc);
      TH1 *nd_data_numu_rhc_hist_1D =
          spectra->at(kNDNumuRHC).spect->ToTH1(pot_nd_rhc);
      nd_data_numu_rhc_hist->SetName("data_nd_numu_rhc");
      nd_data_numu_rhc_hist_1D->SetName("data_nd_numu_rhc_1D");
      nd_data_numu_rhc_hist->Write();
      nd_data_numu_rhc_hist_1D->Write();

      TH1 *pre_nd_numu_rhc = GetMCSystTotal(&predNDNumuRHC, fitOsc, fitSyst,
                                            "prefit_nd_numu_rhc", pot_nd_rhc);
      pre_nd_numu_rhc->SetTitle(
          std::to_string(nd_expt_rhc->ChiSq(fitOsc, fitSyst)).c_str());
      pre_nd_numu_rhc->Write();
      TH1 *pre_nd_numu_rhc_1D =
          GetMCSystTotal(&predNDNumuRHC, fitOsc, fitSyst,
                         "prefit_nd_numu_rhc_1D", pot_nd_rhc, true);
      pre_nd_numu_rhc_1D->SetTitle(
          std::to_string(nd_expt_rhc->ChiSq(fitOsc, fitSyst)).c_str());
      pre_nd_numu_rhc_1D->Write();
    }

    if (turbose) {
      std::cout << "[INFO]: Done writing prefit " << BuildLogInfoString()
                << std::endl;
    }
  }

  // Now sort out the experiment
  MultiExperiment this_expt;
  if(nd_expt_joint){
    this_expt.Add(nd_expt_joint);
  }
  else{
    if (pot_nd_fhc > 0)
      this_expt.Add(nd_expt_fhc);
    if (pot_nd_rhc > 0)
      this_expt.Add(nd_expt_rhc);
  }
  if (pot_fd_fhc_numu > 0)
    this_expt.Add(&dis_expt_fhc);
  if (pot_fd_rhc_numu > 0)
    this_expt.Add(&dis_expt_rhc);
  if (pot_fd_fhc_nue > 0)
    this_expt.Add(&app_expt_fhc);
  if (pot_fd_rhc_nue > 0)
    this_expt.Add(&app_expt_rhc);

  if (turbose) {
    std::cout << "[INFO]: Built multi-experiment " << BuildLogInfoString()
              << std::endl;
  }

  // Add in the penalty...
  if (penaltyTerm) {
    this_expt.Add(penaltyTerm);
  }

  auto start_fit = std::chrono::system_clock::now();
  // Now set up the fit itself
  std::cerr << "[INFO]: Beginning fit. " << BuildLogInfoString();
  MinuitFitter this_fit(&this_expt, oscVars, systlist, fitStrategy);
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

  bf = fitSyst;

  // If we have a directory to save to... save some stuff...
  if (outDir) {
    if (turbose) {
      std::cout << "[INFO]: Writing fit tree " << BuildLogInfoString();
    }
    std::vector<std::string> fParamNames = this_fit.GetParamNames();
    std::vector<double> fPreFitValues = this_fit.GetPreFitValues();
    std::vector<double> fPreFitErrors = this_fit.GetPreFitErrors();
    std::vector<double> fPostFitValues = this_fit.GetPostFitValues();
    std::vector<double> fPostFitErrors = this_fit.GetPostFitErrors();
    std::vector<double> fCentralValues = this_fit.GetCentralValues();
    double fNFCN = this_fit.GetNFCN();
    double fEDM = this_fit.GetEDM();
    bool fIsValid = this_fit.GetIsValid();

    TMatrixDSym *covar = (TMatrixDSym *)this_fit.GetCovariance();
    TH2D hist_covar = TH2D(*covar);
    hist_covar.SetName("covar");
    TH2D hist_corr = *make_corr_from_covar(&hist_covar);

    if (turbose) {
      std::cout << "[INFO]: Writing postfit spectra " << BuildLogInfoString()
                << std::endl;
    }

    if (pot_fd_fhc_nue) {
      TH1 *post_fd_nue_fhc = GetMCSystTotal(
          &predFDNueFHC, fitOsc, fitSyst, "postfit_fd_nue_fhc", pot_fd_fhc_nue);
      post_fd_nue_fhc->SetTitle(
          std::to_string(app_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_fd_nue_fhc->Write();
    }
    if (pot_fd_fhc_numu) {
      TH1 *post_fd_numu_fhc =
          GetMCSystTotal(&predFDNumuFHC, fitOsc, fitSyst, "postfit_fd_numu_fhc",
                         pot_fd_fhc_numu);
      post_fd_numu_fhc->SetTitle(
          std::to_string(dis_expt_fhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_fd_numu_fhc->Write();
    }
    if (pot_fd_rhc_nue) {
      TH1 *post_fd_nue_rhc = GetMCSystTotal(
          &predFDNueRHC, fitOsc, fitSyst, "postfit_fd_nue_rhc", pot_fd_rhc_nue);
      post_fd_nue_rhc->SetTitle(
          std::to_string(app_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_fd_nue_rhc->Write();
    }
    if (pot_fd_rhc_numu) {
      TH1 *post_fd_numu_rhc =
          GetMCSystTotal(&predFDNumuRHC, fitOsc, fitSyst, "postfit_fd_numu_rhc",
                         pot_fd_rhc_numu);
      post_fd_numu_rhc->SetTitle(
          std::to_string(dis_expt_rhc.ChiSq(fitOsc, fitSyst)).c_str());
      post_fd_numu_rhc->Write();
    }
    if (pot_nd_fhc) {
      TH1 *post_nd_numu_fhc = GetMCSystTotal(&predNDNumuFHC, fitOsc, fitSyst,
                                             "postfit_nd_numu_fhc", pot_nd_fhc);
      post_nd_numu_fhc->SetTitle(
          std::to_string(nd_expt_fhc->ChiSq(fitOsc, fitSyst)).c_str());
      post_nd_numu_fhc->Write();
      TH1 *post_nd_numu_fhc_1D =
          GetMCSystTotal(&predNDNumuFHC, fitOsc, fitSyst,
                         "postfit_nd_numu_fhc_1D", pot_nd_fhc, true);
      post_nd_numu_fhc_1D->SetTitle(
          std::to_string(nd_expt_fhc->ChiSq(fitOsc, fitSyst)).c_str());
      post_nd_numu_fhc_1D->Write();
    }
    if (pot_nd_rhc) {
      TH1 *post_nd_numu_rhc = GetMCSystTotal(&predNDNumuRHC, fitOsc, fitSyst,
                                             "postfit_nd_numu_rhc", pot_nd_rhc);
      post_nd_numu_rhc->SetTitle(
          std::to_string(nd_expt_rhc->ChiSq(fitOsc, fitSyst)).c_str());
      post_nd_numu_rhc->Write();
      TH1 *post_nd_numu_rhc_1D =
          GetMCSystTotal(&predNDNumuRHC, fitOsc, fitSyst,
                         "postfit_nd_numu_rhc_1D", pot_nd_rhc, true);
      post_nd_numu_rhc_1D->SetTitle(
          std::to_string(nd_expt_rhc->ChiSq(fitOsc, fitSyst)).c_str());
      post_nd_numu_rhc_1D->Write();
    }

    // Save information
    hist_covar.Write();
    hist_corr.Write();
    covar->Write("covar_mat");
  }

  if (PostFitTreeBlob) {
    if (turbose) {
      std::cout << "[INFO]: Assigning fit tree blob values "
                << BuildLogInfoString() << std::endl;
    }
    (*PostFitTreeBlob->fParamNames) = this_fit.GetParamNames();
    (*PostFitTreeBlob->fPreFitValues) = this_fit.GetPreFitValues();
    (*PostFitTreeBlob->fPreFitErrors) = this_fit.GetPreFitErrors();
    (*PostFitTreeBlob->fPostFitValues) = this_fit.GetPostFitValues();
    (*PostFitTreeBlob->fPostFitErrors) = this_fit.GetPostFitErrors();
    (*PostFitTreeBlob->fCentralValues) = this_fit.GetCentralValues();
    (*PostFitTreeBlob->fFakeDataVals) = fFakeDataVals;
    PostFitTreeBlob->fNFCN = this_fit.GetNFCN();
    PostFitTreeBlob->fEDM = this_fit.GetEDM();
    PostFitTreeBlob->fIsValid = this_fit.GetIsValid();
    PostFitTreeBlob->fChiSq = thischisq;
    PostFitTreeBlob->fNSeconds =
        std::chrono::duration_cast<std::chrono::seconds>(end_fit - start_fit)
            .count();
    PostFitTreeBlob->fNOscSeeds = oscSeeds.size();

    ProcInfo_t procinfo;
    gSystem->GetProcInfo(&procinfo);
    PostFitTreeBlob->fResMemUsage = procinfo.fMemResident;
    PostFitTreeBlob->fVirtMemUsage = procinfo.fMemVirtual;
  }

  if (turbose) {
    std::cout << "[INFO]: Finished run fit point." << BuildLogInfoString()
              << std::endl;
  }

  return thischisq;
}
