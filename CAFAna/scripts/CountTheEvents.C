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

  // std::cout << "[INFO]: Looking for files matching: \"" << pattern
  //           << "\" in directory: " << directory << std::endl;

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

inline bool IsInNDFV(double pos_x_cm, double pos_y_cm, double pos_z_cm) {
  bool inDeadRegion = false;
  for (int i = -3; i <= 3; ++i) {
    // 0.5cm cathode in the middle of each module, plus 0.5cm buffer
    double cathode_center = i * 102.1;
    if (pos_x_cm > cathode_center - 0.75 && pos_x_cm < cathode_center + 0.75)
      inDeadRegion = true;

    // 1.6cm dead region between modules (0.5cm module wall and 0.3cm pixel
    // plane, x2) don't worry about outer boundary because events are only
    // generated in active Ar + insides
    double module_boundary = i * 102.1 + 51.05;
    if (i <= 2 && pos_x_cm > module_boundary - 1.3 &&
        pos_x_cm < module_boundary + 1.3)
      inDeadRegion = true;
  }
  for (int i = 1; i <= 4; ++i) {
    // module boundaries in z are 1.8cm (0.4cm ArCLight plane + 0.5cm module
    // wall, x2) module is 102.1cm wide, but only 101.8cm long due to cathode
    // (0.5cm) being absent in length but ArCLight is 0.1cm thicker than pixel
    // plane so it's 0.3cm difference positions are off-set by 0.6 because I
    // defined 0 to be the upstream edge based on the active volume by
    // inspecting a plot, and aparently missed by 3 mm, but whatever add 8mm =
    // 2 pad buffer due to worse position resolution in spatial dimension z
    // compared to timing direction x so total FV gap will be 1.8 + 2*0.8
    // = 3.4cm
    double module_boundary = i * 101.8 - 0.6;
    if (pos_z_cm > module_boundary - 1.7 && pos_z_cm < module_boundary + 1.7)
      inDeadRegion = true;
  }

  return (abs(pos_x_cm) < 200 && abs(pos_y_cm) < 100 && pos_z_cm > 50 &&
          pos_z_cm < 350 && !inDeadRegion);
}

void CountTheEvents(std::string InputFilePattern) {

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
  if ((asterisk_loc == std::string::npos) && (class_loc == std::string::npos)) {
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

  TChain caf("caf");
  TChain meta("meta");

  for (auto file : files) {
    // std::cout << "Adding file: " << (dir + file) << std::endl;
    caf.Add((dir + file).c_str());
    meta.Add((dir + file).c_str());
  }

  double POT;
  meta.SetBranchAddress("pot", &POT);
  size_t nmeta_ents = meta.GetEntries();

  double totalPOT = 0;
  for (size_t mit = 0; mit < nmeta_ents; ++mit) {
    meta.GetEntry(mit);
    totalPOT += POT;
  }

  double vtx_x, vtx_y, vtx_z, Ehad_veto, Ev;
  int reco_numu, muon_contained, muon_tracker, reco_q;

  caf.SetBranchAddress("vtx_x", &vtx_x);
  caf.SetBranchAddress("vtx_y", &vtx_y);
  caf.SetBranchAddress("vtx_z", &vtx_z);
  caf.SetBranchAddress("Ehad_veto", &Ehad_veto);

  caf.SetBranchAddress("reco_numu", &reco_numu);
  caf.SetBranchAddress("muon_contained", &muon_contained);
  caf.SetBranchAddress("muon_tracker", &muon_tracker);
  caf.SetBranchAddress("reco_q", &reco_q);

  std::vector<double> cvwgts;
  std::vector<std::string> dialnames = {

"MaCCQE",
        "VecFFCCQEshape",
        "MaNCEL",
        "EtaNCEL",
        "MaCCRES",
        "MvCCRES",
        "MaNCRES",
        "MvNCRES",
        "RDecBR1gamma",
        "RDecBR1eta",
        "Theta_Delta2Npi",
        "AhtBY",
        "BhtBY",
        "CV1uBY",
        "CV2uBY",
        "FormZone",
        "MFP_pi",
        "FrCEx_pi",
        "FrElas_pi",
        "FrInel_pi",
        "FrAbs_pi",
        "FrPiProd_pi",
        "MFP_N",
        "FrCEx_N",
        "FrElas_N",
        "FrInel_N",
        "FrAbs_N",
        "FrPiProd_N",
        "CCQEPauliSupViaKF",
        "Mnv2p2hGaussEnhancement",
        "MKSPP_ReWeight",
        "E2p2h_A_nu",
        "E2p2h_B_nu",
        "E2p2h_A_nubar",
        "E2p2h_B_nubar",
        "NR_nu_n_CC_2Pi",
        "NR_nu_n_CC_3Pi",
        "NR_nu_p_CC_2Pi",
        "NR_nu_p_CC_3Pi",
        "NR_nu_np_CC_1Pi",
        "NR_nu_n_NC_1Pi",
        "NR_nu_n_NC_2Pi",
        "NR_nu_n_NC_3Pi",
        "NR_nu_p_NC_1Pi",
        "NR_nu_p_NC_2Pi",
        "NR_nu_p_NC_3Pi",
        "NR_nubar_n_CC_1Pi",
        "NR_nubar_n_CC_2Pi",
        "NR_nubar_n_CC_3Pi",
        "NR_nubar_p_CC_1Pi",
        "NR_nubar_p_CC_2Pi",
        "NR_nubar_p_CC_3Pi",
        "NR_nubar_n_NC_1Pi",
        "NR_nubar_n_NC_2Pi",
        "NR_nubar_n_NC_3Pi",
        "NR_nubar_p_NC_1Pi",
        "NR_nubar_p_NC_2Pi",
        "NR_nubar_p_NC_3Pi",
        "BeRPA_A",
        "BeRPA_B",
        "BeRPA_D",
        "BeRPA_E",
        "C12ToAr40_2p2hScaling_nu",
        "C12ToAr40_2p2hScaling_nubar",
        "nuenuebar_xsec_ratio",
        "nuenumu_xsec_ratio",
        "SPPLowQ2Suppression",
        "FSILikeEAvailSmearing"
  };

  cvwgts.resize(dialnames.size());

  int i = 0;
  for (auto n : dialnames) {
    caf.SetBranchAddress((n + "_cvwgt").c_str(), &cvwgts[i++]);
  }

  caf.SetBranchAddress("Ev", &Ev);
  size_t ncaf_ents = caf.GetEntries();

  std::vector<double> nevents_w(dialnames.size(), 0);
  double nevents = 0;
  double nevents_aw = 0;

  for (size_t cit = 0; cit < ncaf_ents; ++cit) {
    caf.GetEntry(cit);

    bool ND_FV = IsInNDFV(vtx_x, vtx_y, vtx_z);
    bool pass_ND = (reco_numu && (muon_contained || muon_tracker) &&
                    reco_q == -1 && Ehad_veto < 30);
    if (!ND_FV || !pass_ND) {
      continue;
    }

    nevents++;
    int i = 0;
    double allw = 1;
    for (double w : cvwgts) {
      nevents_w[i++] += w;
      allw *= w;
    }
    nevents_aw += allw;
  }

  std::cout << "NEvents in file: " << ncaf_ents
            << ", NSelected events: " << nevents
            << ", nevents CV weighted: " << nevents_aw << ", POT: " << totalPOT
            << " NEvents for 1/2 beam yr = "
            << (nevents * (1.1E21 / 2.0) / totalPOT)
            << " (CV weighted: " << (nevents_aw * (1.1E21 / 2.0) / totalPOT)
            << " )" << std::endl;

  i = 0;
  for (auto n : dialnames) {
    std::cout << n << " " << (nevents_w[i++] * (1.1E21 / 2.0) / totalPOT)
              << std::endl;
  }
}
