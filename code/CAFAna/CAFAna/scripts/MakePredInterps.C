#include "CAFAna/Analysis/common_fit_definitions.h"

using namespace ana;

#include <dirent.h>

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

SampleType sample;
std::string output_file_name;
std::string syst_descriptor = "";
AxisBlob axes = default_axes_v4;
std::vector<std::string> input_patterns;
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
      << "\t-n|--n-max <N>         : Max number of events to read.\n"
      << "\t                         and not within directory names.\n"
      << "\t                         i.e. /a/b/c/nd_[0-9]*.root\n"
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
      axes = GetAxisBlob(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-o") ||
               (std::string(argv[opt]) == "--output")) {
      output_file_name = argv[++opt];
    } else if ((std::string(argv[opt]) == "-i") ||
               (std::string(argv[opt]) == "--input-pattern")) {
      input_patterns.push_back(argv[++opt]);
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

  if (!do_no_op) {
    if (!input_patterns.size()) {
      std::cout << "[ERROR]: Expected to recieve at least one input pattern."
                << std::endl;
      SayUsage(argv);
      exit(2);
    }
  }

  std::vector<std::string> file_list;
  for (auto InputFilePattern : input_patterns) {
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
    std::string pattern =
        (last_slash_loc == std::string::npos)
            ? InputFilePattern
            : InputFilePattern.substr(last_slash_loc + 1,
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
    std::copy(CAFs.begin(), CAFs.end(), std::back_inserter(file_list));
  }

  if (!do_no_op) {
    if (!file_list.size()) {
      std::cout << "[ERROR]: Failed to find any matching input files."
                << std::endl;
      exit(4);
    }
  }
  for (auto f : file_list) {
    std::cout << "[INFO]: Reading from: " << f << std::endl;
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
    MakePredictionInterp(&fout, sample, los, nmax, axes, file_list);
    fout.Write();
    fout.Close();
  }
}
