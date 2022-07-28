#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/Prediction/PredictionsForPRISM.h"

#include "CAFAna/PRISM/Axes.h"
#include "CAFAna/PRISM/Cuts.h"
#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PredictionPRISM.h"
#include "CAFAna/PRISM/Weights.h"

#include "CAFAna/Systs/RecoEnergyFDSysts.h"
#include "CAFAna/Systs/RecoEnergyNDSysts.h"
#include "CAFAna/Systs/TruthEnergyFDSysts.h"
#include "CAFAna/Systs/TruthEnergyNDSysts.h"
#include "CAFAna/Systs/XSecSysts.h"

#include "OscLib/IOscCalc.h"

using namespace ana;
using namespace PRISM;

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::string output_file_name;
std::string syst_descriptor = "nosyst";
std::string axdescriptor = "EVisReco";
std::string binningdescriptor = "prism_default";
std::string oabinningdescriptor = "default";
std::string truthbinningdescriptor = "event_rate_match"; // was event_rate_match

std::vector<std::vector<std::string>> input_CAF_descriptors;
std::vector<std::vector<std::string>> input_CAF_files;

bool addfakedata = true;
bool do_no_op = false;
unsigned nmax = 0;
bool UseSel = false;
bool isReco = false;
std::string FakeDataShiftDescript = "MissingProtonFakeData_pos";

void SayUsage(char const *argv[]) {
  std::cout
      << "[USAGE]: " << argv[0] << "\n"
      << "\t-A|--axes <A>             : Fit axis specifier. (default: A = "
         "\"EProxy\")\n"
      << "\t-o|--output <F>           : Output file name. Should not "
         "already "
         "exist.\n"
      << "\t-N-nu|--ND-input-numode <P> : Regex pattern to search for input\n"
      << "\t                            Files. Can only include pattern \n"
         "\t                            elements for files.\n"
      << "\t-N-280-nu|--ND-280kA-input-numode <P> : Regex pattern to search "
         "for input\n"
      << "\t                            Files. Can only include pattern \n"
         "\t                            elements for files.\n"
      << "\t-F-nu|--FD-input-numode <P> : Regex pattern to search for input\n"
      << "\t                            Files. Can only include pattern \n"
         "\t                            elements for files.\n"
      << "\t-Fe-nu|--FD-nue-input-numode <P> : Regex pattern to search for\n"
      << "\t                            input files. Can only include pattern\n"
         "\t                            elements for files.\n"
      << "\t-Ft-nu|--FD-nutau-input-numode <P> : Regex pattern to search for\n"
      << "\t                            input files. Can only include pattern\n"
         "\t                            elements for files.\n"
      << "\t-N-nub|--ND-input-nubmode <P> : Regex pattern to search for input\n"
      << "\t                            Files. Can only include pattern \n"
         "\t                            elements for files.\n"
      << "\t-N-280-nub|--ND-280kA-input-nubmode <P> : Regex pattern to search "
         "for input\n"
      << "\t                            Files. Can only include pattern \n"
         "\t                            elements for files.\n"
      << "\t-F-nub|--FD-input-nubmode <P> : Regex pattern to search for input\n"
      << "\t                            Files. Can only include pattern \n"
         "\t                            elements for files.\n"
      << "\t-Fe-nub|--FD-nue-input-nubmode <P> : Regex pattern to search for\n"
      << "\t                            input files. Can only include pattern\n"
         "\t                            elements for files.\n"
      << "\t-Ft-nub|--FD-nutau-input-nubmode <P> : Regex pattern to search "
         "for\n"
      << "\t                            input files. Can only include pattern\n"
         "\t                            elements for files.\n"
      << "\t-n|--n-max <N>            : Max number of events to read.\n"
      << "\t--syst-descriptor <str>   : Only add dials matching the syst\n"
         "\t                            descriptor <str> to the state "
         "file.\n"
      << "\t--bin-descriptor <str>  : Analysis bin descriptor.\n"
      << "\t--OA-bin-descriptor <str>    : Bin descriptor for off axis "
         "samples.\n"
      << "\t--truth-bin-descriptor <str> : Bin descriptor for matching "
         "distributions.\n"
      << "\t--no-fakedata-dials       : Do not add the fake data dials to "
         "the\n"
         "\t                            state file\n"
      << "\t--no-op                   : Do nothing but dump dials that "
         "would\n"
      << "\t--PRISM-fake-data <fake_data_dial> : A fake data dial to set on "
         "the PRISM ND data.\n"
         "\t                            be included.\n"
      << "\t--UseSelection            : Use the on-axis analysis selection. \n"
         "\t                            This will produce a bad PRISM "
         "prediction."
      << std::endl;
}

//std::cout << "line 110" << std::endl;

void handleOpts(int argc, char const *argv[]) {
  int opt = 1;
  while (opt < argc) {
    if (std::string(argv[opt]) == "-?" || std::string(argv[opt]) == "--help") {
      SayUsage(argv);
      exit(0);
    } else if ((std::string(argv[opt]) == "-A") ||
               (std::string(argv[opt]) == "--axes")) {
      axdescriptor = argv[++opt];
      isReco = isRecoND(axdescriptor);
    } else if ((std::string(argv[opt]) == "-o") ||
               (std::string(argv[opt]) == "--output")) {
      output_file_name = argv[++opt];
    } else if ((std::string(argv[opt]) == "-N-nu") ||
               (std::string(argv[opt]) == "--ND-input-numode")) {
      input_CAF_descriptors[kND_293kA_nu_numu].push_back(argv[++opt]); //eran name build
    } else if ((std::string(argv[opt]) == "-N-280-nu") ||
               (std::string(argv[opt]) == "--ND-280kA-input-numode")) {
      input_CAF_descriptors[kND_280kA_nu_numu].push_back(argv[++opt]); //eran
    } else if ((std::string(argv[opt]) == "-F-nu") ||
               (std::string(argv[opt]) == "--FD-input-numode")) {
      input_CAF_descriptors[kFD_nu_nonswap].push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-Fe-nu") ||
               (std::string(argv[opt]) == "--FD-nue-input-numode")) {
      input_CAF_descriptors[kFD_nu_nueswap].push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-Ft-nu") ||
               (std::string(argv[opt]) == "--FD-nutau-input-numode")) {
      input_CAF_descriptors[kFD_nu_tauswap].push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-N-nub") ||
               (std::string(argv[opt]) == "--ND-input-nubmode")) {
      input_CAF_descriptors[kND_293kA_nub_numub].push_back(argv[++opt]);  //eran
    } else if ((std::string(argv[opt]) == "-N-280-nub") ||
               (std::string(argv[opt]) == "--ND-280kA-input-nubmode")) {
      input_CAF_descriptors[kND_280kA_nub_numub].push_back(argv[++opt]);  //eran
    } else if ((std::string(argv[opt]) == "-F-nub") ||
               (std::string(argv[opt]) == "--FD-input-nubmode")) {
      input_CAF_descriptors[kFD_nub_nonswap].push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-Fe-nub") ||
               (std::string(argv[opt]) == "--FD-nue-input-nubmode")) {
      input_CAF_descriptors[kFD_nub_nueswap].push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-Ft-nub") ||
               (std::string(argv[opt]) == "--FD-nutau-input-nubmode")) {
      input_CAF_descriptors[kFD_nub_tauswap].push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-n") ||
               (std::string(argv[opt]) == "--n-max")) {
      nmax = atoi(argv[++opt]);
    } else if (std::string(argv[opt]) == "--syst-descriptor") {
      syst_descriptor = argv[++opt];
    } else if (std::string(argv[opt]) == "--bin-descriptor") {
      binningdescriptor = argv[++opt];
    } else if (std::string(argv[opt]) == "--OA-bin-descriptor") {
      oabinningdescriptor = argv[++opt];
    } else if (std::string(argv[opt]) == "--truth-bin-descriptor") {
      truthbinningdescriptor = argv[++opt];
    } else if (std::string(argv[opt]) == "--no-fakedata-dials") {
      addfakedata = false;
    } else if (std::string(argv[opt]) == "--PRISM-fake-data") {
      FakeDataShiftDescript = argv[++opt];
    } else if (std::string(argv[opt]) == "--no-op") {
      do_no_op = true;
    } else if (std::string(argv[opt]) == "--UseSelection") {
      UseSel = true;
    } else {
      std::cout << "[ERROR]: Unknown option: " << argv[opt] << std::endl;
      SayUsage(argv);
      exit(1);
    }
    opt++;
  }
}

int main(int argc, char const *argv[]) {

  TestConfigDefinitions();

  input_CAF_descriptors.resize(kNPRISMConfigs);

  handleOpts(argc, argv);
   std::cout << "selection value " << UseSel << std::endl;
  //	return 0;

  input_CAF_files.resize(kNPRISMConfigs);

  size_t NFiles = 0;
  for (size_t config = 0; config < kNPRISMConfigs; ++config) {
    for (auto InputFilePattern : input_CAF_descriptors[config]) {
      size_t asterisk_loc = InputFilePattern.find_first_of('*');
      size_t class_loc = InputFilePattern.find_first_of('[');
      size_t earliest_regex = std::min(asterisk_loc, class_loc);
      size_t last_slash_loc = InputFilePattern.find_last_of('/');

      if ((asterisk_loc != std::string::npos) &&
          (last_slash_loc != std::string::npos) &&
          (earliest_regex < last_slash_loc)) {

        std::cout << "[ERROR]: Can only wildcard filenames, not directories."
                  << std::endl;
        exit(2);
      }

      std::string dir = (last_slash_loc == std::string::npos)
                            ? std::string("")
                            : InputFilePattern.substr(0, last_slash_loc + 1);
      std::string pattern = (last_slash_loc == std::string::npos)
                                ? InputFilePattern
                                : InputFilePattern.substr(
                                      last_slash_loc + 1,
                                      InputFilePattern.size() - last_slash_loc);

      std::vector<std::string> CAFs;
      if (earliest_regex == std::string::npos) {
        CAFs.push_back(dir + pattern);
      } else {
        try {
          CAFs = GetMatchingFiles(dir, pattern);
        } catch (std::regex_error const &e) {
          std::cout << "[ERROR]: " << e.what() << ", "
                    << parse_stdRegex_ErrorCode(e.code()) << std::endl;
          exit(3);
        }
      }
      std::copy(CAFs.begin(), CAFs.end(),
                std::back_inserter(input_CAF_files[config]));
      NFiles += CAFs.size();
    }
  }
  // Have lists of files.

std::cout << "line 240" << std::endl;

  for (size_t config = 0; config < kNPRISMConfigs; ++config) {
    if (!input_CAF_files[config].size()) {
      std::cout << "[WARN]: Failed to find any " << DescribeConfig(config)
                << " files." << std::endl;
    }
    for (auto &f : input_CAF_files[config]) {
      std::cout << "[INFO]: Reading from: " << f << std::endl;

      // Make files pnfs friendly.
      std::string xrootd_f = pnfs2xrootd(f);
      if (xrootd_f != f) {
        std::cout << "[INFO]: XRootD: " << f << std::endl;
        f = xrootd_f;
      }
    }
  }

  if (!NFiles) {
    std::cout << "[ERROR]: Found no files." << std::endl;
    abort();
  }

  // Sort out systematics if they've been requested
  // Separate out flux systs as only flux systs are
  // needed for the PredInterps that are used to
  // determine the PRISM coefficient
  std::vector<ana::ISyst const *> los, los_flux;
  if (syst_descriptor.size()) {
    los = GetListOfSysts(syst_descriptor);

    // Have to add fake data systs too.
    std::vector<ana::ISyst const *> fdlos = GetListOfSysts(
        false, false, false, false, false, false, addfakedata, false);
    los.insert(los.end(), fdlos.begin(), fdlos.end());

    los_flux = los;
    KeepSysts(los_flux, GetListOfSysts("nov17flux:nodet:noxsec"));
  } else {
    // Default but allow fake data dials to be turned off
    los = GetListOfSysts(true, true, true, true, true, false, addfakedata);
  }

  std::cout << "[INFO]: Using " << los.size()
            << " systematic dials: " << std::endl;
  for (auto s : los) {
    std::cout << "[INFO]:\t" << s->ShortName() << std::endl;
  }

  // If we are just testing the command line arguments... don't do anything
  if (do_no_op) {
    return 0;
  }

  TFile fout(output_file_name.c_str(), "RECREATE");

std::cout << "line 297" << std::endl;

  //--------------------------------------
  // Sort out the axes:
  PRISMAxisBlob axes =
      GetPRISMAxes(axdescriptor, binningdescriptor, oabinningdescriptor);

  HistAxis MatchAxis = GetEventRateMatchAxes(truthbinningdescriptor);

  bool OneDAxis(false);
  if (axdescriptor == "EVisReco" || axdescriptor == "EProxy")
    OneDAxis = true;
  HistAxis TrueObsAxis = TrueObservable(
      axdescriptor, OneDAxis ? "prism_fine_default" : binningdescriptor);

  std::vector<HistAxis> AxisVec = {axes.XProjectionFD, axes.XProjectionFD};
  HistAxis CovarianceAxis = GetMatrixAxis(AxisVec);

  HistAxis _OffPredictionAxis =
      GetTwoDAxis(axes.XProjectionND, axes.OffAxisPosition);
  HistAxis _280kAPredictionAxis =
      GetTwoDAxis(axes.XProjectionND, axes.OffAxis280kAPosition);
  HistAxis _FluxMatcherCorrectionAxes =
      GetTwoDAxis(axes.XProjectionFD, MatchAxis);
  //--------------------------------------

  ana::Weight kNDCVWeight = GetNDCVWeight();
  ana::Weight kFDCVWeight = GetFDCVWeight();

  ana::Cut kNDSelectionCuts_numu =
      UseSel ? kPRISMNDSignal_Selected_numu : kPRISMNDSignal_True_numu;
  ana::Cut kNDSelectionCuts_numub =
      UseSel ? kPRISMNDSignal_Selected_numub : kPRISMNDSignal_True_numub;

  ana::Cut kNDSelectionCuts_nue =
      UseSel ? kPRISMNDSignal_Selected_nue : kPRISMNDSignal_True_nue;
  ana::Cut kNDSelectionCuts_nueb =
      UseSel ? kPRISMNDSignal_Selected_nueb : kPRISMNDSignal_True_nueb;

  ana::Cut kFDSelectionCuts_numu =
      UseSel ? kPRISMFDSignal_Selected_numu : kPRISMFDSignal_True_numu;
  ana::Cut kFDSelectionCuts_numub =
      UseSel ? kPRISMFDSignal_Selected_numub : kPRISMFDSignal_True_numub;

  ana::Cut kFDSelectionCuts_nue =
      UseSel ? kPRISMFDSignal_Selected_nue : kPRISMFDSignal_True_nue;
  ana::Cut kFDSelectionCuts_nueb =
      UseSel ? kPRISMFDSignal_Selected_nueb : kPRISMFDSignal_True_nueb;
  ana::SystShifts DataShift =
      GetFakeDataGeneratorSystShift(FakeDataShiftDescript);


std::cout << "line 349, after kSelectionCuts" << std::endl;
  //-------------------------------------------------------

  auto PRISM = std::make_unique<PredictionPRISM>(
      axes.XProjectionND, axes.XProjectionFD, axes.OffAxisPosition,
      axes.OffAxis280kAPosition, TrueObsAxis, MatchAxis, CovarianceAxis,
      _OffPredictionAxis, _280kAPredictionAxis, _FluxMatcherCorrectionAxes);
  PRISM->Initialize();

  Loaders Loaders_nu, Loaders_nub;

  std::vector<std::shared_ptr<SpectrumLoader>> FileLoaders;

  FillWithNulls(FileLoaders, kNPRISMConfigs);
  for (size_t config = 0; config < kNPRISMConfigs; ++config) {
    bool IsNu = IsNuConfig(config);
    bool IsND = IsNDConfig(config);
    bool IsNDNue = IsNDNueConfig(config);//eran
    bool IsND280kA = IsND280kA_numu_Config(config); //eran
    size_t IsNueSwap = IsFDNueConfig(config); //eran
    size_t IsNonSwap = IsNumuConfig(config);
    size_t IsNuTauSwap = IsNutauConfig(config);

    std::cout << "IsNu = " << IsNu << ", IsND = " << IsND
              << ", IsND280kA = " << IsND280kA << std::endl;
    if (!input_CAF_files[config].size()) {
      std::cout << "[WARNING] No files loaded." << std::endl;
      continue;
    }
    std::cout << "We have " << input_CAF_files[config].size() << " file loaded."
              << std::endl;

    Loaders &Loaders_bm = IsNu ? Loaders_nu : Loaders_nub;
    if (IsND) {

      if (!IsND280kA && !IsNDNue) {
        // For ND we have to combine the 293/280 loaders because CAFAna cannot
        // keep them separate
        std::vector<std::string> ND_input_files;

        std::copy(input_CAF_files[config].begin(),
                  input_CAF_files[config].end(),
                  std::back_inserter(ND_input_files));

        std::copy(input_CAF_files[GetND280kAConfig(config)].begin(),
                  input_CAF_files[GetND280kAConfig(config)].end(),
                  std::back_inserter(ND_input_files));

        FileLoaders[config] =
            std::make_shared<SpectrumLoader>(ND_input_files, nmax);

        // Fix this up so that the 280kA config has a loader
        FileLoaders[GetND280kAConfig(config)] = FileLoaders[config];
        // Fix this up so that the 280kA config has a loader
        //FileLoaders[GetNDNueConfig(config)] = FileLoaders[config]; //eran here!!
      }

      if (IsNDNue) {
        PRISM->AddNDDataLoader(
            *FileLoaders[GetND293kAConfig(config)],
            IsNu ? kNDSelectionCuts_nue : kNDSelectionCuts_nueb, kNDCVWeight,
            DataShift, IsNu ? kNue_I_Numode : kNueBar_I_NuBarmode);

      } else if (!IsND280kA) { // 293kA covers 280kA samples

        PRISM->AddNDDataLoader(
            *FileLoaders[GetND293kAConfig(config)],
            IsNu ? kNDSelectionCuts_numu : kNDSelectionCuts_numub, kNDCVWeight,
            DataShift, IsNu ? kNumu_Numode : kNumuBar_NuBarmode);
        Loaders_bm.AddLoader(FileLoaders[config].get(), caf::kNEARDET,
                             Loaders::kMC);
      }
std::cout << "line 421 after the loader stuff" << std::endl;

    } else if (!IsND) { // Is FD and files either nonswap, nueswap or tauswap.
      FileLoaders[config] =
          std::make_shared<SpectrumLoader>(input_CAF_files[config], nmax);

      if (IsNonSwap) {
        Loaders_bm.AddLoader(FileLoaders[config].get(), caf::kFARDET,
                             Loaders::kMC, Loaders::kNonSwap);
      } else if (IsNueSwap) {
        Loaders_bm.AddLoader(FileLoaders[config].get(), caf::kFARDET,
                             Loaders::kMC, Loaders::kNueSwap);
      } else {
        Loaders_bm.AddLoader(FileLoaders[config].get(), caf::kFARDET,
                             Loaders::kMC, Loaders::kNuTauSwap);
      }
    }
  } // end for loop

std::cout << "line 440 after all loaders, inculding FD loaders" << std::endl;

  // Make the ND prediction interp include the same off-axis axis used for
  // PRISM weighting.
  // Match axis is in true neutrino energy
  // Add off-axis axies for 293kA and 280kA run
  // 2D hists of energy and off-axis position
  std::vector<std::string> Labels_match = MatchAxis.GetLabels();
  std::vector<Binning> Bins_match = MatchAxis.GetBinnings();
  std::vector<Var> Vars_match = MatchAxis.GetVars();

  Labels_match.push_back(axes.OffAxisPosition.GetLabels().front());
  Bins_match.push_back(axes.OffAxisPosition.GetBinnings().front());
  Vars_match.push_back(axes.OffAxisPosition.GetVars().front());

  HistAxis const NDEventRateSpectraAxis(Labels_match, Bins_match, Vars_match);

  std::vector<std::string> Labels_match_280kA = MatchAxis.GetLabels();
  std::vector<Binning> Bins_match_280kA = MatchAxis.GetBinnings();
  std::vector<Var> Vars_match_280kA = MatchAxis.GetVars();

  Labels_match_280kA.push_back(axes.OffAxis280kAPosition.GetLabels().front());
  Bins_match_280kA.push_back(axes.OffAxis280kAPosition.GetBinnings().front());
  Vars_match_280kA.push_back(axes.OffAxis280kAPosition.GetVars().front());

  HistAxis const NDEventRateSpectraAxis_280kA(
      Labels_match_280kA, Bins_match_280kA, Vars_match_280kA);

  // HistAxis for Erec vs ETrue smearing matrix predictions
  // --> Need a axis which is the true version of the observable:
  // --> e.g. EProxy --> ETrue

  std::vector<HistAxis> NDAxisVec = {TrueObsAxis, axes.XProjectionND};
  std::vector<HistAxis> FDAxisVec = {TrueObsAxis, axes.XProjectionFD};

  // Get axes for ND and FD smearring matrices.
  // Account for 2D predictions by projecting a 4D smearing matrix
  // on to a 2D histogram.
  HistAxis const ERecETrueAxisND = GetMatrixAxis(NDAxisVec);
  HistAxis const ERecETrueAxisFD = GetMatrixAxis(FDAxisVec);

  //----------------------------------------------------------------
  // HistAxis needed for MC efficiency correction
  std::vector<std::string> Labels_eff_293kA = TrueObsAxis.GetLabels();
  std::vector<Binning> Bins_eff_293kA = TrueObsAxis.GetBinnings();
  std::vector<Var> Vars_eff_293kA = TrueObsAxis.GetVars();

  Labels_eff_293kA.push_back(axes.OffAxisPosition.GetLabels().front());
  Bins_eff_293kA.push_back(axes.OffAxisPosition.GetBinnings().front());
  Vars_eff_293kA.push_back(axes.OffAxisPosition.GetVars().front());

  HistAxis const NDTrueEnergyObsBins_293kA(Labels_eff_293kA, Bins_eff_293kA,
                                           Vars_eff_293kA);

  std::vector<std::string> Labels_eff_280kA = TrueObsAxis.GetLabels();
  std::vector<Binning> Bins_eff_280kA = TrueObsAxis.GetBinnings();
  std::vector<Var> Vars_eff_280kA = TrueObsAxis.GetVars();

  Labels_eff_280kA.push_back(axes.OffAxis280kAPosition.GetLabels().front());
  Bins_eff_280kA.push_back(axes.OffAxis280kAPosition.GetBinnings().front());
  Vars_eff_280kA.push_back(axes.OffAxis280kAPosition.GetVars().front());

  HistAxis const NDTrueEnergyObsBins_280kA(Labels_eff_280kA, Bins_eff_280kA,
                                           Vars_eff_280kA);

  HistAxis const FDTrueEnergyObsBins(TrueObsAxis.GetLabels(),
                                     TrueObsAxis.GetBinnings(),
                                     TrueObsAxis.GetVars());
  //----------------------------------------------------------------
std::cout << "line 509 after all hist creation stuff" << std::endl;


  std::vector<std::unique_ptr<IPredictionGenerator>> MatchPredGens;
  std::vector<std::unique_ptr<PredictionInterp>> MatchPredInterps;
  FillWithNulls(MatchPredGens, kNPRISMConfigs);
  FillWithNulls(MatchPredInterps, kNPRISMConfigs);

  // For Smearing Matrix
  std::vector<std::unique_ptr<IPredictionGenerator>> NDMatrixPredGens;
  std::vector<std::unique_ptr<PredictionInterp>> NDMatrixPredInterps;
  std::vector<std::unique_ptr<IPredictionGenerator>> FDMatrixPredGens;
  std::vector<std::unique_ptr<PredictionInterp>> FDMatrixPredInterps;
  FillWithNulls(NDMatrixPredGens, kNPRISMConfigs);
  FillWithNulls(NDMatrixPredInterps, kNPRISMConfigs);
  FillWithNulls(FDMatrixPredGens, kNPRISMFDConfigs);
  FillWithNulls(FDMatrixPredInterps, kNPRISMFDConfigs);

  // True ND and FD spectra for MC efficiency correction
  std::vector<std::unique_ptr<IPredictionGenerator>> NDUnselTruePredGens;
  std::vector<std::unique_ptr<PredictionInterp>> NDUnselTruePredInterps;
  std::vector<std::unique_ptr<IPredictionGenerator>> NDSelTruePredGens;
  std::vector<std::unique_ptr<PredictionInterp>> NDSelTruePredInterps;
  std::vector<std::unique_ptr<IPredictionGenerator>> FDUnselTruePredGens;
  std::vector<std::unique_ptr<PredictionInterp>> FDUnselTruePredInterps;
  std::vector<std::unique_ptr<IPredictionGenerator>> FDSelTruePredGens;
  std::vector<std::unique_ptr<PredictionInterp>> FDSelTruePredInterps;
  FillWithNulls(NDUnselTruePredGens, kNPRISMConfigs);
  FillWithNulls(NDUnselTruePredInterps, kNPRISMConfigs);
  FillWithNulls(NDSelTruePredGens, kNPRISMConfigs);
  FillWithNulls(NDSelTruePredInterps, kNPRISMConfigs);
  FillWithNulls(FDUnselTruePredGens, kNPRISMFDConfigs);
  FillWithNulls(FDUnselTruePredInterps, kNPRISMFDConfigs);
  FillWithNulls(FDSelTruePredGens, kNPRISMFDConfigs);
  FillWithNulls(FDSelTruePredInterps, kNPRISMFDConfigs);
  //---------------------------------------------------
  std::vector<std::unique_ptr<DataPredictionNoExtrap>> FarDetDataPreds;
  FillWithNulls(FarDetDataPreds, kNPRISMFDConfigs);
  std::vector<std::unique_ptr<DataPredictionNoExtrap>> FarDetFakeDataBiasPreds;
  FillWithNulls(FarDetFakeDataBiasPreds, kNPRISMFDConfigs);

std::cout << "line 550 after the FillWithNulls" << std::endl;

  static osc::NoOscillations no;
  static osc::IOscCalc *calc = NuFitOscCalc(1);

  for (size_t config = 0; config < kNPRISMConfigs; ++config) {
    bool IsNu = IsNuConfig(config);
    bool IsND = IsNDConfig(config);
    bool IsNDNue = IsNDNueConfig(config);
    bool IsND280kA = IsND280kA_numu_Config(config); //eran
    size_t fd_config = 0;
    size_t IsNueSwap = IsFDNueConfig(config); //eran
    size_t IsNuTauSwap = IsNutauConfig(config);
    std::cout << "config :" << config << std::endl;
    std::cout << "IsNu :" << IsNu << std::endl;
    std::cout << "IsNDNue :" << IsNDNue << std::endl;
    std::cout << "IsND280kA :" << IsND280kA << std::endl;
    std::cout << "IsNueSwap :" << IsNueSwap << std::endl;
    std::cout << "IsNuTauSwap :" << IsNuTauSwap << std::endl;
    std::cout << "END END" << std::endl;
    if (!IsND) {
      fd_config = GetFDConfig(config);
    }
    Loaders &Loaders_bm = IsNu ? Loaders_nu : Loaders_nub;

    if (!FileLoaders[config]) {
      continue;
    }

    if (IsND) { // Is ND

     ana::Cut * NDCuts;//eran
     // std::unique_ptr<ana::Cut> NDCuts;
     //std::unique_ptr<ana::Cut> NDCuts = std::make_unique<ana::Cut>(
      //std::unique_ptr<ana::Cut> NDCuts (new ana::Cut); 

      BeamChan chanmode;

      if (IsNDNue) {
        //NDCuts = IsNu ? kNDSelectionCuts_nue : kNDSelectionCuts_nueb;
        NDCuts = IsNu ? &kNDSelectionCuts_nue : &kNDSelectionCuts_nueb;
        chanmode = IsNu ? kNue_I_Numode : kNueBar_I_NuBarmode;
      } else {
        //NDCuts = IsNu ? kNDSelectionCuts_numu : kNDSelectionCuts_numub;
        NDCuts = IsNu ? &kNDSelectionCuts_numu : &kNDSelectionCuts_numub;
        chanmode = IsNu ? kNumu_Numode : kNumuBar_NuBarmode;
      }
//)
std::cout << "line 591 after the ND beamchan stuff added" << std::endl;

     // eran think error  if (IsND)  // Is ND
        // Only need to do this once as the PRISM prediction handles the 293,
        // 280 kA runs separately
        if (!IsND280kA) {
          // PRISM->AddNDMCLoader(Loaders_bm, NDCuts, kNDCVWeight, los, &no,
          //                     chanmode);
          PRISM->AddNDMCLoader(Loaders_bm, *NDCuts, kNDCVWeight, los, &no,
                               chanmode);
        }

//)
std::cout << "line 604 after the NDMCLoader added/changed" << std::endl;
std::cout << "IsNDNue?" << IsNDNue << std::endl;

        // Corrects for non-uniform off-axis binning
        auto slice_width_weight = NDSliceCorrection(
            50, (IsND280kA ? axes.OffAxis280kAPosition : axes.OffAxisPosition)
                    .GetBinnings()
                    .front()
                    .Edges());

        //NoOscPredictionGenerator(HistAxis axis, Cut cut,
          //                       Weight wei = kUnweighted)

            MatchPredGens[config] = std::make_unique<NoOscPredictionGenerator>(
                (IsND280kA ? NDEventRateSpectraAxis_280kA
                           : NDEventRateSpectraAxis),
                (IsNDNue ? kActuallyIsNueCC : kIsNumuCC) &&
                    (IsNu ? !kIsAntiNu : kIsAntiNu) && kIsTrueFV &&
                    kIsOutOfTheDesert &&
                    (IsND280kA ? kSel280kARun : kCut280kARun),
                kNDCVWeight * slice_width_weight);

        MatchPredInterps[config] = std::make_unique<PredictionInterp>(
            los_flux, &no, *MatchPredGens[config], Loaders_bm, kNoShift,
            PredictionInterp::kSplitBySign);

//std::cout << "MatchPredGens(config) :" << std::make_unique<NoOscPredictionGenerator>((IsND280kA ? NDEventRateSpectraAxis_280kA : NDEventRateSpectraAxis), (IsNDNue ? kActuallyIsNueCC : kIsNumuCC) && (IsNu ? !kIsAntiNu : kIsAntiNu) && kIsTrueFV && kIsOutOfTheDesert && (IsND280kA ? kSel280kARun : kCut280kARun), kNDCVWeight * slice_width_weight) << std::endl;




        // PredInterps for ND smearing matrix
        // Only need to do this for 293 kA
        // Relationship between ERec and ETrue should be the same
        // for 280kA and 293kA, right?
        if (!IsNDNue) {
          if (!IsND280kA) {
            if (isReco && UseSel) {
              NDMatrixPredGens[config] =
                  std::make_unique<NoOscPredictionGenerator>(
                      //ERecETrueAxisND, NDCuts && kCut280kARun, kNDCVWeight);
		      ERecETrueAxisND, *NDCuts && kCut280kARun, kNDCVWeight);
            } else { // Not using reco variable so don't need reco cut.
              NDMatrixPredGens[config] =
                  std::make_unique<NoOscPredictionGenerator>(
                      ERecETrueAxisND,
                      kIsNumuCC && (IsNu ? !kIsAntiNu : kIsAntiNu) &&
                          kIsTrueFV && kIsOutOfTheDesert && kCut280kARun,
                      kNDCVWeight);
            }
            NDMatrixPredInterps[config] = std::make_unique<PredictionInterp>(
                los, &no, *NDMatrixPredGens[config], Loaders_bm, kNoShift,
                PredictionInterp::kSplitBySign);
          }

std::cout << "line 653" << std::endl;
          // Add another ND unselected spectrum for MC eff correction
          // Use the same axis as the ND DATA
          // don't need it for 280kA, just getting the efficiency
          NDUnselTruePredGens[config] =
              std::make_unique<NoOscPredictionGenerator>(
                  (IsND280kA ? NDTrueEnergyObsBins_280kA
                             : NDTrueEnergyObsBins_293kA),
                  kIsNumuCC && (IsNu ? !kIsAntiNu : kIsAntiNu) && kIsTrueFV &&
                      kIsOutOfTheDesert &&
                      (IsND280kA ? kSel280kARun : kCut280kARun),
                  kNDCVWeight * slice_width_weight);
          NDUnselTruePredInterps[config] = std::make_unique<PredictionInterp>(
              los, &no, *NDUnselTruePredGens[config], Loaders_bm, kNoShift,
              PredictionInterp::kSplitBySign);

          // ND True Selected Spectrum
          NDSelTruePredGens[config] =
              std::make_unique<NoOscPredictionGenerator>(
                  (IsND280kA ? NDTrueEnergyObsBins_280kA
                             : NDTrueEnergyObsBins_293kA),
                  //NDCuts && (IsND280kA ? kSel280kARun : kCut280kARun),
                  *NDCuts && (IsND280kA ? kSel280kARun : kCut280kARun),
                  kNDCVWeight * slice_width_weight);
          NDSelTruePredInterps[config] = std::make_unique<PredictionInterp>(
              los, &no, *NDSelTruePredGens[config], Loaders_bm, kNoShift,
              PredictionInterp::kSplitBySign);
  
//std::cout << "MatchPredGens(config) :" << std::make_unique<NoOscPredictionGenerator>((IsND280kA ? NDEventRateSpectraAxis_280kA : NDEventRateSpectraAxis), (IsNDNue ? kActuallyIsNueCC : kIsNumuCC) && (IsNu ? !kIsAntiNu : kIsAntiNu) && kIsTrueFV && kIsOutOfTheDesert && (IsND280kA ? kSel280kARun : kCut280kARun), kNDCVWeight * slice_width_weight) << std::endl;





        } //added eran )

      } else if (!IsND && !IsNuTauSwap) { // Is FD and do not need specific
                                          // nutau spectra.

        ana::Cut &FDCuts =
            IsNu ? (IsNueSwap ? kFDSelectionCuts_nue : kFDSelectionCuts_numu)
                 : (IsNueSwap ? kFDSelectionCuts_nueb : kFDSelectionCuts_numub);

        BeamChan chanmode{IsNu ? BeamMode::kNuMode : BeamMode::kNuBarMode,
                          IsNueSwap ? NuChan::kNueNueBar
                                    : NuChan::kNumuNumuBar}; //} //eran adding here, issue from merge? 

        PRISM->AddFDMCLoader(Loaders_bm, FDCuts, kFDCVWeight, los, calc,
                             chanmode);

        // We always want to use the numus as we don't want to account for any
        // xsec differences between numu and nues, we use a special prediction
        // object to allow us to oscillate the NuMu spectrum.
        // Intrinsic nue is also added as the target flux
        if (!IsNueSwap) {
          MatchPredGens[config] =
              std::make_unique<NonSwapNoExtrapPredictionGenerator>(
                  MatchAxis,
                  (kIsNumuCC || kIsBeamNue) &&
                      (IsNu ? !kIsAntiNu : kIsAntiNu) && kIsTrueFV,
                  kFDCVWeight);

          MatchPredInterps[config] = std::make_unique<PredictionInterp>(
              los_flux, calc, *MatchPredGens[config], Loaders_bm, kNoShift,
              PredictionInterp::kSplitBySign);
     
//std::cout << "MatchPredGens(config) :" << std::make_unique<NoOscPredictionGenerator>((IsND280kA ? NDEventRateSpectraAxis_280kA : NDEventRateSpectraAxis), (IsNDNue ? kActuallyIsNueCC : kIsNumuCC) && (IsNu ? !kIsAntiNu : kIsAntiNu) && kIsTrueFV && kIsOutOfTheDesert && (IsND280kA ? kSel280kARun : kCut280kARun), kNDCVWeight * slice_width_weight) << std::endl;




   }

std::cout << "line 713" << std::endl;

        size_t non_swap_it = GetConfigNonSwap(config);
        size_t nue_swap_it = GetConfigNueSwap(config);

        FarDetDataPreds[fd_config] = std::make_unique<DataPredictionNoExtrap>(
            Loaders_bm, axes.XProjectionFD, FDCuts, kNoShift, kFDCVWeight);

        FarDetFakeDataBiasPreds[fd_config] =
            std::make_unique<DataPredictionNoExtrap>(
                Loaders_bm, axes.XProjectionFD, FDCuts, DataShift, kFDCVWeight);

        // Ugly temporary hack, hopefully we don't need this
        int from = (IsNu ? 14 : -14);
        int to = (IsNu ? (IsNueSwap ? 12 : 14) : (IsNueSwap ? -12 : -14));
        const ana::Weight kOscWeight(
            [from, to](const caf::StandardRecord *sr) -> double {
              osc::IOscCalc *osc = NuFitOscCalc(1);
              const auto Ps = osc->P(from, to, sr->Ev);
              return Ps;
            });
        // Matrix of ERec v ETrue for FD
        FDMatrixPredGens[fd_config] =
            std::make_unique<FDNoOscPredictionGenerator>(
                ERecETrueAxisFD, FDCuts, kFDCVWeight * kOscWeight);
        FDMatrixPredInterps[fd_config] = std::make_unique<PredictionInterp>(
            los, calc, *FDMatrixPredGens[fd_config], Loaders_bm, kNoShift,
            PredictionInterp::kSplitBySign);

        // True energy FD spectrum with obs binning for MC efficiency correction
        FDUnselTruePredGens[fd_config] =
            std::make_unique<NoExtrapPredictionGenerator>(
                FDTrueEnergyObsBins,
                (IsNueSwap ? kIsSig : kIsNumuCC) &&
                    (IsNu ? !kIsAntiNu : kIsAntiNu) && kIsTrueFV,
                kFDCVWeight);
        FDUnselTruePredInterps[fd_config] = std::make_unique<PredictionInterp>(
            los, calc, *FDUnselTruePredGens[fd_config], Loaders_bm, kNoShift,
            PredictionInterp::kSplitBySign);
        // FD Selected True Spectrum
        FDSelTruePredGens[fd_config] =
            std::make_unique<NoExtrapPredictionGenerator>(FDTrueEnergyObsBins,
                                                          FDCuts, kFDCVWeight);
        FDSelTruePredInterps[fd_config] = std::make_unique<PredictionInterp>(
            los, calc, *FDSelTruePredGens[fd_config], Loaders_bm, kNoShift,
            PredictionInterp::kSplitBySign);
      
//std::cout << "LOOP PRINT OUTS BEGIN" << std::endl;
//std::cout << "MatchPredGens(config) :" << std::make_unique<NoOscPredictionGenerator>((IsND280kA ? NDEventRateSpectraAxis_280kA : NDEventRateSpectraAxis), (IsNDNue ? kActuallyIsNueCC : kIsNumuCC) && (IsNu ? !kIsAntiNu : kIsAntiNu) && kIsTrueFV && kIsOutOfTheDesert && (IsND280kA ? kSel280kARun : kCut280kARun), kNDCVWeight * slice_width_weight) << std::endl;
//std::cout << "MatchPredInterps :" << MatchPredInterps << std::endl;
//std::cout << "NDMatrixPredGens :" << NDMatrixPredGens << std::endl;
//std::cout << "NDMatrixPredInterps :" << NDMatrixPredInterps << std::endl;
//std::cout << "NDUnselTruePredGens :" << NDUnselTruePredGens << std::endl;
//std::cout << "NDUnselTruePredInterps :" << NDUnselTruePredInterps << std::endl;
//std::cout << "END LOOP PRINT OUT" << std::endl;
        }
    }

    std::vector<std::unique_ptr<IPredictionGenerator>> FluxPredGens;
    std::vector<std::unique_ptr<PredictionInterp>> FluxPredInterps;

    std::vector<std::string> Labels_flux_293kA = MatchAxis.GetLabels();
    std::vector<Binning> Bins_flux_293kA = MatchAxis.GetBinnings();
    std::vector<Var> Vars_flux_293kA = MatchAxis.GetVars();

    Labels_flux_293kA.push_back(axes.OffAxisPosition.GetLabels().front());
    Bins_flux_293kA.push_back(axes.OffAxisPosition.GetBinnings().front());
    Vars_flux_293kA.push_back(axes.OffAxisPosition.GetVars().front());

    HistAxis const OffAxisFluxPredictionAxes_293kA(
        Labels_flux_293kA, Bins_flux_293kA, Vars_flux_293kA);

    std::vector<std::string> Labels_flux_280kA = MatchAxis.GetLabels();
    std::vector<Binning> Bins_flux_280kA = MatchAxis.GetBinnings();
    std::vector<Var> Vars_flux_280kA = MatchAxis.GetVars();

    Labels_flux_280kA.push_back(axes.OffAxis280kAPosition.GetLabels().front());
    Bins_flux_280kA.push_back(axes.OffAxis280kAPosition.GetBinnings().front());
    Vars_flux_280kA.push_back(axes.OffAxis280kAPosition.GetVars().front());

    HistAxis const OffAxisFluxPredictionAxes_280kA(
        Labels_flux_280kA, Bins_flux_280kA, Vars_flux_280kA);

    Loaders_nu.Go();
    Loaders_nub.Go();


//for (size_t config = 0; config < kNPRISMConfigs; ++config) {

//}

//std::cout << "RIGHT BEFORE SAVING LOOP PRINT OUTS BEGIN" << std::endl;
//std::cout << "MatchPredGens :" << MatchPredGens[config] << std::endl;
//std::cout << "MatchPredInterps :" << MatchPredInterps << std::endl;
//std::cout << "NDMatrixPredGens :" << NDMatrixPredGens << std::endl;
//std::cout << "NDMatrixPredInterps :" << NDMatrixPredInterps << std::endl;
//std::cout << "NDUnselTruePredGens :" << NDUnselTruePredGens << std::endl;
//std::cout << "NDUnselTruePredInterps :" << NDUnselTruePredInterps << std::endl;
//std::cout << "END RIGHT BEFORE SAVING LOOP PRINT OUT" << std::endl;

std::cout << "line 790 before loop where you actually fill them" << std::endl;




    for (size_t config = 0; config < kNPRISMConfigs; ++config) {
      bool IsNu = IsNuConfig(config);
      bool IsNDNue = IsNDNueConfig(config);
      bool IsND = IsNDConfig(config);
      bool IsND280kA = IsND280kA_numu_Config(config);
      size_t fd_config = 0;
      size_t IsFDNue = IsFDNueConfig(config);
      size_t IsNuTau = IsNutauConfig(config);
     std::cout << "BEGIN BEGIN " << std::endl;
     std::cout << "config :" << config << std::endl;
     std::cout << "IsNu :" << IsNu << std::endl;
    std::cout << "IsNDNue :" << IsNDNue << std::endl;
    std::cout << "IsND :" << IsND << std::endl;
    std::cout << "IsFDNue :" << IsFDNue << std::endl;
    std::cout << "IsNuTau :" << IsNuTau << std::endl;
    std::cout << "END END" << std::endl;

      if (!IsND) {
        fd_config = GetFDConfig(config);
      }
      if (!FileLoaders[config]) {
        continue;
      }


//std::cout << "INSIDE SAVING LOOP PRINT OUTS BEGIN" << std::endl;
//std::cout << "MatchPredGens :" << MatchPredGens[config] << std::endl;
//std::cout << "MatchPredInterps :" << MatchPredInterps << std::endl; 
//std::cout << "NDMatrixPredGens :" << NDMatrixPredGens << std::endl;
//std::cout << "NDMatrixPredInterps :" << NDMatrixPredInterps << std::endl;
//std::cout << "NDUnselTruePredGens :" << NDUnselTruePredGens << std::endl;
//std::cout << "NDUnselTruePredInterps :" << NDUnselTruePredInterps << std::endl;
//std::cout << "END INSIDE SAVING LOOP PRINT OUT" << std::endl;
      if (IsND) { // Is ND

        SaveTo(fout,
               std::string("NDMatchInterp_ETrue") +
                   (IsND280kA ? "_280kA" : "_293kA") + (IsNu ? "_nu" : "_nub") +
                   (IsNDNue ? "_NDnue" : "_NDnumu"),
               MatchPredInterps[config]);

std::cout << std::string("NDMatchInterp_ETrue") +
                   (IsND280kA ? "_280kA" : "_293kA") + (IsNu ? "_nu" : "_nub") +
                   (IsNDNue ? "_NDnue" : "_NDnumu") << std::endl;

        if (!IsND280kA) {
          SaveTo(fout,
                 std::string("NDMatrixInterp_ERecETrue") +
                     (IsNu ? "_nu" : "_nub") + (IsNDNue ? "_NDnue" : "_NDnumu"),
                 NDMatrixPredInterps[config]);
        }

        SaveTo(fout,
               std::string("NDUnSelected_ETrue") +
                   (IsND280kA ? "_280kA" : "_293kA") + (IsNu ? "_nu" : "_nub") +
                   (IsNDNue ? "_NDnue" : "_NDnumu"),
               NDUnselTruePredInterps[config]);
        SaveTo(fout,
               std::string("NDSelected_ETrue") +
                   (IsND280kA ? "_280kA" : "_293kA") + (IsNu ? "_nu" : "_nub") +
                   (IsNDNue ? "_NDnue" : "_NDnumu"),
               NDSelTruePredInterps[config]);

      } else if (!IsND && !IsNuTau) { // Is FD and not nutau.
        if (!IsFDNue) {
          SaveTo(fout,
                 std::string("FDMatchInterp_ETrue_numu") +
                     (IsNu ? "_nu" : "_nub"),
                 MatchPredInterps[config]);
        }

        SaveTo(fout,
               std::string("FDMatrixInterp_ERecETrue") +
                   (IsFDNue ? "_nue" : "_numu") + (IsNu ? "_nu" : "_nub"),
               FDMatrixPredInterps[fd_config]);

        SaveTo(fout,
               std::string("FDUnSelected_ETrue") + (IsFDNue ? "_nue" : "_numu") +
                   (IsNu ? "_nu" : "_nub"),
               FDUnselTruePredInterps[fd_config]);

        SaveTo(fout,
               std::string("FDSelected_ETrue") + (IsFDNue ? "_nue" : "_numu") +
                   (IsNu ? "_nu" : "_nub"),
               FDSelTruePredInterps[fd_config]);

        SaveTo(fout,
               std::string("FDDataPred_") + axdescriptor +
                   (IsFDNue ? "_nue" : "_numu") + (IsNu ? "_nu" : "_nub"),
               FarDetDataPreds[fd_config]);

        SaveTo(fout,
               std::string("FDFakeDataBiasPred_") + axdescriptor +
                   (IsFDNue ? "_nue" : "_numu") + (IsNu ? "_nu" : "_nub"),
               FarDetFakeDataBiasPreds[fd_config]);
      }
    }

std::cout << "line 870 right before the save to LAST ONE" << std::endl;

    SaveTo(fout, (std::string("PRISM_") + axdescriptor), PRISM);

    fout.Write();
    fout.Close();
  }
