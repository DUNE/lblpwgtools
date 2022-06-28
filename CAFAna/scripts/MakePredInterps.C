#include "CAFAna/Analysis/common_fit_definitions.h"

using namespace ana;

#include <dirent.h>

#include <iostream>
#include <regex>
#include <string>
#include <vector>

SampleType sample;
std::string output_file_name;
std::string syst_descriptor = "";
std::string axdescriptor = "v4";
std::vector<std::string> input_patterns;
std::vector<std::string> nue_input_patterns;
std::vector<std::string> tau_input_patterns;
bool addfakedata = true;
bool do_no_op = false;
unsigned nmax = 0;

void SayUsage(char const *argv[]) {
  std::cout
      << "[USAGE]: " << argv[0] << "\n"
      << "\t-S|--sample <S>        : Sample type (<S> = one of "
         "{ND,FD}_{FHC,RHC})\n"
      << "\t-A|--axes <A>          : Fit axis specifier. (default: A = "
         "\"default_axes\")\n"
      << "\t-o|--output <F>        : Output file name. Should not already "
         "exist.\n"
      << "\t-i|--input-pattern <P> : Regex pattern to search for input files.\n"
      << "\t                         Can only include pattern elements for "
         "files\n"
      << "\t                         and not within directory names.\n"
      << "\t                         i.e. /a/b/c/nd_[0-9]*.root\n"
      << "\t-e|--nue-input-pattern : As above but specifies nueswap files.\n"
      << "\t-t|--tau-input-pattern : As above but specifies nutauswap files.\n"
      << "\t-n|--n-max <N>         : Max number of events to read.\n"
      << "\t--syst-descriptor <str>: Only add dials matching the syst\n"
         "\t                         descriptor <str> to the state file.\n"
      << "\t--no-fakedata-dials    : Do not add the fake data dials to the\n"
         "\t                         state file\n"
      << "\t--no-op              : Do nothing but dump dials that would be "
         "included.\n"
      << std::endl;
}

void handleOpts(int argc, char const *argv[]) {
  int opt = 1;
  while (opt < argc) {
    if (std::string(argv[opt]) == "-?" || std::string(argv[opt]) == "--help") {
      SayUsage(argv);
      exit(0);
    } else if ((std::string(argv[opt]) == "-S") ||
               (std::string(argv[opt]) == "--sample")) {
      sample = GetSampleType(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-A") ||
               (std::string(argv[opt]) == "--axes")) {
      axdescriptor = argv[++opt];
    } else if ((std::string(argv[opt]) == "-o") ||
               (std::string(argv[opt]) == "--output")) {
      output_file_name = argv[++opt];
    } else if ((std::string(argv[opt]) == "-i") ||
               (std::string(argv[opt]) == "--input-pattern")) {
      input_patterns.push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-e") ||
               (std::string(argv[opt]) == "--nue-input-pattern")) {
      nue_input_patterns.push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-t") ||
               (std::string(argv[opt]) == "--tau-input-pattern")) {
      tau_input_patterns.push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-n") ||
               (std::string(argv[opt]) == "--n-max")) {
      nmax = atoi(argv[++opt]);
    } else if (std::string(argv[opt]) == "--syst-descriptor") {
      syst_descriptor = argv[++opt];
    } else if (std::string(argv[opt]) == "--no-fakedata-dials") {
      addfakedata = false;
    } else if (std::string(argv[opt]) == "--no-op") {
      do_no_op = true;
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

  AxisBlob axes = GetAxisBlob(axdescriptor);

  if (!do_no_op) {
    if (!input_patterns.size()) {
      std::cout << "[ERROR]: Expected to recieve at least one input pattern."
                << std::endl;
      SayUsage(argv);
      exit(2);
    }
  }

  std::vector<std::vector<std::string>> file_lists;
  size_t NFiles = 0;
  for (auto ip : {input_patterns, nue_input_patterns, tau_input_patterns}) {
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
                std::back_inserter(file_lists.back()));
      NFiles += CAFs.size();
    }
  }

  if (!do_no_op) {
    if (!NFiles) {
      std::cout << "[ERROR]: Failed to find any matching input files."
                << std::endl;
      exit(4);
    }
  }
  for (auto fl : file_lists) {
    for (auto f : fl) {
      std::cout << "[INFO]: Reading from: " << f << std::endl;
    }
  }

  std::vector<ana::ISyst const *> los;
  if (syst_descriptor.size()) {
    los = GetListOfSysts(syst_descriptor);

    // Have to add fake data systs too.
    std::vector<ana::ISyst const *> fdlos = GetListOfSysts(
        false, false, false, false, false, false, addfakedata, false);
    los.insert(los.end(), fdlos.begin(), fdlos.end());

  } else {
    // Default but allow fake data fials to be turned off
    los = GetListOfSysts(true, true, true, true, true, false, addfakedata);
  }
  std::cout << "[INFO]: Using " << los.size()
            << " systematic dials: " << std::endl;
  for (auto s : los) {
    std::cout << "[INFO]:\t" << s->ShortName() << std::endl;
  }

  if (!do_no_op) {
    TFile fout(output_file_name.c_str(), "RECREATE");
    MakePredictionInterp(&fout, sample, los, axes, file_lists[0], file_lists[1],
                         file_lists[2], nmax);
    fout.Write();
    fout.Close();
  }
}
