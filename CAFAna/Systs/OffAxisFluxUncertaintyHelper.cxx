#include "CAFAna/Systs/OffAxisFluxUncertaintyHelper.h"

#include "CAFAna/Core/Utilities.h"

#include "TH2Jagged.h"

#include "TAxis.h"
#include "TFile.h"
#include "TH1.h"
#include "TObjString.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace ana;

TFile *CheckOpenFile(std::string const &fname, char const *opts = "") {
  TFile *inpF = new TFile(fname.c_str(), opts);
  if (!inpF || !inpF->IsOpen()) {
    std::cout << "[ERROR]: Couldn't open input file: " << fname << std::endl;
    exit(1);
  }
  return inpF;
}

template <class TH>
inline TH *GetHistogram(TFile *f, std::string const &fhname,
                        bool exit_on_fail = true) {
  TH *inpH = dynamic_cast<TH *>(f->Get(fhname.c_str()));

  if (!inpH) {
    if (exit_on_fail) {
      std::cout << "[ERROR]: Couldn't get TH: " << fhname
                << " from input file: " << f->GetName() << std::endl;
      exit(1);
    } else {
      return nullptr;
    }
  }

  inpH = static_cast<TH *>(inpH->Clone());
  inpH->SetDirectory(nullptr);
  return inpH;
}

template <class TH>
inline TH *GetHistogram(std::string const &fname, std::string const &hname,
                        bool exit_on_fail = true) {
  TDirectory *ogDir = gDirectory;

  TFile *inpF = CheckOpenFile(fname);

  TH *h = GetHistogram<TH>(inpF, hname, exit_on_fail);

  inpF->Close();
  delete inpF;

  if (ogDir) {
    ogDir->cd();
  }

  return h;
}

bool IsFlowBin(TAxis const &ax, Int_t bin_it) {
  return ((bin_it <= 0) || (bin_it >= (ax.GetNbins() + 1)));
}
bool IsFlowBin(TH2 const *h, Int_t xbin_it, Int_t ybin_it) {
  return IsFlowBin(*h->GetXaxis(), xbin_it) ||
         IsFlowBin(*h->GetYaxis(), ybin_it);
}
bool IsFlowBin(TH1 const *h, Int_t bin_it) {
  return IsFlowBin(*h->GetXaxis(), bin_it);
}
bool IsFlowBin(TH2JaggedF const *h, double xval, double yval) {
  int bin = h->FindFixBin(xval, yval);
  return h->IsFlowBin(bin);
}
bool IsFlowBin(TH2 const *h, double xval, double yval) {
  int xbin = h->GetXaxis()->FindFixBin(xval);
  int ybin = h->GetYaxis()->FindFixBin(yval);
  return IsFlowBin(*h->GetXaxis(), xbin) || IsFlowBin(*h->GetYaxis(), ybin);
}
bool IsFlowBin(TH1 const *h, double xval) {
  int xbin = h->GetXaxis()->FindFixBin(xval);
  return IsFlowBin(*h->GetXaxis(), xbin);
}

static OffAxisFluxUncertaintyHelper *globalFluxHelper = nullptr;
OffAxisFluxUncertaintyHelper const &OffAxisFluxUncertaintyHelper::Get() {
  if (!globalFluxHelper) {
    globalFluxHelper = new OffAxisFluxUncertaintyHelper();
    globalFluxHelper->Initialize(ana::FindCAFAnaDir() +
                                 "/Systs/flux_shifts_OffAxis.root");
  }
  return *globalFluxHelper;
}

void OffAxisFluxUncertaintyHelper::Initialize(std::string const &filename) {

  fNDIs2D = -1;
  NDTweaks.clear();
  FDTweaks.clear();

  bool IsCAFAnaFormat = false;

  size_t NParams = 50;
  std::string ND_detector_tag = "ND";
  std::string ND_SpecHCRun_detector_tag = "280kA";
  std::string FD_detector_tag = "FD";
  std::string nu_mode_beam_tag = "nu";
  std::string nubar_mode_beam_tag = "nubar";
  std::string numu_species_tag = "numu";
  std::string nue_species_tag = "nue";
  std::string numubar_species_tag = "numubar";
  std::string nuebar_species_tag = "nuebar";

  std::string input_file = filename;
  std::string input_dir = IsCAFAnaFormat ? "" : "FluxParameters";

  TDirectory *ogDir = gDirectory;

  TFile *inpF = CheckOpenFile(filename);

  TDirectory *d = inpF->GetDirectory(input_dir.c_str());

  std::vector<std::string> param_names;
  TList *param_names_list = nullptr;
  d->GetObject("param_names", param_names_list);
  if (!param_names_list) {
    std::cout << "[ERROR]: Failed to find param_name list in TDirectory: "
              << input_dir << " in file: " << input_file
              << " and was passed no param_names FHiCL list to try to load."
              << std::endl;
    abort();
  }

  for (size_t p_it = 0;
       p_it < std::min(NParams, size_t(param_names_list->GetSize())); ++p_it) {
    param_names.push_back(
        static_cast<TObjString *>(param_names_list->At(p_it))->String().Data());
  }

  inpF->Close();
  delete inpF;

  if (ogDir) {
    ogDir->cd();
  }

  for (size_t p_it = 0; p_it < NParams; ++p_it) {
    NDTweaks.push_back(std::vector<TH1 *>());
    NDSpecHCRunTweaks.push_back(std::vector<TH1 *>());
    FDTweaks.push_back(std::vector<TH1 *>());

    std::stringstream input_dir_i("");
    input_dir_i << input_dir << (input_dir.size() ? "/" : "")
                << param_names[p_it] << "/";

    size_t NHistsLoaded = 0;
    int nucf = kND_numu_numode;

    static std::string const location_tags[] = {
        ND_detector_tag, ND_SpecHCRun_detector_tag, FD_detector_tag};
    static std::string const beam_mode_tags[] = {nu_mode_beam_tag,
                                                 nubar_mode_beam_tag};

    static std::string const species_tags[] = {
        numu_species_tag, nue_species_tag, numubar_species_tag,
        nuebar_species_tag};

    for (size_t lt_it = 0; lt_it < 3; ++lt_it) {
      std::string const &location_tag = location_tags[lt_it];
      for (size_t bm_it = 0; bm_it < 2; ++bm_it) {
        std::string const &beam_mode_tag = beam_mode_tags[bm_it];
        for (size_t sp_it = 0; sp_it < 4; ++sp_it) {
          std::string const &species_tag = species_tags[sp_it];

          NDTweaks.back().push_back(nullptr);
          NDSpecHCRunTweaks.back().push_back(nullptr);
          FDTweaks.back().push_back(nullptr);

          std::string hname =
              (IsCAFAnaFormat ? input_dir_i.str() + location_tag + "_" +
                                    species_tag + "_" + beam_mode_tag
                              : input_dir_i.str() + location_tag + "_" +
                                    beam_mode_tag + "_" + species_tag);

          if (nucf < kND_SpecHCRun_numu_numode) { // Is ND
            NDTweaks.back().back() = GetHistogram<TH1>(input_file, hname, true);

            if ((fNDIs2D == -1) && NDTweaks.back().back()) {
              if (NDTweaks.back().back()->GetDimension() == 1) {
                fNDIs2D = kOneD;
              } else if (NDTweaks.back().back()->GetDimension() == 2) {
                TH2JaggedF *jag =
                    dynamic_cast<TH2JaggedF *>(NDTweaks.back().back());
                fNDIs2D = bool(jag) ? kTwoDJagged : kTwoD;
              }
            }

            NHistsLoaded += bool(NDTweaks.back().back());
          } else if (nucf < kFD_numu_numode) { // ND Spec Run
            // hack becasue I have been inconsistent in whether SpecRuns are
            // considered a different 'detector' or a different 'beam mode'
            std::string hname =
                (IsCAFAnaFormat
                     ? input_dir_i.str() + ND_detector_tag + "_" + species_tag +
                           "_" + ND_SpecHCRun_detector_tag + "_" + beam_mode_tag
                     : input_dir_i.str() + ND_detector_tag + "_" +
                           beam_mode_tag + "_" + ND_SpecHCRun_detector_tag +
                           "_" + species_tag);

            NDSpecHCRunTweaks.back().back() =
                GetHistogram<TH1>(input_file, hname, true);

            if ((fNDSpecHCRunIs2D == -1) && NDSpecHCRunTweaks.back().back()) {
              if (NDSpecHCRunTweaks.back().back()->GetDimension() == 1) {
                fNDSpecHCRunIs2D = kOneD;
              } else if (NDSpecHCRunTweaks.back().back()->GetDimension() == 2) {
                TH2JaggedF *jag =
                    dynamic_cast<TH2JaggedF *>(NDSpecHCRunTweaks.back().back());
                fNDSpecHCRunIs2D = bool(jag) ? kTwoDJagged : kTwoD;
              }
            }

            NHistsLoaded += bool(NDSpecHCRunTweaks.back().back());
          } else { // FD
            FDTweaks.back().back() = GetHistogram<TH1>(input_file, hname, true);

            NHistsLoaded += bool(FDTweaks.back().back());
          }
          nucf += 1;
        }
      }
    }
    // std::cout << "[INFO]: For parameter " << p_it << ", found " <<
    // NHistsLoaded
    //           << " inputs." << std::endl;
    if (!NHistsLoaded) {
      NDTweaks.pop_back();
      NDSpecHCRunTweaks.pop_back();
      FDTweaks.pop_back();
    }
  }
}

size_t OffAxisFluxUncertaintyHelper::GetNEnuBins(int nu_pdg,
                                                   double off_axis_pos_m,
                                                   bool IsND, bool IsNuMode,
                                                   bool isSpecHCRun) {

  int nucf = GetNuConfig(nu_pdg, IsND, IsNuMode, isSpecHCRun);

  if (nucf == kUnhandled) {
    return 0;
  }

  if (nucf < kND_SpecHCRun_numu_numode) {

    if (fNDIs2D == kTwoDJagged) {
      throw "This cannot be used for TH2Jagged inputs";

    } else {
      return NDTweaks[0][nucf]->GetNbinsX();
    }
  } else if (nucf < kFD_numu_numode) {

    if (fNDSpecHCRunIs2D == kTwoDJagged) {
      throw "This cannot be used for TH2Jagged inputs";

    } else {
      return NDSpecHCRunTweaks[0][nucf]->GetNbinsX();
    }
  } else {
    return FDTweaks[0][nucf]->GetNbinsX();
  }
}

int OffAxisFluxUncertaintyHelper::GetNuConfig(int nu_pdg, bool IsND,
                                                bool IsNuMode,
                                                bool isSpecHCRun) const {

  int nucf = kUnhandled;

  switch (nu_pdg) {
  case 14: {
    nucf =
        IsNuMode
            ? (isSpecHCRun ? kND_SpecHCRun_numu_numode : kND_numu_numode)
            : (isSpecHCRun ? kND_SpecHCRun_numu_nubarmode : kND_numu_nubarmode);
    break;
  }
  case -14: {
    nucf = IsNuMode ? (isSpecHCRun ? kND_SpecHCRun_numubar_numode
                                   : kND_numubar_numode)
                    : (isSpecHCRun ? kND_SpecHCRun_numubar_nubarmode
                                   : kND_numubar_nubarmode);
    break;
  }
  case 12: {
    nucf = IsNuMode ? (isSpecHCRun ? kND_SpecHCRun_nue_numode : kND_nue_numode)
                    : (isSpecHCRun ? kND_SpecHCRun_nue_nubarmode
                                   : kND_nue_nubarmode);
    break;
  }
  case -12: {
    nucf = IsNuMode
               ? (isSpecHCRun ? kND_SpecHCRun_nuebar_numode : kND_nuebar_numode)
               : (isSpecHCRun ? kND_SpecHCRun_nuebar_nubarmode
                              : kND_nuebar_nubarmode);
    break;
  }
  }

  return nucf + (IsND ? 0 : 16);
}

int OffAxisFluxUncertaintyHelper::GetNuConfig_checked(
    int nu_pdg, double enu_GeV, double off_axis_pos_m, size_t param_id,
    bool IsND, bool IsNuMode, bool isSpecHCRun) const {
  if (GetBin(nu_pdg, enu_GeV, off_axis_pos_m, param_id, IsND, IsNuMode,
             isSpecHCRun) == kInvalidBin) {
    return kUnhandled;
  }
  return GetNuConfig(nu_pdg, IsND, IsNuMode, isSpecHCRun);
}

int OffAxisFluxUncertaintyHelper::GetBin(int nu_pdg, double enu_GeV,
                                           double off_axis_pos_m,
                                           size_t param_id, bool IsND,
                                           bool IsNuMode,
                                           bool isSpecHCRun) const {
  int nucf = GetNuConfig(nu_pdg, IsND, IsNuMode, isSpecHCRun);

  if (nucf < kND_SpecHCRun_numu_numode) { // Is ND
    if (!NDTweaks[param_id][nucf]) {
      return kInvalidBin;
    } else {
      switch (fNDIs2D) {
      case kOneD: {
        if (IsFlowBin(NDTweaks[param_id][nucf], enu_GeV)) {
          return kInvalidBin;
        }
        return NDTweaks[param_id][nucf]->FindFixBin(enu_GeV);
      }
      case kTwoD: {
        TH2 *h2 = static_cast<TH2 *>(NDTweaks[param_id][nucf]);
        if (IsFlowBin(h2, enu_GeV, off_axis_pos_m)) {
          return kInvalidBin;
        }
        int xbin = (h2->GetXaxis()->FindFixBin(enu_GeV) - 1);
        int ybin = (h2->GetYaxis()->FindFixBin(off_axis_pos_m) - 1);
        return h2->GetBin(xbin, ybin);
      }
      case kTwoDJagged: {
        TH2JaggedF *h2 = static_cast<TH2JaggedF *>(NDTweaks[param_id][nucf]);
        if (IsFlowBin(h2, enu_GeV, off_axis_pos_m)) {
          return kInvalidBin;
        }
        return h2->FindFixBin(enu_GeV, off_axis_pos_m);
      }
      }
    }
  } else if (nucf < kFD_numu_numode) { // Is ND Spec run
    if (!NDSpecHCRunTweaks[param_id][nucf]) {
      return kInvalidBin;
    } else {
      switch (fNDSpecHCRunIs2D) {
      case kOneD: {
        if (IsFlowBin(NDSpecHCRunTweaks[param_id][nucf], enu_GeV)) {
          return kInvalidBin;
        }
        return NDSpecHCRunTweaks[param_id][nucf]->FindFixBin(enu_GeV);
      }
      case kTwoD: {
        TH2 *h2 = static_cast<TH2 *>(NDSpecHCRunTweaks[param_id][nucf]);
        if (IsFlowBin(h2, enu_GeV, off_axis_pos_m)) {
          return kInvalidBin;
        }
        int xbin = (h2->GetXaxis()->FindFixBin(enu_GeV) - 1);
        int ybin = (h2->GetYaxis()->FindFixBin(off_axis_pos_m) - 1);
        return h2->GetBin(xbin, ybin);
      }
      case kTwoDJagged: {
        TH2JaggedF *h2 =
            static_cast<TH2JaggedF *>(NDSpecHCRunTweaks[param_id][nucf]);
        if (IsFlowBin(h2, enu_GeV, off_axis_pos_m)) {
          return kInvalidBin;
        }
        return h2->FindFixBin(enu_GeV, off_axis_pos_m);
      }
      }
    }
  } else {
    if (!FDTweaks[param_id][nucf]) {
      return kInvalidBin;
    } else {
      std::cout << "FDTweaks OKAY" << std::endl;
      std::cout << "FDTweaks Nbins = " << FDTweaks[param_id][nucf]->GetNbinsX() << std::endl;
      if (IsFlowBin(FDTweaks[param_id][nucf], enu_GeV)) {
        return kInvalidBin;
      }
      std::cout << "Passed IsFlowBin(...)" << std::endl;
      return FDTweaks[param_id][nucf]->FindFixBin(enu_GeV);
    }
  }
  throw;
}

int OffAxisFluxUncertaintyHelper::GetFirstEnuBin(int nu_pdg, bool IsND,
                                                   bool IsNuMode,
                                                   bool isSpecHCRun) {
  int nucf = GetNuConfig(nu_pdg, IsND, IsNuMode, isSpecHCRun);

  if (nucf == kUnhandled) {
    return 0;
  }

  if (nucf < kND_SpecHCRun_numu_numode) {

    if (fNDIs2D != kOneD) {
      if (fNDIs2D == kTwoDJagged) {
        throw "This cannot be used for TH2Jagged inputs";
      } else {
        return NDTweaks[0][nucf]->GetBin(1, 1);
      }
    } else {
      return NDTweaks[0][nucf]->GetBin(1);
    }
  } else if (nucf < kFD_numu_numode) {

    if (fNDSpecHCRunIs2D != kOneD) {
      if (fNDSpecHCRunIs2D == kTwoDJagged) {
        throw "This cannot be used for TH2Jagged inputs";
      } else {
        return NDSpecHCRunTweaks[0][nucf]->GetBin(1, 1);
      }
    } else {
      return NDSpecHCRunTweaks[0][nucf]->GetBin(1);
    }
  } else {
    return FDTweaks[0][nucf]->GetBin(1);
  }
}

int OffAxisFluxUncertaintyHelper::GetMatrixBin(int nu_config, double enu_GeV,
                                                 double off_axis_pos_m) {
  if (nu_config == kUnhandled) {
    return -1;
  }

  if (!fBinOffsets.size()) {
    fBinOffsets.push_back(0);
    for (int i = 0; i < kFD_nuebar_nubarmode; ++i) {
      if (i < kND_SpecHCRun_numu_numode) {
        switch (fNDIs2D) {
        case kOneD: {
          fBinOffsets.push_back(fBinOffsets.back() +
                                NDTweaks[0][i]->GetNbinsX());
          // std::cout << "[INFO]: Added " << NDTweaks[0][i]->GetName() << "
          // with "
          //           << NDTweaks[0][i]->GetNbinsX() << " for a total of "
          //           << fBinOffsets.back() << " bins" << std::endl;

          break;
        }
        case kTwoD: {
          TH2 *h2 = static_cast<TH2 *>(NDTweaks[0][i]);
          fBinOffsets.push_back(fBinOffsets.back() +
                                (h2->GetNbinsX() * h2->GetNbinsY()));
          // std::cout << "[INFO]: Added " << h2->GetName() << " with "
          //           << h2->GetNbinsX() << " for a total of "
          //           << (h2->GetNbinsX() * h2->GetNbinsY()) << " bins"
          //           << std::endl;
          break;
        }
        case kTwoDJagged: {
          TH2JaggedF *h2 = static_cast<TH2JaggedF *>(NDTweaks[0][i]);
          fBinOffsets.push_back(fBinOffsets.back() + h2->GetNbinsNonFlow());
          // std::cout << "[INFO]: Added " << h2->GetName() << " with "
          //           << h2->GetNbinsNonFlow() << " for a total of "
          //           << fBinOffsets.back() << " bins" << std::endl;
          break;
        }
        }
      } else if (i < kFD_numu_numode) {
        switch (fNDSpecHCRunIs2D) {
        case kOneD: {
          fBinOffsets.push_back(fBinOffsets.back() +
                                NDSpecHCRunTweaks[0][i]->GetNbinsX());
          // std::cout << "[INFO]: Added " << NDSpecHCRunTweaks[0][i]->GetName()
          // << " with "
          //           << NDSpecHCRunTweaks[0][i]->GetNbinsX() << " for a total
          //           of "
          //           << fBinOffsets.back() << " bins" << std::endl;

          break;
        }
        case kTwoD: {
          TH2 *h2 = static_cast<TH2 *>(NDSpecHCRunTweaks[0][i]);
          fBinOffsets.push_back(fBinOffsets.back() +
                                (h2->GetNbinsX() * h2->GetNbinsY()));
          // std::cout << "[INFO]: Added " << h2->GetName() << " with "
          //           << h2->GetNbinsX() << " for a total of "
          //           << (h2->GetNbinsX() * h2->GetNbinsY()) << " bins"
          //           << std::endl;
          break;
        }
        case kTwoDJagged: {
          TH2JaggedF *h2 = static_cast<TH2JaggedF *>(NDSpecHCRunTweaks[0][i]);
          fBinOffsets.push_back(fBinOffsets.back() + h2->GetNbinsNonFlow());
          // std::cout << "[INFO]: Added " << h2->GetName() << " with "
          //           << h2->GetNbinsNonFlow() << " for a total of "
          //           << fBinOffsets.back() << " bins" << std::endl;
          break;
        }
        }
      } else {
        fBinOffsets.push_back(fBinOffsets.back() + FDTweaks[0][i]->GetNbinsX());
        // std::cout << "[INFO]: Added " << FDTweaks[0][i]->GetName() << " with
        // "
        //           << FDTweaks[0][i]->GetNbinsX() << " for a total of "
        //           << fBinOffsets.back() << " bins" << std::endl;
      }
    }
  }

  if (nu_config < kND_SpecHCRun_numu_numode) {
    switch (fNDIs2D) {
    case kOneD: {
      if (IsFlowBin(NDTweaks[0][nu_config], enu_GeV)) {
        return -1;
      }
      return fBinOffsets[nu_config] +
             (NDTweaks[0][nu_config]->GetXaxis()->FindFixBin(enu_GeV) - 1);
    }
    case kTwoD: {
      TH2 *h2 = static_cast<TH2 *>(NDTweaks[0][nu_config]);
      if (IsFlowBin(h2, enu_GeV, off_axis_pos_m)) {
        return -1;
      }
      int xbin = (h2->GetXaxis()->FindFixBin(enu_GeV) - 1);
      int ybin = (h2->GetYaxis()->FindFixBin(off_axis_pos_m) - 1);
      return fBinOffsets[nu_config] + (ybin * h2->GetNbinsY()) + xbin;
    }
    case kTwoDJagged: {
      TH2JaggedF *h2 = static_cast<TH2JaggedF *>(NDTweaks[0][nu_config]);
      if (IsFlowBin(h2, enu_GeV, off_axis_pos_m)) {
        return -1;
      }
      return fBinOffsets[nu_config] +
             h2->FindFixBinNoFlow(enu_GeV, off_axis_pos_m);
    }
    }
  } else if (nu_config < kFD_numu_numode) {
    switch (fNDSpecHCRunIs2D) {
    case kOneD: {
      if (IsFlowBin(NDSpecHCRunTweaks[0][nu_config], enu_GeV)) {
        return -1;
      }
      return fBinOffsets[nu_config] +
             (NDSpecHCRunTweaks[0][nu_config]->GetXaxis()->FindFixBin(enu_GeV) -
              1);
    }
    case kTwoD: {
      TH2 *h2 = static_cast<TH2 *>(NDSpecHCRunTweaks[0][nu_config]);
      if (IsFlowBin(h2, enu_GeV, off_axis_pos_m)) {
        return -1;
      }
      int xbin = (h2->GetXaxis()->FindFixBin(enu_GeV) - 1);
      int ybin = (h2->GetYaxis()->FindFixBin(off_axis_pos_m) - 1);
      return fBinOffsets[nu_config] + (ybin * h2->GetNbinsY()) + xbin;
    }
    case kTwoDJagged: {
      TH2JaggedF *h2 =
          static_cast<TH2JaggedF *>(NDSpecHCRunTweaks[0][nu_config]);
      if (IsFlowBin(h2, enu_GeV, off_axis_pos_m)) {
        return -1;
      }
      return fBinOffsets[nu_config] +
             h2->FindFixBinNoFlow(enu_GeV, off_axis_pos_m);
    }
    }
  } else {
    if (IsFlowBin(FDTweaks[0][nu_config], enu_GeV)) {
      return -1;
    }
    return fBinOffsets[nu_config] +
           (FDTweaks[0][nu_config]->GetXaxis()->FindFixBin(enu_GeV) - 1);
  }
  throw;
}

double OffAxisFluxUncertaintyHelper::GetFluxWeight(size_t param_id,
                                                     double param_val, int bin,
                                                     int nucf) const {
  if (nucf == kUnhandled) {
    return 1;
  }

  if (bin == kInvalidBin) {
    return 1;
  }

  if (nucf < kND_SpecHCRun_numu_numode) {
    return 1 + param_val * (NDTweaks[param_id][nucf]->GetBinContent(bin));
  } else if (nucf < kFD_numu_numode) {
    return 1 +
           param_val * (NDSpecHCRunTweaks[param_id][nucf]->GetBinContent(bin));
  } else {
    return 1 + param_val * (FDTweaks[param_id][nucf]->GetBinContent(bin));
  }
}

double OffAxisFluxUncertaintyHelper::GetFluxWeight(
    size_t param_id, double param_val, double enu_GeV, double off_axis_pos_m,
    int nu_pdg, bool IsND, bool IsNuMode, bool isSpecHCRun) const {
  return GetFluxWeight(param_id, param_val,
                       GetBin(nu_pdg, enu_GeV, off_axis_pos_m, param_id, IsND,
                              IsNuMode, isSpecHCRun),
                       GetNuConfig(nu_pdg, IsND, IsNuMode, isSpecHCRun));
}

OffAxisFluxUncertaintyHelper::~OffAxisFluxUncertaintyHelper() {
  for (size_t i = 0; i < FDTweaks.size(); ++i) {
    for (size_t j = 0; j < FDTweaks[i].size(); ++j) {
      delete FDTweaks[i][j];
    }
  }
  for (size_t i = 0; i < NDTweaks.size(); ++i) {
    for (size_t j = 0; j < NDTweaks[i].size(); ++j) {
      delete NDTweaks[i][j];
    }
  }
}
