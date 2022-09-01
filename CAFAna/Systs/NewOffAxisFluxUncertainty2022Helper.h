#pragma once
#include "TH1.h"
#include "TFile.h"

#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

namespace ana {

  class NewOffAxisFluxUncertainty2022Helper {
  public:
    ~NewOffAxisFluxUncertainty2022Helper() {
      std::cout << "Calling destructor." << std::endl;
    }

    static int const kND_numu_numode = 0;
    static int const kND_nue_numode = 1;
    static int const kND_numubar_numode = 2;
    static int const kND_nuebar_numode = 3;

    static int const kND_SpecHCRun_numu_numode = 4; //4; nonsense values until we have nubar and SpecRun
    static int const kND_SpecHCRun_nue_numode = 5; //5;
    static int const kND_SpecHCRun_numubar_numode = 6;//6;
    static int const kND_SpecHCRun_nuebar_numode = 7;//7;

    static int const kND_numu_nubarmode = 28;//8;
    static int const kND_nue_nubarmode = 29;//9;
    static int const kND_numubar_nubarmode = 30;//10;
    static int const kND_nuebar_nubarmode = 31;//11;

    static int const kND_SpecHCRun_numu_nubarmode = 32;//12;
    static int const kND_SpecHCRun_nue_nubarmode = 33;//13;
    static int const kND_SpecHCRun_numubar_nubarmode = 34;//14;
    static int const kND_SpecHCRun_nuebar_nubarmode = 35;//15;

    static int const kFD_numu_numode = 8; //16;
    static int const kFD_nue_numode = 9; //17;
    static int const kFD_numubar_numode = 10; //18;
    static int const kFD_nuebar_numode = 11; //19;

    static int const kFD_numu_nubarmode = 20;
    static int const kFD_nue_nubarmode = 21;
    static int const kFD_numubar_nubarmode = 22;
    static int const kFD_nuebar_nubarmode = 23;

    static int const kUnhandled = 8; //24;

    static int const kInvalidBin = std::numeric_limits<int>::max();

    void Initialize(std::string const &filename);

    static NewOffAxisFluxUncertainty2022Helper const &Get();

    int GetNuConfig(int nu_pdg, bool IsND,
                    bool IsNuMode,
                    bool isSpecHCRun) const;

    int GetNuConfig_checked(int nu_pdg, double enu_GeV, 
                            double off_axis_pos_m, size_t param_id,
                            bool IsND, bool IsNuMode, bool isSpecHCRun) const;

    std::vector<std::unique_ptr<TH1D>> GetNDOffAxisShifts(TFile *f, 
                                                          std::string nd_dir, 
                                                          std::string hname) const;

    int GetBin(int nu_pdg, double enu_GeV, double off_axis_pos_m,
               size_t param_id = 0, bool IsND = true, 
               bool IsNuMode = true, bool isSpecHCRun = false) const;

    double GetFluxWeight(size_t param_id, double param_val, double off_axis_pos_m, 
                         int bin, int nu_config) const;

    double GetFluxWeight(size_t param_id, double param_val, double enu_GeV,
                         double off_axis_pos_m, int nu_pdg, bool IsND,
                         bool IsNuMode, bool isSpecHCRun = false) const;

  protected:
    std::vector<std::unique_ptr<TAxis>> OffAxisTAxes;
    // param | nucf | OA bin | E bin
    std::vector<std::vector<std::vector<std::unique_ptr<TH1D>>>> NDuncerts;
    // param | nucf | E bin
    std::vector<std::vector<std::unique_ptr<TH1D>>> FDuncerts;
    //std::map<int, std::vector<std::vector<std::unique_ptr<TH1D>>>> NDuncerts;
    //std::map<int, std::vector<std::unique_ptr<TH1D>>> FDuncerts;

  };
} // namespace ana
