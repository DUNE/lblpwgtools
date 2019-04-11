#include "CAFAna/Core/Progress.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

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
    bool preSelect = false,
    size_t NMaxEvents = std::numeric_limits<size_t>::max()) {

  if (CombiningCombinedCAFs && preSelect) {
    std::cout << "[ERROR]: Cannot run event preselection when combining CAFs."
              << std::endl;
  }

  size_t asterisk_loc = InputFilePattern.find_first_of('*');
  size_t class_loc = InputFilePattern.find_first_of('[');
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

  if ((asterisk_loc == std::string::npos) && (class_loc == std::string::npos)) {
    CAFs.push_back(pattern);
  } else {
    if (NMaxEvents != std::numeric_limits<size_t>::max()) {
      std::cout
          << "Set NMaxEvents but found a regex pattern, this will not do what "
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

  if (!CAFs.size()) {
    std::cout << "[WARN]: Found no matching files." << std::endl;
    return;
  }

  double min_m = -4;
  double max_m = 40;
  double step_m = 0.25; // 25 cm
  double offset_m = 0;  // (step_m / 2.0)
  size_t NStep = (max_m - min_m) / step_m;

  TH1D *POTExposure =
      new TH1D("POTExposure", ";OffAxisPosition (m);Exposure (POT)", NStep,
               min_m - offset_m, max_m - offset_m);
  POTExposure->SetDirectory(nullptr);
  TH2D *POTExposure_stop =
      new TH2D("POTExposure_stop", ";OffAxisPosition (m);Exposure (POT)", NStep,
               min_m - offset_m, max_m - offset_m, 10, -2, 38);
  POTExposure_stop->SetDirectory(nullptr);
  TH1D *FileExposure =
      new TH1D("FileExposure", ";OffAxisPosition (m);Exposure (NFiles)", NStep,
               min_m - offset_m, max_m - offset_m);
  FileExposure->SetDirectory(nullptr);

  TChain *caf = new TChain(cafTreeName.c_str());
  TChain *meta = new TChain("meta");
  TTree *POTWeightFriend;
  double perPOT;

  if (CombiningCombinedCAFs) {
    POTWeightFriend = new TChain("POTWeightFriend");
  } else {
    POTWeightFriend = new TTree("POTWeightFriend", "");
    POTWeightFriend->Branch("perPOT", &perPOT, "perPOT/D");
    POTWeightFriend->SetDirectory(nullptr);
  }

  for (size_t fctr = 0; fctr < CAFs.size(); ++fctr) {

    std::string const &file_name = CAFs[fctr];

    std::cout << "[INFO]: Opening file: " << file_name << std::endl;

    TFile f((dir + file_name).c_str());

    assert(!f.IsZombie());

    TTree *f_caf;
    f.GetObject(cafTreeName.c_str(), f_caf);
    assert(f_caf);

    // Assume input file was generated with a single stop.
    double det_x;
    f_caf->SetBranchAddress("det_x", &det_x);
    double vtx_x, vtx_y, vtx_z;
    f_caf->SetBranchAddress("vtx_x", &vtx_x);
    f_caf->SetBranchAddress("vtx_y", &vtx_y);
    f_caf->SetBranchAddress("vtx_z", &vtx_z);
    f_caf->GetEntry(0);

    if (CombiningCombinedCAFs) {
      TH1D *f_POTExposure;
      f.GetObject("POTExposure", f_POTExposure);
      assert(f_POTExposure);
      POTExposure->Add(f_POTExposure);

      TH2D *f_POTExposure_stop;
      f.GetObject("POTExposure_stop", f_POTExposure_stop);
      assert(f_POTExposure_stop);
      POTExposure_stop->Add(f_POTExposure_stop);

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
        double nevs = std::min(NMaxEvents, size_t(f_caf->GetEntries()));
        file_pot *= nevs / double(f_caf->GetEntries());
        std::cout << "Rescaling POT by: " << nevs / double(f_caf->GetEntries())
                  << " as only taking " << nevs << "/" << f_caf->GetEntries()
                  << " file entries." << std::endl;
      }

      std::cout << "[INFO]: Found ND file (" << (fctr + 1) << "/" << CAFs.size()
                << ") with detector at " << det_x
                << " m off axis which contained " << file_pot << " POT and "
                << f_caf->GetEntries() << " events from " << nmeta_ents
                << " files." << std::endl;

      double det_min_m = -3;
      double det_max_m = 3;
      double average_step = 1E-6;
      size_t det_steps = (det_max_m - det_min_m) / (step_m * average_step);

      for (size_t pos_it = 0; pos_it < det_steps; ++pos_it) {
        double det_x_pos_m = det_min_m + pos_it * (step_m * average_step);

        if (!ana::IsInNDFV(det_x_pos_m * 1E2, /*Dummy y_pos_m*/ 0,
                           /*Dummy z_pos_m*/ 150)) {
          // std::cout << "out of FV: " << (det_x_pos_m + det_x) << std::endl;
          continue;
        }

        POTExposure->Fill(det_x_pos_m + det_x, average_step * file_pot);
        POTExposure_stop->Fill(det_x_pos_m + det_x, det_x,
                               average_step * file_pot);
        FileExposure->Fill(det_x_pos_m + det_x, average_step * nmeta_ents);
      }

      if (!CombiningCombinedCAFs) {
        size_t nevs = std::min(NMaxEvents, size_t(f_caf->GetEntries()));
        perPOT = 1.0 / file_pot;
        for (size_t e_it = 0; e_it < nevs; ++e_it) {
          if (preSelect) {
            f_caf->GetEntry(e_it);
            if (!ana::IsInNDFV(vtx_x, vtx_y, vtx_z)) {
              continue;
            }
          }
          POTWeightFriend->Fill();
        }
        std::cout << "POTWeightFriend->GetEntries(): "
                  << POTWeightFriend->GetEntries() << " / " << nevs << " events."
                  << std::endl;
      }
    }

    f.Close();

    caf->Add((dir + file_name).c_str());
    meta->Add((dir + file_name).c_str());
    if (CombiningCombinedCAFs) {
      static_cast<TChain *>(POTWeightFriend)->Add((dir + file_name).c_str());
    }
  }

  TFile *fout = TFile::Open(OutputFileName.c_str(), "RECREATE");
  std::cout << "[INFO]: Copying caf tree..." << std::endl;
  TTree *treecopy =
      caf->CloneTree(preSelect ? 0 : NMaxEvents, preSelect ? "" : "fast");
  treecopy->SetName("cafTree");

  if (preSelect) {
    double vtx_x, vtx_y, vtx_z;
    caf->SetBranchAddress("vtx_x", &vtx_x);
    caf->SetBranchAddress("vtx_y", &vtx_y);
    caf->SetBranchAddress("vtx_z", &vtx_z);
    size_t nents = std::min(NMaxEvents, size_t(caf->GetEntries()));
    ana::Progress preselprog("Copy with selection progress.");
    for (size_t ent_it = 0; ent_it < nents; ++ent_it) {
      if (ent_it && !(ent_it % 10000)) {
        preselprog.SetProgress(double(ent_it) / double(nents));
      }
      caf->GetEntry(ent_it);
      if (!ana::IsInNDFV(vtx_x, vtx_y, vtx_z)) {
        continue;
      }
      treecopy->Fill();
    }
    preselprog.Done();
  }

  delete caf;
  std::cout << "[INFO]: Copying meta tree..." << std::endl;
  TTree *metacopy = meta->CloneTree(-1, "fast");
  delete meta;
  if (CombiningCombinedCAFs) {
    std::cout << "[INFO]: Copying POTWeightFriend tree..." << std::endl;
    TTree *POTWeightFriendcopy = POTWeightFriend->CloneTree(-1, "fast");
    delete POTWeightFriend;
  } else {
    std::cout << "[INFO]: Writing POTWeightFriend tree..." << std::endl;
    POTWeightFriend->SetDirectory(fout);
    POTWeightFriend->Write();
  }

  POTExposure->Write("POTExposure");
  POTExposure_stop->Write("POTExposure_stop");
  FileExposure->Write("FileExposure");

  fout->Write();
}
