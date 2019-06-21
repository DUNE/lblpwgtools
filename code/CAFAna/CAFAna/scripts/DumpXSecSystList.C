#include "CAFAna/Analysis/XSecSystList.h"

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
}
