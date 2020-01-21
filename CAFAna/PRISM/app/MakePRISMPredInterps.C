#include "CAFAna/Analysis/common_fit_definitions.h"

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

bool IsRHC = false;
std::string output_file_name;
std::string syst_descriptor = "nosyst";
std::string axdescriptor = "EProxy";
std::string binningdescriptor = "default";
std::string oabinningdescriptor = "default";
std::string truthbinningdescriptor = "uniform";
std::vector<std::string> ND_input_patterns;
std::vector<std::string> FD_input_patterns;
std::pair<std::string, std::string> SpecialRunWeightHist;
bool addfakedata = true;
bool do_no_op = false;
unsigned nmax = 0;

void SayUsage(char const *argv[]) {
  std::cout
      << "[USAGE]: " << argv[0] << "\n"
      << "\t-A|--axes <A>             : Fit axis specifier. (default: A = "
         "\"EProxy\")\n"
      << "\t-o|--output <F>           : Output file name. Should not "
         "already "
         "exist.\n"
      << "\t-N|--ND-input-pattern <P> : Regex pattern to search for input\n"
      << "\t                            Files. Can only include pattern \n"
         "\t                            elements for files.\n"
      << "\t-F|--FD-input-pattern <P> : Regex pattern to search for input\n"
      << "\t                            Files. Can only include pattern \n"
         "\t                            elements for files.\n"
      << "\t-R|--is-RHC               : Inputs are RHC."
      << "\t-n|--n-max <N>            : Max number of events to read.\n"
      << "\t--syst-descriptor <str>   : Only add dials matching the syst\n"
         "\t                            descriptor <str> to the state "
         "file.\n"
      << "\t--bin-descriptor <str>    : Bin descriptor.\n"
      << "\t--OA-bin-descriptor <str>    : Bin descriptor.\n"
      << "\t--truth-bin-descriptor <str> : Bin descriptor.\n"
      << "\t--no-fakedata-dials       : Do not add the fake data dials to "
         "the\n"
         "\t                            state file\n"
      << "\t--no-op                   : Do nothing but dump dials that "
         "would\n"
         "\t                            be included.\n"
      << "\t--FakeSR <file:whist>     : Fake a single special beam run by\n"
         "\t                            weighting on axis events with x < 0 \n"
         "\t                            in true energy with associated \n"
         "\t                            histogram."
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
    } else if ((std::string(argv[opt]) == "-N") ||
               (std::string(argv[opt]) == "--ND-input-pattern")) {
      ND_input_patterns.push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-F") ||
               (std::string(argv[opt]) == "--FD-input-pattern")) {
      FD_input_patterns.push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-R") ||
               (std::string(argv[opt]) == "--is-RHC")) {
      IsRHC = true;
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
    } else if (std::string(argv[opt]) == "--no-op") {
      do_no_op = true;
    } else if (std::string(argv[opt]) == "--FakeSR") {

      std::string arg = argv[++opt];

      if (std::count(arg.begin(), arg.end(), ';') != 1) {
        std::cout << "[ERROR]: --FakeSR option expected to be passed "
                     "\"file.root;histname\""
                  << std::endl;
      }

      size_t scl = arg.find_first_of(';');
      SpecialRunWeightHist.first = arg.substr(0, scl);
      SpecialRunWeightHist.second = arg.substr(scl + 1);

      std::cout << "[INFO]: Will build fake special HC run with "
                << SpecialRunWeightHist.first << ";"
                << SpecialRunWeightHist.second << std::endl;
    } else {
      std::cout << "[ERROR]: Unknown option: " << argv[opt] << std::endl;
      SayUsage(argv);
      exit(1);
    }
    opt++;
  }
}

int main(int argc, char const *argv[]) {

  handleOpts(argc, argv);

  if (!do_no_op) {
    if (!ND_input_patterns.size() || !FD_input_patterns.size()) {
      std::cout << "[ERROR]: Expected to receive at least one ND and one FD "
                   "input pattern."
                << std::endl;
      SayUsage(argv);
      exit(2);
    }
  }

  std::vector<std::vector<std::string>> file_lists;
  size_t NFiles = 0;
  for (auto ip : {ND_input_patterns, FD_input_patterns}) {
    file_lists.emplace_back();
    for (auto InputFilePattern : ip) {
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
                std::back_inserter(file_lists.back()));
      NFiles += CAFs.size();
    }
  }

  for (size_t fl_it = 0; fl_it < file_lists.size(); ++fl_it) {
    std::vector<std::string> fl = file_lists[fl_it];
    if (!fl.size()) {
      std::cout << "[ERROR]: Failed to find any " << (fl_it ? "ND" : "FD")
                << " files." << std::endl;
    }
    for (auto f : fl) {
      std::cout << "[INFO]: Reading from: " << f << std::endl;
    }
  }

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

  TH1 *whist = nullptr;
  if (SpecialRunWeightHist.first.size()) {
    TDirectory *gdc = gDirectory;
    TFile wf(SpecialRunWeightHist.first.c_str());
    wf.GetObject(SpecialRunWeightHist.second.c_str(), whist);
    if (!whist) {
      std::cout << "[ERROR]: Failed to read " << SpecialRunWeightHist.second
                << " from " << SpecialRunWeightHist.first << std::endl;
      throw;
    }
    whist->SetDirectory(nullptr);
    if (gdc) {
      gdc->cd();
    }
  }

  if (!do_no_op) {
    TFile fout(output_file_name.c_str(), "RECREATE");

    PRISMAxisBlob axes =
        GetPRISMAxes(axdescriptor, binningdescriptor, oabinningdescriptor);

    HistAxis EventRateMatchAxis = GetEventRateMatchAxes(truthbinningdescriptor);

    Var NDWeight = GetNDWeight();
    Var FDWeight = GetFDWeight();
    Cut NDSignalCut = GetNDSignalCut();
    Cut FDSignalCut = GetFDSignalCut();

    if (whist) { // If we are faking a special run
      NDWeight =
          NDWeight * ana::Var({}, [&](const caf::StandardRecord *sr) -> double {
            if (sr->dune.det_x) {
              return 1;
            }
            if (sr->dune.vtx_x > 0) {
              return 1;
            }
            std::cout << "bla: " << sr->dune.Ev << " gw = "
                      << whist->GetBinContent(
                             whist->GetXaxis()->FindFixBin(sr->dune.Ev))
                      << std::endl;
            // Correct for the bin width
            return (0.5 / 3) * whist->GetBinContent(
                                   whist->GetXaxis()->FindFixBin(sr->dune.Ev));
          });
    }

    Loaders TheLoaders;

    SpectrumLoader PRISMNDLoader(file_lists[0], kBeam, nmax);
    TheLoaders.AddLoader(&PRISMNDLoader, caf::kNEARDET, Loaders::kMC);

    SpectrumLoader loaderFDNumu(file_lists[1], kBeam, nmax);
    TheLoaders.AddLoader(&loaderFDNumu, caf::kFARDET, Loaders::kMC, ana::kBeam,
                         Loaders::kNonSwap);

    auto PRISM = std::make_unique<PredictionPRISM>(
        PRISMNDLoader, axes.XProjection, axes.OffAxisPosition, NDSignalCut,
        NDWeight);

    // Don't need to specify full truth signal here as it will not apply any
    // corrections by default, allows us to debug what the corrections would
    // be.
    // If you aren't doing systematic studies then this is just a waste of
    // time, but if you are you should pass a loader here and call
    // SetIgnoreData to use the 'MC' near detector signal prediction in the
    // linear combination.
    PRISM->AddNDMCLoader(TheLoaders, kIsTrueFV && kIsOutOfTheDesert, NDWeight,
                         los);

    PRISM->AddFDMCLoader(TheLoaders, kIsTrueFV, FDWeight, los);

    // Make the ND prediction interp include the same off-axis axis used for
    // PRISM weighting.
    std::vector<std::string> Labels = EventRateMatchAxis.GetLabels();
    std::vector<Binning> Bins = EventRateMatchAxis.GetBinnings();
    std::vector<Var> Vars = EventRateMatchAxis.GetVars();

    Labels.push_back(axes.OffAxisPosition.GetLabels().front());
    Bins.push_back(axes.OffAxisPosition.GetBinnings().front());
    Vars.push_back(axes.OffAxisPosition.GetVars().front());

    HistAxis const NDEventRateSpectraAxis(Labels, Bins, Vars);

    std::unique_ptr<NoOscPredictionGenerator> NDMatchPredGen(
        new NoOscPredictionGenerator(NDEventRateSpectraAxis, NDSignalCut,
                                     NDWeight));

    std::unique_ptr<NoExtrapPredictionGenerator> FDMatchPredGen(
        new NoExtrapPredictionGenerator(EventRateMatchAxis, FDSignalCut,
                                        FDWeight));

    osc::IOscCalculatorAdjustable *calc = NuFitOscCalc(1);

    // These only require access to the flux systs.
    auto NDMatchInterp = std::make_unique<PredictionInterp>(
        los_flux, calc, *NDMatchPredGen, TheLoaders);
    auto FDMatchInterp = std::make_unique<PredictionInterp>(
        los_flux, calc, *FDMatchPredGen, TheLoaders);

    std::unique_ptr<NoExtrapPredictionGenerator> FDPredGen(
        new NoExtrapPredictionGenerator(axes.XProjection, FDSignalCut,
                                        FDWeight));

    auto FarDet =
        std::make_unique<PredictionInterp>(los, calc, *FDPredGen, TheLoaders);

    TheLoaders.Go();

    PRISM->SaveTo(fout.mkdir((std::string("PRISM_") + axdescriptor +
                              std::string(IsRHC ? "_rhc" : "_fhc"))
                                 .c_str()));

    NDMatchInterp->GetPredNomAs<PredictionNoOsc>()->OverridePOT(1);
    NDMatchInterp->SaveTo(fout.mkdir((std::string("NDMatchInterp_ETrue") +
                                      std::string(IsRHC ? "_rhc" : "_fhc"))
                                         .c_str()));
    FDMatchInterp->SaveTo(fout.mkdir((std::string("FDMatchInterp_ETrue") +
                                      std::string(IsRHC ? "_rhc" : "_fhc"))
                                         .c_str()));

    FarDet->SaveTo(fout.mkdir((std::string("FarDet_") + axdescriptor +
                               std::string(IsRHC ? "_rhc" : "_fhc"))
                                  .c_str()));

    fout.Write();
    fout.Close();
  }
}
