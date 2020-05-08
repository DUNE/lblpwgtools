#include "CAFAna/Core/Progress.h"
#include "CAFAna/Cuts/TruthCuts.h"

#include "CAFAna/PRISM/PRISMUtils.h"

#include "CAFAna/Analysis/common_fit_definitions.h"

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

// If CombiningCombinedCAFs = true, then take the POT for the input file from
// histograms produced by a previous invocation of this script.
void OffAxisNDCAFCombiner(
    std::string InputFilePatterns, std::string OutputFileName,
    bool CombiningCombinedCAFs = false, std::string cafTreeName = "cafTree",
    bool preSelect = false, bool justDoSummaryTree = false, double min_m = -4,
    double max_m = 40, double step_m = 0.25,
    size_t NMaxEvents = std::numeric_limits<size_t>::max()) {

  if (CombiningCombinedCAFs && preSelect) {
    std::cout << "[ERROR]: Cannot run event preselection when combining CAFs."
              << std::endl;
  }

  std::map<std::string, std::vector<std::string>> CAFs;
  size_t NFiles = 0;

  for (std::string const &InputFilePattern :
       SplitString(InputFilePatterns, ',')) {
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
      if (NMaxEvents != std::numeric_limits<size_t>::max()) {
        std::cout << "Set NMaxEvents but found a regex pattern, this will not "
                     "do what "
                     "you want, aborting."
                  << std::endl;
        abort();
      }
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

  double offset_m = 0; // (step_m / 2.0)
  size_t NStep = (max_m - min_m) / step_m;

  TH1D *POTExposure =
      new TH1D("POTExposure", ";OffAxisPosition (m);Exposure (POT)", NStep,
               min_m - offset_m, max_m - offset_m);
  POTExposure->SetDirectory(nullptr);
  TH2D *POTExposure_stop =
      new TH2D("POTExposure_stop", ";OffAxisPosition (m);Exposure (POT)", NStep,
               min_m - offset_m, max_m - offset_m, NStep, min_m - offset_m,
               max_m - offset_m);
  POTExposure_stop->SetDirectory(nullptr);
  TH1D *FileExposure =
      new TH1D("FileExposure", ";OffAxisPosition (m);Exposure (NFiles)", NStep,
               min_m - offset_m, max_m - offset_m);
  FileExposure->SetDirectory(nullptr);

  TChain *caf = new TChain(cafTreeName.c_str());
  TChain *meta = new TChain("meta");
  TTree *OffAxisWeightFriend;
  double perPOT, perFile, massCorr;
  ana::FVMassCorrection SliceMassCorrector;
  TTree *FileSummaryTree;
  fileSummary fs;

  std::map<double, int> det_x_files;

  size_t const kDetX = 0;
  size_t const kPerPOT = 1;
  size_t const kMassCorr = 2;

  std::vector<std::tuple<double, double, double>> EventPOTEventFiles;

  if (CombiningCombinedCAFs) {
    OffAxisWeightFriend = new TChain("OffAxisWeightFriend");
    FileSummaryTree = new TChain("FileSummaryTree");
  } else {
    OffAxisWeightFriend = new TTree("OffAxisWeightFriend", "");
    OffAxisWeightFriend->Branch("perPOT", &perPOT, "perPOT/D");
    OffAxisWeightFriend->Branch("perFile", &perFile, "perFile/D");
    OffAxisWeightFriend->Branch("massCorr", &massCorr, "massCorr/D");
    OffAxisWeightFriend->SetDirectory(nullptr);

    FileSummaryTree = new TTree("FileSummaryTree", "");
    FileSummaryTree->Branch("NEvents", &fs.NEvents, "NEvents/I");
    FileSummaryTree->Branch("POT", &fs.POT, "POT/D");
    FileSummaryTree->Branch("det_x", &fs.det_x, "det_x/D");
    FileSummaryTree->Branch("fileName", &fs.fileName);
    FileSummaryTree->SetDirectory(nullptr);
  }

  bool Flipdetx = false;

  if (getenv("CAFANA_FLIP_DET_X") &&
      (std::atoi(getenv("CAFANA_FLIP_DET_X")) == 1)) {
    Flipdetx = true;
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
      f.GetObject(cafTreeName.c_str(), f_caf);
      if (!f_caf) {
        std::cout << "[WARN]: Failed to read " << cafTreeName << " TTree. "
                  << std::endl;
        continue;
      }

      // Assume input file was generated with a single stop.
      double det_x;
      f_caf->SetBranchAddress("det_x", &det_x);
      double vtx_x, vtx_y, vtx_z;
      f_caf->SetBranchAddress("vtx_x", &vtx_x);
      f_caf->SetBranchAddress("vtx_y", &vtx_y);
      f_caf->SetBranchAddress("vtx_z", &vtx_z);
      f_caf->GetEntry(0);

      if (Flipdetx) {
        det_x = -det_x;
      }

      double file_det_x = det_x;

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

        if (!f_meta) {
          std::cout << "[ERROR]: Failed to read " << cafTreeName << " TTree. "
                    << std::endl;
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

        if (justDoSummaryTree) {
          if (CombiningCombinedCAFs) {
            static_cast<TChain *>(FileSummaryTree)
                ->Add((dir + file_name).c_str());
          }
          f.Close();
          continue;
        }

        if (NMaxEvents != std::numeric_limits<size_t>::max()) {
          double nevs = std::min(NMaxEvents, size_t(f_caf->GetEntries()));
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

        if (!det_x_files.count(file_det_x)) {
          det_x_files[file_det_x] = 0;
        }
        det_x_files[file_det_x]++;

        double vtx_min_m = -3;
        double vtx_max_m = 3;
        double average_step = 1E-6;
        size_t vtx_steps = (vtx_max_m - vtx_min_m) / (step_m * average_step);

        for (size_t pos_it = 0; pos_it < vtx_steps; ++pos_it) {
          double vtx_x_pos_m = vtx_min_m + pos_it * (step_m * average_step);

          FileExposure->Fill(vtx_x_pos_m + det_x, average_step * nmeta_ents);

          if (!ana::IsInNDFV(vtx_x_pos_m * 1E2, /*Dummy y_pos_m*/ 0,
                             /*Dummy z_pos_m*/ 150)) {
            // std::cout << "out of FV: " << (det_x_pos_m + det_x) << std::endl;
            continue;
          }

          POTExposure->Fill(vtx_x_pos_m + det_x, average_step * file_pot);
          POTExposure_stop->Fill(vtx_x_pos_m + det_x, det_x,
                                 average_step * file_pot);
        }

        if (!CombiningCombinedCAFs) {
          NPrevOffAxisWeightFriendEntries = EventPOTEventFiles.size();
          size_t nevs = std::min(NMaxEvents, size_t(f_caf->GetEntries()));
          perPOT = 1.0 / file_pot;
          for (size_t e_it = 0; e_it < nevs; ++e_it) {
            if (preSelect) {
              f_caf->GetEntry(e_it);
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
            EventPOTEventFiles.emplace_back(
                det_x, perPOT, SliceMassCorrector.GetWeight(vtx_x));
          }
          if (preSelect) {
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
      }

      f.Close();

      caf->Add((dir + file_name).c_str());
      meta->Add((dir + file_name).c_str());
      if (CombiningCombinedCAFs) {
        static_cast<TChain *>(OffAxisWeightFriend)
            ->Add((dir + file_name).c_str());
        static_cast<TChain *>(FileSummaryTree)->Add((dir + file_name).c_str());
      }
    }
  }

  TFile *fout = TFile::Open(justDoSummaryTree ? "CAFFileSummaryTree.root"
                                              : OutputFileName.c_str(),
                            "RECREATE");

  if (CombiningCombinedCAFs) {
    std::cout << "[INFO]: Rebuilding File count at each stop..." << std::endl;

    double det_x;
    FileSummaryTree->SetBranchAddress("det_x", &det_x);
    Long64_t NFSEnts = FileSummaryTree->GetEntries();
    for (Long64_t ent_it = 0; ent_it < NFSEnts; ++ent_it) {
      FileSummaryTree->GetEntry(ent_it);

      if (!det_x_files.count(det_x)) {
        det_x_files[det_x] = 0;
      }
      det_x_files[det_x]++;
    }
    std::cout << "[INFO]: Copying FileSummaryTree tree..." << std::endl;
    TTree *FileSummaryTreecopy = FileSummaryTree->CloneTree(-1, "fast");
    delete FileSummaryTree;
  } else {
    std::cout << "[INFO]: Writing FileSummaryTree tree..." << std::endl;
    FileSummaryTree->SetDirectory(fout);
    FileSummaryTree->Write();
  }

  if (!justDoSummaryTree) {
    std::cout << "[INFO]: Copying caf tree..." << std::endl;
    bool canfast = !preSelect && !Flipdetx;
    TTree *treecopy = nullptr;

    if (!canfast) {
      double vtx_x, vtx_y, vtx_z, det_x;
      caf->SetBranchAddress("vtx_x", &vtx_x);
      caf->SetBranchAddress("vtx_y", &vtx_y);
      caf->SetBranchAddress("vtx_z", &vtx_z);
      caf->SetBranchAddress("det_x", &det_x);
      treecopy = caf->CloneTree(0, "");
      treecopy->SetName("cafTree");

      size_t nents = std::min(NMaxEvents, size_t(caf->GetEntries()));
      ana::Progress preselprog("Copy with selection progress.");
      for (size_t ent_it = 0; ent_it < nents; ++ent_it) {
        if (ent_it && !(ent_it % 10000)) {
          preselprog.SetProgress(double(ent_it) / double(nents));
        }
        caf->GetEntry(ent_it);
        if (Flipdetx) {
          det_x = -det_x;
        }
        if (preSelect && !ana::IsInNDFV(vtx_x, vtx_y, vtx_z)) {
          continue;
        }
        treecopy->Fill();
      }
      preselprog.Done();
    } else {
      treecopy = caf->CloneTree(NMaxEvents, "fast");
      treecopy->SetName("cafTree");
    }
    delete caf;

    std::cout << "[INFO]: Copying meta tree..." << std::endl;
    TTree *metacopy = meta->CloneTree(-1, "fast");
    delete meta;

    if (!CombiningCombinedCAFs) {
      std::cout << "[INFO]: Writing OffAxisWeightFriend tree..." << std::endl;
      size_t NOffAxisWeightFriends = EventPOTEventFiles.size();
      ana::Progress potfillprog("Writing OffAxisWeightFriend tree.");
      for (size_t ev_it = 0; ev_it < NOffAxisWeightFriends; ++ev_it) {
        perFile =
            1.0 /
            double(det_x_files[std::get<kDetX>(EventPOTEventFiles[ev_it])]);
        perPOT = std::get<kPerPOT>(EventPOTEventFiles[ev_it]);
        massCorr = std::get<kMassCorr>(EventPOTEventFiles[ev_it]);
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
      double det_x;
      treecopy->SetBranchAddress("det_x", &det_x);
      OffAxisWeightFriend->SetBranchAddress("perFile", &perFile);
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
        perFile = 1.0 / double(det_x_files[det_x]);
        OffAxisWeightFriendcopy->Fill();
        if (ent_it && !(ent_it % 10000)) {
          filerecalcprog.SetProgress(double(ent_it) /
                                     double(NPOTWeightEntries));
        }
      }
      delete OffAxisWeightFriend;
      filerecalcprog.Done();
    }

    POTExposure->Write("POTExposure");
    POTExposure_stop->Write("POTExposure_stop");
    FileExposure->Write("FileExposure");
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

void HelpText(char const *argv[]) {
  std::cout << "[USAGE]: " << argv[0]
            << " \"<InputFilePattern1>[,<InputFilePattern2>,...]\" "
               "OutputFile.root <Running on the output of this script? false> "
               "<Input CAF Tree Name: cafTree> <Filter events in ND FV? false> "
               "<Just write out a summary tree detailing POT and event rate at "
               "each stop? false> <Maximum number of events>"
            << std::endl;
}

int main(int argc, char const *argv[]) {

  if ((argc == 2) &&
      ((std::string(argv[1]) == "-?") || (std::string(argv[1]) == "--help"))) {
    HelpText(argv);
    return 0;
  }

  if (argc < 3) {
    std::cout << "[ERROR]: Expect at least 2 arguments." << std::endl;
    HelpText(argv);

    return 1;
  }
  std::string InputFilePattern = argv[1];
  std::string OutputFileName = argv[2];
  bool CombiningCombinedCAFs = (argc >= 4) ? strToBool(argv[3]) : false;
  std::string cafTreeName = (argc >= 5) ? argv[4] : "cafTree";
  bool preSelect = (argc >= 6) ? strToBool(argv[5]) : false;
  bool justDoSummaryTree = (argc >= 7) ? strToBool(argv[6]) : false;
  double min_m = (argc >= 8) ? std::atof(argv[7]) : -4;
  double max_m = (argc >= 9) ? std::atof(argv[8]) : 40;
  double step_m = (argc >= 10) ? std::atof(argv[9]) : 0.25;
  size_t NMaxEvents = (argc >= 11) ? size_t(std::strtol(argv[10], nullptr, 10))
                                   : std::numeric_limits<size_t>::max();
  OffAxisNDCAFCombiner(InputFilePattern, OutputFileName, CombiningCombinedCAFs,
                       cafTreeName, preSelect, justDoSummaryTree, min_m, max_m,
                       step_m, NMaxEvents);
}
#endif
