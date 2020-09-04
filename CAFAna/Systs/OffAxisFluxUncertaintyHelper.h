#pragma once
#include "TH1.h"

#include <limits>
#include <memory>
#include <string>
#include <vector>

namespace ana {

class OffAxisFluxUncertaintyHelper {
public:
  static int const kND_numu_numode = 0;
  static int const kND_nue_numode = 1;
  static int const kND_numubar_numode = 2;
  static int const kND_nuebar_numode = 3;

  static int const kND_numu_nubarmode = 4;
  static int const kND_nue_nubarmode = 5;
  static int const kND_numubar_nubarmode = 6;
  static int const kND_nuebar_nubarmode = 7;

  static int const kND_SpecHCRun_numu_numode = 8;
  static int const kND_SpecHCRun_nue_numode = 9;
  static int const kND_SpecHCRun_numubar_numode = 10;
  static int const kND_SpecHCRun_nuebar_numode = 11;

  static int const kND_SpecHCRun_numu_nubarmode = 12;
  static int const kND_SpecHCRun_nue_nubarmode = 13;
  static int const kND_SpecHCRun_numubar_nubarmode = 14;
  static int const kND_SpecHCRun_nuebar_nubarmode = 15;

  static int const kFD_numu_numode = 16;
  static int const kFD_nue_numode = 17;
  static int const kFD_numubar_numode = 18;
  static int const kFD_nuebar_numode = 19;

  static int const kFD_numu_nubarmode = 20;
  static int const kFD_nue_nubarmode = 21;
  static int const kFD_numubar_nubarmode = 22;
  static int const kFD_nuebar_nubarmode = 23;

  static int const kUnhandled = 24;

  static int const kInvalidBin = std::numeric_limits<int>::max();

private:
  static int const kOneD = 0;
  static int const kTwoD = 1;
  static int const kTwoDJagged = 2;

  int fNDIs2D;
  int fNDSpecHCRunIs2D;
  std::vector<std::vector<TH1 *>> NDTweaks;
  std::vector<std::vector<TH1 *>> NDSpecHCRunTweaks;
  std::vector<std::vector<TH1 *>> FDTweaks;

  void Initialize(std::string const &filename);

public:
  static OffAxisFluxUncertaintyHelper const &Get();

  size_t GetNParameters() { return NDTweaks.size(); }
  size_t GetNEnuBins(int nu_pdg, double off_axis_pos_m = 0, bool IsND = true,
                     bool IsNuMode = true, bool isSpecHCRun = false);

  int GetNuConfig(int nu_pdg, bool IsND = true, bool IsNuMode = true,
                  bool isSpecHCRun = false) const;

  // This form checks that the neutrino is within weighting bin ranges and
  // returns kUnhandled if not
  int GetNuConfig_checked(int nu_pdg, double enu_GeV, double off_axis_pos_m,
                          size_t param_id = 0, bool IsND = true,
                          bool IsNuMode = true, bool isSpecHCRun = false) const;

  int GetBin(int nu_pdg, double enu_GeV, double off_axis_pos_m,
             size_t param_id = 0, bool IsND = true, bool IsNuMode = true,
             bool isSpecHCRun = false) const;

  int GetFirstEnuBin(int nu_pdg, bool IsND = true, bool IsNuMode = true,
                     bool isSpecHCRun = false);

  // This method presumes that the matrix is ordered as:
  // ND,nu-mode: {numu,numubar,nue,nuebar}
  // ND,nubar-mode: {numu,numubar,nue,nuebar}
  // FD,nu-mode: {numu,numubar,nue,nuebar}
  // FD,nubar-mode: {numu,numubar,nue,nuebar}
  std::vector<size_t> fBinOffsets;
  int GetMatrixBin(int nu_config, double enu_GeV, double off_axis_pos_m = 0);

  // This form does no checking other than for kUnhandled
  double GetFluxWeight(size_t param_id, double param_val, int bin,
                       int nu_config) const;

  double GetFluxWeight(size_t param_id, double param_val, double enu_GeV,
                       double off_axis_pos_m, int nu_pdg, bool IsND,
                       bool IsNuMode, bool isSpecHCRun = false) const;

  ~OffAxisFluxUncertaintyHelper();
};

} // namespace ana