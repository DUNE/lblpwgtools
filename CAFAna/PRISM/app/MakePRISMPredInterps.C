#include "CAFAna/Analysis/common_fit_definitions.h"

#include "CAFAna/Prediction/PredictionsForPRISM.h"

#include "CAFAna/PRISM/PRISMAnalysisDefinitions.h"
#include "CAFAna/PRISM/PRISMUtils.h"
#include "CAFAna/PRISM/PredictionPRISM.h"

using namespace ana;
using namespace PRISM;

#include <dirent.h>

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::string EnsureTrailingSlash(std::string str) {
  if (!str.size()) {
    return str;
  }
  if (str.back() != '/') {
    return str + '/';
  }
  return str;
}

std::string parseCode(std::regex_constants::error_type etype) {
  switch (etype) {
  case std::regex_constants::error_collate:
    return "error_collate: invalid collating element request";
  case std::regex_constants::error_ctype:
    return "error_ctype: invalid character class";
  case std::regex_constants::error_escape:
    return "error_escape: invalid escape character or trailing escape";
  case std::regex_constants::error_backref:
    return "error_backref: invalid back reference";
  case std::regex_constants::error_brack:
    return "error_brack: mismatched bracket([ or ])";
  case std::regex_constants::error_paren:
    return "error_paren: mismatched parentheses(( or ))";
  case std::regex_constants::error_brace:
    return "error_brace: mismatched brace({ or })";
  case std::regex_constants::error_badbrace:
    return "error_badbrace: invalid range inside a { }";
  case std::regex_constants::error_range:
    return "erro_range: invalid character range(e.g., [z-a])";
  case std::regex_constants::error_space:
    return "error_space: insufficient memory to handle this regular expression";
  case std::regex_constants::error_badrepeat:
    return "error_badrepeat: a repetition character (*, ?, +, or {) was not "
           "preceded by a valid regular expression";
  case std::regex_constants::error_complexity:
    return "error_complexity: the requested match is too complex";
  case std::regex_constants::error_stack:
    return "error_stack: insufficient memory to evaluate a match";
  default:
    return "";
  }
}

std::string DeGlobPattern(std::string const &pattern) {
  std::stringstream ss("");
  size_t next_asterisk = pattern.find_first_of('*');
  size_t next_to_add = 0;
  bool modified = false;
  while (next_asterisk != std::string::npos) {
    if ((pattern[next_asterisk - 1] !=
         ']') && // Try to allow valid uses of an asterisk without a preceding.
        (pattern[next_asterisk - 1] != '.')) {
      modified = true;
      // Add a .
      ss << pattern.substr(next_to_add, next_asterisk - next_to_add) << ".*";
      next_to_add = next_asterisk + 1;
      if (next_to_add >= pattern.size()) {
        next_to_add = std::string::npos;
      }
    }
    next_asterisk = pattern.find_first_of('*', next_to_add);
  }

  if (next_to_add != std::string::npos) {
    ss << pattern.substr(next_to_add);
  }

  if (modified) {
    std::cout << "[INFO]: DeGlobified input pattern: " << pattern
              << " to std::regex_friendly: " << ss.str() << std::endl;
  }

  return ss.str();
}

std::vector<std::string> GetMatchingFiles(std::string directory,
                                          std::string pattern,
                                          bool IncDir = true) {

  directory = EnsureTrailingSlash(directory);
  pattern = DeGlobPattern(pattern);

  std::cout << "[INFO]: Looking for files matching: \"" << pattern
            << "\" in directory: " << directory << std::endl;

  std::regex rpattern(pattern);

  std::vector<std::string> matches;

  DIR *dir;
  struct dirent *ent;
  dir = opendir(directory.c_str());
  if (dir != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      if (std::regex_match(ent->d_name, rpattern)) {
        matches.push_back(IncDir ? directory + std::string(ent->d_name)
                                 : std::string(ent->d_name));
      }
    }
  }
  return matches;
}

std::string output_file_name;
std::string syst_descriptor = "nosyst";
std::string axdescriptor = "EProxy";
std::string binningdescriptor = "default";
std::string oabinningdescriptor = "default";
std::string truthbinningdescriptor = "uniform";
std::vector<std::string> ND_input_numode;
std::vector<std::string> FD_nonswap_input_numode;
std::vector<std::string> FD_nueswap_input_numode;
std::vector<std::string> ND_input_nubmode;
std::vector<std::string> FD_nonswap_input_nubmode;
std::vector<std::string> FD_nueswap_input_nubmode;
bool addfakedata = true;
bool do_no_op = false;
unsigned nmax = 0;
bool UseSel = false;
std::string anaweighters = "";
std::string FakeDataShiftDescript = "";
double ThinFactor = 0;

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
      << "\t-F-nu|--FD-input-numode <P> : Regex pattern to search for input\n"
      << "\t                            Files. Can only include pattern \n"
         "\t                            elements for files.\n"
      << "\t-Fe-nu|--FD-nue-input-numode <P> : Regex pattern to search for\n"
      << "\t                            input files. Can only include pattern\n"
         "\t                            elements for files.\n"
      << "\t-N-nub|--ND-input-nubmode <P> : Regex pattern to search for input\n"
      << "\t                            Files. Can only include pattern \n"
         "\t                            elements for files.\n"
      << "\t-F-nub|--FD-input-nubmode <P> : Regex pattern to search for input\n"
      << "\t                            Files. Can only include pattern \n"
         "\t                            elements for files.\n"
      << "\t-Fe-nub|--FD-nue-input-nubmode <P> : Regex pattern to search for\n"
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
      << "\t--thin-factor [0-1]       : Factor to thin input by, will also "
         "reduce input POT."
      << std::endl;
}

void handleOpts(int argc, char const *argv[]) {
  int opt = 1;
  while (opt < argc) {
    if (std::string(argv[opt]) == "-?" || std::string(argv[opt]) == "--help") {
      SayUsage(argv);
      exit(0);
    } else if ((std::string(argv[opt]) == "-A") ||
               (std::string(argv[opt]) == "--axes")) {
      axdescriptor = argv[++opt];
    } else if ((std::string(argv[opt]) == "-o") ||
               (std::string(argv[opt]) == "--output")) {
      output_file_name = argv[++opt];
    } else if ((std::string(argv[opt]) == "-N-nu") ||
               (std::string(argv[opt]) == "--ND-input-numode")) {
      ND_input_numode.push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-F-nu") ||
               (std::string(argv[opt]) == "--FD-input-numode")) {
      FD_nonswap_input_numode.push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-Fe-nu") ||
               (std::string(argv[opt]) == "--FD-nue-input-numode")) {
      FD_nueswap_input_numode.push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-N-nub") ||
               (std::string(argv[opt]) == "--ND-input-nubmode")) {
      ND_input_nubmode.push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-F-nub") ||
               (std::string(argv[opt]) == "--FD-input-nubmode")) {
      FD_nonswap_input_nubmode.push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-Fe-nub") ||
               (std::string(argv[opt]) == "--FD-nue-input-nubmode")) {
      FD_nueswap_input_nubmode.push_back(argv[++opt]);
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
    } else if (std::string(argv[opt]) == "--thin-factor") {
      ThinFactor = std::stod(argv[++opt]);
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

  handleOpts(argc, argv);

  // Parse input file list patterns.
  std::vector<std::pair<std::string, std::vector<std::string>>> file_lists;
  size_t NFiles = 0;
  for (auto ip : std::vector<std::pair<std::string, std::vector<std::string>>>{
           {"ND_numode", ND_input_numode},
           {"FD_nonswap_numode", FD_nonswap_input_numode},
           {"FD_nueswap_numode", FD_nueswap_input_numode},
           {"ND_nubmode", ND_input_nubmode},
           {"FD_nonswap_nubmode", FD_nonswap_input_nubmode},
           {"FD_nueswap_nubmode", FD_nueswap_input_nubmode}}) {
    file_lists.emplace_back(ip.first, std::vector<std::string>{});
    for (auto InputFilePattern : ip.second) {
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

      if ((earliest_regex == std::string::npos)) {
        CAFs.push_back(dir + pattern);
      } else {
        try {
          CAFs = GetMatchingFiles(dir, pattern);
        } catch (std::regex_error const &e) {
          std::cout << "[ERROR]: " << e.what() << ", " << parseCode(e.code())
                    << std::endl;
          exit(3);
        }
      }
      std::copy(CAFs.begin(), CAFs.end(),
                std::back_inserter(file_lists.back().second));
      NFiles += CAFs.size();
    }
  }
  // Have lists of files.

  for (size_t fl_it = 0; fl_it < file_lists.size(); ++fl_it) {
    std::vector<std::string> const &fl = file_lists[fl_it].second;
    if (!fl.size()) {
      std::cout << "[WARN]: Failed to find any " << file_lists[fl_it].first
                << " files." << std::endl;
    }
    for (auto f : fl) {
      std::cout << "[INFO]: Reading from: " << f << std::endl;
    }
  }

  if (!NFiles) {
    std::cout << "[ERROR]: Found no files." << std::endl;
    abort();
  }

  // Sort out systematics if they've been requested
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

  PRISMAxisBlob axes =
      GetPRISMAxes(axdescriptor, binningdescriptor, oabinningdescriptor);

  HistAxis EventRateMatchAxis = GetEventRateMatchAxes(truthbinningdescriptor);

  std::vector<Var> WeightVars(6, ana::Constant(1));
  WeightVars[kND_nu] = GetNDWeight("", true);
  WeightVars[kND_nub] = GetNDWeight("", false);
  WeightVars[kFD_nu_nonswap] = GetFDWeight("", true);
  WeightVars[kFD_nu_nueswap] = GetFDWeight("", true);
  WeightVars[kFD_nub_nonswap] = GetFDWeight("", false);
  WeightVars[kFD_nub_nueswap] = GetFDWeight("", false);

  std::vector<Var> AnaWeightVars(6, ana::Constant(1));
  AnaWeightVars[kND_nu] = GetNDWeight(anaweighters, true);
  AnaWeightVars[kND_nub] = GetNDWeight(anaweighters, false);
  AnaWeightVars[kFD_nu_nonswap] = GetFDWeight(anaweighters, true);
  AnaWeightVars[kFD_nu_nueswap] = GetFDWeight(anaweighters, true);
  AnaWeightVars[kFD_nub_nonswap] = GetFDWeight(anaweighters, false);
  AnaWeightVars[kFD_nub_nueswap] = GetFDWeight(anaweighters, false);

  // Generally these will be just selecting signal and are the ones used in the
  // PRISM interp
  std::vector<Cut> AnalysisCuts(
      6, Cut([](const caf::StandardRecord *) { return false; }));
  AnalysisCuts[kND_nu] = GetNDSignalCut(UseSel, true);
  AnalysisCuts[kND_nub] = GetNDSignalCut(UseSel, false);
  AnalysisCuts[kFD_nu_nonswap] = GetFDSignalCut(UseSel, true, true);
  AnalysisCuts[kFD_nu_nueswap] = GetFDSignalCut(UseSel, true, false);
  AnalysisCuts[kFD_nub_nonswap] = GetFDSignalCut(UseSel, false, true);
  AnalysisCuts[kFD_nub_nueswap] = GetFDSignalCut(UseSel, false, false);

  // These are the current 'standard' analysis cuts that try to mock up a real
  // selection, these will be used for
  std::vector<Cut> OnAxisSelectionCuts(
      6, Cut([](const caf::StandardRecord *) { return false; }));
  OnAxisSelectionCuts[kND_nu] = GetNDSignalCut(true, true);
  OnAxisSelectionCuts[kND_nub] = GetNDSignalCut(true, false);
  OnAxisSelectionCuts[kFD_nu_nonswap] = GetFDSignalCut(true, true, true);
  OnAxisSelectionCuts[kFD_nu_nueswap] = GetFDSignalCut(true, true, false);
  OnAxisSelectionCuts[kFD_nub_nonswap] = GetFDSignalCut(true, false, true);
  OnAxisSelectionCuts[kFD_nub_nueswap] = GetFDSignalCut(true, false, false);

  if (ThinFactor > 0) { // Have to correct for the Thinned POT as the total
                        // Input POT is included in the PerPOTWeight.
    auto thinfactweight = ana::Var([](const caf::StandardRecord *) -> double {
      return 1.0 / (1.0 - ThinFactor);
    });
    AnaWeightVars[kND_nu] = AnaWeightVars[kND_nu] * thinfactweight;
    AnaWeightVars[kND_nub] = AnaWeightVars[kND_nub] * thinfactweight;
  }

  ana::SystShifts DataShift =
      GetFakeDataGeneratorSystShift(FakeDataShiftDescript);

  auto PRISM = std::make_unique<PredictionPRISM>(
      axes.XProjection, axes.OffAxisPosition, axes.OffAxis280kAPosition,
      EventRateMatchAxis);

  Loaders Loaders_nu, Loaders_nub, Loaders_thinned;

  std::vector<std::unique_ptr<SpectrumLoader>> FileLoaders, ThinnedLoaders;
  FillWithNulls(FileLoaders, 6);
  FillWithNulls(ThinnedLoaders, 2);
  for (size_t it = 0; it < file_lists.size(); ++it) {
    bool IsNu = IsNuConfig(it);
    bool IsND = IsNDConfig(it);
    size_t IsNueSwap = IsNueConfig(it);

    if (!file_lists[it].second.size()) {
      continue;
    }
    Loaders &Loaders_bm = IsNu ? Loaders_nu : Loaders_nub;
    FileLoaders[it] =
        std::make_unique<SpectrumLoader>(file_lists[it].second, kBeam, nmax);
    if (IsND) { // Is ND

      BeamChan chanmode = IsNu ? kNumu_Numode : kNumuBar_NuBarmode;

      if (ThinFactor > 0) { // If we are thinning then we only want to thin the
                            // 'data' and so have to have a separate loader.
        ThinnedLoaders[it / 3] = std::make_unique<SpectrumLoader>(
            file_lists[it].second, kBeam, nmax);
        ThinnedLoaders[it / 3]->SetThinFactor(ThinFactor);
        PRISM->AddNDDataLoader(*ThinnedLoaders[it / 3], AnalysisCuts[it],
                               AnaWeightVars[it], DataShift, chanmode);
        Loaders_thinned.AddLoader(ThinnedLoaders[it / 3].get(), caf::kNEARDET,
                                  Loaders::kMC);
      } else {

        PRISM->AddNDDataLoader(*FileLoaders[it], AnalysisCuts[it],
                               AnaWeightVars[it], DataShift, chanmode);
      }

      Loaders_bm.AddLoader(FileLoaders[it].get(), caf::kNEARDET, Loaders::kMC);

    } else { // Is FD
      Loaders_bm.AddLoader(FileLoaders[it].get(), caf::kFARDET, Loaders::kMC,
                           kBeam,
                           IsNueSwap ? Loaders::kNueSwap : Loaders::kNonSwap);
    }
  }

  // Make the ND prediction interp include the same off-axis axis used for
  // PRISM weighting.
  std::vector<std::string> Labels_evrmatch = EventRateMatchAxis.GetLabels();
  std::vector<Binning> Bins_evrmatch = EventRateMatchAxis.GetBinnings();
  std::vector<Var> Vars_evrmatch = EventRateMatchAxis.GetVars();

  Labels_evrmatch.push_back(axes.OffAxisPosition.GetLabels().front());
  Bins_evrmatch.push_back(axes.OffAxisPosition.GetBinnings().front());
  Vars_evrmatch.push_back(axes.OffAxisPosition.GetVars().front());

  HistAxis const NDEventRateSpectraAxis(Labels_evrmatch, Bins_evrmatch,
                                        Vars_evrmatch);

  std::vector<std::string> Labels_obs = axes.XProjection.GetLabels();
  std::vector<Binning> Bins_obs = axes.XProjection.GetBinnings();
  std::vector<Var> Vars_obs = axes.XProjection.GetVars();

  Labels_obs.push_back(axes.OffAxisPosition.GetLabels().front());
  Bins_obs.push_back(axes.OffAxisPosition.GetBinnings().front());
  Vars_obs.push_back(axes.OffAxisPosition.GetVars().front());

  HistAxis const NDObservedSpectraAxis(Labels_obs, Bins_obs, Vars_obs);

  std::vector<std::unique_ptr<IPredictionGenerator>> MatchPredGens;
  std::vector<std::unique_ptr<PredictionInterp>> MatchPredInterps;
  FillWithNulls(MatchPredGens, 6);
  FillWithNulls(MatchPredInterps, 6);

  std::vector<std::unique_ptr<IPredictionGenerator>> SelPredGens;
  std::vector<std::unique_ptr<PredictionInterp>> SelPredInterps;
  FillWithNulls(SelPredGens, 6);
  FillWithNulls(SelPredInterps, 6);

  std::vector<std::unique_ptr<OscillatableSpectrum>> FarDetData_nonswap;
  std::vector<std::unique_ptr<OscillatableSpectrum>> FarDetData_nueswap;
  FillWithNulls(FarDetData_nonswap, 4);
  FillWithNulls(FarDetData_nueswap, 4);

  std::vector<std::unique_ptr<IPredictionGenerator>> FarDetPredGens;
  std::vector<std::unique_ptr<PredictionInterp>> FarDetPredInterps;
  FillWithNulls(FarDetPredGens, 4);
  FillWithNulls(FarDetPredInterps, 4);

  static osc::NoOscillations no_osc;

  for (size_t it = 0; it < file_lists.size(); ++it) {
    bool IsNu = IsNuConfig(it);
    bool IsND = IsNDConfig(it);
    size_t fd_it = 0;
    size_t IsNueSwap = IsNueConfig(it);
    if (!IsND) {
      fd_it = GetFDConfig(it);
    }

    Loaders &Loaders_bm = IsNu ? Loaders_nu : Loaders_nub;

    if (!FileLoaders[it]) {
      continue;
    }
    if (IsND) { // Is ND

      BeamChan chanmode = IsNu ? kNumu_Numode : kNumuBar_NuBarmode;

      PRISM->AddNDMCLoader(Loaders_bm, AnalysisCuts[it], AnaWeightVars[it], los,
                           chanmode);

      MatchPredGens[it] = std::make_unique<NoOscPredictionGenerator>(
          NDEventRateSpectraAxis,
          kIsNumuCC && (IsNu ? !kIsAntiNu : kIsAntiNu) && kIsTrueFV &&
              kIsOutOfTheDesert,
          WeightVars[it]);

      MatchPredInterps[it] = std::make_unique<PredictionInterp>(
          los_flux, &no_osc, *MatchPredGens[it], Loaders_bm);

      SelPredGens[it] = std::make_unique<NoOscPredictionGenerator>(
          NDObservedSpectraAxis, OnAxisSelectionCuts[it], AnaWeightVars[it]);
      SelPredInterps[it] = std::make_unique<PredictionInterp>(
          los, &no_osc, *SelPredGens[it], Loaders_bm);
    } else { // Is FD

      BeamChan chanmode{IsNu ? BeamMode::kNuMode : BeamMode::kNuBarMode,
                        IsNueSwap ? NuChan::kNueNueBar : NuChan::kNumuNumuBar};

      PRISM->AddFDMCLoader(Loaders_bm, AnalysisCuts[it], AnaWeightVars[it], los,
                           chanmode);
      // We always want to use the numus as we don't want to account for any
      // xsec differences between numu and nues, we use a special prediction
      // object to allow us to oscillate the NuMu spectrum.
      if (!IsNueSwap) {
        MatchPredGens[it] =
            std::make_unique<NonSwapNoExtrapPredictionGenerator>(
                EventRateMatchAxis,
                kIsNumuCC && (IsNu ? !kIsAntiNu : kIsAntiNu) && kIsTrueFV,
                WeightVars[it]);

        MatchPredInterps[it] = std::make_unique<PredictionInterp>(
            los_flux, &no_osc, *MatchPredGens[it], Loaders_bm);
      }

      size_t non_swap_it = GetConfigNonSwap(it);
      size_t nue_swap_it = GetConfigNueSwap(it);

      if (FileLoaders[non_swap_it]) {
        FarDetData_nonswap[fd_it] = std::make_unique<OscillatableSpectrum>(
            *FileLoaders[non_swap_it], axes.XProjection, AnalysisCuts[it],
            DataShift, AnaWeightVars[it]);
      }

      if (FileLoaders[nue_swap_it]) {
        FarDetData_nueswap[fd_it] = std::make_unique<OscillatableSpectrum>(
            *FileLoaders[nue_swap_it], axes.XProjection, AnalysisCuts[it],
            DataShift, AnaWeightVars[it]);
      }

      FarDetPredGens[fd_it] = std::make_unique<NoExtrapPredictionGenerator>(
          axes.XProjection, AnalysisCuts[it], AnaWeightVars[it]);
      FarDetPredInterps[fd_it] = std::make_unique<PredictionInterp>(
          los, &no_osc, *FarDetPredGens[fd_it], Loaders_bm);

      SelPredGens[it] = std::make_unique<NoExtrapPredictionGenerator>(
          axes.XProjection, OnAxisSelectionCuts[it], AnaWeightVars[it]);
      SelPredInterps[it] = std::make_unique<PredictionInterp>(
          los, &no_osc, *SelPredGens[it], Loaders_bm);
    }
  }

  Loaders_nu.Go();
  Loaders_nub.Go();
  Loaders_thinned.Go();

  for (size_t it = 0; it < file_lists.size(); ++it) {
    bool IsNu = IsNuConfig(it);
    bool IsND = IsNDConfig(it);
    size_t fd_it = 0;
    size_t IsNue = IsNueConfig(it);
    if (!IsND) {
      fd_it = GetFDConfig(it);
    }
    if (!FileLoaders[it]) {
      continue;
    }

    if (IsND) { // Is ND
      MatchPredInterps[it]->GetPredNomAs<PredictionNoOsc>()->OverridePOT(1);
      SelPredInterps[it]->GetPredNomAs<PredictionNoOsc>()->OverridePOT(1);
      SaveTo(fout, std::string("NDMatchInterp_ETrue") + (IsNu ? "_nu" : "_nub"),
             MatchPredInterps[it]);
      SaveTo(fout,
             std::string("NDSelectedInterp_") + axdescriptor +
                 (IsNu ? "_nu" : "_nub"),
             SelPredInterps[it]);

    } else { // Is FD
      if (!IsNue) {
        SaveTo(fout,
               std::string("FDMatchInterp_ETrue_numu") +
                   (IsNu ? "_nu" : "_nub"),
               MatchPredInterps[it]);
      }

      SaveTo(fout,
             std::string("FDInterp_") + axdescriptor +
                 (IsNue ? "_nue" : "_numu") + (IsNu ? "_nu" : "_nub"),
             FarDetPredInterps[fd_it]);

      std::cout << "Saving: "
                << std::string("FDInterp_") + axdescriptor +
                       (IsNue ? "_nue" : "_numu") + (IsNu ? "_nu" : "_nub")
                << " to " << it << ", " << fd_it << std::endl;

      if (FarDetData_nonswap[fd_it]) {
        SaveTo(fout,
               std::string("FDDataNonSwap_") + axdescriptor +
                   (IsNue ? "_nue" : "_numu") + (IsNu ? "_nu" : "_nub"),
               FarDetData_nonswap[fd_it]);
      }
      if (FarDetData_nueswap[fd_it]) {
        SaveTo(fout,
               std::string("FDDataNueSwap_") + axdescriptor +
                   (IsNue ? "_nue" : "_numu") + (IsNu ? "_nu" : "_nub"),
               FarDetData_nueswap[fd_it]);
      }

      SaveTo(fout,
             std::string("FDSelectedInterp_") + axdescriptor +
                 (IsNue ? "_nue" : "_numu") + (IsNu ? "_nu" : "_nub"),
             SelPredInterps[it]);
    }
  }

  PRISM->SaveTo(fout.mkdir((std::string("PRISM_") + axdescriptor).c_str()));

  fout.Write();
  fout.Close();
}
