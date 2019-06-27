#include "CAFAna/Analysis/AnalysisDialGroups.h"
#include "CAFAna/Analysis/XSecSystList.h"
#include "CAFAna/Analysis/common_fit_definitions.h"

#include <iostream>

using namespace ana;

void DumpXSecSystList() {
  /// All dial names that should go into a state file
  std::cout << "All XSec Systs: " << std::endl;
  for (auto n : GetAllXSecSystNames()) {
    std::cout << "\t" << n << std::endl;
  }
  std::cout << std::endl << std::endl;

  /// All dials used as freedoms in standard fits
  std::cout << "All Fit Systs: " << std::endl;
  for (auto n : GetFitSystNames()) {
    int indx = GetXSecSystIndex(n);
    std::cout << "\t" << n << ", {" << GetXSecSystMin(indx) << ", "
              << GetXSecSystMax(indx) << "}." << std::endl;
  }
  std::cout << std::endl << std::endl;

  /// Dials which should not be used as freedoms (CV weights if they exist
  /// removed in SpectrumLoader)
  std::cout << "All Non-fit Systs: " << std::endl;
  for (auto n : GetDoNotIncludeSystNames()) {
    std::cout << "\t" << n << std::endl;
  }
  std::cout << std::endl << std::endl;

  /// Dials which have an extrapolated response outside of 0->1
  std::cout << "All extrapolated off -> on Systs: " << std::endl;
  for (auto n : GetExtrapolateOffToOnSystNames()) {
    int indx = GetXSecSystIndex(n);
    std::cout << "\t" << n << ", {" << GetXSecSystMin(indx) << ", "
              << GetXSecSystMax(indx) << "}." << std::endl;
  }
  std::cout << std::endl << std::endl;

  /// Dials used to generate fake data sets when set to 1, should not be fit.
  std::cout << "All Fake data generation Systs: " << std::endl;
  for (auto n : GetFakeDataGenerationSystNames()) {
    std::cout << "\t" << n << std::endl;
  }
  std::cout << std::endl << std::endl;

  std::cout << "All: " << std::endl;
  for (auto s : GetListOfSysts("allsyst")) {
    std::cout << "\t" << s->ShortName() << std::endl;
  }
  std::cout << std::endl << std::endl;
  std::cout << "Flux: " << std::endl;
  for (auto s : GetListOfSysts("flux")) {
    std::cout << "\t" << s->ShortName() << std::endl;
  }
  std::cout << std::endl << std::endl;

  std::cout << "XSec: " << std::endl;
  for (auto s : GetListOfSysts("nodet:noxsec")) {
    std::cout << "\t" << s->ShortName() << std::endl;
  }
  std::cout << std::endl << std::endl;

  std::cout << "Det: " << std::endl;
  for (auto s : GetListOfSysts("noflux:noxsec")) {
    std::cout << "\t" << s->ShortName() << std::endl;
  }
  std::cout << std::endl << std::endl;

  auto los = GetListOfSysts("allsyst");

  for (auto const &str_p :
       std::vector<std::pair<std::string, std::vector<std::string>>>{

           {"fd_det_list", fd_det_list},
           {"fd_escale_list", fd_escale_list},
           {"fd_muon_escale_list", fd_muon_escale_list},
           {"fd_eres_list", fd_eres_list},
           {"fd_muon_eres_list", fd_muon_eres_list},
           {"fd_other_det_list", fd_other_det_list},
           {"fd_nonlin_det_list", fd_nonlin_det_list},
           {"xsec_qe_list", xsec_qe_list},
           {"xsec_res_list", xsec_res_list},
           {"xsec_dis_list", xsec_dis_list},
           {"xsec_fsi_list", xsec_fsi_list},
           {"xsec_ratios_list", xsec_ratios_list},

       }) {
    std::cout << "For helper list: " << str_p.first << " expect to remove "
              << str_p.second.size() << " systs." << std::endl;
    auto los_copy = los;
    RemoveSysts(los_copy, str_p.second);
    std::cout << " Removed " << (los.size() - los_copy.size()) << std::endl;
    if ((los_copy.size() + str_p.second.size()) != los.size()) {
      auto str_p_copy = str_p.second;
      for (auto s : str_p_copy) {
        bool found = false;
        for (auto syst : los) {
          if (syst->ShortName() == s) {
            found = true;
            break;
          }
        }
        if (found) {
          continue;
        }
        std::cout << "Failed to find " << s << std::endl;
      }
    }
    assert((los_copy.size() + str_p.second.size()) == los.size());
  }
}
