#include "CAFAna/Core/Progress.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/PRISM/PRISMUtils.h"

#include "CAFAna/Analysis/common_fit_definitions.h"

#include "string_parsers/from_string.hxx"

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

struct fileSummary {
  fileSummary() : NEvents(0), POT(0), det_x(0), fileName(nullptr) {}
  int NEvents;
  double POT;
  double det_x;
  std::string *fileName;
};

namespace args {
std::vector<std::string> InputFilePatterns;
std::string OutputFileName;

// If args::CombiningCombinedCAFs = true, then take the POT for the input file
// from histograms produced by a previous invocation of this script.

bool CombiningCombinedCAFs = false;
std::string cafTreeName = "cafTree";
bool preSelect = false;
bool justDoSummaryTree = false;
double min_cm = -4;
double max_cm = 40;
double step_cm = 0.25;
bool isFHC = false;
bool IsSpecRun = false;
size_t NMaxEvents = std::numeric_limits<size_t>::max();
size_t NMaxFiles = std::numeric_limits<size_t>::max();
size_t NSkip = 0;
} // namespace args

std::vector<int> SpecRunIds_all = {-293, -280, 280, 293};

void OffAxisNDCAFCombiner() {

  if (args::CombiningCombinedCAFs && args::preSelect) {
    std::cout << "[ERROR]: Cannot run event preselection when combining CAFs."
              << std::endl;
  }

  std::map<std::string, std::vector<std::string>> CAFs;
  size_t NFiles = 0;

  for (std::string const &InputFilePattern : args::InputFilePatterns) {
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

    std::vector<std::string> files;
    if ((asterisk_loc == std::string::npos) &&
        (class_loc == std::string::npos)) {
      files.push_back(pattern);
    } else {
      try {
        files = GetMatchingFiles(dir, pattern);
      } catch (std::regex_error const &e) {
        std::cout << "[ERROR]: " << e.what() << ", " << parseCode(e.code())
                  << std::endl;
        throw;
      }
    }
    std::copy(files.begin(), files.end(), std::back_inserter(CAFs[dir]));
    NFiles += files.size();
    std::cout << "[INFO]: Added " << files.size() << " files, for a total of "
              << NFiles << std::endl;
  }

  if (!NFiles) {
    std::cout << "[WARN]: Found no matching files." << std::endl;
    return;
  }

  double offset_cm = 0; // (args::step_cm / 2.0)
  size_t NStep = (args::max_cm - args::min_cm) / args::step_cm;

  std::map<int, TH1D *> POTExposures;
  std::map<int, TH2D *> POTExposures_stop;
  std::map<int, TH1D *> FileExposures;

  for (int SpecRunID_local : SpecRunIds_all) {
    std::stringstream ss("");
    ss << ((SpecRunID_local < 0) ? "m" : "") << SpecRunID_local;

    POTExposures[SpecRunID_local] =
        new TH1D(("POTExposure_" + ss.str()).c_str(),
                 ";OffAxisPosition (cm);Exposure (POT)", NStep,
                 args::min_cm - offset_cm, args::max_cm - offset_cm);
    POTExposures[SpecRunID_local]->SetDirectory(nullptr);
    POTExposures_stop[SpecRunID_local] =
        new TH2D(("POTExposure_stop_" + ss.str()).c_str(),
                 ";OffAxisPosition (cm);Exposure (POT)", NStep,
                 args::min_cm - offset_cm, args::max_cm - offset_cm, NStep,
                 args::min_cm - offset_cm, args::max_cm - offset_cm);
    POTExposures_stop[SpecRunID_local]->SetDirectory(nullptr);
    FileExposures[SpecRunID_local] =
        new TH1D(("FileExposure_" + ss.str()).c_str(),
                 ";OffAxisPosition (cm);Exposure (NFiles)", NStep,
                 args::min_cm - offset_cm, args::max_cm - offset_cm);
    FileExposures[SpecRunID_local]->SetDirectory(nullptr);
  }

  TChain *caf = new TChain(args::cafTreeName.c_str());
  TChain *meta = new TChain("meta");
  TTree *OffAxisWeightFriend;
  double perPOT, perFile, massCorr, specRunWght = 1;
  int specRunId_file = (args::isFHC ? +1 : -1) * (args::IsSpecRun ? 280 : 293);
  ana::FVMassCorrection SliceMassCorrector;
  TTree *FileSummaryTree;
  fileSummary fs;

  std::map<int, std::map<double, int>> det_x_files;

  size_t const kDetX = 0;
  size_t const kPerPOT = 1;
  size_t const kMassCorr = 2;
  size_t const kSpecialRunWeight = 3;
  size_t const kSpecialRunId = 4;

  std::vector<std::tuple<double, double, double, double, int>>
      EventPOTEventFiles;
  std::vector<size_t> FileBoundaries = {
      0,
  };

  if (args::CombiningCombinedCAFs) {
    OffAxisWeightFriend = new TChain("OffAxisWeightFriend");
    FileSummaryTree = new TChain("FileSummaryTree");
  } else {
    OffAxisWeightFriend = new TTree("OffAxisWeightFriend", "");
    OffAxisWeightFriend->Branch("perPOT", &perPOT, "perPOT/D");
    OffAxisWeightFriend->Branch("perFile", &perFile, "perFile/D");
    OffAxisWeightFriend->Branch("massCorr", &massCorr, "massCorr/D");
    OffAxisWeightFriend->Branch("specRunWght", &specRunWght, "specRunWght/D");
    OffAxisWeightFriend->Branch("specRunId", &specRunId_file, "specRunId/I");

    OffAxisWeightFriend->SetDirectory(nullptr);

    FileSummaryTree = new TTree("FileSummaryTree", "");
    FileSummaryTree->Branch("NEvents", &fs.NEvents, "NEvents/I");
    FileSummaryTree->Branch("POT", &fs.POT, "POT/D");
    FileSummaryTree->Branch("det_x", &fs.det_x, "det_x/D");
    FileSummaryTree->Branch("fileName", &fs.fileName);
    FileSummaryTree->Branch("specRunId", &specRunId_file, "specRunId/I");

    FileSummaryTree->SetDirectory(nullptr);
  }

  bool Flipdetx = false;
  double detx_to_m = 1E-2; // by default det_in cm
  bool loud = false;

  if (getenv("CAFANA_TURBOSE") && (std::atoi(getenv("CAFANA_TURBOSE")) == 1)) {
    loud = true;
  }

  if (getenv("CAFANA_FLIP_DET_X") &&
      (std::atoi(getenv("CAFANA_FLIP_DET_X")) == 1)) {
    Flipdetx = true;
  }

  if (getenv("CAFANA_DET_X_M") && (std::atoi(getenv("CAFANA_DET_X_M")) == 1)) {
    detx_to_m = 1;
  }

  size_t NPrevOffAxisWeightFriendEntries = 0;
  size_t fctr = 0;
  for (auto dir_files : CAFs) {
    std::string dir = dir_files.first;
    for (std::string const &file_name : dir_files.second) {
      fctr++;

      std::cout << "[INFO]: Opening file: " << file_name << "(" << fctr << "/"
                << NFiles << ")" << std::endl;

      TFile f((dir + file_name).c_str());

      if (f.IsZombie()) {
        std::cout << "[WARN]: Failed." << std::endl;
        continue;
      }

      TTree *f_caf;
      f.GetObject(args::cafTreeName.c_str(), f_caf);
      if (!f_caf) {
        std::cout << "[WARN]: Failed to read " << args::cafTreeName
                  << " TTree. " << std::endl;
        continue;
      }

      // Assume input file was generated with a single stop.
      double det_x;
      f_caf->SetBranchAddress("det_x", &det_x);
      double vtx_x, vtx_y, vtx_z;
      f_caf->SetBranchAddress("vtx_x", &vtx_x);
      f_caf->SetBranchAddress("vtx_y", &vtx_y);
      f_caf->SetBranchAddress("vtx_z", &vtx_z);
      int nuPDG;
      double Ev;
      f_caf->SetBranchAddress("nuPDG", &nuPDG);
      f_caf->SetBranchAddress("Ev", &Ev);

      f_caf->GetEntry(0);

      if (Flipdetx) {
        det_x = -det_x;
      }

      double file_det_x = det_x;

      if (args::CombiningCombinedCAFs) {
        for (int SpecRunID_local : SpecRunIds_all) {

          std::stringstream ss("");
          ss << ((SpecRunID_local < 0) ? "m" : "") << SpecRunID_local;

          TH1D *f_POTExposure;
          f.GetObject(("POTExposure_" + ss.str()).c_str(), f_POTExposure);
          assert(f_POTExposure);
          POTExposures[SpecRunID_local]->Add(f_POTExposure);

          TH2D *f_POTExposure_stop;
          f.GetObject(("POTExposure_stop_" + ss.str()).c_str(),
                      f_POTExposure_stop);
          assert(f_POTExposure_stop);
          POTExposures_stop[SpecRunID_local]->Add(f_POTExposure_stop);

          TH1D *f_FileExposure;
          f.GetObject(("FileExposure_" + ss.str()).c_str(), f_FileExposure);
          assert(f_FileExposure);
          FileExposures[SpecRunID_local]->Add(f_FileExposure);
        }
      } else {
        TTree *f_meta;
        f.GetObject("meta", f_meta);

        if (!f_meta) {
          std::cout << "[ERROR]: Failed to read " << args::cafTreeName
                    << " TTree. " << std::endl;
          abort();
        }

        double file_pot = 0;
        double pot;
        f_meta->SetBranchAddress("pot", &pot);

        size_t nmeta_ents = f_meta->GetEntries();
        for (size_t meta_it = 0; meta_it < nmeta_ents; ++meta_it) {
          f_meta->GetEntry(meta_it);
          file_pot += pot;
        }

        fs.NEvents = f_caf->GetEntries();
        fs.POT = file_pot;
        fs.det_x = file_det_x;
        (*fs.fileName) = file_name;
        FileSummaryTree->Fill();

        if (args::justDoSummaryTree) {
          f.Close();
          continue;
        }

        if (args::NSkip >= size_t(f_caf->GetEntries())) {
          std::cout << "[ERROR]: Requested a skip of " << args::NSkip
                    << " but input file only has" << f_caf->GetEntries()
                    << " entries." << std::endl;
          abort();
        }

        if (args::NSkip) {
          std::cout << "\tSkipping the first " << args::NSkip << " / "
                    << f_caf->GetEntries() << " entries." << std::endl;
        }

        FileBoundaries.push_back(FileBoundaries.back() + f_caf->GetEntries());

        if (loud) {
          std::cout << "File: " << (FileBoundaries.size() - 1) << " up to "
                    << FileBoundaries.back() << " with " << f_caf->GetEntries()
                    << " events." << std::endl;
        }

        size_t nents_after_skip = f_caf->GetEntries() - args::NSkip;
        if (args::NMaxEvents != std::numeric_limits<size_t>::max() ||
            bool(args::NSkip)) {
          double nevs = std::min(args::NMaxEvents, nents_after_skip);
          file_pot *= nevs / double(f_caf->GetEntries());
          std::cout << "Rescaling POT by: "
                    << nevs / double(f_caf->GetEntries()) << " as only taking "
                    << nevs << "/" << f_caf->GetEntries() << " file entries."
                    << std::endl;
        }

        std::cout << "[INFO]: Found ND file with detector at " << det_x
                  << " m off axis which contained " << file_pot << " POT and "
                  << f_caf->GetEntries() << " events from " << nmeta_ents
                  << " files." << std::endl;

        if (!det_x_files[specRunId_file].count(file_det_x)) {
          det_x_files[specRunId_file][file_det_x] = 0;
        }
        det_x_files[specRunId_file][file_det_x]++;

        double vtx_min_cm = -200;
        double vtx_max_cm = 200;
        size_t vtx_steps = (vtx_max_cm - vtx_min_cm) / (args::step_cm);

        for (size_t pos_it = 0; pos_it < vtx_steps; ++pos_it) {
          double vtx_x_pos_cm = vtx_min_cm + (pos_it + 0.5) * (args::step_cm);

          // checking the FV results in double-correcting for the mass
          // differences in different slices.
          // if (!ana::IsInNDFV(vtx_x_pos_cm, /*Dummy y_pos_m*/ 0,
          //                    /*Dummy z_pos_m*/ 150)) {
          //   // std::cout << "out of FV: " << (det_x_pos_m + (det_x *
          //   detx_to_m))
          //   // << std::endl;
          //   continue;
          // }

          FileExposures[specRunId_file]->Fill(
              vtx_x_pos_cm + (det_x * detx_to_m * 1E2), nmeta_ents);

          POTExposures[specRunId_file]->Fill(
              vtx_x_pos_cm + (det_x * detx_to_m * 1E2), file_pot);
          POTExposures_stop[specRunId_file]->Fill(
              vtx_x_pos_cm + (det_x * detx_to_m * 1E2),
              (det_x * detx_to_m * 1E2), file_pot);
        }

        NPrevOffAxisWeightFriendEntries = EventPOTEventFiles.size();
        size_t nevs = std::min(args::NMaxEvents, nents_after_skip);
        perPOT = 1.0 / file_pot;
        for (size_t e_it = 0; e_it < nevs; ++e_it) {
          if (args::preSelect) {
            f_caf->GetEntry(e_it + args::NSkip);
            if (Flipdetx) {
              det_x = -det_x;
            }
            if (file_det_x != det_x) {
              std::cout << "[ERROR]: In file " << file_name
                        << " found an event with det_x = " << det_x
                        << ", but the first event had det_x = " << file_det_x
                        << std::endl;
              throw;
            }
            if (!ana::IsInNDFV(vtx_x, vtx_y, vtx_z)) {
              continue;
            }
          }

          bool isRightSignNumu = (args::isFHC && (nuPDG == 14)) ||
                                 (!args::isFHC && (nuPDG == -14));
          if (args::IsSpecRun && (det_x == 0) && isRightSignNumu) {
            specRunWght = PRISM::Get280kAWeight_numu(Ev, args::isFHC);
          } else {
            specRunWght = 1;
          }

          EventPOTEventFiles.emplace_back(det_x, perPOT,
                                          SliceMassCorrector.GetWeight(vtx_x),
                                          specRunWght, specRunId_file);
        }
        if (args::preSelect) {
          std::cout << "\t-FV selection : NInFile: "
                    << double(EventPOTEventFiles.size() -
                              NPrevOffAxisWeightFriendEntries)
                    << ", NInFile: " << nevs << std::endl;
          std::cout << "\t-FV selection efficiency: "
                    << (double(EventPOTEventFiles.size() -
                               NPrevOffAxisWeightFriendEntries) /
                        double(nevs))
                    << std::endl;
        }
      }

      f.Close();

      caf->Add((dir + file_name).c_str());
      meta->Add((dir + file_name).c_str());
      if (args::CombiningCombinedCAFs) {
        static_cast<TChain *>(OffAxisWeightFriend)
            ->Add((dir + file_name).c_str());
        static_cast<TChain *>(FileSummaryTree)->Add((dir + file_name).c_str());
      }

      if (fctr >= args::NMaxFiles) {
        std::cout << "[INFO]: Only processing " << args::NMaxFiles << " files."
                  << std::endl;
        break;
      }
    }
    if (fctr >= args::NMaxFiles) {
      break;
    }
  }

  TFile *fout =
      TFile::Open(args::justDoSummaryTree ? "CAFFileSummaryTree.root"
                                          : args::OutputFileName.c_str(),
                  "RECREATE");

  if (args::CombiningCombinedCAFs) {
    std::cout << "[INFO]: Rebuilding File count at each stop..." << std::endl;

    double det_x;
    int SpecRunId_read;
    FileSummaryTree->SetBranchAddress("det_x", &det_x);
    FileSummaryTree->SetBranchAddress("specRunId", &SpecRunId_read);
    Long64_t NFSEnts = FileSummaryTree->GetEntries();
    for (Long64_t ent_it = 0; ent_it < NFSEnts; ++ent_it) {
      FileSummaryTree->GetEntry(ent_it);

      if (!det_x_files[SpecRunId_read].count(det_x)) {
        det_x_files[SpecRunId_read][det_x] = 0;
      }
      det_x_files[SpecRunId_read][det_x]++;
    }
    std::cout << "[INFO]: Copying FileSummaryTree tree..." << std::endl;
    TTree *FileSummaryTreecopy = FileSummaryTree->CloneTree(-1, "fast");
    delete FileSummaryTree;
  } else {
    std::cout << "[INFO]: Writing FileSummaryTree tree..." << std::endl;
    FileSummaryTree->SetDirectory(fout);
    FileSummaryTree->Write();
  }

  if (!args::justDoSummaryTree) {
    std::cout << "[INFO]: Copying caf tree..." << std::endl;

    double vtx_x, vtx_y, vtx_z, det_x;
    caf->SetBranchAddress("vtx_x", &vtx_x);
    caf->SetBranchAddress("vtx_y", &vtx_y);
    caf->SetBranchAddress("vtx_z", &vtx_z);
    caf->SetBranchAddress("det_x", &det_x);
    TTree *treecopy = nullptr;

    bool cannotfast =
        (args::NSkip) || (Flipdetx) ||
        (args::NMaxEvents != std::numeric_limits<size_t>::max()) ||
        args::preSelect;

    if (cannotfast) {
      treecopy = caf->CloneTree(0, "");
      treecopy->SetName("cafTree");

      size_t nents = caf->GetEntries();
      ana::Progress preselprog("Copy with selection progress.");

      size_t file_it = 0;
      for (size_t ent_it = 0; ent_it < nents; ++ent_it) {
        if (ent_it && !(ent_it % 10000)) {
          preselprog.SetProgress(double(ent_it) / double(nents));
        }

        // Into the next file, start skipping if needs be
        if (ent_it == FileBoundaries[file_it + 1]) {
          file_it++;

          if (loud) {
            std::cout << "--At entry: " << ent_it
                      << " which corresponds to the start of file: " << file_it
                      << "." << std::endl;
          }
        }

        if ((ent_it - FileBoundaries[file_it]) <
            args::NSkip) { // Skip the same ones that we skipped when building
                           // the friend tree
          continue;
        }

        if ((ent_it - FileBoundaries[file_it]) >=
            args::NMaxEvents) { // Skip the same ones that we skipped when
                                // building
                                // the friend tree
          continue;
        }

        caf->GetEntry(ent_it);
        if (Flipdetx) {
          det_x = -det_x;
        }
        if (args::preSelect && !ana::IsInNDFV(vtx_x, vtx_y, vtx_z)) {
          continue;
        }
        treecopy->Fill();
      }
      preselprog.Done();
    } else {
      treecopy = caf->CloneTree(-1, "fast");
    }

    delete caf;

    std::cout << "[INFO]: Copying meta tree..." << std::endl;
    TTree *metacopy = meta->CloneTree(-1, "fast");
    delete meta;

    if (!args::CombiningCombinedCAFs) {
      std::cout << "[INFO]: Writing OffAxisWeightFriend tree..." << std::endl;
      size_t NOffAxisWeightFriends = EventPOTEventFiles.size();
      ana::Progress potfillprog("Writing OffAxisWeightFriend tree.");
      for (size_t ev_it = 0; ev_it < NOffAxisWeightFriends; ++ev_it) {
        perPOT = std::get<kPerPOT>(EventPOTEventFiles[ev_it]);
        massCorr = std::get<kMassCorr>(EventPOTEventFiles[ev_it]);
        specRunWght = std::get<kSpecialRunWeight>(EventPOTEventFiles[ev_it]);
        specRunId_file = std::get<kSpecialRunId>(EventPOTEventFiles[ev_it]);
        perFile = 1.0 / double(det_x_files[specRunId_file][std::get<kDetX>(
                            EventPOTEventFiles[ev_it])]);

        OffAxisWeightFriend->Fill();
        if (ev_it && !(ev_it % 10000)) {
          potfillprog.SetProgress(double(ev_it) /
                                  double(NOffAxisWeightFriends));
        }
      }
      potfillprog.Done();
      OffAxisWeightFriend->SetDirectory(fout);
      OffAxisWeightFriend->Write();
    } else {
      std::cout
          << "[INFO]: Updating OffAxisWeightFriend tree with combined NFiles "
             "per det_x info..."
          << std::endl;
      double perFile;
      double det_x, vtx_x;
      int specRunId_read;
      treecopy->SetBranchAddress("det_x", &det_x);
      treecopy->SetBranchAddress("vtx_x", &vtx_x);
      OffAxisWeightFriend->SetBranchAddress("perFile", &perFile);
      OffAxisWeightFriend->SetBranchAddress("specRunId", &specRunId_read);

      TTree *OffAxisWeightFriendcopy = OffAxisWeightFriend->CloneTree(0, "");

      Long64_t NPOTWeightEntries = OffAxisWeightFriend->GetEntries();

      if (NPOTWeightEntries != treecopy->GetEntries()) {
        std::cout << "[ERROR]: The number of entries in the POT friend tree "
                     "has got out of synch with the event tree. This is bad."
                  << std::endl;
      }
      assert(NPOTWeightEntries == treecopy->GetEntries());

      ana::Progress filerecalcprog("Updating OffAxisWeightFriend tree");
      for (Long64_t ent_it = 0; ent_it < NPOTWeightEntries; ++ent_it) {
        OffAxisWeightFriend->GetEntry(ent_it);
        treecopy->GetEntry(ent_it);

        double nfiles = FileExposures[specRunId_read]->GetBinContent(
            FileExposures[specRunId_read]->FindFixBin(
                (det_x * detx_to_m * 1E2) + vtx_x));

        perFile = 1.0 / nfiles;
        OffAxisWeightFriendcopy->Fill();
        if (ent_it && !(ent_it % 10000)) {
          filerecalcprog.SetProgress(double(ent_it) /
                                     double(NPOTWeightEntries));
        }
      }
      delete OffAxisWeightFriend;
      filerecalcprog.Done();
    }
    for (int SpecRunID_local : SpecRunIds_all) {
      POTExposures[SpecRunID_local]->Write(
          POTExposures[SpecRunID_local]->GetName());
      POTExposures_stop[SpecRunID_local]->Write(
          POTExposures_stop[SpecRunID_local]->GetName());
      FileExposures[SpecRunID_local]->Write(
          FileExposures[SpecRunID_local]->GetName());
    }
  }

  fout->Write();
}

#ifndef __CINT__

bool strToBool(std::string const &str) {
  if (str == "true") {
    return true;
  }
  if (str == "True") {
    return true;
  }
  if (str == "1") {
    return true;
  }
  if (str == "false") {
    return false;
  }
  if (str == "False") {
    return false;
  }
  if (str == "0") {
    return false;
  }

  std::cout << "[ERROR]: Failed to parse " << str << " as bool." << std::endl;
  throw std::runtime_error(str);
}

void SayUsage(char const *argv[]) {
  std::cout << "[USAGE]: " << argv[0]
            << "\t-i|--input-pattern \"</path/to/*file*.root>\" : Add input "
               "pattern, can contain wildcards at the file-level \n"
               "\t                                                (i.e. no "
               "wild-carded directories). Wildcard expressions must\n"
               "\t                                                be quoted to "
               "stop shell expansion. Can add multiple patterns,\n"
               "\t                                                must add at "
               "least one.\n"
            << "\t-o|--output </path/to/write.root>             : Set output "
               "file name.\n"
            << "\t-t|--treename <treename[=cafTree]>            : Set input "
               "tree name [default=cafTree].\n"
            << "\t-p|--apply-preselection                       : Apply true "
               "ND FV selection.\n"
            << "\t-M|--summary-only                             : Only produce "
               "summary tree and not output CAF.\n"
            << "\t-x|--off-axis-range <min,max,step>            : Set the off "
               "axis detector range and minimum step in meters \n"
            << "\t                                                "
               "[default=-4,40,0.25]\n"
            << "\t-f|--nu-mode                                  : Is a "
               "neutrino-mode file\n"
            << "\t-r|--nubar-mode                               : Is a "
               "antineutrino-mode file\n"
            << "\t-s|--skip <n>                                 : Skip the "
               "first <n> events from the input chain.\n"
            << "\t-n|--nmax <n>                                 : Process at "
               "most <n> events from the input chain.\n"
            << "\t-N|--nmaxfiles <n>                            : Process at "
               "most <n> files from the input descriptor.\n"
            << "\t-C|--recombining                              : Set this "
               "flag if this run includes files that have \n"
            << "\t                                                already been "
               "process by this script.\n"
            << "\t--280kA                                       : Weight on "
               "axis events to look like a special horn current \n"
            << "\t                                                run.\n";
}

void handleOpts(int argc, char const *argv[]) {
  int opt = 1;
  while (opt < argc) {
    if (std::string(argv[opt]) == "-?" || std::string(argv[opt]) == "--help") {
      SayUsage(argv);
      exit(0);
    } else if ((std::string(argv[opt]) == "-i") ||
               (std::string(argv[opt]) == "--input-pattern")) {
      args::InputFilePatterns.push_back(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-o") ||
               (std::string(argv[opt]) == "--output")) {
      args::OutputFileName = argv[++opt];
    } else if ((std::string(argv[opt]) == "-t") ||
               (std::string(argv[opt]) == "--treename")) {
      args::cafTreeName = argv[++opt];
    } else if ((std::string(argv[opt]) == "-p") ||
               (std::string(argv[opt]) == "--apply-preselection")) {
      args::preSelect = true;
    } else if ((std::string(argv[opt]) == "-M") ||
               (std::string(argv[opt]) == "--summary-only")) {
      args::justDoSummaryTree = true;
    } else if ((std::string(argv[opt]) == "-x") ||
               (std::string(argv[opt]) == "--off-axis-range")) {

      std::vector<double> range = fhicl::string_parsers::ParseToVect<double>(
          argv[++opt], ",", false, true);
      args::min_cm = range[0];
      args::max_cm = range[1];
      args::step_cm = range[2];

    } else if ((std::string(argv[opt]) == "-f") ||
               (std::string(argv[opt]) == "--nu-mode")) {
      args::isFHC = true;
    } else if ((std::string(argv[opt]) == "-r") ||
               (std::string(argv[opt]) == "--nubar-mode")) {
      args::isFHC = false;
    } else if ((std::string(argv[opt]) == "-s") ||
               (std::string(argv[opt]) == "--skip")) {
      args::NSkip = fhicl::string_parsers::str2T<size_t>(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-n") ||
               (std::string(argv[opt]) == "--nmax")) {
      args::NMaxEvents = fhicl::string_parsers::str2T<size_t>(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-N") ||
               (std::string(argv[opt]) == "--nmaxfiles")) {
      args::NMaxFiles = fhicl::string_parsers::str2T<size_t>(argv[++opt]);
    } else if ((std::string(argv[opt]) == "-C") ||
               (std::string(argv[opt]) == "--recombining")) {
      args::CombiningCombinedCAFs = true;
    } else if (std::string(argv[opt]) == "--280kA") {
      args::IsSpecRun = true;
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

  if (!args::InputFilePatterns.size() || !args::OutputFileName.length()) {
    std::cout
        << "[ERROR]: Must specify at least one -i option, and a -o option."
        << std::endl;
    SayUsage(argv);
    return 1;
  }

  OffAxisNDCAFCombiner();
}
#endif
