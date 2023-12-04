#include "CAFAna/Systs/NewOffAxisFluxUncertainty2022Helper.h"

#include "CAFAna/Core/Utilities.h"

#include "TAxis.h"
#include "TFile.h"
#include "TH1.h"
#include "TKey.h"
#include "TObjString.h"

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

using namespace ana;

static NewOffAxisFluxUncertainty2022Helper *globalFluxHelper = nullptr;
NewOffAxisFluxUncertainty2022Helper const &NewOffAxisFluxUncertainty2022Helper::Get() {
  if (!globalFluxHelper) {
    globalFluxHelper = new NewOffAxisFluxUncertainty2022Helper();
    globalFluxHelper->Initialize(ana::FindCAFAnaDir() +
                                 "/Systs/flux_shifts_OffAxis2022.root");
  }
  return *globalFluxHelper;
}


void NewOffAxisFluxUncertainty2022Helper::Initialize(std::string const &filename) {

  NDuncerts.clear();
  FDuncerts.clear();

  size_t NParams = 50;
  std::string ND_detector_tag = "ND";
  std::string ND_SpecHCRun_detector_tag = "_specrun_";
  std::string FD_detector_tag = "FD";
  std::string nu_mode_beam_tag = "nu";
  std::string nubar_mode_beam_tag = "nubar";
  std::string numu_species_tag = "numu";
  std::string nue_species_tag = "nue";
  std::string numubar_species_tag = "numubar";
  std::string nuebar_species_tag = "nuebar";

  static std::string const location_tags[] = {
        ND_detector_tag, FD_detector_tag};
  static std::string const beam_mode_tags[] = {
        nu_mode_beam_tag, nubar_mode_beam_tag}; 
  static std::string const species_tags[] = {
      numu_species_tag, nue_species_tag, numubar_species_tag,
      nuebar_species_tag};
  static std::string const spec_run_tags[] = {
      "_", ND_SpecHCRun_detector_tag};

  std::string input_dir = "FluxParameters";

  TFile *inpF = new TFile(filename.c_str());
  if (!inpF || !inpF->IsOpen()) {
    std::cout << "[ERROR]: Couldn't open input file: " << filename << std::endl;
    exit(1);
  }

  TDirectory *d = inpF->GetDirectory(input_dir.c_str());
  TIter nextkey(d->GetListOfKeys());
  TKey *key;

  int param_id(0);

  while ((key = (TKey*)nextkey())) {
    std::string param_name(key->GetName());
    int nucf = kND_numu_numode; // = 0

    std::string syst = param_name.substr(param_name.find("_") + 1, std::string::npos);
    //std::cout << "Syst name = " << syst << std::endl;    

    std::stringstream input_dir_i("");
    input_dir_i << input_dir << (input_dir.size() ? "/" : "")
                << param_name << "/";
    TDirectory *param_d = inpF->GetDirectory(input_dir_i.str().c_str());
    //inpF->cd(input_dir_i.c_str());

    std::unique_ptr<TAxis> oa_axis =
      std::unique_ptr<TAxis>((TAxis*)param_d->Get("OffAxisTAxis"));
    OffAxisTAxes.emplace_back(std::move(oa_axis));

    NDuncerts.push_back(std::vector<std::vector<std::unique_ptr<TH1D>>>());
    FDuncerts.push_back(std::vector<std::unique_ptr<TH1D>>());

    std::vector<std::vector<std::unique_ptr<TH1D>>> param_NDuncerts;
    std::vector<std::unique_ptr<TH1D>> param_FDuncerts;

    for (size_t lt_it = 0; lt_it < 2; ++lt_it) {
      std::string const &location_tag = location_tags[lt_it];
      for (size_t bm_it = 0; bm_it < 2; ++bm_it) {
        std::string const &beam_mode_tag = beam_mode_tags[bm_it];
        for (size_t sr_it = 0; sr_it < 2; ++sr_it) {
          std::string const &spec_run_tag = spec_run_tags[sr_it];
          for (size_t sp_it = 0; sp_it < 4; ++sp_it) {
            std::string const &species_tag = species_tags[sp_it];

            std::string hname = location_tag + "_" +
                                beam_mode_tag + spec_run_tag + species_tag;

            NDuncerts.back().emplace_back(std::vector<std::unique_ptr<TH1D>>());
            FDuncerts.back().emplace_back(nullptr);
    
            if (nucf < kFD_numu_numode) { // Is ND (Any horn current for now)
              // ND dir name is the same as the hist name
              std::string nd_dir = input_dir_i.str() + hname;
              std::vector<std::unique_ptr<TH1D>> AllOffAxisShifts = 
                GetNDOffAxisShifts(inpF, nd_dir, hname);
              NDuncerts.back().at(nucf) = std::move(AllOffAxisShifts);
              nucf += 1;
            } else if (spec_run_tag == "_") { // Is FD and not 280kA run
              std::unique_ptr<TH1D> h_in = 
                std::unique_ptr<TH1D>((TH1D*)param_d->Get(hname.c_str()));
              if (!h_in) {
                std::cout << "[WARN] Cannot find " << hname << std::endl;
                continue;
              }
                            
              h_in->SetDirectory(nullptr);
              //std::cout << hname << "; nucf = " << nucf << std::endl;       
              //std::cout << "FD uncert nbins: " << 
              //  h_in->GetXaxis()->GetNbins() << std::endl; 
              //std::cout << "FD uncert integral = " << h_in->Integral() << std::endl;     
              //param_FDuncerts.emplace_back(std::move(h_in));
              //FDuncerts.back().back() = std::move(h_in);
              FDuncerts.back().at(nucf) = std::move(h_in);
              nucf += 1;
            }
          }
        }
      }
    }
    param_id += 1;
  }
}

int NewOffAxisFluxUncertainty2022Helper::GetNuConfig(int nu_pdg, bool IsND,
                                                     bool IsNuMode,
                                                     bool isSpecHCRun) const {

  int nucf = kUnhandled;

  switch (nu_pdg) {
  case 14: {
    if (IsND) {
      nucf = IsNuMode ? (isSpecHCRun ? kND_SpecHCRun_numu_numode
                                     : kND_numu_numode)
                      : (isSpecHCRun ? kND_SpecHCRun_numu_nubarmode
                                     : kND_numu_nubarmode);
    } else {
      nucf =  IsNuMode ? kFD_numu_numode : kFD_numu_nubarmode;
    }
    break;
  }
  case -14: {
    if (IsND) {
      nucf = IsNuMode ? (isSpecHCRun ? kND_SpecHCRun_numubar_numode
                                     : kND_numubar_numode)
                      : (isSpecHCRun ? kND_SpecHCRun_numubar_nubarmode
                                     : kND_numubar_nubarmode);
    } else {
      nucf = IsNuMode ? kFD_numubar_numode : kFD_numubar_nubarmode;
    }
    break;
  }
  case 12: {
    if (IsND) {
      nucf = IsNuMode ? (isSpecHCRun ? kND_SpecHCRun_nue_numode : kND_nue_numode)
                      : (isSpecHCRun ? kND_SpecHCRun_nue_nubarmode
                                     : kND_nue_nubarmode);
    } else {
      nucf = IsNuMode ? kFD_nue_numode : kFD_nue_nubarmode;
    }
    break;
  }
  case -12: {
    if (IsND) {
      nucf = IsNuMode
                 ? (isSpecHCRun ? kND_SpecHCRun_nuebar_numode : kND_nuebar_numode)
                 : (isSpecHCRun ? kND_SpecHCRun_nuebar_nubarmode
                                : kND_nuebar_nubarmode);
    } else {
      nucf = IsNuMode ? kFD_nuebar_numode : kFD_nuebar_nubarmode;
    }
    break;
  }
  }

  return nucf;
}


int NewOffAxisFluxUncertainty2022Helper::GetNuConfig_checked(
    int nu_pdg, double enu_GeV, double off_axis_pos_m, size_t param_id,
    bool IsND, bool IsNuMode, bool isSpecHCRun) const {
  if (GetBin(nu_pdg, enu_GeV, off_axis_pos_m, param_id, IsND, IsNuMode,
             isSpecHCRun) == kInvalidBin) {
    return kUnhandled;
  }
  return GetNuConfig(nu_pdg, IsND, IsNuMode, isSpecHCRun);
}


std::vector<std::unique_ptr<TH1D>> 
  NewOffAxisFluxUncertainty2022Helper::GetNDOffAxisShifts(TFile *f,
                                                          std::string nd_dir,
                                                          std::string hname) const {
    std::vector<std::unique_ptr<TH1D>> OffAxisUncerts;
    TDirectory *d = f->GetDirectory(nd_dir.c_str());  
    //std::cout << "Shift dir: " << nd_dir << std::endl; 
    TIter nextkey(d->GetListOfKeys());
    int n_offaxis = nextkey.GetCollection()->GetSize();

    for (int oa = 0; oa < n_offaxis; oa++) {
      std::string hname_oa = hname + "_" + std::to_string(oa);
      std::unique_ptr<TH1D> h_oa = 
        std::unique_ptr<TH1D>((TH1D*)d->Get(hname_oa.c_str()));
      if (!h_oa) {
        std::cout << "[WARNING} " << hname_oa << " not found." << std::endl;
        continue;
      }
      h_oa->SetDirectory(nullptr);
      OffAxisUncerts.emplace_back(std::move(h_oa));
    }
 
    return OffAxisUncerts;
}

int NewOffAxisFluxUncertainty2022Helper::GetBin(int nu_pdg, double enu_GeV, 
                                                double off_axis_pos_m,
                                                size_t param_id, bool IsND,
                                                bool IsNuMode, bool isSpecHCRun) const {
  // Get the correct energy bin for specific channel and off-axis position.

  int nucf = GetNuConfig(nu_pdg, IsND, IsNuMode, isSpecHCRun); 

  int bin_oa(1);
  int bin_e(1); 
  //std::cout << "param = " << param_id << ", nucf = " << nucf << ", OA m = " << off_axis_pos_m << 
  //    ", enu_GeV = " << enu_GeV << std::endl;
  //std::cout << "NDuncerts param size = " << NDuncerts.size() << ", nucf size = " << NDuncerts.at(param_id).size() <<
  //  ", OA bin size = " << NDuncerts.at(param_id).at(nucf).size() << std::endl;
  if (IsND) {
    // Sign flip in off axis position
    bin_oa = OffAxisTAxes.at(param_id)->FindFixBin(off_axis_pos_m);
    bin_e = NDuncerts.at(param_id).at(nucf).at(bin_oa - 1)->FindFixBin(enu_GeV);
  } else {
    if (FDuncerts.at(param_id).size()<1) {
      std::cout << "[WARN] no param_id" << std::endl;
    }
    if (!FDuncerts.at(param_id).at(nucf)) {
      std::cout << "[WARN] no nucf = " << nucf << std::endl;
    }
    //std::cout << "FDuncerts.at(param_id) size: " << 
    //  FDuncerts.at(param_id).size() << std::endl;    
    //std::cout << "HERE"<< std::endl;
    bin_e = FDuncerts.at(param_id).at(nucf)->FindFixBin(enu_GeV);
    //std::cout << "FOUND E BIN: " << bin_e << std::endl;
  } 
  return bin_e;
}     

double NewOffAxisFluxUncertainty2022Helper::GetFluxWeight(size_t param_id,
                                                          double param_val, 
                                                          double off_axis_pos_m, 
                                                          int bin,
                                                          int nucf) const {

  if (nucf == kUnhandled) {
    return 1;
  }

  if (bin == kInvalidBin) {
    return 1;
  }

  if (nucf < kFD_numu_numode) {
    // Sign flip in off axis position
    int bin_oa = OffAxisTAxes.at(param_id)->FindFixBin(off_axis_pos_m);
    return 1 + 
           param_val * 
           NDuncerts.at(param_id).at(nucf).at(bin_oa - 1)->GetBinContent(bin);
  } else {
    //std::cout << "param_id: " << param_id << ", param_val = " << param_val << ", bin = " << bin << 
    //  ", bincont = " << FDuncerts.at(param_id - 1).at(nucf)->GetBinContent(bin) << std::endl;
    //std::cout << "NBins = " << FDuncerts.at(param_id - 1).at(nucf)->GetXaxis()->GetNbins() << std::endl;
    //std::cout << "Bin Integral = " << FDuncerts.at(param_id - 1).at(nucf)->Integral() << std::endl;
    //std::cout << "Weight = " << 
    //  1 + param_val * FDuncerts.at(param_id - 1).at(nucf)->GetBinContent(bin) << std::endl;
    return 1 + param_val * FDuncerts.at(param_id).at(nucf)->GetBinContent(bin);
  }
}

double NewOffAxisFluxUncertainty2022Helper::GetFluxWeight(
  size_t param_id, double param_val, double enu_GeV, double off_axis_pos_m, 
  int nu_pdg, bool IsND, bool IsNuMode, bool isSpecHCRun) const {

  return GetFluxWeight(param_id, param_val, off_axis_pos_m, 
                       GetBin(nu_pdg, enu_GeV, off_axis_pos_m, param_id, IsND,
                              IsNuMode, isSpecHCRun),
                       GetNuConfig(nu_pdg, IsND, IsNuMode, isSpecHCRun));
}
