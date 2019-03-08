#include "CAFAna/Cuts/TruthCuts.h"

#include "TChain.h"
#include "TFile.h"
#include "TH1.h"

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

string parseCode(regex_constants::error_type etype) {
  switch (etype) {
  case regex_constants::error_collate:
    return "error_collate: invalid collating element request";
  case regex_constants::error_ctype:
    return "error_ctype: invalid character class";
  case regex_constants::error_escape:
    return "error_escape: invalid escape character or trailing escape";
  case regex_constants::error_backref:
    return "error_backref: invalid back reference";
  case regex_constants::error_brack:
    return "error_brack: mismatched bracket([ or ])";
  case regex_constants::error_paren:
    return "error_paren: mismatched parentheses(( or ))";
  case regex_constants::error_brace:
    return "error_brace: mismatched brace({ or })";
  case regex_constants::error_badbrace:
    return "error_badbrace: invalid range inside a { }";
  case regex_constants::error_range:
    return "erro_range: invalid character range(e.g., [z-a])";
  case regex_constants::error_space:
    return "error_space: insufficient memory to handle this regular expression";
  case regex_constants::error_badrepeat:
    return "error_badrepeat: a repetition character (*, ?, +, or {) was not "
           "preceded by a valid regular expression";
  case regex_constants::error_complexity:
    return "error_complexity: the requested match is too complex";
  case regex_constants::error_stack:
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
                                          std::string pattern) {

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
        matches.push_back(ent->d_name);
      }
    }
  }
  return matches;
}

// If CombiningCombinedCAFs = true, then take the POT for the input file from
// histograms produced by a previous invocation of this script.
void OffAxisNDCAFCombiner(
    std::string InputFilePattern, std::string OutputFileName,
    bool CombiningCombinedCAFs = false, std::string cafTreeName = "cafTree",
    size_t NMaxEvents = std::numeric_limits<size_t>::max()) {

  size_t asterisk_loc = InputFilePattern.find_first_of('*');
  size_t last_slash_loc = InputFilePattern.find_last_of('/');

  if ((asterisk_loc != std::string::npos) &&
      (last_slash_loc != std::string::npos) &&
      (asterisk_loc < last_slash_loc)) {

    std::cout << "[ERROR]: Can only wildcard filenames, not directories."
              << std::endl;
    return;
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

  if (asterisk_loc == std::string::npos) {
    CAFs.push_back(pattern);
  } else {
    if (NMaxEvents != std::numeric_limits<size_t>::max()) {
      std::cout << "Set NMaxEvents but found a wildcard, this will not do what "
                   "you want, aborting."
                << std::endl;
      abort();
    }
    try {
      CAFs = GetMatchingFiles(dir, pattern);
    } catch (std::regex_error const &e) {
      std::cout << "[ERROR]: " << e.what() << ", " << parseCode(e.code())
                << std::endl;
      throw;
    }
  }

  double min_m = -4;
  double max_m = 40;
  double step_m = 0.5; // 50 cm
  size_t NStep = (max_m - min_m) / step_m;

  TH1D *POTExposure =
      new TH1D("POTExposure", ";OffAxisPosition (m);Exposure (POT)", NStep,
               min_m - (step_m / 2.0), max_m - (step_m / 2.0));
  POTExposure->SetDirectory(nullptr);
  TH1D *FileExposure =
      new TH1D("FileExposure", ";OffAxisPosition (m);Exposure (NFiles)", NStep,
               min_m - (step_m / 2.0), max_m - (step_m / 2.0));
  FileExposure->SetDirectory(nullptr);

  TChain *caf = new TChain(cafTreeName.c_str());
  TChain *meta = new TChain("meta");

  for (std::string file_name : CAFs) {

    std::cout << "[INFO]: Opening file: " << file_name << std::endl;

    TFile f((dir + file_name).c_str());

    assert(!f.IsZombie());

    TTree *f_caf;
    f.GetObject(cafTreeName.c_str(), f_caf);
    assert(f_caf);

    // Assume input file was generated with a single stop.
    double det_x;
    f_caf->SetBranchAddress("det_x", &det_x);
    f_caf->GetEntry(0);

    if (CombiningCombinedCAFs) {
      TH1D *f_POTExposure;
      f.GetObject("POTExposure", f_POTExposure);
      assert(f_POTExposure);
      POTExposure->Add(f_POTExposure);
      TH1D *f_FileExposure;
      f.GetObject("FileExposure", f_FileExposure);
      assert(f_FileExposure);
      FileExposure->Add(f_FileExposure);
    } else {
      TTree *f_meta;
      f.GetObject("meta", f_meta);

      assert(f_meta);

      double file_pot = 0;
      double pot;
      f_meta->SetBranchAddress("pot", &pot);

      size_t nmeta_ents = f_meta->GetEntries();
      for (size_t meta_it = 0; meta_it < nmeta_ents; ++meta_it) {
        f_meta->GetEntry(meta_it);
        file_pot += pot;
      }

      if (NMaxEvents != std::numeric_limits<size_t>::max()) {
        double nevs = std::min(Long64_t(NMaxEvents), f_caf->GetEntries());
        file_pot *= nevs / double(f_caf->GetEntries());
        std::cout << "Rescaling POT by: " << nevs / double(f_caf->GetEntries())
                  << " as only taking " << nevs << "/" << f_caf->GetEntries()
                  << " file entries." << std::endl;
      }

      std::cout << "[INFO]: Found ND file with detector at " << det_x
                << " m off axis which contained " << file_pot << " POT from "
                << nmeta_ents << " files." << std::endl;

      double det_min_m = -3;
      double det_max_m = 3;
      double average_step = 1E-6;
      size_t det_steps = (det_max_m - det_min_m) / (step_m * average_step);

      for (size_t pos_it = 0; pos_it < det_steps; ++pos_it) {
        double det_x_pos_m = det_min_m + pos_it * step_m;

        if (!ana::IsInNDFV(det_x_pos_m * 1E2, /*Dummy y_pos_m*/ 0,
                           /*Dummy z_pos_m*/ 150)) {
          continue;
        }

        POTExposure->Fill(det_x_pos_m + det_x, average_step * file_pot);
        FileExposure->Fill(det_x_pos_m + det_x, average_step * nmeta_ents);
      }
    }

    f.Close();

    caf->Add((dir + file_name).c_str());
    meta->Add((dir + file_name).c_str());
  }

  TFile *fout = TFile::Open(OutputFileName.c_str(), "RECREATE");
  std::cout << "[INFO]: Copying caf tree..." << std::endl;
  TTree *treecopy = caf->CloneTree(NMaxEvents, "fast");
  treecopy->SetName("cafTree");
  delete caf;
  std::cout << "[INFO]: Copying meta tree..." << std::endl;
  TTree *metacopy = meta->CloneTree(-1, "fast");
  delete meta;
  POTExposure->Write("POTExposure");
  FileExposure->Write("FileExposure");
  fout->Write();
}
